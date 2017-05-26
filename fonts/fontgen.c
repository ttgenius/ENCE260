/** @file   fontgen.c
    @author M. P. Hayes, UCECE
    @date   15 May 2007
    @brief 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>

/* FIXME.  This is not robust.  It's an ugly hack.  Rewrite in python!  */


typedef struct
{
    char name[64];
    char *comment;
    uint8_t width;
    uint8_t height;
    uint8_t offset;
    uint8_t size;
    uint8_t bytes;
    uint8_t interlaced;
    uint8_t *data;
} font_t;


#define FONT_SIZE_MAX 255

int font_zoom (font_t *font, font_t *zoom_font, int zoom_x, int zoom_y)
{
    int i;
    uint8_t *image;
    uint8_t *zoom_image;
    int bits;

    *zoom_font = *font;
    zoom_font->width = font->width * zoom_x;
    zoom_font->height = font->height * zoom_y;
    zoom_font->bytes = font->bytes * zoom_x * zoom_y;
    zoom_font->data = calloc (FONT_SIZE_MAX, zoom_font->bytes);

    bits = zoom_font->width * zoom_font->height;
    if (zoom_font->interlaced)
        bits = (bits + 1) / 2;
    zoom_font->bytes = (bits + CHAR_BIT - 1) / CHAR_BIT;

    image = malloc (font->width * font->height);
    zoom_image = malloc (font->width * zoom_x * font->height * zoom_y);

    for (i = font->offset; i < font->size + font->offset; i++)
    {
        int k;
        int x;
        int y;
        uint8_t font_byte;
        uint8_t *font_elt;

        font_elt = &font->data[i * font->bytes];

        k = 0;
        font_byte = *font_elt++;
        
        for (y = 0; y < font->height; y++)
        {
            for (x = 0; x < font->width; x++)
            {
                image[y * font->width + x] = (font_byte & 1) * 255;
                font_byte >>= 1;
                k++;
                if (k >= CHAR_BIT)
                {
                    k = 0;
                    font_byte = *font_elt++;
                }
            }
        }
        
        for (y = 0; y < zoom_font->height; y++)
        {
            for (x = 0; x < zoom_font->width; x++)        
                zoom_image[y * zoom_font->width + x]
                    = image[(y / zoom_y) * font->width + (x / zoom_x)];
        }


        font_elt = &zoom_font->data[i * zoom_font->bytes];
        k = 0;
        *font_elt = 0;
        for (y = 0; y < zoom_font->height; y++)
        {
            for (x = 0; x < zoom_font->width; x++)        
            {
                if (zoom_image[y * zoom_font->width + x])
                    *font_elt |= (1 << k);
                k++;
                if (k >= CHAR_BIT)
                {
                    k = 0;
                    font_elt++;
                    *font_elt = 0;
                }
            }
        }
    }

    free (image);
    free (zoom_image);
    return 1;
}


int font_scan (font_t *font)
{
    char buffer[132];
    int line;
    int symcnt;
    int symline;
    uint8_t symbol;
    uint8_t symbol_min;
    uint8_t symbol_max;
    int tmp;
    int bits;
    unsigned int i;

    font->comment = 0;
    font->interlaced = 0;

    /* FIXME, use more robust key-value pair parsing.  */
    while (1)
    {
        int c;

        c = fgetc (stdin);
        if (c == EOF)
            return 0;

        ungetc (c, stdin);

        /* Ignore lines starting with #, w, h, i.  */
        if (!strchr ("#whi", c))
            break;

        if (!fgets (buffer, sizeof (buffer), stdin))
            return 0;

        if (c == 'w')
        {
            if (sscanf (buffer, "width=%d", &tmp) != 1)
                fprintf (stderr, "Width parse error: %s\n", buffer);
            font->width = tmp;
        }
        else if (c == 'h')
        {
            if (sscanf (buffer, "height=%d", &tmp) != 1)
                fprintf (stderr, "Height parse error: %s\n", buffer);
            font->height = tmp;
        }
        else if (c == 'i')
        {
            if (sscanf (buffer, "interlaced=%d", &tmp) != 1)
                fprintf (stderr, "Interlaced parse error: %s\n", buffer);
            font->interlaced = tmp;
        }
        else if (c == '#')
        {
            if (!font->comment)
                font->comment = calloc (1, strlen (buffer) + 1);
            else
                font->comment = realloc (font->comment,
                                         strlen (font->comment)
                                         + strlen (buffer) + 1);

            strcat (font->comment, buffer + 1);
        }
        
    }

    if (!font->width)
    {
        fprintf (stderr, "Missing width specifier\n");
        return 1;
    }

    if (!font->height)
    {
        fprintf (stderr, "Missing height specifier\n");
        return 1;
    }

    bits = font->width * font->height;
    if (font->interlaced)
        bits = (bits + 1) / 2;
    font->bytes = (bits + CHAR_BIT - 1) / CHAR_BIT;

    font->offset = 0;
    font->size = ~0;

    symbol_min = ~0;
    symbol_max = 0;

    font->data = calloc (FONT_SIZE_MAX, font->bytes);

    symbol = 0;
    
    line = 4;
    for (symcnt = 1; ; symcnt++)
    {
        int bit = 0;

        for (symline = 1; symline <= font->height + 1; symline++)
        {
            unsigned int num;
            
            if (!fgets (buffer, sizeof (buffer), stdin))
            {
                /* Have reached end of file.  */
                font->offset = symbol_min;
                font->size = symbol_max - symbol_min + 1;

                return 0;
            }

            /* Remove newline.  */
            buffer[strlen (buffer) - 1] = '\0';
            
            if (symline != 1)
            {
                char *p;

                if (strlen (buffer) != font->width)
                {
                    fprintf (stderr, "Parse error at line %d: %s\n", line, buffer);
                    return 1;
                }
            
                p = buffer;
                if (font->interlaced)
                {
                    num = symline & 1 ? font->width / 2 : (font->width + 1) / 2;
                    if (symline & 1)
                        p++;
                }
                else
                    num = font->width;
                
                for (i = 0; i < num; i++)
                {
                    if (*p != '.')
                    {
                        font->data[symbol * font->bytes + bit / CHAR_BIT] 
                            |= (1 << (bit % CHAR_BIT));
                    }
                    else if (*p != '.')
                    {
                        fprintf (stderr, "Parse error at line %d: %c\n", line, *p);
                        return 1;
                    }
                    bit++;
                    if (font->interlaced)
                        p += 2;
                    else
                        p++;
                }
            }
            else
            {
                if (buffer[0] == '\\' && buffer[1] != '\0')
                {
                    if (sscanf (buffer + 1, "%o", &tmp) != 1)
                    {
                        fprintf (stderr, "Parse error (octal num) at line %d: %s\n", line, buffer);                 
                        return 2;
                    }
                    symbol = tmp;
                }
                else
                {
                    char sym;

                    if (sscanf (buffer, "%c", &sym) != 1)
                    {
                        fprintf (stderr, "Parse error (char) at line %d: %s\n", line, buffer);              
                        return 2;
                    }
                    symbol = sym;
                }
                if (symbol < symbol_min)
                    symbol_min = symbol;
                if (symbol > symbol_max)
                    symbol_max = symbol;
            }

            line++;
        }
    }
}


static void font_print_1 (font_t *font, uint8_t symbol)
{
    int i;

    for (i = 0; i < font->bytes; i++)
        printf ("0x%02x, ", font->data[symbol * font->bytes + i]);
    fputs ("\n", stdout);
}


static void font_rotate (font_t *font, font_t *rotate_font)
{
    int i;
    int x;
    int y;

    *rotate_font = *font;
    rotate_font->width = font->height;
    rotate_font->height = font->width;
    rotate_font->data = calloc (FONT_SIZE_MAX, font->bytes);

    for (i = 0; i < font->size; i++)    
    {
        for (y = 0; y < font->height; y++)
        {
            for (x = 0; x < font->width; x++)
            {
                int bit;
                int newbit;

                bit = y * font->width + x;
                newbit = x * font->height + y;

                if (font->data[i * font->bytes + bit / CHAR_BIT] 
                    & (1 << (bit % CHAR_BIT)))
                    rotate_font->data[i * font->bytes + newbit / CHAR_BIT] 
                    |= (1 << (newbit % CHAR_BIT));
            }
        }
    }
}


static void font_print (font_t *font)
{
    int i;
    uint8_t bytes_per_char;
    time_t now;
    char name_upper[128];
    char timestr[32];

    bytes_per_char = (font->width * font->height + CHAR_BIT - 1) / CHAR_BIT;

    for (i= 0; font->name[i]; i++)
        name_upper[i] = toupper (font->name[i]);
    name_upper[i] = '\0';

    time (&now);
    strftime (timestr, sizeof (timestr), "%d %b %Y", localtime (&now));
    
    printf ("/** @file    %s.h\n", font->name);
    printf ("    @author  fontgen\n");
    printf ("    @date    %s\n\n", timestr);
    printf ("    @defgroup %s %s\n", font->name, font->comment);
    printf ("*/\n\n");
    printf ("#ifndef %s_H_\n", name_upper);
    printf ("#define %s_H_\n\n", name_upper);
    printf ("#define %s_WIDTH %d\n", name_upper, font->width);
    printf ("#define %s_HEIGHT %d\n", name_upper, font->height);
    printf ("#define %s_OFFSET %d\n", name_upper, font->offset);
    printf ("#define %s_SIZE %d\n\n", name_upper, font->size);

    printf ("#ifndef FONT_WIDTH\n#define FONT_WIDTH %d\n#endif\n",
            font->width);
    printf ("#ifndef FONT_HEIGHT\n#define FONT_HEIGHT %d\n#endif\n",
            font->height);
    printf ("#ifndef FONT_OFFSET\n#define FONT_OFFSET %d\n#endif\n",
            font->offset);
    printf ("#ifndef FONT_SIZE_MAX\n#define FONT_SIZE_MAX %d\n#endif\n",
            font->size);

    printf ("#include \"font.h\"\n\n");
    printf ("static font_t %s =\n{\n", font->name);    

    printf ("    .flags = 1, /* (packed)  */\n");
    printf ("    .width = %d,\n", font->width);
    printf ("    .height = %d,\n", font->height);
    printf ("    .offset = %d,\n", font->offset);
    printf ("    .size = %d,\n", font->size);
    printf ("    .bytes = %d,\n", bytes_per_char);
    printf ("    .data =\n    {\n");

    for (i = font->offset; i < font->size + font->offset; i++)
    {
        if (i < 32)
            printf ("        /* \\%o  */\n        ", i);
        else
            printf ("        /* %c  */\n        ", i);

        font_print_1 (font, i);
    }
    printf ("    }\n};\n#endif  /* %s_H_  */\n", name_upper);
}


static void font_draw_1 (font_t *font, uint8_t symbol)
{
    int i;
    int j;
    int bit;
    char ch;
    uint8_t *data;

    data = &font->data[symbol * font->bytes];
    bit = 0;
    ch = *data++;
    for (i = 0; i < font->height; i++)
    {
        for (j = 0; j < font->width; j++)
        {
            if (font->interlaced && ((i & 1) ^ (j & 1)))
            {
                fputc (' ', stdout);
                continue;
            }
            fputc (ch & 1 ? '@' : '.', stdout);
            ch >>= 1;
            bit++;
            if ((bit % CHAR_BIT) == 0)
                ch = *data++;
        }
        fputc ('\n', stdout);
    }
}


static void font_draw (font_t *font)
{
    int i;

    printf ("width=%d\n", font->width);
    printf ("height=%d\n", font->height);
    printf ("interlaced=%d\n", font->interlaced);

    for (i = 0; i < 32; i++)
    {
        printf ("\\%o\n", i);
        font_draw_1 (font, i);
    }

    for (; i < FONT_SIZE_MAX; i++)
    {
        printf ("%c\n", i);
        font_draw_1 (font, i);
    }
}


int main (int argc, char **argv)
{
    int ret;
    font_t font;

    ret = font_scan (&font);
    if (ret)
        return ret;

    if (argc < 2)
    {
        fprintf (stderr, "Usage: fontgen fontname\n");
        return 1;
    }
    strcpy (font.name, argv[1]);
    argc--;
    argv++;

    if (argc > 2 && !strcmp (argv[1], "--zoom"))
    {
        int zoom;
        font_t zoom_font;

        zoom = atoi (argv[2]);
        argc -= 2;
        argv += 2;
        
        font_zoom (&font, &zoom_font, zoom, zoom);
        font = zoom_font;
    }

    if (argc > 1 && !strcmp (argv[1], "--rotate"))
    {
        font_t rotate_font;

        argc -= 1;
        argv += 1;
        
        font_rotate (&font, &rotate_font);
        font = rotate_font;
    }

    if (argc > 1 && !strcmp (argv[1], "--ascii"))
        font_draw (&font);
    else
        font_print (&font);
    return 0;
}

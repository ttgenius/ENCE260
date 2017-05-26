/** @file   tinygl.c
    @author M. P. Hayes, UCECE
    @date   24 August 2010
    @brief  Tiny graphics library.
*/
#include <string.h>
#include "system.h"
#include "tinygl.h"
#include "display.h"
#include "font.h"


enum {TINYGL_SPEED_DEFAULT = 20};

typedef struct tinygl_state_struct
{
    tinygl_text_mode_t mode;
    tinygl_text_dir_t dir;
    tinygl_point_t pos;
    uint16_t speed;
    font_t *font;
    uint16_t update_rate;
    uint8_t message_index;
    uint8_t message_repeats;
    uint16_t text_advance_period;
    int8_t scroll_pos;
    char message[TINYGL_MESSAGE_SIZE];
} tinygl_state_t;


static tinygl_state_t tinygl =
{
    .mode = TINYGL_TEXT_MODE_STEP,
    .dir = TINYGL_TEXT_DIR_NORMAL,
    .speed = TINYGL_SPEED_DEFAULT
};


/** Set pixel.
    @param pos coordinates of point
    @param pixel_value pixel value.  */
void tinygl_pixel_set (tinygl_point_t pos, tinygl_pixel_value_t pixel_value)
{
    display_pixel_set (pos.x, pos.y, pixel_value);
}


/** Get pixel.
    @param pos coordinates of point
    @return pixel value.  */
tinygl_pixel_value_t tinygl_pixel_get (tinygl_point_t pos)
{
    return display_pixel_get (pos.x, pos.y);
}


/** Draw point.
    @param pos coordinates of point
    @param pixel_value pixel value for line.  */
void tinygl_draw_point (tinygl_point_t pos, tinygl_pixel_value_t pixel_value)
{
    display_pixel_set (pos.x, pos.y, pixel_value);
}


/** Draw line.
    @param pos1 coordinates of start of line
    @param pos2 coordinates of end of line
    @param pixel_value pixel value for line.  */
void tinygl_draw_line (tinygl_point_t pos1, tinygl_point_t pos2,
                       tinygl_pixel_value_t pixel_value)
{
    int dx;
    int dy;
    int x_inc;
    int y_inc;

    /* Draw a line using Bresenham's algorithm.  */

    dx = pos2.x - pos1.x;
    dy = pos2.y - pos1.y;

    x_inc = dx >= 0 ? 1 : -1;
    y_inc = dy >= 0 ? 1 : -1;
    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;

    if (dx >= dy)
    {
        int error;

        error = dx / 2;

        for (; pos1.x != pos2.x; pos1.x += x_inc)
        {
            tinygl_draw_point (pos1, pixel_value);

            error -= dy;
            if (error < 0)
            {
                error += dx;
                pos1.y += y_inc;
            }
        }
    }
    else
    {
        int error;

        error = dy / 2;
        for (; pos1.y != pos2.y; pos1.y += y_inc)
        {
            tinygl_draw_point (pos1, pixel_value);

            error -= dx;
            if (error < 0)
            {
                error += dy;
                pos1.x += x_inc;
            }
        }
    }
    tinygl_draw_point (pos1, pixel_value);
}


/** Draw box.
    @param tl coordinates of top left corner of box
    @param br coordinates of bottom right of box
    @param pixel_value pixel value for box.  */
void tinygl_draw_box (tinygl_point_t tl, tinygl_point_t br,
                      tinygl_pixel_value_t pixel_value)
{
    tinygl_draw_line (tl, tinygl_point (br.x, tl.y), pixel_value);
    tinygl_draw_line (tl, tinygl_point (tl.x, br.y), pixel_value);
    tinygl_draw_line (tinygl_point (br.x, tl.y), br, pixel_value);
    tinygl_draw_line (tinygl_point (tl.x, br.y), br, pixel_value);
}


/** Clear display.  */
void tinygl_clear (void)
{
    display_clear ();

    /* Stop message display.  */
    tinygl.message[0] = 0;
    tinygl.message_index = 0;
}


/** Determine if pixel on or off.
    @param ch character to display
    @param col column of font element
    @param row row of font element
    @return 1 if pixel on; if pixel out of bounds return 0.  */
static bool tinygl_font_pixel_get (char ch, uint8_t col, uint8_t row)
{
    return font_pixel_get (tinygl.font, ch, col, row);
}


/** Draw character using current font.
    @param ch character to draw
    @param pos coordinates of top left position
    @return position to draw next character.  */
tinygl_point_t tinygl_draw_char (char ch, tinygl_point_t pos)
{
    uint8_t x;
    uint8_t y;

    if (tinygl.dir == TINYGL_TEXT_DIR_ROTATE)
    {
        for (y = 0; y < tinygl.font->width; y++)
        {
            for (x = 0; x < tinygl.font->height; x++)
            {
                tinygl_draw_point (tinygl_point (x + pos.x, pos.y),
                                   tinygl_font_pixel_get (ch, y, x));
            }
            pos.y--;
        }

        /* Draw blank row.  */
        for (x = 0; x < tinygl.font->height; x++)
            tinygl_draw_point (tinygl_point (x + pos.x, pos.y), 0);

        pos.y--;
    }
    else
    {
        for (x = 0; x < tinygl.font->width; x++)
        {
            for (y = 0; y < tinygl.font->height; y++)
            {
                tinygl_draw_point (tinygl_point (pos.x, y + pos.y),
                                   tinygl_font_pixel_get (ch, x, y));
            }
            pos.x++;
        }
        
        /* Draw blank column.  */
        for (y = 0; y < tinygl.font->height; y++)
            tinygl_draw_point (tinygl_point (pos.x, y + pos.y), 0);
        pos.x++;
    }
    return pos;
}


/** Draw string (well, as much as possible) using current font.
    @param str string to draw
    @param pos coordinates of top left position
    @return number of whole characters drawn.  */
uint8_t tinygl_draw_string (const char *str, tinygl_point_t pos)
{
    uint8_t count = 0;

    while (*str)
    {
        pos = tinygl_draw_char (*str, pos);
        count++;

        if (pos.x >= TINYGL_WIDTH || pos.y < 0)
            break;

        str++;
    }
    return count;
}


/** Advance message.  */
static void tinygl_text_advance (void)
{
    if (!tinygl.message[tinygl.message_index])
    {
        if (tinygl.message_repeats <= 1)
            return;
        tinygl.message_repeats--;
        tinygl.message_index = 0;
    }
    
    if (tinygl.message[tinygl.message_index])
    {
        tinygl_point_t pos;

        switch (tinygl.mode)
        {
        case TINYGL_TEXT_MODE_STEP:
            if (tinygl.scroll_pos != 0)
                break;
            
            tinygl.message_index += 
                tinygl_draw_string (tinygl.message + tinygl.message_index,
                                    tinygl.pos) - 1;
            break;

        case TINYGL_TEXT_MODE_SCROLL:
            pos = tinygl.pos;

            if (tinygl.dir == TINYGL_TEXT_DIR_ROTATE)
                pos.y += tinygl.scroll_pos;
            else
                pos.x -= tinygl.scroll_pos;

            /* In scroll mode, display as much as the string that will
               fit on the display then scroll the reset.  */
            tinygl_draw_string (tinygl.message + tinygl.message_index, pos);
            break;
        }

        tinygl.scroll_pos++;
        if (tinygl.scroll_pos > tinygl.font->width)
        {
            tinygl.scroll_pos = 0;
            tinygl.message_index++;
        }
    }
}


/** Display a message a specified number of times.
    @param string null terminated message to display
    @param pos position on screen
    @param repeats number of times to repeat message.  */
void tinygl_draw_message (const char *string, tinygl_point_t pos,
                          uint8_t repeats)
{
    tinygl.message_index = 0;
    tinygl.message_repeats = repeats;
    tinygl.scroll_pos = 0;

    /* Not much we can do without a font.  */
    if (!tinygl.font)
        return;

    tinygl.text_advance_period = tinygl.update_rate / 
        (tinygl.speed * tinygl.font->width) * 10;

    tinygl.pos = pos;

    strncpy (tinygl.message, string, sizeof (tinygl.message));

    if (tinygl.mode == TINYGL_TEXT_MODE_SCROLL)
    {
        uint8_t message_cols;
        uint8_t message_len;
        uint8_t cols;

        message_len = strlen (string);
        message_cols = message_len * tinygl.font->width + message_len - 1;

        cols = (tinygl.dir == TINYGL_TEXT_DIR_ROTATE) ? TINYGL_HEIGHT 
            : TINYGL_WIDTH;
        if (message_cols > cols)
            tinygl.scroll_pos = -2;
    }
}


/** Display a message repeatedly positioned at the top left pixel.
    @param string null terminated message to display.  */
void tinygl_text (const char *string)
{
    tinygl_point_t pos;

    pos.x = 0;
    pos.y = (tinygl.dir == TINYGL_TEXT_DIR_ROTATE) ? TINYGL_HEIGHT - 1 : 0;

    tinygl_draw_message (string, pos, ~0);
}


/** Set the message update speed.
    @param speed text advance speed (characters per 10 s).  */
void tinygl_text_speed_set (uint8_t speed)
{
    tinygl.speed = speed;
}


/** Set the message display mode.
    @param mode display mode.  */
void tinygl_text_mode_set (tinygl_text_mode_t mode)
{
    tinygl.mode = mode;
}


/** Set the message display direction.
    @param dir display direction.  */
void tinygl_text_dir_set (tinygl_text_dir_t dir)
{
    tinygl.dir = dir;
}


/** Set the font to use for text.
    @param font pointer to font description.  */
void tinygl_font_set (font_t *font)
{
    tinygl.font = font;
}


/** Update display (refresh display and update message).  */
void tinygl_update (void)
{
    static uint16_t tick = 0;

    tick++;
    if (tick >= tinygl.text_advance_period)
    {
        tick = 0;

        tinygl_text_advance ();
    }

    display_update ();
}


/** Initialise things.
    @param rate rate in Hz that tinygl_update called.  */
void tinygl_init (uint16_t rate)
{
    tinygl.update_rate = rate;

    display_init ();

    tinygl_text_speed_set (TINYGL_SPEED_DEFAULT);

    tinygl_clear ();
}

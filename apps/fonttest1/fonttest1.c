/** @file   fonttest1.c
    @author M.P. Hayes
    @date   3 Sep 2010
    @brief  Simple font test program

    @defgroup fonttest1 Simple font test program.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "../fonts/font3x5_1.h"
#include "../fonts/font5x7_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

static font_t *fonts[] = 
{
    &font5x7_1,
    &font3x5_1
};
static tinygl_text_mode_t modes[] = 
{
    TINYGL_TEXT_MODE_STEP,
    TINYGL_TEXT_MODE_STEP
};


static void choose_font (int font_num)
{
    tinygl_font_set (fonts[font_num]);
    tinygl_text_mode_set (modes[font_num]);
    tinygl_clear ();
}


static char show_char (char ch)
{
    char string[2];

    string[0] = ch;
    string[1] = 0;
    tinygl_text (string);
    return ch;
}


int main (void)
{
    char c = 'A';
    unsigned int font_num = 0;

    system_init ();
    tinygl_init (LOOP_RATE);

    navswitch_init ();

    pacer_init (LOOP_RATE);

    choose_font (font_num);
    tinygl_text_speed_set (10);

    c = show_char (c);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_PUSH))
        {
            font_num++;
            if (font_num >= ARRAY_SIZE (fonts))
                font_num = 0;

            choose_font (font_num);
            c = show_char (c);
        }
        else if (navswitch_push_event_p (NAVSWITCH_WEST)
                 || navswitch_push_event_p (NAVSWITCH_SOUTH))
        {
            if (font_contains_p (fonts[font_num], c - 1))
                c = show_char (c - 1);
        }
        else if (navswitch_push_event_p (NAVSWITCH_EAST)
                 || navswitch_push_event_p (NAVSWITCH_NORTH))
        {
            if (font_contains_p (fonts[font_num], c + 1))
                c = show_char (c + 1);
        }

        tinygl_update ();
    }

    return 0;
}

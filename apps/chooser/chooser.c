/** @file   chooser.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Homework winner chooser
    
    @defgroup chooser Homework winner chooser.
*/

#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include "navswitch.h"
#include "../fonts/font5x7_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

/* Define text update rate (characters per 10 s).  */
#define MESSAGE_RATE 10


static void show_num (char ch, uint8_t count)
{
    char buffer[4];

    /* FIXME!  */
    buffer[0] = ch;
    buffer[1] = count / 10 + '0';
    buffer[2] = count % 10 + '0';
    buffer[3] = 0;
    tinygl_text (buffer);
}


int main (void)
{
    int count = 0;

    system_init ();
    tinygl_init (LOOP_RATE);
    navswitch_init ();

    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);

    tinygl_text ("NUM?");

    pacer_init (LOOP_RATE);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();
        tinygl_update ();

        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_PUSH))
        {
            show_num ('T', count);
        }
        else if (navswitch_push_event_p (NAVSWITCH_WEST))
        {
            if (count > 0)
                count--;
            show_num ('N', count);
        }
        else if (navswitch_push_event_p (NAVSWITCH_EAST))
        {
            count++;
            show_num ('N', count);
        }


    }

    return 0;
}

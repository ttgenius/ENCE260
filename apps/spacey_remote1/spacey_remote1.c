/** @file   spacey_remote1.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Test program for IR serial communications.

    @defgroup spacey_remote1 Remote control for the @ref spacey10 "spacey10" application.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "ir_serial.h"
#include "../fonts/font5x7_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

/* Define text update rate (characters per 10 s).  */
#define MESSAGE_RATE 10

typedef enum {SPACEY_EVENT_ALIEN_HIT, SPACEY_EVENT_ALIEN_LANDED} spacey_event_t;


int main (void)
{
    int tick = 0;

    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);

    navswitch_init ();
    ir_serial_init ();

    pacer_init (LOOP_RATE);

    tinygl_text ("#");

    /* Paced loop.  */
    while (1)
    {
        int ret;
        uint8_t data;

        /* Wait for next tick.  */
        pacer_wait ();

        tinygl_update ();

        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_WEST))
            ir_serial_transmit (1);

        if (navswitch_push_event_p (NAVSWITCH_EAST))
            ir_serial_transmit (2);

        ret = ir_serial_receive (&data);
        if (ret == 1)
        {
            spacey_event_t event = data;

            tick = 1;
            
            switch (event)
            {
            case SPACEY_EVENT_ALIEN_HIT:
                tinygl_text ("*");
                break;

            case SPACEY_EVENT_ALIEN_LANDED:
                tinygl_text ("@");
                break;
            }

        }

        if (tick)
            tick++;
        if (tick > LOOP_RATE / 2)
        {
            tinygl_clear ();
            tick = 0;
        }
    }

    return 0;
}

/** @file   ir_serial_test3.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Test program for IR serial communnications.

    @defgroup ir_serial_test3 Test program for IR serial communications.
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


static void show_count (uint8_t count)
{
    char buffer[2];

    /* FIXME!  */
    buffer[0] = count + '0';
    buffer[1] = 0;
    tinygl_text (buffer);
}


int main (void)
{
    int count = 5;

    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);

    navswitch_init ();
    ir_serial_init ();

    show_count (count);

    pacer_init (LOOP_RATE);

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
            ir_serial_transmit (--count);

        if (navswitch_push_event_p (NAVSWITCH_EAST))
            ir_serial_transmit (++count);

        ret = ir_serial_receive (&data);
        if (ret == IR_SERIAL_OK)
            show_count (data);
    }

    return 0;
}

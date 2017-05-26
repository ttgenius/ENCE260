/** @file   ir_serial_test1.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Test program for IR serial communnications.

    @defgroup ir_serial_test1 Test program for IR serial communications.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "ir_serial.h"
#include "../fonts/font3x5_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

/* Define text update rate (characters per 10 s).  */
#define MESSAGE_RATE 10


static void show_count (uint8_t count)
{
    char buffer[3];

    /* FIXME!  */
    buffer[0] = 'S';
    buffer[1] = count + '0';
    buffer[2] = 0;
    tinygl_text (buffer);
}


static void show_err (uint8_t err)
{
    char buffer[3];

    buffer[0] = 'E';
    buffer[1] = err + '0';
    buffer[2] = 0;
    tinygl_text (buffer);
}


int main (void)
{
    int count = 5;

    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);

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
            ir_serial_transmit (1);

        if (navswitch_push_event_p (NAVSWITCH_EAST))
            ir_serial_transmit (2);

        ret = ir_serial_receive (&data);
        if (ret == IR_SERIAL_OK)
        {
            if (data == 1)
                count--;
            else if (data == 2)
                count++;
            else
                count = 0;
            show_count (count);
        }
        else if (ret < 0)
        {
            show_err (-ret);
        }
    }

    return 0;
}

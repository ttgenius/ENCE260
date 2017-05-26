/** @file   ir_uart_test3.c
    @author M. P. Hayes, UCECE
    @date   12 October 2011
    @brief  Test program for IR serial communications.

    @defgroup ir_uart_test3 Test program for IR serial communications.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "uint8toa.h"
#include "ir_uart.h"
#include "../fonts/font3x5_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

#define SWITCH_POLL_RATE 20

/* Define text update rate (characters per 10 s).  */
#define MESSAGE_RATE 10


static void show_byte (uint8_t byte)
{
    char buffer[6];

    uint8toa (byte, buffer, 0);

    tinygl_text (buffer);
}


int main (void)
{
    int count;
    uint8_t data = 'M';

    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);

    navswitch_init ();
    ir_uart_init ();

    pacer_init (LOOP_RATE);

    show_byte ('M');

    count = 0;

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        tinygl_update ();

        if (ir_uart_read_ready_p ())
        {
            uint8_t data;

            data = ir_uart_getc ();

            /* Note, if messages come in too fast, say due to IR
               inteference from fluorescent lights, then the display
               will not keep up and will appear to freeze.  */
            show_byte (data);
        }

        count++;
        if (count > LOOP_RATE / SWITCH_POLL_RATE)
        {
            count = 0;

            navswitch_update ();

            if (navswitch_push_event_p (NAVSWITCH_WEST))
            {
                ir_uart_putc (--data);
            }

            if (navswitch_push_event_p (NAVSWITCH_EAST))
            {
                ir_uart_putc (++data);
            }
        }
    }

    return 0;
}

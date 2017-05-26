/** @file   ir_uart_test2.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Test program for IR serial communications.

    @defgroup ir_uart_test2 Test program for IR serial communications.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "ir_uart.h"
#include "../fonts/font5x7_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

/* Define text update rate (characters per 10 s).  */
#define MESSAGE_RATE 10


static void show_char (char ch)
{
    char buffer[2];

    buffer[0] = ch;
    buffer[1] = 0;
    tinygl_text (buffer);
}


int main (void)
{
    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);

    navswitch_init ();
    ir_uart_init ();

    pacer_init (LOOP_RATE);

    show_char ('M');

    /* Paced loop.  */
    while (1)
    {
        uint8_t data = 'M';

        /* Wait for next tick.  */
        pacer_wait ();

        tinygl_update ();

        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_WEST))
            ir_uart_putc (--data);

        if (navswitch_push_event_p (NAVSWITCH_EAST))
            ir_uart_putc (++data);

        if (ir_uart_read_ready_p ())
        {
            uint8_t data;

            data = ir_uart_getc ();
            show_char (data);
        }
    }

    return 0;
}

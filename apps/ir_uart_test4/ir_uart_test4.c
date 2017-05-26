/** @file   ir_uart_test4.c
    @author M. P. Hayes, UCECE
    @date   26 September 2013
    @brief  Test program for IR serial communications.  This transmits 
    continuously while navswitch is pushed and uses alternate codes
    for separate pushes.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "uint8toa.h"
#include "ir_uart.h"
#include "../fonts/font5x7_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

#define SWITCH_POLL_RATE 20


int main (void)
{
    uint16_t tick = 0;
    uint8_t altcode = 0;

    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);

    navswitch_init ();
    ir_uart_init ();

    pacer_init (LOOP_RATE);

    tinygl_draw_char ('*', tinygl_point (0, 0));

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        /* Refresh display.  */
        tinygl_update ();

        /* Note, if messages come in too fast, say due to IR
           inteference from fluorescent lights, then the display will
           not keep up and will appear to freeze.  */
        if (ir_uart_read_ready_p ())
        {
            uint8_t data;

            data = ir_uart_getc ();

            /* Ignore any dodgy received characters.  */
            if (data >= 'A' && data <= 'Z')
                tinygl_draw_char (data, tinygl_point (0, 0));
        }

        tick++;
        if (tick > LOOP_RATE / SWITCH_POLL_RATE)
        {
            tick = 0;

            navswitch_update ();

            /* On each button press event toggle the code.  */
            if (navswitch_push_event_p (NAVSWITCH_NORTH)
                || navswitch_push_event_p (NAVSWITCH_EAST)
                || navswitch_push_event_p (NAVSWITCH_SOUTH)
                || navswitch_push_event_p (NAVSWITCH_WEST)
                || navswitch_push_event_p (NAVSWITCH_PUSH))
                altcode = !altcode;

            /* While a button is pressed, continuously send
               its code.  */
            if (navswitch_down_p (NAVSWITCH_NORTH))
                ir_uart_putc (altcode ? 'O' : 'N');

            if (navswitch_down_p (NAVSWITCH_EAST))
                ir_uart_putc (altcode ? 'F' : 'E');

            if (navswitch_down_p (NAVSWITCH_SOUTH))
                ir_uart_putc (altcode ? 'T' : 'S');

            if (navswitch_down_p (NAVSWITCH_WEST))
                ir_uart_putc (altcode ? 'X' : 'W');

            if (navswitch_down_p (NAVSWITCH_PUSH))
                ir_uart_putc (altcode ? 'Q' : 'P');
        }
    }

    return 0;
}

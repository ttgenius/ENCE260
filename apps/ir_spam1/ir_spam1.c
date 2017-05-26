/** @file   ir_spam1.c
    @author M. P. Hayes, UCECE
    @date   24 September 2013
    @brief  Repeatedly send message over IR.
*/
#include "system.h"
#include "pacer.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

/* Define polling rate in Hz.  */
#define LOOP_RATE 300


int main (void)
{
    uint16_t ticks = 0;
    
    system_init ();

    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);   

    tinygl_draw_char ('*', tinygl_point (0, 0));

    ir_uart_init ();

    pacer_init (LOOP_RATE);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        ticks++;
        if (ticks >= 10)
        {
            ticks = 0;
            ir_uart_putc ('2');
        }

        tinygl_update ();
    }

    return 0;
}

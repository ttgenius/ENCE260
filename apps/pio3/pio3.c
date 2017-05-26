/** @file   pio3.c
    @author M.P. Hayes
    @date   25 Aug 2011
*/

#include "system.h"
#include "pio.h"
#include "timer.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 1000

#define LED_RATE 2

int main (void)
{
    unsigned int count = 0;
    unsigned int period = LOOP_RATE / LED_RATE;

    system_init ();

    timer_init ();

    pio_config_set (LED1_PIO, PIO_OUTPUT_HIGH);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        timer_wait (TIMER_RATE / LOOP_RATE);

        pio_output_set (LED1_PIO, count < period / 2);
        count++;
        if (count >= period)
            count = 0;
    }

    return 0;
}

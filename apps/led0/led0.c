/** @file   led0.c
    @author M.P. Hayes
    @date   5 Oct 2010
*/

#include "system.h"
#include "led.h"
#include "pacer.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 1000

#define LED_RATE 2

int main (void)
{
    unsigned int count = 0;
    unsigned int period = LOOP_RATE / LED_RATE;

    system_init ();

    pacer_init (LOOP_RATE);
    led_init ();

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        led_set (LED1, count < period / 2);
        count++;
        if (count >= period)
            count = 0;

    }

    return 0;
}

/** @file   led1.c
    @author M.P. Hayes
    @date   5 Oct 2010
    @descr  This flashes the LED twice per second.
*/

#include "system.h"
#include "led.h"
#include "pacer.h"


/* Define polling rate in Hz.  (Note, this is much faster than
 needed for this application.)  */
#define LOOP_RATE 1000

/* LED flash rate (Hz).  */
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

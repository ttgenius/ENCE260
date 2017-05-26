/** @file   led2.c
    @author M.P. Hayes
    @date   5 Oct 2010
*/

#include "system.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"


/* Define polling rate in Hz.  (Note, this is much faster than
 needed for this application.)  */
#define LOOP_RATE 1000

#define LED_RATE_INC 2
#define LED_RATE_MIN 0
#define LED_RATE_MAX 100



int main (void)
{
    int freq = 1;
    unsigned int count = 0;
    unsigned int period = LOOP_RATE / freq;

    system_init ();

    pacer_init (LOOP_RATE);
    led_init ();
    navswitch_init ();


    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_SOUTH))
        {
            freq -= LED_RATE_INC;
            if (freq < LED_RATE_MIN)
                freq = LED_RATE_MIN;

            period = LOOP_RATE / freq;
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH))
        {
            freq += LED_RATE_INC;
            if (freq > LED_RATE_MAX)
                freq = LED_RATE_MAX;

            period = LOOP_RATE / freq;
        }

        led_set (LED1, count < period / 2);
        count++;
        if (count >= period)
            count = 0;

    }

    return 0;
}

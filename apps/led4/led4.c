/** @file   led4.c
    @author M.P. Hayes
    @date   5 Oct 2010
*/

#include "system.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 4000

#define LED_RATE 40


int main (void)
{
    unsigned int duty[] = {0, 1, 2, 5, 10, 20, 50, 100};
    unsigned int duty_index = 3;
    unsigned int count = 0;
    unsigned int period = LOOP_RATE / LED_RATE;
    unsigned int offcount = period * duty[duty_index] / 100;

    system_init ();

    pacer_init (LOOP_RATE);
    led_init ();
    navswitch_init ();


    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        /* There is no need to poll the switches so frequently but it
           keeps the code simpler.  */
        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_SOUTH))
        {
            if (duty_index > 0)
                duty_index--;

            offcount = (period * duty[duty_index] + 50) / 100;
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH))
        {
            duty_index++;
            if (duty_index >= ARRAY_SIZE (duty))
                duty_index = ARRAY_SIZE (duty) - 1;
            offcount = (period * duty[duty_index] + 50) / 100;
        }

        led_set (LED1, count < offcount);
        count++;
        if (count >= period)
            count = 0;
    }

    return 0;
}

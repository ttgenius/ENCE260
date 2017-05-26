/** @file   led3.c
    @author M.P. Hayes
    @date   5 Oct 2010
*/

#include "system.h"
#include "navswitch.h"
#include "led.h"
#include "pacer.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 1000

#define LED_RATE 50
#define LED_DUTY_INC 5
#define LED_DUTY_MIN 0
#define LED_DUTY_MAX 100


int main (void)
{
    int duty = 50;
    unsigned int count = 0;
    unsigned int period = LOOP_RATE / LED_RATE;
    unsigned int offcount = period * duty / 100;

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
            duty -= LED_DUTY_INC;
            if (duty < LED_DUTY_MIN)
                duty = LED_DUTY_MIN;

            offcount = period * duty / 100;
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH))
        {
            duty += LED_DUTY_INC;
            if (duty > LED_DUTY_MAX)
                duty = LED_DUTY_MAX;
            offcount = period * duty / 100;
        }

        led_set (LED1, count < offcount);
        count++;
        if (count >= period)
            count = 0;

    }

    return 0;
}

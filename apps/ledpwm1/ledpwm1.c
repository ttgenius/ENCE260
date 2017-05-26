/** @file   ledpwm1.c
    @author M.P. Hayes
    @date   25 Sep 2013
*/

#include "system.h"
#include "pacer.h"
#include "led.h"
#include "spwm.h"

#define LOOP_RATE 100


int main (void)
{
    spwm_t spwm1 = {.duty = 50, .period = 100};
    
    system_init ();

    led_init ();

    pacer_init (LOOP_RATE);
    
    while (1)
    {
        pacer_wait ();

        led_set (LED1, spwm_update (&spwm1));
    }
    return 0;
}

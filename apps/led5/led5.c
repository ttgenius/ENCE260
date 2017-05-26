/** @file   led5.c
    @author M.P. Hayes
    @date   18 Aug 2011
*/

#include "system.h"
#include "timer.h"
#include "led.h"

int main (void)
{
    timer_tick_t now;
    
    system_init ();

    timer_init ();
    led_init ();
    
    now = timer_get ();
    while (1)
    {
        led_set (LED1, 1);
        
        now = timer_wait_until (now + (timer_tick_t)(TIMER_RATE * 0.5));
        
        led_set (LED1, 0);
        
        now = timer_wait_until (now + (timer_tick_t)(TIMER_RATE * 0.75));
    }
    return 0;
}

/** @file   demo2.c
    @author M.P. Hayes
    @date   25 Aug 2011
*/

#include "system.h"
#include "led.h"

int main (void)
{
    bool state = 0;

    system_init ();

    led_init ();

    while (1)
    {
        led_set (LED1, state);
        state = !state;
    }

    return 0;
}

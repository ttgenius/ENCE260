#include "system.h"
#include "led.h"
#include "pacer.h"


int main (void)
{
    uint8_t state = 0;

    system_init ();
    led_init ();
    
    /* Set up pacer with a frequency of 2 Hz.  */
    pacer_init (2);
    
    while (1)
    {
        /* Pace the loop.  */
        pacer_wait ();
        
        /* Toggle LED. */
        led_set(LED1, state);
        state = !state;
    }
}

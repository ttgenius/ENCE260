#include "system.h"
#include "led.h"

#include "timer.h"

int main (void)
{
    system_init ();
    led_init ();
    
    /* Initialise timer.  */
    timer_init ();
    
    while (1)
    {
        /* Turn LED on.  */
        led_set (LED1, 1);      
        
        /* Wait 500ms.  */
        timer_delay_ms (500);
        
        /* Turn LED off.  */
        led_set (LED1, 0);
        
        /* Wait 500ms.  */
        timer_delay_ms (500);      
    }
}

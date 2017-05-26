#include <avr/io.h>
#include "led.h"


/** Turn LED1 on.  */
void led_on (void)
{
    PORTC |=(1<<2);
    /* TODO!  */
}


/** Turn LED1 off.  */
void led_off (void)
{
    PORTC &= ~(1<<2);
    /* TODO!  */
}


/** Initialise LED1.  */
void led_init (void)
{
    DDRC |= (1<<2);
    /* TODO!  */
}

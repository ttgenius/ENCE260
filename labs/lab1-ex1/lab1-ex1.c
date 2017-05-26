#include <avr/io.h>
#include "system.h"

int main (void)
{
    system_init ();

    /* Initialise port to drive LED 1.  */
    DDRC |=(1<<2);  // SET THIS TO AN OUTPUT FOR LED
    DDRD &= ~(1<<7); // SET THIS TO AN INPUT FOR SWITCH
    
    /* TODO.  */

    while (1)
    {
        if ((PIND & BIT(7))){
        /* Turn LED 1 on.  */
			PORTC |=(1<<2);
		} else{
			PORTC &= ~(1<<2);	
			

        /* TODO.  */
	    }
    }
}


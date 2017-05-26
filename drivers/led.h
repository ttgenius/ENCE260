/**  @file   led.h
     @author M. P. Hayes, UCECE
     @date   15 Feb 2003
     @brief  LED driver.

    @defgroup LED LED driver
       
    This module implements a simple LED driver.

    Here's an example application that turns on a LED:

    @code
    #include "led.h"

    void main (void)
    {
        system_init ();
        led_init ();

        led_set (LED1, 1);

        while (1)
        {
        }
    }
    @endcode
*/

#ifndef LED_H
#define LED_H

#include "system.h"


/** Set LED to desired state.
    @param led LED to select
    @param state desired state (non-zero for on).  */
void led_set (uint8_t led, bool state);


/** Initialise LED driver.  */
void led_init (void);

#endif

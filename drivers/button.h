/**  @file   button.h
     @author M. P. Hayes, UCECE
     @date   15 Feb 2003
     @brief  Button polling (no debouncing).

    @defgroup button Pushbutton driver
       
    This module implements a simple pushbutton driver.  Note, no
    debouncing is performed.

    Here's an example application where the buttons are polled at 50 Hz:

    @code
    #include "pacer.h"
    #include "button.h"

    void main (void)
    {
        system_init ();
        button_init ();
        pacer_init (50);

        while (1)
        {
	    pacer_wait ();
            button_update ();

            if (button_push_event_p (0))
                // Do something
        }
    }
    @endcode
*/


#ifndef BUTTON_H
#define BUTTON_H

#include "system.h"


/** Poll all the buttons and update their state.  */
void button_update (void);


/** Return true if button state changed from up to down since 
    last call to button_update
    @param  button index of button to select
    @return 1 if button changed from up to down otherwise 0  */
bool button_push_event_p (uint8_t button);


/** Return true if button state changed from down to up since 
    last call to button_update
    @param  button index of button to select
    @return 1 if button changed from down to up otherwise 0  */
bool button_release_event_p (uint8_t button);


/** Return true if button down (pushed).
    @param  button index of button to select
    @return 1 if button down otherwise 0  */
bool button_down_p (uint8_t button);


/** Return true if button up (released).
    @param  button index of button to select
    @return 1 if button down otherwise 0  */
bool button_up_p (uint8_t button);


/** Initialise button driver.  */
void button_init (void);
#endif

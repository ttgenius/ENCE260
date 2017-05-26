/**  @file   navswitch.h
     @author M. P. Hayes, UCECE
     @date   15 Feb 2003
     @brief  Navswitch polling (no debouncing).

     @defgroup navswitch Navswitch driver
       
     This module implements a simple navswitch driver.  Note, no
     debouncing is performed.

     Here's an example application where the navswitches are polled at 50 Hz:

    @code
    #include "pacer.h"
    #include "navswitch.h"

    void main (void)
    {
        system_init ();
        navswitch_init ();
        pacer_init (50);

        while (1)
        {
	    pacer_wait ();
            navswitch_update ();

            if (navswitch_push_event_p (NAVSWITCH_EAST))
                // Do something

            if (navswitch_push_event_p (NAVSWITCH_WEST))
                // Do something
        }
    }
    @endcode
*/


#ifndef NAVSWITCH_H
#define NAVSWITCH_H

#include "system.h"


enum {NAVSWITCH_NORTH, NAVSWITCH_EAST, NAVSWITCH_SOUTH, NAVSWITCH_WEST, 
      NAVSWITCH_PUSH};


/** Poll all the navswitches and update their state.  */
void navswitch_update (void);


/** Return true if navswitch state changed from up to down since 
    last call to navswitch_update
    @param  navswitch index of navswitch to select
    @return 1 if navswitch changed from up to down otherwise 0  */
bool navswitch_push_event_p (uint8_t navswitch);


/** Return true if navswitch state changed from down to up since 
    last call to navswitch_update
    @param  navswitch index of navswitch to select
    @return 1 if navswitch changed from down to up otherwise 0  */
bool navswitch_release_event_p (uint8_t navswitch);


/** Return true if navswitch down (pushed).
    @param  navswitch index of navswitch to select
    @return 1 if navswitch down otherwise 0  */
bool navswitch_down_p (uint8_t navswitch);


/** Return true if navswitch up (released).
    @param  navswitch index of navswitch to select
    @return 1 if navswitch down otherwise 0  */
bool navswitch_up_p (uint8_t navswitch);


/** Initialise navswitch driver.  */
void navswitch_init (void);
#endif

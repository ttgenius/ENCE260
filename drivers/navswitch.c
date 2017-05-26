/** @file   navswitch.c
    @author M.P. Hayes
    @date   28 Aug 2008
    @brief Navswitch driver.
    @note This polls the 5-way navigation switch
          but does not do debouncing.
*/

#include "system.h"
#include "navswitch.h"
#include "delay.h"
#include "pio.h"


/* Navswitch configuration structure.  */
typedef struct
{
    pio_t pio;
} navswitch_cfg_t;


/** Navswitch state structure.  */
typedef struct
{
    bool current;
    bool previous;
} navswitch_state_t;


/* Define navswitch PIO connections.  */    
static const navswitch_cfg_t navswitch_cfg[] =
{
    {NAVSWITCH_NORTH_PIO},
    {NAVSWITCH_EAST_PIO},
    {NAVSWITCH_SOUTH_PIO}, 
    {NAVSWITCH_WEST_PIO},
    {NAVSWITCH_PUSH_PIO}
};

#define NAVSWITCH_NUM ARRAY_SIZE (navswitch_cfg)

static navswitch_state_t navswitch_state[NAVSWITCH_NUM];


/** Return true if navswitch state changed from up to down since 
    last call to navswitch_update
    @param  navswitch index of navswitch to select
    @return 1 if navswitch changed from up to down otherwise 0  */
bool
navswitch_push_event_p (uint8_t navswitch)
{
    return navswitch_state[navswitch].current
        && !navswitch_state[navswitch].previous;
}


/** Return true if navswitch state changed from down to up since 
    last call to navswitch_update
    @param  navswitch index of navswitch to select
    @return 1 if navswitch changed from down to up otherwise 0  */
bool
navswitch_release_event_p (uint8_t navswitch)
{
    return !navswitch_state[navswitch].current 
        && navswitch_state[navswitch].previous;
}


/** Return true if navswitch down (pushed).
    @param  navswitch index of navswitch to select
    @return 1 if navswitch down otherwise 0  */
bool
navswitch_down_p (uint8_t navswitch)
{
    return navswitch_state[navswitch].current;
}


/** Return true if navswitch up (released).
    @param  navswitch index of navswitch to select
    @return 1 if navswitch down otherwise 0  */
bool
navswitch_up_p (uint8_t navswitch)
{
    return !navswitch_state[navswitch].current;
}


/** Poll all the navswitches and update their state.  */
void
navswitch_update (void)
{
    uint8_t i;

    /* The switch is a 5-way navigation switch with push.  The common
       connection is connected to ground.  The 5 switch outputs share
       the LED matrix display column drive pins so we need to save the
       state of the column outputs, switch them to inputs, read the
       switch states, switch them back to outputs, and restore the
       output state.

       When a switch is pushed it will pull its associated PIO pin low
       via a 2.2 kohm resistor.  When the PIO pin is configured as an
       input with pullup, pushing the switch will also turn on the
       column MOSFET driver.  This will cause some ghosting of the
       display.  This can be avoided by driving all the LED matrix
       rows low while we read the switches.  */

    for (i = 0; i < NAVSWITCH_NUM; i++)
    {
        pio_config_t config;

        navswitch_state[i].previous = navswitch_state[i].current;
    
        config = pio_config_get (navswitch_cfg[i].pio);

        /* Momentarily force PIO pin high to charge gate capacitance of
           MOSFET otherwise will always read logic low and think the
           switch is pushed.  Alternatively, we need to wait 10 us or
           more so that the gate capacitance of the MOSFET charges via
           the internal pullup resistor.  */
        pio_config_set (navswitch_cfg[i].pio, PIO_OUTPUT_HIGH);

        /* Switch PIO to an input to read switch.  */
        pio_config_set (navswitch_cfg[i].pio, PIO_PULLUP);

        /* Wait long enough for MOSFET gate capacitance to discharge
           through 2.2 kohm resistor when switch is pushed (connected
           to ground).  */
        DELAY_US (2.5); 

        navswitch_state[i].current = pio_input_get (navswitch_cfg[i].pio) == 0;

        /* Restore PIO state.  */
        pio_config_set (navswitch_cfg[i].pio, config);
    }
}


/** Initialise navswitch driver.  */
void navswitch_init (void)
{
    /* Nothing to do since we configure PIOs when polling the switch.  */
}

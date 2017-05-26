/** @file   button.c
    @author M.P. Hayes
    @date   28 Aug 2008
    @brief  Button polling (no debouncing).
    @note   This polls pushbutton switches but does not do debouncing.
*/

#include "system.h"
#include "button.h"
#include "pio.h"


/* Button configuration structure.  */
typedef struct
{
    pio_t pio;
    /* True for active high, false for active low.  */
    bool active_high;
} button_cfg_t;


/** Button state structure.  */
typedef struct
{
    bool current;
    bool previous;
} button_state_t;


/* Define buttons.  */    
static const button_cfg_t buttons_cfg[] =
{
    {.pio = BUTTON1_PIO, .active_high = 1},
    /* Add config data for additional buttons here.  */
};

#define BUTTONS_NUM ARRAY_SIZE (buttons_cfg)

static button_state_t buttons_state[BUTTONS_NUM];


/** Return true if button state changed from up to down since 
    last call to button_update
    @param  button index of button to select
    @return 1 if button changed from up to down otherwise 0  */
bool
button_push_event_p (uint8_t button)
{
    if (button >= BUTTONS_NUM)
        return 0;
    return buttons_state[button].current && !buttons_state[button].previous;
}


/** Return true if button state changed from down to up since 
    last call to button_update
    @param  button index of button to select
    @return 1 if button changed from down to up otherwise 0  */
bool
button_release_event_p (uint8_t button)
{
    if (button >= BUTTONS_NUM)
        return 0;
    return !buttons_state[button].current && buttons_state[button].previous;
}


/** Return true if button down (pushed).
    @param  button index of button to select
    @return 1 if button down otherwise 0  */
bool
button_down_p (uint8_t button)
{
    if (button >= BUTTONS_NUM)
        return 0;
    return buttons_state[button].current;
}


/** Return true if button up (released).
    @param  button index of button to select
    @return 1 if button down otherwise 0  */
bool
button_up_p (uint8_t button)
{
    if (button >= BUTTONS_NUM)
        return 0;
    return !buttons_state[button].current;
}


/** Poll all the buttons and update their state.  */
void
button_update (void)
{
    uint8_t i;

    for (i = 0; i < BUTTONS_NUM; i++)
    {
        buttons_state[i].previous = buttons_state[i].current;

        buttons_state[i].current = ! (pio_input_get (buttons_cfg[i].pio)
                                      ^ buttons_cfg[i].active_high);
    }
}


/** Initialise button driver.  */
void button_init (void)
{
    uint8_t i;

    for (i = 0; i < BUTTONS_NUM; i++)
    {
        /* Configure pio for input and enable internal pullup resistor
           if active low.  If button is active high it is assumed
           there is an external pulldon resistor.  */
        pio_config_set (buttons_cfg[i].pio, 
                        buttons_cfg[i].active_high ? PIO_INPUT : PIO_PULLUP);
    }
}



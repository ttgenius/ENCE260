/**  @file   led.c
     @author M. P. Hayes, UCECE
     @date   15 Feb 2003
     @brief  LED driver.
*/

#include "system.h"
#include "pio.h"
#include "led.h"


typedef struct 
{
    pio_t pio;
    /* True for active high, false for active low.  */
    bool active_high;
} led_cfg_t;


/* Define LEDs.  */    
static const led_cfg_t leds_cfg[] =
{
    {.pio = LED1_PIO, .active_high = 1},
    /* Add config data for additional LEDs here.  */
};

#define LEDS_NUM ARRAY_SIZE (leds_cfg)


void led_set (uint8_t led, bool state)
{
    if (led >= LEDS_NUM)
        return;
    pio_output_set (leds_cfg[led].pio,
                    leds_cfg[led].active_high ? state : !state);
}


/** Initialise LED driver.  */
void led_init (void)
{
    uint8_t i;

    for (i = 0; i < LEDS_NUM; i++)
    {
        pio_config_set (leds_cfg[i].pio,
                        leds_cfg[i].active_high 
                        ? PIO_OUTPUT_HIGH : PIO_OUTPUT_LOW);
    }
}

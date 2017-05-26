#include "pio.h"
#include "led.h"

#define LED PIO_DEFINE (PORT_C, 2)
/** Turn LED1 on.  */
void led_on (void)
{
    pio_output_high(LED);/* TODO!  */
}


/** Turn LED1 off.  */
void led_off (void)
{
    pio_output_low(LED);/* TODO!  */
}

void led_toggle(void){
	pio_output_toggle(LED);
}


/** Initialise LED1.  */
void led_init (void)
{
    pio_config_set(LED,PIO_OUTPUT_LOW);/* TODO!  */
}

/** @file   pacerdemo1.c
    @author M.P. Hayes
    @date   24 Sep 2013
*/

#include "system.h"
#include "pacer.h"
#include "pio.h"
#include "led.h"

/* Define paced loop rate in Hz.  */
#define LOOP_RATE 1000U

#define PERIOD_MS 20

#define DUTY_MS 15

#define TICK_PIO PD6_PIO

int main (void)
{
    uint8_t tick = 0;

    system_init ();

    pacer_init (LOOP_RATE);
    led_init ();

    pio_config_set (TICK_PIO, PIO_OUTPUT_LOW);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        pio_output_toggle (TICK_PIO);

        tick++;
        if (tick >= LOOP_RATE * PERIOD_MS / 1000)
        {
            tick = 0;
            led_set (LED1, 1);
        }
        else if (tick == LOOP_RATE * DUTY_MS / 1000)
        {
            led_set (LED1, 0);
        }
    }

    return 0;
}

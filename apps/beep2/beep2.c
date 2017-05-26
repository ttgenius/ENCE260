/** @file   beep2.c
    @author M.P. Hayes
    @date   17 Sep 2013
*/

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "button.h"

/* Connect piezo tweeter to pins 6 and 8 of UCFK4 P1 connector
   for push-pull operation.  */
#define PIEZO1_PIO PIO_DEFINE (PORT_D, 4)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 6)
#define TEST_PIO PIO_DEFINE (PORT_D, 3)

#define TONE_FREQUENCY 440
#define LOOP_RATE 10000

int main (void)
{
    int tick = 0;

    system_init ();

    pio_config_set (PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (PIEZO2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (TEST_PIO, PIO_OUTPUT_HIGH);

    pacer_init (LOOP_RATE);
    
    while (1)
    {
        pacer_wait ();

        pio_output_toggle (TEST_PIO);

        tick = tick + 1;

        if (tick >= (LOOP_RATE / (TONE_FREQUENCY * 2)))
        {
            tick = 0;

            /* Generate annoying tone.  */
            pio_output_toggle (PIEZO1_PIO);
            pio_output_toggle (PIEZO2_PIO);
        }
    }

    return 0;
}

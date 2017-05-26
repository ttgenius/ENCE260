/** @file   beep1.c
    @author M.P. Hayes
    @date   17 Sep 2013
*/

#include "system.h"
#include "pio.h"
#include "pacer.h"

/* Connect piezo tweeter to pins 6 and 8 of UCFK4 P1 connector
   for push-pull operation.  */
#define PIEZO1_PIO PIO_DEFINE (PORT_D, 4)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 6)

#define TONE_FREQUENCY 440
#define LOOP_RATE (TONE_FREQUENCY * 2)

int main (void)
{
    system_init ();

    pio_config_set (PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (PIEZO2_PIO, PIO_OUTPUT_HIGH);

    pacer_init (LOOP_RATE);
    
    while (1)
    {
        pacer_wait ();

        /* Generate annoying tone.  */
        pio_output_toggle (PIEZO1_PIO);
        pio_output_toggle (PIEZO2_PIO);
    }
    return 0;
}

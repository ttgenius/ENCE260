/** @file   pio1.c
    @author M.P. Hayes
    @date   25 Aug 2011
*/

#include "system.h"
#include "pio.h"

int main (void)
{
    system_init ();

    pio_config_set (LED1_PIO, PIO_OUTPUT_HIGH);

    while (1)
    {
        pio_output_toggle (LED1_PIO);
    }

    return 0;
}

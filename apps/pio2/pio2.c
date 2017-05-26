/** @file   pio2.c
    @author M.P. Hayes
    @date   25 Aug 2011
*/

#include "system.h"
#include "pio.h"

int main (void)
{
    uint16_t count = 0;

    system_init ();

    pio_config_set (LED1_PIO, PIO_OUTPUT_HIGH);

    while (1)
    {
        count++;
        if (count > 10000)
        {
            pio_output_toggle (LED1_PIO);
            count = 0;
        }
    }

    return 0;
}

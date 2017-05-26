/** @file   tdmdemo1.c
    @author M. P. Hayes, UCECE
    @date   10 August 2011
    @brief  Time division multiplexing of display demo
    
    @defgroup tdmdemo1 Time division multiplexing of display demo
*/

#include "system.h"
#include "pacer.h"
#include "ledmat.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 1000

/* Define display refresh rate in Hz.  */
#define REFRESH_RATE 1

#define UPDATE_PERIOD (LOOP_RATE / (LEDMAT_COLS_NUM * REFRESH_RATE))


int main (void)
{
    uint8_t col = 0;
    uint8_t tick = 0;

    system_init ();
    ledmat_init ();

    pacer_init (LOOP_RATE);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        tick++;
        if (tick >= UPDATE_PERIOD)
        {
            tick = 0;

            /* Display an 'H' character by time division multiplexing
               the columns of the LED matrix.  */
            switch (col)
            {
            case 0:
                ledmat_display_column (0x7f, 0);
                break;

            case 1:
                ledmat_display_column (0x08, 1);
                break;

            case 2:
                ledmat_display_column (0x08, 2);
                break;

            case 3:
                ledmat_display_column (0x08, 3);
                break;

            case 4:
                ledmat_display_column (0x7f, 4);
                break;

            default:
                break;
            }
                
            col++;
            if (col >= LEDMAT_COLS_NUM )
                col = 0;
        }
        
    }

    return 0;
}

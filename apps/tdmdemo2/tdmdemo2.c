/** @file   tdmdemo2.c
    @author M. P. Hayes, UCECE
    @date   10 August 2011
    @brief  Time division multiplexing of display demo
    
    @defgroup tdmdemo2 Time division multiplexing of display demo
*/

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ledmat.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 1000


int main (void)
{
    uint8_t col = 0;
    uint8_t tick = 0;
    uint8_t refresh_rate = 1;
    uint16_t update_period;

    system_init ();
    ledmat_init ();
    navswitch_init ();

    pacer_init (LOOP_RATE);

    update_period = LOOP_RATE / (LEDMAT_COLS_NUM * refresh_rate);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_EAST))
        {
            refresh_rate++;
            update_period = LOOP_RATE / (LEDMAT_COLS_NUM * refresh_rate);
        }

        if (navswitch_push_event_p (NAVSWITCH_WEST) && refresh_rate > 0)
        {
            /* Allow refresh_rate to go to zero so can stop display
               refreshing.  */
            refresh_rate--;
            update_period = LOOP_RATE / (LEDMAT_COLS_NUM * refresh_rate);
        }

        tick++;
        if (tick >= update_period)
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

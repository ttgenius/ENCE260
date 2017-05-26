/** @file   bounce2.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Simple bouncing dot program
    
    @defgroup bounce2 Bounce2 application
*/
#include "system.h"
#include "pacer.h"
#include "display.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 200


int main (void)
{
    int row;
    int col;
    int rowinc;
    int colinc;
    int tick;

    system_init ();
    display_init ();

    row = 3;
    col = 2;
    rowinc = 1;
    colinc = 1;

    display_pixel_set (col, row, 1);

    pacer_init (LOOP_RATE);
    tick = 0;

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        tick++;

        if (tick >= 10)
        {
            tick = 0;

            /* Erase previous position.  */
            display_pixel_set (col, row, 0);        
            
            col += colinc;
            row += rowinc;
            
            if (row > 6 || row < 0)
            {
                row -= rowinc * 2;
                rowinc = -rowinc;
            }
            
            if (col > 4 || col < 0)
            {
                col -= colinc * 2;
                colinc = -colinc;
            }
            
            /* Draw new position.  */
            display_pixel_set (col, row, 1);        
        }

        display_update ();
    }
}

/** @file   bounce4.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Multiple bouncing dot program
    
    @defgroup bounce4 Bounce4 application
*/
#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "boing.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 200


int main (void)
{
    int tick;
    boing_state_t balls[2];

    system_init ();
    tinygl_init (LOOP_RATE);

    pacer_init (LOOP_RATE);
    tick = 0;

    balls[0] = boing_init (0, 1, DIR_NE);
    balls[1] = boing_init (4, 5, DIR_SE);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        tick++;

        if (tick >= 40)
        {
            int i;

            tick = 0;

            for (i = 0; i < 2; i++)
            {
                /* Erase previous position.  */
                tinygl_draw_point (balls[i].pos, 0);
                
                balls[i] = boing_update (balls[i]);

                /* Draw new position.  */
                tinygl_draw_point (balls[i].pos, 1);
            }
        }

        tinygl_update ();
    }
}

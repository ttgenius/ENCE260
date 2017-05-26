/** @file   bounce3.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Simple bouncing dot program.  This has a deliberate bug!
    
    @defgroup bounce3 Bounce3 application
*/
#include "system.h"
#include "pacer.h"
#include "tinygl.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 200


int main (void)
{
    tinygl_point_t pos;
    int xinc;
    int yinc;
    int tick;

    system_init ();
    tinygl_init (LOOP_RATE);

    pos.y = 3;
    pos.x = 2;
    yinc = 1;
    xinc = 1;

    tinygl_draw_point (pos, 1);

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
            tinygl_draw_point (pos, 0);        
            
            pos.x += xinc;
            pos.y += yinc;
            
            if (pos.y > 6)
            {
                pos.y -= yinc * 2;
                yinc = -yinc;
            }
            
            if (pos.x > 4)
            {
                pos.x -= xinc * 2;
                xinc = -xinc;
            }
            
            /* Draw new position.  */
            tinygl_draw_point (pos, 1);        
        }

        tinygl_update ();
    }
}

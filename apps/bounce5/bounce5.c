/** @file   bounce5.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Multiple bouncing dot program
    
    @defgroup bounce5 Bounce5 application
*/
#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "boing.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 200


/* Determine if two balls have collided
   @param balls array of all the ball states
   @param num_balls number of balls in array
   @param this_ball index of ball to check
   @return index of ball collided with or -1 for no collision  */
static int collision_detect (boing_state_t *balls, int num_balls, int this_ball)
{
    int i;
    
    for (i = 0; i < num_balls; i++)
    {
        /* Cannot collide with self.  */
        if (i == this_ball)
            continue;
        
        if (balls[i].pos.x == balls[this_ball].pos.x
            && balls[i].pos.y == balls[this_ball].pos.y)
            return i;
    }
    return -1;
}



int main (void)
{
    int tick;
    boing_state_t balls[3];

    system_init ();
    tinygl_init (LOOP_RATE);

    pacer_init (LOOP_RATE);
    tick = 0;

    balls[0] = boing_init (0, 1, DIR_NE);
    balls[1] = boing_init (4, 5, DIR_SE);
    balls[2] = boing_init (4, 5, DIR_SW);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        tick++;

        /* Flash the first two balls at different rates.  */
        tinygl_draw_point (balls[0].pos, tick % 2 < 1);
        tinygl_draw_point (balls[1].pos, tick % 4 < 2);

        if (tick >= 40)
        {
            int i;

            tick = 0;

            for (i = 0; i < 3; i++)
            {
                /* Erase previous position.  */
                tinygl_draw_point (balls[i].pos, 0);
                
                /* Check for collision; if so reverse direction.  */
                balls[i] = boing_update (balls[i]);

                /* Perhaps should make ball that is hit reverse as well?  */
                if (collision_detect (balls, 2, i) > 0)
                {
                    balls[i] = boing_reverse (balls[i]);
                }

                /* Draw previous position.  */
                tinygl_draw_point (balls[i].pos, 1);
            }
        }

        tinygl_update ();
    }
}

/** @file   random1.c
    @author M.P. Hayes
    @date   25 Sep 2013
    @brief  Example to show how pseudorandom numbers follow the same
    sequence.
*/

#include <stdlib.h>
#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "navswitch.h"
#include "../fonts/font5x7_1.h"

#define LOOP_RATE 10000

#define NAVSWITCH_RATE 20


int main (void)
{
    uint16_t navswitch_tick = 0;

    system_init ();

    navswitch_init ();

    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);

    pacer_init (LOOP_RATE);

    tinygl_draw_char ('*', tinygl_point (0, 0));
    
    while (1)
    {
        pacer_wait ();

        tinygl_update ();

        navswitch_tick++;
        if (navswitch_tick >= LOOP_RATE / NAVSWITCH_RATE)
        {
            navswitch_tick = 0;

            navswitch_update ();

            if (navswitch_push_event_p (NAVSWITCH_PUSH))
            {
                char c;

                c = (rand () % 26) + 'A';

                tinygl_draw_char (c, tinygl_point (0, 0));
            }
        }
    }
    return 0;
}

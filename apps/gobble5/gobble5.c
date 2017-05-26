/** @file   gobble5.c
    @author M.P. Hayes
    @date   29 Sep 2013
    @brief 
*/

#include <stdlib.h>
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "led.h"
#include "timer.h"
#include "tinygl.h"

/* Number of initial objects.  */
#define NUM_THINGS 6

#define LOOP_RATE 300

#define MOVE_RATE 20

#define NAVSWITCH_RATE 50


typedef struct thing_struct
{
    tinygl_point_t pos;
    bool alive;
} thing_t;


static int8_t thing_find (thing_t *things, uint8_t num, tinygl_point_t pos)
{
    uint8_t i;

    for (i = 0; i < num; i++)
    {
        if (things[i].pos.x == pos.x
            && things[i].pos.y == pos.y
            && things[i].alive)
            return i;
    }
    return -1;
}


static uint8_t thing_distance_squared (thing_t *things, uint8_t this)
{
    int8_t dx;
    int8_t dy;

    dx = things[this].pos.x - things[0].pos.x;
    dy = things[this].pos.y - things[0].pos.y;

    return dx * dx + dy * dy;
}


static uint8_t bounds_p (tinygl_point_t pos)
{
    return pos.x >= 0 && pos.x < TINYGL_WIDTH
        && pos.y >= 0 && pos.y < TINYGL_HEIGHT;    
}


static void monster_move (thing_t *things, uint8_t num, int8_t dx, int8_t dy)
{
    int8_t other;
    tinygl_point_t newpos;

    newpos = tinygl_point (things[0].pos.x + dx, things[0].pos.y + dy);
    if (!bounds_p (newpos))
        return;

    other = thing_find (things, num, newpos);
    if (other != -1)
        things[other].alive = 0;

    tinygl_draw_point (things[0].pos, 0);
    things[0].pos = newpos;
    tinygl_draw_point (newpos, 1);
}


static void thing_move (thing_t *things, uint8_t num, uint8_t this)
{
    uint8_t i;
    uint8_t distsq;
    tinygl_point_t pos;

    tinygl_draw_point (things[this].pos, 0);

    distsq = thing_distance_squared (things, this);
    pos = things[this].pos;

    for (i = 0; i < 12; i++)
    {
        int8_t dx;
        int8_t dy;
        tinygl_point_t newpos;
        
        dx = (rand () % 3) - 1;
        dy = (rand () % 3) - 1;

        newpos = tinygl_point (pos.x + dx, pos.y + dy);
    
        /* Check for a valid position.  */
        if ((dx || dy) 
            && (! bounds_p (newpos) || thing_find (things, num, newpos) != -1))
            continue;

        /* Try new position.  */
        things[this].pos = newpos;

        if (thing_distance_squared (things, this) > distsq)
            break;
    }

    /* Stay put if cannot find better position.  */
    if (i == 12)
        things[this].pos = pos;
        
    tinygl_draw_point (things[this].pos, 1);
}


static void things_move (thing_t *things, uint8_t num)
{
    uint8_t i;

    for (i = 1; i < num; i++)
        if (things[i].alive)
            thing_move (things, num, i);
}


static void things_create (thing_t *things, uint8_t num)
{
    uint8_t i;

    things[0].pos.x = 0;
    things[0].pos.y = 0;
    things[0].alive = 1;
    tinygl_draw_point (things[0].pos, 1);

    for (i = 1; i < num; i++)
    {
        uint8_t x;
        uint8_t y;

        do
        {
            x = rand () % TINYGL_WIDTH;
            y = rand () % TINYGL_HEIGHT;
        } while (thing_find (things, i, tinygl_point (x, y)) != -1);
        
        things[i].pos.x = x;
        things[i].pos.y = y;
        things[i].alive = 1;

        tinygl_draw_point (things[i].pos, 1);
    }
}


static void things_erase (thing_t *things, uint8_t num)
{
    uint8_t i;

    for (i = 0; i < num; i++)
    {
        tinygl_draw_point (things[i].pos, 0);
    }
}


int main (void)
{
    uint16_t tick = 0;
    uint16_t navswitch_tick = 0;
    uint8_t running = 0;
    thing_t things[NUM_THINGS];

    system_init ();
    navswitch_init ();
    led_init ();
    led_set (LED1, 0);

    tinygl_init (LOOP_RATE);

    pacer_init (LOOP_RATE);

    while (1)
    {
        uint8_t col;

        /* Refresh things.  */
        for (col = 0; col < TINYGL_WIDTH; col++)
        {
            pacer_wait ();

            tinygl_update ();
        }

        navswitch_tick++;
        if (navswitch_tick >= LOOP_RATE / NAVSWITCH_RATE)
        {
            navswitch_tick = 0;

            navswitch_update ();

            if (navswitch_push_event_p (NAVSWITCH_PUSH))
            {
                srand (timer_get ());
                running = ! running;
                led_set (LED1, running);
            }

            if (running)
            {
                if (navswitch_push_event_p (NAVSWITCH_NORTH))
                    monster_move (things, NUM_THINGS, 0, -1);
                if (navswitch_push_event_p (NAVSWITCH_SOUTH))
                    monster_move (things, NUM_THINGS, 0, 1);
                if (navswitch_push_event_p (NAVSWITCH_EAST))
                    monster_move (things, NUM_THINGS, 1, 0);
                if (navswitch_push_event_p (NAVSWITCH_WEST))
                    monster_move (things, NUM_THINGS, -1, 0);
            }
        }

        tick++;
        if (tick >= LOOP_RATE / MOVE_RATE)
        {
            tick = 0;

            if (running)
            {
                things_move (things, NUM_THINGS);
            }
            else
            {
                things_erase (things, NUM_THINGS);
                things_create (things, NUM_THINGS);
            }
        }
    }
    return 0;
}

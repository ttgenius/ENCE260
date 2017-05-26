#include <stdlib.h>
#include "things.h"
#include "tinygl.h"


/* Number of initial objects.  */
#define THINGS_NUM 6

#define SAFE_DISTANCE 3

#define ANXIETY_LEVEL 2

#define PANIC_LEVEL 50

#define MOVE_ATTEMPTS 12


typedef struct thing_struct
{
    tinygl_point_t pos;
    uint8_t anxiety;
    bool alive;
} thing_t;


static thing_t things[THINGS_NUM];


bool things_killed_p (void)
{
    uint8_t i;

    for (i = 1; i < THINGS_NUM; i++)
    {
        if (things[i].alive)
            return 0;
    }
    return 1;
}


static int8_t thing_find (tinygl_point_t pos)
{
    uint8_t i;

    for (i = 0; i < THINGS_NUM; i++)
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


static bool thing_inbounds_p (tinygl_point_t pos)
{
    return pos.x >= 0 && pos.x < TINYGL_WIDTH
        && pos.y >= 0 && pos.y < TINYGL_HEIGHT;    
}


static bool thing_position_valid_p (tinygl_point_t pos)
{
    return thing_inbounds_p (pos) && thing_find (pos) == -1;
}


void things_monster_move (int8_t dx, int8_t dy)
{
    int8_t other;
    tinygl_point_t newpos;

    newpos = tinygl_point (things[0].pos.x + dx, things[0].pos.y + dy);
    if (!thing_inbounds_p (newpos))
        return;

    other = thing_find (newpos);
    if (other != -1)
        things[other].alive = 0;

    tinygl_draw_point (things[0].pos, 0);
    things[0].pos = newpos;
    tinygl_draw_point (newpos, 1);
}


void things_monster_toggle (void)
{
    tinygl_pixel_set (things[0].pos, ! tinygl_pixel_get (things[0].pos));
}



static void thing_move (uint8_t this)
{
    uint8_t i;
    uint8_t distsq;
    uint8_t panic;
    tinygl_point_t pos;

    tinygl_draw_point (things[this].pos, 0);

    distsq = thing_distance_squared (things, this);
    pos = things[this].pos;

    if (thing_distance_squared (things, this) <= SAFE_DISTANCE * SAFE_DISTANCE)
        things[this].anxiety++;
    else
        things[this].anxiety = 0;

    panic = things[this].anxiety > ANXIETY_LEVEL 
        && (rand () % 100 > (100 - PANIC_LEVEL));

    for (i = 0; i < MOVE_ATTEMPTS; i++)
    {
        int8_t dx;
        int8_t dy;
        tinygl_point_t newpos;
        
        dx = (rand () % 3) - 1;
        dy = (rand () % 3) - 1;

        newpos = tinygl_point (pos.x + dx, pos.y + dy);
    
        if ((dx || dy) && ! thing_position_valid_p (newpos))
            continue;

        things[this].pos = newpos;

        if (panic || thing_distance_squared (things, this) > distsq)
            break;
    }

    /* Stay put if cannot find better position.  */
    if (i == MOVE_ATTEMPTS)
        things[this].pos = pos;
        
    tinygl_draw_point (things[this].pos, 1);
}


void things_move (void)
{
    uint8_t i;

    for (i = 1; i < THINGS_NUM; i++)
        if (things[i].alive)
            thing_move (i);
}


void things_create (void)
{
    uint8_t i;

    things[0].pos.x = 0;
    things[0].pos.y = 0;
    things[0].alive = 1;
    tinygl_draw_point (things[0].pos, 1);

    for (i = 1; i < THINGS_NUM; i++)
    {
        uint8_t x;
        uint8_t y;

        do
        {
            x = rand () % TINYGL_WIDTH;
            y = rand () % TINYGL_HEIGHT;
        } while (thing_find (tinygl_point (x, y)) != -1);
        
        things[i].pos.x = x;
        things[i].pos.y = y;
        things[i].alive = 1;

        tinygl_draw_point (things[i].pos, 1);
    }
}


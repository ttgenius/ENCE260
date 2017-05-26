/** @file   gobble1.c
    @author M.P. Hayes
    @date   28 Sep 2013
    @brief 
*/

#include <stdlib.h>
#include "system.h"
#include "pacer.h"
#include "tinygl.h"

/* Number of initial objects.  */
#define NUM_MONSTERS 6

#define LOOP_RATE 300

#define MOVE_RATE 10


typedef struct monster_struct
{
    tinygl_point_t pos;
    bool alive;
} monster_t;


static int8_t monster_find (monster_t *monsters, uint8_t num,
                                   uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < num; i++)
    {
        if (monsters[i].pos.x == x
            && monsters[i].pos.y == y
            && monsters[i].alive)
            return i;
    }
    return -1;
}


static int8_t monster_choose (monster_t *monsters, uint8_t num)
{
    while (1)
    {
        int8_t this;

        this = rand () % num;
        
        if (monsters[this].alive)
            return this;
    }
}


static void monster_move (monster_t *monsters, uint8_t num)
{
    uint8_t x;
    uint8_t y;
    uint8_t this;

    this = monster_choose (monsters, num);

    tinygl_draw_point (monsters[this].pos, 0);

    x = monsters[this].pos.x;
    y = monsters[this].pos.y;

    while (1)
    {
        int8_t dx;
        int8_t dy;
        
        dx = (rand () % 3) - 1;
        dy = (rand () % 3) - 1;

        if ((dx || dy)
            && x + dx >= 0 && x + dx < TINYGL_WIDTH
            && y + dy >= 0 && y + dy < TINYGL_HEIGHT)
        {
            int8_t other;
            
            other = monster_find (monsters, num, x + dx, y + dy);

            if (other != -1)
                monsters[other].alive = 0;

            monsters[this].pos.x = x + dx;
            monsters[this].pos.y = y + dy;
            tinygl_draw_point (monsters[this].pos, 1);
            return;
        }
    }
}


static void monsters_create (monster_t *monsters, uint8_t num)
{
    uint8_t i;

    for (i = 0; i < num; i++)
    {
        uint8_t x;
        uint8_t y;

        do
        {
            x = rand () % TINYGL_WIDTH;
            y = rand () % TINYGL_HEIGHT;
        } while (monster_find (monsters, i, x, y) != -1);
        
        monsters[i].pos.x = x;
        monsters[i].pos.y = y;
        monsters[i].alive = 1;

        tinygl_draw_point (things[i].pos, 1);
    }
}


int main (void)
{
    uint16_t tick = 0;
    monster_t monsters[NUM_MONSTERS];

    system_init ();

    tinygl_init (LOOP_RATE);

    pacer_init (LOOP_RATE);

    monsters_create (monsters, NUM_MONSTERS);

    while (1)
    {
        uint8_t col;

        /* Refresh monsters.  */
        for (col = 0; col < TINYGL_WIDTH; col++)
        {
            pacer_wait ();

            tinygl_update ();
        }

        tick++;
        if (tick >= LOOP_RATE / MOVE_RATE)
        {
            tick = 0;

            monster_move (monsters, NUM_MONSTERS);
        }
    }
    return 0;
}

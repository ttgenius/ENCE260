/** @file   snake1.c
    @author M.P. Hayes
    @date   5 Oct 2010
    @note   The snake head deliberately moves off screen unless turned.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

#define SNAKE_SPEED 2

enum dir {DIR_N, DIR_E, DIR_S, DIR_W};

typedef enum dir dir_t;

struct snake 
{
    /* Current head of snake.  */
    tinygl_point_t pos;
    /* Current direction.  */
    enum dir dir;
};

typedef struct snake snake_t;


static snake_t snake_move (snake_t snake)
{
    switch (snake.dir)
    {
        case DIR_N:
            snake.pos.y = snake.pos.y - 1;
            break;

        case DIR_E:
            snake.pos.x = snake.pos.x + 1;
            break;

        case DIR_S:
            snake.pos.y = snake.pos.y + 1;
            break;

        case DIR_W:
            snake.pos.x = snake.pos.x - 1;
            break;
    }
    tinygl_draw_point (snake.pos, 1);
    return snake;
}


static snake_t snake_turn_left (snake_t snake)
{
    dir_t newdir[] = {DIR_W, DIR_N, DIR_E, DIR_S};

    snake.dir = newdir[snake.dir];
    return snake;
}


static snake_t snake_turn_right (snake_t snake)
{
    dir_t newdir[] = {DIR_E, DIR_S, DIR_W, DIR_N};

    snake.dir = newdir[snake.dir];
    return snake;
}


int main (void)
{
    snake_t snake;
    int tick = 0;

    system_init ();

    snake.dir = DIR_N;
    snake.pos.x = TINYGL_WIDTH / 2;
    snake.pos.y = TINYGL_HEIGHT - 1;

    tinygl_init (LOOP_RATE);

    navswitch_init ();

    pacer_init (LOOP_RATE);

    tinygl_draw_point (snake.pos, 1);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        navswitch_update ();

        tick = tick + 1;
        if (tick > LOOP_RATE / SNAKE_SPEED)
        {   
            tick = 0;
            snake = snake_move (snake);
        }

        if (navswitch_push_event_p (NAVSWITCH_WEST))
        {
            snake = snake_turn_left (snake);
        }

        if (navswitch_push_event_p (NAVSWITCH_EAST))
        {
            snake = snake_turn_right (snake);
        }

  
        tinygl_update ();
    }

    return 0;
}

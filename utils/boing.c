/** @file   boing.c
    @author M. P. Hayes, UCECE
    @date   24 August 2010
    @brief  Bouncing ball module
*/
#include "system.h"
#include "boing.h"

/** Update the state of a ball bouncing off the edges of the display
    @param state current state
    @return new state.  */
boing_state_t boing_update (boing_state_t state)
{
    tinygl_point_t hops[] = {{0, 1}, {1, 1}, {1, 0}, {1, -1},
                             {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

    state.pos.x += hops[state.dir].x;
    state.pos.y += hops[state.dir].y;
    
    if (state.pos.x > TINYGL_WIDTH - 1 || state.pos.x < 0)
    {
        boing_dir_t newdir[] = {DIR_N, DIR_NW, DIR_W, DIR_SW,
                                DIR_S, DIR_SE, DIR_E, DIR_NE};
        state.pos.x -= 2 * hops[state.dir].x;
        state.dir = newdir[state.dir];
    }
    
    if (state.pos.y > TINYGL_HEIGHT - 1 || state.pos.y < 0)
    {
        boing_dir_t newdir[] = {DIR_S, DIR_SE, DIR_E, DIR_NE,
                                DIR_N, DIR_NW, DIR_W, DIR_SW};
        state.pos.y -= 2 * hops[state.dir].y;
        state.dir = newdir[state.dir];
    }    

    return state;
}


/** Reverse the direction of a ball
    @param state current state
    @return new state with direction reversed.  */
boing_state_t boing_reverse (boing_state_t state)
{
    boing_dir_t newdir[] = {DIR_S, DIR_SW, DIR_W, DIR_NW,
                            DIR_N, DIR_NE, DIR_E, DIR_SE};    

    state.dir = newdir[state.dir];
    return state;
}


/** Initialise the state of a ball
    @param xstart x coordinate to start at
    @param ystart y coordinate to start at
    @param dir initial direction
    @return state.  */
boing_state_t boing_init (uint8_t xstart, uint8_t ystart, boing_dir_t dir)
{
    boing_state_t state;

    if (xstart > TINYGL_WIDTH)
        xstart = TINYGL_WIDTH - 1;
    if (ystart > TINYGL_HEIGHT)
        ystart = TINYGL_HEIGHT - 1;

    state.pos.x = xstart;
    state.pos.y = ystart;
    state.dir = dir;

    return state;
}

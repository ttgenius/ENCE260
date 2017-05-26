/** @file   boing.h
    @author M. P. Hayes, UCECE
    @date   24 August 2010
    @brief  Bouncing ball module

    @defgroup boing Bouncing ball module
*/
#ifndef BOING_H
#define BOING_H

#include "tinygl.h"

/** Compass direction of ball.  */
typedef enum dir {DIR_N, DIR_NE, DIR_E, DIR_SE,
                  DIR_S, DIR_SW, DIR_W, DIR_NW} boing_dir_t;


/** Structure defining state of ball.  */
typedef struct state {tinygl_point_t pos; boing_dir_t dir;} boing_state_t;


/** Update the state of a ball bouncing off the edges of the display
    @param state current state
    @return new state.  */
boing_state_t boing_update (boing_state_t state);


/** Reverse the direction of a ball
    @param state current state
    @return new state with direction reversed.  */
boing_state_t boing_reverse (boing_state_t state);


/** Initialise the state of a ball
    @param xstart x coordinate to start at
    @param ystart y coordinate to start at
    @param dir initial direction
    @return state.  */
boing_state_t boing_init (uint8_t xstart, uint8_t ystart, boing_dir_t dir);


#endif

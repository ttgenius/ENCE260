/** @file   chef.c
    @author Melody Zhu (yzh231) Team78
    @date   _____ October 2016
    @brief  Chef module for the game.c
*/

#ifndef CHEF_H
#define CHEF_H

/* Course-provided modules. */
#include "system.h"
#include "tinygl.h"
#include "ir_uart.h"

/* Student-created modules. */
#include "compass_directions.h"

/* Define coordinates for chef's starting position */
#define CHEF_START_X_POS 2
#define CHEF_START_Y_POS 3


/* Chef struct indicating chef's position */
typedef struct 
{
	uint8_t x_pos;
	uint8_t y_pos;
} chef_t;
	

/** Chef's movement */
void chef_move (chef_t *player, int8_t direction);


void chef_move_east (chef_t *player);


void chef_move_west (chef_t *player);


void chef_move_north (chef_t *player);


void chef_move_south (chef_t *player);


void chef_drop_food (chef_t *chef);


/** Chef receives game over signal from snake */
int8_t chef_receive_signal (void);


#endif

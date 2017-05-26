/** @file   snake.h
    @author Amanda Deacon (asd35) - structs, and most movement functions.
			Melody Zhu (yzh231) - IR functionality, and code in 
			snake_new_head.
			
			Team 78
			
    @date   12 October 2016
    @brief  This is the interface to the snake module.

*/


#ifndef SNAKE_H
#define SNAKE_H


/* Course-provided modules. */
#include <stdlib.h>
#include "system.h"
#include "tinygl.h"
#include "ir_uart.h"


/* Student-created modules. */
#include "compass_directions.h"


/* Define snake starting coordinates and direction. */
#define SNAKE_START_X_POS 2
#define SNAKE_START_Y_POS 3
#define SNAKE_START_DIRECTION NORTH


/* The SnakeSegment struct. Each SnakeSegment represents one "pixel"
 * of the snake, and holds a pointer to the next segment. */
typedef struct snake_segment_s SnakeSegment;

struct snake_segment_s 
{
	int8_t x_pos;
	int8_t y_pos;
	SnakeSegment* next;
};


/* The Snake struct. A Snake is composed of a linked list of 
 * SnakeSegments. */
typedef struct snake_s Snake;

struct snake_s 
{
	// This is a pointer to the first SnakeSegment of the snake.
	SnakeSegment* head;
	
	// This indicates the number of consumed food which has yet to 
	// be added to the snake's tail.
	int8_t num_food_waiting;
	
	// This stores the total number of food consumed.
	uint8_t score;
	
	// This stores the direction the head of the snake is facing.
	int8_t direction;
};


/* Given a pointer to a Snake struct, create new head and tail segments,
 * and set the Snake's initial information (location, direction, score).
 * 
 * For use the first time a snake is created at the start of a 
 * session of games. If re-setting the snake in-between games, please 
 * use snake_reset instead (which itself will call this function). */
void snake_initialize (Snake* snake);


/* Given a pointer to a Snake, free all SnakeSegments, and initialize
 * the Snake.
 * 
 * For use when re-setting the snake in-between games. */
void snake_reset (Snake* snake);


/* Set a Snake's direction to be the given direction. */
void snake_set_direction (Snake* snake, int8_t direction);


/* Move the given Snake forward by one "step", in the direction it is 
 * currently facing. 
 * 
 * This involves moving the head forwards, as well as retracting the
 * tail if needed. The tail will not be retracted if the Snake has 
 * recently eaten food which has yet to be incorporated into the Snake's 
 * length. */
void snake_move (Snake* snake, int8_t food_positions[][TINYGL_HEIGHT]);


/* Consume food from given food array if any food is present at location 
 * of snake head. Do nothing otherwise. */
void snake_consume_food (Snake* snake, int8_t food_positions[][TINYGL_HEIGHT]);


/* Return 1 if snake head has collided with some part of itself, and
 * 0 otherwise. Note that it is not possible for a snake of length two
 * to collide with itself. */
int8_t snake_has_collided (Snake* snake);


/* Melody's function. Originally in chef module. This function takes a
 * food array of 35 '0's and changes 0 to 1 if food is dropped at that 
 * position. */
void snake_receive_food_pos (int8_t food_positions[TINYGL_WIDTH][TINYGL_HEIGHT]);


/* Transmit the given gameover signal using the ir_uart module. */
void snake_send_gameover_msg(int8_t signal);


#endif

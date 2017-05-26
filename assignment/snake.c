/** @file   snake.c
    @author Amanda Deacon (asd35) - structs, and most movement functions.
			Melody Zhu (yzh231) - IR functionality, and code in 
			snake_new_head.
			
			Team 78
				
    @date   12 October 2016
    @brief  This implements the snake module.

*/


/* Course-provided modules. */
#include <stdlib.h>
#include "system.h"
#include "tinygl.h"
#include "ir_uart.h"


/* Student-created modules. */
#include "snake.h"
#include "compass_directions.h"


/* Given a pointer to a Snake struct, create new head and tail segments,
 * and set the Snake's initial information (location, direction, score).
 * 
 * For use the first time a snake is created at the start of a 
 * session of games. If re-setting the snake in-between games, please 
 * use snake_reset instead (which itself will call this function). */
void snake_initialize (Snake* snake)
{
	SnakeSegment* new_head = malloc(sizeof(SnakeSegment));
	SnakeSegment* new_tail= malloc(sizeof(SnakeSegment));
	
	new_head->x_pos = SNAKE_START_X_POS;
	new_head->y_pos = SNAKE_START_Y_POS;
	new_head->next = new_tail;
	
	new_tail->x_pos = SNAKE_START_X_POS;
	new_tail->y_pos = SNAKE_START_Y_POS + 1;
	new_tail->next= NULL;
	
	snake->head = new_head;
	snake->num_food_waiting = 0;
	snake->score = 0;
	snake->direction = SNAKE_START_DIRECTION;
}


/* Given a pointer to a Snake, free all SnakeSegments, and initialize
 * the Snake.
 * 
 * For use when re-setting the snake in-between games. */
void snake_reset (Snake* snake)
{
	SnakeSegment* current = snake->head;
	SnakeSegment* prev = current;
	
	// Free the linked list of SnakeSegments.
	while (current) {
		
		prev = current;
		current = current->next;
		free(prev);
	}
	
	snake_initialize (snake);
}


/* Set a Snake's direction to be the given direction. */
void snake_set_direction (Snake* snake, int8_t direction)
{
	snake->direction = direction;
}


/* Create and return a pointer to a snake segment corresponding to 
 * the snake head's next upcoming location. */
static SnakeSegment* snake_new_head (Snake* snake)
{
	SnakeSegment* new_head = malloc(sizeof(SnakeSegment));
	SnakeSegment* old_head = snake->head;
	
	int8_t x_pos = old_head->x_pos;
	int8_t y_pos = old_head->y_pos;
	
	switch (snake->direction) {
		
		case NORTH:
			y_pos = (y_pos - 1 + TINYGL_HEIGHT) % TINYGL_HEIGHT;
			break;
		
		case EAST:
			x_pos = (x_pos + 1) % TINYGL_WIDTH;
			break;
			
		case SOUTH:
			y_pos = (y_pos + 1) % TINYGL_HEIGHT;
			break;
		
		case WEST:
			x_pos = (x_pos - 1 + TINYGL_WIDTH) % TINYGL_WIDTH;
			break;
	}
	
	new_head->x_pos= x_pos;
	new_head->y_pos = y_pos;
	
	return new_head;
}
	

/* Insert a new snake head at the start of the linked list of 
 * SnakeSegments. This represents the Snake's next "step" forward. */
static void advance_snake_head (Snake* snake)
{
	SnakeSegment* new_head = snake_new_head (snake);
	
	new_head->next = snake->head;
	snake->head = new_head;
}


/* Remove a SnakeSegment from the end of the linked list of 
 * SnakeSegments. This represents the Snake's "retracting" its "tail"
 * as it moves onward in its quest for more noms. */
static void snake_retract_tail (Snake* snake)
{
	SnakeSegment* current = snake->head;
	SnakeSegment* prev = current;
	
	while (current->next) {
		prev = current;
		current = current->next;
	}
	
	prev->next = NULL;
	free(current);
}


/* Move the given Snake forward by one "step", in the direction it is 
 * currently facing. This involves moving the head forwards, as well as 
 * retracting the tail if needed.
 * 
 * The tail will not be retracted if there is food waiting to be added 
 * i.e. if the Snake has consumed food which has yet to contribute to 
 * the Snake's length, in the form of another SnakeSegment. */
void snake_move (Snake* snake, int8_t food_positions[][TINYGL_HEIGHT])
{
	advance_snake_head (snake);
	
	if (snake->num_food_waiting > 0) {
		snake->num_food_waiting -= 1;
	} else {
		snake_retract_tail (snake);
	}
	
	snake_consume_food (snake, food_positions);
}


/* Consume food from given food array if any food is present at location 
 * of snake head. Do nothing otherwise. */
void snake_consume_food (Snake* snake, int8_t food_positions[][TINYGL_HEIGHT])
{
	int8_t x_pos = snake->head->x_pos;
	int8_t y_pos = snake->head->y_pos;
	
	if (food_positions[x_pos][y_pos]) {
		food_positions[x_pos][y_pos] = 0;
		snake->num_food_waiting += 1;
		snake->score += 1;
	}
}


/* Return 1 if snake head has collided with some part of itself, and
 * 0 otherwise. Note that it is not possible for a snake of length two
 * to collide with itself. */
int8_t snake_has_collided (Snake* snake)
{
	int8_t x_pos = snake->head->x_pos;
	int8_t y_pos = snake->head->y_pos;
	
	SnakeSegment* current = snake->head->next;	// second segment
	
	while (current) {
		if (current->x_pos == x_pos && current->y_pos == y_pos) {
			// collision detected 
			return 1;
		}
		current = current->next;
	}
	
	return 0;
}


/* Melody's function. Originally in chef module. This function takes a
 * food array of 35 '0's and changes 0 to 1 if food is dropped at that 
 * position. */
void snake_receive_food_pos (int8_t food_positions[TINYGL_WIDTH][TINYGL_HEIGHT])
{
	uint8_t food_pos;
	uint8_t col_x;
	uint8_t row_y;

	if (ir_uart_read_ready_p ()) {
		
		food_pos = TINYGL_WIDTH * TINYGL_HEIGHT - ir_uart_getc ();
		row_y = food_pos / TINYGL_WIDTH;          //decode position. ie.28 is (3,6) but if tinygl starts from 0 then 28 is (2,5)
		col_x = food_pos % TINYGL_WIDTH;
		food_positions[col_x][row_y] = 1; //this is the point to display on mouse's board.
	}	                                  //1 means there's food at this point but hasn't been eaten.  
}                                         //0 means there's no food at the point
		                           

/* Transmit the given gameover signal using the ir_uart module. */
void snake_send_gameover_msg (int8_t signal)
{
     ir_uart_putc(signal);
}


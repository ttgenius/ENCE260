/** @file   game_display.c
    @author Amanda Deacon (asd35) - Team 78
    @date   05 October 2016
    @brief  A game display module for the Mouse & Chef game. Takes 
			information about the positions of players and food, and 
			displays this to the LEDMAT. Makes heavy use of the tinygl
			module.
*/

#include "tinygl.h"
#include "chef.h"
#include "snake.h"
#include "game_display.h"


/* A handler to draw points to the LEDMAT using the tinygl module.
 * Inspired by content in ../../apps/space12. */
static void draw_point_handler (int8_t i, int8_t j, tinygl_pixel_value_t value)
{
	tinygl_point_t point = {i, j};
	tinygl_draw_point (point, value);
}


/* External use. Display the snake position as well as the position of 
 * any present food to the LEDMAT. */
void draw_p1_game_display (Snake* snake, int8_t food_positions[][TINYGL_HEIGHT])
{
	SnakeSegment* current = snake->head;
	int8_t i;
	int8_t j;
	
	tinygl_clear ();
	
	// Display each snake segment
	while (current) {
		draw_point_handler (current->x_pos, current->y_pos, LED_ON);
		current = current->next;
	}
	
	// Display any present food
	for (i = 0; i < TINYGL_WIDTH; i++) {
		for (j = 0; j < TINYGL_HEIGHT; j++) {
			if (food_positions[i][j] == 1) {
				draw_point_handler (i, j, LED_ON);
			}
		}
	}	
}


/* External use. Display the chef position to the LEDMAT. */
void draw_p2_game_display (chef_t* chef)
{
	tinygl_clear ();
	draw_point_handler (chef->x_pos, chef->y_pos, LED_ON);
}

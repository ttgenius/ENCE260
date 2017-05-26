/** @file   chef.c
    @author Melody Zhu (yzh231) Team78
    @date   _____ October 2016
    @brief  Chef module for game.c
*/


/* Course-provided modules */
#include "tinygl.h"
#include "ir_uart.h"

/* Student-created modules */
#include "compass_directions.h"
#include "chef.h"


/* Chef module for game.c. Choose Player 2 to be Chef.
 * Chef can move in 4 directions and drop the food by using
 * the naviswith.  When chef drops the food, her position will 
 * appear as the food's position on snake's board.
 */


/* Chef checks naviswitch direction and move */
void chef_move (chef_t *player, int8_t direction)
{
	if (direction == NORTH) {
		chef_move_north (player);
	} else if (direction == EAST) {
		chef_move_east (player);
	} else if (direction == SOUTH) {
		chef_move_south(player);
	} else if (direction == WEST) {
		chef_move_west (player);
	}
}


/* Chef's movement */
void chef_move_east (chef_t *player)
{
	player->x_pos = (player->x_pos + 1) % (TINYGL_WIDTH);
}


void chef_move_west (chef_t *player)
{
	player->x_pos = ((player->x_pos - 1) + TINYGL_WIDTH) % TINYGL_WIDTH;
}


void chef_move_north (chef_t *player)
{
	player->y_pos = ((player->y_pos - 1) + TINYGL_HEIGHT) % TINYGL_HEIGHT;
}


void chef_move_south (chef_t *player)
{
	player->y_pos = (player->y_pos + 1) % TINYGL_HEIGHT;
}	


/* Transmit chef's position to snake.
 * Chef's position is the food's postion appearing on snake's board.
 */ 
void chef_drop_food (chef_t *chef)
{
	uint8_t pos;
	
	//encode chef's position into a number between 1 and 35 
	//ie.y=5, x=2 (row 6,col 3), which is 5 * 5 + (2 + 1) = 28 
	pos = (chef->y_pos) * TINYGL_WIDTH + (chef->x_pos + 1);
	
	//transmit chef's (food's) position to snake
	ir_uart_putc (pos);											     
	
}

/* Chef to receive gameover signal from snake */
int8_t chef_receive_signal (void)
{
	int8_t signal;
	
	if (ir_uart_read_ready_p ()) {
		signal = ir_uart_getc ();
		return signal;
	} else {
		return 0;
	}
}		
	


		
	
		  
			
	



/** @file   game.c
    @author Amanda Deacon (asd35) & Melody Zhu (yzh231) Team78
    @date    October 2016
    @brief  A Snake & Chef game for the ENCE260 assignmnent.
    
			Snake & Chef is an asymmetric two-player twist on the 
			classic arcade game Snake. 
			 
			Player One plays as the Snake, and moves around the board 
			using the navswitch to gather food and grow happy and long.
			Player Two plays as the Chef, and moves around their own 
			board, pushing down on the navswitch to drop food in the 
			corresponding location of the Snake's board.

*/


/* Course-provided modules. */
#include "system.h"
#include "button.h"
#include "task.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "timer0.h"
#include "pio.h"
#include "delay.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "uint8toa.h" //for converting score
#include <string.h>

/* Student-created modules. */
#include "chef.h"
#include "snake.h"
#include "compass_directions.h"
#include "game_display.h"


/* Define polling rates for the task scheduler. All values in Hz. */
#define DISPLAY_TASK_RATE 300
#define NAVSWITCH_TASK_RATE 100
#define NETWORK_TASK_RATE 20
#define SNAKE_ADVANCE_TASK_RATE 3


/* The signal sent from Player One to Player Two upon snake death. */
#define GAMEOVER_SIGNAL 45


/* Enumerate game states. */
typedef enum {STATE_INIT, STATE_READY, STATE_START, 
		      STATE_P1_WAITING, STATE_P2_WAITING, 
		      STATE_P1_PLAYING, STATE_P2_PLAYING, 
		      STATE_P1_GAMEOVER, STATE_P2_GAMEOVER} state_t;


static state_t state = STATE_INIT;


/* Create players. A single microcontroller initializes structs for both
 * player1 / player2 roles. It will only use one per game, but may use 
 * both over the course of a multi-game session. */
chef_t chef = {CHEF_START_X_POS, CHEF_START_Y_POS};
Snake snake = {NULL, 0, 0, SNAKE_START_DIRECTION};


/* Initialize an array of food position information. Begins empty. */
int8_t food_positions[TINYGL_WIDTH][TINYGL_HEIGHT] = {{0}};


/* Initialize the display task. */
static void display_task_init (void)
{
	tinygl_init (DISPLAY_TASK_RATE);
	tinygl_font_set (&font5x7_1);
	tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
	tinygl_text_speed_set (15);
}


/* Load start menu text into tinygl. */
static void start_menu_display (void)
{
	tinygl_clear ();
    tinygl_text ("P1:PUSH UP. P2:PUSH DOWN.");
}


/* Display game over message including the score, from space12.c */
static void p1_gameover_message (char *buffer)
{
    char *str = buffer;

    strcpy (str, "GAME OVER. SCORE=");

    while (*str)
        str++;

    uint8toa (snake.score, str, 0);
    tinygl_clear ();
    tinygl_text (buffer);
}


/* Player2 displays game over message */
static void p2_gameover_message (void)
{
	tinygl_clear();
	tinygl_text("SNAKE DIED!");
	tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
}


/* Initialize game. */
static void game_init (void)
{
	state = STATE_READY;
	start_menu_display ();
	snake_initialize (&snake);
}


/* Clear food left in the given food array for restarting the game */
static void clear_food (int8_t food_positions[][TINYGL_HEIGHT]){

	int8_t i;
	int8_t j;
	
	for (i = 0; i < TINYGL_WIDTH; i++){
		for (j=0; j < TINYGL_HEIGHT; j++){
			food_positions[i][j] = 0;
		}
	}
}			


/* The display task:
 * --- For player 1 (snake), draw snake and food.
 * --- For player 2 (chef) draw just the chef. */
static void display_task (__unused__ void *data)
{
	switch (state) {
		
		case STATE_P1_PLAYING:
			draw_p1_game_display (&snake,food_positions);
			break;
			
		case STATE_P2_PLAYING:
			draw_p2_game_display (&chef);
			break;
			
		default:
			break;
	}
	
	tinygl_update ();
}


/* The navswitch task. Check for navswitch events, and update game 
 * states and player positions. */
static void navswitch_task (__unused__ void *data)
{
    navswitch_update ();
        
    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {

        switch (state) {
			
			case STATE_P1_WAITING:
				state = STATE_P1_PLAYING;
				tinygl_text ("");
				break;
				
			case STATE_P2_WAITING:
				state = STATE_P2_PLAYING;
				tinygl_text ("");
				break;
				
			case STATE_P1_PLAYING:
			    break;
			
			case STATE_P2_PLAYING:
			    chef_drop_food (&chef);
			    break;
			
			case STATE_P1_GAMEOVER:
			    clear_food (food_positions);
			    start_menu_display();
			    state = STATE_READY;
			    break;
			
			case STATE_P2_GAMEOVER:
			    start_menu_display();
			    state = STATE_READY;
			    break;  
			     
				
			default:
				break;
        }
    }
    
    if (navswitch_push_event_p (NAVSWITCH_NORTH)) {

        switch (state) {
			
			case STATE_READY:
				tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
				// deliberate fall through to next case.
			case STATE_P2_WAITING:
				state = STATE_P1_WAITING;
				tinygl_text ("1");
				break;
				
			case STATE_P1_PLAYING:
				snake_set_direction (&snake, NORTH);
				break;
				
			case STATE_P2_PLAYING:
				chef_move (&chef, NORTH);
				break;
				
			default:
				break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {

        switch (state) {
			
			case STATE_READY:
				tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
				// deliberate fall through to next case.
			case STATE_P1_WAITING:
				state = STATE_P2_WAITING;
				tinygl_text ("2");
				break;
				
			case STATE_P1_PLAYING:
				snake_set_direction (&snake, SOUTH);
				break;
			
			case STATE_P2_PLAYING:
			    chef_move (&chef, SOUTH);
				break;
				
			default:
				break;
        }
    }
    
    if (navswitch_push_event_p (NAVSWITCH_EAST)) {
		
        switch (state) {
			
			case STATE_READY:
				break;
				
			case STATE_P1_PLAYING:
				snake_set_direction (&snake, EAST);
				break;
				
			case STATE_P2_PLAYING:
				chef_move (&chef, EAST);
				break;
				
			default:
				break;
        }
    }

    if (navswitch_push_event_p (NAVSWITCH_WEST)) { 

        switch (state) {
			
			case STATE_READY:
				break;
			
			case STATE_P1_PLAYING:
				snake_set_direction (&snake, WEST);
				
				break;
				
			case STATE_P2_PLAYING:
				chef_move (&chef, WEST);
				break;
				
			default:
				break;
        }
    }
}


/* The network task. When player 1 is playing, call 
 * snake_receive_food_pos to check for transmission from player 2. */
static void network_task (__unused__ void *data)
{
	switch (state) {
		
		case STATE_P1_PLAYING:
			snake_receive_food_pos (food_positions);
			break;
			
		case STATE_P1_GAMEOVER:
		    snake_send_gameover_msg (GAMEOVER_SIGNAL);
		    break;
		    
		case STATE_P2_PLAYING:;
		    int8_t signal = chef_receive_signal();
		    
		    if (signal == GAMEOVER_SIGNAL){
				state = STATE_P2_GAMEOVER;
				p2_gameover_message ();
			}
			
			break;	
		        
		default:
			break;
	}
}


/* The snake task. Move, eat food and reset after game over */
static void snake_advance_task (__unused__ void *data)
{   
	char message[44];

	if (state == STATE_P1_PLAYING) {
		snake_move (&snake, food_positions);
		
		if (snake_has_collided (&snake)) {
			
			state = STATE_P1_GAMEOVER;
			tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
			p1_gameover_message (message);
			snake_reset (&snake);
		}
	}
}


	
int main (void)
{
	task_t tasks[] = 
	{
		{.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE},
		{.func = navswitch_task, .period = TASK_RATE / NAVSWITCH_TASK_RATE},
		{.func = network_task, .period = TASK_RATE / NETWORK_TASK_RATE},
		{.func = snake_advance_task, .period= TASK_RATE / SNAKE_ADVANCE_TASK_RATE}
	};

    system_init ();
    navswitch_init ();
	ir_uart_init ();
    
    display_task_init ();
    game_init ();
    
    task_schedule (tasks, 4);
    
    return 0;
}

/** @file   game_display.h
    @author Amanda Deacon (asd35) - Team 78
    @date   05 October 2016
    @brief  A game display module for the Mouse & Chef game. Takes 
			information about the positions of players and food, and 
			displays this to the LEDMAT. Makes heavy use of the tinygl
			module.
*/

#ifndef GAME_DISPLAY_H
#define GAME_DISPLAY_H


#include "tinygl.h"
#include "chef.h"
#include "snake.h"


/* This enumeration is for internal and external use, to improve 
 * readability of code which changes LED on/off status. */
enum {LED_OFF, LED_ON};


/* External use. Display the snake position as well as the position of 
 * any present food to the LEDMAT. */
void draw_p1_game_display (Snake* snake, int8_t food_positions[][TINYGL_HEIGHT]);


/* External use. Display the chef position to the LEDMAT. */
void draw_p2_game_display (chef_t* chef);


#endif

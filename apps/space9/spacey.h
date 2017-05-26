/** @file   spacey.h
    @author M. P. Hayes, UCECE
    @date   13 March 2007
    @brief  This is the interface for a simple space invaders game.
*/

#ifndef SPACEY_H
#define SPACEY_H

#include "system.h"


enum {SPACEY_ALIENS_MAX = 4, SPACEY_SHELLS_MAX = 4};


typedef enum {SPACEY_PIX_OFF, SPACEY_PIX_GUN, SPACEY_PIX_ALIEN,
	      SPACEY_PIX_SHELL, SPACEY_PIX_TYPE_NUM} spacey_pix_t;

typedef enum {SPACEY_EVENT_ALIEN_HIT, SPACEY_EVENT_ALIEN_LANDED} spacey_event_t;


typedef enum {SPACEY_ALIEN, SPACEY_SHELL} spacey_type_t;

/* Positions must be signed.  */
typedef struct
{
    int8_t x;
    int8_t y;
} spacey_pos_t;


typedef struct
{
    spacey_pos_t pos;
    bool active;
} spacey_obj_t;


typedef struct
{
    spacey_obj_t array[SPACEY_ALIENS_MAX];
    uint8_t num;
    uint16_t move_period;
    uint16_t move_clock;
} spacey_objs_t;


typedef struct
{
    uint8_t aliens_killed;
    uint8_t aliens_landed;
    uint8_t shells_fired;
    uint8_t aliens_live;
} spacey_stats_t;


typedef struct
{
    int aliens_wrap:1;
    int aliens_evasive:1;
} spacey_options_t;


typedef struct
{
    /* If we have lots of objects then a linked list would be more
       efficient for the aliens and shells.  */
    spacey_objs_t aliens;
    spacey_objs_t shells;
    spacey_pos_t gun;
    spacey_pos_t size;
    spacey_stats_t stats;
    uint16_t poll_rate;		/* Hz  */
    uint8_t alien_create_steps;
    uint8_t lives;
    void (*display_hook) (void *data, uint8_t row, uint8_t col, spacey_pix_t type);
    void *display_data;
    void (*event_hook) (void *data, spacey_event_t event);
    void *event_data;
    spacey_options_t options;
    bool active;
} spacey_t;


void
spacey_event_handler_set (void (*event_handler) (void *, spacey_event_t),
			  void *event_data);

void
spacey_init (uint16_t poll_rate,
	     uint8_t x_size, uint8_t y_size,
	     void (*display_handler) (void *data, uint8_t row, uint8_t col, 
				      spacey_pix_t type),
	     void *display_data);

/** Start a new game.  */
void
spacey_start (void);

/** Update state of game.  */
bool
spacey_update (void);

/** Specify number of lives we have before we die.  */
void
spacey_lives_set (uint8_t lives);

/** Set alien speed in Hz.  */
void
spacey_alien_speed_set (uint8_t alien_speed);

/** Set shell speed in Hz.  */
void 
spacey_shell_speed_set (uint8_t shell_speed);

/** Set number of steps that a new alien is created on average.  */
void 
spacey_alien_create_steps_set (uint8_t alien_create_steps);

/** Set maximum number of aliens on display at once.  */
void
spacey_alien_num_set (uint8_t aliens_num);

/** Set maximum number of shells on display at once.  */
void
spacey_shell_num_set (uint8_t shells_num);

/** Set game options.  */
void
spacey_options_set (spacey_options_t options);

/** Move gun to right.  */
void 
spacey_gun_move_right (void);

/** Move gun to left.  */
void 
spacey_gun_move_left (void);

/** Fire gun.  */
void
spacey_gun_fire (void);

/** Return number of aliens killed in current game.  */
uint8_t
spacey_aliens_killed_get (void);

/** Return number of shells fired in current game.  */
uint8_t
spacey_shells_fired_get (void);

#endif

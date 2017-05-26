/** @file   gobble7.c
    @author M.P. Hayes
    @date   1 Oct 2013
    @brief  A simple game where a monster has to capture `things' that run away.*/

#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "task.h"
#include "navswitch.h"
#include "led.h"
#include "timer.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "things.h"


#define DISPLAY_TASK_RATE 200

#define NAVSWITCH_TASK_RATE 50

#define THINGS_MOVE_TASK_RATE 10

#define MONSTER_FLASH_TASK_RATE 5


typedef enum game_state
{
    GAME_WAIT,                  /* Waiting to start.  */
    GAME_RUNNING,               /* Things running around.   */
    GAME_PAUSED                 /* Things paused.  */
} game_state_t;


static game_state_t game_state = GAME_WAIT;
static int duration = 0;


static void display_task (__unused__ void *data)
{
    tinygl_update ();
}


static void monster_flash_task (__unused__ void *data)
{
    /* Flash the monster.  */
    if (game_state == GAME_RUNNING)
        things_monster_toggle ();
}


static void things_move_task (__unused__ void *data)
{
    /* Move the things.  */
    if (game_state == GAME_RUNNING)
    {
        duration++;
        things_move ();
    }
}


static void navswitch_task (__unused__ void *data)
{
    navswitch_update ();
    
    if (navswitch_push_event_p (NAVSWITCH_NORTH))
        things_monster_move (0, -1);
    if (navswitch_push_event_p (NAVSWITCH_SOUTH))
        things_monster_move (0, 1);
    if (navswitch_push_event_p (NAVSWITCH_EAST))
        things_monster_move (1, 0);
    if (navswitch_push_event_p (NAVSWITCH_WEST))
        things_monster_move (-1, 0);
    
    /* Pause/resume things running around.  */
    if (navswitch_push_event_p (NAVSWITCH_PUSH))
    {
        switch (game_state)
        {
        case GAME_WAIT:
            game_state = GAME_RUNNING;
            srand (timer_get ());
            tinygl_clear ();
            things_create ();
            duration = 0;
            led_set (LED1, 1);
            break;
            
        case GAME_RUNNING:
            game_state = GAME_PAUSED;
            led_set (LED1, 0);
            break;

        case GAME_PAUSED:
            game_state = GAME_RUNNING;
            led_set (LED1, 1);
            break;
        }
    }
    
    if (game_state == GAME_RUNNING && things_killed_p ())
    {
        char buffer[6];
        
        game_state = GAME_WAIT;
        led_set (LED1, 0);
        sprintf (buffer, "%d", duration);
        tinygl_text (buffer);
    }
}


int main (void)
{
    task_t tasks[] =
    {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = navswitch_task, .period = TASK_RATE / NAVSWITCH_TASK_RATE},
        {.func = things_move_task, .period = TASK_RATE / THINGS_MOVE_TASK_RATE},
        {.func = monster_flash_task, .period = TASK_RATE / MONSTER_FLASH_TASK_RATE},
    };


    system_init ();
    navswitch_init ();
    led_init ();
    led_set (LED1, 0);

    tinygl_init (DISPLAY_TASK_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);

    tinygl_text ("GOBBLE7: PUSH TO START");

    task_schedule (tasks, 4);

    return 0;
}

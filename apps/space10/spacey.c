/** @file   spacey.c
    @author M. P. Hayes, UCECE
    @date   13 March 2007
    @brief  This implements a simple space invaders game.
*/

#include <stdlib.h>
#include "spacey.h"

/* This function implements a simple space invades program.  Aliens
   randomly appear on the top row of the display and work their way
   randomly down the display.  The game ends when 5 aliens have
   reached the bottom of the display.  To kill the aliens, shells can
   be fired at them from a gun on the bottom of the display.  The gun
   can only move across the bottom and when it gets to the side it
   wraps around to the other side.

   The aliens could move at different speeds but currently they are
   set at the same speed to keep things simple.  
*/


#ifndef DISPLAY_INTERLACE
#define DISPLAY_INTERLACE 0
#endif   


#if DISPLAY_INTERLACE
/* With an interlaced display we do not have odd numbered rows and
   columns.  */
enum {SPACEY_SHELL_INC = 2, SPACEY_GUN_INC = 2};
#else
enum {SPACEY_SHELL_INC = 1, SPACEY_GUN_INC = 1};
#endif


enum {SPACEY_LIVES_DEFAULT = 5};
enum {SPACEY_SHELL_SPEED_DEFAULT = 16};
enum {SPACEY_ALIEN_SPEED_DEFAULT = 2};
enum {SPACEY_ALIEN_CREATE_STEPS_DEFAULT = 4};
enum {SPACEY_ALIEN_NUM_DEFAULT = 3};
enum {SPACEY_SHELL_NUM_DEFAULT = 2};


#define min(x, y) ((x) < (y) ? (x) : (y))

static spacey_t spacey_data;
static spacey_t * const spacey = &spacey_data;


void 
spacey_lives_set (uint8_t lives)
{
    spacey->lives = lives;
}


void 
spacey_alien_speed_set (uint8_t alien_speed)
{
    spacey->aliens.move_period = spacey->poll_rate / alien_speed;
}


void 
spacey_shell_speed_set (uint8_t shell_speed)
{
    spacey->shells.move_period = spacey->poll_rate / shell_speed;
}


void 
spacey_alien_create_steps_set (uint8_t alien_create_steps)
{
    spacey->alien_create_steps = alien_create_steps;
}


void
spacey_alien_num_set (uint8_t aliens_num)
{
    spacey->aliens.num = min (aliens_num, SPACEY_ALIENS_MAX);
}


void
spacey_shell_num_set (uint8_t shells_num)
{
    spacey->shells.num = min (shells_num, SPACEY_SHELLS_MAX);
}


void
spacey_options_set (spacey_options_t options)
{
    spacey->options = options;
}


void
spacey_init (uint16_t poll_rate,
             uint8_t x_size, uint8_t y_size,
             void (*display_handler) (void *data, uint8_t row, uint8_t col,
                                      spacey_pix_t type),
             void *display_data)
{
    spacey->size.x = x_size;
    spacey->size.y = y_size;
    spacey->display_hook = display_handler;
    spacey->display_data = display_data;
    spacey->active = 0;
    spacey->poll_rate = poll_rate;
    spacey->event_hook = NULL;

    spacey->options.aliens_wrap = 0;
    spacey->options.aliens_evasive = 0;

    spacey_alien_num_set (SPACEY_ALIEN_NUM_DEFAULT);
    spacey_shell_num_set (SPACEY_SHELL_NUM_DEFAULT);
    spacey_shell_speed_set (SPACEY_SHELL_SPEED_DEFAULT);
    spacey_alien_speed_set (SPACEY_ALIEN_SPEED_DEFAULT);
    spacey_alien_create_steps_set (SPACEY_ALIEN_CREATE_STEPS_DEFAULT);
    spacey->lives = SPACEY_LIVES_DEFAULT;
}


void
spacey_event_handler_set (void (*event_handler) (void *, spacey_event_t),
                          void *event_data)
{
    spacey->event_hook = event_handler;
    spacey->event_data = event_data;
}


static void
spacey_pixel_set (spacey_pos_t *pos,
                  spacey_pix_t type)
{
    spacey->display_hook (spacey->display_data, pos->x, pos->y, type);
}


/* Return if obj is at the same point as another alien.  */
static spacey_obj_t *
spacey_alien_conflict_p (spacey_obj_t *obj)
{
    int8_t i;

    for (i = 0; i < spacey->aliens.num; i++)
    {
        spacey_obj_t *alien = &spacey->aliens.array[i];
        
        if (!alien->active || alien == obj)
            continue;

        if (alien->pos.x == obj->pos.x
            && alien->pos.y == obj->pos.y)
            return alien;
    }
    return 0;
}


/* Return if obj is at the same point as another shell.  */
static spacey_obj_t *
spacey_shell_conflict_p (spacey_obj_t *obj)
{
    int8_t i;

    for (i = 0; i < spacey->shells.num; i++)
    {
        spacey_obj_t *shell = &spacey->shells.array[i];
        
        if (!shell->active || shell == obj)
            continue;

        if (shell->pos.x == obj->pos.x
            && shell->pos.y == obj->pos.y)
            return shell;
    }
    return 0;
}


static void
spacey_gun_move (int8_t inc)
{
    int8_t gun_x;

    spacey_pixel_set (&spacey->gun, SPACEY_PIX_OFF);

    gun_x = spacey->gun.x + inc;
    if (gun_x >= spacey->size.x)
        gun_x = 0;
    else if (gun_x < 0)
        gun_x = spacey->size.x - 1;
    spacey->gun.x = gun_x;

    spacey_pixel_set (&spacey->gun, SPACEY_PIX_GUN);
}


/* Return true if move off display.  */
static bool
spacey_shell_move (spacey_obj_t *shell)
{
    /* Erase the previous position of the shell except if still
       at gun.  */
    if (shell->pos.x != spacey->gun.x
        || shell->pos.y != spacey->gun.y)
        spacey_pixel_set (&shell->pos, SPACEY_PIX_OFF);

    /* Shells only go straight up.  */
    shell->pos.y -= SPACEY_SHELL_INC;
    
    if (shell->pos.y < 0)
        return 1;

    spacey_pixel_set (&shell->pos, SPACEY_PIX_SHELL);
    return 0;
}


static void
spacey_alien_erase (spacey_obj_t *alien)
{
    spacey_pixel_set (&alien->pos, SPACEY_PIX_OFF);
}


/* Return true if move off display or get to the bottom.  */
static bool
spacey_alien_move (spacey_obj_t *alien)
{
    int8_t x_jump;

    /* Aliens move down randomly zig zagging.  */
    alien->pos.y++;

    /* Randomly move left or right.  */
    x_jump = 2 * (rand () & 1) - 1;

    /* Don't move into line of gun if being evasive.  */
    if (spacey->options.aliens_evasive &&
        spacey->gun.x == alien->pos.x + x_jump)
        x_jump = -x_jump;

    alien->pos.x += x_jump;
    
    /* Keep the alien within bounds. */
    if (alien->pos.x < 0 || alien->pos.x >= spacey->size.x)
    {
        if (spacey->options.aliens_wrap)
            alien->pos.x = spacey->size.x - abs (alien->pos.x);
        else
            alien->pos.x -= 2 * x_jump;
    }

    if (alien->pos.y < spacey->size.y)
        spacey_pixel_set (&alien->pos, SPACEY_PIX_ALIEN);

    if (alien->pos.y >= (spacey->size.y - 1))
        return 1;

    return 0;
}


static void 
spacey_alien_create (void)
{
    int8_t i;

    for (i = 0; i < spacey->aliens.num; i++)
    {
        spacey_obj_t *alien = &spacey->aliens.array[i];

        if (alien->active)
            continue;

        /* Aliens start at the top.  */
        alien->pos.y = 0;

        while (1)
        {
            /* Choose random x position.  */
            alien->pos.x = rand () % spacey->size.x;
#if DISPLAY_INTERLACE
            /* Ensure x position is even.  */
            alien->pos.x &= ~1;
#endif
            alien->active = 1;

            /* Ensure aliens start at different spots to be fair!  */
            if (!spacey_alien_conflict_p (alien))
                break;
        }
        spacey->stats.aliens_live++;
        spacey_pixel_set (&alien->pos, SPACEY_PIX_ALIEN);
        return;
    }
    /* If we get to here then we already have too many aliens.  */
}


static void
spacey_shell_create (void)
{
    int8_t i;

    for (i = 0; i < spacey->shells.num; i++)
    {
        spacey_obj_t *shell = &spacey->shells.array[i];

        if (shell->active)
            continue;

        shell->pos = spacey->gun;
        shell->active = 1;
        spacey->stats.shells_fired++;
        /* Don't turn shell on initially since it will erase the gun.  */
        return;
    }
    /* If we get to here then we already have too many shells.  */
}


static void
spacey_alien_kill (spacey_obj_t *alien)
{
    alien->active = 0;
    spacey->stats.aliens_live--;
}


static void
spacey_shell_kill (spacey_obj_t *shell)
{
    shell->active = 0;
}


static void
spacey_event (spacey_event_t event)
{
    if (spacey->event_hook)
        spacey->event_hook (spacey->event_data, event);
}


static void
spacey_alien_hit (spacey_obj_t *alien, spacey_obj_t *shell)
{
    spacey_alien_kill (alien);
    spacey->stats.aliens_killed++;

    spacey_shell_kill (shell);
    spacey_pixel_set (&shell->pos, SPACEY_PIX_OFF);

    spacey_event (SPACEY_EVENT_ALIEN_HIT);
}


static void
spacey_alien_landed (void)
{
    spacey->stats.aliens_landed++;
    if (spacey->stats.aliens_landed >= spacey->lives)
        spacey->active = 0;

    spacey_event (SPACEY_EVENT_ALIEN_LANDED);
}


static void
spacey_shells_move (void)
{
    int8_t i;

    spacey->shells.move_clock++;
    if (spacey->shells.move_clock < spacey->shells.move_period)
        return;
    spacey->shells.move_clock = 0;

    /* Shells move until they hit an alien or move off the display.  */

    for (i = 0; i < spacey->shells.num; i++)
    {
        spacey_obj_t *shell = &spacey->shells.array[i];

        if (!shell->active)
            continue;

        if (spacey_shell_move (shell))
            spacey_shell_kill (shell);
        else
        {
            spacey_obj_t *alien;

            /* Check if we've hit an alien.  */
            while ((alien = spacey_alien_conflict_p (shell)))
            {
                /* We've hit an alien so kill it and the shell.  */
                spacey_alien_hit (alien, shell);
            }
        }
    }   
}


static void 
spacey_aliens_move (void)
{
    int8_t i;
    uint8_t steps;

    spacey->aliens.move_clock++;
    if (spacey->aliens.move_clock < spacey->aliens.move_period)
        return;
    spacey->aliens.move_clock = 0;

    /* Erase all aliens before moving them.  */
    for (i = 0; i < spacey->aliens.num; i++)
    {
        spacey_obj_t *alien = &spacey->aliens.array[i];

        if (!alien->active)
            continue;

        spacey_alien_erase (alien);
    }

    /* Now move and redraw aliens.  */
    for (i = 0; i < spacey->aliens.num; i++)
    {
        spacey_obj_t *alien = &spacey->aliens.array[i];

        if (!alien->active)
            continue;

        if (spacey_alien_move (alien))
        {
            if (alien->pos.y == spacey->gun.y)
            {
                /* The alien has reached Earth.  */
                spacey_alien_landed ();
            }
            else
            {
                /* The alien has moved off the display so disable it.  */
                spacey_alien_kill (alien);

                /* Redraw the gun in case it got hit by the alien.  */
                spacey_pixel_set (&spacey->gun, SPACEY_PIX_GUN);
            }
        }
        else
        {
            spacey_obj_t *shell;

            while ((shell = spacey_shell_conflict_p (alien)))
            {
                /* The alien hit a shell so kill it and the shell.  */
                spacey_alien_hit (alien, shell);
            }
        }
    }

    /* Randomly create an alien.  If there are no aliens create a new
       one more rapidly.  */
    if (!spacey->stats.aliens_live)
        steps = 2;
    else
        steps = spacey->alien_create_steps;
        
    if ((rand () % steps) == 0)
        spacey_alien_create ();
}


/* Update the state of the game.  */
bool
spacey_update (void)
{
    /* Allow playing with the gun even if game inactive.  */
    spacey_shells_move ();
    
    if (!spacey->active)
        return 0;

    spacey_aliens_move ();
    return 1;
}


/* Move the gun position to the right wrapping back around on left.  */
void
spacey_gun_move_right (void)
{
    spacey_gun_move (SPACEY_GUN_INC);
}


/* Move the gun position to the left wrapping back around on right.  */
void
spacey_gun_move_left (void)
{
    spacey_gun_move (-SPACEY_GUN_INC);
}


/* Fire the gun.  */
void
spacey_gun_fire (void)
{
    spacey_shell_create ();
}


/* Start a new game.  */
void
spacey_start (void)
{
    int8_t i;
    int8_t j;

    spacey->shells.move_clock = 0;
    spacey->aliens.move_clock = 0;

    for (i = 0; i < spacey->shells.num; i++)
    {
        spacey_obj_t *shell = &spacey->shells.array[i];

        shell->active = 0;
    }

    for (i = 0; i < spacey->aliens.num; i++)
    {
        spacey_obj_t *alien = &spacey->aliens.array[i];

        alien->active = 0;
    }

    /* Turn all pixels off.  */
    for (i = 0; i < spacey->size.x; i++)
        for (j = 0; j < spacey->size.y; j++)
        {
            spacey_pos_t pos = {i, j};

            spacey_pixel_set (&pos, SPACEY_PIX_OFF);
        }

    /* Display gun in centre of display.  */
    spacey->gun.x = (spacey->size.x / 2) & ~1;
    spacey->gun.y = spacey->size.y - 1;
    spacey_pixel_set (&spacey->gun, SPACEY_PIX_GUN);

    spacey->stats.aliens_live = 0;
    spacey->stats.aliens_killed = 0;
    spacey->stats.aliens_landed = 0;
    spacey->stats.shells_fired = 0;
    spacey->active = 1;
}


uint8_t
spacey_aliens_killed_get (void)
{
    return spacey->stats.aliens_killed;
}


uint8_t
spacey_shells_fired_get (void)
{
    return spacey->stats.shells_fired;
}

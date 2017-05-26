/** @file   space10.c
    @author M. P. Hayes, UCECE
    @date   20 April 2007
    @brief  A simple space invaders game with different difficulty levels 
            and IR remote control.

    @defgroup space10 A simple space invaders game controlled by the @ref spacey_remote1 "spacey_remote1" application.
*/

#include <string.h>
#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include "ir_serial.h"
#include "flasher.h"
#include "spacey.h"
#include "eeprom.h"
#include "uint8toa.h"
#include "../fonts/font3x5_1.h"


#define VERSION "1.6"

/** Define polling rates in Hz.  */
enum {LOOP_RATE = 500};
enum {FLASHER_UPDATE_RATE = LOOP_RATE};
enum {GAME_UPDATE_RATE = 100};
enum {GAME_OVER_PERIOD = 2};


/** Define flasher modes.  */
static flasher_pattern_t flasher_patterns[] =
{
    /** POLL_RATE, MOD_FREQ, MOD_DUTY, FLASHER_PERIOD,
       FLASHER_DUTY, FLASHES, PERIOD.  */
    {FLASHER_PATTERN (FLASHER_UPDATE_RATE, 100, 100, 0.4, 100, 1, 0.4)},
    {FLASHER_PATTERN (FLASHER_UPDATE_RATE, 100, 100, 0.4, 100, 1, 0.4)},
    {FLASHER_PATTERN (FLASHER_UPDATE_RATE, 200, 100, 0.1, 50, 1, 0.1)},
};

typedef enum {FLASH_MODE_GUN, FLASH_MODE_SHELL,
              FLASH_MODE_ALIEN, FLASH_MODE_NUM} flash_mode_t;


typedef enum {STATE_DUMMY, STATE_INIT, STATE_START,
              STATE_PLAYING, STATE_OVER, 
              STATE_READY, STATE_MENU_LEVEL} state_t;

enum {GAME_LEVEL_MAX = 5};


typedef struct
{
    uint8_t level;
    uint8_t games;
} game_data_t;



/** Draw pixel on display.  */
static void
display_handler (void *data, uint8_t col, uint8_t row, spacey_pix_t type)
{
    uint8_t *display = data;
    uint8_t pixel;

    pixel = row * TINYGL_WIDTH + col;
    display[pixel] = type;
}


/** Display the game over message.  */
static void
game_over_display (char *buffer)
{
    char *str = buffer;

    strcpy (str, "GAME OVER ");
    while (*str)
        str++;
    *str++ = 'K';
    uint8toa (spacey_aliens_killed_get (), str, 0);
    while (*str)
        str++;
    *str++ = 'F';
    uint8toa (spacey_shells_fired_get (), str, 0);
    tinygl_clear ();
    tinygl_text (buffer);
}


static void
game_text_display (uint8_t num, char *buffer, char *msg)
{
    char *str = buffer;

    while (*msg)
        *str++ = *msg++;
    uint8toa (num, str, 0);
    tinygl_clear ();
    tinygl_text (buffer);
}


/** Display the game level of difficulty.  */
static void
game_level_display (uint8_t level, char *buffer)
{
    game_text_display (level, buffer, "LEVEL");
}


/** Set the game level of difficulty.  */
static void
game_level_set (uint8_t level)
{
    spacey_options_t options;
    uint8_t alien_num = 3;
    uint8_t alien_steps = 4;

    switch (level)
    {
    default:
    case 0:
        options.aliens_wrap = 0;
        options.aliens_evasive = 0;
        break;

    case 1:
        options.aliens_wrap = 0;
        options.aliens_evasive = 1;
        alien_steps = 3;
        break;

    case 2:
        options.aliens_wrap = 1;
        options.aliens_evasive = 0;
        alien_steps = 3;
        break;

    case 3:
        options.aliens_wrap = 1;
        options.aliens_evasive = 1;
        alien_steps = 3;
        break;

    case 4:
        options.aliens_wrap = 1;
        options.aliens_evasive = 1;
        alien_steps = 3;
        alien_num = 4;
        break;

    case 5:
        options.aliens_wrap = 1;
        options.aliens_evasive = 1;
        alien_steps = 2;
        alien_num = 4;
        break;
    }
    
    spacey_alien_num_set (alien_num);
    spacey_alien_create_steps_set (alien_steps);
    spacey_options_set (options);
}


static void
game_start (game_data_t *data)
{
    tinygl_clear ();
    data->games++;
    game_level_set (data->level);
    spacey_start ();
    eeprom_write (0, data, sizeof (*data));
}


void game_event (__unused__ void *data, spacey_event_t event)
{
    ir_serial_transmit (event);
}


int
main (void)
{
    uint8_t game_ticks;
    uint8_t game_over_ticks;
    state_t state = STATE_INIT;
    flasher_t flashers[SPACEY_PIX_TYPE_NUM];
    uint8_t flasher_state[SPACEY_PIX_TYPE_NUM];
    flasher_obj_t flashers_info[SPACEY_PIX_TYPE_NUM];
    uint8_t display[TINYGL_WIDTH * TINYGL_HEIGHT];
    uint8_t i;
    uint8_t j;
    game_data_t data;
    char message[44];
    ir_serial_ret_t ret;
    uint8_t ir_data;

    system_init ();

    /* The first time EEPROM is read all the bytes are 0xFF so set to
       sensible defaults.  */
    eeprom_read (0, &data, sizeof (data));
    if (data.level == 0xff)
    {
        data.level = 0;
        data.games = 0;
    }

    for (i = 0; i < ARRAY_SIZE (flashers); i++)
    {
        flashers[i] = flasher_init (&flashers_info[i]);
        flasher_state[i] = 0;
    }

    for (i = 0; i < ARRAY_SIZE (display); i++)
        display[i] = 0;

    /* Set up flash patterns for different pixel types.  */
    flasher_pattern_set (flashers[SPACEY_PIX_GUN],
                         &flasher_patterns[FLASH_MODE_GUN]);
    flasher_pattern_set (flashers[SPACEY_PIX_SHELL],
                         &flasher_patterns[FLASH_MODE_SHELL]);
    flasher_pattern_set (flashers[SPACEY_PIX_ALIEN],
                         &flasher_patterns[FLASH_MODE_ALIEN]);
    
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_speed_set (10);

    spacey_init (GAME_UPDATE_RATE, TINYGL_WIDTH, TINYGL_HEIGHT, 
                 display_handler, display);

    spacey_event_handler_set (game_event, 0);

    game_ticks = 0;
    game_over_ticks = 0;

    pacer_init (LOOP_RATE);

    while (1)
    {
        pacer_wait ();

        if (state == STATE_PLAYING)
        {
            uint8_t *src;

            /* Update flasher states.  NB, the first flasher is always off.  */
            for (i = 1; i < ARRAY_SIZE (flashers); i++)
                flasher_state[i] = flasher_update (flashers[i]);
            
            /* Update display.  */
            src = display;
            for (j = 0; j < TINYGL_HEIGHT; j++)
                for (i = 0; i < TINYGL_WIDTH; i++)
                {
                    tinygl_point_t point = {i, j};

                    tinygl_draw_point (point, flasher_state[*src++]);
                }
        }

        /* Advance messages and refresh display.  */
        tinygl_update ();
        
        game_ticks++;
        if (game_ticks >= LOOP_RATE / GAME_UPDATE_RATE)
        {
            game_ticks = 0;
                
            switch (state)
            {
            case STATE_PLAYING:
                if (! spacey_update ())
                {
                    game_over_display (message);
                    game_over_ticks = 0;
                    state = STATE_OVER;
                }
                break;
                
            case STATE_INIT:
                tinygl_text ("SPACEY READY V" VERSION " BY MPH ");
                state = STATE_READY;
                break;
                
            case STATE_OVER:
                game_over_ticks ++;
                if (game_over_ticks >= GAME_UPDATE_RATE * GAME_OVER_PERIOD)
                    state = STATE_READY;
                /* Fall through.  */
                
            case STATE_READY:
            case STATE_MENU_LEVEL:
            default:
                break;
                
            case STATE_START:
                /* Turn that bloody blimey space invader off...  */
                game_start (&data);
                state = STATE_PLAYING;
                break;
            }
        }

        ret = ir_serial_receive (&ir_data);
        if (ret == IR_SERIAL_OK)
        {
            if (ir_data == 1)
            {
                switch (state)
                {
                case STATE_READY:
                    state = STATE_MENU_LEVEL;
                    game_level_display (data.level, message);
                    break;

                case STATE_MENU_LEVEL:
                    state = STATE_INIT;
                    break;

                case STATE_PLAYING:
                    spacey_gun_move_right ();
                    break;

                default:
                    break;
                }
            }

            if (ir_data == 2)
            {
                switch (state)
                {
                case STATE_READY:
                    state = STATE_START;
                    break;

                case STATE_PLAYING:
                    spacey_gun_fire ();
                    break;

                case STATE_MENU_LEVEL:
                    data.level++;
                    if (data.level > GAME_LEVEL_MAX)
                        data.level = 0;
                    game_level_display (data.level, message);
                    break;

                default:
                    break;
                }
            }
        }
    }
}

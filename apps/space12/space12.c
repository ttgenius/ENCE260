/** @file   space12.c
    @author M. P. Hayes, UCECE
    @date   20 April 2007
    @brief  A simple space invaders game with different difficulty levels
            and sound effects.

    @defgroup space12 A simple space invaders game with sound effects.
*/

#include <string.h>
#include "system.h"
#include "tinygl.h"
#include "task.h"
#include "navswitch.h"
#include "led.h"
#include "flasher.h"
#include "spacey.h"
#include "eeprom.h"
#include "uint8toa.h"
#include "../fonts/font3x5_1.h"
#include "tweeter.h"
#include "mmelody.h"
#include "pio.h"


#define VERSION "1.6"

/** Define polling rates in Hz.  The tweeter task needs to be the highest
    priority since any jitter will make the sound awful.  */
enum {TWEETER_TASK_RATE = 5000};
enum {DISPLAY_TASK_RATE = 300};
enum {TUNE_TASK_RATE = 100};
enum {BUTTON_TASK_RATE = 20};
enum {GAME_TASK_RATE = 100};

enum {FLASHER_UPDATE_RATE = 500};
enum {TUNE_BPM_RATE = 200};
enum {GAME_OVER_PERIOD = 2};
enum {BUTTON_HOLD_PERIOD = 1};


/* Connect piezo tweeter to first and third pin of UCFK4 P1 connector
   for push-pull operation.  For single-ended drive (with reduced
   volume) connect the other piezo connection to ground or Vcc and do
   not define PIEZO2_PIO.  */
#define PIEZO1_PIO PIO_DEFINE (PORT_D, 4)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 6)


/** Define flasher modes.  */
static flasher_pattern_t flasher_patterns[] =
{
    /** TASK_RATE, MOD_FREQ, MOD_DUTY, FLASHER_PERIOD,
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


static state_t state = STATE_INIT;
static game_data_t game_data;
static uint8_t display[TINYGL_WIDTH * TINYGL_HEIGHT];

static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE (TWEETER_TASK_RATE);
static tweeter_t tweeter;
static mmelody_t melody;
static mmelody_obj_t melody_info;
static tweeter_obj_t tweeter_info;


static const char game_intro_tune[] =
{
#include "temple_of_love.mmel"
"> "
};


static const char game_over_tune[] =
{
#include "imperial_march.mmel"
"    >"
};


static void tweeter_task_init (void)
{
    tweeter = tweeter_init (&tweeter_info, TWEETER_TASK_RATE, scale_table);

    pio_config_set (PIEZO1_PIO, PIO_OUTPUT_LOW);
#ifdef PIEZO2_PIO
    pio_config_set (PIEZO2_PIO, PIO_OUTPUT_LOW);
#endif
}


static void tweeter_task (__unused__ void *data)
{
    bool state;

    state = tweeter_update (tweeter);

    pio_output_set (PIEZO1_PIO, state);
#ifdef PIEZO2_PIO
    /* Push-pull piezo tweeter drive.  */
    pio_output_set (PIEZO2_PIO, !state);
#endif
}


static void tune_task_init (void)
{
    melody = mmelody_init (&melody_info, TUNE_TASK_RATE, 
			   (mmelody_callback_t) tweeter_note_play, tweeter);

    mmelody_speed_set (melody, TUNE_BPM_RATE);
}


static void tune_task (__unused__ void *data)
{
    mmelody_update (melody);
}


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
    uint8toa (spacey_aliens_killed_get (), str, 0);
    while (*str)
        str++;
    *str++ = '/';
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
    game_text_display (level, buffer, "L");
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
    /* Stop infuriating tune.  */
    mmelody_play (melody, "");
}


static void game_event (__unused__ void *data, spacey_event_t event)
{
    switch (event)
    {
    case SPACEY_EVENT_ALIEN_HIT:
	mmelody_play (melody, "C+");
        break;

    case SPACEY_EVENT_ALIEN_LANDED:
	mmelody_play (melody, "C");
        break;
    }
}



static void game_task (__unused__ void *data)
{
    static bool init = 0;
    static uint8_t game_over_ticks;
    char message[44];

    if (!init)
    {
        led_init ();
        
        led_set (LED1, 1);
        
        /* When the EEPROM is erased all the bytes are 0xFF so set to
           sensible defaults.  */
        eeprom_read (0, &game_data, sizeof (game_data));
        if (game_data.level == 0xff)
        {
            game_data.level = 0;
            game_data.games = 0;
        }

        spacey_init (GAME_TASK_RATE, TINYGL_WIDTH, TINYGL_HEIGHT, 
                     display_handler, display);
        
        spacey_event_handler_set (game_event, 0);

        init = 1;
    }

    switch (state)
    {
    case STATE_PLAYING:
        if (! spacey_update ())
        {
            game_over_display (message);
            mmelody_play (melody, game_over_tune);
            game_over_ticks = 0;
            led_set (LED1, 1);
            state = STATE_OVER;
        }
        break;
        
    case STATE_INIT:
        tinygl_text ("SPACEY READY V" VERSION " BY MPH ");
        mmelody_play (melody, game_intro_tune);
        state = STATE_READY;
        break;
        
    case STATE_OVER:
        game_over_ticks ++;
        if (game_over_ticks >= GAME_TASK_RATE * GAME_OVER_PERIOD)
            state = STATE_READY;
        /* Fall through.  */
        
    case STATE_READY:
    case STATE_MENU_LEVEL:
    default:
        break;
        
    case STATE_START:
        /* Turn that bloody blimey space invader off...  */
        game_start (&game_data);
        led_set (LED1, 0);
        state = STATE_PLAYING;
        break;
    }
}



static void display_task (__unused__ void *data)
{
    static bool init = 0;
    static flasher_t flashers[SPACEY_PIX_TYPE_NUM];
    static uint8_t flasher_state[SPACEY_PIX_TYPE_NUM];
    static flasher_obj_t flashers_info[SPACEY_PIX_TYPE_NUM];

    if (!init)
    {
        uint8_t i;

        tinygl_init (DISPLAY_TASK_RATE);
        tinygl_font_set (&font3x5_1);
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
        tinygl_text_speed_set (10);

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
        
        init = 1;
    }

    if (state == STATE_PLAYING)
    {
        uint8_t *src;
        uint8_t i;
        uint8_t j;
        
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
}



static void navswitch_task (__unused__ void *data)
{
    static bool init = 0;
    char message[44];

    if (!init)
    {
        navswitch_init ();
        init = 1;
    }

    navswitch_update ();
    
    if (navswitch_push_event_p (NAVSWITCH_EAST))
    {
        switch (state)
        {
        case STATE_READY:
            break;
            
        case STATE_MENU_LEVEL:
            if (game_data.level < GAME_LEVEL_MAX - 1)
                game_data.level++;
            game_level_display (game_data.level, message);
            break;
            
        case STATE_PLAYING:
            spacey_gun_move_right ();
            break;
            
        default:
            break;
        }
    }
    
    if (navswitch_push_event_p (NAVSWITCH_WEST))
    {
        switch (state)
        {
        case STATE_READY:
            break;
            
        case STATE_MENU_LEVEL:
            if (game_data.level > 1)
                game_data.level--;
            game_level_display (game_data.level, message);
            break;
            
        case STATE_PLAYING:
            spacey_gun_move_left ();
            break;
            
        default:
            break;
        }
    }
    
    if (navswitch_push_event_p (NAVSWITCH_WEST))
    {
        switch (state)
        {
        case STATE_READY:
            state = STATE_MENU_LEVEL;
            game_level_display (game_data.level, message);
            break;
            
        case STATE_MENU_LEVEL:
            break;
            
        case STATE_PLAYING:
            break;
            
        default:
            break;
        }
    }
    
    if (navswitch_push_event_p (NAVSWITCH_PUSH))
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
            state = STATE_READY;
            break;
            
        default:
            break;
        }
    }
}


int
main (void)
{
    task_t tasks[] =
    {
        {.func = tweeter_task, .period = TASK_RATE / TWEETER_TASK_RATE,
         .data = 0},
        {.func = tune_task, .period = TASK_RATE / TUNE_TASK_RATE,
         .data = 0},
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE,
         .data = 0},
        {.func = navswitch_task, .period = TASK_RATE / BUTTON_TASK_RATE,
         .data = 0},
        {.func = game_task, .period = TASK_RATE / GAME_TASK_RATE,
         .data = 0},
    };


    system_init ();

    tweeter_task_init ();
    tune_task_init ();

    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}

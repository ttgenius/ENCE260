/** @file   jukebox1.c
    @author M.P. Hayes
    @date   30 Aug 2013

    @note   The fidelity of the notes is poor due to jitter
    produced by the display task.
    @note   The tunes are stored in RAM.  Adding more tunes can cause
    a subtle memory overflow and the program will fail.  A solution
    would be to store the tunes in flash memory but this requires
    some jiggery-pokery due to the Harvard architecture of the AVR.
*/

#include "system.h"
#include "navswitch.h"
#include "led.h"
#include "pio.h"
#include "task.h"
#include "tweeter.h"
#include "mmelody.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"


/* Connect piezo tweeter to pins 6 and 8 of UCFK4 P1 connector
   for push-pull operation.  */
#define PIEZO1_PIO PIO_DEFINE (PORT_D, 4)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 6)

/* Define polling rates in Hz.  */
#define TWEETER_TASK_RATE 20000

#define TUNE_TASK_RATE 200

#define NAVSWITCH_TASK_RATE 10

#define TUNE_BPM_RATE 200

#define LED_TASK_RATE (TUNE_BPM_RATE / 60.0)

#define DISPLAY_TASK_RATE 200


static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE (TWEETER_TASK_RATE);
static tweeter_t tweeter;
static mmelody_t melody;
static mmelody_obj_t melody_info;
static tweeter_obj_t tweeter_info;
static char *note_names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

static const char tune1[] =
{
#include "are_friends_electric.mmel"
};

static const char tune2[] =
{
#include "imperial_march.mmel"
};


void
note_play (tweeter_t tweeter, tweeter_note_t note, uint8_t velocity)
{
    tinygl_clear ();
    if (note != 0 && velocity != 0)
        tinygl_text (note_names[note % 12]);

    tweeter_note_play (tweeter, note, velocity);
}


static void led_flash_task_init (void)
{
    led_init ();
}


static void led_flash_task (__unused__ void *data)
{
    static uint8_t state = 0;

    led_set (LED1, state);
    state = !state;
}


static void tweeter_task_init (void)
{
    tweeter = tweeter_init (&tweeter_info, TWEETER_TASK_RATE, scale_table);

    pio_config_set (PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (PIEZO2_PIO, PIO_OUTPUT_HIGH);
}


static void tweeter_task (__unused__ void *data)
{
    uint8_t state;

    state = tweeter_update (tweeter);
    pio_output_set (PIEZO1_PIO, state);
    pio_output_set (PIEZO2_PIO, !state);
}


static void tune_task_init (void)
{
    melody = mmelody_init (&melody_info, TUNE_TASK_RATE, 
			   (mmelody_callback_t) note_play, tweeter);

    mmelody_speed_set (melody, TUNE_BPM_RATE);
}


static void tune_task (__unused__ void *data)
{
    mmelody_update (melody);
}


static void navswitch_task_init (void)
{
    navswitch_init ();
}


static void navswitch_task (__unused__ void *data)
{
    navswitch_update ();

    if (navswitch_push_event_p (NAVSWITCH_PUSH))
    {
        mmelody_play (melody, 0);            
    }
    if (navswitch_push_event_p (NAVSWITCH_NORTH))
        mmelody_play (melody, tune1);            
    if (navswitch_push_event_p (NAVSWITCH_SOUTH))
        mmelody_play (melody, tune2);            
}


static void display_task_init (void)
{
    tinygl_init (DISPLAY_TASK_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}


static void display_task (__unused__ void *data)
{
    tinygl_update ();
}


int main (void)
{
    task_t tasks[] =
    {
        {.func = tweeter_task, .period = TASK_RATE / TWEETER_TASK_RATE, .data = 0},
        {.func = led_flash_task, .period = TASK_RATE / LED_TASK_RATE, .data = 0},
        {.func = tune_task, .period = TASK_RATE / TUNE_TASK_RATE, .data = 0},
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE, .data = 0}, 
        {.func = navswitch_task, .period = TASK_RATE / NAVSWITCH_TASK_RATE, .data = 0},
    };

    system_init ();

    led_flash_task_init ();
    tweeter_task_init ();
    tune_task_init ();
    display_task_init ();
    navswitch_task_init ();

    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}

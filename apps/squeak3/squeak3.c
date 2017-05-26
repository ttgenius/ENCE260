/** @file   squeak3.c
    @author M.P. Hayes
    @date   30 Aug 2011
*/

#include "system.h"
#include "button.h"
#include "navswitch.h"
#include "led.h"
#include "pio.h"
#include "task.h"
#include "tweeter.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"


/* Connect piezo tweeter to outermost pins of UCFK4 P1 connector.  */
#define PIEZO_PIO PIO_DEFINE (PORT_D, 6)

/* Define polling rate in Hz.  */
#define LED_TASK_RATE 2

#define TWEETER_TASK_RATE 20000

#define BUTTON_TASK_RATE 10

#define DISPLAY_TASK_RATE 200

/* Define text update rate (characters per 10 s).  */
#define MESSAGE_RATE 10

#define MIDI_NOTE_C4 60


static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE (TWEETER_TASK_RATE);
static tweeter_t tweeter;
static tweeter_obj_t tweeter_info;
static char *note_names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
static uint8_t note = 0;


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

    pio_config_set (PIEZO_PIO, PIO_OUTPUT_LOW);
}


static void tweeter_task (__unused__ void *data)
{
    pio_output_set (PIEZO_PIO, tweeter_update (tweeter));
}


static void button_task_init (void)
{
    button_init ();
}


static void display_note (void)
{
    tinygl_clear ();
    tinygl_text (note_names[note]);
}


static void button_task (__unused__ void *data)
{
    button_update ();
    navswitch_update ();

    if (navswitch_push_event_p (NAVSWITCH_NORTH))
    {
        if (note < ARRAY_SIZE (note_names) - 1)
            note++;
        else
            note = 0;
        display_note ();
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH))
    {
        if (note > 0)
            note--;
        else
            note = ARRAY_SIZE (note_names) - 1;
        display_note ();
    }

    if (button_push_event_p (BUTTON1)
        || navswitch_push_event_p (NAVSWITCH_PUSH))

    {
        tweeter_note_play (tweeter, note + MIDI_NOTE_C4, 127);
    }

    if (button_release_event_p (BUTTON1)
        || navswitch_release_event_p (NAVSWITCH_PUSH))
    {
        tweeter_note_play (tweeter, 0, 127);
    }
}


static void display_task_init (void)
{
    tinygl_init (DISPLAY_TASK_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);

    display_note ();
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
        {.func = button_task, .period = TASK_RATE / BUTTON_TASK_RATE, .data = 0},
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE, .data = 0},
    };

    system_init ();

    led_flash_task_init ();
    tweeter_task_init ();
    button_task_init ();
    display_task_init ();

    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}

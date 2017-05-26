/** @file   squeak1.c
    @author M.P. Hayes
    @date   30 Aug 2011
*/

#include "system.h"
#include "led.h"
#include "pio.h"
#include "task.h"
#include "tweeter.h"
#include "mmelody.h"


/* Connect piezo tweeter to outermost pins of UCFK4 P1 connector.  */
#define PIEZO_PIO PIO_DEFINE (PORT_D, 6)

/* Define polling rate in Hz.  */
#define LED_TASK_RATE 2

#define TWEETER_TASK_RATE 20000

#define TUNE_TASK_RATE 100

#define TUNE_BPM_RATE 200


static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE (TWEETER_TASK_RATE);
static tweeter_t tweeter;
static mmelody_t melody;
static mmelody_obj_t melody_info;
static tweeter_obj_t tweeter_info;


static const char tune1[] =
{
#include "imperial_march.mmel"
"    >"
};


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


static void tune_task_init (void)
{
    melody = mmelody_init (&melody_info, TUNE_TASK_RATE, 
			   (mmelody_callback_t) tweeter_note_play, tweeter);

    mmelody_speed_set (melody, TUNE_BPM_RATE);

    mmelody_play (melody, tune1);
}


static void tune_task (__unused__ void *data)
{
    mmelody_update (melody);
}


int main (void)
{
    task_t tasks[] =
    {
        {.func = tweeter_task, .period = TASK_RATE / TWEETER_TASK_RATE, .data = 0},
        {.func = led_flash_task, .period = TASK_RATE / LED_TASK_RATE, .data = 0},
        {.func = tune_task, .period = TASK_RATE / TUNE_TASK_RATE, .data = 0},
    };

    system_init ();

    led_flash_task_init ();
    tweeter_task_init ();
    tune_task_init ();

    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}

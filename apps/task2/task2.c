/** @file   task2.c
    @author M.P. Hayes
    @date   5 Oct 2010
*/

#include "system.h"
#include "led.h"
#include "task.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


/* Define polling rate in Hz.  */
#define LED_TASK_RATE 2

#define DISPLAY_TASK_RATE 400

/* Define text update rate (characters per 10 s).  */
#define MESSAGE_RATE 10


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


static void display_task_init (void)
{
    tinygl_init (DISPLAY_TASK_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    
    tinygl_text ("HELLO WORLD ");
}


static void display_task (__unused__ void *data)
{
    tinygl_update ();
}


int main (void)
{
    task_t tasks[] =
    {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE, .data = 0},
        {.func = led_flash_task, .period = TASK_RATE / LED_TASK_RATE, .data = 0},
    };

    system_init ();

    led_flash_task_init ();
    display_task_init ();

    task_schedule (tasks, 2);
    return 0;
}

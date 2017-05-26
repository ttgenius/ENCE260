/** @file   stopwatch1.c
    @author M.P. Hayes
    @date   22 Sep 2011
*/

#include "system.h"
#include "button.h"
#include "task.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"


/* Define polling rates in Hz.  */
#define BUTTON_TASK_RATE 100

#define DISPLAY_TASK_RATE 250

#define TIMER_TASK_RATE 100


static bool run;


static void button_task_init (void)
{
    button_init ();
}


static void button_task (__unused__ void *data)
{
    button_update ();

    if (button_push_event_p (BUTTON1))
    {
        run = !run;
    }
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


static void timer_task_init (void)
{
    tinygl_text ("00");
}


static void timer_task (__unused__ void *data)
{
    static uint16_t time;
    char str[3];

    if (!run)
    {
        time = 0;
        return;
    }

    str[0] = ((time / 10) % 10) + '0';
    str[1] = (time % 10) + '0';
    str[2] = 0;

    tinygl_text (str);

    time++;
}


int main (void)
{
    task_t tasks[] =
    {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE}, 
        {.func = button_task, .period = TASK_RATE / BUTTON_TASK_RATE},
        {.func = timer_task, .period = TASK_RATE / TIMER_TASK_RATE},
    };

    system_init ();

    display_task_init ();
    button_task_init ();
    timer_task_init ();

    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}

/** @file   task1.c
    @author M.P. Hayes
    @date   5 Oct 2010
*/

#include "system.h"
#include "led.h"
#include "task.h"


/* Define polling rate in Hz.  */
#define LED_TASK_RATE 2


static void led_flash_task (__unused__ void *data)
{
    static uint8_t state = 0;

    led_set (LED1, state);
    state = !state;
}


int main (void)
{
    task_t tasks[] =
    {
        {.func = led_flash_task, .period = TASK_RATE / LED_TASK_RATE, .data = 0},
    };

    system_init ();
    led_init ();

    task_schedule (tasks, 1);
    return 0;
}

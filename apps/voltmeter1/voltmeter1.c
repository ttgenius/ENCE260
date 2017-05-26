/** @file   voltmeter1.c
    @author M. P. Hayes, UCECE
    @date   1 February 2013
    @brief  Simple message display program
    
    @defgroup voltmeter1 Simple voltmeter
*/

#include "system.h"
#include "task.h"
#include "tinygl.h"
#include "adc.h"
#include "../fonts/font3x5_1.h"


/* Define polling rates in Hz.  */
#define DISPLAY_TASK_RATE 250

#define ADC_TASK_RATE 1

#define ADC_AIN 3


static void adc_task_init (void)
{
    adc_init ();
    adc_enable (ADC_AIN);
}


static void adc_task (__unused__ void *data)
{
    uint8_t count;
    char str[3];

    count = adc_measure (ADC_AIN);

    str[0] = ((count / 10) % 10) + '0';
    str[1] = (count % 10) + '0';
    str[2] = 0;

    tinygl_text (str);
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
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE}, 
        {.func = adc_task, .period = TASK_RATE / ADC_TASK_RATE},
    };

    system_init ();

    display_task_init ();
    adc_task_init ();

    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}

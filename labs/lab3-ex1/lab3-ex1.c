#include "system.h"
#include "button.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"

//static uint16_t pacer_period;
static bool run;
static void display_task ()
{
    tinygl_update ();
}
static void timer_task ()
{
    static uint16_t time;
    char str[3];

    if (!run)
    {
        time = 0;
        return;
    }

    str[0] = ((time / 100) % 10) + '0';
    str[1] = (time /10 % 10) + '0';
    str[2] = 0;

    tinygl_text (str);

    time++;
}
static void button_task ()
{
    button_update ();

    if (button_push_event_p (BUTTON1))
    {
        run = !run;
    }
}
int main (void)
{
    system_init();
    
    /* TODO: Initialise the button driver, tinygl, and the pacer.  */
    button_init();
   
    pacer_init(1000);
    tinygl_init (1000);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_text ("00");
    while(1)
    {
        pacer_wait();
        
	/* TODO: Implement the functionality of the tasks in the
           stopwatch1 program.  */
        button_task();
        timer_task();
		display_task();
    }
    return 0;
}

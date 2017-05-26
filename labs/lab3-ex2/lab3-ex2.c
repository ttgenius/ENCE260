#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


#define PACER_RATE 500
#define MESSAGE_RATE 10
#define LOOP_RATE 500

int main (void)
{
    system_init();

    /* TODO: Initialise tinygl. */
    tinygl_init (LOOP_RATE);
	tinygl_font_set (&font5x7_1);
	tinygl_text_speed_set (MESSAGE_RATE);

    /* TODO: Set the message using tinygl_tlab3-ext().  */
    tinygl_text("Hello world");
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    

    pacer_init (PACER_RATE);

    while(1)
    {
        pacer_wait();
        
        /* TODO: Call the tinygl update function. */
        tinygl_update ();
        
    }
    return 0;
}

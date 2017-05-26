/** @file   scribble1.c
    @author M.P. Hayes
    @date   5 Oct 2010
*/

#include "system.h"
#include "button.h"
#include "navswitch.h"
#include "led.h"
#include "tinygl.h"
#include "pacer.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

#define SCRIBBLE_SPEED 2


typedef enum dir dir_t;

struct pen 
{
    /* Current pos of pen.  */
    tinygl_point_t pos;
    /* Current state of pen.  */
    bool state;
};

typedef struct pen pen_t;


int main (void)
{
    pen_t pen;

    system_init ();

    pen.pos.x = TINYGL_WIDTH / 2;
    pen.pos.y = TINYGL_HEIGHT / 2;
    pen.state = 1;

    pacer_init (LOOP_RATE);
    led_init ();
    button_init ();
    navswitch_init ();

    tinygl_init (LOOP_RATE);
    tinygl_draw_point (pen.pos, pen.state);

    led_set (LED1, pen.state);

    /* Paced loop.  */
    while (1)
    {
        bool draw = 0;

        /* Wait for next tick.  */
        pacer_wait ();

        navswitch_update ();
        button_update ();

        if (navswitch_push_event_p (NAVSWITCH_WEST) && pen.pos.x > 0)
        {
            pen.pos.x--;
            draw = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_EAST) && pen.pos.x < TINYGL_WIDTH - 1)
        {
            pen.pos.x++;
            draw = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH) && pen.pos.y < TINYGL_HEIGHT - 1)
        {
            pen.pos.y++;
            draw = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH) && pen.pos.y > 0)
        {
            pen.pos.y--;
            draw = 1;
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH)
            || button_push_event_p (BUTTON1))
        {
            pen.state = !pen.state;
            led_set (LED1, pen.state);
        }

        if (draw)
            tinygl_draw_point (pen.pos, pen.state);

  
        tinygl_update ();
    }

    return 0;
}

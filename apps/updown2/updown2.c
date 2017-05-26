/** @file   updown2.c
    @author M.P. Hayes
    @date   28 Aug 2008

    @defgroup updown2 Simple counting program using navswitch
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "pio.h"
#include "../fonts/font3x5_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300


static void show_count (uint8_t count)
{
    char buffer[2];

    /* FIXME!  */
    buffer[0] = count + '0';
    buffer[1] = 0;
    tinygl_text (buffer);
}


int main (void)
{
    uint8_t count = 5;
    uint16_t tick = 0;

    system_init ();
    pio_config_set (LED1_PIO, PIO_OUTPUT_HIGH);

    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font3x5_1);

    navswitch_init ();
    pacer_init (LOOP_RATE);

    show_count (count);


    /* Paced loop.  */
    while (1)
    {

        /* Wait for next tick.  */
        pacer_wait ();

        tick++;
        if (tick > LOOP_RATE / 2)
        {
            tick = 0;
            pio_output_toggle (LED1_PIO);
        }

        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_WEST))
        {
            count--;
            show_count (count);
        }

        if (navswitch_push_event_p (NAVSWITCH_EAST))
        {
            count++;
            show_count (count);
        }

        tinygl_update ();
    }

    return 0;
}

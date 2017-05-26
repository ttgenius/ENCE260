/** @file   updown1.c
    @author M.P. Hayes
    @date   28 Aug 2008

    @defgroup updown1 Simple counting program using button.
*/

#include "system.h"
#include "pacer.h"
#include "pio.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


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
    bool state;
    bool prev_state;
    uint8_t count = 0;

    system_init ();
    tinygl_init (LOOP_RATE);

    tinygl_font_set (&font5x7_1);

    pio_config_set (BUTTON1_PIO, PIO_INPUT);

    state = !pio_input_get (BUTTON1_PIO);
    prev_state = state;

    show_count (count);

    pacer_init (LOOP_RATE);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        state = pio_input_get (BUTTON1_PIO);
        if (state && !prev_state)
        {
            count++;
            show_count (count);
        }
        prev_state = state;

        tinygl_update ();
    }

    return 0;
}

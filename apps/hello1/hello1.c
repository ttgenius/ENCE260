/** @file   hello1.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Simple message display program
    
    @defgroup hello1 Simple message display program
*/

#include "system.h"
#include "simplefont.h"
#include "pacer.h"
#include "ledmat.h"



/* Define polling rate in Hz.  */
#define LOOP_RATE 300

/* Define text update rate in Hz.  */
#define MESSAGE_RATE 2


int main (void)
{
    char message[] = "HELLO WORLD";
    uint8_t col = 0;
    uint8_t index = 0;
    uint8_t tick = 0;

    system_init ();
    ledmat_init ();

    pacer_init (LOOP_RATE);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        ledmat_display_column (font[(message[index] - ' ') * 5 + col], 
                               col);

        col++;
        if (col > 4)
            col = 0;

        /* Advance message.  */
        tick++;
        if (tick >= LOOP_RATE / MESSAGE_RATE)
        {
            tick = 0;
            index++;
            if (!message[index])
                index = 0;
        }
    }

    return 0;
}

/** @file   ir_grab2.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Test program for IR serial communnications.

    @defgroup ir_grab2 Test program for IR serial communications.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "ir.h"
#include "delay.h"
#include "uint8toa.h"
#include "../fonts/font3x5_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 2000

/* Define text update rate (characters per 10 s).  */
#define MESSAGE_RATE 10

/* At 2400 baud and 36 kHz modulation, there are 15 cycles per bit.
   With a Sony start code of length 4.5 x 0.6 ms there are 86 cycles.  */
#define COUNT_MAX 250
#define CODESEQ_LEN_MAX 40

#define IR_MODULATION_PERIOD_US (1e6 / IR_MODULATION_FREQ)
#define TWEAK_US 0.5


typedef uint8_t count_t;

typedef struct code
{
    count_t on;
    count_t off;
} code_t;


static void transmit (code_t *codeseq, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        ir_tx_set (1, codeseq[i].on);
        ir_tx_set (0, codeseq[i].off);
    }
}


static uint8_t capture (code_t *codeseq, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        count_t count;

        for (count = 0; count < COUNT_MAX && ir_rx_get (); count++)
        {
            DELAY_US (IR_MODULATION_PERIOD_US - TWEAK_US);
        }
        codeseq[i].on = count;

        /* Something funny here.  */
        if (count >= COUNT_MAX)
            return 0;

        for (count = 0; count < COUNT_MAX && !ir_rx_get (); count++)
        {
            DELAY_US (IR_MODULATION_PERIOD_US - TWEAK_US);
        }
        codeseq[i].off = count;

        if (count >= COUNT_MAX)
            return i + 1;
    }
    return i;
}


static void show_char (char ch)
{
    char buffer[2];

    buffer[0] = ch;
    buffer[1] = 0;
    tinygl_text (buffer);
}


static void show_num (char ch, uint8_t num)
{
    char buffer[5];

    buffer[0] = ch;
    uint8toa (num, buffer + 1, 0);
    tinygl_text (buffer);
}


int main (void)
{
    uint8_t size = 0;
    code_t codeseq[CODESEQ_LEN_MAX];
    uint8_t count = 0;

    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_speed_set (MESSAGE_RATE);

    navswitch_init ();
    ir_init ();

    pacer_init (LOOP_RATE);

    show_num ('W', count);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        tinygl_update ();

        navswitch_update ();

        if (navswitch_push_event_p (NAVSWITCH_PUSH))
        {
            transmit (codeseq, size);
            show_char ('T');
        }

        if (ir_rx_get ())
        {
            size = capture (codeseq, CODESEQ_LEN_MAX);
            show_num (size == 0 ? 'E' : 'R', size);
            count++;
//            size = capture (codeseq, 9);
//            show_char ('0' + size);
        }
    }

    return 0;
}

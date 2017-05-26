/** @file   ir_grab1.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Test program for IR serial communnications.

    @defgroup ir_grab1 Test program for IR serial communications.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include "ir.h"
#include "delay.h"
#include "../fonts/font5x7_1.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 300

/* Define text update rate (characters per 10 s).  */
#define MESSAGE_RATE 10

/* At 2400 baud and 36 kHz modulation, there are 15 cycles per bit.  */
#define CODE_LEN_MAX 2000
#define CODESEQ_LEN_MAX 200

#define IR_MODULATION_PERIOD_US (1e6 / IR_MODULATION_FREQ)
#define TWEAK_US 0.5


typedef struct code
{
    uint16_t on;
    uint16_t off;
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
        uint16_t count;

        for (count = 0; count < CODE_LEN_MAX && ir_rx_get (); count++)
        {
            DELAY_US (IR_MODULATION_PERIOD_US - TWEAK_US);
        }
        codeseq[i].on = count;

        /* Something funny here.  */
        if (count >= CODE_LEN_MAX)
            return 0;

        for (count = 0; count < CODE_LEN_MAX && !ir_rx_get (); count++)
        {
            DELAY_US (IR_MODULATION_PERIOD_US - TWEAK_US);
        }
        codeseq[i].off = count;

        if (count >= CODE_LEN_MAX)
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


int main (void)
{
    uint8_t size = 0;
    code_t codeseq[CODESEQ_LEN_MAX];

    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);

    navswitch_init ();
    ir_init ();

    pacer_init (LOOP_RATE);

    show_char ('W');

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
            show_char (size == 0 ? 'E' : 'R');
//            size = capture (codeseq, 9);
//            show_char ('0' + size);
        }
    }

    return 0;
}

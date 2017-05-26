/** @file   ir_grab3.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Test program for IR serial communnications.

    @defgroup ir_grab3 Test program for IR serial communications.
*/

#include "system.h"
#include "navswitch.h"
#include "button.h"
#include "tinygl.h"
#include "pacer.h"
#include "ir.h"
#include "led.h"
#include "delay.h"
#include "eeprom.h"
#include "../fonts/font5x7_1.h"
#include <string.h>


/* Define polling rate in Hz.  */
#define LOOP_RATE 2000

/* Define text update rate (characters per 10 s).  */
#define MESSAGE_RATE 10

/* At 2400 baud and 36 kHz modulation, there are 15 cycles per bit.
   With a Sony start code of length 4.5 x 0.6 ms there are 86 cycles.  */
#define COUNT_MAX 250

#define CODESEQ_LEN_MAX 25

#define CODESEQ_NUM 5

#define IR_MODULATION_PERIOD_US (1e6 / IR_MODULATION_FREQ)

#define LOOP_TWEAK_US 1.5


typedef enum {STATE_NORMAL, STATE_LEARN, STATE_STORE} state_t;


typedef uint8_t count_t;

typedef struct code
{
    count_t on;
    count_t off;
} code_t;


static void transmit (code_t *codeseq)
{
    uint8_t i;

    for (i = 0; codeseq[i].on != 0; i++)
    {
        ir_tx_set (1, codeseq[i].on);
        ir_tx_set (0, codeseq[i].off);
    }
}


static uint8_t capture (code_t *codeseq, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size - 1; i++)
    {
        count_t count;

        for (count = 0; count < COUNT_MAX && ir_rx_get (); count++)
        {
            DELAY_US (IR_MODULATION_PERIOD_US - LOOP_TWEAK_US);
        }
        codeseq[i].on = count;

        for (count = 0; count < COUNT_MAX && !ir_rx_get (); count++)
        {
            DELAY_US (IR_MODULATION_PERIOD_US - LOOP_TWEAK_US - 1.0);
        }
        codeseq[i].off = count;

        if (count >= COUNT_MAX)
        {
            /* Mark end of sequence.  */
            codeseq[i + 1].on = 0;
            return i + 1;
        }
    }
    /* Sequence is too long.  */
    return 0;
}


static void codeseqs_write (code_t *codeseqs, uint8_t len, uint8_t num)
{
    eeprom_write (0, codeseqs, sizeof (codeseqs[0]) * len * num);
}


static void codeseqs_read (code_t *codeseqs, uint8_t len, uint8_t num)
{
    /* When the EEPROM is erased all the bytes are 0xFF so set to
       sensible defaults.  */
    eeprom_read (0, codeseqs, sizeof (codeseqs[0]) * len * num);
    if (codeseqs[0].on == 0xff)
    {
        uint8_t i;

        for (i = 0; i < num; i++)
            codeseqs[i * len].on = 0;

        codeseqs_write (codeseqs, len, num);
    }
}


static int switch_get (void)
{
    if (navswitch_push_event_p (NAVSWITCH_PUSH))
        return 0;
    else if (navswitch_push_event_p (NAVSWITCH_NORTH))
        return 1;
    else if (navswitch_push_event_p (NAVSWITCH_EAST))
        return 2;
    else if (navswitch_push_event_p (NAVSWITCH_SOUTH))
        return 3;
    else if (navswitch_push_event_p (NAVSWITCH_WEST))
        return 4;

    return -1;
}


int main (void)
{
    state_t state = STATE_NORMAL;
    code_t codeseq[CODESEQ_LEN_MAX];
    code_t codeseqs[CODESEQ_LEN_MAX * CODESEQ_NUM];
    int seq;
    static const char *strings[] = {"P", "N", "E", "S", "W"};

    codeseqs_read (codeseqs, CODESEQ_LEN_MAX, CODESEQ_NUM);

    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);

    navswitch_init ();
    button_init ();
    led_init ();
    ir_init ();

    pacer_init (LOOP_RATE);

    tinygl_text ("X");
    led_set (LED1, 0);

    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();
        
        switch (state)
        {
        case STATE_NORMAL:

            tinygl_update ();
            navswitch_update ();
            button_update ();

            seq = switch_get ();

            if (seq != -1)
            {
                tinygl_text (strings[seq]);
                transmit (&codeseqs[seq * CODESEQ_LEN_MAX]);
            }
                    
            if (button_push_event_p (BUTTON1))
            {
                led_set (LED1, 1);
                state = STATE_LEARN;
            }
            break;

        case STATE_LEARN:

            while (1)
            {
                /* Loop as fast as possible so that capture start of IR transmission
                   as soon as possible.  */
                button_update ();            
                
                if (ir_rx_get ())
                {
                    if (capture (codeseq, CODESEQ_LEN_MAX))
                    {
                        tinygl_text ("?");
                        led_set (LED1, 1);
                        state = STATE_STORE;
                        break;
                    }
                }
                
                if (button_push_event_p (BUTTON1))
                {
                    led_set (LED1, 0);
                    state = STATE_NORMAL;
                    break;
                }
            }
            break;

        case STATE_STORE:

            tinygl_update ();
            navswitch_update ();
            button_update ();

            seq = switch_get ();

            if (seq != -1)
            {
                memcpy (&codeseqs[seq * CODESEQ_LEN_MAX], codeseq, sizeof (codeseq));
                codeseqs_write (codeseqs, CODESEQ_LEN_MAX, CODESEQ_NUM);
                tinygl_text (strings[seq]);
                led_set (LED1, 0);
                state = STATE_NORMAL;
            }
                    
            if (button_push_event_p (BUTTON1))
            {
                led_set (LED1, 0);
                state = STATE_NORMAL;
            }
            break;
        }
    }

    return 0;
}

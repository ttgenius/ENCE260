/** @file   ir.c
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Infrared driver.
    @note   This should be compiled with optimisation enabled to make the
            modulation frequency more accurate.
*/
#include "system.h"
#include "delay.h"
#include "ir.h"
#include "pio.h"

#define IR_MODULATION_PERIOD_US (1e6 / IR_MODULATION_FREQ)

#define IR_SET_TWEAK_US 0.2
#define LOOP_TWEAK_US 0.8


/** Turn IR transmitter LED on.  */
static inline void ir_tx_on (void)
{
    pio_output_high (IR_TX_HIGH_PIO);
}


/** Turn IR transmitter LED off.  */
static inline void ir_tx_off (void)
{
    pio_output_low (IR_TX_HIGH_PIO);
}


/** Modulate the IR transmitter LED at IR_MODULATION_FREQ. 
    @param state is 1 to enable modulation, 0 to disable modulation
    @param count is the number of modulation periods
    @note This returns after @c count modulation periods.  */
void ir_tx_set (uint8_t state, uint16_t count)
{
    uint16_t i = 0;

    /* This modulates the IR LED by bit bashing the IR LED PIO.
       Alternatively, we could use the timer0 peripheral for a more
       accurate modulation frequency.

       The timing could be made more accurate by tweaking the delays
       to compensate for the other instructions.  */
       
    if (state)
    {
        for (i = 0; i < count; i++)
        {
            ir_tx_on ();
            DELAY_US (IR_MODULATION_PERIOD_US / 2 - IR_SET_TWEAK_US);
            ir_tx_off ();
            DELAY_US (IR_MODULATION_PERIOD_US / 2 - IR_SET_TWEAK_US - LOOP_TWEAK_US);
        }
    }
    else
    {
        for (i = 0; i < count; i++)
        {
            DELAY_US (IR_MODULATION_PERIOD_US - LOOP_TWEAK_US);
        }
    }
}


/** Return output state of IR receiver.
    @return IR receiver state (1 = IR modulation detected).  */
uint8_t ir_rx_get (void)
{
    /* The output of the IR receiver (TSOP6136TR) is inverted.  It is
       normally high but goes low when it detects modulated IR light.  */

    return !pio_input_get (IR_RX_PIO);
}


/** Initialise PIOs for IR transmitter LED and receiver.  */
void ir_init (void)
{
    /* Configure transmitter PIOs as outputs and ensure IR LED off.
       Note that the IR LED is driven by two PIOs; to turn on the LED
       IR_TX_HIGH_PIO must be high and IR_TX_LOW_PIO must be low.
       IR_TX_HIGH_PIO is also connected to the timer 0 output so
       we can modulate the LED by the timer.  IR_TX_LOW_PIO is also
       connected to the UART TXD output so we can drive it from the UART.  */
       
    pio_config_set (IR_TX_HIGH_PIO, PIO_OUTPUT_LOW);
    pio_config_set (IR_TX_LOW_PIO, PIO_OUTPUT_LOW);

    /* Configure receiver PIO as input.  IR_RX_PIO is also connected to the
       UART RXD input.  */
    pio_config_set (IR_RX_PIO, PIO_INPUT);
}

/** @file   ir_serial.c
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Infrared serial driver.
*/

/* This should probably be compiled with optimisation enabled
   otherwise some of the timing may be dodgy. 
   For documentation of the protocol see ir_serial.h  */


#include "ir_serial.h"
#include "delay.h"

#define IR_SERIAL_DIT_PERIOD 0.6e-3
#define IR_SERIAL_MODULATIONS_PER_DIT ((int)(IR_SERIAL_DIT_PERIOD * IR_MODULATION_FREQ))
#define IR_SERIAL_MODULATIONS_PER_START (IR_SERIAL_MODULATIONS_PER_DIT * 4)
#define IR_SERIAL_MODULATIONS_PER_ONE   (IR_SERIAL_MODULATIONS_PER_DIT * 2)
#define IR_SERIAL_MODULATIONS_PER_ZERO  (IR_SERIAL_MODULATIONS_PER_DIT)
#define IR_SERIAL_MODULATIONS_PER_BREAK  (IR_SERIAL_MODULATIONS_PER_DIT)

#define IR_SERIAL_DELAY_US 10
#define IR_SERIAL_DIT_COUNT (1e6 * IR_SERIAL_DIT_PERIOD / IR_SERIAL_DELAY_US)
#define IR_SERIAL_START_COUNT_MAX ((int)(4.5 * IR_SERIAL_DIT_COUNT))
#define IR_SERIAL_ONE_COUNT_MAX ((int)(2.5 * IR_SERIAL_DIT_COUNT))
#define IR_SERIAL_ZERO_COUNT_MAX ((int)(1.25 * IR_SERIAL_DIT_COUNT))
#define IR_SERIAL_BREAK_COUNT_MAX ((int)(1.5 * IR_SERIAL_DIT_COUNT))


/** Transmit a start code.  */
static inline void ir_serial_tx_start (void)
{
    ir_tx_set (1, IR_SERIAL_MODULATIONS_PER_START);
    ir_tx_set (0, IR_SERIAL_MODULATIONS_PER_BREAK);
}


/** Transmit a data bit.  */
static inline void ir_serial_tx_bit (uint8_t state)
{
    ir_tx_set (1, state ? IR_SERIAL_MODULATIONS_PER_ONE 
               : IR_SERIAL_MODULATIONS_PER_ZERO);
    ir_tx_set (0, IR_SERIAL_MODULATIONS_PER_BREAK);
}


/** Transmit a stop code.  */
static inline void ir_serial_tx_stop (void)
{
    ir_tx_set (0, IR_SERIAL_MODULATIONS_PER_BREAK);
}


/** Transmit 8 bits of data over IR serial link.
    @param data byte to transmit
    @note No error checking is performed.  This function blocks until 
    the frame is transmitted.  */
void ir_serial_transmit (uint8_t data)
{
    int i;

    /* Send start code.  */
    ir_serial_tx_start ();
    
    /* Eight data bits LSB first.  */
    for (i = 0; i < 8; i++)
    {
        ir_serial_tx_bit (data & 1);
        data >>= 1;
    }

    /* Send stop code.  */
    ir_serial_tx_stop ();
}


/** Receive 8 bits of data over IR serial link.  
    @param pdata pointer to byte to store received data
    @return status code
    @note No error checking is performed.  If there is no activity on the
    IR serial link, this function returns immediately.  Otherwise, this
    function blocks until the entire frame is received.  */
ir_serial_ret_t ir_serial_receive (uint8_t *pdata)
{
    int i;
    int count;
    uint8_t data;
    bool data_err;

    /* Check for start code; if not present return.  */
    if (!ir_rx_get ())
        return IR_SERIAL_NONE;

    /* Wait for end of start code or timeout.  */
    for (count = 0; ir_rx_get (); count++)
    {
        if (count >= IR_SERIAL_START_COUNT_MAX)
            return IR_SERIAL_START_ERR;

        DELAY_US (IR_SERIAL_DELAY_US);
    }

    /* We may have received a rogue short pulse but we may have been
       called closed to the falling transition.  */

    data_err = 0;
    data = 0;

    /* Eight data bits LSB first.  */
    for (i = 0; i < 8; i++)
    {
        data >>= 1;

        /* Wait for IR modulation to start or timeout (indicating
           detection of a false start code).  */
        for (count = 0; !ir_rx_get (); count++)
        {
            if (count >= IR_SERIAL_BREAK_COUNT_MAX)
                return IR_SERIAL_BREAK_ERR;

            DELAY_US (IR_SERIAL_DELAY_US);
        }

        /* If there is another transmission in this slot we will get
           too small a value for dit_count.  This is likely to trigger
           a bit error.  */
        for (count = 0; count < IR_SERIAL_ONE_COUNT_MAX
                 && ir_rx_get (); count++)
        {
            DELAY_US (IR_SERIAL_DELAY_US);
        }

        if (count >= IR_SERIAL_ONE_COUNT_MAX)
            data_err = 1;
            
        if (count >= IR_SERIAL_ZERO_COUNT_MAX)
            data |= 0x80;
    }

    /* Perhaps should check for stop code.  If not found, there is
       likely to have been interference from another transmitter.  */

    *pdata = data;
    return data_err ? IR_SERIAL_DATA_ERR : IR_SERIAL_OK;
}


/** Initialise IR serial driver.  */
void ir_serial_init (void)
{
    ir_init ();
}

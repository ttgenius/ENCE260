/** @file   ir_serial.h
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Infrared serial driver.

    @defgroup IR_serial Infrared serial driver
       
    This module implements a simple serial protocol over an IR link.
    It uses a pulse width modulation technique similar to the Sony
    SIRC protocol.  A message consists of a start code, 8 data bits
    (transmitted LSB first), and a stop code.  Each code is comprised
    of 0.6 ms intervals called a dit:

      Start bit: four dits on, one dit off

      One bit:   two dits on, one dit off

      Zero bit:  one dit on, one dit off

      Stop bit:  one dit off

    Note, the duration of a message depends on the number of non-zero
    bits in the data byte.  The shortest message duration is 22 dits
    (13.2 ms) and the longest message is 30 dits (18 ms).  No error
    detection or correction is performed apart from checking of
    invalid codes.

    Here's a simple application that transmits a data byte of value 7.

       @code
       #include "ir_serial.h"

       void main (void)
       {
           system_init ();
           ir_serial_init ();

           ir_serial_transmit (7);

           while (1)
           {
           }
       }
       @endcode

    Here's another simple application that receives a data byte.

       @code
       #include "ir_serial.h"
       #include "pacer.h"

       void main (void)
       {
           system_init ();
           ir_serial_init ();

           pacer_init (1000);

           while (1)
           {
               ir_serial_ret_t ret;

               pacer_wait ();

               ret = ir_serial_receive (&data);
               if (ret == IR_SERIAL_OK)
               {
                   // Process the received byte.
               }
           }
        }
       @endcode
*/

#ifndef IR_SERIAL_H
#define IR_SERIAL_H

#include "ir.h"

/** Status return codes.  */
typedef enum ir_serial_ret 
{
    /** A valid frame has been received.  */
    IR_SERIAL_OK = 1,
    /** No data to read.  */
    IR_SERIAL_NONE = 0,
    /** Invalid start code detected.  */
    IR_SERIAL_START_ERR = -1,
    /** Invalid data code detected.  */
    IR_SERIAL_DATA_ERR = -2,
    /** Invalid break code detected.  */
    IR_SERIAL_BREAK_ERR = -3,
    /** Invalid stop code detected.  */
    IR_SERIAL_STOP_ERR = -4
} ir_serial_ret_t;


/** Transmit 8 bits of data over IR serial link.
    @param data byte to transmit
    @note No error checking is performed.  This function blocks until 
    the frame is transmitted.  */
void ir_serial_transmit (uint8_t data);


/** Receive 8 bits of data over IR serial link.  
    @param pdata pointer to byte to store received data
    @return status code
    @note No error checking is performed.  If there is no activity on the
    IR serial link, this function returns immediately.  Otherwise, this
    function blocks until the entire frame is received.  */
ir_serial_ret_t ir_serial_receive (uint8_t *pdata);


/** Initialise IR serial driver.  */
void ir_serial_init (void);

#endif

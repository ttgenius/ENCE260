/** @file   ir_uart.h
    @author Michael Hayes
    @date   10 December 2004

    @defgroup IR_uart Infrared uart driver

    Here's a simple application that transmits the ASCII code '?' repeatedly.

       @code
       #include "ir_uart.h"

       void main (void)
       {
           system_init ();
           ir_uart_init ();

           while (1)
           {
                ir_uart_putc ('?');
           }
       }
       @endcode

    Here's another simple application that receives data bytes.

       @code
       #include "ir_uart.h"

       void main (void)
       {
           system_init ();
           ir_uart_init ();

           while (1)
           {
               if (ir_uart_read_ready_p ())
               {
                   char ch;

                   ch = ir_uart_getc ();

                   // Process the received byte.
               }
           }
        }
       @endcode
*/

#ifndef IR_UART_H
#define IR_UART_H

#include "system.h"

#ifndef IR_UART_BAUD_RATE
#define IR_UART_BAUD_RATE 2400
#endif


/* Return non-zero if there is a character ready to be read.  */
bool
ir_uart_read_ready_p (void);


/* Read character from IR_UART.  This blocks if nothing
   is available to read.  */
int8_t
ir_uart_getc (void);


/* Return non-zero if a character can be written without blocking.  */
bool
ir_uart_write_ready_p (void);


/* Return non-zero if transmitter finished.  */
bool
ir_uart_write_finished_p (void);


/** Write character to IR_UART.  This blocks until the character can
    be written into the transmit buffer.  It does not check to see
    if there is any echoed character (see ir_uart_putc).  */
void
ir_uart_putc_nocheck (char ch);


/* Write character to IR_UART.  This blocks until the character is
   written.  It then checks if data has been received and if so, reads
   the data and throws it away on the assumption that it is electrical
   or optical echoing.  */
void
ir_uart_putc (char ch);


/* Write string to IR_UART.  */
void
ir_uart_puts (const char *str);


/* Initialise ir_uart and set baud rate.  */
uint8_t
ir_uart_init (void);

#endif

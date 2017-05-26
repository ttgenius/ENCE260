/** @file  ir_uart.h
    @author Michael Hayes
    @date   10 December 2004
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


/* Write character to IR_UART.  This returns zero if
   the character could not be written.  */
int8_t
ir_uart_putc (char ch);


/* Write string to IR_UART.  */
void
ir_uart_puts (const char *str);


/* Initialise ir_uart and set baud rate.  */
uint8_t
ir_uart_init (void);

#endif

/** @file   ir_uart.c
    @author M. P. Hayes, UCECE
    @date   1 Aug 2011
    @brief  This is just a stub that does nothing.
*/
#include "system.h"
#include "ir_uart.h"


/* Return non-zero if there is a character ready to be read.  */
bool
ir_uart_read_ready_p (void)
{
    return 0;
}


/* Read character from IR_UART.  This blocks if nothing
   is available to read.  */
int8_t
ir_uart_getc (void)
{
    return 0;
}


/* Return non-zero if a character can be written without blocking.  */
bool
ir_uart_write_ready_p (void)
{
    return 0;
}


/* Return non-zero if transmitter finished.  */
bool
ir_uart_write_finished_p (void)
{
    return 1;
}


/* Write character to IR_UART.  This returns zero if
   the character could not be written.  */
int8_t
ir_uart_putc (__unused__ char ch)
{
    return 0;
}


/* Write string to IR_UART.  */
void
ir_uart_puts (__unused__ const char *str)
{
}


/* Initialise ir_uart and set baud rate.  */
uint8_t
ir_uart_init (void)
{
    return 0;
}

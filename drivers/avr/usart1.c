/** @file  usart1.c
    @author Michael Hayes
    @date   10 March 2005

    @brief Routines for interfacing with the usart1 on an Atmega32u2
*/
#include "system.h"
#include "usart1.h"
#include <avr/io.h>

void
usart1_baud_divisor_set (uint16_t divisor)
{
    UBRR1H = (divisor - 1) >> 8;
    UBRR1L = (divisor - 1) & 0xFF;
}


uint8_t
usart1_init (const usart1_cfg_t *cfg)
{
    uint8_t bits;

    usart1_baud_divisor_set (cfg->baud_divisor);
    
    /* Default to 8 bits.  */
    bits = cfg->bits;
    if (bits < 5 || bits > 8)
        bits = 8;

    /* Enable receiver and transmitter.  */
    UCSR1B = BIT (RXEN1) | BIT (TXEN1);

    /* Set frame format: asynchronous USART, 1 stop bit, no parity.  */
    UCSR1C = ((bits - 5) << UCSZ10) | (0 << UPM10);

    return 1;
}


/** Return non-zero if there is a character ready to be read.  */
bool
usart1_read_ready_p (void)
{
    return (UCSR1A & BIT (RXC1)) != 0;
}


/** Return non-zero if a character can be written without blocking.  */
bool
usart1_write_ready_p (void)
{
    return (UCSR1A & BIT (UDRE1)) != 0;
}


/** Return non-zero if transmission finished.  This is useful for
    half-duplex operation.  */
bool
usart1_write_finished_p (void)
{
    return (UCSR1A & BIT (TXC1)) != 0;
}


/** Write character to USART1.  This blocks until the character can be
    written into the transmit register.  */
void
usart1_putc (char ch)
{
    while (!usart1_write_ready_p ())
        continue;                   

    /* Write a 1 to the TXC1 bit to clear it!  It will be set when the
       data is shifted out of the transmit shift register.  */
    UCSR1A |= BIT (TXC1);
    
    UDR1 = ch;
}


/** Read character from USART1.  This blocks until a character is read.  */
int8_t
usart1_getc (void)
{
    /* Wait for something in receive buffer.  */
    while (!usart1_read_ready_p ())
        continue;

    return UDR1;
}


/** Write string to USART1.  This blocks until the string is written.  */
void 
usart1_puts (const char *str)
{
    while (*str)
        usart1_putc (*str++);
}

/** @file   usart1.h
    @author Michael Hayes
    @date   10 December 2004

    @brief Routines for interfacing with the usart1 on an Atmega32u2
*/

#ifndef USART1_H
#define USART1_H

#include "system.h"

typedef struct usart1_cfg_struct
{
    uint16_t baud_divisor;
    uint8_t bits;
} usart1_cfg_t;



#define USART1_BAUD_DIVISOR(BAUD_RATE)  ((F_CPU / 16) / (BAUD_RATE))


/** Return non-zero if there is a character ready to be read.  */
bool
usart1_read_ready_p (void);


/** Read character from USART1.  This blocks if nothing is available to
    read.  */
int8_t
usart1_getc (void);


/** Return non-zero if a character can be written without blocking.  */
bool
usart1_write_ready_p (void);


/** Return non-zero if transmission finished.  This is useful for
    half-duplex operation.  */
bool
usart1_write_finished_p (void);


/** Write character to USART1.  This blocks until the character can be
    written into the transmit register.  */
void
usart1_putc (char ch);


/** Write string to USART1.  */
void
usart1_puts (const char *str);


/** Initialise usart1 and set baud rate.  */
uint8_t
usart1_init (const usart1_cfg_t *cfg);

#endif

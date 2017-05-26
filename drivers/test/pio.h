/** @file   pio.h
    @author M. P. Hayes, UCECE
    @date   11 Jan 2006
    @brief  PIO hardware abstraction for AVR microcontroller.
    @note   Macros and inline functions are used to avoid function
            call overhead and to allow compile-time constant folding. 

    @defgroup pio PIO driver
       
    This module implements a driver for general purpose I/O ports.
    Its purpose is to provide a microcontroller independent
    abstraction of a PIO pin.  Here's an example:

    @code
    #include "pio.h"
    
    #define LED_PIO PIO_DEFINE (PORT_B, 5)

    void main (void)
    {
        system_init ();
        pio_config_set (LED_PIO, PIO_OUTPUT_LOW);

        pio_output_high (LED_PIO);

        while (1);
    }
    @endcode

    In this example, when port B5 is configured as an output it is set
    low.
*/
#ifndef PIO_H
#define PIO_H

#include "system.h"
#include "avrtest.h"


/** Define port names, note not all the ports are available on some AVRs.  */

enum {PORT_A, PORT_B, PORT_C, PORT_D, PORT_E, PORT_F, PORT_G};

typedef volatile uint8_t *pio_port_t;
typedef uint8_t pio_mask_t;
typedef uint16_t pio_t;


/** Define PIO pin types.  The two flavours of PIO_OUTPUT are to
    ensure the port is configured without glitching due to the time
    taken before calling pio_output_set.  */
typedef enum pio_config_enum 
{
    PIO_INPUT = 1,          /** Configure as input pin.  */
    PIO_PULLUP,             /** Configure as input pin with pullup.  */
    PIO_OUTPUT_LOW,         /** Configure as output, initially low.  */
    PIO_OUTPUT_HIGH,        /** Configure as output, initially high.  */
} pio_config_t;


/** DDR  PORT Comment
     0     0  High impedance input
     0     1  Weak pullup input
     1     0  Output low
     1     1  Output high
*/


/** Define a PIO as a unique 16 bit number encoding the low part of
    the PORT address in the low byte and the bit mask in the high
    byte.  PORTB is used for the pattern since PORTA is not always
    defined for some AVRs.  */
#define PIO_DEFINE(PORT, PORTBIT) ((((PORT) - PORT_B) * 3) | (BIT(PORTBIT) << 8))


/** Private macro to lookup bitmask.  */
#define PIO_BITMASK_(PIO) ((PIO) >> 8)


/** Private macro to lookup port register.  */
#define PIO_PORT_(PIO) (&PORTB + ((PIO) & 0xff))


/** Private macro to map a pio to its corresponding data direction
   register (DDR).  NB, the DDR and PORT registers must be separated
   by the same number of bytes in all cases.  PORTB is used for the
   pattern since PORTA is not always defined for some AVRs.  */
#define PIO_DDR_(PIO) (*(PIO_PORT_ (PIO) + (&DDRB - &PORTB)))

/** Private macro to map a pio to its input register (PIN).  NB, the
   PIN and PORT registers must be separated by the same number of
   bytes in all cases.  PORTB is used for the pattern since PORTA is
   not always defined for some AVRs.  */
#define PIO_PIN_(pio) (*(PIO_PORT_ (pio) + (&PINB - &PORTB)))

/** Private macro to access a pio data register.  */
#define PIO_DATA_(pio) (*PIO_PORT_ (pio))


#ifdef DEBUG
/** Configure pio.
    @param pio PIO to configure
    @param config PIO configuration type
    @return non-zero for success.  */
bool pio_config_set (pio_t pio, pio_config_t config);


/** Return pio configuration
    @param pio 
    @return config  */
pio_config_t pio_config_get (pio_t pio);


/** Set pio high.
    @param pio  */
void pio_output_high (pio_t pio);


/** Set pio low. 
    @param pio  */
void pio_output_low (pio_t pio);


/** Toggle pio.
    @param pio  */
void pio_output_toggle (pio_t pio);


/** Read input state from pio. 
    @param pio
    @return input state of pio  */
bool pio_input_get (pio_t pio);


/** Read input state from pio. 
    @param pio
    @return input state of pio  */
bool pio_input_get (pio_t pio);


/** Get output state of pio. 
    @param pio
    @return output state of pio  */
bool pio_output_get (pio_t pio);


/** Set pio to desired state.
    @param pio
    @param state value to write pio  */
void pio_output_set (pio_t pio, bool state);

#else

/** Configure pio.
    @param pio PIO to configure
    @param config PIO configuration type
    @return non-zero for success.  */
static inline
bool pio_config_set (pio_t pio, pio_config_t config)
{
    switch (config)
    {
    case PIO_OUTPUT_LOW:
        PIO_DATA_ (pio) &= ~PIO_BITMASK_ (pio);
        PIO_DDR_ (pio) |= PIO_BITMASK_ (pio);
        return 1;

    case PIO_OUTPUT_HIGH:
        PIO_DATA_ (pio) |= PIO_BITMASK_ (pio);
        PIO_DDR_ (pio) |= PIO_BITMASK_ (pio);
        return 1;

    case PIO_INPUT:
        PIO_DDR_ (pio) &= ~PIO_BITMASK_ (pio);
        PIO_DATA_ (pio) &= ~PIO_BITMASK_ (pio);
        return 1;

    case PIO_PULLUP:
        PIO_DDR_ (pio) &= ~PIO_BITMASK_ (pio);
        PIO_DATA_ (pio) |= PIO_BITMASK_ (pio); 
        return 1;

    default:
        return 0;
    }
}


/** Return pio configuration
    @param pio 
    @return config  */
static inline
pio_config_t pio_config_get (pio_t pio)
{
    bool ddr;
    bool port;

    ddr = PIO_DDR_ (pio) & PIO_BITMASK_ (pio);
    port = PIO_DATA_ (pio) & PIO_BITMASK_ (pio);
    
    if (ddr)
    {
        if (port)
            return PIO_OUTPUT_HIGH;
        else
            return PIO_OUTPUT_LOW;
    }
    
    if (port)
        return PIO_PULLUP;
    
    return PIO_INPUT;
}



/** Set pio high.
    @param pio  */
static inline
void pio_output_high (pio_t pio)
{
    PIO_DATA_ (pio) |= PIO_BITMASK_ (pio);
}


/** Set pio low. 
    @param pio  */
static inline
void pio_output_low (pio_t pio)
{
    PIO_DATA_ (pio) &= ~PIO_BITMASK_ (pio);
}


/** Toggle pio.
    @param pio  */
static inline
void pio_output_toggle (pio_t pio)
{
    PIO_DATA_ (pio) ^= PIO_BITMASK_ (pio);
}


/** Read input state from pio. 
    @param pio
    @return input state of pio  */
static inline
bool pio_input_get (pio_t pio)
{
    return (PIO_PIN_ (pio) & PIO_BITMASK_ (pio)) != 0;
}


/** Get output state of pio. 
    @param pio
    @return output state of pio  */
static inline
bool pio_output_get (pio_t pio)
{
    return (PIO_DATA_ (pio) & PIO_BITMASK_ (pio)) != 0;
}


/** Set pio to desired state.
    @param pio
    @param state value to write pio  */
static inline
void pio_output_set (pio_t pio, bool state)
{
    state ? pio_output_high (pio) : pio_output_low (pio);
}

/** Set input state for pio (this is only for use by the test scaffold). 
    @param pio
    @param state value for pio input  */
static inline
void pio_input_set (pio_t pio, bool state)
{
    if (state)
        PIO_PIN_ (pio) |= PIO_BITMASK_ (pio);
    else
        PIO_PIN_ (pio) &= ~PIO_BITMASK_ (pio);
}
#endif

#endif

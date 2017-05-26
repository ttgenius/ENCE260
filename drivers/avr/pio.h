/** @file   pio.h
    @author M. P. Hayes, UCECE
    @date   11 Jan 2006
    @brief  PIO hardware abstraction for AVR microcontroller.

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
#ifdef __AVR__
#include <avr/io.h>
#endif

/** AVR microcontrollers use two flip-flops per PIO stored in two
    separate registers: the data direction register (DDRx) and the
    port register (PORTx).  The DDRx register controls the whether the
    pin is an input or an output; the PORTx register determines the
    output state when the pin is an output but also doubles as an
    enable for a pull-up resistor when the pin is an input.

    Mode:              DDRx    PORTx
    Input, no pull-up     0        0
    Input, pull-up        0        1
    Output, low           1        0
    Output, high          1        1

    These two registers cannot be changed at the same time so a
    transient intermediate state can occur if both these registers
    need to be changed.  This is only a concern for pins that need to
    switched between being an input and an output; say for a
    bidirectional communication link.

    Let's consider the scenario where we change DDRx before PORTx.

    If we want to reconfigure a PIO from an input with no pull-up to a
    high output it will be momentarily a low output.

    If we want to reconfigure a PIO from an input with pull-up to a
    low output it will be momentarily a high output.

    Let's consider the alternative scenario where we change DDRx after PORTx.

    If we want to reconfigure a PIO from an input with no pull-up to a
    high output it will be momentarily an input with a pull-up.  This
    is not a bad option since a high-impedance load will not notice.

    If we want to reconfigure a PIO from an input with pull-up to a
    low output it will be momentarily an input with no-pullup.

    p.s.  I am sure this subtle quirk has caused much grief to some
    embedded systems engineers.
 */



/** Define port names; note not all the ports are available on some AVRs.  */

enum {PORT_A, PORT_B, PORT_C, PORT_D, PORT_E, PORT_F, PORT_G};

typedef uint16_t pio_t;


/** Define PIO pin types.  The two flavours of PIO_OUTPUT are to
    ensure the port is configured without glitching due to the time
    taken before calling pio_output_set.  */
typedef enum pio_config_enum 
{
    PIO_INPUT = 1,          /** Configure as input pin.  */
    PIO_PULLUP,             /** Configure as input pin with pullup enabled.  */
    PIO_OUTPUT_LOW,         /** Configure as output, initially low.  */
    PIO_OUTPUT_HIGH,        /** Configure as output, initially high.  */
} pio_config_t;


/** Define the pins.  */
#define PA0_PIO PIO_DEFINE(PORT_A, 0)
#define PA1_PIO PIO_DEFINE(PORT_A, 1)
#define PA2_PIO PIO_DEFINE(PORT_A, 2)
#define PA3_PIO PIO_DEFINE(PORT_A, 3)
#define PA4_PIO PIO_DEFINE(PORT_A, 4)
#define PA5_PIO PIO_DEFINE(PORT_A, 5)
#define PA6_PIO PIO_DEFINE(PORT_A, 6)
#define PA7_PIO PIO_DEFINE(PORT_A, 7)

#define PB0_PIO PIO_DEFINE(PORT_B, 0)
#define PB1_PIO PIO_DEFINE(PORT_B, 1)
#define PB2_PIO PIO_DEFINE(PORT_B, 2)
#define PB3_PIO PIO_DEFINE(PORT_B, 3)
#define PB4_PIO PIO_DEFINE(PORT_B, 4)
#define PB5_PIO PIO_DEFINE(PORT_B, 5)
#define PB6_PIO PIO_DEFINE(PORT_B, 6)
#define PB7_PIO PIO_DEFINE(PORT_B, 7)

#define PC0_PIO PIO_DEFINE(PORT_C, 0)
#define PC1_PIO PIO_DEFINE(PORT_C, 1)
#define PC2_PIO PIO_DEFINE(PORT_C, 2)
#define PC3_PIO PIO_DEFINE(PORT_C, 3)
#define PC4_PIO PIO_DEFINE(PORT_C, 4)
#define PC5_PIO PIO_DEFINE(PORT_C, 5)
#define PC6_PIO PIO_DEFINE(PORT_C, 6)
#define PC7_PIO PIO_DEFINE(PORT_C, 7)

#define PD0_PIO PIO_DEFINE(PORT_D, 0)
#define PD1_PIO PIO_DEFINE(PORT_D, 1)
#define PD2_PIO PIO_DEFINE(PORT_D, 2)
#define PD3_PIO PIO_DEFINE(PORT_D, 3)
#define PD4_PIO PIO_DEFINE(PORT_D, 4)
#define PD5_PIO PIO_DEFINE(PORT_D, 5)
#define PD6_PIO PIO_DEFINE(PORT_D, 6)
#define PD7_PIO PIO_DEFINE(PORT_D, 7)


#ifdef DEBUG

/* Define PIO as a unique integer.  */
#define PIO_DEFINE(PORT, PORTBIT) ((PORT) * 8 + (PORTBIT))


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


/** Get output state of pio. 
    @param pio
    @return output state of pio  */
bool pio_output_get (pio_t pio);


/** Set pio to desired state.
    @param pio
    @param state value to write pio  */
void pio_output_set (pio_t pio, bool state);







#else

/* WARNING: BEWARE ALL YE WHO LOOK BELOW!  IT MAY DO YOUR HEAD IN.

   The following uses advanced C techniques such as macros and inline
   functions to achieve a fast PIO implementation.  */


/** Define a PIO as a unique 16 bit number encoding the low part of
    the PORT address offset in the low byte and the bit mask in the
    high byte.  PORTB is used for the pattern since PORTA is not
    always defined for some AVRs.  */
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
#define PIO_PIN_(PIO) (*(PIO_PORT_ (PIO) + (&PINB - &PORTB)))


/** Private macro to access a pio data register.  */
#define PIO_DATA_(PIO) (*PIO_PORT_ (PIO))


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
        PIO_DDR_ (pio) |= PIO_BITMASK_ (pio);
        PIO_DATA_ (pio) &= ~PIO_BITMASK_ (pio);
        return 1;

    case PIO_OUTPUT_HIGH:
        PIO_DDR_ (pio) |= PIO_BITMASK_ (pio);
        PIO_DATA_ (pio) |= PIO_BITMASK_ (pio);
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
    @param state value to write to pio  */
static inline
void pio_output_set (pio_t pio, bool state)
{
    state ? pio_output_high (pio) : pio_output_low (pio);
}
#endif

#endif

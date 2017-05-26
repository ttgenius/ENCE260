/** @file   pio.c
    @author M. P. Hayes, UCECE
    @date   11 Jan 2006
    @brief  PIO hardware abstraction for AVR microcontroller.
*/
#include "system.h"
#include "pio.h"

#ifdef DEBUG


/** DDR  PORT Comment
     0     0  High impedance input
     0     1  Weak pullup input
     1     0  Output low
     1     1  Output high
*/

#define PIO_BITMASK(PIO) (BIT((PIO) & 7))

#define PIO_PORT(PIO) ((PIO) >> 3)


/** Configure pio.
    @param pio PIO to configure
    @param config PIO configuration type
    @return non-zero for success.  */
bool pio_config_set (pio_t pio, pio_config_t config)
{
    uint8_t bitmask;

    bitmask = PIO_BITMASK (pio);

    switch (PIO_PORT (pio))
    {
    case PORT_B:
        switch (config)
        {
        case PIO_INPUT:
            DDRB &= ~bitmask;
            PORTB &= ~bitmask;
            break;
            
        case PIO_PULLUP:
            DDRB &= ~bitmask;
            PORTB |= bitmask;
            break;
            
        case PIO_OUTPUT_LOW:
            PORTB &= ~bitmask;
            DDRB |= bitmask;
            break;

        case PIO_OUTPUT_HIGH:
            PORTB |= bitmask;
            DDRB |= bitmask;
            break;
            
        default:
            return 0;
        }
        break;
        
    case PORT_C:
        switch (config)
        {
        case PIO_INPUT:
            DDRC &= ~bitmask;
            PORTC &= ~bitmask;
            break;
            
        case PIO_PULLUP:
            DDRC &= ~bitmask;
            PORTC |= bitmask;
            break;
            
        case PIO_OUTPUT_LOW:
            PORTC &= ~bitmask;
            DDRC |= bitmask;
            break;

        case PIO_OUTPUT_HIGH:
            PORTC |= bitmask;
            DDRC |= bitmask;
            break;
            
        default:
            return 0;
        }
        break;
        
    case PORT_D:
        switch (config)
        {
        case PIO_INPUT:
            DDRD &= ~bitmask;
            PORTD &= ~bitmask;
            break;
            
        case PIO_PULLUP:
            DDRD &= ~bitmask;
            PORTD |= bitmask;
            break;
            
        case PIO_OUTPUT_LOW:
            PORTD &= ~bitmask;
            DDRD |= bitmask;
            break;

        case PIO_OUTPUT_HIGH:
            PORTD |= bitmask;
            DDRD |= bitmask;
            break;
            
        default:
            return 0;
        }
        break;
        
    default:
        return 0;
    }
    return 1;
}


/** Set pio high.
    @param pio  */
void pio_output_high (pio_t pio)
{
    uint8_t bitmask;

    bitmask = PIO_BITMASK (pio);

    switch (PIO_PORT (pio))
    {
    case PORT_B:
        PORTB |= bitmask;
        break;
         
    case PORT_C:
        PORTC |= bitmask;
        break;
        
    case PORT_D:
        PORTD |= bitmask;
        break;
        
    default:
        break;
    }
}


/** Set pio low. 
    @param pio  */
void pio_output_low (pio_t pio)
{
    uint8_t bitmask;

    bitmask = PIO_BITMASK (pio);

    switch (PIO_PORT (pio))
    {
    case PORT_B:
        PORTB &= ~bitmask;
        break;
         
    case PORT_C:
        PORTC &= ~bitmask;
        break;
        
    case PORT_D:
        PORTD &= ~bitmask;
        break;
        
    default:
        break;
    }
}


/** Toggle pio.
    @param pio  */
void pio_output_toggle (pio_t pio)
{
    uint8_t bitmask;

    bitmask = PIO_BITMASK (pio);

    switch (PIO_PORT (pio))
    {
    case PORT_B:
        PORTB ^= bitmask;
        break;
         
    case PORT_C:
        PORTC ^= bitmask;
        break;
        
    case PORT_D:
        PORTD ^= bitmask;
        break;
        
    default:
        break;
    }
}


/** Read input state from pio. 
    @param pio
    @return input state of pio  */
bool pio_input_get (pio_t pio)
{
    uint8_t bitmask;

    bitmask = PIO_BITMASK (pio);

    switch (PIO_PORT (pio))
    {
    case PORT_B:
        return (PINB & bitmask) != 0;

    case PORT_C:
        return (PINC & bitmask) != 0;

    case PORT_D:
        return (PIND & bitmask) != 0;
        
    default:
        return 0;
    }
}


/** Set pio to desired state
    @param pio
    @param state value to write pio  */
void pio_output_set (pio_t pio, bool state)
{
    if (state)
        pio_output_high (pio);
    else
        pio_output_low (pio);
}


/** Return pio configuration
    @param pio 
    @return config  */
pio_config_t pio_config_get (pio_t pio)
{
    bool ddr;
    bool port;
    uint8_t bitmask;

    bitmask = PIO_BITMASK (pio);
    
    switch (PIO_PORT (pio))
    {
    case PORT_B:
        ddr = (DDRB & bitmask) != 0;
        port = (DDRB & bitmask) != 0;
        break;
        
    case PORT_C:
        ddr = (DDRC & bitmask) != 0;
        port = (DDRC & bitmask) != 0;
        break;
        
    case PORT_D:
        ddr = (DDRD & bitmask) != 0;
        port = (DDRD & bitmask) != 0;
        break;
        
     default:
         return 0;
     }
    
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


/** Get output state of pio. 
    @param pio
    @return output state of pio  */
bool pio_output_get (pio_t pio)
{
    uint8_t bitmask;

    bitmask = PIO_BITMASK (pio);

    switch (PIO_PORT (pio))
    {
    case PORT_B:
        return (PORTB & bitmask) != 0;

    case PORT_C:
        return (PORTC & bitmask) != 0;

    case PORT_D:
        return (PORTD & bitmask) != 0;
        
    default:
        return 0;
    }
}


#endif

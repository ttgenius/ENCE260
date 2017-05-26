/** @file   pio.c
    @author M. P. Hayes, UCECE
    @date   11 Jan 2006
    @brief  PIO hardware abstraction for AVR microcontroller.
    @note   Inline functions are used in pio.h instead for performance.
*/
#include "system.h"
#include "pio.h"

#ifdef DEBUG

/** Configure pio.
    @param pio PIO to configure
    @param config PIO configuration type
    @return non-zero for success.  */
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
void pio_output_high (pio_t pio)
{
    PIO_DATA_ (pio) |= PIO_BITMASK_ (pio);
}


/** Set pio low. 
    @param pio  */
void pio_output_low (pio_t pio)
{
    PIO_DATA_ (pio) &= ~PIO_BITMASK_ (pio);
}


/** Toggle pio.
    @param pio  */
void pio_output_toggle (pio_t pio)
{
    PIO_DATA_ (pio) ^= PIO_BITMASK_ (pio);
}


/** Read input state from pio. 
    @param pio
    @return input state of pio  */
bool pio_input_get (pio_t pio)
{
    return (PIO_PIN_ (pio) & PIO_BITMASK_ (pio)) != 0;
}


/** Get output state of pio. 
    @param pio
    @return output state of pio  */
bool pio_output_get (pio_t pio)
{
    return (PIO_DATA_ (pio) & PIO_BITMASK_ (pio)) != 0;
}


/** Set pio to desired state.
    @param pio
    @param state value to write pio  */
void pio_output_set (pio_t pio, bool state)
{
    state ? pio_output_high (pio) : pio_output_low (pio);
}

#endif

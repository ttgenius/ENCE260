/** @file   adc.c
    @author Michael Hayes
    @date   3 Feb 2005
    @brief  Routines to use AVR onboard ADC in polling mode.
*/

#include "system.h"
#include "adc.h"
#include "bits.h"

/* ADC Clock Prescaler : 2, 2, 4, 8, 16, 32, 64, 128 =
   2^ADCCLKPRE.  Prescale ADC clock by 64 (13 cycles * (64
   prescaler) / 12MHz = 70 us conversion time).  */
#define ADCCLKPRE 6


void adc_reference_select (adc_ref_mode_t mode)
{
    BITS_INSERT (ADMUX, mode, 6, 7);
}


/* Initalise the ADC.  */
void adc_init (uint8_t channels)
{
    adc_reference_select (ADC_REF_AVCC);

    /* Right adjust result.  */
    ADMUX &= ~BIT (ADLAR);

    /* Enable ADC, prescale ADC clock by 64.   */
    ADCSRA |= BIT (ADEN) | BIT (ADPS2) | BIT (ADPS1);

#ifdef ADATE
    /* Disable ADC auto trigger, disable ADC interrupt.  */
    ADCSRA &= ~ (BIT (ADATE) | BIT (ADIE) | BIT (ADPS0));
#else
    ADCSRA &= ~ (BIT (ADIE) | BIT (ADPS0));
#endif

    /* Disable digital input buffers for selected ADC channels.  */    
#ifdef DIDR0
    DIDR0 = channels;
#endif
}


/* Starts a conversion in the ADC on the specified channel.  */
bool adc_start (adc_channel_t channel)
{
    if (channel >= ADC_NUM)
	return 0;

    BITS_INSERT (ADMUX, channel, 0, 3);

    if (!adc_ready_p ())
	return 0;

    /* Start conversion.  */
    ADCSRA |= BIT (ADSC);
    return 1;
}


/* Returns true if a conversion is not in progress.  */
bool adc_ready_p (void)
{
    return ! (ADCSRA & BIT (ADSC));
}


/** Read the current ADC value.  
    @return the ADC value. 
    @note this blocks until the ADC has finished a conversion.   */
adc_sample_t
adc_read (void)
{
    while (!adc_ready_p ())
        continue;

    return ADC;
}


/** Disables the ADC.  */
void adc_disable (void)
{
    /* Disable ADC, Disable ADC Interrupt.  */
    ADCSRA &= ~ (BIT (ADEN) | BIT (ADIE));
}

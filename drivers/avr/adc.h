/** @file   adc.h
    @author M. P. Hayes, UCECE
    @date   3 Feb 2005

    @brief Routines to use AVR onboard ADC in both a polling mode
           and automatically updating mode.
*/

#ifndef ADC_H
#define ADC_H

#include "system.h"

/** ADC channels.  */
typedef enum {ADC0 = 0, ADC1, ADC2, ADC3, ADC4, ADC5, ADC6, ADC7, ADC_NUM}
    adc_channel_t;

/** ADC reference modes.  */
typedef enum {ADC_REF_EXT = 0, ADC_REF_AVCC, ADC_REF_RESERVED, ADC_REF_2V56}
    adc_ref_mode_t;

/** ADC bits per conversion.  */
enum {ADC_BITS = 10};

/** ADC sample size.  */
typedef uint16_t adc_sample_t;


/** Select ADC reference mode.  */
extern void 
adc_reference_select (adc_ref_mode_t mode);


/** Initalises the ADC registers for polling operation.  */
extern void 
adc_init (uint8_t channels);


/** Starts a conversion in the ADC on the specified channel.  */
extern bool
adc_conversion_start (adc_channel_t channel);


/** Returns true if a conversion is not in progress.  */
extern bool
adc_ready_p (void);


/** Returns 1 if valid sample read.  */
extern int8_t
adc_read (adc_sample_t *value);


/** Halts any currently running conversion.  */
extern void 
adc_stop (void);


/** Disables the ADC from doing anything.  Requires reinitalisation.  */
extern void
adc_disable (void);

#endif

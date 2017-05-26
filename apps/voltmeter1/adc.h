/** @file   adc.h
    @author M. P. Hayes, UCECE
    @date   23 April 2013
    @brief  Simple ADC using a comparator and RC network.
*/

#ifndef ADC_H
#define ADC_H

#include "system.h"

void adc_init(void);

void adc_enable(uint8_t adc_channel);

void adc_disable(uint8_t adc_channel);

uint8_t adc_measure(uint8_t adc_channel);


#endif



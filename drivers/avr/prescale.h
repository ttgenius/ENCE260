/** @file   prescale.h
    @author M. P. Hayes, UCECE
    @date   3 Aug 2011
    @brief  Prescaler selection for timer/counters.
*/

#ifndef PRESCALE_H
#define PRESCALE_H

#include "system.h"

uint8_t prescale_select (uint16_t period, const uint8_t *log_prescales,
                         uint8_t num_prescales, uint8_t *pcounter);

#endif

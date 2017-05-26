/** @file   prescale.c
    @author M. P. Hayes, UCECE
    @date   3 Aug 2011
    @brief  Prescaler selection for 8-bit timer/counters.
*/
#include "system.h"
#include "prescale.h"
#include <stdlib.h>

uint8_t prescale_select (uint16_t period, const uint8_t *log_prescales,
                         uint8_t num_prescales, uint8_t *pcounter)
{
    uint8_t i;
    uint16_t counter = 0;

    /* If the desired period is T we need to choose the counter value
       c and the prescale value s to minimise |T - c x 2^p| with the
       constraints that 0 < c < 256 and 2^p is one of the available
       prescale values.  */

    /* Search through prescaler options looking for first one
       where the counter value is < 256.  */
    for (i = 0; i < num_prescales; i++)
    {
        uint16_t divisor;

        divisor = 1 << log_prescales[i];
        counter = (period + (divisor + 1) / 2) >> log_prescales[i];
        if (counter < 256)
            break;
    }

    *pcounter = counter;
    return i;
}

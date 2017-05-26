/** @file   spwm.c
    @author M. P. Hayes, UCECE
    @date   25 Sep 2013
    @brief  These routines are for software pulse width modulation.  They are
    useful for modulating the luiminance of an LED or beeping a piezo
    tweeter. 
*/
#include "system.h"
#include "spwm.h"


/** Set the period.  This must be greater than the duty.  */
void
spwm_period_set (spwm_t *spwm, spwm_period_t period)
{
    spwm->period = period;
}


/** Set the duty.  This must be less than the period.  */
void
spwm_duty_set (spwm_t *spwm, spwm_period_t duty)
{
    spwm->duty = duty;
}


/** Return whether spwm output should be high or low.
    For example, to control the flashing of a LED use:

    led_set (led, spwm_update (spwm));  */
bool
spwm_update (spwm_t *spwm)
{
    spwm->count++;
    if (spwm->count >= spwm->period)
        spwm->count = 0;

    return spwm->count < spwm->duty;
}


/* Reset spwm.  */
void
spwm_reset (spwm_t *spwm)
{
    spwm->count = 0;
}

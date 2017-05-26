/** @file   spwm.h
    @author M. P. Hayes, UCECE
    @date   25 Sep 2013
    @brief  These routines are for software pulse width modulation.  They are
    useful for modulating the luiminance of an LED or beeping a piezo
    tweeter. 
*/
#ifndef SPWM_H
#define SPWM_H

#include "system.h"


typedef uint8_t spwm_period_t;


/* This structure is defined here so the compiler can allocate enough
   memory for it.  However, its fields should be treated as
   private.  */
typedef struct spwm_struct
{
    spwm_period_t period;
    spwm_period_t duty;
    spwm_period_t count;
} spwm_t;


/** Set the period.  This must be greater than the duty.  */
void spwm_period_set (spwm_t *spwm, spwm_period_t period);


/** Set the duty.  This must be less than the period.  */
void spwm_duty_set (spwm_t *spwm, spwm_period_t duty);


/** Return whether spwm output should be high or low.
    For example, to control the flashing of a LED use:

    led_set (led, spwm_update (spwm));  */
bool spwm_update (spwm_t *spwm);


/* Reset spwm.  */
void spwm_reset (spwm_t *spwm);
#endif

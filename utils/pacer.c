/** @file   pacer.c
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Paced loop module
*/
#include "system.h"
#include "timer.h"
#include "pacer.h"

static timer_tick_t pacer_period;


/** Initialise pacer:
    @param pacer_rate rate in Hz.  */
void pacer_init (pacer_rate_t pacer_rate)
{
    timer_init ();

    pacer_period = TIMER_RATE / pacer_rate;
}


/** Wait until next pacer tick.  */
void pacer_wait (void)
{
    static timer_tick_t when = 0;

    timer_wait_until (when);
    when += pacer_period;
}

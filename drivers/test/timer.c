/** @file   timer.c
    @author M. P. Hayes, UCECE
    @date   21 August 2007
    Description:  Support for timer.  
*/
#include "timer.h"
#include "unistd.h"
#include "time.h"
#include "system.h"

#include <stdio.h>

static timer_tick_t offset;


/* Initialise timer.  */
void timer_init (void)
{
    /* Save offset so timer starts around 0.  */
    offset = timer_get ();
}


/** Get current time:
    @return current time in ticks.  */
timer_tick_t timer_get (void)
{
    struct timespec time;
    uint64_t time_us;
    uint64_t ticks;

    clock_gettime (CLOCK_MONOTONIC, &time);

    time_us = time.tv_sec * 1000000 + time.tv_nsec / 1000;
    ticks = time_us * TIMER_RATE / 1000000;

    return (timer_tick_t) ticks - offset;
}


/** Wait until specified time:
    @param when time to sleep until
    @return current time.  */
timer_tick_t timer_wait_until (timer_tick_t when)
{
    while (1)
    {
        timer_tick_t diff;
        timer_tick_t now;
        
        now = timer_get ();

        diff = now - when;

        /* Allow for 1000 tick overrun.  */
        if (diff < TIMER_OVERRUN_MAX)
            return now;

        diff = when - now;

        usleep (diff * 1e6 / TIMER_RATE);
    }
}


/** Wait for specified period:
    @param period how long to wait
    @return current time.  */
timer_tick_t timer_wait (timer_tick_t period)
{
    return timer_wait_until (timer_get () + period);
}

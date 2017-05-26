/** @file   timer.c
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Timer module
*/
#include <avr/io.h>
#include "timer.h"
#include "system.h"

/* Timer/counter1 is a 16-bit counter with prescale options 1, 8, 64,
   256, and 1024.  With F_CPU = 8e6 this corresponds to clock rates of
   8 MHz, 1 MHz, 125 kHz, 31.25 kHz, 7.8125 kHz.  The counter will
   roll over every 8.192 ms, 65.536 ms, 524.288 ms, 2.097s, and 8.389 s.  */

#if TIMER_CLOCK_DIVISOR == 1
#define TCCR1B_INIT 0x01
#elif TIMER_CLOCK_DIVISOR == 8
#define TCCR1B_INIT 0x02
#elif TIMER_CLOCK_DIVISOR == 64
#define TCCR1B_INIT 0x03
#elif TIMER_CLOCK_DIVISOR == 256
#define TCCR1B_INIT 0x04
#elif TIMER_CLOCK_DIVISOR == 1024
#define TCCR1B_INIT 0x05
#else
#error Invalid TIMER_CLOCK_DIVISOR
#endif


/** Initialise timer.  */
void timer_init (void)
{
    /* Start timer in normal mode so that it rolls over at 65535 to 0.  */
    TCCR1A = 0x00;
    TCCR1B = TCCR1B_INIT;
    TCNT1 = 0;
}


/** Get current time:
    @return current time in ticks.  */
timer_tick_t timer_get (void)
{
    return TCNT1;
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

        /* Note, both when and now are unsigned so they wrap around to
           zero when they overflow.  The difference is also unsigned
           so when now < when then the result wraps around to a large
           positive value and fails the following test.  */
        diff = now - when;

        /* There is a possibility that we are called when when < now.
           So we allow a possible overrun defined by
           TIMER_OVERRUN_MAX.  */
        if (diff < TIMER_OVERRUN_MAX)
            return now;
    }
}


/** Wait for specified period:
    @param period how long to wait
    @return current time.  */
timer_tick_t timer_wait (timer_tick_t period)
{
    return timer_wait_until (timer_get () + period);
}

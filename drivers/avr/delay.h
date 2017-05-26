/** @file   delay.h
    @author M. P. Hayes, UCECE
    @date   08 June 2002
    @brief  Delay routines for timing.
    @note   All files that use DELAY_US must be compiled with full optimisation.

    @defgroup delay Delay routines
       
    This module provides routines for short timing delays.  These are
    implemented as delay loops.  DELAY_US must not be called with a variable
    argument since the run-time floating point calculation will almost certainly
    swamp the delay you require.

    @note The macro F_CPU defines the CPU clock frequency (in Hertz). 
*/
#ifndef DELAY_H
#define DELAY_H

#include "system.h"
#include <util/delay.h>

/* There are two timing loop routines provided in delay.h:

    _delay_loop_1 uses an 8-bit counter and requires 3 cycles per
    iteration but has a maximum of 256 iterations.  With F_CPU = 8e6
    then the range of delays is 0.375 us to 96.0 us in steps of 0.375 us.

    _delay_loop_2 uses a 16-bit counter, requires 4 cycles per
    iteration and has a maximum of 65536 iterations.  With F_CPU = 8e6
    then the range of delays is 0.5 us to 32.768 ms in steps of 0.5 us.  */

#define DELAY1_LOOP_CYCLES 3
#define DELAY1_LOOPS_MAX 256
#define DELAY1_US_MAX (DELAY1_LOOPS_MAX * DELAY1_LOOP_CYCLES * 1e6 / (F_CPU))

#define DELAY2_LOOP_CYCLES 4
#define DELAY2_LOOPS_MAX 65536
#define DELAY2_US_MAX (DELAY2_LOOPS_MAX * DELAY2_LOOP_CYCLES * 1e6 / (F_CPU))


/** Delay a selected number of microseconds
    @param us number of microseconds to delay.  This must be a constant.
    @note This should be only used with a constant argument otherwise
    it will expand to a lot of code and require slow floating point operations to compute
    the number of cycles to delay.  */
#define DELAY_US(us)				\
do						\
{						\
    if ((us) > DELAY1_US_MAX)			\
    {						\
        double __tmp2 = ((double)F_CPU * (us)) / (DELAY2_LOOP_CYCLES * 1e6);	\
	uint16_t __ticks2;			\
						\
	if ((us) > DELAY2_US_MAX)		\
	    __ticks2 = 0;			\
	else					\
	    __ticks2 = (uint16_t)__tmp2;	\
	_delay_loop_2 (__ticks2);		\
    }						\
    else					\
    {						\
	double __tmp1 = ((double)F_CPU * (us)) / (DELAY1_LOOP_CYCLES * 1e6);	\
	uint8_t __ticks1;			\
						\
	if (__tmp1 < 1.0)			\
	    __ticks1 = 1;			\
	else					\
	    __ticks1 = (uint8_t)__tmp1;		\
	_delay_loop_1 (__ticks1);		\
    }						\
}						\
while (0)


/** Delay a selected number of milliseconds
    @param ms number of milliseconds to delay.  This can be a variable.  */
static __inline__ void
delay_ms (uint8_t ms)
{
    /* FIXME:  "i" should only need to be a uint8_t
      Some bug has occured using a newer version of the avr-gcc that
      causes this routine to only delay 1ms if both "ms" and "i"
      are uint8_t. */
    uint16_t i;

    for (i = 0; i < ms; i++)
    {
	DELAY_US (1000);
    }
}


#endif /* DELAY_H  */

/** @file   timer0.c
    @author M. P. Hayes, UCECE
    @date   15 May 2007
    @brief 
*/
#include "system.h"
#include "timer0.h"
#include "bits.h"
#include "prescale.h"
#include <avr/io.h>

/* Timer/Counter0 is a general purpose, single channel, 8-bit
   Timer/Counter module.

   It has two output compare registers and two corresponding outputs:
   OC0A and OCOB.  However, the counter can only be reset by the A
   output compure register, OCRA.

   TODO: rewrite driver to provide standard modes, such as:
   * generate square wave on OCOA
   * generate square wave on OCOB
   * generate PWM on OCOA
   * generate PWM on OCOB
   * programmable timer (for delays)
   
  */


/* Prescale values:
   1, 8, 64, 256, or 1024.

   2^0, 2^3, 2^6, 2^8, 2^10.  */

static const uint8_t log_prescales[] = {0, 3, 6, 8, 10};
static uint8_t prescale_bits = 1;



static inline void
timer0_prescaler_set (uint8_t value)
{
    BITS_INSERT (TCCR0B, value, CS00, CS02);
}


static inline uint8_t
timer0_prescaler_get (void)
{
    return BITS_EXTRACT (TCCR0B, CS00, CS02);
}


static inline bool
timer0_running_p (void)
{
    return timer0_prescaler_get () != 0;
}


/* Set the desired period in terms of clocks.  The closest
   suitable period is returned.  */
uint16_t timer0_period_set (uint16_t period)
{
    uint8_t counter;
    uint8_t index;

    index = prescale_select (period, log_prescales,
                             ARRAY_SIZE (log_prescales), &counter);

    prescale_bits = index + 1;


   /*  F = F_CPU / (2 * prescale * (1 + OCR0A))
       Maximum frequency of F_CPU / 2 when OCR0A = 0.  */
    OCR0A = counter - 1;
    OCR0B = 0;

    /* A prescale value of 0 selects no clock source and is used to
       stop the timer.  The cheapskates haven't provided a start/stop
       bit so we cannot set the prescaler if the timer is stopped.  */
    if (timer0_running_p ())
	timer0_prescaler_set (prescale_bits);

    /* Return the quantised period.  */
    return counter << log_prescales[index];
}


void
timer0_start (void)
{
    /* Clear overflow flags.  */
    TIFR0 &= ~(BIT (OCF0A) | BIT (OCF0B));

    /* Enable timer by supplying a clock from the prescaler.  */
    timer0_prescaler_set (prescale_bits);
}


void
timer0_stop (void)
{
    /* Disable clock source.  */
    timer0_prescaler_set (0);
}


bool
timer0_compare_p (void)
{
    return (TIFR0 & BIT (OCF0B)) != 0;
}


void timer0_output_set (timer0_output_t output, timer0_output_mode_t mode)
{
    switch (output)
    {
    case TIMER0_OUTPUT_A:
        /* Select output mode for 0C0A.
           The port pin must also be configured as an output.  */
        BITS_INSERT (TCCR0A, mode, COM0A0, COM0A1);
        break;

    case TIMER0_OUTPUT_B:
        /* Select output mode for 0C0B.
           The port pin must also be configured as an output.  */
        BITS_INSERT (TCCR0A, mode, COM0B0, COM0B1);
        break;

    default:
        break;
    }
}


int8_t
timer0_mode_set (timer0_mode_t mode)
{
    switch (mode)
    {
    case TIMER0_MODE_NORMAL:
        /* Normal mode---counter not reset.  It rolls over when it
           gets to 0xFF and thus there is limited frequency control
           for waveform generation.  */
        TCCR0A &= ~(BIT (WGM01) | BIT (WGM00));

        /* Need WGM02 bit set to 0 (default) in TCCR0B.  */
        break;

    case TIMER0_MODE_CTC:
        /* Set CTC mode.  Clear timer in compare mode---counter reset
           on match with OCRA.  */
        TCCR0A &= ~BIT (WGM00);
        TCCR0A |= BIT (WGM01);

        /* Need WGM02 bit set to 0 (default) in TCCR0B.  */
        break;

    default:
        return 0;
    }
    
    return 1;
}


int8_t
timer0_init (void)
{
    /* Set to normal mode, clock source disabled, output compare match
       to not trigger OC pin.  */
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;

    OCR0B = 0;

    timer0_mode_set (TIMER0_MODE_CTC);

    return 1;
}

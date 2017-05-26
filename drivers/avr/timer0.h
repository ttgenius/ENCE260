/** @file   timer0.h
    @author M. P. Hayes, UCECE
    @date   15 May 2007
    @brief 
*/
#ifndef TIMER0_H
#define TIMER0_H

#include "system.h"


#define TIMER0_DIVISOR(FREQ) ((uint16_t) (F_CPU / (FREQ)))


typedef enum {TIMER0_MODE_NORMAL,
              TIMER0_MODE_CTC,
              TIMER0_MODE_PWM} timer0_mode_t;

typedef enum {TIMER0_OUTPUT_A,
              TIMER0_OUTPUT_B} timer0_output_t;

typedef enum {TIMER0_OUTPUT_MODE_DISABLE,
              TIMER0_OUTPUT_MODE_TOGGLE,
              TIMER0_OUTPUT_MODE_CLEAR,
              TIMER0_OUTPUT_MODE_SET} timer0_output_mode_t;


uint16_t timer0_period_set (uint16_t period);


void timer0_start (void);


void timer0_stop (void);


bool timer0_compare_p (void);


void timer0_output_set (timer0_output_t output, timer0_output_mode_t mode);


int8_t timer0_mode_set (timer0_mode_t mode);


int8_t timer0_init (void);

#endif

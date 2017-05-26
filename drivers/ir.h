/** @file   ir.h
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Infrared driver.

    @defgroup IR Infrared (IR) driver
       
    This module implements a simple hardware abstraction of an IR LED and
    IR receiver.
*/

#ifndef IR_H
#define IR_H

#include "system.h"

#ifndef IR_MODULATION_FREQ
/** The nominal modulation frequency.  This can be defined in system.h.  */
#define IR_MODULATION_FREQ 36e3
#endif


/** Modulate the IR transmitter LED at IR_MODULATION_FREQ. 
    @param state is 1 to enable modulation, 0 to disable modulation
    @param count is the number of modulation periods
    @note This returns after @c count modulation periods.  */
void ir_tx_set (uint8_t state, uint16_t count);


/** Return output state of IR receiver.
    @return IR receiver state (1 = IR modulation detected).  */
uint8_t ir_rx_get (void);


/* Initialise PIOs for IR transmitter LED and receiver.  */
void ir_init (void);

#endif

/** @file   ledmat.h
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  LED matrix driver.

    @defgroup ledmat LED matrix driver
       
    This module implements a simple hardware abstraction of a LED matrix.
*/
#ifndef LEDMAT_H
#define LEDMAT_H

#include "system.h"

/** Initialise PIO pins to drive LED matrix.  */
void ledmat_init (void);


/** Display pattern on specified column.
    @param pattern bit pattern to display for selected column
    @param col selected column.  */
void ledmat_display_column (uint8_t pattern, uint8_t col);


#endif

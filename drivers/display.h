/** @file   display.h
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  Bit-mapped display driver.

    @defgroup display Bit-mapped display driver
       
    This module implements a simple display frame-buffer.  It abstracts the
    multiplexing of a LED matrix display.
*/
#ifndef DISPLAY_H
#define DISPLAY_H

#include "system.h"

#define DISPLAY_WIDTH LEDMAT_COLS_NUM
#define DISPLAY_HEIGHT LEDMAT_ROWS_NUM

/** Set state of a display pixel.
    @param col pixel column (0 left)
    @param row pixel row (0 top)
    @param val pixel state.  */
void display_pixel_set (uint8_t col, uint8_t row, bool val);


/** Get state of a display pixel.
    @param col pixel column (0 left)
    @param row pixel row (0 top)
    @return pixel state or zero if outside display.  */
bool display_pixel_get (uint8_t col, uint8_t row);


/** Update display (perform refreshing).  */
void display_update (void);


/** Clear display.   */
void display_clear (void);


/** Initialise display.   */
void display_init (void);

#endif

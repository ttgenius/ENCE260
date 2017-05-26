/** @file   font.h
    @author M. P. Hayes, UCECE
    @date   1 March 2007
    @brief  Font lookup.
*/

#ifndef FONT_H
#define FONT_H

#include "system.h"

typedef const uint8_t font_data_t;

/** Font structure.  */
typedef const struct
{
    /** Flags for future options.  */
    uint8_t flags;
    /** Width of font element.  */
    uint8_t width;
    /** Height of font element.  */
    uint8_t height;
    /** Index of first entry in font.  */
    uint8_t offset;
    /** Number of font entries in table.  */
    uint8_t size;
    /** Number of bytes per font entry.  */
    uint8_t bytes;
    /** Font element data.  */
    font_data_t data[];
} font_t;


/** Determine if character in font.
    @param font pointer to font structure
    @param ch character to check
    @return non-zero if character is in font.  */
bool font_contains_p (font_t *font, char ch);


/** Determine if pixel on or off.
    @param font pointer to font structure
    @param ch character to display
    @param col column of font element
    @param row row of font element
    @return 1 if pixel on; if pixel out of bounds return 0.  */
bool
font_pixel_get (font_t *font, char ch, uint8_t col, uint8_t row);

#endif

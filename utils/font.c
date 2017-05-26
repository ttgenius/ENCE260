/** @file   font.h
    @author M. P. Hayes, UCECE
    @date   18 Sep 2011
    @brief  Font lookup.
*/

#include "font.h"


/** Determine if character in font.
    @param font pointer to font structure
    @param ch character to check
    @return non-zero if character is in font.  */
bool font_contains_p (font_t *font, char ch)
{
    int8_t index;

    if (!font)
        return 0;

    index = ch - font->offset;
    return index >= 0 && index < font->size;
}


/** Determine if pixel on or off.
    @param font pointer to font structure
    @param ch character to display
    @param col column of font element
    @param row row of font element
    @return 1 if pixel on; if pixel out of bounds return 0.  */
bool font_pixel_get (font_t *font, char ch, uint8_t col, uint8_t row)
{
    int8_t index;
    font_data_t *char_data;
    uint8_t offset;

    if (!font)
        return 0;

    /* Check if desired pixel within size of an element.  */
    if (col >= font->width || row >= font->height)
        return 0;

    /* Find index of font element.  */
    index = ch - font->offset;
    if (index < 0 || index >= font->size)
        return 0;

    /* Get start of font element data for ch.  */
    char_data = &font->data[index * font->bytes];

    /* Extract whether pixel should be on or off.  */
    offset = row * font->width + col;
    return (char_data[offset >> 3] & BIT (offset % 8)) != 0;
}


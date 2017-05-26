/** @file   tinygl.h
    @author M. P. Hayes, UCECE
    @date   24 August 2010
    @brief  Tiny graphics library.

    @defgroup tinygl Tiny graphics library
       
       <h3>Introduction to Tinygl</h3>

       Tinygl is a tiny graphics library for a simple dot matrix
       display.  This file declares the data types and graphics
       primitives provided by tinygl.  The basic graphics primitive is
       a point.  This specifies a pair of (x, y) coordinates on the
       display.  Each point has an associated pixel value.  Currently,
       this can be 0 for off and 1 for on.


       <h3>A simple Tinygl application</h3>

       Here's a simple application that draws a line from (1, 2) to
       (3, 5).  The paced loop runs at 1 kHz and this sets the display
       refresh period to 200 Hz since there are 5 columns.

       @code
       #include "pacer.h"
       #include "tinygl.h"

       void main (void)
       {
           pacer_init (1000);
           tinygl_init (1000);

           tinygl_draw_line (tinygl_point (1, 2), tinygl_point (3, 5));

           while (1)
           {
               pacer_wait ();
               tinygl_update ();
           }
       }
       @endcode

       <h3>A Tinygl text application</h3>

       Here's another simple application that displays a message.

       @code
       #include "pacer.h"
       #include "tinygl.h"
       #include "../fonts/font5x7_1.h"

       void main (void)
       {
           pacer_init (1000);
           tinygl_init (1000);

           tinygl_font_set (&font5x7_1);
           tinygl_text_speed_set (10);
           tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
           tinygl_text ("HELLO WORLD ");

           while (1)
           {
               pacer_wait ();
               tinygl_update ();
           }
       }
       @endcode

*/

#ifndef TINYGL_H
#define TINYGL_H

#include "system.h"
#include "display.h"
#include "font.h"

/** Define size of display.  */
#define TINYGL_WIDTH DISPLAY_WIDTH
#define TINYGL_HEIGHT DISPLAY_HEIGHT


/** Maximum length of a message to display.  */
#define TINYGL_MESSAGE_SIZE 32


/** Define a pixel value.  Currently the only supported values are 0
    for off and 1 for on.  */
typedef uint8_t tinygl_pixel_value_t;


/** Define a display coordinate.  Note these are signed to allow
    relative coordinates.  */
typedef int8_t tinygl_coord_t;


/** Define a point as a pair of coordinates.  The display origin is the
    top left corner.  */
typedef struct tinygl_point
{
    tinygl_coord_t x;
    tinygl_coord_t y;
} tinygl_point_t;


/** Define text display modes.  */
typedef enum 
{
    /** Stepping text.  */
    TINYGL_TEXT_MODE_STEP, 
    /** Scrolling text.  */
    TINYGL_TEXT_MODE_SCROLL,
} tinygl_text_mode_t;


/** Define text display directions.  */
typedef enum 
{
    /** Normal text.  */
    TINYGL_TEXT_DIR_NORMAL, 
    /** Rotated text.  */
    TINYGL_TEXT_DIR_ROTATE, 
} tinygl_text_dir_t;


/** Construct a point from a pair of coordinates.
    @param x x coordinate
    @param y y coordinate   
    @return point.  */
static inline tinygl_point_t tinygl_point (tinygl_coord_t x, tinygl_coord_t y)
{
    tinygl_point_t point = {x, y};

    return point;
}


/** Set the message update speed.
    @param speed text advance speed (characters per 10 s).  */
void tinygl_text_speed_set (uint8_t speed);


/** Set the message display mode.
    @param mode display mode.  */
void tinygl_text_mode_set (tinygl_text_mode_t mode);


/** Set the message display direction.
    @param dir display direction.  */
void tinygl_text_dir_set (tinygl_text_dir_t dir);


/** Set the font to use for text.
    @param font pointer to font description.  */
void tinygl_font_set (font_t *font);


/** Draw character using current font.
    @param ch character to draw
    @param pos coordinates of top left position
    @return position to draw next character.  */
tinygl_point_t tinygl_draw_char (char ch, tinygl_point_t pos);


/** Draw string (well, as much as possible) using current font.
    @param str string to draw
    @param pos coordinates of top left position
    @return number of whole characters drawn.  */
uint8_t tinygl_draw_string (const char *str, tinygl_point_t pos);


/** Display a message a specified number of times.
    @param string null terminated message to display
    @param pos position on screen
    @param repeats number of times to repeat message.  */
void tinygl_draw_message (const char *string, tinygl_point_t pos,
                          uint8_t repeats);


/** Display a message repeatedly.
    @param string null terminated message to display.  */
void tinygl_text (const char *string);


/** Draw point.
    @param pos coordinates of point
    @param pixel_value pixel value to draw point.  */
void tinygl_draw_point (tinygl_point_t pos, tinygl_pixel_value_t pixel_value);


/** Set pixel.
    @param pos coordinates of point
    @param pixel_value pixel value.  */
void tinygl_pixel_set (tinygl_point_t pos, tinygl_pixel_value_t pixel_value);


/** Get pixel.
    @param pos coordinates of point
    @return pixel value.  */
tinygl_pixel_value_t tinygl_pixel_get (tinygl_point_t pos);


/** Draw line.
    @param pos1 coordinates of start of line
    @param pos2 coordinates of end of line
    @param pixel_value pixel value to draw line.  */
void tinygl_draw_line (tinygl_point_t pos1, tinygl_point_t pos2, 
                       tinygl_pixel_value_t pixel_value);


/** Draw box.
    @param tl coordinates of top left corner of box
    @param br coordinates of bottom right of box
    @param pixel_value pixel value to draw box.  */
void tinygl_draw_box (tinygl_point_t tl, tinygl_point_t br,
                      tinygl_pixel_value_t pixel_value);


/** Clear display.  */
void tinygl_clear (void);


/** Update display and advance message.  */
void tinygl_update (void);


/** Initialise things.
    @param rate rate in Hz that tinygl_update called.  */
void tinygl_init (uint16_t rate);


#endif /* TINYGL_H  */

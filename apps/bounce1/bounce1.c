/** @file   bounce1.c
    @author M. P. Hayes, UCECE
    @date   24 August 2009
    @brief  Simple bouncing dot program
    @note   This program has deliberate magic numbers as
    an exercise for students to replace.
    
    @defgroup bounce1 Bounce1 application
*/
#include "system.h"
#include "pacer.h"
#include "pio.h"


/* Define polling rate in Hz.  */
#define LOOP_RATE 10


/* Define PIO pins driving LED matrix rows and columns.  */
static pio_t ledmat_rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, LEDMAT_ROW4_PIO,
    LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW7_PIO
};
static pio_t ledmat_cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


/** Turn single LED within matrix on or off.
    @param col LED column number
    @param row LED row number
    @param state LED state  */
static void ledmat_pixel_set (int col, int row, bool state)
{
    if (state)
    {
        pio_output_low (ledmat_rows[row]);
        pio_output_low (ledmat_cols[col]);
    }
    else
    {
        pio_output_high (ledmat_rows[row]);
        pio_output_high (ledmat_cols[col]);
    }
}


/** Initialise LED matrix PIO pins.  */
static void ledmat_init (void)
{
    uint8_t row;
    uint8_t col;

    for (row = 0; row < 7; row++)
    {
        pio_config_set (ledmat_rows[row], PIO_OUTPUT_HIGH);
        pio_output_high (ledmat_rows[row]);
    }

    for (col = 0; col < 5; col++)
    {
        pio_config_set (ledmat_cols[col], PIO_OUTPUT_HIGH);
        pio_output_high (ledmat_cols[col]);
    }
}


int main (void)
{
    int row;
    int col;
    int rowinc;
    int colinc;

    system_init ();
    ledmat_init ();

    row = 3;
    col = 2;
    rowinc = 1;
    colinc = 1;

    ledmat_pixel_set (col, row, 1);

    pacer_init (LOOP_RATE);

    /* Paced loop.  */
    while (1)
    {
        /* Wait for next tick.  */
        pacer_wait ();

        ledmat_pixel_set (col, row, 0);        

        col += colinc;
        row += rowinc;

        if (row > 6 || row < 0)
        {
            row -= rowinc * 2;
            rowinc = -rowinc;
        }

        if (col > 4 || col < 0)
        {
            col -= colinc * 2;
            colinc = -colinc;
        }
        
        ledmat_pixel_set (col, row, 1);        
    }
}

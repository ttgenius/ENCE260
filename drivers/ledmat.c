/** @file   ledmat.c
    @author M. P. Hayes, UCECE
    @date   23 August 2010
    @brief  LED matrix driver.
*/
#include "system.h"
#include "pio.h"
#include "ledmat.h"


/** Define PIO pins driving LED matrix rows.  */
static const pio_t ledmat_rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, 
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t ledmat_cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


/** Initialise PIO pins to drive LED matrix.  */
void ledmat_init (void)
{
    uint8_t row;
    uint8_t col;

    for (row = 0; row < LEDMAT_ROWS_NUM; row++)
    {
        /* The rows are active low so configure PIO as an initially
           high output.  */
        pio_config_set (ledmat_rows[row], PIO_OUTPUT_HIGH);
    }

    for (col = 0; col < LEDMAT_COLS_NUM; col++)
    {
        /* The columns are active low so configure PIO as an initially
           high output.  */
        pio_config_set (ledmat_cols[col], PIO_OUTPUT_HIGH);
    }
}


/** Display pattern on specified column.
    @param pattern bit pattern to display for selected column
    @param col selected column.  */
void ledmat_display_column (uint8_t pattern, uint8_t col)
{
    static uint8_t col_prev = 0;
    uint8_t row;

    /* Disable previous column to prevent ghosting while rows modified.  */
    pio_output_high (ledmat_cols[col_prev]);

    /* Activate desired rows based on desired pattern.  */
    for (row = 0; row < LEDMAT_ROWS_NUM; row++)
    {
        /* The rows are active low.  */
        if (pattern & 1)
            pio_output_low (ledmat_rows[row]);
        else
            pio_output_high (ledmat_rows[row]);

        pattern >>= 1;
    }

    /* Enable new column.  */
    pio_output_low (ledmat_cols[col]);
    col_prev = col;
}



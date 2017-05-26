/** @file   system.h
    @author M. P. Hayes, UCECE
    @date   15 May 2007
    @brief  System specific definitions
*/
#ifndef SYSTEM_H
#define SYSTEM_H

/* Data typedefs.  */
#include <stdint.h>
#include <stdbool.h>


/** Useful macros.  */
#define BIT(X) (1 << (X))

#define ARRAY_SIZE(ARRAY) (sizeof (ARRAY) / sizeof (ARRAY[0]))

#define  __unused__ __attribute__ ((unused))


/** Clock frequency Hz.  */
#define F_CPU 8000000


/** LED matrix columns.  (Note these are labelled in reverse order on
   the schematic.)  */
#define LEDMAT_COL1_PIO PC6_PIO
#define LEDMAT_COL2_PIO PB7_PIO
#define LEDMAT_COL3_PIO PC4_PIO
#define LEDMAT_COL4_PIO PC7_PIO
#define LEDMAT_COL5_PIO PC5_PIO
#define LEDMAT_COLS_NUM 5


/** LED matrix rows.  */
#define LEDMAT_ROW1_PIO PB6_PIO
#define LEDMAT_ROW2_PIO PB5_PIO
#define LEDMAT_ROW3_PIO PB4_PIO 
#define LEDMAT_ROW4_PIO PB3_PIO 
#define LEDMAT_ROW5_PIO PB2_PIO 
#define LEDMAT_ROW6_PIO PB1_PIO 
#define LEDMAT_ROW7_PIO PB0_PIO 
#define LEDMAT_ROWS_NUM 7


/** Button.  */
#define BUTTON1 0
#define BUTTON1_PIO PD7_PIO


/** Navswitch.  */
#define NAVSWITCH_PUSH_PIO LEDMAT_COL3_PIO
#define NAVSWITCH_DOWN_PIO NAVSWITCH_PUSH_PIO 
#define NAVSWITCH_EAST_PIO LEDMAT_COL1_PIO
#define NAVSWITCH_WEST_PIO LEDMAT_COL2_PIO
#define NAVSWITCH_NORTH_PIO LEDMAT_COL4_PIO
#define NAVSWITCH_SOUTH_PIO LEDMAT_COL5_PIO


/** LED (active high).  */
#define LED1 0
#define LED1_PIO PC2_PIO


/** Infrared transmitter LED and receiver.  */
#define IR_TX_LOW_PIO PD3_PIO 
#define IR_TX_HIGH_PIO PD0_PIO
#define IR_RX_PIO PD2_PIO

#define IR_MODULATION_FREQ 36e3

/** Initialise clock and disable watchdog timer.  */
void system_init (void);

#endif

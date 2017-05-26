/** @file   system.h
    @author M. P. Hayes, UCECE
    @date   15 May 2007
    @brief  System specific definitions
*/
#ifndef SYSTEM_H
#define SYSTEM_H

/* Data typedefs.  */
#include <stdint.h>

typedef uint8_t bool;


/* Useful macros.  */
#define BIT(X) (1 << (X))

#define ARRAY_SIZE(ARRAY) (sizeof (ARRAY) / sizeof (ARRAY[0]))

#define  __unused__ __attribute__ ((unused))


/* Clock frequency Hz.  */
#define F_CPU 8000000


/* LED matrix columns.  */
#define LEDMAT_COL1_PIO PIO_DEFINE(PORT_C, 6)
#define LEDMAT_COL2_PIO PIO_DEFINE(PORT_B, 7)
#define LEDMAT_COL3_PIO PIO_DEFINE(PORT_C, 4)
#define LEDMAT_COL4_PIO PIO_DEFINE(PORT_C, 7)
#define LEDMAT_COL5_PIO PIO_DEFINE(PORT_C, 5)
#define LEDMAT_COLS_NUM 5


/* LED matrix rows.  */
#define LEDMAT_ROW1_PIO PIO_DEFINE(PORT_B, 6)
#define LEDMAT_ROW2_PIO PIO_DEFINE(PORT_B, 5)
#define LEDMAT_ROW3_PIO PIO_DEFINE(PORT_B, 4)
#define LEDMAT_ROW4_PIO PIO_DEFINE(PORT_B, 3)
#define LEDMAT_ROW5_PIO PIO_DEFINE(PORT_B, 2)
#define LEDMAT_ROW6_PIO PIO_DEFINE(PORT_B, 1)
#define LEDMAT_ROW7_PIO PIO_DEFINE(PORT_B, 0)
#define LEDMAT_ROWS_NUM 7


/* Button.  */
#define BUTTON1 0
#define BUTTON1_PIO PIO_DEFINE(PORT_D, 7)


/* Navswitch.  */
#define NAVSWITCH_PUSH_PIO LEDMAT_COL3_PIO
#define NAVSWITCH_EAST_PIO LEDMAT_COL1_PIO
#define NAVSWITCH_WEST_PIO LEDMAT_COL2_PIO
#define NAVSWITCH_NORTH_PIO LEDMAT_COL4_PIO
#define NAVSWITCH_SOUTH_PIO LEDMAT_COL5_PIO


/* LED (active high).  */
#define LED1 0
#define LED1_PIO PIO_DEFINE(PORT_C, 2)


/* Infrared transmitter LED and receiver.  */
#define IR_TX_LOW_PIO PIO_DEFINE(PORT_D, 3)
#define IR_TX_HIGH_PIO PIO_DEFINE(PORT_D, 0)
#define IR_RX_PIO PIO_DEFINE(PORT_D, 2)

#define IR_MODULATION_FREQ 36e3

void system_init (void);

#endif

/** @file   ir_uart_demo1.c
    @author M. P. Hayes, UCECE
    @date   3 October 2013
    @brief  Test program for IR UART communications showing encoding.
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "task.h"
#include "ir_uart.h"

#define DISPLAY_TASK_RATE 200

#define IR_UART_TX_TASK_RATE 20

#define IR_UART_RX_TASK_RATE 20

#define NAVSWITCH_TASK_RATE 20

typedef enum {XPOS, YPOS} tx_message_t;

typedef enum {SEND_XPOS, SEND_YPOS} tx_state_t;


static tinygl_point_t this_pos;
static tinygl_point_t other_pos;


static void display_task (__unused__ void *data)
{
    tinygl_update ();
}


static void ir_uart_tx_task (__unused__ void *data)
{
    static uint8_t state = SEND_XPOS;

    /* Alternately, send x position and y position messages.  */
    switch (state)
    {
    case SEND_XPOS:
        ir_uart_putc ((XPOS << 4) | (this_pos.x & 0x0f));
        state = SEND_YPOS;
        break;

    case SEND_YPOS:
        ir_uart_putc ((YPOS << 4) | (this_pos.y & 0x0f));
        state = SEND_XPOS;
        break;
    }
}


static void ir_uart_rx_task (__unused__ void *data)
{
    uint8_t c;

    if (!ir_uart_read_ready_p ())    
        return;

    tinygl_draw_point (other_pos, 0);

    c = ir_uart_getc ();

    /* Decode the received message.  */
    switch (c >> 4)
    {
    case XPOS:
        other_pos.x = c & 0x0f;
        break;

    case YPOS:
        other_pos.y = c & 0x0f;
        break;
    }
    tinygl_draw_point (other_pos, 1);
}


static void navswitch_task (__unused__ void *data)
{
    tinygl_point_t newpos;

    navswitch_update ();
    
    newpos = this_pos;

    if (navswitch_push_event_p (NAVSWITCH_NORTH))
        newpos.y--;
    if (navswitch_push_event_p (NAVSWITCH_SOUTH))
        newpos.y++;
    if (navswitch_push_event_p (NAVSWITCH_EAST))
        newpos.x++;
    if (navswitch_push_event_p (NAVSWITCH_WEST))
        newpos.x--;

    /* Note, there is no checking that have moved off screen.  */

    if (this_pos.x != newpos.x || this_pos.y != newpos.y)
    {
        tinygl_draw_point (this_pos, 0);
        this_pos = newpos;
        tinygl_draw_point (this_pos, 1);
    }
}


int main (void)
{
    task_t tasks[] =
    {
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE},
        {.func = ir_uart_rx_task, .period = TASK_RATE / IR_UART_RX_TASK_RATE},
        {.func = ir_uart_tx_task, .period = TASK_RATE / IR_UART_TX_TASK_RATE},
        {.func = navswitch_task, .period = TASK_RATE / NAVSWITCH_TASK_RATE},
    };    

    system_init ();
    ir_uart_init ();
    tinygl_init (DISPLAY_TASK_RATE);

    this_pos = tinygl_point (TINYGL_WIDTH / 2, TINYGL_HEIGHT / 2);
    tinygl_draw_point (this_pos, 1);

    task_schedule (tasks, 4);

    return 0;
}

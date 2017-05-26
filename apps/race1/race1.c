/** @file   race1.c
    @author M. P. Hayes, UCECE
    @date   7 October 2013
    @brief  Test program for IR UART communications showing possible 
            race condition.
    @note   Not tested yet...
*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "task.h"
#include "ir_uart.h"
#include "../fonts/font5x7_1.h"

#define DISPLAY_TASK_RATE 200

#define IR_UART_TX_TASK_RATE 20

#define IR_UART_RX_TASK_RATE 20

#define NAVSWITCH_TASK_RATE 20

typedef enum state {STATE_UNKNOWN, STATE_FIRST, STATE_SECOND} state_t;

static state_t this_state = STATE_UNKNOWN;
static state_t other_state = STATE_UNKNOWN;

static void display_task (__unused__ void *data)
{
    tinygl_update ();
}


static void ir_uart_tx_task (__unused__ void *data)
{
    /* Send our state.  */
    ir_uart_putc ('A' + this_state);
}


static void ir_uart_rx_task (__unused__ void *data)
{
    state_t newstate;

    if (!ir_uart_read_ready_p ())    
        return;

    newstate = ir_uart_getc () - 'A';
 
   /* Check for corrupted message.  */
    if (newstate > STATE_SECOND)
        return;

    /* See if other devices state has changed.  */
    if (other_state != newstate)
    {
        other_state = newstate;
        if (this_state == STATE_UNKNOWN && other_state == STATE_FIRST)
        {
            this_state = STATE_SECOND;
            tinygl_text ("2");
        }
        else
        {
            /* So what should we do if there is a disagreement? 
               What if both devices think they were first?  */
        }
    }
}


static void navswitch_task (__unused__ void *data)
{
    navswitch_update ();
    
    if (navswitch_push_event_p (NAVSWITCH_PUSH))
    {
        if (this_state == STATE_UNKNOWN)
        {
            this_state = STATE_FIRST;
            tinygl_text ("1");
        }
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

    tinygl_font_set (&font5x7_1);
    tinygl_text ("*");

    task_schedule (tasks, 4);

    return 0;
}

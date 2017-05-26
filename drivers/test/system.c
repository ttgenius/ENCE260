/** @file   system.c
    @author M. P. Hayes, UCECE
    @date   15 May 2007
    @brief  Test scaffold for UCFK4.
*/

#include "system.h"
#include "avrtest.h"
#include "pio.h"
#include "mgetkey.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>


/** This needs to be at least 50 * 5 for a refresh rate of 50 Hz.  */
#define SYSTEM_UPDATE_RATE 400

#define SYSTEM_KEYBOARD_RELEASE_PERIOD 0.2

#define SYSTEM_DISPLAY_PERSISTENCE_PERIOD 0.01


SFR_t SFR = {0, 0, ~0, 0, 0, ~0, 0, 0, ~0};


/* Define PIO pins driving LED matrix rows and columns.  */
static const pio_t ledmat_rows[] =
{
    LEDMAT_ROW1_PIO,
    LEDMAT_ROW2_PIO,
    LEDMAT_ROW3_PIO, 
    LEDMAT_ROW4_PIO,
    LEDMAT_ROW5_PIO,
    LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};

static const pio_t ledmat_cols[] =
{
    LEDMAT_COL1_PIO,
    LEDMAT_COL2_PIO,
    LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO,
    LEDMAT_COL5_PIO
};


typedef enum {NAVSWITCH_NONE, NAVSWITCH_EAST, NAVSWITCH_SOUTH,
              NAVSWITCH_WEST, NAVSWITCH_NORTH, NAVSWITCH_PUSH} system_navswitch_t;

typedef enum {BUTTON_NONE, BUTTON_1} system_button_t;


static void system_display_update (void)
{
    int row;
    int col;
    /* Persistence time (s).  */
    double tau = SYSTEM_DISPLAY_PERSISTENCE_PERIOD;
    /* Sample period (s).  */
    double delta_t;
    /* Exponential filter parameter.  */
    double alpha;
    static uint8_t tdisplay[LEDMAT_ROWS_NUM][LEDMAT_COLS_NUM];

    delta_t = 1.0 / SYSTEM_UPDATE_RATE;
    alpha = tau / (tau + delta_t);

    for (row = 0; row < LEDMAT_ROWS_NUM; row++)
    {
        for (col = 0; col < LEDMAT_COLS_NUM; col++)
        {
            uint8_t on;

            /* Determine if pixel is on.  */
            on = !pio_output_get (ledmat_rows[row])
                && !pio_output_get (ledmat_cols[col]);

            /* Apply exponential filter to provide some persistence.
               This has an impulse response of exp(-t / tau) u(t).  */
            tdisplay[row][col] = tdisplay[row][col] * alpha + (1 - alpha) * 100 * on;

            printf ("%c", tdisplay[row][col] > 1 ? '@' : '.');
        }
        printf ("\n");
    }
    printf ("\n");
    /* Move cursor up.  */
    printf ("\e[%dA", LEDMAT_ROWS_NUM + 1);
}


static void system_navswitch_set (system_navswitch_t navswitch, bool state)
{
    switch (navswitch)
    {
    case NAVSWITCH_NONE:
        break;

    case NAVSWITCH_NORTH:
        pio_input_set (NAVSWITCH_NORTH_PIO, !state);
        break;

    case NAVSWITCH_EAST:
        pio_input_set (NAVSWITCH_EAST_PIO, !state);
        break;

    case NAVSWITCH_SOUTH:
        pio_input_set (NAVSWITCH_SOUTH_PIO, !state);
        break;

    case NAVSWITCH_WEST:
        pio_input_set (NAVSWITCH_WEST_PIO, !state);
        break;

    case NAVSWITCH_PUSH:
        pio_input_set (NAVSWITCH_PUSH_PIO, !state);
        break;
    }

}


static void system_navswitch_press (system_navswitch_t navswitch)
{
    system_navswitch_set (navswitch, 1);
}


static void system_navswitch_release (system_navswitch_t navswitch)
{
    system_navswitch_set (navswitch, 0);
}


static void system_navswitch_select (system_navswitch_t navswitch)
{
    static system_navswitch_t last = NAVSWITCH_NONE;
    static int counter;

    if (navswitch == NAVSWITCH_NONE)
    {
        if (counter)
        {
            counter--;
            if (!counter)
            {
                system_navswitch_release (last);
                last = NAVSWITCH_NONE;
            }
        }
        return;
    }

    counter = SYSTEM_UPDATE_RATE * SYSTEM_KEYBOARD_RELEASE_PERIOD;

    if (last == navswitch)
        return;

    system_navswitch_release (last);

    last = navswitch;

    system_navswitch_press (navswitch);
}


static void system_button_set (system_button_t button, bool state)
{
    switch (button)
    {
    case BUTTON_NONE:
        break;

    case BUTTON_1:
        /* Active high.  */
        pio_input_set (BUTTON1_PIO, state);
        break;
    }

}


static void system_button_press (system_button_t button)
{
    system_button_set (button, 1);
}


static void system_button_release (system_button_t button)
{
    system_button_set (button, 0);
}


static void system_button_select (system_button_t button)
{
    static system_button_t last = BUTTON_NONE;
    static int counter;

    if (button == BUTTON_NONE)
    {
        if (counter)
        {
            counter--;
            if (!counter)
            {
                system_button_release (last);
                last = BUTTON_NONE;
            }
        }
        return;
    }

    counter = SYSTEM_UPDATE_RATE * SYSTEM_KEYBOARD_RELEASE_PERIOD;

    if (last == button)
        return;

    system_button_release (last);

    last = button;

    system_button_press (button);
}


static void system_keyboard_init (void)
{
    system_navswitch_release (NAVSWITCH_PUSH);
    system_navswitch_release (NAVSWITCH_NORTH);
    system_navswitch_release (NAVSWITCH_EAST);
    system_navswitch_release (NAVSWITCH_SOUTH);
    system_navswitch_release (NAVSWITCH_WEST);

    system_button_release (BUTTON_1);
}


static void system_keyboard_update (void)
{
    static int state = 0;
    int key;
    
    system_navswitch_select (NAVSWITCH_NONE);
    system_button_select (BUTTON_NONE);

    key = mgetkey ();

    switch (state)
    {
    case 0:
        switch (key)
        {
        case '\e':
            /* Have escape character.  */
            state = 1;
            break;

        case ' ':
            system_navswitch_select (NAVSWITCH_PUSH);
            break;

        case '.':
            system_button_select (BUTTON_1);
            break;

        default:
            break;
        }
        break;
 
    case 1:
        if (key == '[')
            state = 2;
        else
            state = 0;
        break;

    case 2:
        state = 0;
        switch (key)
        {
        case 'A':
            system_navswitch_select (NAVSWITCH_NORTH);
            break;
            
        case 'B':
            system_navswitch_select (NAVSWITCH_SOUTH);
            break;

        case 'C':
            system_navswitch_select (NAVSWITCH_EAST);
            break;

        case 'D':
            system_navswitch_select (NAVSWITCH_WEST);
            break;
            
        default:
            break;
        }
        break;
    }
}


static void system_update (void)
{
    system_display_update ();
    system_keyboard_update ();
}


static void
system_handler (__unused__ int sig, __unused__ siginfo_t *si,
                __unused__ void *uc)
{
    system_update ();
}


typedef void (*sighandler_t)(int, siginfo_t *, void *);

static int system_interrupt_init (sighandler_t handler, int rate)
{
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;
    long long period_ns;
    sigset_t mask;
    struct sigaction sa;
    static int signum = 0;

    /* When debugging with gdb use "handle SIG34 noprint".  */

    if (!signum)
        signum = SIGRTMIN;
    
    /* Establish handler for timer signal.  */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset (&sa.sa_mask);
    if (sigaction (signum, &sa, NULL) == -1)
    {
        perror ("sigaction");
        exit (EXIT_FAILURE);
    }
    
    /* Block timer signal temporarily.  */
    sigemptyset (&mask);
    sigaddset (&mask, signum);
    if (sigprocmask (SIG_SETMASK, &mask, NULL) == -1)
    {
        perror ("sigprocmask");
        exit (EXIT_FAILURE);
    }
    
    /* Create the timer.  */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = signum;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create (CLOCK_REALTIME, &sev, &timerid) == -1)
    {
        perror ("timer_create");
        exit (EXIT_FAILURE);
    }
    
    /* Start the timer */
    period_ns = 1000000000 / rate;
    its.it_value.tv_sec = period_ns / 1000000000;
    its.it_value.tv_nsec = period_ns % 1000000000;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;
    if (timer_settime (timerid, 0, &its, NULL) == -1)
    {
        perror ("timer_settime");
        exit (EXIT_FAILURE);
    }
    
    /* Unlock the timer signal, so that timer notification
       can be delivered.  */
    if (sigprocmask (SIG_UNBLOCK, &mask, NULL) == -1)
    {
        perror ("sigprocmask");
        exit (EXIT_FAILURE);
    }

    signum++;
    return signum;
}


void system_init (void)
{
    system_keyboard_init ();

    system_interrupt_init (system_handler, SYSTEM_UPDATE_RATE);
}

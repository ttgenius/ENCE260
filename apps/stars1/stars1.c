/** @file   stars1.c
    @author M.P. Hayes
    @date   25 Sep 2013
    @brief  Example of using pseudorandom numbers and controlling pixel
    brightness with PWM.  Note, pseudorandom numbers follow the same
    sequence.
*/

#include <stdlib.h>
#include "system.h"
#include "pacer.h"
#include "tinygl.h"

/* This controls the range of luminance.  A bigger number gives more dynamic range
   but requires a faster update rate.  */
#define LUMINANCE_STEPS 25

/* This needs to be fast enough to prevent the eye noticing flicker.
   A lower value (say 5) is useful for flashing pixels.  */
#define PWM_RATE 40

/* Number of twinkling stars.  */
#define NUM_STARS 10

/* This is the rate (Hz) of luminance changes.  */
#define TWINKLE_RATE 5

#define UPDATE_RATE (LUMINANCE_STEPS * PWM_RATE)

#define LOOP_RATE (TINYGL_WIDTH * UPDATE_RATE)

int main (void)
{
    uint16_t twinkle_tick = 0;
    uint8_t pwm_tick = 0;
    uint8_t i;
    uint8_t x;
    uint8_t y;
    uint8_t col;
    /* This stores the current luminance level for each pixel.  */
    uint8_t display[TINYGL_HEIGHT][TINYGL_WIDTH] = {{0, }};
    /* This controls the luminance levels.  The maximum value
       of LUMINANCE_STEPS gives 100 percent duty cycle.   */
    const uint8_t levels[] = {0, 1, 2, 4, 8, 15, 25, 15, 8, 4, 2, 1};

    system_init ();

    tinygl_init (LOOP_RATE);

    pacer_init (LOOP_RATE);

    for (i = 0; i < NUM_STARS; i++)
    {
        /* Create new star, but not over a live one.  */
        do
        {
            x = rand () % TINYGL_WIDTH;
            y = rand () % TINYGL_HEIGHT;
        } while (display[y][x]);
        
        display[y][x] = rand () % ARRAY_SIZE (levels);
    }

    
    while (1)
    {
        /* Refresh display.  */
        for (col = 0; col < TINYGL_WIDTH; col++)
        {
            pacer_wait ();

            tinygl_update ();
        }

        twinkle_tick++;
        if (twinkle_tick >= UPDATE_RATE / TWINKLE_RATE)
        {
            twinkle_tick = 0;

            /* Vary luminance of stars.  */
            for (x = 0; x < TINYGL_WIDTH; x++)
            {
                for (y = 0; y < TINYGL_HEIGHT; y++)
                {
                    if (display[y][x] > 0)
                    {
                        display[y][x]--;
                        if (display[y][x] == 0)
                            display[y][x] = ARRAY_SIZE (levels) - 1;
                    }
                }
            }
        }

        /* Pulse width modulate pixels to control luminance.  */
        for (x = 0; x < TINYGL_WIDTH; x++)
        {
            for (y = 0; y < TINYGL_HEIGHT; y++)
            {
                tinygl_draw_point (tinygl_point (x, y), 
                                   levels[display[y][x]] > pwm_tick);
            }
        }

        pwm_tick++;
        if (pwm_tick >= UPDATE_RATE / PWM_RATE)
        {
            pwm_tick = 0;
        }
    }
    return 0;
}

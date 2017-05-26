/** @file   stars3.c
    @author M.P. Hayes
    @date   28 Sep 2013
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


static void star_twinkle (uint8_t starmap[TINYGL_HEIGHT][TINYGL_WIDTH],
                          uint8_t x, uint8_t y, int num_levels)
{
    /* Check if star alive.  */
    if (starmap[y][x] == 0)
        return;

    /* Change star luminance until it dies.  */
    starmap[y][x]--;
    if (starmap[y][x] > 0)
        return;

    /* Create a new star at a point neighbouring the old one
       or at the same place.  */
    while (1)
    {
        int8_t dx;
        int8_t dy;
        
        dx = (rand () % 3) - 1;
        dy = (rand () % 3) - 1;
        
        if (x + dx >= 0 && x + dx < TINYGL_WIDTH
            && y + dy >= 0 && y + dy < TINYGL_HEIGHT
            && starmap[y + dy][x + dx] == 0)
        {
            starmap[y + dy][x + dx] = num_levels - 1;
            return;
        }
    }
}


static void stars_twinkle (uint8_t starmap[TINYGL_HEIGHT][TINYGL_WIDTH],
                           int num_levels)
{
    uint8_t x;
    uint8_t y;

    for (x = 0; x < TINYGL_WIDTH; x++)
    {
        for (y = 0; y < TINYGL_HEIGHT; y++)
        {
            star_twinkle (starmap, x, y, num_levels);
        }
    }
}


static void stars_create (uint8_t starmap[TINYGL_HEIGHT][TINYGL_WIDTH],
                          int num_levels)
{
    uint8_t x;
    uint8_t y;
    uint8_t i;

    for (i = 0; i < NUM_STARS; i++)
    {
        /* Create new star, but not over a live one.  */
        do
        {
            x = rand () % TINYGL_WIDTH;
            y = rand () % TINYGL_HEIGHT;
        } while (starmap[y][x]);
        
        starmap[y][x] = rand () % num_levels;
    }
}


int main (void)
{
    uint16_t twinkle_tick = 0;
    uint8_t pwm_tick = 0;
    uint8_t x;
    uint8_t y;
    uint8_t col;
    /* This stores the current luminance level for each pixel.  */
    uint8_t starmap[TINYGL_HEIGHT][TINYGL_WIDTH] = {{0, }};
    /* This controls the luminance levels.  The maximum value
       of LUMINANCE_STEPS gives 100 percent duty cycle.   */
    const uint8_t levels[] = {0, 1, 2, 4, 8, 15, 25, 15, 8, 4, 2, 1};

    system_init ();

    tinygl_init (LOOP_RATE);

    pacer_init (LOOP_RATE);

    stars_create (starmap, ARRAY_SIZE (levels));

    while (1)
    {
        /* Refresh starmap.  */
        for (col = 0; col < TINYGL_WIDTH; col++)
        {
            pacer_wait ();

            tinygl_update ();
        }

        twinkle_tick++;
        if (twinkle_tick >= UPDATE_RATE / TWINKLE_RATE)
        {
            twinkle_tick = 0;

            /* Change luminance of stars until they die.  Then create
               a new one nearby.  */
            stars_twinkle (starmap, ARRAY_SIZE (levels));
        }

        /* Pulse width modulate pixels to control luminance.  */
        for (x = 0; x < TINYGL_WIDTH; x++)
        {
            for (y = 0; y < TINYGL_HEIGHT; y++)
            {
                tinygl_draw_point (tinygl_point (x, y), 
                                   levels[starmap[y][x]] > pwm_tick);
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

#include "system.h"
#include "pio.h"

/* Include the pacer module from the previous lab.
   You must have completed this before starting this lab2-exercise.  */
#include "pacer.h"

int main (void)
{
    system_init ();
    
    /* TODO: Initialise the pins of the LED matrix.  */
    pio_config_set(LEDMAT_COL1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_COL5_PIO, PIO_OUTPUT_HIGH);
    
    pio_config_set(LEDMAT_ROW1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW5_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW6_PIO, PIO_OUTPUT_HIGH);
    pio_config_set(LEDMAT_ROW7_PIO, PIO_OUTPUT_HIGH);

    /* Set up pacer with a frequency of 50 Hz.  */
    pacer_init (50);
    int state = 0;
    
    while (1)
    {
        /* Pace the loop.  */
        pacer_wait ();
        
        /* TODO: Drive the LED matrix using the pio functions,
           displaying only three corner LEDs.  */
        if (state==1){
			pio_config_set(LEDMAT_COL1_PIO, PIO_OUTPUT_HIGH); //should use pio out put high
            pio_config_set(LEDMAT_COL5_PIO, PIO_OUTPUT_LOW); // should use pio out put low
            pio_config_set(LEDMAT_ROW1_PIO, PIO_OUTPUT_LOW);
            pio_config_set(LEDMAT_ROW7_PIO, PIO_OUTPUT_LOW);
        }else{
			pio_config_set(LEDMAT_COL5_PIO, PIO_OUTPUT_HIGH);
			pio_config_set(LEDMAT_ROW1_PIO, PIO_OUTPUT_HIGH);
			pio_config_set(LEDMAT_ROW7_PIO, PIO_OUTPUT_LOW);
			pio_config_set(LEDMAT_COL1_PIO, PIO_OUTPUT_LOW); 
		}	
	   state = !state;
	}		    
}

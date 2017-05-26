#include "pio.h"
#include "system.h"

//#define BUTTON_PIN PIO_DEFINE (PORT_D, 7)
//#define LEDMAT_COL1_PIO PIO_DEFINE (PORT_C, 6)
//#define LEDMAT_ROW1_PIO PIO_DEFINE (PORT_B, 6)
int main (void)
{
    system_init ();
    pio_config_set(BUTTON1_PIO, PIO_INPUT);
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

    while (1)
    {
        if (pio_input_get(BUTTON1_PIO)){
			pio_output_low(LEDMAT_ROW6_PIO);
			pio_output_low(LEDMAT_COL2_PIO);
			pio_output_low(LEDMAT_COL4_PIO);
			pio_output_low(LEDMAT_ROW3_PIO);
			
			pio_output_low(LEDMAT_ROW2_PIO);
			pio_output_low(LEDMAT_COL3_PIO);
		}
		else{
			pio_output_high(LEDMAT_ROW6_PIO);
			pio_output_high(LEDMAT_COL2_PIO);
			pio_output_high(LEDMAT_COL4_PIO);
			pio_output_high(LEDMAT_ROW3_PIO);
			pio_output_high(LEDMAT_ROW2_PIO);
			pio_output_high(LEDMAT_COL3_PIO);
		
		}
				
        /* TODO.  Use PIO module to turn on LEDs in
           LED matrix.  */

    }
}

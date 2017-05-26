#include "pio.h"
#include "button.h"

#define BUTTON_PIN PIO_DEFINE (PORT_D, 7)

/** Return non-zero if button pressed.  */
int button_pressed_p (void)
{
    /* TODO.  */
    return pio_input_get(BUTTON_PIN);
}


/** Initialise button1.  */
void button_init (void)
{
    /* TODO.  */
    pio_config_set(BUTTON_PIN, PIO_INPUT);
}


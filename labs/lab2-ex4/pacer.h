#ifndef PACER_H
#define PACER_H

#include "system.h"

/* Initialise the pacer module.  */
void pacer_init (uint16_t pacer_frequency);


/* Pace a while loop.  */
void pacer_wait (void);

#endif //PACER_H

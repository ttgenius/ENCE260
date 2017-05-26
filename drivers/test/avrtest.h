#ifndef AVRTEST_H
#define AVRTEST_H

#include "system.h"

typedef struct SFR
{
    uint8_t PORTB;
    uint8_t DDRB;
    uint8_t PINB;
    uint8_t PORTC;
    uint8_t DDRC;
    uint8_t PINC;
    uint8_t PORTD;
    uint8_t DDRD;
    uint8_t PIND;
} SFR_t;


#define PORTB SFR.PORTB
#define PORTC SFR.PORTC
#define PORTD SFR.PORTD
#define DDRB SFR.DDRB
#define DDRC SFR.DDRC
#define DDRD SFR.DDRD
#define PINB SFR.PINB
#define PINC SFR.PINC
#define PIND SFR.PINC

extern SFR_t SFR;


#endif

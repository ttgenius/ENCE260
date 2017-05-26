#ifndef THINGS_H
#define THINGS_H

#include "system.h"

bool things_killed_p (void);

void things_create (void);

void things_move (void);

void things_monster_move (int8_t dx, int8_t dy);

void things_monster_toggle (void);

#endif

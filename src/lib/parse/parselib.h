#ifndef PARSELIB_H
#define PARSELIB_H

#include "../tokenlib.h"
#include "variables.h"

// These variables must be visible to other parsing components
// and users of parselib
extern Token *_IP;
extern int line;

int initial_state(Variable **table);

#endif

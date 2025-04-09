#ifndef PARSELIB_H
#define PARSELIB_H

#include <stdbool.h>

#include "../tokenlib.h"
#include "variables.h"

// Hashmap library from https://troydhanson.github.io/uthash/
#include "uthash.h"

// These variables must be visible to parse.c
extern Token *_IP;
extern long _length;


int handle_identifier(Variable **table);
Token parse_expression(Variable **table);

#endif

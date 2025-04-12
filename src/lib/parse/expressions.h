#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "variables.h"

// Hashmap library from https://troydhanson.github.io/uthash/
#include "uthash.h"

Token parse_expression(Variable **table);

#endif

#ifndef VARIABLES_H
#define VARIABLES_H

#include "../tokenlib.h"

// Hashmap library from https://troydhanson.github.io/uthash/
#include "uthash.h"

// Global variable hashmap
typedef struct {
    char *name; // Key
    Token value;
    UT_hash_handle hh;
} Variable;


int set_variable(Variable **table, char *key, Token value);
Token* get_variable(Variable **table, char *key);
void destroy_variables(Variable **table);
void destroy_variables_after_token_list(Variable **table);


#endif

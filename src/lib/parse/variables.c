#include "variables.h"

// Set a variable in the hashmap
int set_variable(Variable **table, char *key, Token value) {
    Variable *var;

    HASH_FIND_STR(*table, key, var);

    // If the variable doesn't exist, create it
    if (var == NULL) {
        var = (Variable*)malloc(sizeof(Variable));
        var -> name = strdup(key);
        if (var -> name == NULL)
            return EXIT_FAILURE;
        HASH_ADD_STR(*table, name, var);
    }

    var -> value = value;
    return EXIT_SUCCESS;
}


Token* get_variable(Variable **table, char *key) {
    Variable *var;

    HASH_FIND_STR(*table, key, var);
    // You could get rid of the condition, since we return NULL if it isn't
    // NULL anyways, but I'm keeping it for explicitness
    if (var != NULL)
        return &var -> value;
    return NULL;
}


// Clear and cleanup a hashmap
void destroy_variables(Variable **table) {
    Variable *current, *tmp;

    HASH_ITER(hh, *table, current, tmp) {
        HASH_DEL(*table, current);

        free(current -> name);

        switch (current -> value.type) {
            case STRING:
                if (current -> value.literal.String != NULL) {
                    free(current -> value.literal.String);
                    current -> value.literal.String = NULL;
                }
                break;
            default:
                break;
        }

        free(current);
    }
}

// When a STRING token is copied into the variable hashmap,
// it receives a copy of the pointer to the string. To prevent
// a double free, if you already destroyed the token list containing
// the STRING, call this to destroy the variable map.
void destroy_variables_after_token_list(Variable **table) {
    Variable *current, *tmp;

    HASH_ITER(hh, *table, current, tmp) {
        HASH_DEL(*table, current);

        free(current -> name);
        free(current);
    }
}

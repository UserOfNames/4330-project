#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse.h"
#include "lib/parse/parselib.h"
#include "lib/parse/expressions.h"
#include "lib/parse/ptr_stack.h"
#include "lib/tokenlib.h"


// Accepts an ENDPOINT-terminated TokenList
int parse(TokenList *token_list) {
    int result = EXIT_SUCCESS;
    _Bool break_loop = false;
    Variable *table  = NULL;

    _IP = token_list -> tokens;

    // Walk the token list
    while (_IP -> type != ENDPOINT && !break_loop) {
        switch (_IP -> type) {
            case IDENTIFIER:
                break;

            default:
                fprintf(stderr, "Error: Invalid token on line %d\n", _IP -> line);
                break_loop = true;
                result = EXIT_FAILURE;
                break;
        }
    }

    destroy_variables(&table);
    return result;
}

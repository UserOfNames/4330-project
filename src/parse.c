#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse.h"
#include "lib/parse/parselib.h"
#include "lib/parse/ptr_stack.h"
#include "lib/tokenlib.h"


int parse(TokenList *token_list) {
    int result = EXIT_SUCCESS;
    _Bool break_loop = false;
    Variable *table  = NULL;

    _IP     = token_list -> tokens;
    _length = token_list -> used;

    // Walk the token list
    while (_IP < _IP + _length && !break_loop) {
        switch (_IP -> type) {
            case IDENTIFIER:
                handle_identifier(&table);
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

#include <stdio.h>
#include <stdlib.h>

#include "assignment.h"
#include "expressions.h"
#include "parselib.h"
#include "../tokenlib.h"
#include "variables.h"

int assign(Variable **table) {
    // Mark the name of the variable to assign to, then advance IP
    // to the token after the equals (the token to assign from)
    char *assign_to = _IP -> literal.Name;
    _IP += 2;

    // Result to assign from
    Token result;

    switch (_IP -> type) {
        case NONE:
            result = *_IP;
            break;

        case STRING:
            result = *_IP;
            break;

        case IDENTIFIER:
            result = parse_expression(table);
            if (result.type == DISCARD) {
                return EXIT_FAILURE;
            }
            break;


        case TRUE:
            result = parse_expression(table);
            if (result.type == DISCARD) {
                return EXIT_FAILURE;
            }

            break;

        case FALSE:
            result = parse_expression(table);
            if (result.type == DISCARD) {
                return EXIT_FAILURE;
            }
            break;

        case NUMBER:
            result = parse_expression(table);
            if (result.type == DISCARD) {
                return EXIT_FAILURE;
            }
            break;

        case BANG:
            result = parse_expression(table);
            if (result.type == DISCARD) {
                return EXIT_FAILURE;
            }
            break;

        case NEGATE:
            result = parse_expression(table);
            if (result.type == DISCARD) {
                return EXIT_FAILURE;
            }
            break;

        // Any other token should not follow an assignment
        default:
            fprintf(stderr, "Error: Unexpected token following identifier '%s' on line %d\n",
                    (_IP-2) -> literal.Name, _IP -> line);
            return EXIT_FAILURE;
    }

    set_variable(table, assign_to, result);
    return EXIT_SUCCESS;
}


int parse_assignment(Variable **table) {
    // If a loose identifier is spotted, it should be an assignment
    if ((_IP + 1) -> type == EQ) {
        return assign(table);
    } else {
        fprintf(stderr, "Error: Unexpected identifier %s on line %d\n",
                _IP -> literal.Name, _IP -> line);
        return EXIT_FAILURE;
    }
}

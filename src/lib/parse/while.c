#include <stdlib.h>
#include <stdio.h>

#include "expressions.h"
#include "variables.h"
#include "parselib.h"
#include "while.h"

int parse_while(Variable **table) {
    int result;

    // Store a copy of the pointer to the WHILE token so we can loop back to it
    Token *while_ptr = _IP;
    Token expr_result;
    while (true) {
        _IP++; // Pass the WHILE token
        expr_result = parse_expression(table);

        // If parse_expression gives a DISCARD token, it should have already
        // printed an error message, so we can just immediately fail here
        if (expr_result.type == DISCARD)
            return EXIT_FAILURE;

        _IP++; // Pass the SEMICOLON token from the expression

        if (_IP -> type != LSQUARE) {
            fprintf(stderr, "Error: Expected '[' after while condition on line %d\n",
                    _IP -> line);
            return EXIT_FAILURE;
        }

        _IP++; // Pass the LSQUARE token

        if (expr_result.type == TRUE) {
            while ((result = initial_state(table, false, true)) != RSQUARE) {
                if (result == EXIT_FAILURE)
                    return result;

                if (result == ENDPOINT) {
                    fprintf(stderr, "Error: Unterminated while loop; ']' expected on line %d\n",
                            _IP -> line);
                    return EXIT_FAILURE;
                }
            }

            // Go back to the start of the loop
            _IP = while_ptr;
            continue;
        }

        // Pass the block
        else if (expr_result.type == FALSE) {
            while (_IP -> type != RSQUARE) {
                if (_IP -> type == ENDPOINT) {
                    fprintf(stderr, "Error: Unterminated while loop; ']' expected on line %d\n",
                            _IP -> line);
                    return EXIT_FAILURE;
                }
                _IP++;
            }
            _IP++; // Pass the final ]
            break;
        }

        else {
            fprintf(stderr, "Error: 'while' loop condition on line %d received an invalid type\n"
                    "While loop conditions must be given boolean values or expressions\n", _IP -> line);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

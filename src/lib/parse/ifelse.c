#include <stdio.h>
#include <stdlib.h>

#include "../tokenlib.h"
#include "expressions.h"
#include "variables.h"
#include "parselib.h"
#include "ifelse.h"

int parse_ifelse(Variable **table) {
    int result;

    // _IP currently points at the 'if' token; the next token should begin a
    // boolean expression
    _IP++;
    Token expr_result = parse_expression(table);

    // If parse_expression gives a DISCARD token, it should have already
    // printed an error message, so we can just immediately fail here
    if (expr_result.type == DISCARD)
        return EXIT_FAILURE;

    _IP++;
    if (_IP -> type != LCURLY) {
        fprintf(stderr, "Error: Expected '{' after 'if' condition on line %d\n",
                _IP -> line);
        return EXIT_FAILURE;
    }

    // Pass the { to begin parsing the block
    _IP++;

    switch (expr_result.type) {
        // Continue the normal execution pattern until we find the closing
        // RCURLY. Then, skip the else block (if it exists)
        case TRUE:
            while ((result = initial_state(table)) != RCURLY) {
                if (result == EXIT_FAILURE)
                    return result;

                if (result == ENDPOINT) {
                    fprintf(stderr, "Error: Unterminated if block; '}' expected on line %d\n",
                            _IP -> line);
                    return EXIT_FAILURE;
                }
            }

            // See if there's an else block; if not, return
            _IP++;
            if (_IP -> type != ELSE)
                return EXIT_SUCCESS;

            // Otherwise, validate the syntax and skip it
            _IP++;
            if (_IP -> type != LCURLY) {
                fprintf(stderr, "Error: Expected '{' after 'else' on line %d\n",
                        _IP -> line);
                return EXIT_FAILURE;
            }

            while (_IP -> type != RCURLY) {
                if (_IP -> type == ENDPOINT) {
                    fprintf(stderr, "Error: Unterminated else block; '}' expected on line %d\n",
                            _IP -> line);
                    return EXIT_FAILURE;
                }
                _IP++;
            }

            // Increment IP one more time to pass the '}'
            _IP++;
            break;

        // Skip the if block by finding the next RCURLY. If it does not exist,
        // that's an error. If the following token is an else, execute its block.
        case FALSE:
            while (_IP -> type != RCURLY) {
                if (_IP -> type == ENDPOINT) {
                    fprintf(stderr, "Error: Unterminated if block; '}' expected on line %d\n",
                            _IP -> line);
                    return EXIT_FAILURE;
                }
                _IP++;
            }
            _IP++;

            if (_IP -> type != ELSE)
                return EXIT_SUCCESS;

            _IP++;
            if (_IP -> type != LCURLY) {
                fprintf(stderr, "Error: Expected '{' after 'else' on line %d\n",
                        _IP -> line);
                return EXIT_FAILURE;
            }

            // Pass the '{'
            _IP++;

            while ((result = initial_state(table)) != RCURLY) {
                if (result == EXIT_FAILURE)
                    return result;

                if (result == ENDPOINT) {
                    fprintf(stderr, "Error: Unterminated else block; '}' expected on line %d\n",
                            _IP -> line);
                    return EXIT_FAILURE;
                }
            }

            // Increment IP one more time to pass the '}'
            _IP++;
            break;

        default:
            fprintf(stderr, "Error: 'if' statement on line %d received invalid type\n"
                    "Conditional statements must be given boolean values or expressions\n",
                    _IP -> line);
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

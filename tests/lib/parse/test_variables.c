#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "test_variables.h"
#include "../../../src/lib/parse/variables.h"

// Testing everything at once since all the functions are closely related
int test_variables() {
    Variable *vars = NULL;
    Token t1 = make_token_with_literal(IDENTIFIER, (Literal){.Name=strdup("_ident1")});
    Token t2 = make_token_with_literal(IDENTIFIER, (Literal){.Name=strdup("_ident2")});
    set_variable(&vars, t1.literal.Name, make_token(TRUE));
    set_variable(&vars, t2.literal.Name,
                 make_token_with_literal(NUMBER, (Literal){.Number=10000}));

    Token *r1 = get_variable(&vars, "_ident1");
    Token *r2 = get_variable(&vars, "_ident2");
    Token *r3 = get_variable(&vars, "nothere");

    assert(r1 -> type == TRUE);
    assert(r2 -> type == NUMBER);
    assert(r2 -> literal.Number == 10000);
    assert(r3 == NULL);

    // Update variable
    set_variable(&vars, t1.literal.Name,
                 make_token_with_literal
                 (STRING, (Literal){.String=strdup("helloworld")}));

    Token *r4 = get_variable(&vars, "_ident1");
    assert(r4 -> type == STRING);
    assert(strcmp(r4 -> literal.Name, "helloworld") == 0);

    destroy_variables(&vars);
    free(t1.literal.Name);
    free(t2.literal.Name);
    return EXIT_SUCCESS;
}

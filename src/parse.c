#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse.h"
#include "lib/parselib.h"
#include "lib/stack.h"
#include "lib/tokenlib.h"


int parse(TokenList *token_list) {
    ip     = token_list -> tokens;
    length = token_list -> used;

    // Walk the token list
    while (ip < ip + length) {
        ;
    }
}

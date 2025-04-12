#include <stdbool.h>

#include "utils.h"
#include "../src/lib/tokenlib.h"

_Bool token_eq(Token l, Token r) {
    if (l.type != r.type)
        return false;

    // We already verified the types are equivalent,
    // so we only need to check one
    if (l.type == TRUE || l.type == FALSE)
        return true;

    // Discard tokens indicate failure
    if (l.type == DISCARD)
        return true;

    if (l.type != NUMBER)
        return false; // Tokens should only be boolean or numeric here

    return (l.literal.Number == r.literal.Number);
}

#include "parselib.h"
#include "tokenlib.h"
#include "stack.h"


// Instruction pointer; pointer to current token being parsed
// Since this is accessed and mutated everywhere, it's useful to make it global
Token *_ip;

// Length of token list being parsed; we need to check this in many functions
// to not overflow, so it's globally accessible. Should not be mutated!
long _length;


// Evaluate an arithmetic expression; _ip should be pointing to an operator
// when this is called
double expression() {
    Token l = _ip[-1];
    Token r = _ip[1];
    
    if (l.type != NUMBER || r.type != NUMBER) {
        // FIXME Error
    }

    switch (_ip -> type) {
        case PLUS:
            return l.literal.Number + r.literal.Number;
            break;

        case MINUS:
            return l.literal.Number - r.literal.Number;
            break;

        case STAR:
            return l.literal.Number * r.literal.Number;
            break;

        case SLASH:
            return l.literal.Number / r.literal.Number;
            break;

        default:
            // FIXME error
            break;
    }
}

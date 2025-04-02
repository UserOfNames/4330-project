#include "parselib.h"
#include "queue.h"
#include "stack.h"
#include "tokenlib.h"


// Instruction pointer; pointer to current token being parsed
// Since this is accessed and mutated everywhere, it's useful to make it global
Token *_IP;

// Length of token list being parsed; we need to check this in many functions
// to not overflow, so it's globally accessible. Should not be mutated!
long _length;


// When IP finds a number, that indicates an arithmetic expression.
// When this is called, IP should be pointing to that first number.
// This evaluates the expression using a modified shunting-yard algorithm
double expression() {
    Queue output = make_queue();
    // Note that this stack implementation was made to track pointers to
    // tokens, not tokens. It would be better to track the tokens themselves,
    // but C sucks for generic programming and making an entire other stack for
    // this would be bulky and bug-prone, so I'm just doing this.
    Stack operators = make_stack();

    switch (_IP -> type) {
        case LPAREN:
            break;

        case RPAREN:
            break;

        case PLUS:
            break;

        case MINUS:
            break;

        case STAR:
            break;

        case SLASH:
            break;

        case NUMBER:
            enqueue(&output, _IP -> literal.Number);
            break;

        // All other types are invalid for an arithmetic expression,
        // so finish evaluating
        default:
            break;
    }
}

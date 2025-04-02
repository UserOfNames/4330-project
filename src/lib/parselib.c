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


// Evaluate an arithmetic expression; _ip should be pointing to a NUMBER
double expression() {
    
}

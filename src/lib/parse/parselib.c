#include "../tokenlib.h"

// Instruction pointer; pointer to current token being parsed
// Since this is accessed and mutated everywhere, it's useful to make it global
Token *_IP;

// Line number, tracked for error reporting; updated on each token
int line;

#ifndef STACK_H
#define STACK_H

#include "tokenlib.h"

// Stack containing token pointers
// Designed to manage control flow in the parser
typedef struct {
    Token **stack;
    long capacity;
    long used;
} Stack;

Stack make_stack();
int push(Stack *stack, Token *token);
Token* pop(Stack *stack);
Token* top(Stack *stack);

#endif

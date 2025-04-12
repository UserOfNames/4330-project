#ifndef PTR_STACK_H
#define PTR_STACK_H

#include <stdbool.h>

#include "../tokenlib.h"

// Stack containing token pointers
// Designed to manage control flow in the parser
typedef struct {
    Token **stack;
    long capacity;
    long used;
} PtrStack;

PtrStack make_ptr_stack();
int push_ptr(PtrStack *stack, Token *token);
Token* pop_ptr(PtrStack *stack);
Token* top_ptr(PtrStack *stack);
_Bool is_empty_ptr(PtrStack *stack);
void destroy_ptr_stack(PtrStack *stack);

#endif

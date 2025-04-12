#ifndef TOKEN_STACK_H
#define TOKEN_STACK_H

#include <stdbool.h>

#include "../tokenlib.h"

// Stack containing tokens
// Used for expression parsing
typedef struct {
    Token *stack;
    long capacity;
    long used;
} TokenStack;

TokenStack make_token_stack();
int push_token(TokenStack *stack, Token token);
Token pop_token(TokenStack *stack);
Token top_token(TokenStack *stack);
_Bool is_empty_token(TokenStack *stack);
void destroy_token_stack(TokenStack *stack);

#endif

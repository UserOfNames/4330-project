#include <stdlib.h>

#include "token_stack.h"

TokenStack make_token_stack() {
    TokenStack stack = {
        .stack = NULL,
        .capacity = 0,
        .used = 0,
    };

    return stack;
}

int push_token(TokenStack *stack, Token token) {
    long capacity = stack -> capacity;
    long used     = stack -> used;
    Token *temp;

    // Grow the stack as needed
    if (capacity <= used) {
        stack -> capacity = (capacity == 0) ? 16 : capacity * 2;

        // Check that realloc() succeeded to prevent memory leaks
        temp = (Token*)realloc(stack -> stack, stack -> capacity * sizeof(Token));

        if (temp == NULL) {
            return EXIT_FAILURE;
        }

        stack -> stack = temp;
    }

    stack -> stack[stack -> used] = token;
    stack -> used++;

    return EXIT_SUCCESS;
}

// The caller must verify that the stack is not empty before popping
Token pop_token(TokenStack *stack) {
    Token result;

    result = (stack -> stack)[stack -> used - 1];
    (stack -> used)--;

    return result;
}

// The caller must verify that the stack is not empty before checking 
Token top_token(TokenStack *stack) {
    return (stack -> stack)[stack -> used - 1];
}

_Bool is_empty_token(TokenStack *stack) {
    return ((stack -> used) <= 0 || (stack -> stack) == NULL);
}

// Free the stack
void destroy_token_stack(TokenStack *stack) {
    free(stack -> stack);
    stack -> used  = stack -> capacity = 0;
    stack -> stack = NULL;
}

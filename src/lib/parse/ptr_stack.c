#include <stdlib.h>

#include "ptr_stack.h"

PtrStack make_ptr_stack() {
    PtrStack stack = {
        .stack = NULL,
        .capacity = 0,
        .used = 0,
    };

    return stack;
}

int push_ptr(PtrStack *stack, Token *token) {
    long capacity = stack -> capacity;
    long used     = stack -> used;
    Token **temp;

    // Grow the stack as needed
    if (capacity <= used) {
        stack -> capacity = (capacity == 0) ? 16 : capacity * 2;

        // Check that realloc() succeeded to prevent memory leaks
        temp = (Token**)realloc(stack -> stack, stack -> capacity * sizeof(Token*));

        if (temp == NULL) {
            return EXIT_FAILURE;
        }

        stack -> stack = temp;
    }

    stack -> stack[stack -> used] = token;
    stack -> used++;

    return EXIT_SUCCESS;
}

Token* pop_ptr(PtrStack *stack) {
    Token *result;

    if (stack -> used == 0)
        return NULL;

    result = (stack -> stack)[stack -> used - 1];
    (stack -> stack)[stack -> used - 1] = NULL;
    (stack -> used)--;

    return result;
}

Token* top_ptr(PtrStack *stack) {
    if (stack -> used == 0)
        return NULL;

    return (stack -> stack)[stack -> used - 1];
}

_Bool is_empty_ptr(PtrStack *stack) {
    return ((stack -> used) <= 0 || (stack -> stack) == NULL);
}

// Free the stack
void destroy_ptr_stack(PtrStack *stack) {
    free(stack -> stack);
    stack -> used  = stack -> capacity = 0;
    stack -> stack = NULL;
}

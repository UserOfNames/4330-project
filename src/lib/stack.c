#include <stdlib.h>

#include "stack.h"

Stack make_stack() {
    Stack stack = {
        .stack = NULL,
        .capacity = 0,
        .used = 0,
    };

    return stack;
}

int push(Stack *stack, Token *token) {
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

Token* pop(Stack *stack) {
    Token **temp, *result;

    if (stack -> used == 0)
        return NULL;

    result = (stack -> stack)[stack -> used - 1];
    (stack -> stack)[stack -> used - 1] = NULL;
    (stack -> used)--;

    return result;
}

Token* top(Stack *stack) {
    if (stack -> used == 0)
        return NULL;

    return (stack -> stack)[stack -> used - 1];
}

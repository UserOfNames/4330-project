#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "test_stack.h"
#include "../src/lib/stack.h"

int test_make_stack() {
    Stack s = make_stack();
    assert(s.stack == NULL);
    assert(s.capacity == 0);
    assert(s.used == 0);

    return EXIT_SUCCESS;
}

// This is important to cleanup other tests, so we'll do it
// first, even though it means manually wiring up a stack
// (since we haven't tested push() yet, we can't use it here)
int test_destroy_stack() {
    Stack s = make_stack();
    s.used = 0;
    s.capacity = 16;
    s.stack = (Token**)malloc(s.capacity * sizeof(Token*));

    Token t0 = (Token){.type=PLUS, .literal.Number=0};
    Token t1 = (Token){.type=MINUS, .literal.Number=0};

    s.stack[0] = &t0;
    s.stack[1] = &t1;

    destroy_stack(&s);
    assert(s.used == 0);
    assert(s.capacity == 0);
    assert(s.stack == NULL);

    return EXIT_SUCCESS;
}

int test_push() {
    Token t;

    Stack s = make_stack();
    // Not using make_token() to avoid inter-test dependency
    Token t1 = (Token){.type=PLUS, .literal.Number=0};
    push(&s, &t1);
    assert(s.capacity == 16);
    assert(s.used == 1);
    assert(s.stack != NULL);
    t = *s.stack[s.used - 1];
    assert(t.type == PLUS);

    Token t2 = (Token){.type=NUMBER, .literal.Number=6.5};
    push(&s, &t2);
    assert(s.capacity == 16);
    assert(s.used == 2);
    assert(s.stack != NULL);
    t = *s.stack[s.used - 1];
    assert(t.type == NUMBER);
    assert(t.literal.Number == 6.5);

    int i;
    // Capacity should not grow until 16 elements are pushed
    for (i=0; i<14; i++)
        push(&s, &t2);

    assert(s.capacity == 16);
    assert(s.used == 16);

    push(&s, &t2);
    assert(s.capacity == 32);
    assert(s.used == 17);
    assert(s.stack != NULL);
    t = *s.stack[s.used - 1];
    assert(t.type == NUMBER);
    assert(t.literal.Number == 6.5);

    destroy_stack(&s);

    return EXIT_SUCCESS;
}

int test_top() {
    Stack s = make_stack();
    Token t1 = (Token){.type=PLUS, .literal.Number=0};
    Token t2 = (Token){.type=NUMBER, .literal.Number=6.5};

    // Empty stack
    assert(top(&s) == NULL);

    push(&s, &t1);
    // Not bothering to check in-depth; since this is a pointer,
    // the addresses being equal should suffice for equality
    assert(top(&s) == s.stack[s.used - 1]);

    push(&s, &t2);
    assert(top(&s) == s.stack[s.used - 1]);

    destroy_stack(&s);

    return EXIT_SUCCESS;
}

int test_pop() {
    Stack s = make_stack();

    Token t1 = (Token){.type=PLUS, .literal.Number=0};
    Token t2 = (Token){.type=NUMBER, .literal.Number=6.5};

    push(&s, &t1);
    push(&s, &t2);

    Token *r1 = pop(&s);
    assert(top(&s) == &t1);
    assert(r1 -> type == NUMBER);
    assert(r1 -> literal.Number == 6.5);

    Token *r2 = pop(&s);
    assert(top(&s) == NULL);
    assert(r2 -> type == PLUS);
    assert(r2 -> literal.Number == 0);

    assert(pop(&s) == NULL);

    return EXIT_SUCCESS;
}

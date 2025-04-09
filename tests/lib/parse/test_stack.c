#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "test_stack.h"
#include "../../../src/lib/parse/ptr_stack.h"

// NOTE: Although there are two types of stack, they function
// identically, only taking different types. So, I'm only testing
// one type of stack (pointer stack, since that's more complex)

int test_make_ptr_stack() {
    PtrStack s = make_ptr_stack();
    assert(s.stack == NULL);
    assert(s.capacity == 0);
    assert(s.used == 0);

    return EXIT_SUCCESS;
}

// This is important to cleanup other tests, so we'll do it
// first, even though it means manually wiring up a stack
// (since we haven't tested push_ptr() yet, we can't use it here)
int test_destroy_ptr_stack() {
    PtrStack s = make_ptr_stack();
    s.used = 0;
    s.capacity = 16;
    s.stack = (Token**)malloc(s.capacity * sizeof(Token*));

    Token t0 = (Token){.type=PLUS, .literal.Number=0};
    Token t1 = (Token){.type=MINUS, .literal.Number=0};

    s.stack[0] = &t0;
    s.stack[1] = &t1;

    destroy_ptr_stack(&s);
    assert(s.used == 0);
    assert(s.capacity == 0);
    assert(s.stack == NULL);

    return EXIT_SUCCESS;
}

int test_push_ptr() {
    Token t;

    PtrStack s = make_ptr_stack();
    // Not using make_token() to avoid inter-test dependency
    Token t1 = (Token){.type=PLUS, .literal.Number=0};
    push_ptr(&s, &t1);
    assert(s.capacity == 16);
    assert(s.used == 1);
    assert(s.stack != NULL);
    t = *s.stack[s.used - 1];
    assert(t.type == PLUS);

    Token t2 = (Token){.type=NUMBER, .literal.Number=6.5};
    push_ptr(&s, &t2);
    assert(s.capacity == 16);
    assert(s.used == 2);
    assert(s.stack != NULL);
    t = *s.stack[s.used - 1];
    assert(t.type == NUMBER);
    assert(t.literal.Number == 6.5);

    int i;
    // Capacity should not grow until 16 elements are push_ptred
    for (i=0; i<14; i++)
        push_ptr(&s, &t2);

    assert(s.capacity == 16);
    assert(s.used == 16);

    push_ptr(&s, &t2);
    assert(s.capacity == 32);
    assert(s.used == 17);
    assert(s.stack != NULL);
    t = *s.stack[s.used - 1];
    assert(t.type == NUMBER);
    assert(t.literal.Number == 6.5);

    destroy_ptr_stack(&s);
    return EXIT_SUCCESS;
}

int test_top_ptr() {
    PtrStack s = make_ptr_stack();
    Token t1 = (Token){.type=PLUS, .literal.Number=0};
    Token t2 = (Token){.type=NUMBER, .literal.Number=6.5};

    // Empty stack
    assert(top_ptr(&s) == NULL);

    push_ptr(&s, &t1);
    // Not bothering to check in-depth; since this is a pointer,
    // the addresses being equal should suffice for equality
    assert(top_ptr(&s) == s.stack[s.used - 1]);

    push_ptr(&s, &t2);
    assert(top_ptr(&s) == s.stack[s.used - 1]);

    destroy_ptr_stack(&s);
    return EXIT_SUCCESS;
}

int test_pop_ptr() {
    PtrStack s = make_ptr_stack();

    Token t1 = (Token){.type=PLUS, .literal.Number=0};
    Token t2 = (Token){.type=NUMBER, .literal.Number=6.5};

    push_ptr(&s, &t1);
    push_ptr(&s, &t2);

    Token *r1 = pop_ptr(&s);
    assert(top_ptr(&s) == &t1);
    assert(r1 -> type == NUMBER);
    assert(r1 -> literal.Number == 6.5);

    Token *r2 = pop_ptr(&s);
    assert(top_ptr(&s) == NULL);
    assert(r2 -> type == PLUS);
    assert(r2 -> literal.Number == 0);

    assert(pop_ptr(&s) == NULL);

    destroy_ptr_stack(&s);
    return EXIT_SUCCESS;
}

int test_is_empty_ptr() {
    PtrStack s = make_ptr_stack();
    Token t1 = make_token(PLUS);

    assert(is_empty_ptr(&s));

    push_ptr(&s, &t1);
    assert(!is_empty_ptr(&s));

    destroy_ptr_stack(&s);
    return EXIT_SUCCESS;
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_stack.h"
#include "../../../src/lib/parse/token_stack.h"
#include "../../utils.h"

int test_make_token_stack() {
    TokenStack s = make_token_stack();
    assert(s.stack == NULL);
    assert(s.capacity == 0);
    assert(s.used == 0);

    return EXIT_SUCCESS;
}

// This is important to cleanup other tests, so we'll do it
// first, even though it means manually wiring up a stack
// (since we haven't tested push_token() yet, we can't use it here)
int test_destroy_token_stack() {
    TokenStack s = make_token_stack();
    s.used = 0;
    s.capacity = 16;
    s.stack = (Token*)malloc(s.capacity * sizeof(Token*));

    Token t0 = (Token){.type=PLUS, .literal.Number=0};
    Token t1 = (Token){.type=MINUS, .literal.Number=0};

    s.stack[0] = t0;
    s.stack[1] = t1;

    destroy_token_stack(&s);
    assert(s.used == 0);
    assert(s.capacity == 0);
    assert(s.stack == NULL);

    return EXIT_SUCCESS;
}

int test_push_token() {
    Token t;

    TokenStack s = make_token_stack();
    // Not using make_token() to avoid inter-test dependency
    Token t1 = (Token){.type=PLUS, .literal.Number=0};
    push_token(&s, t1);
    assert(s.capacity == 16);
    assert(s.used == 1);
    assert(s.stack != NULL);
    t = s.stack[s.used - 1];
    assert(t.type == PLUS);

    Token t2 = (Token){.type=NUMBER, .literal.Number=6.5};
    push_token(&s, t2);
    assert(s.capacity == 16);
    assert(s.used == 2);
    assert(s.stack != NULL);
    t = s.stack[s.used - 1];
    assert(t.type == NUMBER);
    assert(t.literal.Number == 6.5);

    int i;
    // Capacity should not grow until 16 elements are pushed
    for (i=0; i<14; i++)
        push_token(&s, t2);

    assert(s.capacity == 16);
    assert(s.used == 16);

    push_token(&s, t2);
    assert(s.capacity == 32);
    assert(s.used == 17);
    assert(s.stack != NULL);
    t = s.stack[s.used - 1];
    assert(t.type == NUMBER);
    assert(t.literal.Number == 6.5);

    destroy_token_stack(&s);
    return EXIT_SUCCESS;
}

int test_top_token() {
    TokenStack s = make_token_stack();
    Token t1 = (Token){.type=PLUS, .literal.Number=0};
    Token t2 = (Token){.type=NUMBER, .literal.Number=6.5};

    push_token(&s, t1);
    assert(top_token(&s).type ==  s.stack[s.used - 1].type);

    push_token(&s, t2);
    assert(top_token(&s).type == s.stack[s.used - 1].type);

    destroy_token_stack(&s);
    return EXIT_SUCCESS;
}

int test_pop_token() {
    TokenStack s = make_token_stack();

    Token t1 = (Token){.type=PLUS, .literal.Number=0};
    Token t2 = (Token){.type=NUMBER, .literal.Number=6.5};

    push_token(&s, t1);
    push_token(&s, t2);

    Token r1 = pop_token(&s);
    assert(top_token(&s).type == t1.type);
    assert(r1.type == NUMBER);
    assert(r1.literal.Number == 6.5);

    Token r2 = pop_token(&s);
    assert(r2.type == PLUS);
    assert(r2.literal.Number == 0);

    destroy_token_stack(&s);
    return EXIT_SUCCESS;
}

int test_is_empty_token() {
    TokenStack s = make_token_stack();
    Token t1 = make_token(PLUS);

    assert(is_empty_token(&s));

    push_token(&s, t1);
    assert(!is_empty_token(&s));

    destroy_token_stack(&s);
    return EXIT_SUCCESS;
}

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_queue.h"
#include "../../../src/lib/parse/queue.h"

int test_make_queue() {
    Queue q = make_queue();
    assert(q.head == NULL);
    assert(q.tail == NULL);
    assert(q.length == 0);

    return EXIT_SUCCESS;
}

int test_make_node() {
    Node *n = make_node(make_token_with_literal(NUMBER, (Literal){.Number=100.3}));

    assert(n -> token.literal.Number == 100.3);
    assert(n -> next == NULL);

    return EXIT_SUCCESS;
}

// This function is useful for cleaning up future tests,
// so we manually construct a linked list to test it now
// Unfortunately, testing that memory was freed is impossible,
// so this test is kinda lame
int test_destroy_queue() {
    Queue q = make_queue();

    Node *n0 = make_node(make_token_with_literal(NUMBER, (Literal){.Number=0}));
    Node *n1 = make_node(make_token_with_literal(NUMBER, (Literal){.Number=1}));
    Node *n2 = make_node(make_token_with_literal(NUMBER, (Literal){.Number=2}));

    n0 -> next = n1;
    n1 -> next = n2;
    assert(n2 -> next == NULL);

    q.head = n0;
    q.tail = n2;
    q.length = 3;

    destroy_queue(&q);

    assert(q.length == 0);
    assert(q.tail == NULL);
    assert(q.head == NULL);

    return EXIT_SUCCESS;
}

int test_enqueue() {
    Queue q = make_queue();

    enqueue(&q, make_token_with_literal(NUMBER, (Literal){.Number=100.6}));
    assert(q.length == 1);
    assert(q.tail == q.head);
    assert(q.head -> next == NULL);
    assert(q.tail -> next == NULL);
    assert(q.head -> token.literal.Number == 100.6);
    assert(q.tail -> token.literal.Number == 100.6);

    enqueue(&q, make_token_with_literal(NUMBER, (Literal){.Number=-4.3}));
    assert(q.length == 2);
    assert(q.tail != q.head);
    assert(q.head -> next == q.tail);
    assert(q.tail -> next == NULL);
    assert(q.head -> token.literal.Number == 100.6);
    assert(q.tail -> token.literal.Number == -4.3);

    enqueue(&q, make_token_with_literal(NUMBER, (Literal){.Number=5.1}));
    assert(q.length == 3);
    assert(q.tail != q.head);
    assert(q.tail -> next == NULL);
    assert(q.head -> token.literal.Number == 100.6);
    assert(q.head -> next -> token.literal.Number == -4.3);
    assert(q.tail -> token.literal.Number == 5.1);

    destroy_queue(&q);

    return EXIT_SUCCESS;
}

int test_dequeue() {
    Queue q = make_queue();
    Token result;

    enqueue(&q, make_token_with_literal(NUMBER, (Literal){.Number=0}));
    enqueue(&q, make_token_with_literal(NUMBER, (Literal){.Number=1}));
    enqueue(&q, make_token_with_literal(NUMBER, (Literal){.Number=2}));

    result = dequeue(&q);
    assert(result.literal.Number == 0);
    assert(q.length == 2);

    result = dequeue(&q);
    assert(result.literal.Number == 1);
    assert(q.length == 1);
    assert(q.head == q.tail);

    result = dequeue(&q);
    assert(result.literal.Number == 2);
    assert(q.length == 0);
    assert(q.head == NULL);
    assert(q.tail == NULL);

    return EXIT_SUCCESS;
}

int test_peek() {
    Queue q = make_queue();
    Token result;

    enqueue(&q, make_token_with_literal(NUMBER, (Literal){.Number=123}));
    enqueue(&q, make_token_with_literal(NUMBER, (Literal){.Number=1234}));

    result = peek(&q);
    assert(result.literal.Number == 123);

    dequeue(&q);
    result = peek(&q);
    assert(result.literal.Number == 1234);

    return EXIT_SUCCESS;
}

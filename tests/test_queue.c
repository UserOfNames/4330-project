#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_queue.h"
#include "../src/lib/queue.h"

int test_make_queue() {
    Queue q = make_queue();
    assert(q.head == NULL);
    assert(q.tail == NULL);
    assert(q.length == 0);

    return EXIT_SUCCESS;
}

int test_make_node() {
    Node *n = make_node(100.3);

    assert(n -> num == 100.3);
    assert(n -> next == NULL);

    return EXIT_SUCCESS;
}

// This function is useful for cleaning up future tests,
// so we manually construct a linked list to test it now
// Unfortunately, testing that memory was freed is impossible,
// so this test is kinda lame
int test_destroy_queue() {
    Queue q = make_queue();

    Node *n0 = make_node(0);
    Node *n1 = make_node(1);
    Node *n2 = make_node(2);

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

    enqueue(&q, 100.6);
    assert(q.length == 1);
    assert(q.tail == q.head);
    assert(q.head -> next == NULL);
    assert(q.tail -> next == NULL);
    assert(q.head -> num == 100.6);
    assert(q.tail -> num == 100.6);

    enqueue(&q, -4.3);
    assert(q.length == 2);
    assert(q.tail != q.head);
    assert(q.head -> next == q.tail);
    assert(q.tail -> next == NULL);
    assert(q.head -> num == 100.6);
    assert(q.tail -> num == -4.3);

    enqueue(&q, 5.1);
    assert(q.length == 3);
    assert(q.tail != q.head);
    assert(q.tail -> next == NULL);
    assert(q.head -> num == 100.6);
    assert(q.head -> next -> num == -4.3);
    assert(q.tail -> num == 5.1);

    destroy_queue(&q);

    return EXIT_SUCCESS;
}

int test_dequeue() {
    Queue q = make_queue();
    DoubleResult result;

    // Empty queue
    result = dequeue(&q);
    assert(result.success == false);

    enqueue(&q, 0);
    enqueue(&q, 1);
    enqueue(&q, 2);

    result = dequeue(&q);
    assert(result.success == true);
    assert(result.result  == 0);
    assert(q.length == 2);

    result = dequeue(&q);
    assert(result.success == true);
    assert(result.result  == 1);
    assert(q.length == 1);
    assert(q.head == q.tail);

    result = dequeue(&q);
    assert(result.success == true);
    assert(result.result  == 2);
    assert(q.length == 0);
    assert(q.head == NULL);
    assert(q.tail == NULL);

    return EXIT_SUCCESS;
}

int test_peek() {
    Queue q = make_queue();
    DoubleResult result;

    enqueue(&q, 123);
    enqueue(&q, 1234);

    result = peek(&q);
    assert(result.success == true);
    assert(result.result == 123);

    dequeue(&q);
    result = peek(&q);
    assert(result.success == true);
    assert(result.result == 1234);

    dequeue(&q);
    result = peek(&q);
    assert(result.success == false);

    return EXIT_SUCCESS;
}

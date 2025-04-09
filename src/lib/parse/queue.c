#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

// Make an empty queue
Queue make_queue() {
    Queue res = {
        .head = NULL,
        .tail = NULL,
        .length = 0,
    };

    return res;
}

Node* make_node(Token token) {
    Node *res = (Node*)malloc(sizeof(Node));

    if (res == NULL)
        return NULL;

    res -> token = token;
    res -> next  = NULL;

    return res;
}

int enqueue(Queue *queue, Token token) {
    Node *new_tail = make_node(token);

    if (new_tail == NULL)
        return EXIT_FAILURE;

    if (queue -> length == 0) {
        queue -> head = queue -> tail = new_tail;
    } else {
        queue -> tail -> next = new_tail;
        queue -> tail = new_tail;
    }

    (queue -> length)++;

    return EXIT_SUCCESS;
}

// Free the head node, return its value, and update the head
// to point to the previous head's 'next'
// The caller should determine whether the queue is empty
// before calling this
Token dequeue(Queue *queue) {
    Node *killme = queue -> head;
    Token result = killme -> token;

    queue -> head = queue -> head -> next;

    free(killme);
    (queue -> length)--;

    if (queue -> length == 0)
        queue -> head = queue -> tail = NULL;

    return result;
}

Token peek(Queue *queue) {
    Token result;

    if (queue -> length == 0) {
        result.type = DISCARD;
        return result;
    }

    result = queue -> head -> token;

    return result;
}

// Free all nodes in the queue
void destroy_queue(Queue *queue) {
    Node* current = queue -> head;
    Node* next;
    if (current == NULL)
        return;

    while (current != NULL) {
        next = current -> next;
        free(current);
        current = next;
    }

    queue -> head = queue -> tail = NULL;
    queue -> length = 0;
}

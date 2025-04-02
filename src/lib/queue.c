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

Node* make_node(double num) {
    Node *res = (Node*)malloc(sizeof(Node));

    if (res == NULL)
        return NULL;

    res -> num  = num;
    res -> next = NULL;

    return res;
}

int enqueue(Queue *queue, double num) {
    Node *new_tail = make_node(num);

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
DoubleResult dequeue(Queue *queue) {
    DoubleResult result = { .success = true };

    if (queue -> length == 0) {
        fprintf(stderr, "Error: Attempted to dequeue an empty queue\n");
        // I hate C error handling I hate C error handling I hate C error handling
        result.success = false;
        return result;
    }

    Node *killme = queue -> head;
    result.result = killme -> num;

    queue -> head = queue -> head -> next;

    free(killme);
    (queue -> length)--;

    if (queue -> length == 0)
        queue -> head = queue -> tail = NULL;

    return result;
}

DoubleResult peek(Queue *queue) {
    DoubleResult result = { .success = true };

    if (queue -> length == 0) {
        result.success = false;
        return result;
    }

    result.result = queue -> head -> num;

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

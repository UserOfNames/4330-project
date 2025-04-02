#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#include "tokenlib.h"

// Node for the linked list underlying the queue implementation
typedef struct Node Node;
struct Node {
    double num;
    Node *next;
};

// A queue used to track numbers for the shunting yard algorithm
typedef struct {
    Node *head;
    Node *tail;
    long length;
} Queue;

// Result of dequeue
typedef struct {
    _Bool success;
    double result;
} DoubleResult;

Queue make_queue();
Node* make_node(double num);
int enqueue(Queue *queue, double num);
DoubleResult dequeue(Queue *queue);
DoubleResult peek(Queue *queue);
void destroy_queue(Queue *queue);

#endif

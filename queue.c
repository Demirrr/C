#include <stdio.h>
#include "queue.h"

// Initializes an empty queue.
void init_queue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

// Checks if the queue is full. This implementation uses a circular array.
bool is_full_queue(const Queue* q) {
    return (q->front == 0 && q->rear == MAX_QUEUE_SIZE - 1) || 
           (q->front == q->rear + 1);
}

// Checks if the queue is empty.
bool is_empty_queue(const Queue* q) {
    return q->front == -1;
}

// Adds an item to the rear of the queue (enqueue).
bool enqueue(Queue* q, int value) {
    if (is_full_queue(q)) {
        printf("Error: Queue overflow!\n");
        return false;
    }
    // If the queue is initially empty, set front to 0.
    if (q->front == -1) {
        q->front = 0;
    }
    // Increment rear circularly.
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->items[q->rear] = value;
    return true;
}

// Removes an item from the front of the queue (dequeue).
bool dequeue(Queue* q, int* value) {
    if (is_empty_queue(q)) {
        printf("Error: Queue underflow!\n");
        return false;
    }
    *value = q->items[q->front];
    // If this was the last element, reset front and rear.
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        // Increment front circularly.
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    }
    return true;
}


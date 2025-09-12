#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

// Define the maximum size of the queue.
#define MAX_QUEUE_SIZE 100

// Define the structure for a queue.
// The queue is implemented using a fixed-size circular array.
typedef struct {
    int items[MAX_QUEUE_SIZE];
    int front; // Index of the front element
    int rear;  // Index of the rear element
} Queue;

// Function prototypes for queue operations.

/**
 * @brief Initializes an empty queue.
 * * @param q A pointer to the queue to be initialized.
 */
void init_queue(Queue* q);

/**
 * @brief Checks if the queue is full.
 * * @param q A pointer to the queue.
 * @return true if the queue is full, false otherwise.
 */
bool is_full_queue(const Queue* q);

/**
 * @brief Checks if the queue is empty.
 * * @param q A pointer to the queue.
 * @return true if the queue is empty, false otherwise.
 */
bool is_empty_queue(const Queue* q);

/**
 * @brief Adds an item to the rear of the queue (enqueue).
 * * @param q A pointer to the queue.
 * @param value The value to add.
 * @return true if the enqueue was successful, false otherwise.
 */
bool enqueue(Queue* q, int value);

/**
 * @brief Removes an item from the front of the queue (dequeue).
 * * @param q A pointer to the queue.
 * @param value A pointer to store the removed value.
 * @return true if the dequeue was successful, false otherwise.
 */
bool dequeue(Queue* q, int* value);

#endif // QUEUE_H


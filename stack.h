#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

// Define the maximum size of the stack.
#define MAX_STACK_SIZE 100

// Define the structure for a stack.
// The stack is implemented using a fixed-size array.
typedef struct {
    int items[MAX_STACK_SIZE];
    int top; // top of the stack, initialized to -1 for an empty stack
} Stack;

// Function prototypes for stack operations.

/**
 * @brief Initializes an empty stack.
 * * @param s A pointer to the stack to be initialized.
 */
void init_stack(Stack* s);

/**
 * @brief Checks if the stack is full.
 * * @param s A pointer to the stack.
 * @return true if the stack is full, false otherwise.
 */
bool is_full(const Stack* s);

/**
 * @brief Checks if the stack is empty.
 * * @param s A pointer to the stack.
 * @return true if the stack is empty, false otherwise.
 */
bool is_empty(const Stack* s);

/**
 * @brief Pushes an item onto the top of the stack.
 * * @param s A pointer to the stack.
 * @param value The value to push.
 * @return true if the push was successful, false otherwise.
 */
bool push(Stack* s, int value);

/**
 * @brief Pops an item from the top of the stack.
 * * @param s A pointer to the stack.
 * @param value A pointer to store the popped value.
 * @return true if the pop was successful, false otherwise.
 */
bool pop(Stack* s, int* value);

/**
 * @brief Peeks at the top item of the stack without removing it.
 * * @param s A pointer to the stack.
 * @param value A pointer to store the top value.
 * @return true if the peek was successful, false otherwise.
 */
bool peek(const Stack* s, int* value);

#endif // STACK_H


#include <stdio.h>
#include "stack.h"

// Initializes an empty stack.
void init_stack(Stack* s) {
    s->top = -1; // -1 indicates an empty stack.
}

// Checks if the stack is full.
bool is_full(const Stack* s) {
    return s->top == MAX_STACK_SIZE - 1;
}

// Checks if the stack is empty.
bool is_empty(const Stack* s) {
    return s->top == -1;
}

// Pushes an item onto the top of the stack.
bool push(Stack* s, int value) {
    // Check for stack overflow.
    if (is_full(s)) {
        printf("Error: Stack overflow!\n");
        return false;
    }
    // Increment the top and add the new value.
    s->items[++(s->top)] = value;
    return true;
}

// Pops an item from the top of the stack.
bool pop(Stack* s, int* value) {
    // Check for stack underflow.
    if (is_empty(s)) {
        printf("Error: Stack underflow!\n");
        return false;
    }
    // Get the value from the top and then decrement the top.
    *value = s->items[(s->top)--];
    return true;
}

// Peeks at the top item of the stack without removing it.
bool peek(const Stack* s, int* value) {
    // Check if the stack is empty.
    if (is_empty(s)) {
        printf("Error: Stack is empty.\n");
        return false;
    }
    *value = s->items[s->top];
    return true;
}


/*
gcc -g main.c singly_linked_list.c queue.c stack.c binary_search_tree.c -o main && ./main
*/
#include <stdio.h>
#include "singly_linked_list.h"
#include "stack.h"
#include "queue.h"

int main() {
    // --- DEMONSTRATION OF SINGLY LINKED LIST ---
    printf("--- Singly Linked List Demonstration ---\n");
    
    LinkedList* list = init_list();
    if (list == NULL) {
        printf("Failed to initialize linked list. Exiting.\n");
        return 1;
    }

    // Insert elements at the end
    insert_at_end(list, 10);
    insert_at_end(list, 20);
    insert_at_end(list, 30);
    printf("After inserting 10, 20, 30 at end:\n");
    print_list(list); // Expected: List: 10 -> 20 -> 30 -> NULL

    // Insert an element at the beginning
    insert_at_beginning(list, 5);
    printf("After inserting 5 at beginning:\n");
    print_list(list); // Expected: List: 5 -> 10 -> 20 -> 30 -> NULL

    // Delete a node
    printf("Deleting node with data 20...\n");
    if (delete_node(list, 20)) {
        printf("Node deleted successfully.\n");
    } else {
        printf("Node not found.\n");
    }
    print_list(list); // Expected: List: 5 -> 10 -> 30 -> NULL

    // Delete the head node
    printf("Deleting node with data 5 (head)...\n");
    if (delete_node(list, 5)) {
        printf("Node deleted successfully.\n");
    } else {
        printf("Node not found.\n");
    }
    print_list(list); // Expected: List: 10 -> 30 -> NULL

    // Attempt to delete a non-existent node
    printf("Attempting to delete non-existent node 99...\n");
    if (delete_node(list, 99)) {
        printf("Node deleted successfully.\n");
    } else {
        printf("Node not found.\n");
    }
    print_list(list); // Expected: List: 10 -> 30 -> NULL

    // Cleanup the list
    destroy_list(list);
    printf("Linked list destroyed.\n\n");

    // --- DEMONSTRATION OF STACK ---
    printf("--- Stack Demonstration (LIFO) ---\n");
    Stack stack;
    init_stack(&stack);
    int value;

    // Push elements onto the stack
    printf("Pushing 10, 20, 30 onto the stack.\n");
    push(&stack, 10);
    push(&stack, 20);
    push(&stack, 30);

    // Peek at the top element
    if (peek(&stack, &value)) {
        printf("Top element is: %d\n", value); // Expected: 30
    }

    // Pop elements from the stack
    if (pop(&stack, &value)) {
        printf("Popped value: %d\n", value); // Expected: 30
    }
    if (peek(&stack, &value)) {
        printf("Top element is now: %d\n", value); // Expected: 20
    }
    if (pop(&stack, &value)) {
        printf("Popped value: %d\n", value); // Expected: 20
    }
    if (pop(&stack, &value)) {
        printf("Popped value: %d\n", value); // Expected: 10
    }

    // Try to pop from an empty stack
    if (pop(&stack, &value) == false) {
        printf("Successfully prevented stack underflow.\n\n");
    }

    // --- DEMONSTRATION OF QUEUE ---
    printf("--- Queue Demonstration (FIFO) ---\n");
    Queue queue;
    init_queue(&queue);

    // Enqueue elements
    printf("Enqueuing 100, 200, 300 to the queue.\n");
    enqueue(&queue, 100);
    enqueue(&queue, 200);
    enqueue(&queue, 300);

    // Dequeue elements
    if (dequeue(&queue, &value)) {
        printf("Dequeued value: %d\n", value); // Expected: 100
    }
    if (dequeue(&queue, &value)) {
        printf("Dequeued value: %d\n", value); // Expected: 200
    }
    if (dequeue(&queue, &value)) {
        printf("Dequeued value: %d\n", value); // Expected: 300
    }

    // Try to dequeue from an empty queue
    if (dequeue(&queue, &value) == false) {
        printf("Successfully prevented queue underflow!!.\n\n");
    }


    return 0;
}


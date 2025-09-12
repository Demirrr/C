#include <stdio.h>
#include <stdlib.h>
#include "singly_linked_list.h"

// Initializes a new, empty linked list.
LinkedList* init_list() {
    // Allocate memory for the LinkedList structure.
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list == NULL) {
        // Handle memory allocation failure.
        fprintf(stderr, "Error: Memory allocation failed for list.\n");
        return NULL;
    }
    // Set the head to NULL, indicating an empty list.
    list->head = NULL;
    return list;
}

// Inserts a new node at the beginning of the list.
void insert_at_beginning(LinkedList* list, int data) {
    // 1. Create a new node and allocate memory.
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        // Handle memory allocation failure.
        fprintf(stderr, "Error: Memory allocation failed for new node.\n");
        return;
    }

    // 2. Set the data of the new node.
    newNode->data = data;

    // 3. Point the new node's 'next' pointer to the current head.
    newNode->next = list->head;

    // 4. Update the list's head to point to the new node.
    list->head = newNode;
}

// Inserts a new node at the end of the list.
void insert_at_end(LinkedList* list, int data) {
    // 1. Create a new node and allocate memory.
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        // Handle memory allocation failure.
        fprintf(stderr, "Error: Memory allocation failed for new node.\n");
        return;
    }

    // 2. Set the data of the new node and set its 'next' pointer to NULL.
    newNode->data = data;
    newNode->next = NULL;

    // 3. If the list is empty, make the new node the head.
    if (list->head == NULL) {
        list->head = newNode;
        return;
    }

    // 4. Traverse the list to find the last node.
    Node* current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }

    // 5. Point the last node's 'next' pointer to the new node.
    current->next = newNode;
}

// Deletes the first occurrence of a node with the given data.
bool delete_node(LinkedList* list, int data) {
    // Case 1: The list is empty. Nothing to delete.
    if (list->head == NULL) {
        return false;
    }

    Node* current = list->head;
    Node* prev = NULL;

    // Case 2: The node to be deleted is the head.
    if (current != NULL && current->data == data) {
        list->head = current->next; // Change head.
        free(current); // Free old head.
        return true;
    }

    // Case 3: Search for the node to be deleted, keeping track of the previous node.
    while (current != NULL && current->data != data) {
        prev = current;
        current = current->next;
    }

    // If the data was not found in the list.
    if (current == NULL) {
        return false;
    }

    // Unlink the node from the list.
    prev->next = current->next;

    // Free the memory of the deleted node.
    free(current);
    return true;
}

// Prints all elements of the linked list.
void print_list(const LinkedList* list) {
    Node* current = list->head;
    printf("List: ");
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

// Deallocates all memory used by the linked list.
void destroy_list(LinkedList* list) {
    if (list == NULL) return;
    Node* current = list->head;
    Node* nextNode;
    // Iterate through the list and free each node.
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    // Free the LinkedList structure itself.
    free(list);
}


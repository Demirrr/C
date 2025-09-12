#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <stdbool.h>

// Define the structure for a single node in the linked list.
// Each node contains an integer data value and a pointer to the next node.
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Define the structure for the linked list itself, which holds a pointer
// to the head (first) node.
typedef struct LinkedList {
    Node* head;
} LinkedList;

// Function prototypes for linked list operations.

/**
 * @brief Initializes a new, empty linked list.
 * * @return A pointer to the newly created LinkedList.
 */
LinkedList* init_list();

/**
 * @brief Inserts a new node at the beginning of the list.
 * * @param list The linked list to modify.
 * @param data The integer data to be stored in the new node.
 */
void insert_at_beginning(LinkedList* list, int data);

/**
 * @brief Inserts a new node at the end of the list.
 * * @param list The linked list to modify.
 * @param data The integer data to be stored in the new node.
 */
void insert_at_end(LinkedList* list, int data);

/**
 * @brief Deletes the first occurrence of a node with the given data.
 * * @param list The linked list to modify.
 * @param data The data of the node to be deleted.
 * @return true if a node was deleted, false otherwise.
 */
bool delete_node(LinkedList* list, int data);

/**
 * @brief Prints all elements of the linked list from head to tail.
 * * @param list The linked list to print.
 */
void print_list(const LinkedList* list);

/**
 * @brief Deallocates all memory used by the linked list.
 * * @param list The linked list to destroy.
 */
void destroy_list(LinkedList* list);

#endif // SINGLY_LINKED_LIST_H


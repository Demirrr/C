#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stdbool.h>

// Node structure for doubly linked list
typedef struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

// Doubly linked list structure
typedef struct DoublyLinkedList {
    DNode* head;
    DNode* tail;
} DoublyLinkedList;

// Function prototypes
DoublyLinkedList* dll_init_list();
void dll_insert_at_beginning(DoublyLinkedList* list, int data);
void dll_insert_at_end(DoublyLinkedList* list, int data);
bool dll_delete_node(DoublyLinkedList* list, int data);
void dll_print_forward(const DoublyLinkedList* list);
void dll_print_backward(const DoublyLinkedList* list);
void dll_destroy_list(DoublyLinkedList* list);

#endif // DOUBLY_LINKED_LIST_H

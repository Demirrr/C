#include "doubly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Initializes a new empty doubly linked list.
 *
 * Returns: Pointer to a newly allocated DoublyLinkedList struct.
 */
DoublyLinkedList* dll_init_list() {
    DoublyLinkedList* list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (list) {
        list->head = NULL;
        list->tail = NULL;
    }
    return list;
}

/**
 * Inserts a new node with the given data at the beginning of the list.
 *
 * list: Pointer to the DoublyLinkedList.
 * data: Integer value to insert.
 */
void dll_insert_at_beginning(DoublyLinkedList* list, int data) {
    DNode* new_node = (DNode*)malloc(sizeof(DNode));
    if (!new_node) return;
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = list->head;
    if (list->head) {
        list->head->prev = new_node;
    } else {
        list->tail = new_node;
    }
    list->head = new_node;
}

/**
 * Inserts a new node with the given data at the end of the list.
 *
 * list: Pointer to the DoublyLinkedList.
 * data: Integer value to insert.
 */
void dll_insert_at_end(DoublyLinkedList* list, int data) {
    DNode* new_node = (DNode*)malloc(sizeof(DNode));
    if (!new_node) return;
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = list->tail;
    if (list->tail) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
}

/**
 * Deletes the first node with the specified data value from the list.
 *
 * list: Pointer to the DoublyLinkedList.
 * data: Integer value to delete.
 *
 * Returns: true if a node was deleted, false otherwise.
 */
bool dll_delete_node(DoublyLinkedList* list, int data) {
    DNode* curr = list->head;
    while (curr) {
        if (curr->data == data) {
            if (curr->prev) curr->prev->next = curr->next;
            else list->head = curr->next;
            if (curr->next) curr->next->prev = curr->prev;
            else list->tail = curr->prev;
            free(curr);
            return true;
        }
        curr = curr->next;
    }
    return false;
}

/**
 * Prints the list from head to tail.
 *
 * list: Pointer to the DoublyLinkedList.
 */
void dll_print_forward(const DoublyLinkedList* list) {
    printf("List (forward): ");
    DNode* curr = list->head;
    while (curr) {
        printf("%d <-> ", curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

/**
 * Prints the list from tail to head.
 *
 * list: Pointer to the DoublyLinkedList.
 */
void dll_print_backward(const DoublyLinkedList* list) {
    printf("List (backward): ");
    DNode* curr = list->tail;
    while (curr) {
        printf("%d <-> ", curr->data);
        curr = curr->prev;
    }
    printf("NULL\n");
}

/**
 * Frees all nodes in the list and the list struct itself.
 *
 * list: Pointer to the DoublyLinkedList to destroy.
 */
void dll_destroy_list(DoublyLinkedList* list) {
    DNode* curr = list->head;
    while (curr) {
        DNode* next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

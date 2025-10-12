/*
gcc -g main.c singly_linked_list.c queue.c stack.c binary_search_tree.c doubly_linked_list.c -o main && ./main
*/
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "singly_linked_list.h"
#include "stack.h"
#include "queue.h"
#include "doubly_linked_list.h"
#include "binary_search_tree.h"
#include "avl_tree.h"


// This works only for arrays whose size is known at compile time (i.e., not pointers).
// compile-time trick to ensure arr is not a pointer
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
// Helper for singly linked list insertion sort
Node* insertion_sort_list(Node* head) {
    if (!head || !head->next) return head;
    Node* sorted = NULL;
    Node* curr = head;
    while (curr) {
        Node* next = curr->next;
        if (!sorted || curr->data < sorted->data) {
            curr->next = sorted;
            sorted = curr;
        } else {
            Node* s = sorted;
            while (s->next && s->next->data < curr->data) s = s->next;
            curr->next = s->next;
            s->next = curr;
        }
        curr = next;
    }
    return sorted;
}

// Helper for doubly linked list insertion sort
DNode* insertion_sort_dlist(DNode* head) {
    if (!head) return NULL;
    DNode* curr = head->next;
    while (curr) {
        DNode* next = curr->next;
        DNode* pos = curr->prev;
        while (pos && pos->data > curr->data) pos = pos->prev;
        if ((curr->prev && pos != curr->prev) || (!curr->prev && pos)) {
            // Remove curr
            if (curr->prev) curr->prev->next = curr->next;
            if (curr->next) curr->next->prev = curr->prev;
            // Insert after pos
            if (!pos) {
                // Insert at head
                curr->next = head;
                curr->prev = NULL;
                head->prev = curr;
                head = curr;
            } else {
                curr->next = pos->next;
                curr->prev = pos;
                if (pos->next) pos->next->prev = curr;
                pos->next = curr;
            }
        }
        curr = next;
    }
    return head;
}
// Function prototype for print_array
void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
// Bubble Sort implementation with verbosity and comments
// arr: the array to sort
// n: number of elements in the array
// reverse: if true, sort in descending order; otherwise ascending
// verbose: if true, print array after each swap; if false, suppress inner loop printing
void bubble_sort(int arr[], int n, bool reverse, bool verbose) {
    printf("Original array: ");
    print_array(arr, n);

    // Outer loop for each pass
    for (int i = 0; i < n-1; i++) {
        // Inner loop for pairwise comparison
        for (int j = 0; j < n-i-1; j++) {
            // Compare adjacent elements and swap if out of order
            if (reverse ? arr[j] < arr[j+1] : arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
            // Print array after each swap if verbose is true
            if (verbose) {
                printf("Array after swap: ");
                print_array(arr, n);
                printf("\n");
            }
        }
    }
    printf("Sorted array : ");
    print_array(arr, n);
    printf("\n");
}
// Insertion Sort implementation
void insertion_sort(int arr[], int n, bool reverse, bool verbose) {
    printf("--- Insertion Sort Demonstration (Array) ---\n");
    printf("Original array : ");
    print_array(arr, n);
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && (reverse ? arr[j] < key : arr[j] > key)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
        // Print array after each swap if verbose is true
        if (verbose) {
            printf("Array after swap: ");
            print_array(arr, n);
            printf("\n");
        }
    }
    printf("Sorted array : ");
    print_array(arr, n);
    printf("\n");
}



void demonstrate_singly_linked_list() {
    printf("--- Singly Linked List Demonstration ---\n");
    LinkedList* list = init_list();
    if (list == NULL) {
        printf("Failed to initialize linked list. Exiting.\n");
        return;
    }
    insert_at_end(list, 10);
    insert_at_end(list, 20);
    insert_at_end(list, 30);
    printf("After inserting 10, 20, 30 at end:\n");
    print_list(list);
    insert_at_beginning(list, 5);
    printf("After inserting 5 at beginning:\n");
    print_list(list);
    printf("Deleting node with data 20...\n");
    if (delete_node(list, 20)) printf("Node deleted successfully.\n");
    else printf("Node not found.\n");
    print_list(list);
    printf("Deleting node with data 5 (head)...\n");
    if (delete_node(list, 5)) printf("Node deleted successfully.\n");
    else printf("Node not found.\n");
    print_list(list);
    printf("Attempting to delete non-existent node 99...\n");
    if (delete_node(list, 99)) printf("Node deleted successfully.\n");
    else printf("Node not found.\n");
    print_list(list);
    destroy_list(list);
    printf("Linked list destroyed.\n\n");

        // Insertion Sort for Singly Linked List
    printf("--- Insertion Sort Demonstration (Singly Linked List) ---\n");
    LinkedList* llist = init_list();
    insert_at_end(llist, 29);
    insert_at_end(llist, 10);
    insert_at_end(llist, 14);
    insert_at_end(llist, 37);
    insert_at_end(llist, 13);
    insert_at_end(llist, 5);
    insert_at_end(llist, 7);
    insert_at_end(llist, 18);
    insert_at_end(llist, 2);
    insert_at_end(llist, 11);
    printf("Original list: ");
    print_list(llist);
    llist->head = insertion_sort_list(llist->head);
    printf("Sorted list : ");
    print_list(llist);
    destroy_list(llist);
    printf("\n");
}

void demonstrate_doubly_linked_list() {
    printf("--- Doubly Linked List Demonstration ---\n");
    DoublyLinkedList* dlist = dll_init_list();
    if (dlist == NULL) {
        printf("Failed to initialize doubly linked list. Exiting.\n");
        return;
    }
    dll_insert_at_end(dlist, 100);
    dll_insert_at_end(dlist, 200);
    dll_insert_at_end(dlist, 300);
    printf("After inserting 100, 200, 300 at end:\n");
    dll_print_forward(dlist);
    dll_print_backward(dlist);
    dll_insert_at_beginning(dlist, 50);
    printf("After inserting 50 at beginning:\n");
    dll_print_forward(dlist);
    dll_print_backward(dlist);
    printf("Deleting node with data 200...\n");
    if (dll_delete_node(dlist, 200)) printf("Node deleted successfully.\n");
    else printf("Node not found.\n");
    dll_print_forward(dlist);
    dll_print_backward(dlist);
    printf("Deleting node with data 50 (head)...\n");
    if (dll_delete_node(dlist, 50)) printf("Node deleted successfully.\n");
    else printf("Node not found.\n");
    dll_print_forward(dlist);
    dll_print_backward(dlist);
    printf("Attempting to delete non-existent node 999...\n");
    if (dll_delete_node(dlist, 999)) printf("Node deleted successfully.\n");
    else printf("Node not found.\n");
    dll_print_forward(dlist);
    dll_print_backward(dlist);
    dll_destroy_list(dlist);
    printf("Doubly linked list destroyed.\n\n");

        // Insertion Sort for Doubly Linked List
    printf("--- Insertion Sort Demonstration (Doubly Linked List) ---\n");
    DoublyLinkedList* ddlist = dll_init_list();
    dll_insert_at_end(ddlist, 29);
    dll_insert_at_end(ddlist, 10);
    dll_insert_at_end(ddlist, 14);
    dll_insert_at_end(ddlist, 37);
    dll_insert_at_end(ddlist, 13);
    dll_insert_at_end(ddlist, 5);
    dll_insert_at_end(ddlist, 7);
    dll_insert_at_end(ddlist, 18);
    dll_insert_at_end(ddlist, 2);
    dll_insert_at_end(ddlist, 11);
    printf("Original doubly list: ");
    dll_print_forward(ddlist);
    ddlist->head = insertion_sort_dlist(ddlist->head);
    printf("Sorted doubly list : ");
    dll_print_forward(ddlist);
    dll_destroy_list(ddlist);
    printf("\n");
}

void demonstrate_stack() {
    printf("--- Stack Demonstration (LIFO) ---\n");
    Stack stack;
    init_stack(&stack);
    int value;
    printf("Pushing 10, 20, 30 onto the stack.\n");
    push(&stack, 10);
    push(&stack, 20);
    push(&stack, 30);
    if (peek(&stack, &value)) printf("Top element is: %d\n", value);
    if (pop(&stack, &value)) printf("Popped value: %d\n", value);
    if (peek(&stack, &value)) printf("Top element is now: %d\n", value);
    if (pop(&stack, &value)) printf("Popped value: %d\n", value);
    if (pop(&stack, &value)) printf("Popped value: %d\n", value);
    if (pop(&stack, &value) == false) printf("Successfully prevented stack underflow.\n\n");
}

void demonstrate_queue() {
    printf("--- Queue Demonstration (FIFO) ---\n");
    Queue queue;
    init_queue(&queue);
    int value;
    printf("Enqueuing 100, 200, 300 to the queue.\n");
    enqueue(&queue, 100);
    enqueue(&queue, 200);
    enqueue(&queue, 300);
    if (dequeue(&queue, &value)) printf("Dequeued value: %d\n", value);
    if (dequeue(&queue, &value)) printf("Dequeued value: %d\n", value);
    if (dequeue(&queue, &value)) printf("Dequeued value: %d\n", value);
    if (dequeue(&queue, &value) == false) printf("Successfully prevented queue underflow!!.\n\n");
}


void demonstrate_binary_search_tree() {
    printf("--- Binary Search Tree Demonstration ---\n");
    BST* bst = init_bst();
    if (bst == NULL) {
        printf("Failed to initialize BST. Exiting.\n");
        return;
    }
    insert(bst, 50);
    insert(bst, 30);
    insert(bst, 70);
    insert(bst, 20);
    insert(bst, 40);
    insert(bst, 60);
    insert(bst, 80);
    printf("Inorder traversal (should be sorted): ");
    inorder_traversal(bst->root);
    printf("\n");
    printf("Preorder traversal: ");
    preorder_traversal(bst->root);
    printf("\n");
    printf("Postorder traversal: ");
    postorder_traversal(bst->root);
    printf("\n");
    int search_val = 40;
    TNode* found = search(bst, search_val);
    if (found) printf("Found %d in BST.\n", search_val);
    else printf("%d not found in BST.\n", search_val);
    destroy_tree(bst);
    printf("BST destroyed.\n\n");
}


void demonstrate_sorts() {
    int arr1[10] = {29, 10, 14, 37, 13, 5, 7, 18, 2, 11};
    int n1 = ARRAY_SIZE(arr1);
    insertion_sort(arr1, n1, false, false); // Pass true for verbose demonstration

    insertion_sort(arr1, n1, true, false); // Pass true for verbose demonstration
    
    printf("--- Bubble Sort Demonstration (Array) ---\n");
    int arr2[10] = {291, 10, 14, 37, 13, 5, 7, 18, 2, 11};
    int n2 = ARRAY_SIZE(arr2);
    bubble_sort(arr2, n2, false, false); // Pass true for verbose demonstration
    }

void demonstrate_avl_tree() {
    printf("--- AVL Tree Demonstration (self-balancing) ---\n");
    AVLTree* tree = avl_init();
    if (tree == NULL) {
        printf("Failed to initialize AVL tree. Exiting.\n");
        return;
    }
    avl_insert(tree, 50);
    avl_insert(tree, 30);
    avl_insert(tree, 70);
    avl_insert(tree, 20);
    avl_insert(tree, 40);
    avl_insert(tree, 60);
    avl_insert(tree, 80);
    printf("Inorder traversal (should be sorted): ");
    avl_inorder_traversal(tree->root);
    printf("\n");
    printf("Preorder traversal: ");
    avl_preorder_traversal(tree->root);
    printf("\n");
    printf("Postorder traversal: ");
    avl_postorder_traversal(tree->root);
    printf("\n");
    int search_val = 40;
    AVLNode* found = avl_search(tree, search_val);
    if (found) printf("Found %d in AVL tree.\n", search_val);
    else printf("%d not found in AVL tree.\n", search_val);
    avl_destroy_tree(tree);
    printf("AVL tree destroyed.\n\n");
}


int main() {
    demonstrate_singly_linked_list();
    demonstrate_doubly_linked_list();
    demonstrate_stack();
    demonstrate_queue();
    demonstrate_binary_search_tree();
    demonstrate_avl_tree();
    demonstrate_sorts();
    return 0;
}


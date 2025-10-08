/*
gcc -g main.c singly_linked_list.c queue.c stack.c binary_search_tree.c doubly_linked_list.c -o main && ./main
*/
#include <stdio.h>

#include "singly_linked_list.h"
#include "stack.h"
#include "queue.h"
#include "doubly_linked_list.h"
#include "binary_search_tree.h"
#include "avl_tree.h"

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
    return 0;
}


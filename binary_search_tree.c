#include <stdio.h>
#include <stdlib.h>
#include "binary_search_tree.h"

// Helper function to create a new node.
static TNode* create_node(int data) {
    TNode* newNode = (TNode*)malloc(sizeof(TNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node.\n");
        return NULL;
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Helper function to insert a node recursively.
static TNode* insert_recursive(TNode* node, int data) {
    // Base case: If the tree is empty or we've found an empty spot, create and return a new node.
    if (node == NULL) {
        return create_node(data);
    }

    // Recursive case: Traverse the tree.
    if (data < node->data) {
        // If the new data is less, go to the left subtree.
        node->left = insert_recursive(node->left, data);
    } else if (data > node->data) {
        // If the new data is greater, go to the right subtree.
        node->right = insert_recursive(node->right, data);
    }
    // If data is equal, do nothing (we don't allow duplicates in this implementation).
    return node;
}

// Initializes a new, empty Binary Search Tree.
BST* init_bst() {
    BST* bst = (BST*)malloc(sizeof(BST));
    if (bst == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for BST.\n");
        return NULL;
    }
    bst->root = NULL;
    return bst;
}

// Inserts a new value into the BST.
void insert(BST* bst, int data) {
    if (bst == NULL) {
        return;
    }
    bst->root = insert_recursive(bst->root, data);
}

// Helper function to search for a node recursively.
static TNode* search_recursive(TNode* node, int data) {
    // Base case 1: Node is NULL, meaning the value was not found.
    if (node == NULL) {
        return NULL;
    }

    // Base case 2: The value is found.
    if (node->data == data) {
        return node;
    }

    // Recursive case: Traverse based on data value.
    if (data < node->data) {
        return search_recursive(node->left, data);
    } else {
        return search_recursive(node->right, data);
    }
}

// Searches for a value in the BST.
TNode* search(const BST* bst, int data) {
    if (bst == NULL) {
        return NULL;
    }
    return search_recursive(bst->root, data);
}

// Performs an In-order traversal (Left, Root, Right).
void inorder_traversal(TNode* node) {
    if (node != NULL) {
        inorder_traversal(node->left);
        printf("%d ", node->data);
        inorder_traversal(node->right);
    }
}

// Performs a Pre-order traversal (Root, Left, Right).
void preorder_traversal(TNode* node) {
    if (node != NULL) {
        printf("%d ", node->data);
        preorder_traversal(node->left);
        preorder_traversal(node->right);
    }
}

// Performs a Post-order traversal (Left, Right, Root).
void postorder_traversal(TNode* node) {
    if (node != NULL) {
        postorder_traversal(node->left);
        postorder_traversal(node->right);
        printf("%d ", node->data);
    }
}

// Helper function to destroy the tree recursively using post-order traversal.
static void destroy_tree_recursive(TNode* node) {
    if (node != NULL) {
        destroy_tree_recursive(node->left);
        destroy_tree_recursive(node->right);
        free(node);
    }
}

// Deallocates all memory used by the BST.
void destroy_tree(BST* bst) {
    if (bst == NULL) return;
    destroy_tree_recursive(bst->root);
    free(bst);
}


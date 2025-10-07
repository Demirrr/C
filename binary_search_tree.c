#include <stdio.h>
#include <stdlib.h>
#include "binary_search_tree.h"

// Create a new node with given data and initialize height
static TNode* create_node(int data) {
    TNode* newNode = (TNode*)malloc(sizeof(TNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node.\n");
        return NULL;
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // new node is initially a leaf
    return newNode;
}

// Helper to get height of a node (0 for NULL)
static int node_height(TNode* node) {
    return node ? node->height : 0;
}

// Update height based on children's heights
static void update_height(TNode* node) {
    if (node) {
        int lh = node_height(node->left);
        int rh = node_height(node->right);
        node->height = (lh > rh ? lh : rh) + 1;
    }
}

// Get balance factor = height(left) - height(right)
static int get_balance(TNode* node) {
    if (node == NULL) return 0;
    return node_height(node->left) - node_height(node->right);
}

// Right rotate
static TNode* rotate_right(TNode* y) {
    TNode* x = y->left;
    TNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    update_height(y);
    update_height(x);

    // Return new root
    return x;
}

// Left rotate
static TNode* rotate_left(TNode* x) {
    TNode* y = x->right;
    TNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    update_height(x);
    update_height(y);

    // Return new root
    return y;
}

// AVL insert
static TNode* insert_recursive(TNode* node, int data) {
    // Standard BST insert
    if (node == NULL)
        return create_node(data);

    if (data < node->data)
        node->left = insert_recursive(node->left, data);
    else if (data > node->data)
        node->right = insert_recursive(node->right, data);
    else // duplicates not allowed
        return node;

    // Update height of this ancestor node
    update_height(node);

    // Get balance factor to check whether this node became unbalanced
    int balance = get_balance(node);

    // Left Left Case
    if (balance > 1 && data < node->left->data)
        return rotate_right(node);

    // Right Right Case
    if (balance < -1 && data > node->right->data)
        return rotate_left(node);

    // Left Right Case
    if (balance > 1 && data > node->left->data) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // Right Left Case
    if (balance < -1 && data < node->right->data) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    // return the (unchanged) node pointer
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

// Deletes a subtree given its root node.
void delete_subtree(TNode* node) {
    destroy_tree_recursive(node);
}

// Deallocates all memory used by the BST.
void destroy_tree(BST* bst) {
    if (bst == NULL) return;
    destroy_tree_recursive(bst->root);
    free(bst);
}


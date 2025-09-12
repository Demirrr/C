#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdbool.h>

// Define the structure for a single node in the BST.
// Each node has an integer data value and pointers to its left and right children.
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// Define the structure for the Binary Search Tree.
// It simply holds a pointer to the root node of the tree.
typedef struct BST {
    Node* root;
} BST;

// Function prototypes for BST operations.

/**
 * @brief Initializes a new, empty Binary Search Tree.
 * @return A pointer to the newly created BST.
 */
BST* init_bst();

/**
 * @brief Inserts a new value into the Binary Search Tree.
 * The function ensures that the BST property is maintained (left child is smaller, right child is larger).
 * @param bst The BST to insert into.
 * @param data The integer data to be inserted.
 */
void insert(BST* bst, int data);

/**
 * @brief Searches for a value in the Binary Search Tree.
 * The function leverages the BST property to efficiently find the value.
 * @param bst The BST to search in.
 * @param data The integer data to search for.
 * @return A pointer to the node containing the data, or NULL if not found.
 */
Node* search(const BST* bst, int data);

/**
 * @brief Performs an In-order traversal of the tree.
 * The output will be the data in ascending sorted order.
 * @param node The current node to start the traversal from. Use the root node to traverse the whole tree.
 */
void inorder_traversal(Node* node);

/**
 * @brief Performs a Pre-order traversal of the tree.
 * The output is root -> left -> right.
 * @param node The current node to start the traversal from. Use the root node to traverse the whole tree.
 */
void preorder_traversal(Node* node);

/**
 * @brief Performs a Post-order traversal of the tree.
 * The output is left -> right -> root.
 * @param node The current node to start the traversal from. Use the root node to traverse the whole tree.
 */
void postorder_traversal(Node* node);

/**
 * @brief Deallocates all memory used by the Binary Search Tree.
 * @param bst The BST to destroy.
 */
void destroy_tree(BST* bst);

#endif // BINARY_SEARCH_TREE_H


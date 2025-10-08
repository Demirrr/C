
#include <stdio.h>
#include <stdlib.h>
#include "binary_search_tree.h"

// Simple (non-AVL) BST implementation using TNode/BST
static TNode* create_node(int data) {
    TNode* newNode = (TNode*)malloc(sizeof(TNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node.\n");
        return NULL;
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // height field unused in simple BST but kept for compatibility
    return newNode;
}

static TNode* insert_recursive(TNode* node, int data) {
    if (node == NULL) return create_node(data);
    if (data < node->data)
        node->left = insert_recursive(node->left, data);
    else if (data > node->data)
        node->right = insert_recursive(node->right, data);
    // duplicates ignored
    return node;
}

BST* init_bst() {
    BST* bst = (BST*)malloc(sizeof(BST));
    if (bst == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for BST.\n");
        return NULL;
    }
    bst->root = NULL;
    return bst;
}

void insert(BST* bst, int data) {
    if (bst == NULL) return;
    bst->root = insert_recursive(bst->root, data);
}

static TNode* search_recursive(TNode* node, int data) {
    if (node == NULL) return NULL;
    if (node->data == data) return node;
    if (data < node->data) return search_recursive(node->left, data);
    return search_recursive(node->right, data);
}

TNode* search(const BST* bst, int data) {
    if (bst == NULL) return NULL;
    return search_recursive(bst->root, data);
}

void inorder_traversal(TNode* node) {
    if (node != NULL) {
        inorder_traversal(node->left);
        printf("%d ", node->data);
        inorder_traversal(node->right);
    }
}

void preorder_traversal(TNode* node) {
    if (node != NULL) {
        printf("%d ", node->data);
        preorder_traversal(node->left);
        preorder_traversal(node->right);
    }
}

void postorder_traversal(TNode* node) {
    if (node != NULL) {
        postorder_traversal(node->left);
        postorder_traversal(node->right);
        printf("%d ", node->data);
    }
}

static void destroy_tree_recursive(TNode* node) {
    if (node != NULL) {
        destroy_tree_recursive(node->left);
        destroy_tree_recursive(node->right);
        free(node);
    }
}

void delete_subtree(TNode* node) {
    destroy_tree_recursive(node);
}

void destroy_tree(BST* bst) {
    if (bst == NULL) return;
    destroy_tree_recursive(bst->root);
    free(bst);
}


#include <stdio.h>
#include <stdlib.h>
#include "avl_tree.h"

static AVLNode* create_node(int data) {
    AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for new node.\n");
        return NULL;
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

static int node_height(AVLNode* node) {
    return node ? node->height : 0;
}

static void update_height(AVLNode* node) {
    if (node) {
        int lh = node_height(node->left);
        int rh = node_height(node->right);
        node->height = (lh > rh ? lh : rh) + 1;
    }
}

static int get_balance(AVLNode* node) {
    if (node == NULL) return 0;
    return node_height(node->left) - node_height(node->right);
}

static AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    update_height(y);
    update_height(x);
    return x;
}

static AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    update_height(x);
    update_height(y);
    return y;
}

static AVLNode* insert_recursive(AVLNode* node, int data) {
    if (node == NULL)
        return create_node(data);

    if (data < node->data)
        node->left = insert_recursive(node->left, data);
    else if (data > node->data)
        node->right = insert_recursive(node->right, data);
    else
        return node;

    update_height(node);
    int balance = get_balance(node);

    if (balance > 1 && data < node->left->data)
        return rotate_right(node);
    if (balance < -1 && data > node->right->data)
        return rotate_left(node);
    if (balance > 1 && data > node->left->data) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && data < node->right->data) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    return node;
}

AVLTree* avl_init() {
    AVLTree* tree = (AVLTree*)malloc(sizeof(AVLTree));
    if (tree == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for AVL tree.\n");
        return NULL;
    }
    tree->root = NULL;
    return tree;
}

void avl_insert(AVLTree* tree, int data) {
    if (tree == NULL) return;
    tree->root = insert_recursive(tree->root, data);
}

static AVLNode* search_recursive(AVLNode* node, int data) {
    if (node == NULL) return NULL;
    if (node->data == data) return node;
    if (data < node->data) return search_recursive(node->left, data);
    return search_recursive(node->right, data);
}

AVLNode* avl_search(const AVLTree* tree, int data) {
    if (tree == NULL) return NULL;
    return search_recursive(tree->root, data);
}

void avl_inorder_traversal(AVLNode* node) {
    if (node != NULL) {
        avl_inorder_traversal(node->left);
        printf("%d ", node->data);
        avl_inorder_traversal(node->right);
    }
}

void avl_preorder_traversal(AVLNode* node) {
    if (node != NULL) {
        printf("%d ", node->data);
        avl_preorder_traversal(node->left);
        avl_preorder_traversal(node->right);
    }
}

void avl_postorder_traversal(AVLNode* node) {
    if (node != NULL) {
        avl_postorder_traversal(node->left);
        avl_postorder_traversal(node->right);
        printf("%d ", node->data);
    }
}

static void destroy_tree_recursive(AVLNode* node) {
    if (node != NULL) {
        destroy_tree_recursive(node->left);
        destroy_tree_recursive(node->right);
        free(node);
    }
}

void avl_delete_subtree(AVLNode* node) {
    destroy_tree_recursive(node);
}

void avl_destroy_tree(AVLTree* tree) {
    if (tree == NULL) return;
    destroy_tree_recursive(tree->root);
    free(tree);
}

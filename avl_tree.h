#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdbool.h>

// AVL node with height for balancing
typedef struct AVLNode {
    int data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

typedef struct AVLTree {
    AVLNode* root;
} AVLTree;

// AVL function prototypes
AVLTree* avl_init();
void avl_insert(AVLTree* tree, int data);
AVLNode* avl_search(const AVLTree* tree, int data);
void avl_inorder_traversal(AVLNode* node);
void avl_preorder_traversal(AVLNode* node);
void avl_postorder_traversal(AVLNode* node);
void avl_destroy_tree(AVLTree* tree);
void avl_delete_subtree(AVLNode* node);

#endif // AVL_TREE_H

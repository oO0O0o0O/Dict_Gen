/*
Contains methods and resources for implementing a binary search tree.
*/

#ifndef bst
#define bst

/*
BST node
*/
typedef struct node {
    char* string;
    int changes;
    int locked;
    struct node* left;
    struct node* right;
} node;

/*
Stores config information for use when traversing. Refer to README.md for details.
    cc_mode: 0 (lower)(default), 1 (camel), 2 (upper), 3 (lite), 4 (full)
    s_mode: 0 (none), 1 (common)(default), 2 (shuffle), 3 (all)
    c_length: >=1 (default 2)
    c_depth: 0 (common)(default), 1 (most), 2 (all)
    a_length: >=0 (default 3)
    a_mode: 0 (common)(default), 1 (special), 2 (all)
    a_type: 0 (append)(default), 1 (prepend), 2 (both)
*/
typedef struct config {
    int cc_mode;
    int s_mode;
    int c_length;
    int c_depth;
    int a_length;
    int a_mode;
    int a_type;
} config;

/*
Insert a value into the bst. Return pointer to new/existing node.
Does not insert duplicate data.
*/
node* insert(node *head, char *string, int changes);

/*
Iterates through the BST via in-order traversal, and calls a provided function
on each node.
*/
int traverse(node *head, node *root, config *cfg, int override, int f(node*, node*, config*));

/*
Finds the size of the BST.
*/
int count(node *head);

/*
Resets all new flags to false.
*/
node* refresh(node *head);

#endif
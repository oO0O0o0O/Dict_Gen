/*
Contains methods and resources for implementing a binary search tree.
*/

#include <string.h>
#include <stdlib.h>

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
    s_mode: 0 (none), 1 (common)(default), 2 (shuffle)(default), 3 (all)
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
Insert a value into the bst. Return pointer root.
Does not insert duplicate data.
*/
node* insert(node *head, char *string, int changes) {
    // Check end condition
    if (head == NULL) {
        head = malloc(sizeof(node));
        head->changes = changes;
        head->string = malloc(sizeof(char)*(strlen(string) + 1));
        strcpy(head->string, string);
        head->left = NULL;
        head->right = NULL;
        head->locked = 1;
        return head;
    }

    // Compare changes
    if (changes > head->changes) {
        head->right = insert(head->right, string, changes);
    } else if (changes < head->changes) {
        head->left = insert(head->left, string, changes);
    } else {
        // Compare strings
        int cmp = strcmp(string, head->string);
        
        if (cmp > 0) {
            head->right = insert(head->right, string, changes);
        } else if (cmp < 0) {
            head->left = insert(head->left, string, changes);
        }
    }

    return head;
}

/*
Iterates through the BST via in-order traversal, and calls a provided function
on each node.
*/
int traverse(node *head, node *root, config* cfg, int override, int f(node*, node*, config*)) {
    int status = 0;
    
    // Check end condition
    if (head == NULL || (override == 0 && head->locked == 1)) {
        return 0;
    }

    // In-order traversal
    status = status || traverse(head->left, root, cfg, override, f);
    status = status || f(head, root, cfg);
    status = status || traverse(head->right, root, cfg, override, f);

    return status;
}

/*
Finds the size of the BST.
*/
int count(node *head) {
    // Check end condition
    if (head == NULL) return 0;

    return 1 + count(head->left) + count(head->right);
}

/*
Recursive helper for refresh.
*/
int refresh_helper(node *head, node ***arr, int *i, int *size) {
    // Check end condition
    if (head == NULL) {
        return 0;
    }

    // Recurse left
    refresh_helper(head->left, arr, i, size);
    
    // Unlock
    head->locked = 0;

    // Add to temp array
    if (*i >= *size) {
        *arr = realloc(*arr, 2 * (*size) * sizeof(node *));
        *size *= 2;
    }

    *(*arr + *i) = head;
    (*i)++;

    // Recurse right
    refresh_helper(head->right, arr, i, size);

    return 0;
}

/*
Constructs a balanced BST from a sorted array.
*/
node* build_bst(node **arr, int size) {
    // End case
    if (size == 0) return NULL;

    // Find values
    node *middle = *(arr + ((size - 1) / 2));
    int l_size = (size - 0.5) / 2;
    int r_size = size / 2;

    // Clean update values and recurse
    middle->left = build_bst(arr, l_size);
    middle->right = build_bst(arr + ((size - 1) / 2) + 1, r_size);

    return middle;
}

/*
Unlocks all nodes and balances the tree.
*/
node* refresh(node *head) {
    // Initialize array
    node **arr = malloc(sizeof(node *));
    int i = 0;
    int size = 1;

    // Unlock and build array
    refresh_helper(head, &arr, &i, &size);
    size = i;

    // Rebuild BST
    head = build_bst(arr, size);

    // Free and return
    free(arr);
    return head;
}
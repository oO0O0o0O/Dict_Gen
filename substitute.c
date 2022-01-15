/*
Implements the substitute module.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bst.h"

/*
Stores a substitution as a linked list node.
*/
typedef struct sub_pair {
    char in;
    char out;
    struct sub_pair *next;
} sub_pair;

static sub_pair *sub_pairs;

/*
Loads the subs file to the sub_pairs linked list.
*/
int load_pairs(config *cfg) {
    // Check if alreadly loaded
    if (sub_pairs) return 0;

    FILE *sub_file;

    // Check cfg for file
    if (cfg->s_mode == 3) {
        // Open long file
        sub_file = fopen("subs_all.txt", "r");
    } else {
        // Open short file
        sub_file = fopen("subs_common.txt", "r");
    }

    sub_pairs = malloc(sizeof(sub_pair));

    // Parse file
    sub_pair *cur = sub_pairs;
    sub_pair *prev = cur;
    char buff[3];
    while (fscanf(sub_file, "%2s", buff) != EOF) {
        cur->in = buff[0];
        cur->out = buff[1];
        cur->next = malloc(sizeof(sub_pair));
        prev = cur;
        cur = cur->next;
    }

    free(prev->next);
    prev->next = NULL;

    return 0;
}

/*
Adds all combinations of substitutions for a node to the BST.
*/
int s_backtrack(node *head, node *root, char *string, int str_i, int changes) {
    // Check end case
    if (*(string + str_i) == '\0') return 0;
    
    // Recurse without sub
    s_backtrack(head, root, string, str_i + 1, changes);

    // Check for valid subs
    sub_pair *cur = sub_pairs;
    while (cur != NULL) {
        if (*(string + str_i) == cur->in) {
            // Sub and save
            *(string + str_i) = cur->out;
            insert(root, string, changes + 1);

            // Recurse with sub
            s_backtrack(head, root, string, str_i + 1, changes + 1);

            // Revert
            *(string + str_i) = cur->in;
        }

        cur = cur->next;
    }

    return 0;
}

/*
Main traversal function for substitution module.
*/
int sub_main(node *head, node *root, config *cfg) {
    // Load sub_pairs
    load_pairs(cfg);

    // Temp string
    char string[strlen(head->string) + 1];
    strcpy(string, head->string);

    // Check cfg for backtracking
    if (cfg->s_mode == 0) {
        return 0;
        
    } else if (cfg->s_mode == 1) {
        int changed = 0;

        // Iterate through string
        for (int i = 0; *(string + i) != '\0'; i++) {
            // Iterate through sub_pairs
            sub_pair *cur = sub_pairs;
            while (cur != NULL) {
                if (cur->in == *(string +i )) {
                    // Substitute character
                    string[i] = cur->out;
                    changed = 1;
                    break;
                }

                cur = cur->next;
            }
        }

        // Insert new word
        if (changed) insert(root, string, head->changes + 1);

    } else {
        // Call backtracking function
        s_backtrack(head, root, string, 0, head->changes);
    }

    return 0;
}
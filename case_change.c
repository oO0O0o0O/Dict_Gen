/*
Implements the case change module.
*/

#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Adds all combinations of substitutions for a node to the BST.
*/
int c_backtrack(node *head, node *root, char *string, int str_i, int changes) {
    // Check end case
    if (*(string + str_i) == '\0') return 0;
    
    // Recurse without case change
    c_backtrack(head, root, string, str_i + 1, changes);

    // Check if character
    if (*(string + str_i) >= 'a' && *(string + str_i) <= 'z') {
        // Change case
        *(string + str_i) -= 32;

        // Save
        insert(root, string, (head->changes) + changes);

        // Recurse with case change
        c_backtrack(head, root, string, str_i + 1, changes);

        // Revert
        *(string + str_i) += 32;
    }

    return 0;
}

/*
Main case change traversal function.
*/
int case_main(node *head, node *root, config *cfg) {
    // Change to lowercase
    for (int i = 0; *((head->string) + i) != '\0'; i++) {
        if (*((head->string) + i) >= 'A' && *((head->string) + i) <= 'Z') *((head->string) + i) += 32;
    }

    // Create temp string buffer
    char string[strlen(head->string) + 1];
    strcpy(string, head->string);

    // Check config
    if (cfg->cc_mode == 1 || cfg->cc_mode == 3) {
        // Check first letter
        if (*string >= 'a' && *string <= 'z') {
            // Capitalize
            *string -= 32;

            // Insert
            insert(root, string, (head->changes) + 1);
        }
    }

    if (cfg->cc_mode == 2 || cfg->cc_mode == 3) {
        int changed = 0;

        // Change all letters
        for (int i = 0; *(string + i) != '\0'; i++) {
            // Check character
            if (*(string + i) >= 'a' && *(string + i) <= 'z') {
                // Capitalize
                *(string + i) -= 32;

                changed = 1;
            }
        }

        // Check for changes
        if (changed) {
            // Insert
            insert(root, string, (head->changes) + 1);
        }
    }

    if (cfg->cc_mode == 4) {
        // Call backtracking function
        c_backtrack(head, root, string, 0, head->changes);
    }

    return 0;
}
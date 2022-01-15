/*
Implements the combine module.
*/

#include "bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *common_c[3] = {".", "-", "_"};
char *punctuation[32] = {"~", "`", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "-", "_", "=", 
"+", "{", "[", "}", "]", "\\", "|", ";", ":", "'", "\"", ",", "<", ".", ">", "/", "?"};

/*
Builds a combo to a string with a given delimeter.
*/
int build_combo(int i_array[], int length, node *node_array[], char *delim, char **string) {
    // Get new string length
    int s_length = length;
    for (int i = 0; i < length; i++) {
        s_length += strlen(node_array[i_array[i]]->string);
    }
    
    // Initalize string
    *string = calloc(s_length, 8);
    *(*string + s_length - 1) = '\0';
    
    // Combine strings
    int i = 0;
    for (; i < (length - 1); i++) {
        strcat(*string, node_array[i_array[i]]->string);
        strcat(*string, delim);
    }

    strcat(*string, node_array[i_array[i]]->string);
    
    return 0;
}

/*
Adds a combo to a BST.
*/
int add_combo(int i_array[], int length, node *node_array[], config *cfg, node *head) {
    // Avoid single word combos
    if (length < 2) return 0;

    char *tmp = NULL;

    // Choose delimeters set
    if (cfg->c_depth == 0) {
        // Common delimeters
        for (int i = 0; i < 3; i++) {
            build_combo(i_array, length, node_array, common_c[i], &tmp);
            insert(head, tmp, head->changes + 1);
        }
    } else if (cfg->c_depth == 1) {
        // All punctuation
        for (int i = 0; i < 32; i++) {
            build_combo(i_array, length, node_array, punctuation[i], &tmp);
            insert(head, tmp, head->changes + 1);
        }
    } else if (cfg->c_depth == 2) {
        // All characters
        char delim[] = " ";
        for (int i = 0; i < 94; i++) {
            build_combo(i_array, length, node_array, delim, &tmp);
            insert(head, tmp, head->changes + 1);
            delim[0]++;
        }
    }

    return 0;
}

/*
Generates all combinations of a range of numbers
without repeating and of length two up to a given
length. Stores combos in a linked list.
*/
int combo_gen(int array[], int size, int i, node *node_array[], config  *cfg, node *head) {
    // Check end condition
    if (i >= cfg->c_length) return 0;

    // Iterate through possible numbers for a position
    for (int j = 0; j < size; j++) {
        // Prevent repeats

        int safe = 1;
        for (int k = 0; k < i; k++) {
            if (array[k] == j) {
                safe = 0;
                break;
            }
        }

        // Add if safe
        if (safe) {
            array[i] = j;

            add_combo(array, i + 1, node_array, cfg, head);
            combo_gen(array, size, i + 1, node_array, cfg, head);
        }
    }

    return 0;
}

/*
Builds an array from a BST.
*/
int build_array(node *head, node ***arr, int *i, int *size) {
    // Check end condition
    if (head == NULL) {
        return 0;
    }

    // Recurse left
    build_array(head->left, arr, i, size);

    // Add to temp array
    if (*i >= *size) {
        *arr = realloc(*arr, 2 * (*size) * sizeof(node *));
        *size *= 2;
    }

    *(*arr + *i) = head;
    (*i)++;

    // Recurse right
    build_array(head->right, arr, i, size);

    return 0;
}

/*
Main function for combine module.
*/
int comb_main(node *head, config *cfg) {
    // Initialize array
    node **node_array = malloc(sizeof(node *));
    int i = 0;
    int size = 1;

    // Unlock and build array
    build_array(head, &node_array, &i, &size);
    size = i;
    
    // Call combo on array
    int i_array[cfg->c_length];
    combo_gen(i_array, size, 0, node_array, cfg, head);
    free(node_array);

    return 0;
}
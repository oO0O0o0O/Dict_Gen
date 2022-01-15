/*
Implements the append module.
*/

#include "bst.h"
#include <stdlib.h>
#include <string.h>

char *common_a[6] = {"123", "12345", "1", "!", "123456789", "1234567890"};
char *special[42] = {"`", "~", "1", "!", "2", "@", "3", "#", "4", "$", "5", "%",
"6", "^", "7", "&", "8", "*", "9", "(", "0", ")", "-", "_", "=", "+", "[",
"{", "]", "}", "\\", "|", ";", ":", "'", "\"", ",", "<", ".", ">", "/", "?"};
char *all[95] = {"`", "~", "1", "!", "2", "@", "3", "#", "4", "$", "5", "%",
"6", "^", "7", "&", "8", "*", "9", "(", "0", ")", "-", "_", "=", "+", "[",
"{", "]", "}", "\\", "|", ";", ":", "'", "\"", ",", "<", ".", ">", "/", "?",
"a", "A", "b", "B", "c", "C", "d", "D", "e", "E", "f", "F", "g", "G", "h", "H",
"i", "I", "j", "J", "k", "K", "l", "L", "m", "M", "n", "N", "o", "O", "p", "P",
"q", "Q", "r", "R", "s", "S", "t", "T", "u", "U", "v", "V", "w", "W", "x", "X",
"y", "Y", "z", "Z", " "};

/*
Appends a string to another string and returns the new string.
*/
char *append( char *dst, char *src) {
    // Create new string
    int length = strlen(dst) + strlen(src) + 1;
    char *string = calloc(length, 8);
    string[length - 1] = '\0';

    // Append
    strcat(string, dst);
    strcat(string, src);

    return string;
}

/*
Builds a string from an array of indexes to characters.
*/
char *build_string(int mode, int array[], int size) {
    char *string = calloc(size + 1, 8);
    char **chars = NULL;
    
    // Iterate through indexes
    for (int i = 0; i < size; i++) {
        if (mode == 1) chars = special;
        else chars = all;
        string[i] = *chars[array[i]];
    }

    return string;
}

/*
Recursively backtracks through all combinations of characters
from a set in a string of specified size.
*/
int a_backtrack(node *root, config *cfg, char *string, int indexes[], int changes, int i, int type) {
    // Check length
    if (i >= cfg->a_length) return 0;

    // Set max index from mode
    int max;
    if (cfg->a_mode == 1) max = 42;
    else max = 95;

    // Iterate through values for current position
    char *char_str = NULL;
    char *str = NULL;
    int new_indexes[cfg->a_length];
    for (int j = 0; j < max; j++) {
        // Update indexes and insert string
        indexes[i] = j;
        char_str = build_string(cfg->a_mode, indexes, i + 1);

        // Check type
        if (type == 0 || type == 2) {
            // Append/both
            str = append(string, char_str);
            insert(root, str, changes + 1);
            a_backtrack(root, cfg, string, indexes, changes + 1, i + 1, type);
            free(str);
        }
        if (type == 1 || type == 2) {
            // Prepend/both
            str = append(char_str, string);
            insert(root, str, changes + 1);
            a_backtrack(root, cfg, string, indexes, changes + 1, i + 1, type);
            free(str);
        } 
        if (type == 2) {
            // Both
            str = append(char_str, string);
            a_backtrack(root, cfg, str, new_indexes, changes + 1, 0, 0);
            free(str);
        }

        free(char_str);
    }
}

/*
Main traversal function for append module.
*/
int append_main(node *head, node *root, config *cfg) {
    // Check mode
    if (cfg->a_mode == 0 && cfg->a_length > 0) {
        // Common
        char *str = NULL;
        char *str2 = NULL;
        for (int i = 0; i < 6; i++) {
            // Check type
            if (cfg->a_type == 0 || cfg->a_type == 2) {
                // Append/both
                str = append(head->string, common_a[i]);
                insert(root, str, (head->changes + 1));
                free(str);
            }

            if (cfg->a_type == 1 || cfg->a_type == 2) {
                // Prepend/both
                str = append(common_a[i], head->string);
                insert(root, str, (head->changes + 1));
                free(str);
            }

            if (cfg->a_type == 2) {
                // Both
                for (int j = 0; j < 6; j++) {
                    str = append(common_a[j], head->string);
                    str2 = append(str, common_a[i]);
                    insert(root, str2, (head->changes + 2));
                    free(str);
                    free(str2);
                }
            }
        }
    } else {
        // Special/Iterative
        int indexes[cfg->a_length];
        a_backtrack(root, cfg, head->string, indexes, head->changes, 0, cfg->a_type);
    }

    return 0;
}
/*
Main Dic Gen source file
*/

#include "bst.h"
#include "combine.h"
#include "case_change.h"
#include "substitute.h"
#include "append.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Prints each password in a tree.
*/
int print(node *head, node *root, config *cfg) {
    printf("%s\n", head->string);
    return 0;
}

int main(int argc, char** argv) {
    // Initialize
    config cfg = {0, 1, 2, 0, 3, 0, 0};
    node *head = NULL;

    // Convert all input to lowercase
    for (int i = 1; i < argc; i++) {
        for (int j = 0; argv[i][j] != '\0'; j++) {
            if (argv[i][j] >= 'A' && argv[i][j] <= 'Z') argv[i][j] += 32;
        }
    }

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp("-c", argv[i]) == 0 || strcmp("--case-mode", argv[i]) == 0) {
            // cc_mode
            if (i >= argc - 1) return 1;
            i++;

            if (strcmp("lower", argv[i]) == 0) cfg.cc_mode = 0;
            else if (strcmp("camel", argv[i]) == 0) cfg.cc_mode = 1;
            else if (strcmp("upper", argv[i]) == 0) cfg.cc_mode = 2;
            else if (strcmp("lite", argv[i]) == 0) cfg.cc_mode = 3;
            else return 1;

        } else if (strcmp("-s", argv[i]) == 0 || strcmp("--substitution-mode", argv[i]) == 0) {
            // s_mode
            if (i >= argc - 1) return 1;
            i++;

            if (strcmp("none", argv[i]) == 0) cfg.s_mode = 0;
            else if (strcmp("common", argv[i]) == 0) cfg.s_mode = 1;
            else if (strcmp("shuffle", argv[i]) == 0) cfg.s_mode = 2;
            else if (strcmp("all", argv[i]) == 0) cfg.s_mode = 3;
            else return 1;

        } else if (strcmp("-cl", argv[i]) == 0 || strcmp("--combination-length", argv[i]) == 0) {
            // c_length
            if (i >= argc - 1) return 1;
            if (sscanf(argv[++i], "%64i", &(cfg.c_length)) <= 0) return 1;

        } else if (strcmp("-cd", argv[i]) == 0 || strcmp("--combination-depth", argv[i]) == 0) {
            // c_depth
            if (i >= argc - 1) return 1;
            i++;

            if (strcmp("common", argv[i]) == 0) cfg.c_depth = 0;
            else if (strcmp("most", argv[i]) == 0) cfg.c_depth = 1;
            else if (strcmp("all", argv[i]) == 0) cfg.c_depth = 2;
            else return 1;

        } else if (strcmp("-al", argv[i]) == 0 || strcmp("--append-length", argv[i]) == 0) {
            // a_length
            if (i >= argc - 1) return 1;
            if (sscanf(argv[++i], "%64i", &(cfg.a_length)) <= 0) return 1;

        } else if (strcmp("-am", argv[i]) == 0 || strcmp("--append-mode", argv[i]) == 0) {
            // a_mode
            if (i >= argc - 1) return 1;
            i++;

            if (strcmp("common", argv[i]) == 0) cfg.a_mode = 0;
            else if (strcmp("special", argv[i]) == 0) cfg.a_mode = 1;
            else if (strcmp("all", argv[i]) == 0) cfg.a_mode = 2;
            else return 1;

        } else if (strcmp("-at", argv[i]) == 0 || strcmp("--append-type", argv[i]) == 0) {
            // a_type
            if (i >= argc - 1) return 1;
            i++;

            if (strcmp("append", argv[i]) == 0) cfg.a_type = 0;
            else if (strcmp("prepend", argv[i]) == 0) cfg.a_type = 1;
            else if (strcmp("both", argv[i]) == 0) cfg.a_type = 2;
            else return 1;

        } else if (strcmp("-x", argv[i]) == 0 || strcmp("--extreme", argv[i]) == 0) {
            // Extreme
            cfg.cc_mode = 4;
            cfg.s_mode = 3;
            cfg.c_length = 10;
            cfg.c_depth = 2;
            cfg.a_length = 10;
            cfg.a_mode = 2;
            cfg.a_type = 2;

        } else {
            // Keyword
            head = insert(head, argv[i], 0);
        }
    }

    // Check for keywords
    if (count(head) == 0) {
        // Read from stdin
        char tmp[65];
        while (scanf("%64s", tmp) != EOF) {
            head = insert(head, tmp, 0);
        }
    }

    // Combine
    head = refresh(head);
    comb_main(head, &cfg);

    // Case change
    head = refresh(head);
    traverse(head, head, &cfg, 0, case_main);

    // Substitute
    head = refresh(head);
    traverse(head, head, &cfg, 0, sub_main);

    // Append
    head = refresh(head);
    traverse(head, head, &cfg, 0, append_main);

    // Print output
    traverse(head, head, &cfg, 1, print);
    fprintf(stderr, "Count: %i\n", count(head));

    return 0;
}
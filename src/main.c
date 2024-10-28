#include "decoder.h"
#include "dict.h"
#include "dyn-string.h"
#include "encoder.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID_COMMAND                                                                            \
    do                                                                                             \
    {                                                                                              \
        fprintf(stderr, "Usage: %s <encode|decode> <input file> <output file>\n", argv[0]);        \
        return EXIT_FAILURE;                                                                       \
    }                                                                                              \
    while (1)

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        INVALID_COMMAND;
    }

    if (strcmp(argv[1], "encode") == 0)
    {
        FILE *in = fopen(argv[2], "r");
        if (in == NULL)
        {
            perror("failed to open input file");
            return EXIT_FAILURE;
        }

        FILE *out = fopen(argv[3], "w");
        if (out == NULL)
        {
            perror("failed to open output file");
            return EXIT_FAILURE;
        }

        struct TreeNodeList *list = node_list_from_file(in);

        // We'll reuse the FILE, so we need to rewind it.
        rewind(in);

        struct TreeNode *root = build_tree_root(list);
        struct DictEntry *dict = tree_to_dict(root);

        write_header(out, root);
        write_body(out, in, dict);

        free_node_list(list);
        free_node(root);
        free(dict);

        fclose(in);
        fclose(out);
    }
    else if (strcmp(argv[1], "decode") == 0)
    {
        FILE *in = fopen(argv[2], "r");
        if (in == NULL)
        {
            perror("failed to open input file");
            return EXIT_FAILURE;
        }

        FILE *out = fopen(argv[3], "w");
        if (out == NULL)
        {
            perror("failed to open output file");
            return EXIT_FAILURE;
        }

        struct TreeNode *root = read_header(in);
        struct DynString *decoded = read_body(in, root);

        fwrite(decoded->data, sizeof(char), decoded->length, out);

        free_node(root);
        free(decoded);

        fclose(in);
        fclose(out);
    }
    else
    {
        INVALID_COMMAND;
    }

    return EXIT_SUCCESS;
}

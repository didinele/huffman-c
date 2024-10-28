#include "decoder.h"
#include "dict.h"
#include "encoder.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

static inline void encode()
{
    FILE *in = fopen("/Users/didinele/Documents/Work/didinele/huffman-c/in.txt", "r");
    FILE *out = fopen("/Users/didinele/Documents/Work/didinele/huffman-c/out.bin", "w");

    struct TreeNodeList *list = node_list_from_file(in);

    // We'll reuse the file pointer, so we need to rewind it.
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

static inline void decode()
{
    FILE *bin = fopen("/Users/didinele/Documents/Work/didinele/huffman-c/out.bin", "r");

    struct TreeNode* root = read_header(bin);
    char *decoded = read_body(bin, root);

    printf("%s\n", decoded);

    free_node(root);
    free(decoded);
    fclose(bin);
}

int main()
{
    encode();
    decode();

    return 0;
}

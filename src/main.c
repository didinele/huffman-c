#include "dict.h"
#include "encoder.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *in = fopen("/Users/didinele/Documents/Work/didinele/huffman-c/in.txt", "r");
    FILE *out = fopen("/Users/didinele/Documents/Work/didinele/huffman-c/out.bin", "w");

    struct TreeNodeList *list = node_list_from_file(in);

    // We'll reuse the file pointer, so we need to rewind it.
    rewind(in);

    // build_tree_root does not keep the list intact, we need to preserve the length of the list
    // before this function call.
    int leaf_node_count = list->length;
    struct TreeNode *root = build_tree_root(list);

    struct Tree *tree = malloc(sizeof(struct Tree));
    // "A finished tree has up to n leaf nodes and n - 1 internal nodes."
    init_tree(tree, root, 2 * leaf_node_count - 1);

    struct DictEntry *dict = tree_to_dict(tree);

    write_header(out, tree);
    write_body(out, in, dict);

    free_node_list(list);
    free_tree(tree);
    free(dict);

    fclose(in);
    fclose(out);

    return 0;
}

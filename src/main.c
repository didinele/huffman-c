#include <stdio.h>
#include "tree.h"

int main()
{
    FILE *file = fopen("/Users/didinele/Documents/Work/didinele/huffman-c/dummy.txt", "r");

    struct NodeList *list = node_list_from_file(file);
    struct Node *root = build_tree(list);

    free_node_list(list);
    free_node(root);
    fclose(file);
    return 0;
}

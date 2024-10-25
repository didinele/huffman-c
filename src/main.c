#include "dict.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *file = fopen("/Users/didinele/Documents/Work/didinele/huffman-c/dummy.txt", "r");

    struct NodeList *list = node_list_from_file(file);
    struct Node *root = build_tree(list);
    printf("%i\n", root->weight);
    struct DictEntry *dict = tree_to_dict(root);

    for (int i = 0; i < 256; i++)
    {
        if (dict[i].length == 0)
        {
            continue;
        }
        
        printf("%c: ", (char)i);

        for (int j = 0; j < dict[i].length; j++)
        {
            printf("%i ", dict[i].code[j]);
        }

        printf("\n");
    }

    free_node_list(list);
    free_node(root);
    fclose(file);
    free(dict);

    return 0;
}

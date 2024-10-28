#include "dict.h"
#include "encoder.h"
#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static inline struct TreeNode *read_tree_node(struct TreeNode *root, FILE *bin)
{
    int byte;
    fread(&byte, sizeof(unsigned char), 1, bin);

    if (byte == 0 || byte == EOF)
    {
        return NULL;
    }

    struct TreeNode *node = malloc(sizeof(struct TreeNode));

    if (byte == 1)
    {
        init_node(node, 0, '\0');
    }
    else if (byte == 2)
    {
        unsigned char symbol;
        fread(&symbol, sizeof(unsigned char), 1, bin);

        init_node(node, 0, symbol);
    }
    else
    {
        assert(false);
    }

    root->left = read_tree_node(node, bin);
    root->right = read_tree_node(node, bin);

    return root;
}

// For all intents and purposes, while decoding we do not care about weights.
// They don't even exist within the encoded file, we simply set it to 0.
struct TreeNode *read_header(FILE *bin)
{
    assert(bin != NULL);

    struct TreeNode *root = malloc(sizeof(struct TreeNode));
    init_node(root, 0, '\0');

    read_tree_node(root, bin);

    return root;
}

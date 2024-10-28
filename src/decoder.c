#include "decoder.h"
#include "dyn-string.h"
#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline int fpeek2(FILE *stream)
{
    int c1;
    int c2;

    c1 = fgetc(stream);
    c2 = fgetc(stream);

    ungetc(c2, stream);
    ungetc(c1, stream);

    return c2;
}

static inline struct TreeNode *read_tree_node(struct TreeNode *root, FILE *bin)
{
    int byte;
    fread(&byte, sizeof(unsigned char), 1, bin);

    // Null node
    if (byte == 0)
    {
        return NULL;
    }
    // Node with no data
    else if (byte == 1)
    {
        init_node(root, 0, '\0');
    }
    // Node with data
    else if (byte == 2)
    {
        init_node(root, 0, '\0');
        fread(&root->symbol, sizeof(unsigned char), 1, bin);
    }
    else
    {
        assert(false);
    }

    struct TreeNode *left = malloc(sizeof(struct TreeNode));
    struct TreeNode *right = malloc(sizeof(struct TreeNode));

    root->left = read_tree_node(left, bin);
    root->right = read_tree_node(right, bin);

    // Make sure we're not leaking memory
    if (root->left == NULL)
    {
        free(left);
    }

    if (root->right == NULL)
    {
        free(right);
    }

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

struct DynString *read_body(FILE *bin, struct TreeNode *root)
{
    assert(bin != NULL);

    // We need a copy of our root pointer to reset to
    struct TreeNode *root_copy = root;

    struct DynString *string = malloc(sizeof(struct DynString));
    init_dyn_string(string, 16);

    // We need the last byte to tell how much padding we have, but first let's save where we are
    long pos = ftell(bin);

    int padding;
    fseek(bin, -1, SEEK_END);

    fread(&padding, sizeof(unsigned char), 1, bin);

    // Now rewind
    fseek(bin, pos, SEEK_SET);

    int byte;
    while ((byte = fgetc(bin)) != EOF)
    {
        bool last = fpeek2(bin) == EOF;

        // If our condition is true, we're on our last byte,
        // which is the padding we already read
        unsigned char bits_to_read = last ? 8 - padding : 8;

        for (unsigned char i = 0; i < bits_to_read; i++)
        {
            bool bit = byte & (1 << (7 - i));

            root = bit ? root->right : root->left;

            // If we're at a leaf, write the symbol
            if (root->symbol != '\0')
            {
                push_to_dyn_string(string, root->symbol);
                // Reset the root
                root = root_copy;
            }
        }

        if (last)
        {
            break;
        }
    }

    return string;
}

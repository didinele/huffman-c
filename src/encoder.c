#include "encoder.h"
#include "dict.h"
#include "tree.h"
#include <assert.h>

static inline void write_tree_node(FILE *out, struct TreeNode *node)
{
    if (node->symbol == 0)
    {
        // 0 for internal node
        unsigned char type = 0;
        fwrite(&type, sizeof(unsigned char), 1, out);
    }
    else
    {
        // 1 for leaf node
        unsigned char type = 1;
        fwrite(&type, sizeof(unsigned char), 1, out);
        // Write the symbol for our leaf
        fwrite(&node->symbol, sizeof(unsigned char), 1, out);
    }

    // Write the left and right children
    if (node->left != NULL)
    {
        write_tree_node(out, node->left);
    }

    if (node->right != NULL)
    {

        write_tree_node(out, node->right);
    }
}

static inline unsigned char bit_array_to_byte(bool bits[8])
{
    unsigned char byte = 0;

    for (int i = 0; i < 8; i++)
    {
        byte |= bits[i] << (7 - i);
    }

    return byte;
}

void write_header(FILE *out, struct Tree *tree)
{
    assert(out != NULL);

    // Write the number of nodes in the tree
    fwrite(&tree->node_count, sizeof(int), 1, out);

    // Start writing the nodes
    write_tree_node(out, tree->root);
}

void write_body(FILE *out, FILE *in, struct DictEntry dict[255])
{
    assert(out != NULL && in != NULL);

    bool bits_buf[8] = {0};
    int bits_buf_index = 0;

    int c;
    while ((c = fgetc(in)) != EOF)
    {
        struct DictEntry entry = dict[c];
        for (int i = 0; i < entry.length; i++)
        {
            bits_buf[bits_buf_index++] = entry.code[i];
            if (bits_buf_index == 8)
            {
                unsigned char byte = bit_array_to_byte(bits_buf);
                fwrite(&byte, sizeof(unsigned char), 1, out);
                bits_buf_index = 0;
            }
        }
    }

    // Flush the remaining bits, fill to the right with 0s if necessary,
    // because at that point we know for a fact we'll have reached a leaf.
    if (bits_buf_index > 0)
    {
        while (bits_buf_index < 8)
        {
            bits_buf[bits_buf_index++] = 0;
        }

        unsigned char byte = bit_array_to_byte(bits_buf);
        fwrite(&byte, sizeof(unsigned char), 1, out);
    }
}

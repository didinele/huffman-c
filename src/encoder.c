#include "encoder.h"
#include "dict.h"
#include "tree.h"
#include <assert.h>

static inline void write_tree_node(FILE *out, struct TreeNode *node)
{
#define WRITE_TYPE(type) fwrite(&type, sizeof(unsigned char), 1, out)

    if (node == NULL)
    {
        // Write a 0 to indicate that this node is null
        unsigned char type = 0;
        WRITE_TYPE(type);

        return;
    }

    if (node->symbol == 0)
    {
        // 1 for node with no data
        unsigned char type = 1;
        WRITE_TYPE(type);
    }
    else
    {
        // 2 is for node with data
        unsigned char type = 2;
        WRITE_TYPE(type);

        // Write the symbol for our leaf
        fwrite(&node->symbol, sizeof(unsigned char), 1, out);
    }

    // Write the left and right children
    write_tree_node(out, node->left);
    write_tree_node(out, node->right);

#undef WRITE_TYPE
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

void write_header(FILE *out, struct TreeNode *root)
{
    assert(out != NULL);
    write_tree_node(out, root);
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
    // because at that point we know for a fact we'll have reached a leaf,
    // and also the last byte in the file should be how many bits of padding we have.
    unsigned char padding = 0;

    if (bits_buf_index > 0)
    {
        while (bits_buf_index < 8)
        {
            bits_buf[bits_buf_index++] = 0;
            padding++;
        }

        unsigned char byte = bit_array_to_byte(bits_buf);
        fwrite(&byte, sizeof(unsigned char), 1, out);
    }

    fwrite(&padding, sizeof(unsigned char), 1, out);
}

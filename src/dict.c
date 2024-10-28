#include "dict.h"
#include <_string.h>
#include <stdlib.h>

void init_dict_entry(struct DictEntry *entry)
{
    for (int i = 0; i < 255; i++)
    {
        entry->code[i] = 0;
    }

    entry->length = 0;
}

void free_dict_entry(struct DictEntry *entry)
{
    free(entry);
}

static inline void traverse_tree(
    struct DictEntry *dict, bool code[255], unsigned char bit_array_index, struct TreeNode *root
)
{
    if (root == NULL)
    {
        return;
    }

    bool left_code[255];
    bool right_code[255];

    memcpy(left_code, code, 255);
    memcpy(right_code, code, 255);

    left_code[bit_array_index] = 0;
    right_code[bit_array_index] = 1;

    traverse_tree(dict, left_code, bit_array_index + 1, root->left);

    if (root->left == NULL && root->right == NULL)
    {
        // We've reached a leaf. Update the dictionary.
        memcpy(dict[root->symbol].code, code, bit_array_index);
        dict[root->symbol].length = bit_array_index;
    }

    traverse_tree(dict, right_code, bit_array_index + 1, root->right);
}

struct DictEntry *tree_to_dict(struct TreeNode *root)
{
    struct DictEntry *dict = malloc(256 * sizeof(struct DictEntry));
    for (int i = 0; i < 256; i++)
    {
        init_dict_entry(&dict[i]);
    }

    bool code[255] = {0};
    traverse_tree(dict, code, 0, root);

    return dict;
}

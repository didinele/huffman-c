#pragma once

#include "tree.h"

// A big problem with encoding this is as a stream of bits is how we handle
// byte alignment. As a fix, I've decided to fill the last byte with zeros
// ON THE RIGHT SIDE. As such, because a Huffman code can be 255 bits long,
// which we'll represent as a bool[255], and also store the length
// which we can use to determine how much padding we have.
struct DictEntry
{
    bool code[255];
    unsigned char length;
};

void init_dict_entry(struct DictEntry *entry);

void free_dict_entry(struct DictEntry *entry);

struct DictEntry *tree_to_dict(struct Tree *root);

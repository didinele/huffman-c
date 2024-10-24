#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_node(struct Node *node, int weight, char symbol)
{
    node->weight = weight;
    node->symbol = symbol;
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;
}

void free_node(struct Node *node)
{
    free(node);
}

void init_node_list(struct NodeList *list, int length)
{
    list->nodes = malloc(length * sizeof(struct Node));
    list->length = length;
}

void free_node_list(struct NodeList *list)
{
    // probably don't want to actually do anything else here?
    free(list);
}

static inline int compare_nodes(const void *a, const void *b)
{
    return ((struct Node *)a)->weight - ((struct Node *)b)->weight;
}

struct NodeList *node_list_from_file(FILE *file)
{
    assert(file != nullptr);

    int count[256] = {0};
    int unique_chars = 0;

    int c;
    while ((c = fgetc(file)) != EOF)
    {
        if (++count[c] == 1)
        {
            unique_chars++;
        }
    }

    struct NodeList *list = malloc(sizeof(struct NodeList));
    init_node_list(list, unique_chars);

    int node_index = 0;

    for (int i = 0; i < 256; i++)
    {
        if (count[i] > 0)
        {
            init_node(&list->nodes[node_index++], count[i], (char)i);
        }
    }

    return list;
}

struct Node *build_tree(struct NodeList *list)
{
    if (list->length == 1)
    {
        struct Node *final = malloc(sizeof(struct Node));
        memcpy(final, &list->nodes[0], sizeof(struct Node));

        return final;
    }

    qsort(list->nodes, list->length, sizeof(struct Node), &compare_nodes);

    struct Node *new_node = malloc(sizeof(struct Node));
    init_node(new_node, list->nodes[0].weight + list->nodes[1].weight, '\0');

    new_node->left = &list->nodes[0];
    new_node->left->parent = new_node;

    struct Node *right = malloc(sizeof(struct Node));
    memcpy(right, &list->nodes[1], sizeof(struct Node));

    new_node->right = right;
    new_node->right->parent = new_node;

    list->nodes[1] = *new_node;
    list->nodes = &list->nodes[1];
    list->length--;

    return build_tree(list);
}

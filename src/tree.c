#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_node(struct Node *node, int weight, char symbol)
{
    printf("init_node %p %i\n", (void *)node, weight);

    node->weight = weight;
    node->symbol = symbol;
    node->left = nullptr;
    node->right = nullptr;
}

void free_node(struct Node *node)
{
    printf("free_node %p %i\n", (void *)node, node->weight);

    if (node->left != nullptr)
    {
        free_node(node->left);
    }

    if (node->right != nullptr)
    {
        free_node(node->right);
    }

    free(node);
}

void init_node_list(struct NodeList *list, int length)
{
    list->nodes = malloc(length * sizeof(struct Node*));
    list->length = length;
}

void free_node_list(struct NodeList *list)
{
    free(list->nodes);
    free(list);
}

static inline int compare_nodes(const void *a, const void *b)
{
    return (*(struct Node **)a)->weight - (*(struct Node **)b)->weight;
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
            struct Node *node = malloc(sizeof(struct Node));
            init_node(node, count[i], (char)i);
            list->nodes[node_index++] = node;
        }
    }

    return list;
}

struct Node *build_tree(struct NodeList *list)
{
    if (list->length == 1)
    {
        return list->nodes[0];
    }

    qsort(list->nodes, list->length, sizeof(struct Node*), &compare_nodes);

    struct Node *new_node = malloc(sizeof(struct Node));
    init_node(new_node, list->nodes[0]->weight + list->nodes[1]->weight, '\0');

    struct Node *left = malloc(sizeof(struct Node));
    memcpy(left, list->nodes[0], sizeof(struct Node));
    free(list->nodes[0]);
    new_node->left = left;

    struct Node *right = malloc(sizeof(struct Node));
    memcpy(right, list->nodes[1], sizeof(struct Node));
    free(list->nodes[1]);
    new_node->right = right;

    list->nodes[0] = new_node;

    memcpy(&list->nodes[1], &list->nodes[2], (list->length - 2) * sizeof(struct Node*));
    list->length--;

    return build_tree(list);
}

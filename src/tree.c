#include "tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_node(struct TreeNode *node, int weight, unsigned char symbol)
{
    node->weight = weight;
    node->symbol = symbol;
    node->left = NULL;
    node->right = NULL;
}

void free_node(struct TreeNode *node)
{
    if (node->left != NULL)
    {
        free_node(node->left);
    }

    if (node->right != NULL)
    {
        free_node(node->right);
    }

    free(node);
}

void init_node_list(struct TreeNodeList *list, int length)
{
    list->nodes = malloc(length * sizeof(struct TreeNode *));
    list->length = length;
}

void free_node_list(struct TreeNodeList *list)
{
    free(list->nodes);
    free(list);
}

static inline int compare_nodes(const void *a, const void *b)
{
    return (*(struct TreeNode **)a)->weight - (*(struct TreeNode **)b)->weight;
}

struct TreeNodeList *node_list_from_file(FILE *file)
{
    assert(file != NULL);

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

    struct TreeNodeList *list = malloc(sizeof(struct TreeNodeList));
    init_node_list(list, unique_chars);

    int node_index = 0;

    for (int i = 0; i < 256; i++)
    {
        if (count[i] > 0)
        {
            struct TreeNode *node = malloc(sizeof(struct TreeNode));
            init_node(node, count[i], (unsigned char)i);
            list->nodes[node_index++] = node;
        }
    }

    return list;
}

void init_tree(struct Tree *tree, struct TreeNode *root, int node_count) {
    tree->root = root;
    tree->node_count = node_count;
}

void free_tree(struct Tree *tree) {
    free_node(tree->root);
    free(tree);
}

struct TreeNode *build_tree_root(struct TreeNodeList *list)
{
    if (list->length == 1)
    {
        return list->nodes[0];
    }

    qsort(list->nodes, list->length, sizeof(struct TreeNode *), &compare_nodes);

    struct TreeNode *new_node = malloc(sizeof(struct TreeNode));
    init_node(new_node, list->nodes[0]->weight + list->nodes[1]->weight, '\0');

    struct TreeNode *left = malloc(sizeof(struct TreeNode));
    memcpy(left, list->nodes[0], sizeof(struct TreeNode));
    free(list->nodes[0]);
    new_node->left = left;

    struct TreeNode *right = malloc(sizeof(struct TreeNode));
    memcpy(right, list->nodes[1], sizeof(struct TreeNode));
    free(list->nodes[1]);
    new_node->right = right;

    list->nodes[0] = new_node;

    memcpy(&list->nodes[1], &list->nodes[2], (list->length - 2) * sizeof(struct TreeNode *));
    list->length--;

    return build_tree_root(list);
}

#pragma once

#include <stdio.h>

struct TreeNode
{
    int weight;
    // null for leafs
    unsigned char symbol;

    struct TreeNode *left, *right;
};

void init_node(struct TreeNode *node, int weight, unsigned char symbol);
void free_node(struct TreeNode *node);

struct TreeNodeList
{
    struct TreeNode **nodes;
    int length;
};

void init_node_list(struct TreeNodeList *list, int length);
void free_node_list(struct TreeNodeList *list);

struct TreeNodeList *node_list_from_file(FILE *file);

struct Tree
{
    struct TreeNode *root;
    int node_count;
};

void init_tree(struct Tree *tree, struct TreeNode *root, int node_count);
void free_tree(struct Tree *tree);

struct TreeNode *build_tree_root(struct TreeNodeList *list);

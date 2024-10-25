#pragma once

#include <stdio.h>

struct Node
{
    int weight;
    // null for leafs
    unsigned char symbol;

    struct Node *left, *right;
};

void init_node(struct Node *node, int weight, unsigned char symbol);

void free_node(struct Node *node);

struct NodeList
{
    struct Node **nodes;
    int length;
};

void init_node_list(struct NodeList *list, int length);

void free_node_list(struct NodeList *list);

struct NodeList *node_list_from_file(FILE *file);

struct Node *build_tree(struct NodeList *list);

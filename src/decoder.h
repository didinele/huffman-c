#pragma once

#include <stdio.h>

struct TreeNode* read_header(FILE *bin);
char *read_body(FILE *bin, struct TreeNode *root);

#pragma once

#include "dyn-string.h"
#include <stdio.h>

struct TreeNode *read_header(FILE *bin);
struct DynString *read_body(FILE *bin, struct TreeNode *root);

#pragma once

#include "tree.h"
#include "dict.h"

struct Tree* read_header(FILE* bin);
char* read_body(FILE* bin, struct Tree* tree, struct DictEntry dict[255]);

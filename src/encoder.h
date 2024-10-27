#pragma once

#include "tree.h"
#include "dict.h"

void write_header(FILE *out, struct Tree *tree);
void write_body(FILE *out, FILE *in, struct DictEntry dict[255]);

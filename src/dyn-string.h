#pragma once

struct DynString
{
    char *data;
    int length, capacity;
};

void init_dyn_string(struct DynString *string, int capacity);
void free_dyn_string(struct DynString *string);
void resize_dyn_string(struct DynString *string, int new_capacity);
void push_to_dyn_string(struct DynString *string, char c);

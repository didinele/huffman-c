#pragma once

struct TinyString
{
    char *data;
    int length, capacity;
};

void init_string(struct TinyString *string, int capacity);
void free_string(struct TinyString *string);
void resize_string(struct TinyString *string, int new_capacity);
void push_to_string(struct TinyString *string, char c);

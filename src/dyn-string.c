#include "dyn-string.h"
#include <stdlib.h>

void init_dyn_string(struct DynString *string, int capacity)
{
    string->data = malloc(capacity);
    string->length = 0;
    string->capacity = capacity;
}

void free_dyn_string(struct DynString *string)
{
    free(string->data);
}

void resize_dyn_string(struct DynString *string, int new_capacity)
{
    string->data = realloc(string->data, new_capacity);
    string->capacity = new_capacity;
}

void push_to_dyn_string(struct DynString *string, char c)
{
    if (string->length == string->capacity)
    {
        resize_dyn_string(string, string->capacity * 2);
    }

    string->data[string->length++] = c;
}

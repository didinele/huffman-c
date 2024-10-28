#include "tiny-string.h"
#include <stdlib.h>

void init_string(struct TinyString *string, int capacity)
{
    string->data = malloc(capacity);
    string->length = 0;
    string->capacity = capacity;
}

void free_string(struct TinyString *string)
{
    free(string->data);
}

void resize_string(struct TinyString *string, int new_capacity)
{
    string->data = realloc(string->data, new_capacity);
    string->capacity = new_capacity;
}

void push_to_string(struct TinyString *string, char c)
{
    if (string->length == string->capacity)
    {
        resize_string(string, string->capacity * 2);
    }

    string->data[string->length++] = c;
}

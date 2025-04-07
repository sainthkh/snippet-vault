#include "stringstore.h"

#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define STRING_CAPACITY 512

void stringstore_init(StringStore *store)
{
    store->pos = malloc(sizeof(string_id) * INITIAL_CAPACITY);
    memset(store->pos, 0, sizeof(string_id) * INITIAL_CAPACITY);
    store->buffer = malloc(sizeof(char) * STRING_CAPACITY);
    memset(store->buffer, 0, sizeof(char) * STRING_CAPACITY);
    store->count = 0;
    store->capacity = INITIAL_CAPACITY;
    store->bufferCapacity = STRING_CAPACITY;
    store->bufferSize = 0;
}

void stringstore_release(StringStore *store)
{
    free(store->pos);
    free(store->buffer);
    store->count = 0;
    store->capacity = 0;
    store->bufferCapacity = 0;
    store->bufferSize = 0;
}

string_id stringstore_add(StringStore *store, const char *string)
{
    if (store->count == store->capacity)
    {
        store->capacity *= 2;
        store->pos = realloc(store->pos, sizeof(string_id) * store->capacity);
    }

    if (store->bufferSize + strlen(string) + 1 > store->bufferCapacity)
    {
        store->bufferCapacity *= 2;
        store->buffer = realloc(store->buffer, sizeof(char) * store->bufferCapacity);
    }

    string_id id = store->count++;
    store->pos[id] = store->bufferSize;
    strcpy(store->buffer + store->bufferSize, string);
    store->bufferSize += strlen(string) + 1;

    return id;
}

const char *stringstore_get(StringStore *store, string_id id)
{
    return store->buffer + store->pos[id];
}

static StringStore gss;

void gss_init()
{
    stringstore_init(&gss);
}

void gss_release()
{
    stringstore_release(&gss);
}

string_id gss_add(const char *string)
{
    return stringstore_add(&gss, string);
}

const char *gss_get(int id)
{
    return stringstore_get(&gss, id);
}

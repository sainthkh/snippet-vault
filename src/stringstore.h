#pragma once

typedef int string_id;

typedef struct
{
    string_id *pos;
    char *buffer;
    int count;
    int capacity;
    int bufferCapacity;
    int bufferSize;
} StringStore;

void stringstore_init(StringStore *store);
void stringstore_release(StringStore *store);
string_id stringstore_add(StringStore *store, const char *string);
const char *stringstore_get(StringStore *store, string_id id);

void gss_init();
void gss_release();
string_id gss_add(const char *string);
const char *gss_get(int id);

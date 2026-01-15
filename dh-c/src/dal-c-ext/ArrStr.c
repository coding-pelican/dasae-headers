#include "dal-c-ext/ArrStr.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

ArrStr* ArrStr_init(void) {
    ArrStr* const arr = (ArrStr*)calloc(1, sizeof(ArrStr));
    if (!arr) { return NULL; }
    arr->cap = 16;
    arr->items = (char**)malloc((size_t)arr->cap * sizeof(char*));
    if (!arr->items) { return free(arr), NULL; }
    return arr;
}

void ArrStr_fini(ArrStr** self) {
    if (!self || !*self) { return; }
    ArrStr* const arr = *self;
    for (int i = 0; i < arr->items_len; ++i) { free(arr->items[i]); }
    free((void*)arr->items);
    free(arr);
    *self = NULL;
}

char** ArrStr_toRaw(const ArrStr* self) {
    assert(self != NULL);
    // Allocate array with NULL terminator
    char** const result = (char**)malloc((size_t)(self->items_len + 1) * sizeof(char*));
    if (!result) { return NULL; }
    for (int i = 0; i < self->items_len; ++i) { result[i] = strdup(self->items[i]); }
    result[self->items_len] = NULL;
    return result;
}

int ArrStr_len(const ArrStr* self) {
    assert(self != NULL);
    return self->items_len;
}

const char* ArrStr_at(const ArrStr* self, int idx) {
    assert(self != NULL);
    assert(0 <= idx);
    assert(idx < self->items_len);
    return self->items[idx];
}

char* ArrStr_atMut(ArrStr* self, int idx) {
    assert(self != NULL);
    assert(0 <= idx);
    assert(idx < self->items_len);
    return self->items[idx];
}

void ArrStr_push(ArrStr* self, const char* item) {
    assert(self != NULL);
    assert(item != NULL);
    // Grow if needed
    if (self->items_len >= self->cap) {
        const int new_capacity = self->cap * 2;
        char** const temp = (char**)realloc((void*)self->items, (size_t)new_capacity * sizeof(char*));
        if (!temp) { return abort(); /* Out of memory */ }
        self->items = temp;
        self->cap = new_capacity;
    }
    // Add item
    self->items[self->items_len] = strdup(item);
    self->items_len++;
}

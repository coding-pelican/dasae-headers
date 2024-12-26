#include "dh/core.h"

// #include "dh/builtin.h"
// #include <stdbool.h>
// #include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

// typedef uint8_t  u8;
// typedef uint64_t usize;
// #define anyptr void*
// typedef struct Void {
//     u8 unused_[1];
// } Void;
// #define unused(...) ((void)(__VA_ARGS__))
// #define null        NULL



typedef struct mem_Allocator   mem_Allocator;
typedef struct mem_AllocatorVT mem_AllocatorVT;

typedef struct mem_AllocatorError       mem_AllocatorError;
typedef union mem_AllocatorError$anyptr mem_AllocatorError$anyptr;
typedef enum mem_AllocatorErrorType {
    mem_AllocatorError_None        = 0,
    mem_AllocatorError_OutOfMemory = 1
} mem_AllocatorErrorType;



struct mem_Allocator {
    anyptr                 ctx;
    const mem_AllocatorVT* vt;
};

struct mem_AllocatorVT {
    anyptr (*alloc)(anyptr ctx, usize size);
    anyptr (*realloc)(anyptr ctx, anyptr ptr, usize old_size, usize new_size);
    void (*free)(anyptr ctx, anyptr ptr, usize size);
};

struct mem_AllocatorError {
    mem_AllocatorErrorType type;
};

union mem_AllocatorError$anyptr {
    anyptr             ok;
    mem_AllocatorError err;
};



typedef struct heap_C heap_C;

mem_Allocator heap_C_allocator(heap_C* self);

static anyptr heap_C_alloc(anyptr ctx, usize size);
static anyptr heap_C_realloc(anyptr ctx, anyptr ptr, usize old_size, usize new_size);
static void   heap_C_free(anyptr ctx, anyptr ptr, usize size);



typedef struct heap_C {
    Void unused_;
} heap_C;

mem_Allocator heap_C_allocator(heap_C* self) {
    static const mem_AllocatorVT vt[1] = { {
        .alloc   = heap_C_alloc,
        .realloc = heap_C_realloc,
        .free    = heap_C_free,
    } };
    return (mem_Allocator){
        .ctx = self,
        .vt  = vt,
    };
}

static anyptr heap_C_alloc(anyptr ctx, usize size) {
    unused(ctx);
    return malloc(size);
}

static anyptr heap_C_realloc(anyptr ctx, anyptr ptr, usize old_size, usize new_size) {
    unused(ctx);
    unused(old_size);
    return realloc(ptr, new_size);
}

static void heap_C_free(anyptr ctx, anyptr ptr, usize size) {
    unused(ctx);
    unused(size);
    free(ptr);
}



typedef struct Slice Slice;
struct Slice {
    anyptr ptr;
    usize  len;
};



typedef struct ArrayList                    ArrayList;
typedef struct mem_AllocatorError$ArrayList mem_AllocatorError$ArrayList;
typedef struct mem_AllocatorError$Void      mem_AllocatorError$Void;

static mem_AllocatorError$ArrayList ArrayList_init(usize item_size, mem_Allocator allocator);
static void                         ArrayList_fini(ArrayList* self);
static mem_AllocatorError$Void      ArrayList_append(ArrayList* self, const anyptr item);
static Slice                        ArrayList_slice(const ArrayList* self);



struct ArrayList {
    mem_Allocator allocator;
    Slice         items;
    usize         capacity;
    usize         item_size;
};

struct mem_AllocatorError$ArrayList {
    bool is_error;
    union {
        mem_AllocatorError err;
        ArrayList          ok;
    };
};
static mem_AllocatorError$ArrayList ArrayList_init(usize item_size, mem_Allocator allocator) {
    anyptr const ptr = allocator.vt->alloc(allocator.ctx, item_size * 8); // Initial capacity of 8
    if (!ptr) {
        return (mem_AllocatorError$ArrayList){
            .err = {
                .type = mem_AllocatorError_OutOfMemory,
            }
        };
    }
    return (mem_AllocatorError$ArrayList){
        .ok = {
            .allocator = allocator,
            .items     = {
                    .ptr = ptr,
                    .len = 0,
            },
            .capacity  = 8,
            .item_size = item_size,
        }
    };
}

static void ArrayList_fini(ArrayList* self) {
    self->allocator.vt->free(self->allocator.ctx, self->items.ptr, self->capacity * self->item_size);
    self->items.ptr = null;
    self->items.len = 0;
    self->capacity  = 0;
}

struct mem_AllocatorError$Void {
    bool is_error;
    union {
        mem_AllocatorError err;
        Void               ok;
    };
};
static mem_AllocatorError$Void ArrayList_append(ArrayList* self, const anyptr item) {
    if (self->items.len >= self->capacity) {
        const usize  new_capacity = self->capacity * 2;
        anyptr const new_ptr      = self->allocator.vt->realloc(
            self->allocator.ctx,
            self->items.ptr,
            self->capacity * self->item_size,
            new_capacity * self->item_size
        );
        if (!new_ptr) {
            return (mem_AllocatorError$Void){
                .err = {
                    .type = mem_AllocatorError_OutOfMemory,
                }
            };
        }

        self->items.ptr = new_ptr;
        self->capacity  = new_capacity;
    }

    anyptr const dest = (u8*)self->items.ptr + (self->items.len * self->item_size);
    memcpy(dest, item, self->item_size);
    self->items.len++;

    return (mem_AllocatorError$Void){
        .ok = (Void){}
    };
}

static Slice ArrayList_slice(const ArrayList* self) {
    return self->items;
}



// Usage example
void example(void) {
    // Initialize the C heap allocator
    heap_C heap      = {};
    let    allocator = heap_C_allocator(&heap);

    // Create an ArrayList of integers
    let list_result = ArrayList_init(sizeof(int), allocator);
    if (list_result.is_error) {
        printf("Failed to create ArrayList: out of memory\n");
        return;
    }
    var list = list_result.ok;

    // Add some numbers
    for (usize i = 0; i < 15; ++i) {
        let append_result = ArrayList_append(&list, &i);
        if (append_result.is_error) {
            printf("Failed to append item: out of memory\n");
            ArrayList_fini(&list);
            return;
        }
    }

    // Get a slice of the data
    let        slice = ArrayList_slice(&list);
    int* const items = slice.ptr;

    // Print all numbers
    for (usize i = 0; i < slice.len; ++i) {
        printf("%d ", items[i]);
    }
    printf("\n");

    // Clean up
    ArrayList_fini(&list);
}



int main(void) {
    example();
    return 0;
}

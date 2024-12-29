#include "dh/core.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Classic.h"

// typedef struct mem_Allocator   mem_Allocator;
// typedef struct mem_AllocatorVT mem_AllocatorVT;
// impl_Err(mem_AllocErr, OutOfMemory);

// struct mem_Allocator {
//     anyptr                 ctx;
//     const mem_AllocatorVT* vt;
// };

// struct mem_AllocatorVT {
//     anyptr (*alloc)(anyptr ctx, usize size);
//     anyptr (*realloc)(anyptr ctx, anyptr ptr, usize old_size, usize new_size);
//     void (*free)(anyptr ctx, anyptr ptr, usize size);
// };

// typedef struct heap_C heap_C;

// mem_Allocator heap_C_allocator(heap_C* self);

// static anyptr heap_C_alloc(anyptr ctx, usize size);
// static anyptr heap_C_realloc(anyptr ctx, anyptr ptr, usize old_size, usize new_size);
// static void   heap_C_free(anyptr ctx, anyptr ptr, usize size);

// typedef struct heap_C {
//     Void unused_;
// } heap_C;

// mem_Allocator heap_C_allocator(heap_C* self) {
//     static const mem_AllocatorVT vt[1] = { {
//         .alloc   = heap_C_alloc,
//         .realloc = heap_C_realloc,
//         .free    = heap_C_free,
//     } };
//     return (mem_Allocator){
//         .ctx = self,
//         .vt  = vt,
//     };
// }

// static anyptr heap_C_alloc(anyptr ctx, usize size) {
//     unused(ctx);
//     return malloc(size);
// }

// static anyptr heap_C_realloc(anyptr ctx, anyptr ptr, usize old_size, usize new_size) {
//     unused(ctx);
//     unused(old_size);
//     return realloc(ptr, new_size);
// }

// static void heap_C_free(anyptr ctx, anyptr ptr, usize size) {
//     unused(ctx);
//     unused(size);
//     free(ptr);
// }

typedef struct Slice Slice;
struct Slice {
    anyptr ptr;
    usize  len;
};

typedef struct ArrayList ArrayList;
decl_ErrRes(ArrayList);

struct ArrayList {
    mem_Allocator allocator;
    Slice         items;
    usize         capacity;
    usize         item_size;
};
static must_check Err$ArrayList ArrayList_init(TypeInfo type, mem_Allocator allocator);
static void                     ArrayList_fini(ArrayList* self);
static must_check Err$void      ArrayList_append(ArrayList* self, const anyptr item);
static Slice                    ArrayList_slice(const ArrayList* self);

impl_ErrRes(ArrayList);
static must_check Err$ArrayList ArrayList_init(TypeInfo type, mem_Allocator allocator) {
    reserveReturn(Err$ArrayList);

    anyptr const ptr = unwrap(allocator.vt->alloc(allocator.ctx, type.size * 8, type.align)); // Initial capacity of 8
    if (!ptr) {
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }
    return_ok((ArrayList){
        .allocator = allocator,
        .items     = {
                .ptr = ptr,
                .len = 0,
        },
        .capacity  = 8,
        .item_size = type.size,
    });
}

static void ArrayList_fini(ArrayList* self) {
    var memory = (Sli$u8){ .ptr = self->items.ptr, .len = self->capacity };
    mem_Allocator_free(self->allocator, AnySli(memory));
    self->items.ptr = null;
    self->items.len = 0;
    self->capacity  = 0;
}

static must_check Err$void ArrayList_append(ArrayList* self, const anyptr item) {
    reserveReturn(Err$void);

    if (self->items.len == 10) { // Note: Force error raising for exception checking
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }

    if (self->items.len >= self->capacity) {
        let new_capacity = self->capacity * 2;
        var memory       = (Sli$u8){ .ptr = self->items.ptr, .len = self->capacity };
        let reallocated  = mem_Allocator_realloc(self->allocator, AnySli(memory), new_capacity);
        if (!reallocated.has_value) {
            return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }
        let new_slice = unwrap(reallocated);

        self->items.ptr = new_slice.ptr.addr;
        self->capacity  = new_capacity;
    }
    anyptr const dest = (u8*)self->items.ptr + (self->items.len * self->item_size);
    memcpy(dest, item, self->item_size);
    self->items.len++;

    return_ok({});
}

static Slice ArrayList_slice(const ArrayList* self) {
    return self->items;
}

// Example usage in main
Err$void dh_main(int argc, const char* argv[]) {
    reserveReturn(Err$void);
    unused(argc);
    unused(argv);

    // Initialize the C heap allocator
    var heap      = (heap_Classic){};
    let allocator = heap_Classic_allocator(&heap);
    catch (heap_Classic_init(allocator), err, {
        ignore fprintf(stderr, "Failed to initialize heap: %s, in %s:%d\n", Err_message(err), __FILE__, __LINE__);
        return_err(err);
    });

    // Create an ArrayList of integers
    var list = catch (ArrayList_init(typeInfo(i32), allocator), err, {
        ignore fprintf(stderr, "Failed to initialize ArrayList: %s, in %s:%d\n", Err_message(err), __FILE__, __LINE__);
        heap_Classic_fini(allocator);
        return_err(err);
    });

    // Add some numbers
    for (usize i = 0; i < 10; ++i) {
        catch (ArrayList_append(&list, &i), err, {
            ignore fprintf(stderr, "Failed to append to ArrayList: %s, in %s:%d\n", Err_message(err), __FILE__, __LINE__);
            ignore fprintf(stderr, "index %zu\n", i);
            ArrayList_fini(&list);
            heap_Classic_fini(allocator);
            return_err(err);
        });
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
    heap_Classic_fini(allocator);

    return_ok({});
}

// void example(void) {
//     // Initialize the C heap allocator
//     heap_C heap      = {};
//     let    allocator = heap_C_allocator(&heap);

//     // Create an ArrayList of integers
//     let list_result = ArrayList_init(sizeof(int), allocator);
//     if (list_result.is_err) {
//         printf("Failed to create ArrayList: out of memory\n");
//         return;
//     }
//     var list = list_result.ok;

//     // Add some numbers
//     for (usize i = 0; i < 15; ++i) {
//         let append_result = ArrayList_append(&list, &i);
//         if (append_result.is_err) {
//             printf("Failed to append item: out of memory\n");
//             ArrayList_fini(&list);
//             return;
//         }
//     }

//     // Get a slice of the data
//     let        slice = ArrayList_slice(&list);
//     int* const items = slice.ptr;

//     // Print all numbers
//     for (usize i = 0; i < slice.len; ++i) {
//         printf("%d ", items[i]);
//     }
//     printf("\n");

//     // Clean up
//     ArrayList_fini(&list);
// }

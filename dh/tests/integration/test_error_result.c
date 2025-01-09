#include "dh/main.h"
#include "dh/core.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Classic.h"

typedef struct Slice Slice;
struct Slice {
    anyptr ptr;
    usize  len;
};

typedef struct ArrList ArrList;
decl_Err$(ArrList);

struct ArrList {
    mem_Allocator allocator;
    Slice         items;
    usize         cap;
    usize         item_size;
};
static must_check Err$ArrList ArrList_init(TypeInfo type, mem_Allocator allocator);
static void                   ArrList_fini(ArrList* self);
static must_check Err$void    ArrList_append(ArrList* self, const anyptr item);
static Slice                  ArrList_slice(const ArrList* self);

impl_Err$(ArrList);
static must_check Err$ArrList ArrList_init(TypeInfo type, mem_Allocator allocator) {
    reserveReturn(Err$ArrList);

    anyptr const ptr = unwrap(allocator.vt->alloc(allocator.ctx, type.size * 8, type.align)); // Initial capacity of 8
    if (!ptr) {
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }
    return_ok((ArrList){
        .allocator = allocator,
        .items     = {
                .ptr = ptr,
                .len = 0,
        },
        .cap       = 8,
        .item_size = type.size,
    });
}

static void ArrList_fini(ArrList* self) {
    var memory = (Sli$u8){ .ptr = self->items.ptr, .len = self->cap };
    mem_Allocator_free(self->allocator, AnySli(memory));
    self->items.ptr = null;
    self->items.len = 0;
    self->cap       = 0;
}

static must_check Err$void ArrList_append(ArrList* self, const anyptr item) {
    reserveReturn(Err$void);

    if (self->items.len == 10) { // Note: Force error raising for exception checking
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }

    if (self->items.len >= self->cap) {
        let new_capacity = self->cap * 2;
        var memory       = (Sli$u8){ .ptr = self->items.ptr, .len = self->cap };
        let reallocated  = mem_Allocator_realloc(self->allocator, AnySli(memory), new_capacity);
        if (!reallocated.has_value) {
            return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }
        let new_slice = unwrap(reallocated);

        self->items.ptr = new_slice.ptr.addr;
        self->cap       = new_capacity;
    }
    anyptr const dest = (u8*)self->items.ptr + (self->items.len * self->item_size);
    memcpy(dest, item, self->item_size);
    self->items.len++;

    return_ok({});
}

static Slice ArrList_slice(const ArrList* self) {
    return self->items;
}
// todo apply errdefer
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

    // Create an ArrList of integers
    var list = catch (ArrList_init(typeInfo(i32), allocator), err, {
        ignore fprintf(stderr, "Failed to initialize ArrList: %s, in %s:%d\n", Err_message(err), __FILE__, __LINE__);
        heap_Classic_fini(allocator);
        return_err(err);
    });

    // Add some numbers
    for (usize i = 0; i < 10; ++i) {
        catch (ArrList_append(&list, &i), err, {
            ignore fprintf(stderr, "Failed to append to ArrList: %s, in %s:%d\n", Err_message(err), __FILE__, __LINE__);
            ignore fprintf(stderr, "index %zu\n", i);
            ArrList_fini(&list);
            heap_Classic_fini(allocator);
            return_err(err);
        });
    }

    // Get a slice of the data
    let        slice = ArrList_slice(&list);
    int* const items = slice.ptr;

    // Print all numbers
    for (usize i = 0; i < slice.len; ++i) {
        printf("%d ", items[i]);
    }
    printf("\n");

    // Clean up
    ArrList_fini(&list);
    heap_Classic_fini(allocator);

    return_ok({});
}

// void example(void) {
//     // Initialize the C heap allocator
//     heap_C heap      = {};
//     let    allocator = heap_C_allocator(&heap);

//     // Create an ArrList of integers
//     let list_result = ArrList_init(sizeof(int), allocator);
//     if (list_result.is_err) {
//         printf("Failed to create ArrList: out of memory\n");
//         return;
//     }
//     var list = list_result.ok;

//     // Add some numbers
//     for (usize i = 0; i < 15; ++i) {
//         let append_result = ArrList_append(&list, &i);
//         if (append_result.is_err) {
//             printf("Failed to append item: out of memory\n");
//             ArrList_fini(&list);
//             return;
//         }
//     }

//     // Get a slice of the data
//     let        slice = ArrList_slice(&list);
//     int* const items = slice.ptr;

//     // Print all numbers
//     for (usize i = 0; i < slice.len; ++i) {
//         printf("%d ", items[i]);
//     }
//     printf("\n");

//     // Clean up
//     ArrList_fini(&list);
// }

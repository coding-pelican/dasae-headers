#if UNUSED_CODE
#define main_no_args (1)
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
static $must_check Err$ArrList ArrList_init(TypeInfo type, mem_Allocator allocator);
static void                    ArrList_fini(ArrList* self);
static $must_check Err$void    ArrList_append(ArrList* self, const anyptr item);
static Slice                   ArrList_slice(const ArrList* self);

impl_Err$(ArrList);
static $must_check Err$ArrList ArrList_init(TypeInfo type, mem_Allocator allocator) {
    reserveReturn(Err$ArrList);

    anyptr const ptr = unwrap(allocator.vt->alloc(allocator.ptr, type.size * 8ull, type.align)); // Initial capacity of 8
    if (!ptr) {
        return_err(mem_AllocErr_OutOfMemory());
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
    mem_Allocator_free(self->allocator, anySli(memory));
    self->items.ptr = null;
    self->items.len = 0;
    self->cap       = 0;
}

static $must_check Err$void ArrList_append(ArrList* self, const anyptr item) {
    reserveReturn(Err$void);

    if (self->items.len == 10) { // Note: Force error raising for exception checking
        return_err(mem_AllocErr_OutOfMemory());
    }

    if (self->items.len >= self->cap) {
        let new_capacity = self->cap * 2;
        var memory       = (Sli$u8){ .ptr = self->items.ptr, .len = self->cap };
        let reallocated  = mem_Allocator_realloc(self->allocator, anySli(memory), new_capacity);
        if (!reallocated.has_value) {
            return_err(mem_AllocErr_OutOfMemory());
        }
        let new_slice = unwrap(reallocated);

        self->items.ptr = new_slice.addr;
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

Err$void dh_main(void) {
    scope_reserveReturn(Err$void);
    // Initialize the C heap allocator
    let allocator = heap_Classic_allocator(&(heap_Classic){});

    // Create an ArrList of integers
    var list = try(ArrList_init(typeInfo$(i32), allocator));
    defer(ArrList_fini(&list));

    // Add some numbers
    for (usize i = 0; i < 10; ++i) {
        try(ArrList_append(&list, &i));
    }

    // Get a slice of the data
    let slice = ArrList_slice(&list);
    let items = as$(const i32*, slice.ptr);

    // Print all numbers
    {
        for (usize i = 0; i < slice.len; ++i) {
            printf("%d ", items[i]);
        }
        printf("\n");
    }

    return_void();
        scope_returnReserved
    ;
}
#endif /* UNUSED_CODE */

// zig_slice.h
#ifndef ZIG_SLICE_INCLUDED
#define ZIG_SLICE_INCLUDED (1)

#include "zig_ptr.h" // Include previous pointer implementation

#include <assert.h>

/*========== Enhanced Slice Types ===========================================*/

// Basic slice type
typedef struct {
    CPtr  core;     // Core pointer with metadata
    usize len;      // Runtime-known length
    bool  is_const; // Mutability flag
} Slice;

// Sentinel-terminated slice type
typedef struct {
    CPtr  core;          // Core pointer with metadata
    usize len;           // Runtime-known length
    bool  is_const;      // Mutability flag
    u8    sentinel[16];  // Sentinel value (max 16 bytes)
    usize sentinel_size; // Size of sentinel value
} SentinelSlice;

/*========== Slice Creation ===============================================*/

// Create slice from raw pointer and length
force_inline Slice slice_create(anyptr ptr, usize len, usize elem_size, usize alignment, bool is_const) {
    return (Slice){
        .core     = core_ptr_create(ptr, elem_size, alignment),
        .len      = len,
        .is_const = is_const
    };
}

// Create sentinel-terminated slice
force_inline SentinelSlice sentinel_slice_create(
    anyptr ptr, usize len, usize elem_size, usize alignment, const anyptr sentinel, usize sentinel_size, bool is_const
) {
    assert(sentinel_size <= 16);
    SentinelSlice result = {
        .core          = core_ptr_create(ptr, elem_size, alignment),
        .len           = len,
        .is_const      = is_const,
        .sentinel_size = sentinel_size
    };
    memcpy(result.sentinel, sentinel, sentinel_size);

    // Verify sentinel value at len position
    const anyptr sentinel_pos = (const char*)ptr + len * elem_size;
    assert(memcmp(sentinel_pos, sentinel, sentinel_size) == 0);

    return result;
}

/*========== Slice Access ================================================*/

// Get pointer to slice element with bounds checking
force_inline anyptr slice_ptr_at(Slice slice, usize index) {
    assert(index < slice.len && "slice index out of bounds");
    return (char*)core_ptr_raw(slice.core) + index * core_ptr_size(slice.core);
}

// Get pointer to sentinel slice element (allows access at len)
force_inline anyptr sentinel_slice_ptr_at(SentinelSlice slice, usize index) {
    assert(index <= slice.len && "sentinel slice index out of bounds");
    return (char*)core_ptr_raw(slice.core) + index * core_ptr_size(slice.core);
}

// Macros for type-safe element access
#define slice_get(slice, T, index) \
    (*(T*)slice_ptr_at(slice, index))

#define slice_get_sentinel(slice, T, index) \
    (*(T*)sentinel_slice_ptr_at(slice, index))

/*========== Slice Operations ============================================*/

// Create sub-slice with runtime bounds
force_inline Slice slice_slice(Slice slice, usize start, usize end) {
    assert(start <= end && end <= slice.len && "invalid slice bounds");
    return slice_create(
        (char*)core_ptr_raw(slice.core) + start * core_ptr_size(slice.core),
        end - start,
        core_ptr_size(slice.core),
        core_ptr_align(slice.core),
        slice.is_const
    );
}

// Create sentinel-terminated sub-slice
force_inline SentinelSlice slice_slice_sentinel(
    Slice slice, usize start, usize end, const anyptr sentinel, usize sentinel_size
) {
    assert(start <= end && end <= slice.len && "invalid slice bounds");
    return sentinel_slice_create(
        (char*)core_ptr_raw(slice.core) + start * core_ptr_size(slice.core),
        end - start,
        core_ptr_size(slice.core),
        core_ptr_align(slice.core),
        sentinel,
        sentinel_size,
        slice.is_const
    );
}

// Convert array pointer to slice
force_inline Slice array_ptr_to_slice(ArrPtr arr) {
    return slice_create(
        core_ptr_raw(arr.core),
        arr.len,
        core_ptr_size(arr.core),
        core_ptr_align(arr.core),
        false
    );
}

// Convert array pointer to const slice
force_inline Slice array_ptr_to_const_slice(ArrPtr arr) {
    return slice_create(
        core_ptr_raw(arr.core),
        arr.len,
        core_ptr_size(arr.core),
        core_ptr_align(arr.core),
        true
    );
}

// Get many-item pointer from slice
force_inline MPtr slice_ptr(Slice slice) {
    return many_item_ptr(
        core_ptr_raw(slice.core),
        core_ptr_size(slice.core),
        core_ptr_align(slice.core)
    );
}

// Get single-item pointer to element
force_inline SPtr slice_element_ptr(Slice slice, usize index) {
    assert(index < slice.len && "slice index out of bounds");
    return single_item_ptr(
        (char*)core_ptr_raw(slice.core) + index * core_ptr_size(slice.core),
        core_ptr_size(slice.core),
        core_ptr_align(slice.core)
    );
}

/*========== Slice Comparison ============================================*/

// Compare two slices for equality
force_inline bool slice_eql(Slice a, Slice b) {
    if (a.len != b.len || core_ptr_size(a.core) != core_ptr_size(b.core)) {
        return false;
    }
    return memcmp(
               core_ptr_raw(a.core),
               core_ptr_raw(b.core),
               a.len * core_ptr_size(a.core)
           )
        == 0;
}

/*========== Slice Operations (Continued) =========================================*/

// Create slice from many-item pointer
force_inline Slice many_ptr_slice(MPtr ptr, usize start, usize end) {
    assert(start <= end);
    anyptr base      = core_ptr_raw(ptr.core);
    usize  elem_size = core_ptr_size(ptr.core);
    return slice_create(
        (char*)base + start * elem_size,
        end - start,
        elem_size,
        core_ptr_align(ptr.core),
        false
    );
}

// Create slice from array pointer
force_inline Slice array_ptr_slice(ArrPtr ptr, usize start, usize end) {
    assert(start <= end && end <= ptr.len);
    anyptr base      = core_ptr_raw(ptr.core);
    usize  elem_size = core_ptr_size(ptr.core);
    return slice_create(
        (char*)base + start * elem_size,
        end - start,
        elem_size,
        core_ptr_align(ptr.core),
        false
    );
}

// Create slice from single-item pointer
force_inline Slice ptr_slice(SPtr ptr, usize start, usize end) {
    assert(start <= end);
    anyptr base      = core_ptr_raw(ptr.core);
    usize  elem_size = core_ptr_size(ptr.core);
    return slice_create(
        (char*)base + start * elem_size,
        end - start,
        elem_size,
        core_ptr_align(ptr.core),
        false
    );
}

#endif // ZIG_SLICE_INCLUDED


// ptr_macros.h
#ifndef PTR_MACROS_H
#define PTR_MACROS_H

#include "zig_ptr.h"

/*========== Type Information Helpers =====================================*/

// Get sizeof type, with special handling for arrays
#define TYPE_SIZE(T) _Generic((T*)0, \
    char*:   sizeof(char), \
    void*:   sizeof(void), \
    default: sizeof(T))

// Get alignof type, with special handling for arrays
#define TYPE_ALIGN(T) _Generic((T*)0, \
    char*:   _Alignof(char), \
    void*:   _Alignof(void), \
    default: _Alignof(T))

/*========== Safe Pointer Creation =======================================*/

// Create a single-item pointer with type safety
#define Ptr_new(T, ptr) \
    single_item_ptr(ptr, TYPE_SIZE(T), TYPE_ALIGN(T))

// Create a many-item pointer with type safety
#define ManyPtr_new(T, ptr) \
    many_item_ptr(ptr, TYPE_SIZE(T), TYPE_ALIGN(T))

// Create an array pointer with type safety
#define ArrayPtr_new(T, ptr, count) \
    array_ptr(ptr, count, TYPE_SIZE(T), TYPE_ALIGN(T))

/*========== Safe Slice Creation ========================================*/

// Create a slice with type safety
#define Slice_new(T, ptr, count) \
    slice_create(ptr, count, TYPE_SIZE(T), TYPE_ALIGN(T), false)

// Create a const slice with type safety
#define Slice_newConst(T, ptr, count) \
    slice_create(ptr, count, TYPE_SIZE(T), TYPE_ALIGN(T), true)

// Create a sentinel-terminated slice with type safety
#define Slice_newSentinel(T, ptr, count, sentinel_val) \
    ({ \
        __typeof__(sentinel_val) _sentinel = sentinel_val; \
        sentinel_slice_create( \
            ptr, count, \
            TYPE_SIZE(T), TYPE_ALIGN(T), \
            &_sentinel, sizeof(_sentinel), \
            false \
        ); \
    })

/*========== Safe Array List Creation ===================================*/

// Create an ArrayList with type safety
#define ArrayList_new(T, allocator) \
    arraylist_init(allocator, TYPE_SIZE(T), TYPE_ALIGN(T))

// Create an ArrayList with initial capacity
#define ArrayList_newCapacity(T, allocator, capacity) \
    arraylist_init_capacity(allocator, TYPE_SIZE(T), TYPE_ALIGN(T), capacity)

/*========== Safe Element Access ========================================*/

// Get element from slice with type safety
#define Slice_get(T, slice, index) \
    (*(T*)slice_ptr_at(slice, index))

// Set element in slice with type safety
#define Slice_set(T, slice, index, value) \
    do { \
        T _tmp = value; \
        *(T*)slice_ptr_at(slice, index) = _tmp; \
    } while(0)

// Get element from ArrayList with type safety
#define ArrayList_get(T, list, index) \
    Slice_get(T, (list)->items, index)

// Set element in ArrayList with type safety
#define ArrayList_set(T, list, index, value) \
    Slice_set(T, (list)->items, index, value)

/*========== Example Usage =============================================*/

#if 0  // Example usage code (not compiled)

void example_usage(void) {
    int numbers[] = {1, 2, 3, 4, 5};

    // Create pointers
    SingleItemPtr ptr = Ptr_new(int, &numbers[0]);
    ManyItemPtr many = ManyPtr_new(int, numbers);
    ArrayPtr arr = ArrayPtr_new(int, numbers, 5);

    // Create slices
    Slice slice = Slice_new(int, numbers, 5);
    Slice const_slice = Slice_newConst(int, numbers, 5);

    // Create sentinel slice
    char str[] = "hello\0";
    SentinelSlice str_slice = Slice_newSentinel(char, str, 5, '\0');

    // Create ArrayList
    ArrayList* list = ArrayList_new(int, test_allocator);
    ArrayList* list_cap = ArrayList_newCapacity(int, test_allocator, 10);

    // Access elements
    int value = Slice_get(int, slice, 0);
    Slice_set(int, slice, 0, 42);

    int list_value = ArrayList_get(int, list, 0);
    ArrayList_set(int, list, 0, 42);
}

#endif

#endif // PTR_MACROS_H

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/core/ptr.h"
#include "dh/mem.h"
#include "dh/debug/assert.h"
#include "dh/claim/assert_static.h"
#include "dh/mem/Allocator.h"

/*========== Vec Definition =================================================*/

/* Vec structure representing a dynamic array */
typedef struct Vec {
    Slice          buffer;    // Slice representing the allocated memory
    usize          length;    // Number of elements currently in the Vec
    usize          capacity;  // Total capacity of the Vec
    mem_Allocator* allocator; // Allocator for memory management
} Vec;

/*========== Vec Functions ==================================================*/

/* Initialize a new Vec */
force_inline Vec Vec__init(mem_Allocator* allocator, usize elem_size, usize elem_align) {
    Vec self = {
        .buffer    = Slice_undefined,
        .length    = 0,
        .capacity  = 4,
        .allocator = allocator
    };

    // Allocate initial buffer
    Res_Slice res_slice = mem_Allocator_allocSlice(allocator, elem_size, elem_align, self.capacity);
    if (Res_isErr(res_slice)) {
        debug_assert_fmt(false, "Failed to allocate initial Vec buffer");
        // Handle allocation failure as needed
        self.buffer   = Slice_undefined;
        self.capacity = 0;
    } else {
        self.buffer = Res_unwrap(Res_Slice, res_slice);
    }

    return self;
}

/* Free the resources held by the Vec */
force_inline void Vec_fini(Vec* vec) {
    if (vec->buffer.len_ > 0) {
        mem_Allocator_freeSlice(vec->allocator, vec->buffer);
        vec->buffer = Slice_undefined;
    }
    vec->length   = 0;
    vec->capacity = 0;
}

/* Get the current length of the Vec */
force_inline usize Vec_length(const Vec* vec) {
    return vec->length;
}

/* Get the current capacity of the Vec */
force_inline usize Vec_capacity(const Vec* vec) {
    return vec->capacity;
}

/* Internal function to resize the Vec's capacity */
force_inline bool Vec_resize(Vec* vec, usize new_capacity) {
    // Allocate new Slice with the new capacity
    Res_Slice res_new_slice = mem_Allocator_allocSlice(
        vec->allocator,
        Slice_size(vec->buffer),
        Slice_align(vec->buffer),
        new_capacity
    );
    if (Res_isErr(res_new_slice)) {
        return false; // Allocation failed
    }

    Slice new_slice = Res_unwrap(Res_Slice, res_new_slice);

    // Calculate number of elements to copy
    usize copy_count = vec->length < new_capacity ? vec->length : new_capacity;

    // Copy existing data to the new buffer
    memcpy(Slice_addr(new_slice), Slice_addr(vec->buffer), copy_count * Slice_size(vec->buffer));

    // Free the old buffer
    mem_Allocator_freeSlice(vec->allocator, vec->buffer);

    // Update Vec's buffer and capacity
    vec->buffer   = new_slice;
    vec->capacity = new_capacity;
    vec->length   = copy_count;

    return true;
}

/* Push a new element to the Vec */
force_inline bool Vec_push(Vec* vec, const anyptr element) {
    if (vec->length == vec->capacity) {
        // Need to resize (e.g., double the capacity)
        usize new_capacity = vec->capacity * 2;
        if (!Vec_resize(vec, new_capacity)) {
            return false; // Resize failed
        }
    }

    // Calculate the address to insert the new element
    anyptr dest = (u8*)Slice_addr(vec->buffer) + (vec->length * Slice_size(vec->buffer));
    memcpy(dest, element, Slice_size(vec->buffer));
    vec->length += 1;

    return true;
}

/* Pop the last element from the Vec */
force_inline bool Vec_pop(Vec* vec, anyptr const out_element) {
    if (vec->length == 0) {
        return false; // Vec is empty
    }

    vec->length -= 1;
    anyptr src = (u8*)Slice_addr(vec->buffer) + (vec->length * Slice_size(vec->buffer));
    if (out_element != null) {
        memcpy(out_element, src, Slice_size(vec->buffer));
    }

    return true;
}

/* Get a pointer to the element at the specified index */
force_inline anyptr Vec_get(const Vec* vec, usize index) {
    debug_assert_fmt(index < vec->length, "Vec index out of bounds");

    return (u8*)Slice_addr(vec->buffer) + (index * Slice_size(vec->buffer));
}

/* Set the element at the specified index */
force_inline bool Vec_set(Vec* vec, usize index, const anyptr element) {
    debug_assert_fmt(index < vec->length, "Vec index out of bounds");

    anyptr dest = (u8*)Slice_addr(vec->buffer) + (index * Slice_size(vec->buffer));
    memcpy(dest, element, Slice_size(vec->buffer));
    return true;
}

/* Reserve additional capacity in the Vec */
force_inline bool Vec_reserve(Vec* vec, usize additional_capacity) {
    usize required_capacity = vec->length + additional_capacity;
    if (required_capacity <= vec->capacity) {
        return true; // No need to reserve
    }

    // Determine the new capacity (e.g., double until it's sufficient)
    usize new_capacity = vec->capacity;
    while (new_capacity < required_capacity) {
        new_capacity *= 2;
    }

    return Vec_resize(vec, new_capacity);
}

/* Clear all elements from the Vec */
force_inline void Vec_clear(Vec* vec) {
    vec->length = 0;
}

/*========== Example Usage ====================================================*/

#ifdef VEC_EXAMPLE

#include <stdio.h>

/* Example element type */
typedef struct {
    int   id;
    float value;
} Element;

int main() {
    // Assume a default allocator is available
    mem_Allocator default_allocator = get_default_allocator(); // Implement as needed

    // Initialize a Vec for Element type
    Vec vec = Vec_init(&default_allocator, sizeof(Element), alignof(Element));

    // Push elements
    for (int i = 0; i < 10; ++i) {
        Element elem = { .id = i, .value = i * 1.5f };
        if (!Vec_push(&vec, &elem)) {
            printf("Failed to push element %d\n", i);
        }
    }

    // Access and modify elements
    for (usize i = 0; i < Vec_length(&vec); ++i) {
        Element* elem = Vec_get(&vec, i);
        printf("Element %zu: id=%d, value=%.2f\n", i, elem->id, elem->value);
        // Modify the element
        elem->value += 10.0f;
    }

    // Pop an element
    Element popped;
    if (Vec_pop(&vec, &popped)) {
        printf("Popped Element: id=%d, value=%.2f\n", popped.id, popped.value);
    }

    // Clear the Vec
    Vec_clear(&vec);
    printf("Vec cleared. Length: %zu\n", Vec_length(&vec));

    // Free the Vec resources
    Vec_free(&vec);

    return 0;
}

#endif // VEC_EXAMPLE

/*========== Additional Helper Functions =====================================*/

/* Function to retrieve the default allocator (implementation depends on the system) */
force_inline mem_Allocator get_default_allocator(void) {
    // Example implementation using a simple allocator
    // Replace with your allocator's initialization as needed

    // Define a simple allocator that uses malloc and free
    extern Res_Mptr simple_alloc(Sptr self, PtrBase_MetaData meta);
    extern bool     simple_resize(Sptr self, Mptr ptr, usize new_len);
    extern void     simple_free(Sptr self, Mptr ptr);

    static const mem_Allocator_VTable simple_allocator_vtable = {
        .alloc  = simple_alloc,
        .resize = simple_resize,
        .free   = simple_free
    };

    static const mem_Allocator simple_allocator = {
        .ctx_ = Sptr_undefined, // Assign appropriately
        .vt_  = &simple_allocator_vtable
    };

    return simple_allocator;
}

/*========== Simple Allocator Implementation (Example) =======================*/

/* Example implementations for a simple allocator using malloc, realloc, and free */

// Res_Mptr simple_alloc(Sptr self, PtrBase_MetaData meta) {
//     (void)self; // Unused in this simple example
//     anyptr ptr = aligned_alloc((1 << meta.log2_align), meta.type_size);
//     if (ptr == null) {
//         return Res_err_Mptr("Allocation failed");
//     }
//     PtrBase base = PtrBase_make(ptr, meta.type_size, meta.log2_align);
//     return Res_ok_Mptr((Mptr){ .Base_ = base });
// }

// bool simple_resize(Sptr self, Mptr ptr, usize new_len) {
//     (void)self; // Unused in this simple example
//     anyptr resized_ptr = realloc(PtrBase_addr(ptr.Base_), new_len);
//     if (resized_ptr == null) {
//         return false; // Reallocation failed
//     }
//     ptr.Base_ = PtrBase_make(resized_ptr, new_len, ptr.Base_.meta.log2_align);
//     return true;
// }

// void simple_free(Sptr self, Mptr ptr) {
//     (void)self; // Unused in this simple example
//     free(PtrBase_addr(ptr.Base_));
// }

/*========== Assertion Implementations =======================================*/

/* Implement the required assertion functions if not already defined */
// #ifndef debug_assert_fmt
// #include <stdio.h>
// #include <stdlib.h>
// #define debug_assert_fmt(cond, msg, ...)                                   \
//     do {                                                                   \
//         if (!(cond)) {                                                     \
//             fprintf(stderr, "Assertion failed: " msg "\n", ##__VA_ARGS__); \
//             abort();                                                       \
//         }                                                                  \
//     } while (0)
// #endif

// #ifndef claim_assert_static_msg
// #define claim_assert_static_msg(cond, msg) _Static_assert(cond, msg)
// #endif

/*========== Conversion and Undefined Functions ==============================*/

/* Define conversion and undefined functions as needed */

// force_inline PtrBase Slice_makeBase(anyptr addr, usize size, usize align) {
//     PtrBase_MetaData meta = PtrBase_makeTypeInfo(size, align);
//     return PtrBase_fromAddr(addr, meta);
// }

// force_inline Slice Slice_fromBuffer(anyptr addr, usize len, usize size, usize align) {
//     PtrBase base = Slice_makeBase(addr, size * len, align);
//     return Slice__makeFromBase(base, len);
// }

/*========== Additional Macros and Definitions ===============================*/

/* Define any additional macros or helper definitions as needed */

/*========== End of Vec Implementation =======================================*/

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Vec.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-08 (date of creation)
 * @updated 2024-12-11 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  Vec
 *
 * @brief   Dynamic array implementation using mem_Allocator
 * @details Implements a resizable array (vector) data structure that automatically
 *          manages memory using the provided allocator interface
 */

#ifndef DS_VEC_INCLUDED
#define DS_VEC_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core/prim.h"
#include "dh/core/ptr.h"
#include "dh/core/Err.h"
#include "dh/core/Res.h"
#include "dh/core/Opt.h"
#include "dh/debug/assert.h"
#include "dh/mem/Allocator.h"

/*========== Vec Definition =================================================*/

// Vec structure definition
typedef struct Vec {
    Slice          data;      // Underlying slice for storage
    usize          len;       // Current number of elements
    mem_Allocator* allocator; // Memory allocator reference
} Vec;

// Type declarations
#define Vec(THint) typedef Vec
Res(Vec, Err) Res_Vec;

/*========== Core Functions ================================================*/

// Initialize Vec with default capacity
#define Vec_init(T, allocator) Vec__init(allocator, PtrBase_typeInfo(T))
force_inline Vec Vec__init(mem_Allocator* allocator, PtrBase_MetaData type_info);

// Initialize Vec with specific capacity
#define Vec_initCapacity(T, allocator, capacity) Vec__initCapacity(allocator, capacity, PtrBase_typeInfo(T))
force_inline Res_Vec Vec__initCapacity(mem_Allocator* allocator, usize capacity, PtrBase_MetaData type_info);

// Free Vec's memory
force_inline void Vec_fini(Vec* self);

// Get current length
force_inline usize Vec_len(const Vec* self);
// Get current capacity
force_inline usize Vec_capacity(const Vec* self);
// Check if Vec is empty
force_inline bool  Vec_isEmpty(const Vec* self);

/*========== Access Functions ==============================================*/

// Get item at index
#define Vec_at(T, self, idx) (*(T*)Vec__at(self, idx))
force_inline anyptr Vec__at(const Vec* self, usize idx);

// Get first item as optional
force_inline Opt_anyptr Vec_firstOrNull(const Vec* self);
// Get last item as optional
force_inline Opt_anyptr Vec_lastOrNull(const Vec* self);

/*========== Growth Functions ============================================*/

// Calculate growth capacity
force_inline usize    Vec__nextCapacity(usize current_capacity);
// Ensure capacity for n more items
force_inline Res_Void Vec_ensureUnusedCapacity(Vec* self, usize additional);

/*========== Modification Functions =======================================*/

// Append item
force_inline Res_Void Vec_append(Vec* self, const anyptr item);

// Pop last item
#define Vec_pop(T, self) (*(T*)Vec__pop(self))
force_inline anyptr Vec__pop(Vec* self);

// Pop last item as optional
force_inline Opt_anyptr Vec_popOrNull(Vec* self);

// Insert item at index
#define Vec_insert(T, self, idx, item) Vec__insert(self, idx, &(item))
force_inline Res_Void Vec__insert(Vec* self, usize idx, const anyptr item);

// Remove item at index
force_inline void Vec_remove(Vec* self, usize idx);
// Clear all items
force_inline void Vec_clear(Vec* self);

/*========== Slice Access ================================================*/

// Get slice of full capacity
force_inline Slice Vec_slice(const Vec* self);
// Get slice of current items
force_inline Slice Vec_items(const Vec* self);

/*========== Implementation =================================================*/

force_inline Vec Vec__init(mem_Allocator* allocator, PtrBase_MetaData type_info) {
    debug_assert_fmt(allocator != null, "Allocator cannot be null");
    return (Vec){
        .data = {
            .Base = {
                .addr = 0,
                .meta = type_info },
            .len = 0,
        },
        .len       = 0,
        .allocator = allocator
    };
}

force_inline Res_Vec Vec__initCapacity(mem_Allocator* allocator, usize capacity, PtrBase_MetaData type_info) {
    debug_assert_fmt(allocator != null, "Allocator cannot be null");

    if (capacity == 0) {
        return Res_ok(Res_Vec, Vec__init(allocator, type_info));
    }

    Res_Slice slice = mem_Allocator_allocSlice(
        allocator,
        type_info.type_size,
        PtrBase__calcAlignLog2Order(type_info.log2_align),
        capacity
    );

    if (Res_isErr(slice)) {
        return Res_err(Res_Vec, Res_unwrapErr(Res_Slice, slice));
    }

    return Res_ok(Res_Vec, (Vec){ .data = Res_unwrap(Res_Slice, slice), .len = 0, .allocator = allocator });
}

force_inline void Vec_fini(Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    if (!Slice_isUndefined(self->data)) {
        mem_Allocator_freeSlice(self->allocator, self->data);
        self->len = 0;
    }
}

force_inline usize Vec_len(const Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    return self->len;
}

force_inline usize Vec_capacity(const Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    return Slice_len(self->data);
}

force_inline bool Vec_isEmpty(const Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    return self->len == 0;
}

force_inline anyptr Vec__at(const Vec* self, usize idx) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    debug_assert_fmt(idx < self->len, "Index out of bounds");
    return Slice__getAtAddr(self->data, idx, Slice_size(self->data));
}

force_inline Opt_anyptr Vec_firstOrNull(const Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    if (Vec_isEmpty(self)) {
        return Opt_none(Opt_anyptr);
    }
    return Opt_some(Opt_anyptr, Vec__at(self, 0));
}

force_inline Opt_anyptr Vec_lastOrNull(const Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    if (Vec_isEmpty(self)) {
        return Opt_none(Opt_anyptr);
    }
    return Opt_some(Opt_anyptr, Vec__at(self, self->len - 1));
}

force_inline usize Vec__nextCapacity(usize current_capacity) {
    if (current_capacity == 0) {
        return 4;
    }
    return current_capacity + (current_capacity / 2); // Grow by ~1.5x
}

force_inline Res_Void Vec_ensureUnusedCapacity(Vec* self, usize additional) {
    debug_assert_fmt(self != null, "Vec cannot be null");

    const usize required_cap = self->len + additional;
    if (required_cap <= Vec_capacity(self)) {
        return Res_ok(Res_Void, (Void){ 0 });
    }

    const usize new_cap = Vec__nextCapacity(required_cap);

    // Try resize in place first
    if (!Slice_isUndefined(self->data)) {
        if (mem_Allocator_resizeSlice(self->allocator, &self->data, new_cap)) {
            return Res_ok(Res_Void, (Void){ 0 });
        }
    }

    // Allocate new buffer
    Res_Slice new_slice = mem_Allocator_allocSlice(
        self->allocator,
        Slice_size(self->data),
        Slice_align(self->data),
        new_cap
    );

    if (Res_isErr(new_slice)) {
        return Res_err(Res_Void, Res_unwrapErr(Res_Slice, new_slice));
    }

    // Copy existing data if any
    if (!Slice_isUndefined(self->data)) {
        memcpy(
            Slice_addr(Res_unwrap(Res_Slice, new_slice)),
            Slice_addr(self->data),
            self->len * Slice_size(self->data)
        );
        mem_Allocator_freeSlice(self->allocator, self->data);
    }

    self->data = Res_unwrap(Res_Slice, new_slice);
    return Res_ok(Res_Void, (Void){ 0 });
}

force_inline Res_Void Vec_append(Vec* self, const anyptr item) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    debug_assert_fmt(item != null, "Item cannot be null");

    Res_Void ensure_res = Vec_ensureUnusedCapacity(self, 1);
    if (Res_isErr(ensure_res)) {
        return ensure_res;
    }

    memcpy(
        ((u8*)Slice_addr(self->data)) + (self->len * Slice_size(self->data)),
        item,
        Slice_size(self->data)
    );
    self->len += 1;

    return Res_ok(Res_Void, (Void){ 0 });
}

force_inline anyptr Vec__pop(Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    debug_assert_fmt(!Vec_isEmpty(self), "Vec is empty");
    self->len -= 1;
    return (u8*)Slice_addr(self->data) + (self->len * Slice_size(self->data));
}

force_inline Opt_anyptr Vec_popOrNull(Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    if (Vec_isEmpty(self)) {
        return Opt_none(Opt_anyptr);
    }

    self->len -= 1;
    return Opt_some(
        Opt_anyptr,
        (u8*)Slice_addr(self->data) + (self->len * Slice_size(self->data))
    );
}

force_inline Res_Void Vec__insert(Vec* self, usize idx, const anyptr item) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    debug_assert_fmt(item != null, "Item cannot be null");
    debug_assert_fmt(idx <= self->len, "Index out of bounds");

    Res_Void ensure_res = Vec_ensureUnusedCapacity(self, 1);
    if (Res_isErr(ensure_res)) {
        return ensure_res;
    }

    if (idx < self->len) {
        memmove(
            (u8*)Slice_addr(self->data) + ((idx + 1) * Slice_size(self->data)),
            (u8*)Slice_addr(self->data) + (idx * Slice_size(self->data)),
            (self->len - idx) * Slice_size(self->data)
        );
    }

    memcpy(
        (u8*)Slice_addr(self->data) + (idx * Slice_size(self->data)),
        item,
        Slice_size(self->data)
    );
    self->len += 1;

    return Res_ok(Res_Void, (Void){ 0 });
}

force_inline void Vec_remove(Vec* self, usize idx) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    debug_assert_fmt(idx < self->len, "Index out of bounds");

    if (idx < self->len - 1) {
        memmove(
            (u8*)Slice_addr(self->data) + (idx * Slice_size(self->data)),
            (u8*)Slice_addr(self->data) + ((idx + 1) * Slice_size(self->data)),
            (self->len - idx - 1) * Slice_size(self->data)
        );
    }
    self->len -= 1;
}

force_inline void Vec_clear(Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    self->len = 0;
}

force_inline Slice Vec_slice(const Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    return self->data;
}

force_inline Slice Vec_items(const Vec* self) {
    debug_assert_fmt(self != null, "Vec cannot be null");
    if (Slice_isUndefined(self->data)) {
        return Slice_undefined(Void);
    }
    return Slice_prefix(self->data, self->len);
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DS_VEC_INCLUDED */

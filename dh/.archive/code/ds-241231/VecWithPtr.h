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
    Slice              data;      // Underlying slice for storage
    usize              len;       // Current number of elements
    Sptr_mem_Allocator allocator; // Memory allocator reference
} Vec;
typedef Sptr(Vec) Sptr_Vec;
Res(Vec, Err) Res_Vec;

// Type declarations
#define Vec(T) Vec

/*========== Core Functions ================================================*/

// Initialize Vec with default capacity
force_inline Vec     Vec_init(TypeInfo type_info, Sptr_mem_Allocator allocator);
// Initialize Vec with specific capacity
force_inline Res_Vec Vec_initCapacity(TypeInfo type_info, Sptr_mem_Allocator allocator, usize capacity);

// Free Vec's memory
force_inline void Vec_fini(Sptr_Vec self);

// Get current length
force_inline usize Vec_length(const Sptr_Vec self);
// Get current capacity
force_inline usize Vec_capacity(const Sptr_Vec self);
// Check if Vec is empty
force_inline bool  Vec_isEmpty(const Sptr_Vec self);
// Check if Vec is full
force_inline bool  Vec_isFull(const Sptr_Vec self);

/*========== Access Functions ==============================================*/

// Get item at index
force_inline Sptr Vec_at(const Sptr_Vec self, usize index);

// Get first item as optional
force_inline Opt_Sptr Vec_firstOrNull(const Sptr_Vec self);
// Get last item as optional
force_inline Opt_Sptr Vec_lastOrNull(const Sptr_Vec self);

/*========== Growth Functions ============================================*/

// Calculate growth capacity
force_inline usize    Vec__nextCapacity(usize current_capacity);
// Ensure capacity for n more items
force_inline Res_Void Vec_ensureUnusedCapacity(Sptr_Vec self, usize additional);

/*========== Modification Functions =======================================*/

// Append item
force_inline Res_Void Vec_append(Sptr_Vec self, Sptr item);
// Insert item at index
force_inline Res_Void Vec_insert(Sptr_Vec self, usize index, const Sptr item);

// Pop last item
force_inline Sptr     Vec_pop(Sptr_Vec self);
// Pop last item as optional
force_inline Opt_Sptr Vec_popOrNull(Sptr_Vec self);

// Remove item at index
force_inline void Vec_remove(Sptr_Vec self, usize index);
// Clear all items
force_inline void Vec_clear(Sptr_Vec self);

/*========== Slice Access ================================================*/

// Get slice of full capacity
force_inline Slice Vec_slice(const Sptr_Vec self);
// Get slice of current items
force_inline Slice Vec_items(const Sptr_Vec self);

/*========== Implementation =================================================*/

force_inline Vec Vec_init(TypeInfo type_info, Sptr_mem_Allocator allocator) {
    return (Vec){
        .data = {
            .Base = {
                .addr = 0,
                .meta = type_info,
            },
        },
        .len       = 0,
        .allocator = allocator
    };
}

force_inline Res_Vec Vec_initCapacity(TypeInfo type_info, Sptr_mem_Allocator allocator, usize capacity) {
    if (capacity == 0) {
        return Res_ok(Res_Vec, Vec_init(type_info, allocator));
    }

    Res_Slice slice = mem_Allocator_allocSlice(
        allocator.addr,
        type_info.type_size,
        PtrBase__calcAlignLog2Order(type_info.log2_align),
        capacity
    );

    if (Res_isErr(slice)) {
        return Res_err(Res_Vec, Res_unwrapErr(Res_Slice, slice));
    }

    return Res_ok(
        Res_Vec,
        (Vec){
            .data      = Res_unwrap(Res_Slice, slice),
            .len       = 0,
            .allocator = allocator,
        }
    );
}

force_inline void Vec_fini(Sptr_Vec self) {
    if (!Slice_isUndefined(self.addr->data)) {
        mem_Allocator_freeSlice(self.addr->allocator.addr, self.addr->data);
        self.addr->data = Slice_undefined;
        self.addr->len  = 0;
    }
}

force_inline usize Vec_len(const Sptr_Vec self) {
    return self.addr->len;
}

force_inline usize Vec_capacity(const Sptr_Vec self) {
    return Slice_len(self.addr->data);
}

force_inline bool Vec_isEmpty(const Sptr_Vec self) {
    return self.addr->len == 0;
}

force_inline Sptr Vec_at(const Sptr_Vec self, usize index) {
    debug_assert_fmt(index < self.addr->len, "Index out of bounds");
    return Mptr_atSptr(Slice_atMptr(self.addr->data, index), 0);
}

force_inline Opt_Sptr Vec_firstOrNull(const Sptr_Vec self) {
    if (Vec_isEmpty(self)) {
        return Opt_null(Opt_Sptr);
    }
    return Opt_some(Opt_Sptr, Vec_at(self, 0));
}

force_inline Opt_Sptr Vec_lastOrNull(const Sptr_Vec self) {
    if (Vec_isEmpty(self)) {
        return Opt_null(Opt_Sptr);
    }
    return Opt_some(Opt_Sptr, Vec_at(self, self.addr->len - 1));
}

force_inline usize Vec__nextCapacity(usize current_capacity) {
    if (current_capacity == 0) {
        return 4;
    }
    return current_capacity + (current_capacity / 2); // Grow by ~1.5x
}

force_inline Res_Void Vec_ensureUnusedCapacity(Sptr_Vec self, usize additional) {
    const usize required_cap = self.addr->len + additional;
    if (required_cap <= Vec_capacity(self)) {
        return Res_ok(Res_Void, makeCleared(Void));
    }

    const usize new_cap = Vec__nextCapacity(required_cap);

    // Try resize in place first
    if (!Slice_isUndefined(self.addr->data)) {
        if (mem_Allocator_resizeSlice(self.addr->allocator.addr, &self.addr->data, new_cap)) {
            return Res_ok(Res_Void, makeCleared(Void));
        }
    }

    // Allocate new buffer
    Res_Slice new_slice = mem_Allocator_allocSlice(
        self.addr->allocator.addr,
        Slice_size(self.addr->data),
        Slice_align(self.addr->data),
        new_cap
    );

    if (Res_isErr(new_slice)) {
        return Res_err(Res_Void, Res_unwrapErr(Res_Slice, new_slice));
    }

    // Copy existing data if any
    if (!Slice_isUndefined(self.addr->data)) {
        memcpy(
            Slice_addr(Res_unwrap(Res_Slice, new_slice)),
            Slice_addr(self.addr->data),
            self.addr->len * Slice_size(self.addr->data)
        );
        mem_Allocator_freeSlice(self.addr->allocator.addr, self.addr->data);
    }

    self.addr->data = Res_unwrap(Res_Slice, new_slice);
    return Res_ok(Res_Void, makeCleared(Void));
}

force_inline Res_Void Vec_append(Sptr_Vec self, const Sptr item) {
    Res_Void ensure_res = Vec_ensureUnusedCapacity(self, 1);
    if (Res_isErr(ensure_res)) {
        return ensure_res;
    }

    memcpy(
        (u8*)Slice_addr(self.addr->data) + (self.addr->len * Slice_size(self.addr->data)),
        Sptr_rawCast(u8*, item),
        Slice_size(self.addr->data)
    );
    self.addr->len += 1;

    return Res_ok(Res_Void, makeCleared(Void));
}

force_inline Sptr Vec_pop(Sptr_Vec self) {
    debug_assert_fmt(!Vec_isEmpty(self), "Vec is empty");
    self.addr->len -= 1;
    return Mptr_atSptr(Slice_atMptr(self.addr->data, self.addr->len), 0);
}

force_inline Opt_Sptr Vec_popOrNull(Sptr_Vec self) {
    if (Vec_isEmpty(self)) {
        return Opt_null(Opt_Sptr);
    }

    self.addr->len -= 1;
    return Opt_some(
        Opt_Sptr,
        Mptr_atSptr(Slice_atMptr(self.addr->data, self.addr->len), 0)
    );
}

force_inline Res_Void Vec_insert(Sptr_Vec self, usize index, const Sptr item) {
    debug_assert_fmt(index <= self.addr->len, "Index out of bounds");

    Res_Void ensure_res = Vec_ensureUnusedCapacity(self, 1);
    if (Res_isErr(ensure_res)) {
        return ensure_res;
    }

    if (index < self.addr->len) {
        memmove(
            (u8*)Slice_addr(self.addr->data) + ((index + 1) * Slice_size(self.addr->data)),
            (u8*)Slice_addr(self.addr->data) + (index * Slice_size(self.addr->data)),
            (self.addr->len - index) * Slice_size(self.addr->data)
        );
    }

    memcpy(
        (u8*)Slice_addr(self.addr->data) + (index * Slice_size(self.addr->data)),
        Sptr_rawCast(u8*, item),
        Slice_size(self.addr->data)
    );
    self.addr->len += 1;

    return Res_ok(Res_Void, makeCleared(Void));
}

force_inline void Vec_remove(Sptr_Vec self, usize index) {
    debug_assert_fmt(index < self.addr->len, "Index out of bounds");

    if (index < self.addr->len - 1) {
        memmove(
            (u8*)Slice_addr(self.addr->data) + (index * Slice_size(self.addr->data)),
            (u8*)Slice_addr(self.addr->data) + ((index + 1) * Slice_size(self.addr->data)),
            (self.addr->len - index - 1) * Slice_size(self.addr->data)
        );
    }
    self.addr->len -= 1;
}

force_inline void Vec_clear(Sptr_Vec self) {
    self.addr->len = 0;
}

force_inline Slice Vec_slice(const Sptr_Vec self) {
    return self.addr->data;
}

force_inline Slice Vec_items(const Sptr_Vec self) {
    if (Slice_isUndefined(self.addr->data)) {
        return Slice_undefined(Void);
    }
    return Slice_prefix(self.addr->data, self.addr->len);
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DS_VEC_INCLUDED */

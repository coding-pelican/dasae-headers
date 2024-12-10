/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Vec.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-08 (date of creation)
 * @updated 2024-12-08 (date of last update)
 * @version v0.1.0
 * @ingroup dasae-headers(dh)/container
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
#include "dh/core/Opt.h"
#include "dh/core/Res.h"
#include "dh/core/Err.h"
#include "dh/debug/assert.h"
#include "dh/mem/Allocator.h"

/*========== Vec Definition ===========================================*/

// Vec structure definition
typedef struct Vec {
    mem_Allocator* allocator_; // Memory allocator
    Slice          items_;     // Actual data storage
    usize          len_;
} Vec;
#define Vec(T) typedef Vec
Res(Vec, Err) Res_Vec;

// Initialize macros for type checking
#define Vec_init(T, allocator) \
    Vec_initWithCapacity(T, allocator, 0)

#define Vec_initWithCapacity(T, allocator, initial_capacity) \
    Vec__initWithCapacity(sizeof(T), alignof(T), allocator, initial_capacity)

/*========== Core Functions ================================================*/

// Initialize an Vec with given capacity
force_inline Res_Vec Vec__initWithCapacity(
    usize          elem_size,
    usize          elem_align,
    mem_Allocator* allocator,
    usize          initial_capacity
) {
    Vec self = {
        .allocator_ = allocator,
        .items_     = { 0 },
        .len_       = 0
    };

    if (initial_capacity > 0) {
        const Res_Slice slice = mem_Allocator_allocSlice(
            allocator,
            elem_size,
            elem_align,
            initial_capacity
        );

        if (Res_isErr(slice)) {
            return Res_err(Res_Vec, Res_unwrapErr(Res_Slice, slice));
        }

        self.items_ = Res_unwrap(Res_Slice, slice);
        self.len_   = 0;
    }

    return Res_ok(Res_Vec, self);
}

// Free the Vec's memory
force_inline void Vec_fini(Vec* self) {
    debug_assert_nonnull(self);
    if (!Slice_isZero(self->items_)) {
        mem_Allocator_freeSlice(self->allocator_, &self->items_);
    }
    *self = (Vec){ 0 };
}

// Get current length
force_inline usize Vec_len(const Vec* self) {
    debug_assert_nonnull(self);
    return self->len_;
}

// Get current capacity
force_inline usize Vec_capacity(const Vec* self) {
    debug_assert_nonnull(self);
    return Slice_len(self->items_);
}

// Access item at index
#define Vec_at(T, self, idx) \
    (*(T*)Vec__at(self, idx))

force_inline anyptr Vec__at(const Vec* self, usize idx) {
    debug_assert_nonnull(self);
    debug_assert_true(idx < self->len_);
    return Mptr_raw(Mptr_add(Slice_ptr(self->items_), (isize)idx));
}

/*========== Growth Functions =============================================*/

// Helper to calculate new capacity
force_inline usize Vec__calcCapacityIncrease(usize capacity) {
    if (capacity == 0) {
        return 1;
    }
    // Grow by ~1.5x
    return capacity + (capacity / 2) + 1;
}

// Ensure capacity for additional items
force_inline Res_Void Vec_ensureTotalCapacity(
    Vec*  self,
    usize new_capacity
) {
    debug_assert_nonnull(self);

    if (new_capacity <= self->len_) {
        return Res_ok(Res_Void, (Void){ 0 });
    }

    // Try to resize in place first
    if (!Slice_isZero(self->items_)) {
        if (mem_Allocator_resizeSlice(self->allocator_, &self->items_, new_capacity)) {
            debug_assert(Slice_len(self->items_) == new_capacity);
            return Res_ok(Res_Void, (Void){ 0 });
        }
    }

    // Need to allocate new buffer
    const Res_Slice new_memory = mem_Allocator_allocSlice(
        self->allocator_,
        Slice_size(self->items_),
        Slice_align(self->items_),
        new_capacity
    );

    if (Res_isErr(new_memory)) {
        return Res_err(Res_Void, Res_unwrapErr(Res_Slice, new_memory));
    }

    const Slice new_slice = Res_unwrap(Res_Slice, new_memory);

    // Copy old data if any
    if (!Slice_isZero(self->items_)) {
        memcpy(
            Slice_raw(new_slice),
            Slice_raw(self->items_),
            Vec_len(self) * Slice_size(self->items_)
        );
        mem_Allocator_freeSlice(self->allocator_, &self->items_);
    }

    self->items_ = new_slice;
    return Res_ok(Res_Void, (Void){ 0 });
}

force_inline Res_Void Vec_ensureUnusedCapacity(
    Vec*  self,
    usize additional_count
) {
    debug_assert_nonnull(self);
    const usize minimum_capacity = Vec_len(self) + additional_count;
    if (minimum_capacity > Vec_capacity(self)) {
        return Vec_ensureTotalCapacity(
            self,
            Vec__calcCapacityIncrease(minimum_capacity)
        );
    }
    return Res_ok(Res_Void, (Void){ 0 });
}

/*========== Modification Functions =======================================*/

// Append an item
#define Vec_append(T, self, item) \
    Vec__append(self, &(item))

force_inline Res_Void Vec__append(
    Vec*         self,
    const anyptr item
) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(item);

    const usize    elem_size  = Slice_size(self->items_);
    const Res_Void ensure_res = Vec_ensureUnusedCapacity(self, 1);
    if (Res_isErr(ensure_res)) {
        return ensure_res;
    }
    printf("appending %p\n", item);
    printf("appending %lld\n", *(isize*)item);

    memcpy(
        Mptr_raw(Mptr_add(Slice_ptr(self->items_), (isize)self->len_)),
        item,
        elem_size
    );
    self->len_ += 1;

    return Res_ok(Res_Void, (Void){ 0 });
}

// Pop last item
#define Vec_pop(T, self) \
    (*(T*)Vec__pop(self, sizeof(T)))

force_inline anyptr Vec__pop(Vec* self) {
    debug_assert_nonnull(self);
    debug_assert_true(self->len_ > 0);

    self->len_ -= 1;
    const usize elem_size = Slice_size(self->items_);
    return (u8*)Slice_raw(self->items_) + (self->len_ * elem_size);
}

// Clear all items
force_inline void Vec_clear(Vec* self) {
    debug_assert_nonnull(self);
    self->len_ = 0;
}

// Get slice (capacity range)
force_inline Slice Vec_slice(const Vec* self) {
    debug_assert_nonnull(self);
    return self->items_; // Already contains correct size information
}

// Get slice (length range)
force_inline Slice Vec_items(const Vec* self) {
    debug_assert_nonnull(self);
    return Slice_make(Slice_ptr(self->items_), self->len_);
}

// Insert item at index
#define Vec_insert(T, self, idx, item) \
    Vec__insert(self, idx, &(item))

force_inline Res_Void Vec__insert(
    Vec*         self,
    usize        idx,
    const anyptr item
) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(item);
    debug_assert_true(idx <= self->len_);

    const Res_Void ensure_res = Vec_ensureUnusedCapacity(self, 1);
    if (Res_isErr(ensure_res)) {
        return ensure_res;
    }

    const usize elem_size  = Slice_size(self->items_);
    const usize move_count = self->len_ - idx;

    if (move_count > 0) {
        memmove(
            (u8*)Slice_raw(self->items_) + ((idx + 1) * elem_size),
            (u8*)Slice_raw(self->items_) + (idx * elem_size),
            move_count * elem_size
        );
    }

    memcpy(
        (u8*)Slice_raw(self->items_) + (idx * elem_size),
        item,
        elem_size
    );
    self->len_ += 1;

    return Res_ok(Res_Void, (Void){ 0 });
}

// Remove item at index
#define Vec_remove(self, idx) \
    Vec__remove(self, idx)

force_inline void Vec__remove(
    Vec*  self,
    usize idx
) {
    debug_assert_nonnull(self);
    debug_assert_true(idx < self->len_);

    const usize elem_size  = Slice_size(self->items_);
    const usize move_count = self->len_ - idx - 1;

    if (move_count > 0) {
        memmove(
            (u8*)Slice_raw(self->items_) + (idx * elem_size),
            (u8*)Slice_raw(self->items_) + ((idx + 1) * elem_size),
            move_count * elem_size
        );
    }

    self->len_ -= 1;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DS_VEC_INCLUDED */

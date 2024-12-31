/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ArrayList.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-30 (date of creation)
 * @updated 2024-12-30 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  ArrayList
 *
 * @brief
 * @details
 */

#ifndef DS_ARRAY_LIST_INCLUDED
#define DS_ARRAY_LIST_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"
#include "dh/meta/common.h"

// ArrayList structure definition
#define ArrayList$(T)            \
    struct {                     \
        Sli$(T) items;           \
        usize         capacity;  \
        mem_Allocator allocator; \
    }
#define using_ArrayList$(T) \
    decl_ArrayList$(T);     \
    impl_ArrayList$(T)
#define decl_ArrayList$(T) \
    typedef struct pp_join($, ArrayList, T) pp_join($, ArrayList, T)
#define impl_ArrayList$(T)            \
    struct pp_join($, ArrayList, T) { \
        Sli$(T) items;                \
        usize         capacity;       \
        mem_Allocator allocator;      \
    }

typedef 
using_Err$(ArrayList);

/*========== Core Functions ================================================*/

extern ArrayList     ArrayList_init(TypeInfo type, mem_Allocator allocator);
extern Err$ArrayList ArrayList_initCapacity(TypeInfo type, mem_Allocator, usize capacity);
extern void          ArrayList_fini(ArrayList* self);

extern usize ArrayList_len(const ArrayList* self);
extern usize ArrayList_capacity(const ArrayList* self);
extern bool  ArrayList_isEmpty(const ArrayList* self);
extern bool  ArrayList_isFull(const ArrayList* self);

// Free ArrayList's memory
force_inline void Vec_fini(ArrayList* self);

// Get current length
force_inline usize Vec_len(const ArrayList* self);
// Get current capacity
force_inline usize Vec_capacity(const ArrayList* self);
// Check if ArrayList is empty
force_inline bool  Vec_isEmpty(const ArrayList* self);

/*========== Access Functions ==============================================*/

// Get item at index
#define Vec_at(T, self, idx) (*(T*)Vec__at(self, idx))
force_inline anyptr Vec__at(const ArrayList* self, usize idx);

// Get first item as optional
force_inline Opt_anyptr Vec_firstOrNull(const ArrayList* self);
// Get last item as optional
force_inline Opt_anyptr Vec_lastOrNull(const ArrayList* self);

/*========== Growth Functions ============================================*/

// Calculate growth capacity
force_inline usize    Vec__nextCapacity(usize current_capacity);
// Ensure capacity for n more items
force_inline Res_Void Vec_ensureUnusedCapacity(ArrayList* self, usize additional);

/*========== Modification Functions =======================================*/

// Append item
force_inline Res_Void Vec_append(ArrayList* self, const anyptr item);

// Pop last item
#define Vec_pop(T, self) (*(T*)Vec__pop(self))
force_inline anyptr Vec__pop(ArrayList* self);

// Pop last item as optional
force_inline Opt_anyptr Vec_popOrNull(ArrayList* self);

// Insert item at index
#define Vec_insert(T, self, idx, item) Vec__insert(self, idx, &(item))
force_inline Res_Void Vec__insert(ArrayList* self, usize idx, const anyptr item);

// Remove item at index
force_inline void Vec_remove(ArrayList* self, usize idx);
// Clear all items
force_inline void Vec_clear(ArrayList* self);

/*========== Slice Access ================================================*/

// Get slice of full capacity
force_inline Slice Vec_slice(const ArrayList* self);
// Get slice of current items
force_inline Slice Vec_items(const ArrayList* self);

/*========== Implementation =================================================*/

force_inline ArrayList Vec__init(mem_Allocator* allocator, PtrBase_MetaData type_info) {
    debug_assert_fmt(allocator != null, "Allocator cannot be null");
    return (ArrayList){
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

    return Res_ok(Res_Vec, (ArrayList){ .data = Res_unwrap(Res_Slice, slice), .len = 0, .allocator = allocator });
}

force_inline void Vec_fini(ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    if (!Slice_isUndefined(self->data)) {
        mem_Allocator_freeSlice(self->allocator, self->data);
        self->len = 0;
    }
}

force_inline usize Vec_len(const ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    return self->len;
}

force_inline usize Vec_capacity(const ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    return Slice_len(self->data);
}

force_inline bool Vec_isEmpty(const ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    return self->len == 0;
}

force_inline anyptr Vec__at(const ArrayList* self, usize idx) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    debug_assert_fmt(idx < self->len, "Index out of bounds");
    return Slice__getAtAddr(self->data, idx, Slice_size(self->data));
}

force_inline Opt_anyptr Vec_firstOrNull(const ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    if (Vec_isEmpty(self)) {
        return Opt_none(Opt_anyptr);
    }
    return Opt_some(Opt_anyptr, Vec__at(self, 0));
}

force_inline Opt_anyptr Vec_lastOrNull(const ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
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

force_inline Res_Void Vec_ensureUnusedCapacity(ArrayList* self, usize additional) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");

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

force_inline Res_Void Vec_append(ArrayList* self, const anyptr item) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
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

force_inline anyptr Vec__pop(ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    debug_assert_fmt(!Vec_isEmpty(self), "ArrayList is empty");
    self->len -= 1;
    return (u8*)Slice_addr(self->data) + (self->len * Slice_size(self->data));
}

force_inline Opt_anyptr Vec_popOrNull(ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    if (Vec_isEmpty(self)) {
        return Opt_none(Opt_anyptr);
    }

    self->len -= 1;
    return Opt_some(
        Opt_anyptr,
        (u8*)Slice_addr(self->data) + (self->len * Slice_size(self->data))
    );
}

force_inline Res_Void Vec__insert(ArrayList* self, usize idx, const anyptr item) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
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

force_inline void Vec_remove(ArrayList* self, usize idx) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
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

force_inline void Vec_clear(ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    self->len = 0;
}

force_inline Slice Vec_slice(const ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    return self->data;
}

force_inline Slice Vec_items(const ArrayList* self) {
    debug_assert_fmt(self != null, "ArrayList cannot be null");
    if (Slice_isUndefined(self->data)) {
        return Slice_undefined(Void);
    }
    return Slice_prefix(self->data, self->len);
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DS_ARRAY_LIST_INCLUDED */

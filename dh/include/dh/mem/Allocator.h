/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Allocator.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2024-12-07 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_Allocator
 *
 * @brief   Memory allocator interface
 * @details
 */

#ifndef MEM_ALLOCATOR_INCLUDED
#define MEM_ALLOCATOR_INCLUDED (1)
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

/*========== Allocator Interface ============================================*/

typedef struct mem_Allocator        mem_Allocator;
typedef struct mem_Allocator_VTable mem_Allocator_VTable;
Sptr(mem_Allocator) Sptr_mem_Allocator;

// Allocator instance
struct mem_Allocator {
    anyptr                      ptr_;
    const mem_Allocator_VTable* vt_;
};

// Allocator vtable
// The allocator must provide these functions:
struct mem_Allocator_VTable {
    // Try to allocate memory
    Res_Mptr_u8 (*alloc)(anyptr self, usize n, usize align);

    // Try to resize in-place - returns true if successful
    bool (*resize)(anyptr self, Slice_u8 buf, usize buf_align, usize new_len);

    // Free memory
    void (*free)(anyptr self, Slice_u8 buf, usize buf_align);
};

/*========== Core Allocator Functions =======================================*/

// Raw allocation - returns null if out of memory
force_inline Mptr_u8 mem_Allocator_rawAlloc(mem_Allocator self, usize n, usize align);
// Try to resize in-place - returns true if successful
force_inline bool    mem_Allocator_rawResize(mem_Allocator self, Slice_u8 buf, usize buf_align, usize new_len);
// Free memory
force_inline void    mem_Allocator_rawFree(mem_Allocator self, Slice_u8 buf, usize buf_align);

/*========== High-level Allocator Functions =================================*/

#define mem_Allocator_create(self, T) \
    mem_Allocator_createSptr(self, sizeof(T), alignof(T))

#define mem_Allocator_destroy(self, ptr) \
    mem_Allocator_destroySptr(self, ptr)

#define mem_Allocator_alloc(self, T, n) \
    mem_Allocator_allocSlice(self, sizeof(T), alignof(T), n)

#define mem_Allocator_resize(self, ptr, T, n) \
    mem_Allocator_resizeSlice(self, ptr, sizeof(T), alignof(T), n)

#define mem_Allocator_realloc(self, ptr, T, n) \
    mem_Allocator_reallocSlice(self, ptr, sizeof(T), alignof(T), n)

#define mem_Allocator_free(self, ptr) \
    mem_Allocator_freeSlice(self, ptr, sizeof(T))

// Allocate single-item
force_inline Res_Sptr  mem_Allocator_createSptr(mem_Allocator* self, usize size, usize align);
// Free single-item
force_inline void      mem_Allocator_destroySptr(mem_Allocator* self, Sptr* ptr);
// Allocate many-item
force_inline Res_Slice mem_Allocator_allocSlice(mem_Allocator* self, usize elem_size, usize align, usize count);
// Try to resize buffer in-place. Returns true if successful.
// If true is returned, the buffer was resized in-place.
// If false is returned, the buffer was unchanged.
force_inline bool      mem_Allocator_resizeSlice(mem_Allocator* self, Slice* slice, usize new_len);
// Reallocate buffer to new size, possibly moving it.
// Returns optional new slice - null means allocation failed.
// The existing memory is preserved up to min(old_len, new_len).
force_inline Opt_Slice mem_Allocator_reallocSlice(mem_Allocator* self, Slice slice, usize new_len);
// Free many-item
force_inline void      mem_Allocator_freeSlice(mem_Allocator* self, Slice* slice);

/*========== Implementation =================================================*/

force_inline Mptr_u8 mem_Allocator_rawAlloc(mem_Allocator self, usize n, usize align) {
    return Res_unwrap(Res_Mptr_u8, self.vt_->alloc(self.ptr_, n, align));
}

force_inline bool mem_Allocator_rawResize(mem_Allocator self, Slice_u8 buf, usize buf_align, usize new_len) {
    return self.vt_->resize(self.ptr_, buf, buf_align, new_len);
}

force_inline void mem_Allocator_rawFree(mem_Allocator self, Slice_u8 buf, usize buf_align) {
    self.vt_->free(self.ptr_, buf, buf_align);
}

force_inline Res_Sptr mem_Allocator_createSptr(mem_Allocator* self, usize size, usize align) {
    const Mptr_u8 ptr = mem_Allocator_rawAlloc(*self, size, align);
    if (Mptr_isZero(ptr)) {
        return Res_err(Res_Sptr, Err_insufficient_memory);
    }
    return Res_ok(Res_Sptr, cptr_make(Mptr_raw(ptr), size, align));
}

force_inline void mem_Allocator_destroySptr(mem_Allocator* self, Sptr* ptr) {
    const Slice slice = { .ptr = *ptr, .len = 1 };
    mem_Allocator_rawFree(*self, slice, Slice_align(slice));
    *ptr = (Sptr){ 0 };
}

force_inline Res_Slice mem_Allocator_allocSlice(mem_Allocator* self, usize elem_size, usize align, usize count) {
    const usize size = elem_size * count;
    const Mptr  ptr  = mem_Allocator_rawAlloc(*self, size, align);
    if (Mptr_isZero(ptr)) {
        return Res_err(Res_Slice, Err_insufficient_memory);
    }
    return Res_ok(Res_Slice, Slice_make(ptr, count));
}

force_inline bool mem_Allocator_resizeSlice(mem_Allocator* self, Slice* slice, usize new_len) {
    const Slice_u8 bytes = {
        .ptr = Mptr_make(u8, Slice_raw(*slice)),
        .len = slice->len * Slice_size(*slice)
    };

    const usize new_bytes = new_len * Slice_size(*slice);
    if (!mem_Allocator_rawResize(*self, bytes, Slice_align(bytes), new_bytes)) {
        return false;
    }

    slice->len = new_len;
    return true;
}

force_inline Opt_Slice mem_Allocator_reallocSlice(mem_Allocator* self, Slice slice, usize new_len) {
    if (mem_Allocator_resizeSlice(self, &slice, new_len)) {
        return Opt_some(Opt_Slice, slice);
    }

    // Resize failed, need to allocate new buffer and copy
    const Res_Slice new_slice = mem_Allocator_allocSlice(self, Slice_size(slice), Slice_align(slice), new_len);
    if (Res_isErr(new_slice)) {
        return Opt_none(Opt_Slice);
    }

    const Slice new_buf  = Res_unwrap(Res_Slice, new_slice);
    const usize copy_len = (slice.len < new_len) ? slice.len : new_len;
    memcpy(Slice_raw(new_buf), Slice_raw(slice), copy_len * Slice_size(slice));

    mem_Allocator_freeSlice(self, &slice);
    return Opt_some(Opt_Slice, new_buf);
}

force_inline void mem_Allocator_freeSlice(mem_Allocator* self, Slice* slice) {
    const Slice_u8 bytes = {
        .ptr = Mptr_make(u8, Slice_raw(*slice)),
        .len = slice->len * Slice_size(*slice)
    };
    mem_Allocator_rawFree(*self, bytes, Slice_align(bytes));
    *slice = (Slice){ 0 };
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* MEM_ALLOCATOR_INCLUDED */

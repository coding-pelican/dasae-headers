/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Allocator.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2024-12-10 (date of last update)
 * @version v0.2
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_Allocator
 *
 * @brief   Memory allocator interface using smart pointers
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
    Sptr                        self_; // Self pointer with type info
    const mem_Allocator_VTable* vt_;   // Virtual table
};

// Allocator vtable
struct mem_Allocator_VTable {
    // Allocate memory with size/alignment from PtrBase
    // Returns Res_Mptr for allocation result
    Res_Mptr (*alloc)(Sptr self, PtrBase_MetaData meta);

    // Try to resize existing allocation - returns success/failure
    bool (*resize)(Sptr self, Mptr ptr, usize new_len);

    // Free allocation using pointer's metadata
    void (*free)(Sptr self, Mptr ptr);
};

/*========== Core Allocator Functions =======================================*/

/* Raw allocation */
force_inline Res_Mptr mem_Allocator_rawAlloc(mem_Allocator self, PtrBase_MetaData meta);
/* Try to resize in-place */
force_inline bool     mem_Allocator_rawResize(mem_Allocator self, Mptr ptr, usize new_len);
/* Free memory */
force_inline void     mem_Allocator_rawFree(mem_Allocator self, Mptr ptr);

/*========== High-level Allocator Functions =================================*/

/* Single-item allocation */
force_inline Res_Sptr  mem_Allocator_createSptr(mem_Allocator* self, usize size, usize align);
/* Free single-item */
force_inline void      mem_Allocator_destroySptr(mem_Allocator* self, Sptr ptr);
/* Slice allocation */
force_inline Res_Slice mem_Allocator_allocSlice(mem_Allocator* self, usize elem_size, usize elem_align, usize count);
/* Try to resize slice in-place */
force_inline bool      mem_Allocator_resizeSlice(mem_Allocator* self, Slice* slice, usize new_len);
/* Reallocate slice with new size */
force_inline Opt_Slice mem_Allocator_reallocSlice(mem_Allocator* self, Slice slice, usize new_len);
/* Free slice */
force_inline void      mem_Allocator_freeSlice(mem_Allocator* self, Slice slice);

// Helper macros
#define mem_Allocator_create(self, T) \
    mem_Allocator_createSptr(self, sizeof(T), alignof(T))

#define mem_Allocator_destroy(self, ptr) \
    mem_Allocator_destroySptr(self, ptr)

#define mem_Allocator_alloc(self, T, n) \
    mem_Allocator_allocSlice(self, sizeof(T), alignof(T), n)

#define mem_Allocator_resize(self, ptr, n) \
    mem_Allocator_resizeSlice(self, ptr, n)

#define mem_Allocator_realloc(self, ptr, n) \
    mem_Allocator_reallocSlice(self, ptr, n)

#define mem_Allocator_free(self, ptr) \
    mem_Allocator_freeSlice(self, ptr)

/*========== Implementation =================================================*/

force_inline Res_Mptr mem_Allocator_rawAlloc(mem_Allocator self, PtrBase_MetaData meta) {
    return self.vt_->alloc(self.self_, meta);
}

force_inline bool mem_Allocator_rawResize(mem_Allocator self, Mptr ptr, usize new_len) {
    return self.vt_->resize(self.self_, ptr, new_len);
}

force_inline void mem_Allocator_rawFree(mem_Allocator self, Mptr ptr) {
    self.vt_->free(self.self_, ptr);
}

force_inline Res_Sptr mem_Allocator_createSptr(mem_Allocator* self, usize size, usize align) {
    debug_assert_fmt(!PtrBase_isUndefined(self->self_.Base_), "Invalid allocator");

    PtrBase_MetaData meta = PtrBase_makeTypeInfo(size, align);
    Res_Mptr         res  = mem_Allocator_rawAlloc(*self, meta);

    if (Res_isErr(res)) {
        return Res_err(Res_Sptr, Res_unwrapErr(Res_Mptr, res));
    }

    return Res_ok(Res_Sptr, (Sptr){ .Base_ = Res_unwrap(Res_Mptr, res).Base_ });
}

force_inline void mem_Allocator_destroySptr(mem_Allocator* self, Sptr ptr) {
    if (!Sptr_isUndefined(ptr)) {
        mem_Allocator_rawFree(*self, Sptr_toMptr(ptr));
    }
}

force_inline Res_Slice mem_Allocator_allocSlice(mem_Allocator* self, usize elem_size, usize elem_align, usize count) {
    debug_assert_fmt(!PtrBase_isUndefined(self->self_.Base_), "Invalid allocator");

    PtrBase_MetaData meta = PtrBase_makeTypeInfo(elem_size, elem_align);
    Res_Mptr         res  = mem_Allocator_rawAlloc(*self, meta);

    if (Res_isErr(res)) {
        return Res_err(Res_Slice, Res_unwrapErr(Res_Mptr, res));
    }

    Mptr ptr = Res_unwrap(Res_Mptr, res);
    return Res_ok(Res_Slice, Mptr_toSlice(ptr, 0, count));
}

force_inline bool mem_Allocator_resizeSlice(mem_Allocator* self, Slice* slice, usize new_len) {
    debug_assert_fmt(!Slice_isUndefined(*slice), "Invalid slice");

    if (new_len == 0) {
        mem_Allocator_freeSlice(self, *slice);
        return true;
    }

    if (Slice_len(*slice) == 0) {
        return false;
    }

    // Calculate new byte size - check for overflow
    const usize elem_size = Slice_size(*slice);
    if (new_len > SIZE_MAX / elem_size) {
        return false; // Would overflow
    }
    const usize new_byte_size = new_len * elem_size;

    // Call raw resize with byte size
    if (mem_Allocator_rawResize(*self, Slice_beginMptr(*slice), new_byte_size)) {
        slice->len_ = new_len;
        return true;
    }
    return false;
}

force_inline Opt_Slice mem_Allocator_reallocSlice(mem_Allocator* self, Slice slice, usize new_len) {
    // Try resize in place first
    if (mem_Allocator_resizeSlice(self, &slice, new_len)) {
        return Opt_some(Opt_Slice, slice);
    }

    // Allocate new buffer
    Res_Slice new_slice = mem_Allocator_allocSlice(self, Slice_size(slice), Slice_align(slice), new_len);
    if (Res_isErr(new_slice)) {
        return Opt_none(Opt_Slice);
    }

    // Copy data and free old buffer
    Slice result   = Res_unwrap(Res_Slice, new_slice);
    usize copy_len = slice.len_ < new_len ? slice.len_ : new_len;

    memcpy(
        Slice_addr(result),
        Slice_addr(slice),
        copy_len * Slice_size(slice)
    );

    mem_Allocator_rawFree(*self, Slice_beginMptr(slice));
    return Opt_some(Opt_Slice, result);
}

force_inline void mem_Allocator_freeSlice(mem_Allocator* self, Slice slice) {
    if (!Slice_isUndefined(slice)) {
        mem_Allocator_rawFree(*self, Slice_beginMptr(slice));
    }
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_ALLOCATOR_INCLUDED */

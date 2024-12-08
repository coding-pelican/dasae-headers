/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Classic.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2024-12-08 (date of last update)
 * @version v0.1.1
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Classic
 *
 * @brief   Classic C standard library allocator wrapper
 * @details
 */

#include "dh/claim/assert.h"
#ifndef HEAP_CLASSIC_INCLUDED
#define HEAP_CLASSIC_INCLUDED (1)
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

#include <stdlib.h>

/*========== Classic C Allocator ============================================*/

// Classic allocator instance (minimal state)
typedef struct heap_Classic {
    u8 unused_; // Empty struct not allowed in C
} heap_Classic;

static_inline mem_Allocator heap_Classic_allocator(heap_Classic* self);

static_inline Res_Void heap_Classic_init(mem_Allocator* self);
static_inline void     heap_Classic_fini(mem_Allocator* self);

static_inline Res_Mptr_u8 heap_Classic_alloc(anyptr self, usize n, usize align);
static_inline bool        heap_Classic_resize(anyptr self, Slice_u8 buf, usize buf_align, usize new_size);
static_inline void        heap_Classic_free(anyptr ctx, Slice_u8 buf, usize buf_align);

// VTable for Classic allocator
static const mem_Allocator_VTable heap_Classic_vtable[1] = { {
    .alloc  = heap_Classic_alloc,
    .resize = heap_Classic_resize,
    .free   = heap_Classic_free,
} };

/*========== Implementation =================================================*/

// Get underlying malloc_size if available
#if defined(__GLIBC__) || defined(__APPLE__)
#define heap_Classic_has_malloc_size (1)
force_inline usize heap_Classic_mallocSize(const anyptr ptr) {
#if defined(__GLIBC__)
    return malloc_usable_size((anyptr)ptr);
#else
    return malloc_size(ptr);
#endif
}
#else
#define heap_Classic_has_malloc_size (0)
#endif

static_inline mem_Allocator heap_Classic_allocator(heap_Classic* self) {
    debug_assert_nonnull(self);
    return (mem_Allocator){
        .ptr_ = self,
        .vt_  = heap_Classic_vtable
    };
}

static_inline Res_Void heap_Classic_init(mem_Allocator* self) {
    debug_assert_nonnull(self);
    unused(self);
    return Res_ok(Res_Void, (Void){ 0 });
}

static_inline void heap_Classic_fini(mem_Allocator* self) {
    debug_assert_nonnull(self);
    unused(self);
}

static_inline Res_Mptr_u8 heap_Classic_alloc(anyptr self, usize n, usize align) {
    // Handle null sel
    if (self == null) { return Res_err(Res_Mptr_u8, Err_invalid_argument); }

    // Handle zero size allocation
    if (n == 0) { return Res_ok(Res_Mptr_u8, Mptr_make(u8, null)); }

    // Calculate required alignment
    const usize alignment = align;
    anyptr      ptr       = null;

// Use aligned allocation functions if available
#if defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    if (posix_memalign(&ptr, alignment, n) != 0) {
        ptr = null;
    }
#elif defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    ptr = _aligned_malloc(n, alignment);
#else
    anyptr raw = malloc(n + alignment);
    if (raw == null) {
        ptr = null;
    } else {
        ptr                = mem_align_ptr(raw, alignment);
        ((anyptr*)ptr)[-1] = raw;
    }
#endif

    if (ptr == null) {
        return Res_err(Res_Mptr_u8, Err_insufficient_memory);
    }

    return Res_ok(Res_Mptr_u8, cptr_make(ptr, sizeof(ptr), alignment));
}

static_inline bool heap_Classic_resize(anyptr self, Slice_u8 buf, usize buf_align, usize new_size) {
    claim_assert_nonnull(self);
    claim_assert_fmt(Slice_align(buf) == buf_align, "buf_align: %zu, Slice_align(buf): %zu", buf_align, Slice_align(buf));

    // Special case: resizing to 0
    if (new_size == 0) { return true; }

    // Special case: empty buffer
    if (buf.len == 0) { return false; }

    // Check if new size fits in existing allocation
    if (new_size <= buf.len) { return true; }

#if heap_Classic_has_malloc_size
    const usize full_size = heap_Classic_mallocSize(buf.ptr);
    if (new_size <= full_size) {
        return true;
    }
#endif

    return false;
}

static_inline void heap_Classic_free(anyptr ctx, Slice_u8 buf, usize buf_align) {
    claim_assert_nonnull(ctx);
    claim_assert_fmt(Slice_align(buf) == buf_align, "buf_align: %zu, Slice_align(buf): %zu", buf_align, Slice_align(buf));

    if (Mptr_isZero(buf.ptr)) { return; }

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    _aligned_free(Mptr_raw(buf.ptr));
#elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    free(Mptr_raw(buf.ptr));
#else
    free((Mptr_raw(buf.ptr))[-1]);
#endif
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HEAP_CLASSIC_INCLUDED */

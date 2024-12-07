/**
 * @copyright Copyright 2024. All rights reserved.
 * @file    mem/heap.h
 * @brief   General-purpose heap allocator implementation
 */

#ifndef MEM_HEAP_INCLUDED
#define MEM_HEAP_INCLUDED (1)
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
#include "dh/claim/assert.h"
#include "dh/mem/Allocator.h"

#include <stdlib.h>

/*========== Heap Allocator Implementation ================================*/

// Heap allocator context - empty since we use system malloc/free
typedef struct mem_HeapAllocator {
    u8 unused_; // Empty struct is not allowed in C
} mem_HeapAllocator;

// Allocate memory from heap
static inline Res_Mptr_u8 mem_HeapAllocator_alloc(anyptr ctx, usize n, u8 log2_align) {
    if (ctx == null) {
        return Res_err(Res_Mptr_u8, Err_invalid_argument);
    }

    // Handle zero-size allocation
    if (n == 0) {
        return Res_ok(Res_Mptr_u8, Mptr_make(u8, null));
    }

    // Calculate actual alignment in bytes
    const usize alignment = 1ULL << log2_align;

    // Allocate aligned memory
#if defined(__APPLE__)
    anyptr ptr = null;
    if (posix_memalign(&ptr, alignment, n) != 0) {
        ptr = null;
    }
#else
    anyptr ptr = _aligned_malloc(alignment, n);
#endif

    if (ptr == null) {
        return Res_err(Res_Mptr_u8, Err_insufficient_memory);
    }

    return Res_ok(Res_Mptr_u8, Mptr_make(u8, ptr));
}

// Try to resize memory in-place
static inline bool mem_HeapAllocator_resize(anyptr ctx, Slice_u8 buf, u8 log2_buf_align, usize new_len) {
    claim_assert_nonnull(ctx);

    // Special case: resizing to 0 is always successful
    if (new_len == 0) { return true; }

    // Special case: resizing from 0 is never in-place
    if (buf.len == 0) { return false; }

    // Get current allocation
    anyptr ptr = Slice_raw(buf);

    // Try to resize in-place
    anyptr new_ptr = _aligned_realloc(ptr, new_len, log2_buf_align);

    // Check if realloc kept the same address
    return new_ptr == ptr;
}

// Free allocated memory
static inline void mem_HeapAllocator_free(anyptr ctx, Slice_u8 buf, u8 log2_buf_align) {
    claim_assert_nonnull(ctx);
    unused(log2_buf_align);

    anyptr ptr = Slice_raw(buf);

    if (ptr == null) { return; }

    _aligned_free(ptr);
}

// Heap allocator vtable
static const mem_Allocator_VTable mem_HeapAllocator_vt = {
    .alloc  = mem_HeapAllocator_alloc,
    .resize = mem_HeapAllocator_resize,
    .free   = mem_HeapAllocator_free,
};

// Initialize heap allocator
static inline mem_Allocator mem_HeapAllocator_init(mem_HeapAllocator* self) {
    debug_assert_nonnull(self);
    return (mem_Allocator){
        .ptr_ = self,
        .vt_  = &mem_HeapAllocator_vt,
    };
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_HEAP_INCLUDED */

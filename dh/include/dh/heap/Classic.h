/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Classic.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2024-12-10 (date of last update)
 * @version v0.2
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Classic
 *
 * @brief   Classic C standard library allocator wrapper using smart pointers
 */

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
Sptr(heap_Classic) Sptr_heap_Classic;

// Get allocator interface for instance
static_inline mem_Allocator heap_Classic_allocator(heap_Classic* self);

// Lifecycle
static_inline Res_Void heap_Classic_init(mem_Allocator* self);
static_inline void     heap_Classic_fini(mem_Allocator* self);

// VTable implementations
static_inline Res_Mptr heap_Classic_alloc(Sptr self, PtrBase_MetaData meta);
static_inline bool     heap_Classic_resize(Sptr self, Mptr ptr, usize new_len);
static_inline void     heap_Classic_free(Sptr self, Mptr ptr);

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
force_inline usize heap_Classic_mallocSize(anyptr ptr) {
#if defined(__GLIBC__)
    return malloc_usable_size(ptr);
#else
    return malloc_size(ptr);
#endif
}
#else
#define heap_Classic_has_malloc_size (0)
#endif

static_inline mem_Allocator heap_Classic_allocator(heap_Classic* self) {
    debug_assert_fmt(self != null, "Null allocator instance");
    return (mem_Allocator){
        .self_ = Sptr_ref(heap_Classic, *self),
        .vt_   = heap_Classic_vtable
    };
}

static_inline Res_Void heap_Classic_init(mem_Allocator* self) {
    debug_assert_fmt(self != null, "Null allocator");
    unused(self);
    return Res_ok(Res_Void, makeCleared(Void));
}

static_inline void heap_Classic_fini(mem_Allocator* self) {
    debug_assert_fmt(self != null, "Null allocator");
    unused(self);
}

static_inline Res_Mptr heap_Classic_alloc(Sptr self, PtrBase_MetaData meta) {
    // Handle invalid input
    if (Sptr_isUndefined(self)) {
        return Res_err(Res_Mptr, Err_invalid_argument);
    }

    // Get size and alignment from meta
    const usize size  = meta.type_size;
    const usize align = PtrBase__calcAlignLog2Order(meta.log2_align);

    // Handle zero size
    if (size == 0) {
        return Res_ok(Res_Mptr, (Mptr){ .Base_ = PtrBase_undefined(meta) });
    }

    // Allocate aligned memory
    anyptr ptr = null;

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    ptr = _aligned_malloc(size, align);
#elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    if (posix_memalign(&ptr, align, size) != 0) {
        ptr = null;
    }
#else
    // Manual alignment
    const usize header_size = sizeof(anyptr);
    anyptr      raw         = malloc(size + align + header_size);
    if (raw != null) {
        ptr                = (u8*)raw + header_size;
        ptr                = (anyptr)(((usize)ptr + align - 1) & ~(align - 1));
        ((anyptr*)ptr)[-1] = raw;
    }
#endif

    if (ptr == null) {
        return Res_err(Res_Mptr, Err_insufficient_memory);
    }

    // Create Mptr with provided type info
    PtrBase base = PtrBase_fromAddr(ptr, meta);
    return Res_ok(Res_Mptr, (Mptr){ .Base_ = base });
}

static_inline bool heap_Classic_resize(Sptr self, Mptr ptr, usize new_size) {
    debug_assert_fmt(!Sptr_isUndefined(self), "Undefined allocator");
    debug_assert_fmt(!Mptr_isUndefined(ptr), "Undefined pointer");
    unused(self);

    // Special cases
    if (new_size == 0) {
        return true;
    }

    // Get current allocation size
    const anyptr addr = Mptr_addr(ptr);
    if (addr == null) {
        return false;
    }

    // Check if new size fits in current allocation
#if heap_Classic_has_malloc_size
    const usize full_size = heap_Classic_mallocSize(addr);
    if (new_size <= full_size) {
        return true;
    }
#else
    // Without malloc_size, we can only shrink within the original allocation
    // Use PtrBase size which represents the original allocation size
    const usize original_size = Mptr_size(ptr);
    if (new_size <= original_size) {
        return true;
    }
#endif

    return false;
}

static_inline void heap_Classic_free(Sptr self, Mptr ptr) {
    debug_assert_fmt(!Sptr_isUndefined(self), "Undefined allocator");
    unused(self);

    if (Mptr_isUndefined(ptr)) {
        return;
    }

    anyptr raw_ptr = Mptr_addr(ptr);

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    _aligned_free(raw_ptr);
#elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    free(raw_ptr);
#else
    // Manual alignment cleanup
    free(((anyptr*)raw_ptr)[-1]);
#endif
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HEAP_CLASSIC_INCLUDED */

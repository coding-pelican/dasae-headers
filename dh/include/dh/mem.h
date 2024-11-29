/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    mem.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-20 (date of creation)
 * @updated 2024-11-24 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  mem
 *
 * @brief   Memory management with Zig-style semantics
 * @details Provides type-safe memory management with clear separation between
 *          single-element pointers (sptr) and multi-element views (Slice).
 *          Features:
 *          - Type-safe memory operations
 *          - Clear error handling with try/OrNull variants
 *          - Debug assertions in development builds
 *          - Extensible allocator interface
 */

// TODO: Add more details about the memory management system

#ifndef CORE_MEM_INCLUDED
#define CORE_MEM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Types ========================================================*/

// Memory state tracking for debug
typedef enum mem_State {
    mem_State_invalid,   // Not allocated/initialized
    mem_State_allocated, // Currently allocated
    mem_State_freed      // Has been freed
} mem_State;

// Debug information
typedef struct mem_Debug {
    mem_State   state;
    const char* file;
    i32         line;
    const char* func;
} mem_Debug;

// Allocator interface
typedef struct mem_Allocator {
    anyptr ctx; // Allocator context

    // Core allocation functions
    anyptr (*alloc)(anyptr ctx, usize size, mem_Debug* debug);
    void (*free)(anyptr ctx, anyptr ptr, mem_Debug* debug);
    anyptr (*realloc)(anyptr ctx, anyptr ptr, usize size, mem_Debug* debug);

#if DEBUG_ENABLED
    mem_Debug debug; // Debug state tracking
#endif
} mem_Allocator;

/*========== Single Element Functions ======================================*/

// Create single element
#define mem_create(alloc, T) \
    IMPL_mem_create(alloc, sizeof(T), _Alignof(T))

#define mem_createDebug(alloc, T) \
    IMPL_mem_createDebug(alloc, sizeof(T), _Alignof(T), __FILE__, __LINE__, __func__)

// Create with optional
#define mem_createOpt(alloc, T) \
    IMPL_mem_createOpt(alloc, sizeof(T), _Alignof(T))

// Destroy single element
force_inline void mem_destroy(mem_Allocator* alloc, sptr_mut* ptr);
force_inline void mem_destroyDebug(mem_Allocator* alloc, sptr_mut* ptr, const char* file, i32 line, const char* func);

/*========== Multiple Element Functions ===================================*/

// Create array of elements
#define mem_alloc(alloc, T, count) \
    IMPL_mem_alloc(alloc, sizeof(T), _Alignof(T), count)

#define mem_allocDebug(alloc, T, count) \
    IMPL_mem_allocDebug(alloc, sizeof(T), _Alignof(T), count, __FILE__, __LINT__, __func__)

// Create with optional
#define mem_allocOpt(alloc, T, count) \
    IMPL_mem_allocOpt(alloc, sizeof(T), _Alignof(T), count)

// Free array
force_inline void mem_free(mem_Allocator* alloc, Slice_mut* slice);
force_inline void mem_freeDebug(mem_Allocator* alloc, Slice_mut* slice, const char* file, i32 line, const char* func);

// Resize array
#define mem_realloc(alloc, slice, T, new_count) \
    IMPL_mem_realloc(alloc, slice, sizeof(T), _Alignof(T), new_count)

#define mem_reallocOpt(alloc, slice, T, new_count) \
    IMPL_mem_reallocOpt(alloc, slice, sizeof(T), _Alignof(T), new_count)

/*========== Memory Operations ============================================*/

// Copy between memory regions
force_inline void mem_copy(sptr_mut dest, sptr src, usize size);
force_inline void mem_copySlice(Slice_mut dest, Slice src);

// Move memory (handles overlap)
force_inline void mem_move(sptr_mut dest, sptr src, usize size);
force_inline void mem_moveSlice(Slice_mut dest, Slice src);

// Set memory
force_inline void mem_set(sptr_mut ptr, i32 value, usize size);
force_inline void mem_setSlice(Slice_mut slice, i32 value);

// Compare memory
force_inline i32 mem_cmp(sptr lhs, sptr rhs, usize size);
force_inline i32 mem_cmpSlice(Slice lhs, Slice rhs);

/*========== Safety Functions ============================================*/

force_inline bool mem_isValidState(mem_State state);
force_inline bool mem_canAccess(const mem_Debug* debug);
force_inline bool mem_canFree(const mem_Debug* debug);

#ifndef CORE_MEM_IMPL_INCLUDED
#define CORE_MEM_IMPL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Single Element Implementation ================================*/

force_inline sptr_mut IMPL_mem_create(mem_Allocator* alloc, usize elem_size, usize elem_align) {
    mem_Debug debug = {
        .state = mem_State_invalid,
        .file  = __FILE__,
        .line  = __LINE__,
        .func  = __func__
    };

    void* raw = alloc->alloc(alloc->ctx, elem_size, &debug);
    debug_assert_nonnull(raw);

    sptr_mut result = sptr_mut_fromRaw(raw, elem_size);
#if DEBUG_ENABLED
    alloc->debug = debug;
#endif
    return result;
}

force_inline sptr_mut IMPL_mem_createDebug(mem_Allocator* alloc, usize elem_size, usize elem_align, const char* file, i32 line, const char* func) {
    mem_Debug debug = {
        .state = mem_State_invalid,
        .file  = file,
        .line  = line,
        .func  = func
    };

    void* raw = alloc->alloc(alloc->ctx, elem_size, &debug);
    debug_assert_nonnull(raw);

    sptr_mut result = sptr_mut_fromRaw(raw, elem_size);
#if DEBUG_ENABLED
    alloc->debug = debug;
#endif
    return result;
}

force_inline Optional(sptr_mut) IMPL_mem_createOpt(mem_Allocator* alloc, usize elem_size, usize elem_align) {
    mem_Debug debug = {
        .state = mem_State_invalid,
        .file  = __FILE__,
        .line  = __LINE__,
        .func  = __func__
    };

    void* raw = alloc->alloc(alloc->ctx, elem_size, &debug);
    if (!raw) {
        return sptr_mut_none();
    }

    sptr_mut result = sptr_mut_fromRaw(raw, elem_size);
#if DEBUG_ENABLED
    alloc->debug = debug;
#endif
    return sptr_mut_some(result);
}

force_inline void mem_destroy(mem_Allocator* alloc, sptr_mut* ptr) {
    debug_assert_true(mem_canFree(&alloc->debug));

    mem_Debug debug = {
        .state = mem_State_freed,
        .file  = __FILE__,
        .line  = __LINE__,
        .func  = __func__
    };

    alloc->free(alloc->ctx, sptr_mut_raw(*ptr), &debug);
    *ptr = sptr_mut_fromRaw(NULL, 0); // Clear pointer

#if DEBUG_ENABLED
    alloc->debug = debug;
#endif
}

/*========== Multiple Element Implementation ==============================*/

force_inline Slice_mut IMPL_mem_alloc(mem_Allocator* alloc, usize elem_size, usize elem_align, usize count) {
    mem_Debug debug = {
        .state = mem_State_invalid,
        .file  = __FILE__,
        .line  = __LINE__,
        .func  = __func__
    };

    void* raw = alloc->alloc(alloc->ctx, elem_size * count, &debug);
    debug_assert_nonnull(raw);

    Slice_mut result = Slice_mut_fromParts(
        mptr_mut_fromRaw(raw, elem_size),
        count
    );

#if DEBUG_ENABLED
    alloc->debug = debug;
#endif
    return result;
}

force_inline Optional(Slice_mut) IMPL_mem_allocOpt(mem_Allocator* alloc, usize elem_size, usize elem_align, usize count) {
    mem_Debug debug = {
        .state = mem_State_invalid,
        .file  = __FILE__,
        .line  = __LINE__,
        .func  = __func__
    };

    void* raw = alloc->alloc(alloc->ctx, elem_size * count, &debug);
    if (!raw) {
        return Slice_mut_none();
    }

    Slice_mut result = Slice_mut_fromParts(
        mptr_mut_fromRaw(raw, elem_size),
        count
    );

#if DEBUG_ENABLED
    alloc->debug = debug;
#endif
    return Slice_mut_some(result);
}

force_inline void mem_free(mem_Allocator* alloc, Slice_mut* slice) {
    debug_assert_true(mem_canFree(&alloc->debug));

    mem_Debug debug = {
        .state = mem_State_freed,
        .file  = __FILE__,
        .line  = __LINE__,
        .func  = __func__
    };

    alloc->free(alloc->ctx, Slice_mut_raw(*slice), &debug);
    *slice = Slice_mut_fromParts(mptr_mut_fromRaw(NULL, 0), 0); // Clear slice

#if DEBUG_ENABLED
    alloc->debug = debug;
#endif
}

force_inline Optional(Slice_mut) IMPL_mem_realloc(mem_Allocator* alloc, Slice_mut slice, usize elem_size, usize elem_align, usize new_count) {
    debug_assert_true(mem_canAccess(&alloc->debug));

    mem_Debug debug = {
        .state = mem_State_allocated,
        .file  = __FILE__,
        .line  = __LINE__,
        .func  = __func__
    };

    void* raw = alloc->realloc(alloc->ctx, Slice_mut_raw(slice), elem_size * new_count, &debug);
    if (!raw) {
        return Slice_mut_none();
    }

    Slice_mut result = Slice_mut_fromParts(
        mptr_mut_fromRaw(raw, elem_size),
        new_count
    );

#if DEBUG_ENABLED
    alloc->debug = debug;
#endif
    return Slice_mut_some(result);
}

/*========== Memory Operation Implementation =============================*/

force_inline void mem_copy(sptr_mut dest, sptr src, usize size) {
    debug_assert_nonnull(sptr_mut_raw(dest));
    debug_assert_nonnull(sptr_raw(src));
    memcpy(sptr_mut_raw(dest), sptr_raw(src), size);
}

force_inline void mem_copySlice(Slice_mut dest, Slice src) {
    debug_assert_eq(Slice_mut_len(dest), Slice_len(src));
    debug_assert_true(mptr_mut_hasSize(dest.ptr, mptr_size(src.ptr)));
    memcpy(Slice_mut_raw(dest), Slice_raw(src), Slice_len(src) * mptr_size(src.ptr));
}

force_inline void mem_move(sptr_mut dest, sptr src, usize size) {
    debug_assert_nonnull(sptr_mut_raw(dest));
    debug_assert_nonnull(sptr_raw(src));
    memmove(sptr_mut_raw(dest), sptr_raw(src), size);
}

force_inline void mem_moveSlice(Slice_mut dest, Slice src) {
    debug_assert_eq(Slice_mut_len(dest), Slice_len(src));
    debug_assert_true(mptr_mut_hasSize(dest.ptr, mptr_size(src.ptr)));
    memmove(Slice_mut_raw(dest), Slice_raw(src), Slice_len(src) * mptr_size(src.ptr));
}

force_inline void mem_set(sptr_mut ptr, i32 value, usize size) {
    debug_assert_nonnull(sptr_mut_raw(ptr));
    memset(sptr_mut_raw(ptr), value, size);
}

force_inline void mem_setSlice(Slice_mut slice, i32 value) {
    memset(Slice_mut_raw(slice), value, Slice_mut_len(slice) * mptr_mut_size(slice.ptr));
}

force_inline i32 mem_cmp(sptr lhs, sptr rhs, usize size) {
    debug_assert_nonnull(sptr_raw(lhs));
    debug_assert_nonnull(sptr_raw(rhs));
    return memcmp(sptr_raw(lhs), sptr_raw(rhs), size);
}

force_inline i32 mem_cmpSlice(Slice lhs, Slice rhs) {
    debug_assert_eq(Slice_len(lhs), Slice_len(rhs));
    debug_assert_true(mptr_hasSize(lhs.ptr, mptr_size(rhs.ptr)));
    return memcmp(Slice_raw(lhs), Slice_raw(rhs), Slice_len(lhs) * mptr_size(lhs.ptr));
}

/*========== Safety Implementation ======================================*/

force_inline bool mem_isValidState(mem_State state) {
    return state >= mem_State_invalid && state <= mem_State_freed;
}

force_inline bool mem_canAccess(const mem_Debug* debug) {
#if DEBUG_ENABLED
    return debug->state == mem_State_allocated;
#else
    return true;
#endif
}

force_inline bool mem_canFree(const mem_Debug* debug) {
#if DEBUG_ENABLED
    return debug->state == mem_State_allocated;
#else

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_MEM_INCLUDED */

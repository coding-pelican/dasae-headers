/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    mem.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-01 (date of creation)
 * @updated 2024-12-02 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  mem
 *
 * @brief Memory management with type-safe operations
 * @details Provides type-safe memory management with error handling through Result
 */

// TODO: Improve debug usage
// TODO: Add implementation details

#ifndef MEM_INCLUDED
#define MEM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core/prim.h"
#include "core/ptr.h"
#include "debug/assert.h"
#include "Result.h"
#include "Option.h"
#include "Error.h"

/*========== Memory State ==================================================*/

typedef enum mem_State {
    mem_State_invalid   = 0, // Unallocated/uninitialized
    mem_State_allocated = 1, // Currently allocated
    mem_State_freed     = 2  // Has been freed
} mem_State;

typedef struct mem_Debug {
    const char* file;
    const char* func;
    i32         line;
    mem_State   state;
} mem_Debug;

/*========== Memory Interface ==============================================*/

typedef struct mem_Allocator {
    anyptr ctx;

    // Core allocation functions
    Result_anyptr (*alloc)(anyptr ctx, usize size, mem_Debug* debug);
    Result_anyptr (*realloc)(anyptr ctx, anyptr ptr, usize new_size, mem_Debug* debug);
    void (*free)(anyptr ctx, anyptr ptr, mem_Debug* debug);

#if DEBUG_ENABLED
    mem_Debug debug; // Track allocation state
#endif
} mem_Allocator;

/*========== Single Element Operations =====================================*/

// Create single element
#if DEBUG_ENABLED
#define mem_create(alloc, T) \
    mem_createDebug(alloc, T, __FILE__, __LINE__, __func__)
#else
#define mem_create(alloc, T) \
    mem_createSptr(alloc, sizeof(T), alignof(T))
#endif

#define mem_createDebug(alloc, T, file, line, func) \
    mem_createSptrDebug(alloc, sizeof(T), alignof(T), file, line, func)

// Base functions for single element operations
extern Result_sptr mem_createSptr(mem_Allocator* alloc, usize elem_size, usize elem_align);
extern Result_sptr mem_createSptrDebug(mem_Allocator* alloc, usize elem_size, usize elem_align, const char* file, i32 line, const char* func);

// Destroy single element
extern void mem_destroy(mem_Allocator* alloc, sptr* ptr);

/*========== Array Operations =============================================*/

// Create array of elements
#if DEBUG_ENABLED
#define mem_alloc(alloc, T, count) \
    mem_allocDebug(alloc, T, count, __FILE__, __LINE__, __func__)
#else
#define mem_alloc(alloc, T, count) \
    mem_allocSlice(alloc, sizeof(T), alignof(T), count)
#endif

#define mem_allocDebug(alloc, T, count, file, line, func) \
    mem_allocSliceDebug(alloc, sizeof(T), alignof(T), count, file, line, func)

// Base functions for array operations
extern Result_Slice mem_allocSlice(mem_Allocator* alloc, usize elem_size, usize elem_align, usize count);
extern Result_Slice mem_allocSliceDebug(mem_Allocator* alloc, usize elem_size, usize elem_align, usize count, const char* file, i32 line, const char* func);

// Free array
extern void mem_free(mem_Allocator* alloc, Slice* slice);

// Resize array
#define mem_realloc(alloc, slice, T, new_count) \
    mem_reallocSlice(alloc, slice, sizeof(T), alignof(T), new_count)

extern Result_Slice mem_reallocSlice(mem_Allocator* alloc, Slice slice, usize elem_size, usize elem_align, usize new_count);

/*========== Memory Operations ===========================================*/

// Copy operations
extern void mem_copySptr(sptr dest, sptr src, usize size);
extern void mem_copySlice(Slice dest, Slice src);

// Move operations (handles overlap)
extern void mem_moveSptr(sptr dest, sptr src, usize size);
extern void mem_moveSlice(Slice dest, Slice src);

// Set operations
extern void mem_setSptr(sptr dest, i32 value, usize size);
extern void mem_setSlice(Slice dest, i32 value);

// Compare operations
extern i32 mem_cmpSptr(sptr lhs, sptr rhs, usize size);
extern i32 mem_cmpSlice(Slice lhs, Slice rhs);

/*========== General Purpose Allocator ===================================*/

// Standard allocator using malloc/free/realloc
extern const mem_Allocator mem_general;

// Initialize an allocator with debug info
extern void mem_initDebug(mem_Debug* debug, const char* file, i32 line, const char* func);

/*========== Safety Functions ===========================================*/

extern bool mem_isValidState(mem_State state);
extern bool mem_canAccess(const mem_Debug* debug);
extern bool mem_canFree(const mem_Debug* debug);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_INCLUDED */

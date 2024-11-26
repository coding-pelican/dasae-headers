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
 *          single-element pointers (Ptr) and multi-element views (Slice).
 *          Features:
 *          - Type-safe memory operations
 *          - Clear error handling with try/OrNull variants
 *          - Debug assertions in development builds
 *          - Extensible allocator interface
 */

#ifndef MEM_INCLUDED
#define MEM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/core/Ptr.h"
#include "dh/core/Slice.h"

/*========== Core Types =====================================================*/

/**
 * Memory allocator interface
 * Provides raw memory operations that are wrapped with type-safe functions
 */
typedef struct mem_Allocator {
    anyptr (*allocRaw)(usize size);
    void (*freeRaw)(anyptr raw);
    anyptr (*reallocRaw)(anyptr raw, usize size);
} mem_Allocator;

/**
 * General purpose allocator (uses malloc/free)
 */
extern const mem_Allocator mem_general;

/*========== Single Element Operations ======================================*/

/**
 * Create a new element with size
 * Raises debug assertion on failure
 *
 * @param alloc Allocator to use
 * @param elem_size Size of element
 * @return Ptr to allocated memory
 */
extern Ptr mem_createSize(const mem_Allocator* alloc, usize elem_size);

/**
 * Try to create a new element with size
 *
 * @param alloc Allocator to use
 * @param elem_size Size of element
 * @param[out] out_ptr Output pointer if successful
 * @return true if allocation successful
 */
extern bool mem_tryCreateSize(const mem_Allocator* alloc, usize elem_size, Ptr* out_ptr);

/**
 * Create a new element with size
 * Returns null on failure
 *
 * @param alloc Allocator to use
 * @param elem_size Size of element
 * @return Ptr to allocated memory or null
 */
extern Ptr mem_createSizeOrNull(const mem_Allocator* alloc, usize elem_size);

/**
 * Destroy a single element
 *
 * @param alloc Allocator to use
 * @param ptr Pointer to element
 */
extern void mem_destroy(const mem_Allocator* alloc, Ptr* ptr);

/**
 * Type-safe element creation
 */
#define mem_create(_alloc, _T) \
    mem_createSize(_alloc, sizeof(_T))

#define mem_createOrNull(_alloc, _T) \
    mem_createSizeOrNull(_alloc, sizeof(_T))

/*========== Multi-Element Operations =======================================*/

/**
 * Allocate multiple elements
 * Raises debug assertion on failure
 *
 * @param alloc Allocator to use
 * @param elem_size Size of each element
 * @param count Number of elements
 * @return Slice view of allocated memory
 */
extern Slice mem_allocSize(const mem_Allocator* alloc, usize elem_size, usize count);

/**
 * Try to allocate multiple elements
 *
 * @param alloc Allocator to use
 * @param elem_size Size of each element
 * @param count Number of elements
 * @param[out] out_slice Output slice if successful
 * @return true if allocation successful
 */
extern bool mem_tryAllocSize(const mem_Allocator* alloc, usize elem_size, usize count, Slice* out_slice);

/**
 * Allocate multiple elements
 * Returns null slice on failure
 *
 * @param alloc Allocator to use
 * @param elem_size Size of each element
 * @param count Number of elements
 * @return Slice view of allocated memory or null slice
 */
extern Slice mem_allocSizeOrNull(const mem_Allocator* alloc, usize elem_size, usize count);

/**
 * Free multiple elements
 *
 * @param alloc Allocator to use
 * @param slice Slice to free
 */
extern void mem_free(const mem_Allocator* alloc, Slice* slice);

/**
 * Reallocate memory to new size
 * Raises debug assertion on failure
 *
 * @param alloc Allocator to use
 * @param slice Current slice
 * @param elem_size Size of each element
 * @param new_count New number of elements
 * @return New slice view
 */
extern Slice mem_reallocSize(const mem_Allocator* alloc, Slice slice, usize elem_size, usize new_count);

/**
 * Try to reallocate memory to new size
 *
 * @param alloc Allocator to use
 * @param slice Current slice
 * @param elem_size Size of each element
 * @param new_count New number of elements
 * @param[out] out_slice Output slice if successful
 * @return true if reallocation successful
 */
extern bool mem_tryReallocSize(const mem_Allocator* alloc, Slice slice, usize elem_size, usize new_count, Slice* out_slice);

/**
 * Type-safe allocation macros
 */
#define mem_alloc(_alloc, _T, _count) \
    mem_allocSize(_alloc, sizeof(_T), _count)

#define mem_tryAlloc(_alloc, _T, _count, _out_slice) \
    mem_tryAllocSize(_alloc, sizeof(_T), _count, _out_slice)

#define mem_allocOrNull(_alloc, _T, _count) \
    mem_allocSizeOrNull(_alloc, sizeof(_T), _count)

#define mem_realloc(_alloc, _slice, _T, _new_count) \
    mem_reallocSize(_alloc, _slice, sizeof(_T), _new_count)

#define mem_tryRealloc(_alloc, _slice, _T, _new_count, _out_slice) \
    mem_tryReallocSize(_alloc, _slice, sizeof(_T), _new_count, _out_slice)

/*========== Raw Memory Operations (when needed) ============================*/

extern anyptr mem_allocRaw(const mem_Allocator* alloc, usize size);                   // Base function
extern void   mem_freeRaw(const mem_Allocator* alloc, anyptr* raw);                   // Base function
extern anyptr mem_reallocRaw(const mem_Allocator* alloc, anyptr raw, usize new_size); // Base function

/*========== Memory Manipulation ============================================*/

/**
 * Copy memory between pointers
 * Raises debug assertion on invalid parameters
 *
 * @param dest Destination pointer
 * @param src Source pointer
 * @param size Number of bytes to copy
 */
extern void mem_copySize(Ptr dest, Ptr src, usize size);

/**
 * Try to copy memory between pointers
 *
 * @param dest Destination pointer
 * @param src Source pointer
 * @param size Number of bytes to copy
 * @return true if copy successful
 */
extern bool mem_tryCopySize(Ptr dest, Ptr src, usize size);

/**
 * Move memory between possibly overlapping regions
 * Raises debug assertion on invalid parameters
 *
 * @param dest Destination pointer
 * @param src Source pointer
 * @param size Number of bytes to move
 */
extern void mem_moveSize(Ptr dest, Ptr src, usize size);

/**
 * Try to move memory between possibly overlapping regions
 *
 * @param dest Destination pointer
 * @param src Source pointer
 * @param size Number of bytes to move
 * @return true if move successful
 */
extern bool mem_tryMoveSize(Ptr dest, Ptr src, usize size);

/**
 * Set memory to a value
 * Raises debug assertion on invalid parameters
 *
 * @param ptr Pointer to memory
 * @param value Value to set
 * @param size Number of bytes to set
 */
extern void mem_setSize(Ptr ptr, i32 value, usize size);

/**
 * Try to set memory to a value
 *
 * @param ptr Pointer to memory
 * @param value Value to set
 * @param size Number of bytes to set
 * @return true if set successful
 */
extern bool mem_trySetSize(Ptr ptr, i32 value, usize size);

/**
 * Compare memory regions
 * Raises debug assertion on invalid parameters
 *
 * @param lhs First pointer
 * @param rhs Second pointer
 * @param size Number of bytes to compare
 * @return <0 if lhs < rhs, 0 if equal, >0 if lhs > rhs
 */
extern i32 mem_cmpSize(Ptr lhs, Ptr rhs, usize size);

/**
 * Try to compare memory regions
 *
 * @param lhs First pointer
 * @param rhs Second pointer
 * @param size Number of bytes to compare
 * @param[out] out_result Comparison result if successful
 * @return true if comparison successful
 */
extern bool mem_tryCmpSize(Ptr lhs, Ptr rhs, usize size, i32* out_result);

/**
 * Type-safe memory operation macros
 */
#define mem_copy(_dest, _src, _T) \
    mem_copySize(_dest, _src, sizeof(_T))

#define mem_tryCopy(_dest, _src, _T) \
    mem_tryCopySize(_dest, _src, sizeof(_T))

#define mem_move(_dest, _src, _T) \
    mem_moveSize(_dest, _src, sizeof(_T))

#define mem_tryMove(_dest, _src, _T) \
    mem_tryMoveSize(_dest, _src, sizeof(_T))

#define mem_set(_ptr, _value, _T) \
    mem_setSize(_ptr, _value, sizeof(_T))

#define mem_trySet(_ptr, _value, _T) \
    mem_trySetSize(_ptr, _value, sizeof(_T))

#define mem_cmp(_lhs, _rhs, _T) \
    mem_cmpSize(_lhs, _rhs, sizeof(_T))

#define mem_tryCmp(_lhs, _rhs, _T, _out_result) \
    mem_tryCmpSize(_lhs, _rhs, sizeof(_T), _out_result)

/*========== Slice Memory Operations ========================================*/

/**
 * Copy contents between slices
 * Raises debug assertion if sizes don't match or invalid parameters
 *
 * @param dest Destination slice
 * @param src Source slice
 */
extern void mem_copySlice(Slice dest, Slice src);

/**
 * Try to copy contents between slices
 *
 * @param dest Destination slice
 * @param src Source slice
 * @return true if copy successful
 */
extern bool mem_tryCopySlice(Slice dest, Slice src);

/**
 * Move contents between possibly overlapping slices
 * Raises debug assertion if sizes don't match or invalid parameters
 *
 * @param dest Destination slice
 * @param src Source slice
 */
extern void mem_moveSlice(Slice dest, Slice src);

/**
 * Try to move contents between possibly overlapping slices
 *
 * @param dest Destination slice
 * @param src Source slice
 * @return true if move successful
 */
extern bool mem_tryMoveSlice(Slice dest, Slice src);

/**
 * Set all elements in slice to a value
 * Raises debug assertion if invalid parameters
 *
 * @param slice Slice to modify
 * @param value Byte value to set
 */
extern void mem_setSlice(Slice slice, i32 value);

/**
 * Try to set all elements in slice to a value
 *
 * @param slice Slice to modify
 * @param value Byte value to set
 * @return true if set successful
 */
extern bool mem_trySetSlice(Slice slice, i32 value);

/**
 * Compare contents of two slices
 * Raises debug assertion if invalid parameters
 *
 * @param lhs First slice
 * @param rhs Second slice
 * @return <0 if lhs < rhs, 0 if equal, >0 if lhs > rhs
 */
extern i32 mem_cmpSlice(Slice lhs, Slice rhs);

/**
 * Try to compare contents of two slices
 *
 * @param lhs First slice
 * @param rhs Second slice
 * @param[out] out_result Comparison result if successful
 * @return true if comparison successful
 */
extern bool mem_tryCmpSlice(Slice lhs, Slice rhs, i32* out_result);

/*========== Type Checking ==================================================*/

/**
 * Check if element size matches type size
 *
 * @param elem_size Element size to check
 * @param type_size Expected type size
 * @return true if sizes match
 */
extern bool mem_isTypeSize(usize elem_size, usize type_size);

/**
 * Type-safe size checking macro
 */
#define mem_isType(_ptrOrSlice, _T) \
    mem_isTypeSize(Ptr_size((_ptrOrSlice).ptr_), sizeof(_T))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_INCLUDED */

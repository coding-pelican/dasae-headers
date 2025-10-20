/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Fixed.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-21 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Fixed
 *
 * @brief   Fixed buffer allocator that allocates from a predefined memory region
 * @details Provides a simple bump allocator that manages a fixed-size buffer.
 *          Supports allocation, resize, and free operations with appropriate constraints.
 */

#ifndef HEAP_FIXED_INCLUDED
#define HEAP_FIXED_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Fixed Buffer Allocator =========================================*/

/// Fixed buffer allocator instance
typedef struct heap_Fixed {
    Sli$u8 buffer;    ///< Underlying buffer to allocate from
    usize  end_index; ///< Current allocation position
} heap_Fixed;
use_Opt$(heap_Fixed);
use_Err$(heap_Fixed);

/// Get allocator interface for instance
extern fn_((heap_Fixed_allocator(heap_Fixed* self))(mem_Allocator));
/// Get thread-safe allocator interface for instance (only allocate is thread-safe)
extern fn_((heap_Fixed_thrdSafeAllocator(heap_Fixed* self))(mem_Allocator));

/// Initialize with buffer
extern fn_((heap_Fixed_init(Sli$u8 buf))(heap_Fixed));
/// Reset allocator state (frees all allocations)
extern fn_((heap_Fixed_reset(heap_Fixed* self))(void));
/// Check if allocator owns a pointer
extern fn_((heap_Fixed_ownsPtr(const heap_Fixed* self, Ptr_const$u8 ptr))(bool));
/// Check if allocator owns a slice
extern fn_((heap_Fixed_ownsSli(const heap_Fixed* self, Sli_const$u8 sli))(bool));
/// Check if a slice is the last allocation
extern fn_((heap_Fixed_isLastAllocation(const heap_Fixed* self, Sli_const$u8 buf))(bool));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HEAP_FIXED_INCLUDED */

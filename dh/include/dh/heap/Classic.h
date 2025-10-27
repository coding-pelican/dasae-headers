/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Classic.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2025-03-20 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Classic
 *
 * @brief   Classic C standard library allocator wrapper
 * @details Wraps the C standard library allocator functions
 *          for type-safe memory management. Provides a simple
 *          interface for allocating and freeing memory.
 */

#ifndef heap_Classic__included
#define heap_Classic__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Classic C Allocator ============================================*/

/// Classic allocator instance (minimal state)
typedef struct heap_Classic {
    Void unused_[0]; /* Empty struct not allowed in C */
} heap_Classic;

/// Get allocator interface for instance
extern fn_((heap_Classic_allocator(heap_Classic* self))(mem_Allocator));

/*========== Macros and Declarations ========================================*/

#if defined(__GLIBC__) || defined(__APPLE__)
#define heap_Classic_has_malloc_size (1)
#else
#define heap_Classic_has_malloc_size (0)
#endif

#if heap_Classic_has_malloc_size
#if defined(__GLIBC__)
#elif defined(__APPLE__)
#include <malloc/malloc.h>
#else
#endif
/// Get underlying malloc_size if available
$inline_always fn_((heap_Classic_mallocSize(P$raw ptr))(usize)) {
#if defined(__GLIBC__)
    return malloc_usable_size(ptr);
#elif defined(__APPLE__)
    return malloc_size(ptr);
#else
    return 0; // Fallback
#endif
}
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Classic__included */

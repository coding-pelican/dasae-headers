/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Classic.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2024-12-29 (date of last update)
 * @version v0.1-alpha
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

#include "cfg.h"

#include <stdlib.h>

/*========== Classic C Allocator ============================================*/

/* Classic allocator instance (minimal state) */
typedef struct heap_Classic {
    Void unused_[0]; // Empty struct not allowed in C
} heap_Classic;

// Get allocator interface for instance
extern mem_Allocator heap_Classic_allocator(heap_Classic* self);

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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HEAP_CLASSIC_INCLUDED */

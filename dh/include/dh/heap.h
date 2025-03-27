/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    heap.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-27 (date of creation)
 * @updated 2025-03-27 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  heap
 *
 * @brief   Memory allocation and heap management utilities
 * @details Provides a comprehensive set of heap management implementations:
 *          - Classic: Traditional heap allocation with memory tracking
 *          - Page: Page-aligned memory allocation for large blocks
 *          - Fixed: Fixed-size block allocator for efficient memory reuse
 *          - Arena: Region-based memory allocation for bulk operations
 *
 *          All implementations follow strict memory safety practices and
 *          include built-in error detection. Memory leaks are prevented
 *          through explicit allocation tracking and deallocation checks.
 */

#ifndef HEAP_INCLUDED
#define HEAP_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "heap/cfg.h"
#include "heap/Classic.h"
#include "heap/Page.h"
#include "heap/Fixed.h"
#include "heap/Arena.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HEAP_INCLUDED */

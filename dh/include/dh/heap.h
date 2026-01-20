/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    heap.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-27 (date of creation)
 * @updated 2026-01-20 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  heap
 *
 * @brief   Memory allocation and heap management utilities
 * @details Provides a comprehensive set of heap management implementations:
 *          - Classic: Traditional heap allocation (C-Runtime-dependent)
 *          - Page: Page-aligned allocation for mapped memory blocks (OS-dependent)
 *          - Sbrk: Sbrk-based allocation for linear memory growth and reuse (context on OS-dependent)
 *          - Sys: System-dependent allocation (Page/Sbrk)
 *          - Fixed: Fixed-size block allocator for bulk operations
 *          - ThrdSafe: Thread-safe allocator for multi-threaded applications
 *          - Smp: Symmetric-multiprocessing (SMP) memory allocation and reuse
 *          - Arena: Region-based memory allocation for bulk operations
 *          - Pool: Pool-based memory allocation for bulk operations and reuse
 *
 * @see `dh-extras/heap/TLSF` for more efficiently reusable memory allocator implementation
 */
#ifndef heap__included
#define heap__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "heap/cfg.h"
#if heap_Classic_enabled
#include "heap/Classic.h"
#endif /* heap_Classic_enabled */
#include "heap/Page.h"
#include "heap/Sbrk.h"
#include "heap/Sys.h"
#include "heap/Fixed.h"
#include "heap/ThrdSafe.h"
#include "heap/Smp.h"
#include "heap/Arena.h"
#include "heap/Pool.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap__included */

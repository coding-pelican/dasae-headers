/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Page.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-15 (date of creation)
 * @updated 2025-03-20 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Page
 *
 * @brief   Page allocator using OS virtual memory APIs
 * @details Uses OS-level virtual memory APIs to allocate memory in page-sized blocks.
 *          Provides a simple interface for allocating and freeing memory.
 */

#ifndef HEAP_PAGE_INCLUDED
#define HEAP_PAGE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Page Allocator =================================================*/

/// Page allocator instance (minimal state, following Zig's pattern)
typedef struct heap_Page {
    Void unused_[0]; /* Empty struct not allowed in C */
} heap_Page;

/// Get allocator interface for instance
extern fn_(heap_Page_allocator(heap_Page* self), mem_Allocator);

/*========== Implementation Details =========================================*/

// Get next virtual memory address hint
static anyptr heap_Page_s_next_mmap_addr_hint = null;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HEAP_PAGE_INCLUDED */

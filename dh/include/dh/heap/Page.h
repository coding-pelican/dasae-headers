/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Page.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
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
#ifndef heap_Page__included
#define heap_Page__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

/// Page allocator instance
T_alias$((heap_Page)(struct heap_Page {
    /// Hint for the next address to map
    /// When first using the allocator, this is must be `null`
    var_(next_addr_hint_, usize);
}));
$static let_(heap_Page_default, heap_Page) = { .next_addr_hint_ = ptrToInt(null) };
/// Get allocator interface for instance
$extern fn_((heap_Page_alctr(heap_Page* self))(mem_Alctr));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Page__included */

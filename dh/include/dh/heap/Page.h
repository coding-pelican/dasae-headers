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
#pragma once
#ifndef heap_Page__included
#define heap_Page__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "VMap.h"

/*========== Macros and Declarations ========================================*/

/// Page allocator instance
T_alias$((heap_Page)(struct heap_Page {
    var_(vmap, heap_VMap);
    /// Hint for the next address to map
    /// When first using the allocator, this is must be `null`
    var_(_next_addr_hint, usize);
}));
/// Get allocator interface for instance
$extern fn_((heap_Page_alctr(heap_Page* self))(mem_Alctr));
/// Get thread-safe allocator interface for instance
$extern fn_((heap_Page_thrdSafeAlctr(heap_Page* self))(mem_Alctr));

$extern fn_((heap_Page_from(heap_VMap vmap))(heap_Page));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Page__included */

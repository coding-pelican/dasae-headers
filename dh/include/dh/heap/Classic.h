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
#pragma once
#ifndef heap_Classic__included
#define heap_Classic__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

#define heap_Classic_enabled __comp_bool__heap_Classic_enabled
errset_((heap_Classic_E)(heap_Classic_Unsupported));

/// Classic allocator instance
T_alias$((heap_Classic)(struct heap_Classic {
    var_(_unused, Void); /* Empty struct not allowed in C */
}));
T_use_E$($set(heap_Classic_E)(mem_Alctr));
$static let_(heap_Classic_default, heap_Classic) = cleared();
/// Get allocator interface for instance
$attr($must_check)
$extern fn_((heap_Classic_alctr(heap_Classic* self))(heap_Classic_E$mem_Alctr));

/*========== Macros and Definitions =========================================*/

#define __comp_bool__heap_Classic_enabled comp_libc_linked

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Classic__included */

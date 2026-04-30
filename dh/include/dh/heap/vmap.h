/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    vmap.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-29 (date of creation)
 * @updated 2026-04-29 (date of last update)
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_vmap
 *
 * @brief   Writable page-mapped region surface
 * @details Exposes page-aligned map, remap, and release operations for
 *          writable mapped regions. This surface is weaker than `heap_vmem`
 *          and is the intended substrate for page-backed allocation paths.
 */
#ifndef heap_vmap__included
#define heap_vmap__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

$extern fn_((heap_vmap_map(P$raw addr_hint, usize len))(O$P$u8));
$extern fn_((heap_vmap_release(P$raw addr, usize len))(bool));
$extern fn_((heap_vmap_remap(P$raw addr, usize old_len, usize new_len))(O$P$u8));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_vmap__included */

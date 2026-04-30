/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    vmem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-27 (date of creation)
 * @updated 2026-04-27 (date of last update)
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_vmem
 *
 * @brief   Virtual memory address-space control surface
 * @details Exposes sparse reserve, commit, decommit, protect, and release
 *          operations for page-backed virtual address-space management.
 */
#ifndef heap_vmem__included
#define heap_vmem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

T_alias$((heap_vmem_Protect)(enum_((heap_vmem_Protect $fits($packed))(
    heap_vmem_Protect_none,
    heap_vmem_Protect_read_write,
    heap_vmem_Protect_read_write_guard
))));

$extern fn_((heap_vmem_reserve(P$raw addr_hint, usize len))(O$P$u8));
$extern fn_((heap_vmem_commit(P$raw addr, usize len))(bool));
$extern fn_((heap_vmem_decommit(P$raw addr, usize len))(bool));
$extern fn_((heap_vmem_protect(P$raw addr, usize len, heap_vmem_Protect protect))(bool));
$extern fn_((heap_vmem_release(P$raw addr, usize len))(bool));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_vmem__included */

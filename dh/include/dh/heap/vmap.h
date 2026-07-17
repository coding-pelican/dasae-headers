/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    VMap.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-29 (date of creation)
 * @updated 2026-04-29 (date of last update)
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_VMap
 *
 * @brief   Writable page-mapped region surface
 * @details Exposes page-aligned map, remap, and release operations for
 *          writable mapped regions. This surface is weaker than `heap_VMem`
 *          and is the intended substrate for page-backed allocation paths.
 */
#pragma once
#ifndef heap_VMap__included
#define heap_VMap__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

errset_((heap_VMap_E)(heap_VMap_Unsupported));

T_alias$((heap_VMap_VTbl)(struct heap_VMap_VTbl));
T_alias$((heap_VMap)(struct heap_VMap {
    var_(geom, heap_Geom);
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(heap_VMap_VTbl));
}));
T_use_E$($set(heap_VMap_E)(heap_VMap));
$attr($inline_always)
$static fn_((heap_VMap_isValid(heap_VMap self))(bool));
$attr($inline_always)
$static fn_((heap_VMap_assertValid(heap_Geom geom, P$raw ctx, P_const$$(heap_VMap_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((heap_VMap_ensureValid(heap_VMap self))(heap_VMap));

$attr($must_check)
$extern fn_((heap_VMap_system(void))(heap_VMap_E$heap_VMap));
$extern fn_((heap_VMap_geom(heap_VMap self))(heap_Geom));
$extern fn_((heap_VMap_map(heap_VMap self, O$P$raw addr_hint, usize len))(O$P$u8));
$extern fn_((heap_VMap_release(heap_VMap self, P$raw addr, usize len))(bool));
$extern fn_((heap_VMap_remap(heap_VMap self, P$raw addr, usize old_len, usize new_len))(O$P$u8));

struct heap_VMap_VTbl {
    fn_(((*mapFn)(P$raw ctx, O$P$raw addr_hint, usize len))(O$P$u8));
    fn_(((*releaseFn)(P$raw ctx, P$raw addr, usize len))(bool));
    fn_(((*remapFn)(P$raw ctx, P$raw addr, usize old_len, usize new_len))(O$P$u8));
};

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((heap_VMap_isValid(heap_VMap self))(bool)) {
    return heap_Geom_isValid(self.geom)
        && isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->mapFn)
        && isNonnull(self.vtbl->releaseFn)
        && isNonnull(self.vtbl->remapFn);
};
fn_((heap_VMap_assertValid(heap_Geom geom, P$raw ctx, P_const$$(heap_VMap_VTbl) vtbl))(void)) {
    heap_Geom_assertValid(geom.page_size, geom.reserve_align, geom.commit_align, geom.map_align);
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->mapFn);
    claim_assert_nonnull(vtbl->releaseFn);
    claim_assert_nonnull(vtbl->remapFn);
};
fn_((heap_VMap_ensureValid(heap_VMap self))(heap_VMap)) {
    return heap_VMap_assertValid(self.geom, self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_VMap__included */

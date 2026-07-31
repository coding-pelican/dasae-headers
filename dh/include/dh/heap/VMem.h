/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    VMem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-27 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_VMem
 *
 * @brief   Virtual memory address-space control surface
 * @details Exposes sparse reserve, commit, decommit, protect, and release
 *          operations for page-backed virtual address-space management.
 */
#pragma once
#ifndef heap_VMem__included
#define heap_VMem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

errset_((heap_VMem_E)(heap_VMem_Unsupported));

T_alias$((heap_VMem_Protcn)(enum_((heap_VMem_Protcn $fits($packed))(
    heap_VMem_Protcn_none,
    heap_VMem_Protcn_read_write,
    heap_VMem_Protcn_read_write_guard
))));

T_alias$((heap_VMem_Ctx)(struct heap_VMem_Ctx {
    var_(inner, P$raw);
    var_(geom, heap_Geom);
}));
$attr($inline_always)
$static fn_((heap_VMem_Ctx_isValid(heap_VMem_Ctx self))(bool));
$attr($inline_always)
$static fn_((heap_VMem_Ctx_assertValid(P$raw inner, heap_Geom geom))(void));
$attr($inline_always)
$static fn_((heap_VMem_Ctx_ensureValid(heap_VMem_Ctx self))(heap_VMem_Ctx));

T_alias$((heap_VMem_VTbl)(struct heap_VMem_VTbl));
T_alias$((heap_VMem)(struct heap_VMem {
    var_(ctx, const heap_VMem_Ctx*);
    var_(vtbl, P_const$$(heap_VMem_VTbl));
}));
T_use_E$($set(heap_VMem_E)(heap_VMem));
$attr($inline_always)
$static fn_((heap_VMem_isValid(heap_VMem self))(bool));
$attr($inline_always)
$static fn_((heap_VMem_assertValid(const heap_VMem_Ctx* ctx, P_const$$(heap_VMem_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((heap_VMem_ensureValid(heap_VMem self))(heap_VMem));

$attr($must_check)
$extern fn_((heap_VMem_system(void))(heap_VMem_E$heap_VMem));
$extern fn_((heap_VMem_geom(heap_VMem self))(heap_Geom));
$extern fn_((heap_VMem_reserve(heap_VMem self, O$P$raw addr_hint, usize len))(O$P$u8));
$extern fn_((heap_VMem_commit(heap_VMem self, P$raw addr, usize len))(bool));
$extern fn_((heap_VMem_decommit(heap_VMem self, P$raw addr, usize len))(bool));
$extern fn_((heap_VMem_protect(heap_VMem self, P$raw addr, usize len, heap_VMem_Protcn protect))(bool));
$extern fn_((heap_VMem_release(heap_VMem self, P$raw addr, usize len))(bool));

struct heap_VMem_VTbl {
    fn_(((*reserveFn)(const heap_VMem_Ctx* ctx, O$P$raw addr_hint, usize len))(O$P$u8));
    fn_(((*commitFn)(const heap_VMem_Ctx* ctx, P$raw addr, usize len))(bool));
    fn_(((*decommitFn)(const heap_VMem_Ctx* ctx, P$raw addr, usize len))(bool));
    fn_(((*protectFn)(const heap_VMem_Ctx* ctx, P$raw addr, usize len, heap_VMem_Protcn protect))(bool));
    fn_(((*releaseFn)(const heap_VMem_Ctx* ctx, P$raw addr, usize len))(bool));
};

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((heap_VMem_Ctx_isValid(heap_VMem_Ctx self))(bool)) {
    return isNonnull(self.inner)
        && heap_Geom_isValid(self.geom);
};
fn_((heap_VMem_Ctx_assertValid(P$raw inner, heap_Geom geom))(void)) {
    claim_assert_nonnull(inner);
    heap_Geom_assertValid(geom.page_size, geom.reserve_align, geom.commit_align, geom.map_align);
};
fn_((heap_VMem_Ctx_ensureValid(heap_VMem_Ctx self))(heap_VMem_Ctx)) {
    return heap_VMem_Ctx_assertValid(self.inner, self.geom), self;
};

fn_((heap_VMem_isValid(heap_VMem self))(bool)) {
    return isNonnull(self.ctx) && heap_VMem_Ctx_isValid(*self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->reserveFn)
        && isNonnull(self.vtbl->commitFn)
        && isNonnull(self.vtbl->decommitFn)
        && isNonnull(self.vtbl->protectFn)
        && isNonnull(self.vtbl->releaseFn);
};
fn_((heap_VMem_assertValid(const heap_VMem_Ctx* ctx, P_const$$(heap_VMem_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx), heap_VMem_Ctx_assertValid(ctx->inner, ctx->geom);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->reserveFn);
    claim_assert_nonnull(vtbl->commitFn);
    claim_assert_nonnull(vtbl->decommitFn);
    claim_assert_nonnull(vtbl->protectFn);
    claim_assert_nonnull(vtbl->releaseFn);
};
fn_((heap_VMem_ensureValid(heap_VMem self))(heap_VMem)) {
    return heap_VMem_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_VMem__included */

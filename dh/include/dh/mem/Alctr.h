/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Alctr.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2026-02-09 (date of last update)
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_Alctr
 *
 * @brief   Memory allocator interface using smart pointers
 * @details Provides type-safe mem allocation and management
 */
#ifndef mem_Alctr__included
#define mem_Alctr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

/* --- Interface VTable --- */

/// Alctr vtable
T_alias$((mem_Alctr_VTbl)(struct mem_Alctr_VTbl));
$extern let_(mem_Alctr_VTbl_noop, mem_Alctr_VTbl);
$extern let_(mem_Alctr_VTbl_failing, mem_Alctr_VTbl);

/* --- Interface Instance --- */

/// Alctr instance
T_alias$((mem_Alctr)(struct mem_Alctr {
    var_(ctx, P$raw); /**< Context pointer */
    var_(vtbl, P_const$$(mem_Alctr_VTbl)); /**< Virtual table */
}));
T_use_O$(mem_Alctr);
T_use_E$($set(mem_E)(u_P$raw));
T_use_E$($set(mem_E)(u_S$raw));
$extern let_(mem_Alctr_noop, mem_Alctr);
$extern let_(mem_Alctr_failing, mem_Alctr);

$attr($inline_always)
$static fn_((mem_Alctr_isValid(mem_Alctr self))(bool));
$attr($inline_always)
$static fn_((mem_Alctr_assertValid(P$raw ctx, P_const$$(mem_Alctr_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((mem_Alctr_ensureValid(mem_Alctr self))(mem_Alctr));

/* --- Core Operations --- */

/// Raw allocation
$extern fn_((mem_Alctr_rawAlloc($traced mem_Alctr self, usize len, mem_Align align))(O$P$u8));
/// Try to resize in-place
$extern fn_((mem_Alctr_rawResize($traced mem_Alctr self, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
/// Try to resize, allowing relocation
$extern fn_((mem_Alctr_rawRemap($traced mem_Alctr self, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
/// Free mem
$extern fn_((mem_Alctr_rawFree($traced mem_Alctr self, S$u8 buf, mem_Align buf_align))(void));

/* --- High-level Operations --- */

/// Single-item allocation
$attr($must_check)
$extern fn_((mem_Alctr_create($traced mem_Alctr self, TypeInfo type))(mem_E$u_P$raw));
#define T_use_mem_Alctr_create$(_T...) __comp_gen__T_use_mem_Alctr_create$(_T)
/// Free single-item
$extern fn_((mem_Alctr_destroy($traced mem_Alctr self, u_P$raw ptr))(void));
#define T_use_mem_Alctr_destroy$(_T...) __comp_gen__T_use_mem_Alctr_destroy$(_T)
/// Duplicate single-item
$attr($must_check)
$extern fn_((mem_Alctr_clone($traced mem_Alctr self, u_P_const$raw ptr))(mem_E$u_P$raw));
#define T_use_mem_Alctr_clone$(_T...) __comp_gen__T_use_mem_Alctr_clone$(_T)
/// Slice allocation
$attr($must_check)
$extern fn_((mem_Alctr_alloc($traced mem_Alctr self, TypeInfo type, usize count))(mem_E$u_S$raw));
#define T_use_mem_Alctr_alloc$(_T...) __comp_gen__T_use_mem_Alctr_alloc$(_T)
/// Try to resize slice in-place
$extern fn_((mem_Alctr_resize($traced mem_Alctr self, u_S$raw old_mem, usize new_len))(bool));
#define T_use_mem_Alctr_resize$(_T...) __comp_gen__T_use_mem_Alctr_resize$(_T)
/// Try to resize slice, allowing relocation
$extern fn_((mem_Alctr_remap($traced mem_Alctr self, u_S$raw old_mem, usize new_len))(O$u_S$raw));
#define T_use_mem_Alctr_remap$(_T...) __comp_gen__T_use_mem_Alctr_remap$(_T)
/// Reallocate slice with new size
$attr($must_check)
$extern fn_((mem_Alctr_realloc($traced mem_Alctr self, u_S$raw old_mem, usize new_len))(mem_E$u_S$raw));
#define T_use_mem_Alctr_realloc$(_T...) __comp_gen__T_use_mem_Alctr_realloc$(_T)
/// Free slice
$extern fn_((mem_Alctr_free($traced mem_Alctr self, u_S$raw mem))(void));
#define T_use_mem_Alctr_free$(_T...) __comp_gen__T_use_mem_Alctr_free$(_T)
/// Duplicate slice
$attr($must_check)
$extern fn_((mem_Alctr_dupe($traced mem_Alctr self, u_S_const$raw src))(mem_E$u_S$raw));
#define T_use_mem_Alctr_dupe$(_T...) __comp_gen__T_use_mem_Alctr_dupe$(_T)

struct mem_Alctr_VTbl {
    fn_(((*const alloc)(P$raw ctx, usize len, mem_Align align))(O$P$u8));
    fn_(((*const resize)(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
    fn_(((*const remap)(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
    fn_(((*const free)(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));
};

/// Default VTable functions for no-op fallbacks
$extern fn_((mem_Alctr_VTbl_noAlloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
/// Default VTable functions for no-op fallbacks
$extern fn_((mem_Alctr_VTbl_noResize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
/// Default VTable functions for unreachable fallbacks
$extern fn_((mem_Alctr_VTbl_unreachableResize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
/// Default VTable functions for no-op fallbacks
$extern fn_((mem_Alctr_VTbl_noRemap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
/// Default VTable functions for unreachable fallbacks
$extern fn_((mem_Alctr_VTbl_unreachableRemap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
/// Default VTable functions for no-op fallbacks
$extern fn_((mem_Alctr_VTbl_noFree(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));
/// Default VTable functions for unreachable fallbacks
$extern fn_((mem_Alctr_VTbl_unreachableFree(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

/*========== Macros and Definitions =========================================*/

fn_((mem_Alctr_isValid(mem_Alctr self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->alloc)
        && isNonnull(self.vtbl->resize)
        && isNonnull(self.vtbl->remap)
        && isNonnull(self.vtbl->free);
};

fn_((mem_Alctr_assertValid(P$raw ctx, P_const$$(mem_Alctr_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->alloc);
    claim_assert_nonnull(vtbl->resize);
    claim_assert_nonnull(vtbl->remap);
    claim_assert_nonnull(vtbl->free);
};

fn_((mem_Alctr_ensureValid(mem_Alctr self))(mem_Alctr)) {
    return mem_Alctr_assertValid(self.ctx, self.vtbl), self;
};

/* clang-format off */
#define __comp_gen__T_use_mem_Alctr_create$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(mem_Alctr_create, _T)($traced mem_Alctr self))(E$($set(mem_E)(P$(_T)))) $scope) { \
        return u_castE$((ReturnT)(mem_Alctr_create($tracing self, typeInfo$(_T)))); \
    } $unscoped(fn)
#define __comp_gen__T_use_mem_Alctr_destroy$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(mem_Alctr_destroy, _T)($traced mem_Alctr self, P$(_T) ptr))(void)) { \
        return mem_Alctr_destroy($tracing self, u_anyP(ptr)); \
    }
#define __comp_gen__T_use_mem_Alctr_clone$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(mem_Alctr_clone, _T)($traced mem_Alctr self, P_const$(_T) ptr))(E$($set(mem_E)(P$(_T)))) $scope) { \
        return u_castE$((ReturnT)(mem_Alctr_clone($tracing self, u_anyP(ptr)))); \
    } $unscoped(fn)
#define __comp_gen__T_use_mem_Alctr_alloc$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(mem_Alctr_alloc, _T)($traced mem_Alctr self, usize count))(E$($set(mem_E)(S$(_T)))) $scope) { \
        return u_castE$((ReturnT)(mem_Alctr_alloc($tracing self, typeInfo$(_T), count))); \
    } $unscoped(fn)
#define __comp_gen__T_use_mem_Alctr_resize$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(mem_Alctr_resize, _T)($traced mem_Alctr self, S$(_T) old_mem, usize new_len))(bool)) { \
        return mem_Alctr_resize($tracing self, u_anyS(old_mem), new_len); \
    }
#define __comp_gen__T_use_mem_Alctr_remap$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(mem_Alctr_remap, _T)($traced mem_Alctr self, S$(_T) old_mem, usize new_len))(O$(S$(_T))) $scope) { \
        return u_castO$((ReturnT)(mem_Alctr_remap($tracing self, u_anyS(old_mem), new_len))); \
    } $unscoped(fn)
#define __comp_gen__T_use_mem_Alctr_realloc$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(mem_Alctr_realloc, _T)($traced mem_Alctr self, S$(_T) old_mem, usize new_len))(E$($set(mem_E)(S$(_T)))) $scope) { \
        return u_castE$((ReturnT)(mem_Alctr_realloc($tracing self, u_anyS(old_mem), new_len))); \
    } $unscoped(fn)
#define __comp_gen__T_use_mem_Alctr_free$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(mem_Alctr_free, _T)($traced mem_Alctr self, S$(_T) mem))(void)) { \
        return mem_Alctr_free($tracing self, u_anyS(mem)); \
    }
#define __comp_gen__T_use_mem_Alctr_dupe$(_T...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl$(mem_Alctr_dupe, _T)($traced mem_Alctr self, S_const$(_T) src))(E$($set(mem_E)(S$(_T)))) $scope) { \
        return u_castE$((ReturnT)(mem_Alctr_dupe($tracing self, u_anyS(src)))); \
    } $unscoped(fn)
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_Alctr__included */

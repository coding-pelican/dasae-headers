#pragma once
#ifndef mem_dyn__included
#define mem_dyn__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

#define mem_dyn_initCapForSize_static(_elem_size /*: usize*/) \
    __val__mem_dyn_initCapForSize_static(_elem_size)
$attr($inline_always)
$static fn_((mem_dyn_initCapForSize(usize elem_size))(usize));
#define mem_dyn_initCap_static$(_T...) __val__mem_dyn_initCap_static$(_T)
#define mem_dyn_initCap_static(_type /*: TypeInfo*/...) \
    __val__mem_dyn_initCap_static(_type)
$attr($inline_always)
$static fn_((mem_dyn_initCap(TypeInfo type))(usize));
#define T_use_mem_dyn_initCap$(_T...) __stmt__T_use_mem_dyn_initCap$(_T)

$attr($inline_always)
$static fn_((mem_dyn_growCapFrom(usize init_cap, usize curr, usize min))(usize));
$attr($inline_always)
$static fn_((mem_dyn_growCap(TypeInfo type, usize curr, usize min))(usize));
#define T_use_mem_dyn_growCap$(_T...) __stmt__T_use_mem_dyn_growCap$(_T)

T_alias$((mem_dyn_E$usize)(E$$($set(mem_E)(usize))));
$attr($must_check $inline_always)
$static fn_((mem_dyn_addOrOOM(usize lhs, usize rhs))(mem_dyn_E$usize));

/*========== Macros and Definitions =========================================*/

#define mem_dyn__nonZeroSize(_is_static /*: pp_bool*/, _T_size /*: usize*/...) \
    pp_if_(_is_static)( \
        pp_then_(pri_max_static), \
        pp_else_(pri_max) \
    )(usize_(1), (_T_size))
#define mem_dyn__initCap(_is_static /*: pp_bool*/, _T_size /*: usize*/...) \
    pp_if_(_is_static)( \
        pp_then_(pri_max_static), \
        pp_else_(pri_max) \
    )(usize_(1), usize_(arch_cache_line_bytes) / mem_dyn__nonZeroSize(_is_static, _T_size))

#define __val__mem_dyn_initCapForSize_static(_elem_size) \
    mem_dyn__initCap(pp_true, _elem_size)
#define __val__mem_dyn_initCap_static$(_T...) \
    mem_dyn_initCapForSize_static(sizeOf$(_T))
#define __val__mem_dyn_initCap_static(_type...) \
    mem_dyn_initCapForSize_static(TypeInfo_size(_type))

#if in_analysis_active_only || in_comptime
fn_((mem_dyn_initCapForSize(usize elem_size))(usize)) {
    return mem_dyn__initCap(pp_false, elem_size);
};
fn_((mem_dyn_initCap(TypeInfo type))(usize)) {
    return mem_dyn_initCapForSize(type.size);
};
fn_((mem_dyn_growCapFrom(usize init_cap, usize curr, usize min))(usize)) {
    claim_assert(init_cap != 0);
    var_(grown, usize) = curr;
    do { grown = usize_addSat(grown, grown / 2 + init_cap); } while (grown < min);
    return grown;
};
fn_((mem_dyn_growCap(TypeInfo type, usize curr, usize min))(usize)) {
    return mem_dyn_growCapFrom(mem_dyn_initCap(type), curr, min);
};

fn_((mem_dyn_addOrOOM(usize lhs, usize rhs))(mem_dyn_E$usize) $scope) {
    if_some((usize_addChkd(lhs, rhs))(result)) {
        return_ok(result);
    } else_none {
        return_err(E_cause$OutOfMemory());
    }
    claim_unreachable;
} $unscoped(fn);
#endif /* in_analysis_active_only || in_comptime */

#define __stmt__T_use_mem_dyn_initCap$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_dyn_initCap, _T))(void)) { \
        return mem_dyn_initCapForSize(sizeOf$(_T)); \
    }
#define __stmt__T_use_mem_dyn_growCap$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_dyn_growCap, _T))(usize curr, usize min)) { \
        return mem_dyn_growCap(typeInfo$(_T), curr, min); \
    }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_dyn__included */

#pragma once
#ifndef mem_dyn__included
#define mem_dyn__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

#define mem_dyn_initCap_static$(_T...) __val__mem_dyn_initCap_static$(_T)
#define mem_dyn_initCap_static(_type /*: TypeInfo*/...) __val__mem_dyn_initCap_static(_type)
$attr($inline_always)
$static fn_((mem_dyn_initCap(TypeInfo type))(usize));
#define mem_dyn_initCap$(_T...) __stmt__mem_dyn_initCap$(_T)
$attr($inline_always)
$static fn_((mem_dyn_growCap(TypeInfo type, usize curr, usize min))(usize));
#define mem_dyn_growCap$(_T...) __stmt__mem_dyn_growCap$(_T)

T_alias$((mem_dyn_E$usize)(E$$($set(mem_E)(usize))));
$attr($must_check $inline_always)
$static fn_((mem_dyn_addOrOOM(usize lhs, usize rhs))(mem_dyn_E$usize));

/*========== Macros and Definitions =========================================*/

#define mem_dyn__initCap(_is_static /*: pp_bool*/, _T_size /*: usize*/...) \
    pp_if_(_is_static)( \
        pp_then_(pri_max_static), \
        pp_else_(pri_max) \
    )(usize_(1), usize_(arch_cache_line_bytes) / (_T_size))

#define __val__mem_dyn_initCap_static$(_T...) ( \
    mem_dyn__initCap(pp_true, sizeOf$(_T)) \
)
#define __val__mem_dyn_initCap_static(_type /*: TypeInfo*/...) ( \
    mem_dyn__initCap(pp_true, TypeInfo_size(_type)) \
)

#if in_analysis_active_only || in_comptime
fn_((mem_dyn_initCap(TypeInfo type))(usize)) {
    return mem_dyn__initCap(pp_false, type.size);
};
fn_((mem_dyn_growCap(TypeInfo type, usize curr, usize min))(usize)) {
    let init_cap = mem_dyn_initCap(type);
    var_(grown, usize) = curr;
    do { grown = usize_addSat(grown, grown / 2 + init_cap); } while (grown < min);
    return grown;
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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_dyn__included */

#pragma once
#ifndef mem_seq__included
#define mem_seq__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

$attr($inline_always)
$static fn_((mem_seq_set0Range(u_S$raw seq, usize idx, usize count))(void));
#define T_use_mem_seq_set0Range$(_T...) __stmt__T_use_mem_seq_set0Range$(_T)
$attr($inline_always)
$static fn_((mem_seq_copyRange(
    u_S$raw dst, usize dst_idx,
    u_S_const$raw src, usize src_idx,
    usize count
))(void));
#define T_use_mem_seq_copyRange$(_T...) __stmt__T_use_mem_seq_copyRange$(_T)
$attr($inline_always)
$static fn_((mem_seq_moveRange(
    u_S$raw seq, usize dst_idx, usize src_idx, usize count
))(void));
#define T_use_mem_seq_moveRange$(_T...) __stmt__T_use_mem_seq_moveRange$(_T)
$attr($inline_always)
$static fn_((mem_seq_swapAt(u_S$raw seq, usize lhs, usize rhs))(void));
#define T_use_mem_seq_swapAt$(_T...) __stmt__T_use_mem_seq_swapAt$(_T)

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((mem_seq_set0Range(u_S$raw seq, usize idx, usize count))(void)) {
    claim_assert(idx <= seq.len && count <= seq.len - idx);
    if (count == 0 || seq.type.size == 0) return;
    u_memset0S(u_sliceS(seq, $r(idx, idx + count)));
};
fn_((mem_seq_copyRange(
    u_S$raw dst, usize dst_idx,
    u_S_const$raw src, usize src_idx,
    usize count
))(void)) {
    claim_assert(TypeInfo_eql(dst.type, src.type));
    claim_assert(dst_idx <= dst.len && count <= dst.len - dst_idx);
    claim_assert(src_idx <= src.len && count <= src.len - src_idx);
    if (count == 0 || dst.type.size == 0) return;
    u_memcpyS(
        u_sliceS(dst, $r(dst_idx, dst_idx + count)),
        u_sliceS(src, $r(src_idx, src_idx + count))
    );
};
fn_((mem_seq_moveRange(
    u_S$raw seq, usize dst_idx, usize src_idx, usize count
))(void)) {
    claim_assert(dst_idx <= seq.len && count <= seq.len - dst_idx);
    claim_assert(src_idx <= seq.len && count <= seq.len - src_idx);
    if (count == 0 || dst_idx == src_idx || seq.type.size == 0) return;
    u_memmoveS(
        u_sliceS(seq, $r(dst_idx, dst_idx + count)),
        u_sliceS(seq.as_const, $r(src_idx, src_idx + count))
    );
};
fn_((mem_seq_swapAt(u_S$raw seq, usize lhs, usize rhs))(void)) {
    claim_assert(lhs < seq.len), claim_assert(rhs < seq.len);
    if (lhs == rhs || seq.type.size == 0) return;
    mem_swapP(u_atS(seq, lhs), u_atS(seq, rhs));
};
#endif /* in_analysis_active_only || in_comptime */

/*--- Typed facades ---------------------------------------------------------*/

#define __stmt__T_use_mem_seq_set0Range$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_seq_set0Range, _T)(S$(_T) seq, usize idx, usize count))(void)) { \
        return mem_seq_set0Range(u_anyS(seq), idx, count); \
    }
#define __stmt__T_use_mem_seq_copyRange$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_seq_copyRange, _T)( \
        S$(_T) dst, usize dst_idx, \
        S_const$(_T) src, usize src_idx, \
        usize count \
    ))(void)) { \
        return mem_seq_copyRange(u_anyS(dst), dst_idx, u_anyS(src), src_idx, count); \
    }
#define __stmt__T_use_mem_seq_moveRange$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_seq_moveRange, _T)( \
        S$(_T) seq, usize dst_idx, usize src_idx, usize count \
    ))(void)) { \
        return mem_seq_moveRange(u_anyS(seq), dst_idx, src_idx, count); \
    }
#define __stmt__T_use_mem_seq_swapAt$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(mem_seq_swapAt, _T)(S$(_T) seq, usize lhs, usize rhs))(void)) { \
        return mem_seq_swapAt(u_anyS(seq), lhs, rhs); \
    }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_seq__included */

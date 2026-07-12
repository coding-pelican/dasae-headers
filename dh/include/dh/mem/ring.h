#pragma once
#ifndef mem_ring__included
#define mem_ring__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

$attr($inline_always)
$static fn_((mem_ring_idx(usize cap, usize pos))(usize));
$attr($inline_always)
$static fn_((mem_ring_idxFrom(usize head, usize cap, usize idx))(usize));

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((mem_ring_idx(usize cap, usize pos))(usize)) {
    claim_assert(cap != 0);
    return pos % cap;
};
fn_((mem_ring_idxFrom(usize head, usize cap, usize idx))(usize)) {
    claim_assert(cap != 0);
    claim_assert(head < cap);
    claim_assert(idx <= cap);
    let head_len = cap - head;
    return when_(idx < head_len)(
        provide_(head + idx),
        instead_(idx - head_len)
    );
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_ring__included */

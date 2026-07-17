/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    sort.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2026-02-13 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  sort
 *
 * @brief   Sorting utilities
 * @details Provides optimal stable and unstable sorting functions isolated by
 *          auxiliary memory constraints (O(1), O(K), O(N)).
 *          Supports index-based context sorting for non-contiguous layouts.
 */
#pragma once
#ifndef sort__included
#define sort__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "cmp.h"
#include "mem/Alctr.h"

/*========== Macros and Declarations ========================================*/

#define sort_threshold_fallback_to_insert_sort __const__sort_threshold_fallback_to_insert_sort
#define sort_limit_pdq_stack_frames __const__sort_limit_pdq_stack_frames
#define sort_threshold_pdq_tukey_ninther __const__sort_threshold_pdq_tukey_ninther
#define sort_max_swaps_pdq_choose_pivot __const__sort_max_swaps_pdq_choose_pivot
#define sort_max_steps_pdq_partial_insert_sort __const__sort_max_steps_pdq_partial_insert_sort
#define sort_threshold_pdq_partial_insert_sort __const__sort_threshold_pdq_partial_insert_sort
#define sort_limit_pdq_offset_blocks __const__sort_limit_pdq_offset_blocks
#define sort_limit_block_cache_stack_bytes __const__sort_limit_block_cache_stack_bytes

/// Callable types for pointer-based sorting functions
T_alias$((sort_OrdFn)(cmp_OrdFn));
T_alias$((sort_OrdCtxFn)(cmp_OrdCtxFn));

T_alias$((sort_SwapFn)(fn_(((*)(u_V$raw lhs, u_V$raw rhs))(void) $T)));
T_alias$((sort_SwapCtxFn)(fn_(((*)(usize lhs, usize rhs, u_V$raw ctx))(void) $T)));

/// Callable types for index-based (context) sorting functions
T_alias$((sort_IdxOrdFn)(fn_(((*)(usize lhs, usize rhs, u_V$raw ctx))(cmp_Ord) $T)));
T_alias$((sort_IdxSwapFn)(fn_(((*)(usize lhs, usize rhs, u_V$raw ctx))(void) $T)));

/// Compare context for index-based queries and sorting.
T_alias$((sort_IdxCmpr)(struct sort_IdxCmpr {
    var_(inner, u_V$raw);
    var_(ordFn, sort_IdxOrdFn);
}));
$attr($inline_always)
$static fn_((sort_IdxCmpr_ord(sort_IdxCmpr self, usize lhs, usize rhs))(cmp_Ord));

/// Compare-exchange context for index-based sorting.
T_alias$((sort_IdxCmpXchgr)(struct sort_IdxCmpXchgr {
    var_(base, sort_IdxCmpr);
    var_(swapFn, sort_IdxSwapFn);
}));
$attr($inline_always)
$static fn_((sort_IdxCmpXchgr_cmpr(sort_IdxCmpXchgr self))(sort_IdxCmpr));
$attr($inline_always)
$static fn_((sort_IdxCmpXchgr_ord(sort_IdxCmpXchgr self, usize lhs, usize rhs))(cmp_Ord));
$attr($inline_always)
$static fn_((sort_IdxCmpXchgr_swap(sort_IdxCmpXchgr self, usize lhs, usize rhs))(void));

/// Checks if the sequence is ordered in ascending order according to `ordFn`.
/// Empty and single-element sequences are ordered.
/// - Time Complexity: O(N)
$extern fn_((sort_inOrdd(u_S_const$raw seq, sort_OrdFn ordFn))(bool));
$extern fn_((sort_inOrddCtx(u_S_const$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(bool));
$extern fn_((sort_inOrddIdx(R range, sort_IdxCmpr idx_ctx))(bool));

/*========== Base Algorithms ==========*/

/// Insertion sort: for small or partially sorted arrays.
/// Empty and single-element sequences are valid inputs and are returned unchanged
/// without invoking the comparator.
/// - Time Complexity: O(N) best, O(N^2) worst.
/// - Space Complexity: O(1)
$extern fn_((sort_insert(u_S$raw seq, sort_OrdFn ordFn))(void));
$extern fn_((sort_insertCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(void));
/// Index-based insertion sort over a half-open range.
/// Empty and single-index ranges are valid inputs and perform no comparisons or swaps.
$extern fn_((sort_insertIdx(R range, sort_IdxCmpXchgr idx_ctx))(void));

/*========== Unstable ==========*/

/// Heapsort: Optimal for strict O(1) memory constraints.
/// Empty and single-element sequences are valid inputs and are returned unchanged
/// without invoking the comparator.
/// - Time Complexity: O(N log N) deterministic.
/// - Space Complexity: O(1)
$extern fn_((sort_heap(u_S$raw seq, sort_OrdFn ordFn))(void));
$extern fn_((sort_heapCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(void));
/// Index-based heapsort over a half-open range.
/// Empty and single-index ranges are valid inputs and perform no comparisons or swaps.
$extern fn_((sort_heapIdx(R range, sort_IdxCmpXchgr idx_ctx))(void));

/// Pattern-Defeating Quicksort (pdqsort): Optimal for O(log N) stack constraints.
/// Empty and single-element sequences are valid inputs and are returned unchanged
/// without invoking the comparator.
/// - Time Complexity: O(N) best, O(N log N) worst/avg.
/// - Space Complexity: O(log N) stack limit.
$extern fn_((sort_pdq(u_S$raw seq, sort_OrdFn ordFn))(void));
$extern fn_((sort_pdqCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(void));
/// Index-based pdqsort over a half-open range.
/// Empty and single-index ranges are valid inputs and perform no comparisons or swaps.
$extern fn_((sort_pdqIdx(R range, sort_IdxCmpXchgr idx_ctx))(void));

/*========== Stable ==========*/

/// Block Sort (WikiSort): Optimal for strict O(1) memory constraints.
/// Fractional cascading with internal buffers and Hwang-Lin rotations.
/// Empty and single-element sequences are valid inputs and are returned unchanged
/// without invoking the comparator.
/// - Time Complexity: O(N) best, O(N log N) worst.
/// - Space Complexity: O(1)
$extern fn_((sort_block(u_S$raw seq, sort_OrdFn ordFn))(void));
$extern fn_((sort_blockCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(void));
/// Block Sort with Partial Buffer: Optimal for O(K) memory constraints.
/// Bypasses internal buffer extraction when partial contiguous memory is provided.
/// Empty and single-element sequences return the original sequence unchanged
/// without invoking the comparator or touching the cache.
/// - Time Complexity: O(N log N) with reduced constant factors.
/// - Space Complexity: O(K) where K <= N.
$extern fn_((sort_blockCache(u_S$raw seq, sort_OrdFn ordFn, u_S$raw cache))(u_S$raw));
$extern fn_((sort_blockCtxCache(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx, u_S$raw cache))(u_S$raw));
/// Block Sort with Alctr: Optimal for O(K) memory constraints.
/// Uses provided allocator to allocate temporary buffer.
/// Empty and single-element sequences are returned unchanged without invoking
/// the comparator.
/// - Time Complexity: O(N log N) with reduced constant factors.
/// - Space Complexity: O(K) where K <= N.
$attr($must_check)
$extern fn_((sort_blockAlloc(u_S$raw seq, sort_OrdFn ordFn, mem_Alctr gpa))(mem_E$u_S$raw));
$attr($must_check)
$extern fn_((sort_blockCtxAlloc(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx, mem_Alctr gpa))(mem_E$u_S$raw));

/*========== Macros and Definitions =========================================*/

#define __const__sort_threshold_fallback_to_insert_sort 24
#define __const__sort_limit_pdq_stack_frames pp_if_(arch_bits_is_64bit)(pp_then_(64), pp_else_(32))
#define __const__sort_threshold_pdq_tukey_ninther 50
#define __const__sort_max_swaps_pdq_choose_pivot 12
#define __const__sort_max_steps_pdq_partial_insert_sort 5
#define __const__sort_threshold_pdq_partial_insert_sort 50
#define __const__sort_limit_pdq_offset_blocks 64
#define __const__sort_limit_block_cache_stack_bytes pp_if_(arch_bits_is_64bit)(pp_then_(2048), pp_else_(1024))

#if in_analysis_active_only || in_comptime
fn_((sort_IdxCmpr_ord(sort_IdxCmpr self, usize lhs, usize rhs))(cmp_Ord)) {
    return call((self.ordFn)(lhs, rhs, u_load(self.inner)));
};
fn_((sort_IdxCmpXchgr_cmpr(sort_IdxCmpXchgr self))(sort_IdxCmpr)) {
    return self.base;
};
fn_((sort_IdxCmpXchgr_ord(sort_IdxCmpXchgr self, usize lhs, usize rhs))(cmp_Ord)) {
    return sort_IdxCmpr_ord(self.base, lhs, rhs);
};
fn_((sort_IdxCmpXchgr_swap(sort_IdxCmpXchgr self, usize lhs, usize rhs))(void)) {
    if (lhs == rhs) return;
    return call((self.swapFn)(lhs, rhs, u_load(self.base.inner)));
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* $extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sort__included */

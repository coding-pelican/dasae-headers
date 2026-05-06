#include "dh/sort.h"
#include "dh/search.h"
#include "dh/mem/common.h"

/*========== Internal Declarations & Definitions ============================*/

T_alias$((sort_IdxCtx__Inner)(struct sort_IdxCtx__Inner {
    var_(seq, u_S$raw);
    var_(ctx, u_V$raw);
    var_(ordFn, sort_OrdCtxFn);
}));
$attr($inline_always)
$static fn_((sort_IdxCtx__Inner_ord(usize lhs, usize rhs, u_V$raw ctx))(cmp_Ord)) {
    let inner = u_castV$((sort_IdxCtx__Inner)(ctx));
    let lhs_ptr = u_atS(inner.seq, lhs).as_const;
    let rhs_ptr = u_atS(inner.seq, rhs).as_const;
    return cmp_ordCtxP(lhs_ptr, rhs_ptr, inner.ctx, inner.ordFn);
};
$attr($inline_always)
$static fn_((sort_IdxCtx__Inner_swap(usize lhs, usize rhs, u_V$raw ctx))(void)) {
    let inner = u_castV$((sort_IdxCtx__Inner)(ctx));
    mem_swapP(u_atS(inner.seq, lhs), u_atS(inner.seq, rhs));
};

/* Context structure to bridge `sort_OrdCtxFn` to `search_OrdFn` */
T_alias$((sort__SearchOrdAdpCtx)(struct sort__SearchOrdAdpCtx {
    var_(val_ptr, u_P_const$raw); /* pivot value being searched for */
    var_(inner, u_V$raw); /* user's context */
    var_(ordFn, sort_OrdCtxFn); /* user's comparison function */
}));
/* Adapter: `search` expects `(item, ctx)`, `sort` provides `(lhs, rhs, ctx)` */
$attr($inline_always)
$static fn_((sort__searchOrdAdp(u_V$raw item, u_V$raw ctx))(cmp_Ord)) {
    let adapter = u_castV$((sort__SearchOrdAdpCtx)(ctx));
    return cmp_ordCtxP(item.ref.as_const, adapter.val_ptr, adapter.inner, adapter.ordFn);
};

/*========== External Definitions: Query ====================================*/

fn_((sort_inOrdd(u_S_const$raw seq, sort_OrdFn ordFn))(bool)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    return sort_inOrddCtx(seq, cmp_ordNoCtx, u_anyV(no_ctx));
};

fn_((sort_inOrddCtx(u_S_const$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(bool)) {
    if (seq.len <= 1) return true;
    for_(($r(1, seq.len))(i)) {
        let ord = cmp_ordCtxP(u_atS(seq, i), u_atS(seq, i - 1), ctx, ordFn);
        if (cmp_Ord_isLt(ord)) return false;
    } $end(for);
    return true;
};

/*========== External Definitions: Insertion Sort ===========================*/

fn_((sort_insert(u_S$raw seq, sort_OrdFn ordFn))(void)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    sort_insertCtx(seq, cmp_ordNoCtx, u_anyV(no_ctx));
};

fn_((sort_insertCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(void)) {
    let_(inner, sort_IdxCtx__Inner) = { .seq = seq, .ordFn = ordFn, .ctx = ctx };
    let_(idx_ctx, sort_IdxCtx) = {
        .ordFn = sort_IdxCtx__Inner_ord,
        .swapFn = sort_IdxCtx__Inner_swap,
        .inner = u_anyV(inner)
    };
    sort_insertIdx($rt(seq.len), idx_ctx);
};

fn_((sort_insertIdx(R range, sort_IdxCtx idx_ctx))(void)) {
    for_(((R_suffix(range, 1)))(unsorted_idx)) {
        var sorted_bwd_idx = unsorted_idx;
        while (range.begin < sorted_bwd_idx) {
            let curr = sorted_bwd_idx;
            let prev = curr - 1;
            let ord = sort_IdxCtx_ord(idx_ctx, curr, prev);
            if (!cmp_Ord_isLt(ord)) break;
            sort_IdxCtx_swap(idx_ctx, curr, prev);
            sorted_bwd_idx--;
        }
    } $end(for);
};

/*========== External Definitions: Heap Sort ================================*/

fn_((sort_heap(u_S$raw seq, sort_OrdFn ordFn))(void)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    sort_heapCtx(seq, cmp_ordNoCtx, u_anyV(no_ctx));
};

fn_((sort_heapCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(void)) {
    let_(inner, sort_IdxCtx__Inner) = { .seq = seq, .ordFn = ordFn, .ctx = ctx };
    let_(idx_ctx, sort_IdxCtx) = {
        .ordFn = sort_IdxCtx__Inner_ord,
        .swapFn = sort_IdxCtx__Inner_swap,
        .inner = u_anyV(inner)
    };
    sort_heapIdx($rt(seq.len), idx_ctx);
};

$attr($inline_always)
$static fn_((sort_heap__siftDown(R range, usize target, sort_IdxCtx idx_ctx))(void));
fn_((sort_heapIdx(R range, sort_IdxCtx idx_ctx))(void)) {
    let len = R_len(range);
    if (len <= 1) return;
    /* heapify */
    var i = len / 2;
    while (i-- > 0) {
        sort_heap__siftDown(range, range.begin + i, idx_ctx);
    }
    /* extract */
    i = len;
    while (i-- > 0) {
        sort_IdxCtx_swap(idx_ctx, range.begin, range.begin + i);
        sort_heap__siftDown(R_prefix(range, i), range.begin, idx_ctx);
    }
};

fn_((sort_heap__siftDown(R range, usize target, sort_IdxCtx idx_ctx))(void)) {
    var curr = target;
    while (true) {
        var child = ((curr - range.begin) * 2) + range.begin + 1;
        if (child >= range.end) break;
        let next_child = child + 1;
        if (next_child < range.end) {
            let ord = sort_IdxCtx_ord(idx_ctx, child, next_child);
            if (cmp_Ord_isLt(ord)) child = next_child;
        }
        let ord = sort_IdxCtx_ord(idx_ctx, child, curr);
        if (!cmp_Ord_isLt(ord)) {
            sort_IdxCtx_swap(idx_ctx, child, curr);
            curr = child;
        } else {
            break;
        }
    }
};

/*========== External Definitions: PDQ Sort =================================*/

/* --- Internal Declarations --- */

/* Break patterns in the slice by shuffling some elements around. */
$static fn_((sort_pdq__breakPatterns(R range, sort_IdxCtx idx_ctx))(void));
/* Choose a pivot in `items[begin..end]`. Swaps likely_sorted when slice seems already sorted. */
$static fn_((sort_pdq__choosePivot(R range, usize* pivot, sort_IdxCtx idx_ctx))(u8));
/* Sort 3 elements and count swaps performed */
$attr($inline_always)
$static fn_((sort_pdq__sort3(usize lo, usize mid, usize hi, usize* swaps, sort_IdxCtx idx_ctx))(void));
/* Reverse the range in place. */
$static fn_((sort_pdq__reverseRange(R range, sort_IdxCtx idx_ctx))(void));
/* Partially sorts a slice by shifting several out-of-order elements around.
 * Returns `true` if the slice is sorted at the end. This function is O(n) worst-case. */
$static fn_((sort_pdq__insertPartial(R range, sort_IdxCtx idx_ctx))(bool));
/* Partitions `items[begin..end]` into elements equal to `items[pivot]`
 * followed by elements greater than `items[pivot]`.
 * Assumes `items[begin..end]` does not contain elements smaller than `items[pivot]`. */
$static fn_((sort_pdq__partEq(R range, usize pivot, sort_IdxCtx idx_ctx))(usize));
/* Partitions `items[begin..end]` into elements smaller than `items[pivot]`,
 * followed by elements greater than or equal to `items[pivot]`.
 * Sets the new pivot. Returns `true` if already partitioned. */
$static fn_((sort_pdq__part(R range, usize* pivot, sort_IdxCtx idx_ctx))(bool));

/* --- External Definitions --- */

T_alias$((sort_pdq__Frame)(struct sort_pdq__Frame {
    var_(range, R);
    var_(limit, usize);
}));

fn_((sort_pdq(u_S$raw seq, sort_OrdFn ordFn))(void)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    sort_pdqCtx(seq, cmp_ordNoCtx, u_anyV(no_ctx));
};

fn_((sort_pdqCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(void)) {
    let_(inner, sort_IdxCtx__Inner) = { .seq = seq, .ordFn = ordFn, .ctx = ctx };
    let_(idx_ctx, sort_IdxCtx) = {
        .ordFn = sort_IdxCtx__Inner_ord,
        .swapFn = sort_IdxCtx__Inner_swap,
        .inner = u_anyV(inner)
    };
    sort_pdqIdx($rt(seq.len), idx_ctx);
};

fn_((sort_pdqIdx(R range, sort_IdxCtx idx_ctx))(void)) {
    let_(max_insertion, usize) = sort_threshold_fallback_to_insert_sort;
    let len = R_len(range);
    if (len <= 1) return;

    var_(stack, A$$(sort_limit_pdq_stack_frames, sort_pdq__Frame)) = A_zero();
    var_(depth, usize) = 0;

    let max_limit = uint_pow2Floor$((usize)(len)) + 1;
    var_(frame, sort_pdq__Frame) = { .range = range, .limit = max_limit };

    while (true) {
        var_(was_balanced, bool) = true;
        var_(was_partitioned, bool) = true;

        while (true) {
            let frame_len = R_len(frame.range);

            /* very short slices get sorted using insertion sort */
            if (frame_len <= max_insertion) {
                sort_insertIdx(frame.range, idx_ctx);
                break;
            }

            /* if too many bad pivot choices were made, fall back to heapsort
             * in order to guarantee O(n*log(n)) worst-case */
            if (frame.limit == 0) {
                sort_heapIdx(frame.range, idx_ctx);
                break;
            }

            /* if the last partitioning was imbalanced, try breaking patterns
             * in the slice by shuffling some elements around */
            if (!was_balanced) {
                sort_pdq__breakPatterns(frame.range, idx_ctx);
                frame.limit--;
            }

            /* choose a pivot and try guessing whether the slice is already sorted */
            var_(pivot, usize) = 0;
            var_(hint, u8) = sort_pdq__choosePivot(frame.range, &pivot, idx_ctx);

            if (hint == 1) { /* decreasing */
                sort_pdq__reverseRange(frame.range, idx_ctx);
                pivot = (frame.range.end - 1) - (pivot - frame.range.begin);
                hint = 0; /* increasing */
            }

            /* if the last partitioning was decently balanced and didn't shuffle elements,
             * and if pivot selection predicts the slice is likely already sorted... */
            if (was_balanced && was_partitioned && hint == 0) {
                if (sort_pdq__insertPartial(frame.range, idx_ctx)) break;
            }

            /* if the chosen pivot is equal to the predecessor, then it's the smallest
             * element in the slice. Partition the slice into elements equal to and
             * elements greater than the pivot. */
            if (frame.range.begin > range.begin) {
                let ord = sort_IdxCtx_ord(idx_ctx, frame.range.begin - 1, pivot);
                if (!cmp_Ord_isLt(ord)) {
                    frame.range.begin = sort_pdq__partEq(frame.range, pivot, idx_ctx);
                    continue;
                }
            }

            /* partition the slice */
            var mid = pivot;
            was_partitioned = sort_pdq__part(frame.range, &mid, idx_ctx);

            let left_len = mid - frame.range.begin;
            let right_len = frame.range.end - mid;
            let_(balance_factor, usize) = 8;
            let balance_threshold = frame_len / balance_factor;

            if (left_len < right_len) {
                was_balanced = left_len >= balance_threshold;
                asg_l((A_at((stack)[depth++]))({
                    .range = $r(frame.range.begin, mid),
                    .limit = frame.limit,
                }));
                frame.range.begin = mid + 1;
            } else {
                was_balanced = right_len >= balance_threshold;
                asg_l((A_at((stack)[depth++]))({
                    .range = $r(mid + 1, frame.range.end),
                    .limit = frame.limit,
                }));
                frame.range.end = mid;
            }
        }

        if (depth == 0) break;
        frame = *A_at((stack)[--depth]);
    }
};

/* --- Internal Definitions --- */

fn_((sort_pdq__breakPatterns(R range, sort_IdxCtx idx_ctx))(void)) {
    let_(min_partition, usize) = 8;
    let len = R_len(range);
    if (len < min_partition) return;

    var_(rng, u64) = len;
    let modulus = uint_pow2Ceil$((usize)len);
    let mid_start = range.begin + (len / 4) * 2 - 1;
    let mid_end = range.begin + (len / 4) * 2 + 1;
    for_(($r(mid_start, mid_end + 1))(curr)) {
        /* xorshift64 */
        rng ^= rng << 13;
        rng ^= rng >> 7;
        rng ^= rng << 17;
        var target = (usize)(rng & (modulus - 1));
        if (target >= len) target -= len;
        sort_IdxCtx_swap(idx_ctx, curr, range.begin + target);
    } $end(for);
};
fn_((sort_pdq__choosePivot(R range, usize* pivot, sort_IdxCtx idx_ctx))(u8)) {
    let_(shortest_ninther, usize) = sort_threshold_pdq_tukey_ninther;
    let_(max_swaps, usize) = sort_max_swaps_pdq_choose_pivot;
    let len = R_len(range);
    let q1 = range.begin + len / 4;
    let q2 = range.begin + (len / 4) * 2;
    let q3 = range.begin + (len / 4) * 3;
    var_(swaps, usize) = 0;

    if (len >= 8) {
        if (len >= shortest_ninther) {
            /* find medians in the neighborhoods of q1, q2, q3 */
            sort_pdq__sort3(q1 - 1, q1, q1 + 1, &swaps, idx_ctx);
            sort_pdq__sort3(q2 - 1, q2, q2 + 1, &swaps, idx_ctx);
            sort_pdq__sort3(q3 - 1, q3, q3 + 1, &swaps, idx_ctx);
        }
        /* find the median among q1, q2, q3 and store it in q2 */
        sort_pdq__sort3(q1, q2, q3, &swaps, idx_ctx);
    }

    *pivot = q2;
    if (swaps == 0) return 0; /* increasing */
    if (swaps == max_swaps) return 1; /* decreasing */
    return 2; /* unknown */
};
fn_((sort_pdq__sort3(usize lo, usize mid, usize hi, usize* swaps, sort_IdxCtx idx_ctx))(void)) {
    if (cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, mid, lo))) {
        sort_IdxCtx_swap(idx_ctx, mid, lo);
        *swaps += 1;
    }
    if (cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, hi, mid))) {
        sort_IdxCtx_swap(idx_ctx, hi, mid);
        *swaps += 1;
    }
    if (cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, mid, lo))) {
        sort_IdxCtx_swap(idx_ctx, mid, lo);
        *swaps += 1;
    }
};
fn_((sort_pdq__reverseRange(R range, sort_IdxCtx idx_ctx))(void)) {
    var lo = range.begin;
    var hi = range.end - 1;
    while (lo < hi) sort_IdxCtx_swap(idx_ctx, lo++, hi--);
};
fn_((sort_pdq__insertPartial(R range, sort_IdxCtx idx_ctx))(bool)) {
    let_(max_steps, usize) = sort_max_steps_pdq_partial_insert_sort;
    let_(shortest_shifting, usize) = sort_threshold_pdq_partial_insert_sort;
    var curr = range.begin + 1;
    for_(($rt(max_steps))($ignore)) {
        /* find the next pair of adjacent out-of-order elements */
        while (curr < range.end && !cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, curr, curr - 1))) curr++;
        if (curr == range.end) return true;
        /* don't shift elements on short arrays, that has a performance cost */
        if (R_len(range) < shortest_shifting) return false;

        /* swap the found pair of elements. This puts them in correct order. */
        sort_IdxCtx_swap(idx_ctx, curr, curr - 1);
        /* shift the smaller element to the left */
        if (curr - range.begin >= 2) {
            var scan = curr - 1;
            while (scan > range.begin) {
                if (!cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, scan, scan - 1))) break;
                sort_IdxCtx_swap(idx_ctx, scan, scan - 1);
                scan--;
            }
        }
        /* shift the greater element to the right */
        if (range.end - curr >= 2) {
            var scan = curr + 1;
            while (scan < range.end) {
                if (!cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, scan, scan - 1))) break;
                sort_IdxCtx_swap(idx_ctx, scan, scan - 1);
                scan++;
            }
        }
    } $end(for);
    return false;
};
fn_((sort_pdq__partEq(R range, usize pivot, sort_IdxCtx idx_ctx))(usize)) {
    sort_IdxCtx_swap(idx_ctx, range.begin, pivot);
    var lo = range.begin + 1;
    var hi = range.end - 1;
    while (true) {
        while (lo <= hi && !cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, range.begin, lo))) lo++;
        while (lo <= hi && cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, range.begin, hi))) hi--;
        if (lo > hi) break;
        sort_IdxCtx_swap(idx_ctx, lo++, hi--);
    }
    return lo;
};
fn_((sort_pdq__part(R range, usize* pivot, sort_IdxCtx idx_ctx))(bool)) {
    sort_IdxCtx_swap(idx_ctx, range.begin, *pivot);
    let pivot_idx = range.begin;

    var l = range.begin + 1;
    var r = range.end - 1;
    var_(l_offsets, A$$(sort_limit_pdq_offset_blocks, u8)) = A_zero();
    var_(r_offsets, A$$(sort_limit_pdq_offset_blocks, u8)) = A_zero();
    var_(l_count, usize) = 0;
    var_(r_count, usize) = 0;
    var_(l_offset_idx, usize) = 0;
    var_(r_offset_idx, usize) = 0;
    var_(was_partitioned, bool) = true;

    let block_size = sort_limit_pdq_offset_blocks;
    let min_remaining = 2 * block_size;
    while (l < r && (r - l + 1) >= min_remaining) {
        if (l_count == 0) {
            l_offset_idx = 0;
            for_(($rt(block_size))(scan)) {
                let is_ge = cmp_Ord_isGe(sort_IdxCtx_ord(idx_ctx, l + scan, pivot_idx));
                *A_at((l_offsets)[l_count]) = intCast$((u8)(scan));
                l_count += as$(usize)(boolToInt(is_ge));
            } $end(for);
        }

        if (r_count == 0) {
            r_offset_idx = 0;
            for_(($rt(block_size))(scan)) {
                let is_lt = cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, r - scan, pivot_idx));
                *A_at((r_offsets)[r_count]) = intCast$((u8)(scan));
                r_count += as$(usize)(boolToInt(is_lt));
            } $end(for);
        }

        let l_remaining = l_count - l_offset_idx;
        let r_remaining = r_count - r_offset_idx;
        let pair_l_r_count = (l_remaining < r_remaining) ? l_remaining : r_remaining;
        for_(($rt(pair_l_r_count))(i)) {
            sort_IdxCtx_swap(
                idx_ctx,
                l + *A_at((l_offsets)[l_offset_idx + i]),
                r - *A_at((r_offsets)[r_offset_idx + i])
            );
        } $end(for);
        if (pair_l_r_count > 0) was_partitioned = false;

        l_offset_idx += pair_l_r_count;
        r_offset_idx += pair_l_r_count;

        if (l_offset_idx == l_count) {
            l += block_size;
            l_count = 0;
            l_offset_idx = 0;
        }
        if (r_offset_idx == r_count) {
            r -= block_size;
            r_count = 0;
            r_offset_idx = 0;
        }
    }

    while (l <= r && cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, l, pivot_idx))) l++;
    while (l <= r && !cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, r, pivot_idx))) r--;
    if (l > r) {
        sort_IdxCtx_swap(idx_ctx, r, pivot_idx);
        *pivot = r;
        return was_partitioned;
    }

    sort_IdxCtx_swap(idx_ctx, l++, r--);

    while (true) {
        while (l <= r && cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, l, pivot_idx))) l++;
        while (l <= r && !cmp_Ord_isLt(sort_IdxCtx_ord(idx_ctx, r, pivot_idx))) r--;
        if (l > r) break;
        sort_IdxCtx_swap(idx_ctx, l++, r--);
    }

    sort_IdxCtx_swap(idx_ctx, r, pivot_idx);
    *pivot = r;
    return false;
};

/*========== External Definitions: Block Sort ===============================*/

/* --- Internal Definitions --- */

T_alias$((sort_block__Iter)(struct sort_block__Iter {
    var_(size, usize);
    var_(pow2, usize);
    var_(dec, usize);
    var_(dec_step, usize);
    var_(num, usize);
    var_(num_step, usize);
    var_(denom, usize);
}));
$attr($inline_always)
$static fn_((sort_block__Iter_init(usize size2, usize min_level))(sort_block__Iter));
$attr($inline_always)
$static fn_((sort_block__Iter_len(sort_block__Iter* self))(usize));
$attr($inline_always)
$static fn_((sort_block__Iter_begin(sort_block__Iter* self))(void));
$attr($inline_always)
$static fn_((sort_block__Iter_nextRange(sort_block__Iter* self))(R));
$attr($inline_always)
$static fn_((sort_block__Iter_nextLevel(sort_block__Iter* self))(bool));
$attr($inline_always)
$static fn_((sort_block__Iter_finished(sort_block__Iter* self))(bool));

/* In-place merge fallback via Hwang-Lin rotations */
$static fn_((sort_block__mergeInPlace(
    u_S$raw seq, R left, R right,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void));
/* Buffer-assisted merge: Optimal O(N) memory utilization */
$static fn_((sort_block__mergeExternal(
    u_S$raw seq, R left, R right, u_S$raw cache,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void));

/* --- External Definitions --- */

fn_((sort_block(u_S$raw seq, sort_OrdFn ordFn))(void)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    sort_blockCtx(seq, cmp_ordNoCtx, u_anyV(no_ctx));
};

fn_((sort_blockCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(void)) {
    var_(cache_buf, A$$(sort_limit_block_cache_stack_bytes, u8)) = A_zero();
    let cache_cap = (0 < seq.type.size) ? (sort_limit_block_cache_stack_bytes / seq.type.size) : 0;
    let_(cache, u_S$raw) = u_init$S((seq.type)(A_ptr(cache_buf), cache_cap));
    $ignore_void sort_blockCtxCache(cache, seq, ordFn, ctx);
};

fn_((sort_blockCache(u_S$raw cache, u_S$raw seq, sort_OrdFn ordFn))(u_S$raw)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    return sort_blockCtxCache(cache, seq, cmp_ordNoCtx, u_anyV(no_ctx));
};

fn_((sort_blockCtxCache(u_S$raw cache, u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(u_S$raw)) {
    if (seq.len <= 1) return seq;

    let_(min_level, usize) = sort_threshold_fallback_to_insert_sort;
    var iter = sort_block__Iter_init(seq.len, min_level);
    /* O(N) insertion phase for lowest-level runs */
    while (!sort_block__Iter_finished(&iter)) {
        let range = sort_block__Iter_nextRange(&iter);
        sort_insertCtx(u_sliceS(seq, range), ordFn, ctx);
    }
    if (!sort_block__Iter_nextLevel(&iter)) return seq;

    /* bottom-up hierarchical merging */
    while (true) {
        sort_block__Iter_begin(&iter);
        while (!sort_block__Iter_finished(&iter)) {
            let left = sort_block__Iter_nextRange(&iter);
            let right = sort_block__Iter_nextRange(&iter);
            if (R_len(right) == 0) continue;

            let already_sorted = cmp_Ord_isLe(cmp_ordCtxP(
                u_atS(seq, left.end - 1).as_const,
                u_atS(seq, right.begin).as_const,
                ctx,
                ordFn
            ));
            if (already_sorted) continue;

            /* adaptive dispatch: route to fast path if buffer satisfies partial length K */
            if (cache.len < R_len(left)) {
                sort_block__mergeInPlace(seq, left, right, ordFn, ctx);
            } else {
                sort_block__mergeExternal(seq, left, right, cache, ordFn, ctx);
            }
        }
        if (!sort_block__Iter_nextLevel(&iter)) break;
    }
    return seq;
};

fn_((sort_blockAlloc(mem_Alctr gpa, u_S$raw seq, sort_OrdFn ordFn))(mem_E$u_S$raw)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    return sort_blockCtxAlloc(gpa, seq, cmp_ordNoCtx, u_anyV(no_ctx));
};

$static fn_((sort_block__allocCache(mem_Alctr gpa, TypeInfo type, usize len))(mem_E$u_S$raw));
fn_((sort_blockCtxAlloc(mem_Alctr gpa, u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(mem_E$u_S$raw) $scope) {
    let cache = try_(sort_block__allocCache(gpa, seq.type, seq.len));
    return_ok(sort_blockCtxCache(cache, seq, ordFn, ctx));
} $unscoped(fn);

fn_((sort_block__allocCache(mem_Alctr gpa, TypeInfo type, usize len))(mem_E$u_S$raw) $scope) {
    if_ok((mem_Alctr_alloc($trace gpa, type, len))(cover_full)) return_ok(cover_full);
    if_ok((mem_Alctr_alloc($trace gpa, type, len / 2))(cover_half)) return_ok(cover_half);
    return_ok(try_(mem_Alctr_alloc($trace gpa, type, len / 4)));
} $unscoped(fn);

/* --- Internal Definitions --- */

fn_((sort_block__Iter_init(usize size2, usize min_level))(sort_block__Iter)) {
    let pow2 = uint_pow2Floor$((usize)(size2));
    let denom = pow2 / min_level;
    return (sort_block__Iter){
        .size = size2,
        .pow2 = pow2,
        .dec = 0,
        .dec_step = size2 / denom,
        .num = 0,
        .num_step = size2 % denom,
        .denom = denom,
    };
};
fn_((sort_block__Iter_len(sort_block__Iter* self))(usize)) {
    return self->dec_step;
};
fn_((sort_block__Iter_begin(sort_block__Iter* self))(void)) {
    self->dec = 0;
    self->num = 0;
};
fn_((sort_block__Iter_nextRange(sort_block__Iter* self))(R)) {
    let start = self->dec;
    self->dec += self->dec_step;
    self->num += self->num_step;
    if (self->num >= self->denom) {
        self->num -= self->denom;
        self->dec += 1;
    }
    return $r(start, self->dec);
};
fn_((sort_block__Iter_nextLevel(sort_block__Iter* self))(bool)) {
    self->dec_step += self->dec_step;
    self->num_step += self->num_step;
    if (self->num_step >= self->denom) {
        self->num_step -= self->denom;
        self->dec_step += 1;
    }
    return sort_block__Iter_len(self) < self->size;
};
fn_((sort_block__Iter_finished(sort_block__Iter* self))(bool)) {
    return self->dec >= self->size;
};

fn_((sort_block__mergeInPlace(
    u_S$raw seq, R left, R right,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void)) {
    if (R_len(left) == 0 || R_len(right) == 0) return;
    while (true) {
        let pivot_ptr = u_atS(seq.as_const, left.begin);
        let split_offset = search_lowerBound(
            u_sliceS(seq, right).as_const,
            u_anyV(l$((sort__SearchOrdAdpCtx){
                .val_ptr = pivot_ptr,
                .inner = ctx,
                .ordFn = ordFn,
            })),
            sort__searchOrdAdp
        );
        let split_point = split_offset + right.begin;

        let rotate_range = $r(left.begin, split_point);
        let rotate_amount = R_len(left);
        mem_rotate(u_sliceS(seq, rotate_range), rotate_amount);
        if (right.end == split_point) break;

        right.begin = split_point;
        left = $r(left.begin + (split_point - left.end), right.begin);

        let left_pivot_ptr = u_atS(seq.as_const, left.begin);
        let skip_offset = search_upperBound(
            u_sliceS(seq, left).as_const,
            u_anyV(l$((sort__SearchOrdAdpCtx){
                .val_ptr = left_pivot_ptr,
                .inner = ctx,
                .ordFn = ordFn,
            })),
            sort__searchOrdAdp
        );
        left.begin += skip_offset;
        if (R_len(left) == 0) break;
    }
};
fn_((sort_block__mergeExternal(
    u_S$raw seq, R left, R right, u_S$raw cache,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void)) {
    let left_len = R_len(left);
    /* cache left run into external memory */
    u_memcpyS(u_prefixS(cache, left_len), u_sliceS(seq, left).as_const);

    var_(cache_idx, usize) = 0;
    var_(right_idx, usize) = right.begin;
    var_(dst_idx, usize) = left.begin;

    while (cache_idx < left_len && right_idx < right.end) {
        let ord = cmp_ordCtxP(
            u_atS(seq, right_idx).as_const,
            u_atS(cache, cache_idx).as_const,
            ctx,
            ordFn
        );
        if (cmp_Ord_isLt(ord)) {
            u_memcpy(u_atS(seq, dst_idx), u_atS(seq, right_idx++).as_const);
        } else {
            u_memcpy(u_atS(seq, dst_idx), u_atS(cache, cache_idx++).as_const);
        }
        dst_idx++;
    }

    /* flush remainder of left if right is exhausted */
    if (cache_idx < left_len) {
        let remaining = left_len - cache_idx;
        u_memcpyS(
            u_sliceS(seq, $r(dst_idx, dst_idx + remaining)),
            u_sliceS(cache, $rf(cache_idx)).as_const
        );
    }
};

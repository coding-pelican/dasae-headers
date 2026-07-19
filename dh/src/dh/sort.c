#include "dh/sort.h"
#include "dh/search.h"
#include "dh/mem/common.h"

/*========== Internal Declarations & Definitions ============================*/

T_alias$((sort_IdxCmpr__Inner)(struct sort_IdxCmpr__Inner {
    var_(seq, u_S_const$raw);
    var_(ctx, u_V$raw);
    var_(ordFn, sort_OrdCtxFn);
}));
$attr($inline_always)
$static fn_((sort_IdxCmpr__Inner_ord(usize lhs, usize rhs, u_V$raw ctx))(cmp_Ord)) {
    let inner = u_castV$((sort_IdxCmpr__Inner)(ctx));
    return cmp_ordCtxP(u_atS(inner.seq, lhs), u_atS(inner.seq, rhs), inner.ctx, inner.ordFn);
};

T_alias$((sort_IdxCmpXchgr__Inner)(struct sort_IdxCmpXchgr__Inner {
    var_(seq, u_S$raw);
    var_(ctx, u_V$raw);
    var_(ordFn, sort_OrdCtxFn);
}));
$attr($inline_always)
$static fn_((sort_IdxCmpXchgr__Inner_ord(usize lhs, usize rhs, u_V$raw ctx))(cmp_Ord)) {
    let inner = u_castV$((sort_IdxCmpXchgr__Inner)(ctx));
    let lhs_ptr = u_atS(inner.seq, lhs).as_const;
    let rhs_ptr = u_atS(inner.seq, rhs).as_const;
    return cmp_ordCtxP(lhs_ptr, rhs_ptr, inner.ctx, inner.ordFn);
};
$attr($inline_always)
$static fn_((sort_IdxCmpXchgr__Inner_swap(usize lhs, usize rhs, u_V$raw ctx))(void)) {
    let inner = u_castV$((sort_IdxCmpXchgr__Inner)(ctx));
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
    let_(inner, sort_IdxCmpr__Inner) = { .seq = seq, .ordFn = ordFn, .ctx = ctx };
    let_(idx_cmpr, sort_IdxCmpr) = {
        .inner = u_deref(u_anyP(&inner)),
        .ordFn = sort_IdxCmpr__Inner_ord,
    };
    return sort_inOrddIdx($rt(seq.len), idx_cmpr);
};

fn_((sort_inOrddIdx(R range, sort_IdxCmpr idx_cmpr))(bool)) {
    if (R_len(range) <= 1) return true;
    for_(((R_suffix(range, 1)))(i)) {
        let ord = sort_IdxCmpr_ord(idx_cmpr, i, i - 1);
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
    let_(inner, sort_IdxCmpXchgr__Inner) = { .seq = seq, .ordFn = ordFn, .ctx = ctx };
    let_(idx_ctx, sort_IdxCmpXchgr) = {
        .base = {
            .inner = u_deref(u_anyP(&inner)),
            .ordFn = sort_IdxCmpXchgr__Inner_ord,
        },
        .swapFn = sort_IdxCmpXchgr__Inner_swap,
    };
    sort_insertIdx($rt(seq.len), idx_ctx);
};

fn_((sort_insertIdx(R range, sort_IdxCmpXchgr idx_ctx))(void)) {
    if (R_len(range) <= 1) return;
    for_(((R_suffix(range, 1)))(unsorted_idx)) {
        var sorted_bwd_idx = unsorted_idx;
        while (range.begin < sorted_bwd_idx) {
            let curr = sorted_bwd_idx;
            let prev = curr - 1;
            let ord = sort_IdxCmpXchgr_ord(idx_ctx, curr, prev);
            if (!cmp_Ord_isLt(ord)) break;
            sort_IdxCmpXchgr_swap(idx_ctx, curr, prev);
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
    let_(inner, sort_IdxCmpXchgr__Inner) = { .seq = seq, .ordFn = ordFn, .ctx = ctx };
    let_(idx_ctx, sort_IdxCmpXchgr) = {
        .base = {
            .inner = u_deref(u_anyP(&inner)),
            .ordFn = sort_IdxCmpXchgr__Inner_ord,
        },
        .swapFn = sort_IdxCmpXchgr__Inner_swap,
    };
    sort_heapIdx($rt(seq.len), idx_ctx);
};

$attr($inline_always)
$static fn_((sort_heap__siftDown(R range, usize target, sort_IdxCmpXchgr idx_ctx))(void));
fn_((sort_heapIdx(R range, sort_IdxCmpXchgr idx_ctx))(void)) {
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
        sort_IdxCmpXchgr_swap(idx_ctx, range.begin, range.begin + i);
        sort_heap__siftDown(R_prefix(range, i), range.begin, idx_ctx);
    }
};

fn_((sort_heap__siftDown(R range, usize target, sort_IdxCmpXchgr idx_ctx))(void)) {
    let range_len = R_len(range);
    if (range_len <= 1) return;
    var curr = target;
    while (true) {
        let child_offset = orelse_((usize_mulChkd(curr - range.begin, 2))(break));
        if (child_offset >= range_len - 1) break;
        var child = range.begin + child_offset + 1;
        let next_child = child + 1;
        if (next_child < range.end) {
            let ord = sort_IdxCmpXchgr_ord(idx_ctx, child, next_child);
            if (cmp_Ord_isLt(ord)) child = next_child;
        }
        let ord = sort_IdxCmpXchgr_ord(idx_ctx, child, curr);
        if (!cmp_Ord_isLt(ord)) {
            sort_IdxCmpXchgr_swap(idx_ctx, child, curr);
            curr = child;
        } else {
            break;
        }
    }
};

/*========== External Definitions: PDQ Sort =================================*/

/*--- Internal Declarations ---*/

/* Break patterns in the slice by shuffling some elements around. */
$static fn_((sort_pdq__breakPatterns(R range, sort_IdxCmpXchgr idx_ctx))(void));
/* Choose a pivot in `items[begin..end]`. Swaps likely_sorted when slice seems already sorted. */
$static fn_((sort_pdq__choosePivot(R range, usize* pivot, sort_IdxCmpXchgr idx_ctx))(u8));
/* Sort 3 elements and count swaps performed */
$attr($inline_always)
$static fn_((sort_pdq__sort3(usize lo, usize mid, usize hi, usize* swaps, sort_IdxCmpXchgr idx_ctx))(void));
/* Reverse the range in place. */
$static fn_((sort_pdq__reverseRange(R range, sort_IdxCmpXchgr idx_ctx))(void));
/* Partially sorts a slice by shifting several out-of-order elements around.
 * Returns `true` if the slice is sorted at the end. This function is O(n) worst-case. */
$static fn_((sort_pdq__insertPartial(R range, sort_IdxCmpXchgr idx_ctx))(bool));
/* Partitions `items[begin..end]` into elements equal to `items[pivot]`
 * followed by elements greater than `items[pivot]`.
 * Assumes `items[begin..end]` does not contain elements smaller than `items[pivot]`. */
$static fn_((sort_pdq__partEq(R range, usize pivot, sort_IdxCmpXchgr idx_ctx))(usize));
/* Partitions `items[begin..end]` into elements smaller than `items[pivot]`,
 * followed by elements greater than or equal to `items[pivot]`.
 * Sets the new pivot. Returns `true` if already partitioned. */
$static fn_((sort_pdq__part(R range, usize* pivot, sort_IdxCmpXchgr idx_ctx))(bool));

/*--- External Definitions ---*/

T_alias$((sort_pdq__Frame)(struct sort_pdq__Frame {
    var_(range, R);
    var_(limit, usize);
}));

fn_((sort_pdq(u_S$raw seq, sort_OrdFn ordFn))(void)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    sort_pdqCtx(seq, cmp_ordNoCtx, u_anyV(no_ctx));
};

fn_((sort_pdqCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(void)) {
    let_(inner, sort_IdxCmpXchgr__Inner) = { .seq = seq, .ordFn = ordFn, .ctx = ctx };
    let_(idx_ctx, sort_IdxCmpXchgr) = {
        .base = {
            .inner = u_deref(u_anyP(&inner)),
            .ordFn = sort_IdxCmpXchgr__Inner_ord,
        },
        .swapFn = sort_IdxCmpXchgr__Inner_swap,
    };
    sort_pdqIdx($rt(seq.len), idx_ctx);
};

fn_((sort_pdqIdx(R range, sort_IdxCmpXchgr idx_ctx))(void)) {
    let_(max_insertion, usize) = sort_threshold_fallback_to_insert_sort;
    let len = R_len(range);
    if (len <= 1) return;

    var_(stack, A$$(sort_limit_pdq_stack_frames, sort_pdq__Frame)) $undefined;
    var_(depth, usize) = 0;

    let max_limit = uint_log2(len);
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
                let ord = sort_IdxCmpXchgr_ord(idx_ctx, frame.range.begin - 1, pivot);
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

/*--- Internal Definitions ---*/

fn_((sort_pdq__breakPatterns(R range, sort_IdxCmpXchgr idx_ctx))(void)) {
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
        sort_IdxCmpXchgr_swap(idx_ctx, curr, range.begin + target);
    } $end(for);
};
fn_((sort_pdq__choosePivot(R range, usize* pivot, sort_IdxCmpXchgr idx_ctx))(u8)) {
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
fn_((sort_pdq__sort3(usize lo, usize mid, usize hi, usize* swaps, sort_IdxCmpXchgr idx_ctx))(void)) {
    if (cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, mid, lo))) {
        sort_IdxCmpXchgr_swap(idx_ctx, mid, lo);
        *swaps += 1;
    }
    if (cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, hi, mid))) {
        sort_IdxCmpXchgr_swap(idx_ctx, hi, mid);
        *swaps += 1;
    }
    if (cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, mid, lo))) {
        sort_IdxCmpXchgr_swap(idx_ctx, mid, lo);
        *swaps += 1;
    }
};
fn_((sort_pdq__reverseRange(R range, sort_IdxCmpXchgr idx_ctx))(void)) {
    var lo = range.begin;
    var hi = range.end - 1;
    while (lo < hi) sort_IdxCmpXchgr_swap(idx_ctx, lo++, hi--);
};
fn_((sort_pdq__insertPartial(R range, sort_IdxCmpXchgr idx_ctx))(bool)) {
    let_(max_steps, usize) = sort_max_steps_pdq_partial_insert_sort;
    let_(shortest_shifting, usize) = sort_threshold_pdq_partial_insert_sort;
    var curr = range.begin + 1;
    for_(($rt(max_steps))($ignore)) {
        /* find the next pair of adjacent out-of-order elements */
        while (curr < range.end && !cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, curr, curr - 1))) curr++;
        if (curr == range.end) return true;
        /* don't shift elements on short arrays, that has a performance cost */
        if (R_len(range) < shortest_shifting) return false;

        /* swap the found pair of elements. This puts them in correct order. */
        sort_IdxCmpXchgr_swap(idx_ctx, curr, curr - 1);
        /* shift the smaller element to the left */
        if (curr - range.begin >= 2) {
            var scan = curr - 1;
            while (scan > range.begin) {
                if (!cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, scan, scan - 1))) break;
                sort_IdxCmpXchgr_swap(idx_ctx, scan, scan - 1);
                scan--;
            }
        }
        /* shift the greater element to the right */
        if (range.end - curr >= 2) {
            var scan = curr + 1;
            while (scan < range.end) {
                if (!cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, scan, scan - 1))) break;
                sort_IdxCmpXchgr_swap(idx_ctx, scan, scan - 1);
                scan++;
            }
        }
    } $end(for);
    return false;
};
fn_((sort_pdq__partEq(R range, usize pivot, sort_IdxCmpXchgr idx_ctx))(usize)) {
    sort_IdxCmpXchgr_swap(idx_ctx, range.begin, pivot);
    var lo = range.begin + 1;
    var hi = range.end - 1;
    while (true) {
        while (lo <= hi && !cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, range.begin, lo))) lo++;
        while (lo <= hi && cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, range.begin, hi))) hi--;
        if (lo > hi) break;
        sort_IdxCmpXchgr_swap(idx_ctx, lo++, hi--);
    }
    return lo;
};
fn_((sort_pdq__part(R range, usize* pivot, sort_IdxCmpXchgr idx_ctx))(bool)) {
    sort_IdxCmpXchgr_swap(idx_ctx, range.begin, *pivot);
    let pivot_idx = range.begin;

    var l = range.begin + 1;
    var r = range.end - 1;
    var_(l_offsets, A$$(sort_limit_pdq_offset_blocks, u8)) $undefined;
    var_(r_offsets, A$$(sort_limit_pdq_offset_blocks, u8)) $undefined;
    var_(l_count, usize) = 0;
    var_(r_count, usize) = 0;
    var_(l_start, usize) = 0;
    var_(r_start, usize) = 0;
    var_(l_base, usize) = 0;
    var_(r_base, usize) = 0;
    var_(was_partitioned, bool) = true;

    while (l <= r && cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, l, pivot_idx))) l++;
    while (l <= r && !cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, r, pivot_idx))) r--;
    if (l > r) {
        sort_IdxCmpXchgr_swap(idx_ctx, r, pivot_idx);
        *pivot = r;
        return true;
    }

    sort_IdxCmpXchgr_swap(idx_ctx, l, r);
    was_partitioned = false;
    l++;
    r--;

    let_(block_size, usize) = sort_limit_pdq_offset_blocks;
    l_base = l;
    r_base = r;
    while (l <= r) {
        let unknown_count = r + 1 - l;
        let_(l_scan_count, usize) = (l_count == 0)
                                      ? pri_min(block_size, (r_count == 0) ? (unknown_count / 2) : unknown_count)
                                      : usize_(0);
        let_(r_scan_count, usize) = (r_count == 0)
                                      ? pri_min(block_size, unknown_count - l_scan_count)
                                      : usize_(0);

        if (l_count == 0) {
            l_start = 0;
            l_base = l;
            for_(($rt(l_scan_count))(scan)) {
                let is_ge = cmp_Ord_isGe(sort_IdxCmpXchgr_ord(idx_ctx, l + scan, pivot_idx));
                *A_at((l_offsets)[l_count]) = intCast$((u8)(scan));
                l_count += as$(usize)(boolToInt(is_ge));
            } $end(for);
            l += l_scan_count;
        }

        if (r_count == 0) {
            r_start = 0;
            r_base = r;
            for_(($rt(r_scan_count))(scan)) {
                let is_lt = cmp_Ord_isLt(sort_IdxCmpXchgr_ord(idx_ctx, r - scan, pivot_idx));
                *A_at((r_offsets)[r_count]) = intCast$((u8)(scan));
                r_count += as$(usize)(boolToInt(is_lt));
            } $end(for);
            r -= r_scan_count;
        }

        let pair_count = pri_min(l_count, r_count);
        for_(($rt(pair_count))(i)) {
            sort_IdxCmpXchgr_swap(
                idx_ctx,
                l_base + *A_at((l_offsets)[l_start + i]),
                r_base - *A_at((r_offsets)[r_start + i])
            );
        } $end(for);
        if (pair_count > 0) was_partitioned = false;

        l_count -= pair_count;
        r_count -= pair_count;
        l_start += pair_count;
        r_start += pair_count;

        if (l_count == 0) {
            l_start = 0;
            l_base = l;
        }
        if (r_count == 0) {
            r_start = 0;
            r_base = r;
        }
    }

    if (l_count > 0) {
        while (l_count > 0) {
            l_count--;
            sort_IdxCmpXchgr_swap(idx_ctx, l_base + *A_at((l_offsets)[l_start + l_count]), r);
            r--;
        }
        l = r + 1;
    }
    if (r_count > 0) {
        while (r_count > 0) {
            r_count--;
            sort_IdxCmpXchgr_swap(idx_ctx, r_base - *A_at((r_offsets)[r_start + r_count]), l);
            l++;
        }
        r = l - 1;
    }

    sort_IdxCmpXchgr_swap(idx_ctx, r, pivot_idx);
    *pivot = r;
    return was_partitioned;
};

/*========== External Definitions: Block Sort ===============================*/

/*--- Internal Definitions ---*/

T_alias$((sort_block__Iter)(struct sort_block__Iter {
    var_(size, usize);
    var_(pow2, usize);
    var_(dec, usize);
    var_(dec_step, usize);
    var_(num, usize);
    var_(num_step, usize);
    var_(denom, usize);
}));
T_alias$((sort_block__Pull)(struct sort_block__Pull {
    var_(from, usize);
    var_(to, usize);
    var_(count, usize);
    var_(range, R);
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
$static fn_((sort_block__mergeExternalCached(
    u_S$raw seq, R left, R right, u_S$raw cache,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void));
$static fn_((sort_block__mergeExternalRight(
    u_S$raw seq, R left, R right, u_S$raw cache,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void));
$static fn_((sort_block__mergeInternal(
    u_S$raw seq, R left, R right, R buffer,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void));
$static fn_((sort_block__mergeWikiLevel(
    u_S$raw seq, sort_block__Iter* iter, u_S$raw cache,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void));
$attr($inline_always)
$static fn_((sort_block__blockSwap(u_S$raw seq, usize lhs, usize rhs, usize len))(void));
$attr($inline_always)
$static fn_((sort_block__lessIdx(u_S$raw seq, usize lhs, usize rhs, sort_OrdCtxFn ordFn, u_V$raw ctx))(bool));
$static fn_((sort_block__sqrtFloor(usize value))(usize));
$static fn_((sort_block__binaryFirst(u_S$raw seq, u_P_const$raw value, R range, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize));
$static fn_((sort_block__binaryLast(u_S$raw seq, u_P_const$raw value, R range, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize));
$static fn_((sort_block__findFirstForward(u_S$raw seq, u_P_const$raw value, R range, usize unique, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize));
$static fn_((sort_block__findFirstBackward(u_S$raw seq, u_P_const$raw value, R range, usize unique, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize));
$static fn_((sort_block__findLastForward(u_S$raw seq, u_P_const$raw value, R range, usize unique, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize));
$static fn_((sort_block__findLastBackward(u_S$raw seq, u_P_const$raw value, R range, usize unique, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize));

/*--- External Definitions ---*/

fn_((sort_block(u_S$raw seq, sort_OrdFn ordFn))(void)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    sort_blockCtx(seq, cmp_ordNoCtx, u_anyV(no_ctx));
};

fn_((sort_blockCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx))(void)) {
    var_(cache_buf, A$$(sort_limit_block_cache_stack_bytes, u8)) $undefined;
    let_(cache_cap, usize) = (0 < seq.type.size) ? (sort_limit_block_cache_stack_bytes / seq.type.size) : 0;
    let_(cache, u_S$raw) = u_init$S((seq.type)(A_ptr(cache_buf), cache_cap));
    $ignore_void sort_blockCtxCache(seq, ordFn, ctx, cache);
};

fn_((sort_blockCache(u_S$raw seq, sort_OrdFn ordFn, u_S$raw cache))(u_S$raw)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    return sort_blockCtxCache(seq, cmp_ordNoCtx, u_anyV(no_ctx), cache);
};

fn_((sort_blockCtxCache(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx, u_S$raw cache))(u_S$raw)) {
    if (seq.len <= 1) return seq;

    let_(min_level, usize) = sort_threshold_fallback_to_insert_sort;
    if (seq.len <= min_level) {
        sort_insertCtx(seq, ordFn, ctx);
        return seq;
    }
    var iter = sort_block__Iter_init(seq.len, min_level);
    /* O(N) insertion phase for lowest-level runs */
    while (!sort_block__Iter_finished(&iter)) {
        let range = sort_block__Iter_nextRange(&iter);
        sort_insertCtx(u_sliceS(seq, range), ordFn, ctx);
    }
    /* bottom-up hierarchical merging */
    while (true) {
        if (sort_block__Iter_len(&iter) < cache.len) {
            sort_block__Iter_begin(&iter);
            while (!sort_block__Iter_finished(&iter)) {
                let left = sort_block__Iter_nextRange(&iter);
                let right = sort_block__Iter_nextRange(&iter);
                if (R_len(left) == 0 || R_len(right) == 0) continue;

                let already_sorted = cmp_Ord_isLe(cmp_ordCtxP(
                    u_atS(seq, left.end - 1).as_const,
                    u_atS(seq, right.begin).as_const,
                    ctx,
                    ordFn
                ));
                if (already_sorted) continue;

                let reverse_sorted = cmp_Ord_isLt(cmp_ordCtxP(
                    u_atS(seq, right.end - 1).as_const,
                    u_atS(seq, left.begin).as_const,
                    ctx,
                    ordFn
                ));
                if (reverse_sorted) {
                    mem_rotate(u_sliceS(seq, $r(left.begin, right.end)), R_len(left));
                    continue;
                }

                if (R_len(left) <= cache.len) {
                    sort_block__mergeExternal(seq, left, right, cache, ordFn, ctx);
                } else if (R_len(right) <= cache.len) {
                    sort_block__mergeExternalRight(seq, left, right, cache, ordFn, ctx);
                } else {
                    sort_block__mergeInPlace(seq, left, right, ordFn, ctx);
                }
            }
        } else {
            sort_block__mergeWikiLevel(seq, &iter, cache, ordFn, ctx);
        }
        if (!sort_block__Iter_nextLevel(&iter)) break;
    }
    return seq;
};

fn_((sort_blockAlloc(u_S$raw seq, sort_OrdFn ordFn, mem_Alctr gpa))(mem_E$u_S$raw)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    return sort_blockCtxAlloc(seq, cmp_ordNoCtx, u_anyV(no_ctx), gpa);
};

$static fn_((sort_block__allocCache(mem_Alctr gpa, TypeInfo type, usize len))(mem_E$u_S$raw));
fn_((sort_blockCtxAlloc(u_S$raw seq, sort_OrdCtxFn ordFn, u_V$raw ctx, mem_Alctr gpa))(mem_E$u_S$raw) $scope) {
    let cache = try_(sort_block__allocCache(gpa, seq.type, seq.len));
    return_ok(sort_blockCtxCache(seq, ordFn, ctx, cache));
} $unscoped(fn);

fn_((sort_block__allocCache(mem_Alctr gpa, TypeInfo type, usize len))(mem_E$u_S$raw) $scope) {
    if_ok((mem_Alctr_alloc($trace gpa, type, len))(cover_full)) return_ok(cover_full);
    if_ok((mem_Alctr_alloc($trace gpa, type, len / 2))(cover_half)) return_ok(cover_half);
    return_ok(try_(mem_Alctr_alloc($trace gpa, type, len / 4)));
} $unscoped(fn);

/*--- Internal Definitions ---*/

fn_((sort_block__Iter_init(usize size2, usize min_level))(sort_block__Iter)) {
    let pow2 = uint_pow2Floor$((usize)(size2));
    let denom = pri_max(usize_(1), pow2 / min_level);
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
    let start = pri_min(self->dec, self->size);
    self->dec += self->dec_step;
    self->num += self->num_step;
    if (self->num >= self->denom) {
        self->num -= self->denom;
        self->dec += 1;
    }
    return $r(start, pri_min(self->dec, self->size));
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

$attr($inline_always)
fn_((sort_block__blockSwap(u_S$raw seq, usize lhs, usize rhs, usize len))(void)) {
    if (lhs == rhs || len == 0) return;
    for_(($rt(len))(i)) {
        if (lhs + i != rhs + i) {
            mem_swapP(u_atS(seq, lhs + i), u_atS(seq, rhs + i));
        }
    } $end(for);
};
$attr($inline_always)
fn_((sort_block__lessIdx(u_S$raw seq, usize lhs, usize rhs, sort_OrdCtxFn ordFn, u_V$raw ctx))(bool)) {
    return cmp_Ord_isLt(cmp_ordCtxP(
        u_atS(seq, lhs).as_const,
        u_atS(seq, rhs).as_const,
        ctx,
        ordFn
    ));
};
fn_((sort_block__sqrtFloor(usize value))(usize)) {
    var_(root, usize) = 0;
    var_(bit, usize) = usize_(1) << ((sizeof(usize) * 8) - 2);
    while (bit > value) bit >>= 2;
    while (bit != 0) {
        if (value >= root + bit) {
            value -= root + bit;
            root = (root >> 1) + bit;
        } else {
            root >>= 1;
        }
        bit >>= 2;
    }
    return root;
};
fn_((sort_block__binaryFirst(u_S$raw seq, u_P_const$raw value, R range, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize)) {
    var curr = range.begin;
    var size = R_len(range);
    if (range.begin >= range.end) return range.end;
    while (size > 0) {
        let offset = size % 2;
        size /= 2;
        let mid = curr + size;
        if (cmp_Ord_isLt(cmp_ordCtxP(u_atS(seq, mid).as_const, value, ctx, ordFn))) {
            curr += size + offset;
        }
    }
    return curr;
};
fn_((sort_block__binaryLast(u_S$raw seq, u_P_const$raw value, R range, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize)) {
    var curr = range.begin;
    var size = R_len(range);
    if (range.begin >= range.end) return range.end;
    while (size > 0) {
        let offset = size % 2;
        size /= 2;
        let mid = curr + size;
        if (!cmp_Ord_isLt(cmp_ordCtxP(value, u_atS(seq, mid).as_const, ctx, ordFn))) {
            curr += size + offset;
        }
    }
    return curr;
};
fn_((sort_block__findFirstForward(u_S$raw seq, u_P_const$raw value, R range, usize unique, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize)) {
    if (R_len(range) == 0) return range.begin;
    let skip = pri_max(R_len(range) / unique, usize_(1));

    var index = range.begin + skip;
    while (cmp_Ord_isLt(cmp_ordCtxP(u_atS(seq, index - 1).as_const, value, ctx, ordFn))) {
        if (index >= range.end - skip) {
            return sort_block__binaryFirst(seq, value, $r(index, range.end), ordFn, ctx);
        }
        index += skip;
    }
    return sort_block__binaryFirst(seq, value, $r(index - skip, index), ordFn, ctx);
};
fn_((sort_block__findFirstBackward(u_S$raw seq, u_P_const$raw value, R range, usize unique, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize)) {
    if (R_len(range) == 0) return range.begin;
    let skip = pri_max(R_len(range) / unique, usize_(1));

    var index = range.end - skip;
    while (index > range.begin && !cmp_Ord_isLt(cmp_ordCtxP(u_atS(seq, index - 1).as_const, value, ctx, ordFn))) {
        if (index < range.begin + skip) {
            return sort_block__binaryFirst(seq, value, $r(range.begin, index), ordFn, ctx);
        }
        index -= skip;
    }
    return sort_block__binaryFirst(seq, value, $r(index, index + skip), ordFn, ctx);
};
fn_((sort_block__findLastForward(u_S$raw seq, u_P_const$raw value, R range, usize unique, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize)) {
    if (R_len(range) == 0) return range.begin;
    let skip = pri_max(R_len(range) / unique, usize_(1));

    var index = range.begin + skip;
    while (!cmp_Ord_isLt(cmp_ordCtxP(value, u_atS(seq, index - 1).as_const, ctx, ordFn))) {
        if (index >= range.end - skip) {
            return sort_block__binaryLast(seq, value, $r(index, range.end), ordFn, ctx);
        }
        index += skip;
    }
    return sort_block__binaryLast(seq, value, $r(index - skip, index), ordFn, ctx);
};
fn_((sort_block__findLastBackward(u_S$raw seq, u_P_const$raw value, R range, usize unique, sort_OrdCtxFn ordFn, u_V$raw ctx))(usize)) {
    if (R_len(range) == 0) return range.begin;
    let skip = pri_max(R_len(range) / unique, usize_(1));

    var index = range.end - skip;
    while (index > range.begin && cmp_Ord_isLt(cmp_ordCtxP(value, u_atS(seq, index - 1).as_const, ctx, ordFn))) {
        if (index < range.begin + skip) {
            return sort_block__binaryLast(seq, value, $r(range.begin, index), ordFn, ctx);
        }
        index -= skip;
    }
    return sort_block__binaryLast(seq, value, $r(index, index + skip), ordFn, ctx);
};

fn_((sort_block__mergeWikiLevel(
    u_S$raw seq, sort_block__Iter* iter, u_S$raw cache,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void)) {
    var block_size = sort_block__sqrtFloor(sort_block__Iter_len(iter));
    if (block_size == 0) block_size = 1;
    var buffer_size = sort_block__Iter_len(iter) / block_size + 1;

    var_(left, R) = {};
    var_(right, R) = {};
    var_(index, usize) = 0;
    var_(last, usize) = 0;
    var_(count, usize) = 0;
    var_(find, usize) = 0;
    var_(start, usize) = 0;
    var_(pull_index, usize) = 0;
    var pull = A_from$((sort_block__Pull){
        { .from = 0, .to = 0, .count = 0, .range = $r(0, 0) },
        { .from = 0, .to = 0, .count = 0, .range = $r(0, 0) } $listed
    });

    var buffer1 = $r(0, 0);
    var buffer2 = $r(0, 0);
    find = buffer_size + buffer_size;
    var_(find_separately, bool) = false;

    if (block_size <= cache.len) {
        find = buffer_size;
    } else if (find > sort_block__Iter_len(iter)) {
        find = buffer_size;
        find_separately = true;
    }

    sort_block__Iter_begin(iter);
    while (!sort_block__Iter_finished(iter)) {
        left = sort_block__Iter_nextRange(iter);
        right = sort_block__Iter_nextRange(iter);
        if (R_len(left) == 0 || R_len(right) == 0) continue;

        last = left.begin;
        count = 1;
        while (count < find) {
            index = sort_block__findLastForward(
                seq,
                u_atS(seq, last).as_const,
                $r(last + 1, left.end),
                find - count,
                ordFn,
                ctx
            );
            if (index == left.end) break;
            last = index;
            count++;
        }
        index = last;

        if (count >= buffer_size) {
            *A_at((pull)[pull_index]) = (sort_block__Pull){
                .range = $r(left.begin, right.end),
                .count = count,
                .from = index,
                .to = left.begin,
            };
            pull_index = 1;

            if (count == buffer_size + buffer_size) {
                buffer1 = $r(left.begin, left.begin + buffer_size);
                buffer2 = $r(left.begin + buffer_size, left.begin + count);
                break;
            } else if (find == buffer_size + buffer_size) {
                buffer1 = $r(left.begin, left.begin + count);
                find = buffer_size;
            } else if (block_size <= cache.len) {
                buffer1 = $r(left.begin, left.begin + count);
                break;
            } else if (find_separately) {
                buffer1 = $r(left.begin, left.begin + count);
                find_separately = false;
            } else {
                buffer2 = $r(left.begin, left.begin + count);
                break;
            }
        } else if (pull_index == 0 && count > R_len(buffer1)) {
            buffer1 = $r(left.begin, left.begin + count);
            *A_at((pull)[pull_index]) = (sort_block__Pull){
                .range = $r(left.begin, right.end),
                .count = count,
                .from = index,
                .to = left.begin,
            };
        }

        last = right.end - 1;
        count = 1;
        while (count < find) {
            index = sort_block__findFirstBackward(
                seq,
                u_atS(seq, last).as_const,
                $r(right.begin, last),
                find - count,
                ordFn,
                ctx
            );
            if (index == right.begin) break;
            last = index - 1;
            count++;
        }
        index = last;

        if (count >= buffer_size) {
            *A_at((pull)[pull_index]) = (sort_block__Pull){
                .range = $r(left.begin, right.end),
                .count = count,
                .from = index,
                .to = right.end,
            };
            pull_index = 1;

            if (count == buffer_size + buffer_size) {
                buffer1 = $r(right.end - count, right.end - buffer_size);
                buffer2 = $r(right.end - buffer_size, right.end);
                break;
            } else if (find == buffer_size + buffer_size) {
                buffer1 = $r(right.end - count, right.end);
                find = buffer_size;
            } else if (block_size <= cache.len) {
                buffer1 = $r(right.end - count, right.end);
                break;
            } else if (find_separately) {
                buffer1 = $r(right.end - count, right.end);
                find_separately = false;
            } else {
                if (A_at((pull)[0])->range.begin == left.begin) {
                    A_at((pull)[0])->range.end -= A_at((pull)[1])->count;
                }
                buffer2 = $r(right.end - count, right.end);
                break;
            }
        } else if (pull_index == 0 && count > R_len(buffer1)) {
            buffer1 = $r(right.end - count, right.end);
            *A_at((pull)[pull_index]) = (sort_block__Pull){
                .range = $r(left.begin, right.end),
                .count = count,
                .from = index,
                .to = right.end,
            };
        }
    }

    for_(($rt(2))(pull_idx)) {
        let pull_ref = A_at((pull)[pull_idx]);
        let length = pull_ref->count;
        if (length == 0) continue;

        if (pull_ref->to < pull_ref->from) {
            index = pull_ref->from;
            count = 1;
            while (count < length) {
                index = sort_block__findFirstBackward(
                    seq,
                    u_atS(seq, index - 1).as_const,
                    $r(pull_ref->to, pull_ref->from - (count - 1)),
                    length - count,
                    ordFn,
                    ctx
                );
                let range = $r(index + 1, pull_ref->from + 1);
                mem_rotate(u_sliceS(seq, range), R_len(range) - count);
                pull_ref->from = index + count;
                count++;
            }
        } else if (pull_ref->to > pull_ref->from) {
            index = pull_ref->from + 1;
            count = 1;
            while (count < length) {
                index = sort_block__findLastForward(
                    seq,
                    u_atS(seq, index).as_const,
                    $r(index, pull_ref->to),
                    length - count,
                    ordFn,
                    ctx
                );
                let range = $r(pull_ref->from, index - 1);
                mem_rotate(u_sliceS(seq, range), count);
                pull_ref->from = index - 1 - count;
                count++;
            }
        }
    } $end(for);

    buffer_size = R_len(buffer1);
    if (buffer_size == 0) {
        sort_block__Iter_begin(iter);
        while (!sort_block__Iter_finished(iter)) {
            left = sort_block__Iter_nextRange(iter);
            right = sort_block__Iter_nextRange(iter);
            if (R_len(left) == 0 || R_len(right) == 0) continue;
            sort_block__mergeInPlace(seq, left, right, ordFn, ctx);
        }
        return;
    }
    block_size = sort_block__Iter_len(iter) / buffer_size + 1;

    sort_block__Iter_begin(iter);
    while (!sort_block__Iter_finished(iter)) {
        left = sort_block__Iter_nextRange(iter);
        right = sort_block__Iter_nextRange(iter);
        if (R_len(left) == 0 || R_len(right) == 0) continue;

        start = left.begin;
        for_(($rt(2))(pull_idx)) {
            let pull_ref = A_at((pull)[pull_idx]);
            if (start == pull_ref->range.begin) {
                if (pull_ref->from > pull_ref->to) {
                    left.begin += pull_ref->count;
                } else if (pull_ref->from < pull_ref->to) {
                    right.end -= pull_ref->count;
                }
            }
        } $end(for);
        if (R_len(left) == 0 || R_len(right) == 0) continue;

        if (sort_block__lessIdx(seq, right.end - 1, left.begin, ordFn, ctx)) {
            mem_rotate(u_sliceS(seq, $r(left.begin, right.end)), R_len(left));
        } else if (sort_block__lessIdx(seq, right.begin, left.end - 1, ordFn, ctx)) {
            var_(find_left, usize) = 0;
            var block_left = $r(left.begin, left.end);
            var first_left = $r(left.begin, left.begin + (R_len(block_left) % block_size));

            var index_left = buffer1.begin;
            index = first_left.end;
            while (index < block_left.end) {
                if (index_left != index) {
                    mem_swapP(u_atS(seq, index_left), u_atS(seq, index));
                }
                index_left++;
                index += block_size;
            }

            var last_left = first_left;
            var last_right = $r(0, 0);
            var block_right = $r(right.begin, right.begin + pri_min(block_size, R_len(right)));
            block_left.begin += R_len(first_left);
            index_left = buffer1.begin;

            if (R_len(last_left) <= cache.len) {
                u_memcpyS(u_prefixS(cache, R_len(last_left)), u_sliceS(seq, last_left).as_const);
            } else if (R_len(buffer2) > 0) {
                sort_block__blockSwap(seq, last_left.begin, buffer2.begin, R_len(last_left));
            }

            if (R_len(block_left) > 0) {
                while (true) {
                    if ((R_len(last_right) > 0 && !sort_block__lessIdx(seq, last_right.end - 1, index_left, ordFn, ctx))
                        || R_len(block_right) == 0) {
                        let right_split = sort_block__binaryFirst(
                            seq,
                            u_atS(seq, index_left).as_const,
                            last_right,
                            ordFn,
                            ctx
                        );
                        let right_remaining = last_right.end - right_split;

                        var min_left = block_left.begin;
                        find_left = min_left + block_size;
                        while (find_left < block_left.end) {
                            if (sort_block__lessIdx(seq, find_left, min_left, ordFn, ctx)) {
                                min_left = find_left;
                            }
                            find_left += block_size;
                        }
                        sort_block__blockSwap(seq, block_left.begin, min_left, block_size);

                        if (block_left.begin != index_left) {
                            mem_swapP(u_atS(seq, block_left.begin), u_atS(seq, index_left));
                        }
                        index_left++;

                        if (R_len(last_left) <= cache.len) {
                            sort_block__mergeExternalCached(seq, last_left, $r(last_left.end, right_split), cache, ordFn, ctx);
                        } else if (R_len(buffer2) > 0) {
                            sort_block__mergeInternal(seq, last_left, $r(last_left.end, right_split), buffer2, ordFn, ctx);
                        } else {
                            sort_block__mergeInPlace(seq, last_left, $r(last_left.end, right_split), ordFn, ctx);
                        }

                        if (R_len(buffer2) > 0 || block_size <= cache.len) {
                            if (block_size <= cache.len) {
                                u_memcpyS(u_prefixS(cache, block_size), u_sliceS(seq, $r(block_left.begin, block_left.begin + block_size)).as_const);
                            } else {
                                sort_block__blockSwap(seq, block_left.begin, buffer2.begin, block_size);
                            }
                            sort_block__blockSwap(seq, right_split, block_left.begin + block_size - right_remaining, right_remaining);
                        } else {
                            mem_rotate(u_sliceS(seq, $r(right_split, block_left.begin + block_size)), block_left.begin - right_split);
                        }

                        last_left = $r(block_left.begin - right_remaining, block_left.begin - right_remaining + block_size);
                        last_right = $r(last_left.end, last_left.end + right_remaining);

                        block_left.begin += block_size;
                        if (R_len(block_left) == 0) break;
                    } else if (R_len(block_right) < block_size) {
                        mem_rotate(u_sliceS(seq, $r(block_left.begin, block_right.end)), block_right.begin - block_left.begin);
                        last_right = $r(block_left.begin, block_left.begin + R_len(block_right));
                        block_left.begin += R_len(block_right);
                        block_left.end += R_len(block_right);
                        block_right.end = block_right.begin;
                    } else {
                        sort_block__blockSwap(seq, block_left.begin, block_right.begin, block_size);
                        last_right = $r(block_left.begin, block_left.begin + block_size);

                        block_left.begin += block_size;
                        block_left.end += block_size;
                        block_right.begin += block_size;

                        if (block_right.end > right.end - block_size) {
                            block_right.end = right.end;
                        } else {
                            block_right.end += block_size;
                        }
                    }
                }
            }

            if (R_len(last_left) <= cache.len) {
                sort_block__mergeExternalCached(seq, last_left, $r(last_left.end, right.end), cache, ordFn, ctx);
            } else if (R_len(buffer2) > 0) {
                sort_block__mergeInternal(seq, last_left, $r(last_left.end, right.end), buffer2, ordFn, ctx);
            } else {
                sort_block__mergeInPlace(seq, last_left, $r(last_left.end, right.end), ordFn, ctx);
            }
        }
    }

    sort_insertCtx(u_sliceS(seq, buffer2), ordFn, ctx);

    for_(($rt(2))(pull_idx)) {
        let pull_ref = A_at((pull)[pull_idx]);
        var unique = pull_ref->count * 2;
        if (pull_ref->count == 0) continue;

        if (pull_ref->from > pull_ref->to) {
            var buffer = $r(pull_ref->range.begin, pull_ref->range.begin + pull_ref->count);
            while (R_len(buffer) > 0) {
                index = sort_block__findFirstForward(
                    seq,
                    u_atS(seq, buffer.begin).as_const,
                    $r(buffer.end, pull_ref->range.end),
                    unique,
                    ordFn,
                    ctx
                );
                let amount = index - buffer.end;
                mem_rotate(u_sliceS(seq, $r(buffer.begin, index)), R_len(buffer));
                buffer.begin += amount + 1;
                buffer.end += amount;
                unique -= 2;
            }
        } else if (pull_ref->from < pull_ref->to) {
            var buffer = $r(pull_ref->range.end - pull_ref->count, pull_ref->range.end);
            while (R_len(buffer) > 0) {
                index = sort_block__findLastBackward(
                    seq,
                    u_atS(seq, buffer.end - 1).as_const,
                    $r(pull_ref->range.begin, buffer.begin),
                    unique,
                    ordFn,
                    ctx
                );
                let amount = buffer.begin - index;
                mem_rotate(u_sliceS(seq, $r(index, buffer.end)), amount);
                buffer.begin -= amount;
                buffer.end -= amount + 1;
                unique -= 2;
            }
        }
    } $end(for);
};

fn_((sort_block__mergeInPlace(
    u_S$raw seq, R left, R right,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void)) {
    if (R_len(left) == 0 || R_len(right) == 0) return;
    while (true) {
        let pivot_ptr = u_atS(seq.as_const, left.begin);
        let pivot_ctx = l$((sort__SearchOrdAdpCtx){
            .val_ptr = pivot_ptr,
            .inner = ctx,
            .ordFn = ordFn,
        });
        let split_offset = search_lowerBound(
            u_sliceS(seq, right).as_const,
            u_anyV(pivot_ctx),
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
        let left_pivot_ctx = l$((sort__SearchOrdAdpCtx){
            .val_ptr = left_pivot_ptr,
            .inner = ctx,
            .ordFn = ordFn,
        });
        let skip_offset = search_upperBound(
            u_sliceS(seq, left).as_const,
            u_anyV(left_pivot_ctx),
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
    sort_block__mergeExternalCached(seq, left, right, cache, ordFn, ctx);
};
fn_((sort_block__mergeExternalCached(
    u_S$raw seq, R left, R right, u_S$raw cache,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void)) {
    let left_len = R_len(left);

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
            if (dst_idx != right_idx) {
                u_memcpy(u_atS(seq, dst_idx), u_atS(seq, right_idx).as_const);
            }
            right_idx++;
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
            u_sliceS(cache, $r(cache_idx, left_len)).as_const
        );
    }
};
fn_((sort_block__mergeInternal(
    u_S$raw seq, R left, R right, R buffer,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void)) {
    var_(left_count, usize) = 0;
    var_(right_count, usize) = 0;
    var_(insert, usize) = 0;
    let left_len = R_len(left);
    let right_len = R_len(right);

    if (0 < left_len && 0 < right_len) {
        while (true) {
            let ord = cmp_ordCtxP(
                u_atS(seq, right.begin + right_count).as_const,
                u_atS(seq, buffer.begin + left_count).as_const,
                ctx,
                ordFn
            );
            if (!cmp_Ord_isLt(ord)) {
                if (left.begin + insert != buffer.begin + left_count) {
                    mem_swapP(u_atS(seq, left.begin + insert), u_atS(seq, buffer.begin + left_count));
                }
                left_count++;
                insert++;
                if (left_count >= left_len) break;
            } else {
                if (left.begin + insert != right.begin + right_count) {
                    mem_swapP(u_atS(seq, left.begin + insert), u_atS(seq, right.begin + right_count));
                }
                right_count++;
                insert++;
                if (right_count >= right_len) break;
            }
        }
    }

    sort_block__blockSwap(seq, buffer.begin + left_count, left.begin + insert, left_len - left_count);
};
fn_((sort_block__mergeExternalRight(
    u_S$raw seq, R left, R right, u_S$raw cache,
    sort_OrdCtxFn ordFn, u_V$raw ctx
))(void)) {
    let right_len = R_len(right);
    /* cache right run into external memory and merge backward */
    u_memcpyS(u_prefixS(cache, right_len), u_sliceS(seq, right).as_const);

    var_(left_idx, usize) = left.end;
    var_(cache_idx, usize) = right_len;
    var_(dst_idx, usize) = right.end;

    while (left.begin < left_idx && 0 < cache_idx) {
        let ord = cmp_ordCtxP(
            u_atS(cache, cache_idx - 1).as_const,
            u_atS(seq, left_idx - 1).as_const,
            ctx,
            ordFn
        );
        dst_idx--;
        if (cmp_Ord_isLt(ord)) {
            left_idx--;
            if (dst_idx != left_idx) {
                u_memcpy(u_atS(seq, dst_idx), u_atS(seq, left_idx).as_const);
            }
        } else {
            cache_idx--;
            u_memcpy(u_atS(seq, dst_idx), u_atS(cache, cache_idx).as_const);
        }
    }

    if (0 < cache_idx) {
        u_memcpyS(
            u_sliceS(seq, $r(left.begin, left.begin + cache_idx)),
            u_prefixS(cache, cache_idx).as_const
        );
    }
};

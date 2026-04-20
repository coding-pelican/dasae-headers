#include "dh/search.h"
#include "dh/mem.h"

fn_((search_linearFirst(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(O$usize)) {
    return eval_(O$usize $scope)(for_($fwd($r(0, seq.len))(i)) {
        if (!cmp_Ord_isEq(cmp_ordP(u_atS(seq, i), ctx.ref.as_const, ordFn))) continue;
        $break_(some(i));
    } $end(for)) eval_(else)({
        $break_(none());
    }) $unscoped(eval);
};

fn_((search_linearLast(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(O$usize)) {
    return eval_(O$usize $scope)(for_($bwd($r(0, seq.len))(i)) {
        if (!cmp_Ord_isEq(cmp_ordP(u_atS(seq, i), ctx.ref.as_const, ordFn))) continue;
        $break_(some(i));
    } $end(for)) eval_(else)({
        $break_(none());
    }) $unscoped(eval);
};

fn_((search_binary(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(O$usize) $scope) {
    var_(low, usize) = 0;
    var_(high, usize) = seq.len;
    while (low < high) {
        let mid = low + (high - low) / 2;
        switch (cmp_ordP(u_atS(seq, mid), ctx.ref.as_const, ordFn)) {
        case_((cmp_Ord_lt)) low = mid + 1; continue $end(case);
        case_((cmp_Ord_gt)) high = mid; continue $end(case);
        case_((cmp_Ord_eq)) return_some(mid) $end(case);
        }
    }
    return_none();
} $unscoped(fn);

$attr($inline_always)
$static fn_((search__pred(u_V$raw val, u_V$raw ctx, search_PredFn predFn))(bool)) {
    return predFn(u_load(val), u_load(ctx));
};
$attr($inline_always)
$static fn_((search__predP(u_P_const$raw val, u_V$raw ctx, search_PredFn predFn))(bool)) {
    return search__pred(u_deref(val), ctx, predFn);
};

typedef struct search__PredFn_BoundCtx {
    u_P_const$raw inner;
    search_OrdFn ordFn;
} search__PredFn_BoundCtx;

$attr($inline_always)
$static fn_((search_lowerBound__pred(u_V$raw val, u_V$raw ctx))(bool)) {
    let bound_ctx = u_castV$((search__PredFn_BoundCtx)(ctx));
    let inner_ctx = bound_ctx.inner;
    return cmp_Ord_isLt(cmp_ordP(
        val.ref.as_const,
        inner_ctx,
        bound_ctx.ordFn
    ));
};

fn_((search_lowerBound(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(usize)) {
    let_(bound_ctx, search__PredFn_BoundCtx) = {
        .inner = ctx.ref.as_const,
        .ordFn = ordFn,
    };
    return search_partPoint(seq, u_anyV(bound_ctx), search_lowerBound__pred);
};

$attr($inline_always)
$static fn_((search_upperBound__pred(u_V$raw val, u_V$raw ctx))(bool)) {
    let bound_ctx = u_castV$((search__PredFn_BoundCtx)(ctx));
    let inner_ctx = bound_ctx.inner;
    return !cmp_Ord_isGt(cmp_ordP(
        val.ref.as_const,
        inner_ctx,
        bound_ctx.ordFn
    ));
};

fn_((search_upperBound(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(usize)) {
    let_(bound_ctx, search__PredFn_BoundCtx) = {
        .inner = ctx.ref.as_const,
        .ordFn = ordFn,
    };
    return search_partPoint(seq, u_anyV(bound_ctx), search_upperBound__pred);
};

fn_((search_partPoint(u_S_const$raw seq, u_V$raw ctx, search_PredFn predFn))(usize)) {
    var_(low, usize) = 0;
    var_(high, usize) = seq.len;
    while (low < high) {
        let mid = low + (high - low) / 2;
        if (search__predP(u_atS(seq, mid), ctx, predFn)) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
};

fn_((search_eqRange(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(R)) {
    var_(low, usize) = 0;
    var_(high, usize) = seq.len;
    while (low < high) {
        let mid = low + (high - low) / 2;
        switch (cmp_ordP(u_atS(seq, mid), ctx.ref.as_const, ordFn)) {
        case_((cmp_Ord_lt)) low = mid + 1; continue $end(case);
        case_((cmp_Ord_gt)) high = mid; continue $end(case);
        case_((cmp_Ord_eq)) return $r(
            low + search_lowerBound(u_sliceS(seq, $r(low, mid)), ctx, ordFn),
            mid + search_upperBound(u_sliceS(seq, $r(mid, high)), ctx, ordFn)
        ) $end(case);
        }
    }
    return $r(low, low);
};

$attr($inline_always)
$static fn_((search__firstMinMax(u_S_const$raw seq, search_OrdFn ordFn))(search_MinMax) $scope) {
    claim_assert_nonnullS(seq);
    claim_assert(seq.len > 0);
    let begin = 0;
    var min_ref = u_atS(seq, begin);
    var max_ref = u_atS(seq, begin);
    var min_idx = usize_(begin);
    var max_idx = usize_(begin);
    for_(($us(u_suffixS(seq, begin + 1)), $rf(begin + 1))(item, idx)) {
        if (cmp_Ord_isLt(cmp_ordP(item, min_ref, ordFn))) {
            min_ref = item;
            min_idx = idx;
        }
        if (cmp_Ord_isGt(cmp_ordP(item, max_ref, ordFn))) {
            max_ref = item;
            max_idx = idx;
        }
    } $end(for);
    return_({
        .min = min_idx,
        .max = max_idx,
    });
} $unscoped(fn);

$attr($inline_always)
$static fn_((search__lastMinMax(u_S_const$raw seq, search_OrdFn ordFn))(search_MinMax) $scope) {
    claim_assert_nonnullS(seq);
    claim_assert(seq.len > 0);
    let end = seq.len - 1;
    var min_ref = u_atS(seq, end);
    var max_ref = u_atS(seq, end);
    var min_idx = usize_(end);
    var max_idx = usize_(end);
    for_($rev($us(u_prefixS(seq, end)), $rt(end))(item, idx)) {
        if (cmp_Ord_isLt(cmp_ordP(item, min_ref, ordFn))) {
            min_ref = item;
            min_idx = idx;
        }
        if (cmp_Ord_isGt(cmp_ordP(item, max_ref, ordFn))) {
            max_ref = item;
            max_idx = idx;
        }
    } $end(for);
    return_({
        .min = min_idx,
        .max = max_idx,
    });
} $unscoped(fn);

fn_((search_firstMinMax(u_S_const$raw seq, search_OrdFn ordFn))(search_MinMax)) {
    return search__firstMinMax(seq, ordFn);
};

fn_((search_lastMinMax(u_S_const$raw seq, search_OrdFn ordFn))(search_MinMax)) {
    return search__lastMinMax(seq, ordFn);
};

fn_((search_firstMin(u_S_const$raw seq, search_OrdFn ordFn))(usize)) {
    return search__firstMinMax(seq, ordFn).min;
};

fn_((search_lastMin(u_S_const$raw seq, search_OrdFn ordFn))(usize)) {
    return search__lastMinMax(seq, ordFn).min;
};

fn_((search_firstMax(u_S_const$raw seq, search_OrdFn ordFn))(usize)) {
    return search__firstMinMax(seq, ordFn).max;
};

fn_((search_lastMax(u_S_const$raw seq, search_OrdFn ordFn))(usize)) {
    return search__lastMinMax(seq, ordFn).max;
};

fn_((search_firstUnit(u_S_const$raw seq, u_V$raw ctx_unit, search_EqlFn eqlFn))(O$usize $scope)) {
    for_(($us(seq), $rf(0))(val, idx)) {
        let ctx = ctx_unit.ref.as_const;
        if (cmp_eqlP(val, ctx, eqlFn)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((search_lastUnit(u_S_const$raw seq, u_V$raw ctx_unit, search_EqlFn eqlFn))(O$usize $scope)) {
    for_($rev($us(seq), $rt(seq.len))(val, idx)) {
        let ctx = ctx_unit.ref.as_const;
        if (cmp_eqlP(val, ctx, eqlFn)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((search_firstSeq(u_S_const$raw seq, u_S_const$raw ctx_seq, search_EqlFn eqlFn))(O$usize $scope)) {
    if (seq.len < ctx_seq.len) return_none();
    let end = seq.len - ctx_seq.len;
    for_(($rt($incl(end)))(idx)) {
        let seq_part = u_prefixS(u_suffixS(seq, idx), ctx_seq.len);
        if (cmp_eqlS(seq_part, ctx_seq, eqlFn)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((search_lastSeq(u_S_const$raw seq, u_S_const$raw ctx_seq, search_EqlFn eqlFn))(O$usize $scope)) {
    if (seq.len < ctx_seq.len) return_none();
    let end = seq.len - ctx_seq.len;
    for_($rev($rt($incl(end)))(idx)) {
        let seq_part = u_prefixS(u_suffixS(seq, idx), ctx_seq.len);
        if (cmp_eqlS(seq_part, ctx_seq, eqlFn)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((search_firstAny(u_S_const$raw seq, u_S_const$raw ctx_any, search_EqlFn eqlFn))(O$usize $scope)) {
    for_(($us(seq), $rf(0))(val, idx)) {
        for_(($us(ctx_any))(ctx)) {
            if (cmp_eqlP(val, ctx, eqlFn)) return_some(idx);
        } $end(for);
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((search_lastAny(u_S_const$raw seq, u_S_const$raw ctx_any, search_EqlFn eqlFn))(O$usize $scope)) {
    for_($rev($us(seq), $rf(0))(val, idx)) {
        for_(($us(ctx_any))(ctx)) {
            if (cmp_eqlP(val, ctx, eqlFn)) return_some(idx);
        } $end(for);
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((search_firstNone(u_S_const$raw seq, u_S_const$raw ctx_none, search_EqlFn eqlFn))(O$usize $scope)) {
    loop_labeled(outer, for_(($us(seq), $rf(0))(val, idx))) {
        for_(($us(ctx_none))(ctx)) {
            if (cmp_eqlP(val, ctx, eqlFn)) loop_continue_(outer);
        } $end(for);
        return_some(idx);
    } $end(loop_labeled) $end(for);
    return_none();
} $unscoped(fn);

fn_((search_lastNone(u_S_const$raw seq, u_S_const$raw ctx_none, search_EqlFn eqlFn))(O$usize $scope)) {
    loop_labeled(outer, for_($rev($us(seq), $rf(0))(val, idx))) {
        for_(($us(ctx_none))(ctx)) {
            if (cmp_eqlP(val, ctx, eqlFn)) loop_continue_(outer);
        } $end(for);
        return_some(idx);
    } $end(loop_labeled) $end(for);
    return_none();
} $unscoped(fn);

fn_((search_firstDiff(u_S_const$raw seq, u_S_const$raw ctx_diff, search_EqlFn eqlFn))(O$usize $scope)) {
    let shortest = pri_min(seq.len, ctx_diff.len);
    if (seq.ptr == ctx_diff.ptr) return expr_(ReturnType $scope)(
        if (seq.len == ctx_diff.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
    for_(($rt(shortest))(idx)) {
        if (!cmp_eqlP(u_atS(seq, idx), u_atS(ctx_diff, idx), eqlFn)) return_some(idx);
    } $end(for);
    return expr_(ReturnType $scope)(
        if (seq.len == ctx_diff.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
} $unscoped(fn);

fn_((search_lastDiff(u_S_const$raw seq, u_S_const$raw ctx_diff, search_EqlFn eqlFn))(O$usize $scope)) {
    let shortest = pri_min(seq.len, ctx_diff.len);
    if (seq.ptr == ctx_diff.ptr) return expr_(ReturnType $scope)(
        if (seq.len == ctx_diff.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
    for_($rev($rt(shortest))(idx)) {
        if (!cmp_eqlP(u_atS(seq, idx), u_atS(ctx_diff, idx), eqlFn)) return_some(idx);
    } $end(for);
    return expr_(ReturnType $scope)(
        if (seq.len == ctx_diff.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
} $unscoped(fn);

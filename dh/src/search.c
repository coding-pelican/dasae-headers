#include "dh/search.h"

$attr($inline_always)
$static fn_((search__ord(search_OrdFn ordFn, u_P_const$raw val, u_P_const$raw ctx))(cmp_Ord)) {
    return invoke(ordFn, u_load(u_deref(val)), u_load(u_deref(ctx)));
};

fn_((search_linearFirst(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(O$usize)) {
    return eval_(O$usize $scope)(for_($fwd($r(0, seq.len))(i) {
        if (!cmp_Ord_isEq(search__ord(ordFn, u_atS(seq, i), ctx.ref.as_const))) { continue; }
        $break_(some(i));
    })) eval_(else)({
        $break_(none());
    }) $unscoped_(eval);
};

fn_((search_linearLast(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(O$usize)) {
    return eval_(O$usize $scope)(for_($bwd($r(0, seq.len))(i) {
        if (!cmp_Ord_isEq(search__ord(ordFn, u_atS(seq, i), ctx.ref.as_const))) { continue; }
        $break_(some(i));
    })) eval_(else)({
        $break_(none());
    }) $unscoped_(eval);
};

fn_((search_binary(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(O$usize) $scope) {
    var_(low, usize) = 0;
    var_(high, usize) = seq.len;
    while (low < high) {
        let mid = low + (high - low) / 2;
        switch (search__ord(ordFn, u_atS(seq, mid), ctx.ref.as_const)) /* clang-format off */ {
            case_((cmp_Ord_lt)) { low = mid + 1; continue; } $end(case);
            case_((cmp_Ord_gt)) { high = mid; continue; } $end(case);
            case_((cmp_Ord_eq)) { return_some(mid); } $end(case);
        } /* clang-format on */
    }
    return_none();
} $unscoped_(fn);

$attr($inline_always)
$static fn_((search__pred(search_PredFn predFn, u_P_const$raw lhs, u_P_const$raw rhs))(bool)) {
    return invoke(predFn, u_load(u_deref(lhs)), u_load(u_deref(rhs)));
};

typedef struct search_PredFn_BoundCtx {
    u_P_const$raw inner;
    search_OrdFn ordFn;
} search_PredFn_BoundCtx;

$attr($inline_always)
$static fn_((search_lowerBound__pred(u_V$raw val, u_V$raw ctx))(bool)) {
    let bound_ctx = u_castV$((search_PredFn_BoundCtx)(ctx));
    let inner_ctx = bound_ctx.inner;
    return cmp_Ord_isLt(search__ord(
        bound_ctx.ordFn,
        val.ref.as_const,
        inner_ctx
    ));
};

fn_((search_lowerBound(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(usize)) {
    let_(bound_ctx, search_PredFn_BoundCtx) = {
        .inner = ctx.ref.as_const,
        .ordFn = ordFn,
    };
    return search_partPoint(seq, u_anyV(bound_ctx), (search_PredFn)wrapFn(search_lowerBound__pred));
};

$attr($inline_always)
$static fn_((search_upperBound__pred(u_V$raw val, u_V$raw ctx))(bool)) {
    let bound_ctx = u_castV$((search_PredFn_BoundCtx)(ctx));
    let inner_ctx = bound_ctx.inner;
    return !cmp_Ord_isGt(search__ord(
        bound_ctx.ordFn,
        val.ref.as_const,
        inner_ctx
    ));
};

fn_((search_upperBound(u_S_const$raw seq, u_V$raw ctx, search_OrdFn ordFn))(usize)) {
    let_(bound_ctx, search_PredFn_BoundCtx) = {
        .inner = ctx.ref.as_const,
        .ordFn = ordFn,
    };
    return search_partPoint(seq, u_anyV(bound_ctx), (search_PredFn)wrapFn(search_upperBound__pred));
};

fn_((search_partPoint(u_S_const$raw seq, u_V$raw ctx, search_PredFn predFn))(usize)) {
    var_(low, usize) = 0;
    var_(high, usize) = seq.len;
    while (low < high) {
        let mid = low + (high - low) / 2;
        if (search__pred(predFn, u_atS(seq, mid), ctx.ref.as_const)) {
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
        switch (search__ord(ordFn, u_atS(seq, mid), ctx.ref.as_const)) /* clang-format off */ {
            case_((cmp_Ord_lt)) { low = mid + 1; continue; } $end(case);
            case_((cmp_Ord_gt)) { high = mid; continue; } $end(case);
            case_((cmp_Ord_eq)) { return $r(
                low + search_lowerBound(u_sliceS(seq, $r(low, mid)), ctx, ordFn),
                mid + search_upperBound(u_sliceS(seq, $r(mid, high)), ctx, ordFn)
            ); } $end(case);
        } /* clang-format on */
    }
    return $r(low, low);
};

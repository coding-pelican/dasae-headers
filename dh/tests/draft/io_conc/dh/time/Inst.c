#include "Inst.h"

#if plat_based_unix
#include <time.h>
#endif /* plat_based_unix */

/*========== Internal Definitions ===========================================*/

#if plat_is_windows
$static fn_((time_Inst__windows_freqTicksPerSec(void))(u64)) {
    var freq = l0$((time_InstPlatform));
    if (!QueryPerformanceFrequency(&freq)) {
        claim_unreachable_msg("Failed to query performance frequency");
    }
    return as$(u64)(freq.QuadPart);
};

$static fn_((time_Inst__windows_durToTicks(time_Dur dur))(O$u64) $scope) {
    let freq = time_Inst__windows_freqTicksPerSec();
    let nanos_ticks = (as$(u64)(dur.nanos) * freq) / as$(u64)(time_Inst_nanos_per_sec);
    if_some((u64_mulChkd(dur.secs, freq))(secs_ticks)) {
        if_some((u64_addChkd(secs_ticks, nanos_ticks))(ticks)) {
            return_some(ticks);
        }
    }
    return_none();
} $unscoped(fn);
#endif /* plat_is_windows */

/*========== External Definitions ===========================================*/

/* --- Accessors --- */

fn_((time_Inst_freq(time_Self time))(time_Inst)) {
    claim_assert_nonnull(time.ctx);
    claim_assert_nonnull(time.vtbl);
    claim_assert_nonnull(time.vtbl->freqInstFn);
    return time.vtbl->freqInstFn(time.ctx);
};

fn_((time_Inst_freqInv(time_Self time))(f64)) {
    claim_assert_nonnull(time.ctx);
    claim_assert_nonnull(time.vtbl);
    claim_assert_nonnull(time.vtbl->freqInstInvFn);
    return time.vtbl->freqInstInvFn(time.ctx);
};

fn_((time_Inst_offset(time_Self time))(time_Inst)) {
    claim_assert_nonnull(time.ctx);
    claim_assert_nonnull(time.vtbl);
    claim_assert_nonnull(time.vtbl->offsetInstFn);
    return time.vtbl->offsetInstFn(time.ctx);
};

fn_((time_Inst_ticks(time_Inst self))(u64)) {
#if plat_is_windows
    return as$(u64)(self.impl.QuadPart);
#elif plat_based_unix
    return as$(u64)(self.impl.tv_sec) * time_Inst_nanos_per_sec + as$(u64)(self.impl.tv_nsec);
#else
    let_ignore = self;
    return 0;
#endif
};

/* --- Operations --- */

fn_((time_Inst_now(time_Self time))(time_Inst)) {
    claim_assert_nonnull(time.ctx);
    claim_assert_nonnull(time.vtbl);
    claim_assert_nonnull(time.vtbl->nowInstFn);
    return time.vtbl->nowInstFn(time.ctx);
};

fn_((time_Inst_elapsed(time_Inst self, time_Self time))(time_Dur)) {
    return time_Inst_durSince(time_Inst_now(time), self);
};

fn_((time_Inst_durSince(time_Inst later, time_Inst earlier))(time_Dur)) {
    return unwrap_(time_Inst_durSinceChkd(later, earlier));
};

fn_((time_Inst_durSinceChkd(time_Inst later, time_Inst earlier))(O$time_Dur) $scope) {
    if (cmp_lt$(time_Inst)(later, earlier)) {
        return_none();
    }
#if plat_is_windows
    let diff = later.impl.QuadPart - earlier.impl.QuadPart;
    let nanos = as$(u64)(as$(f64)(diff) * as$(f64)(time_Inst_nanos_per_sec)
                     / as$(f64)(time_Inst__windows_freqTicksPerSec()));
    return_some(time_Dur_fromNanos(nanos));
#elif plat_based_unix
    var secs = later.impl.tv_sec - earlier.impl.tv_sec;
    var nanos = later.impl.tv_nsec - earlier.impl.tv_nsec;
    if (nanos < 0) {
        secs -= 1;
        nanos += as$(long)(time_Inst_nanos_per_sec);
    }
    return_some(time_Dur_from(as$(u64)(secs), as$(u32)(nanos)));
#else
    let_ignore = later;
    let_ignore = earlier;
    return_none();
#endif
} $unscoped(fn);

/* --- Arithmetic Operations --- */

op_fn_addWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst)) {
    return unwrap_(time_Inst_addChkdDur(lhs, rhs));
};

op_fn_addAsgWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst*)) {
    return *lhs = unwrap_(time_Inst_addChkdDur(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst)) {
    return unwrap_(time_Inst_subChkdDur(lhs, rhs));
};

op_fn_subAsgWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst*)) {
    return *lhs = unwrap_(time_Inst_subChkdDur(*lhs, rhs)), lhs;
};

fn_((time_Inst_addChkdDur(time_Inst lhs, time_Dur rhs))(O$time_Inst) $scope) {
#if plat_is_windows
    let ticks = orelse_((time_Inst__windows_durToTicks(rhs))(return_none()));
    if (0 <= lhs.impl.QuadPart && ticks <= (u64_limit_max - as$(u64)(lhs.impl.QuadPart))) {
        return_some({ .impl.QuadPart = as$(LONGLONG)(as$(u64)(lhs.impl.QuadPart) + ticks) });
    }
    return_none();
#elif plat_based_unix
    if (lhs.impl.tv_sec < 0) { return_none(); }
    let nanos_sum = as$(u64)(lhs.impl.tv_nsec) + as$(u64)(rhs.nanos);
    let carry = nanos_sum / as$(u64)(time_Inst_nanos_per_sec);
    let nanos = as$(long)(nanos_sum % as$(u64)(time_Inst_nanos_per_sec));
    if_some((u64_addChkd(as$(u64)(lhs.impl.tv_sec), rhs.secs))(secs_without_carry)) {
        if_some((u64_addChkd(secs_without_carry, carry))(secs)) {
            return_some({ .impl = { .tv_sec = as$(time_t)(secs), .tv_nsec = nanos } });
        }
    }
    return_none();
#else
    let_ignore = lhs;
    let_ignore = rhs;
    return_none();
#endif
} $unscoped(fn);

fn_((time_Inst_subChkdDur(time_Inst lhs, time_Dur rhs))(O$time_Inst) $scope) {
#if plat_is_windows
    let ticks = orelse_((time_Inst__windows_durToTicks(rhs))(return_none()));
    if (0 <= lhs.impl.QuadPart && ticks <= as$(u64)(lhs.impl.QuadPart)) {
        return_some({ .impl.QuadPart = as$(LONGLONG)(as$(u64)(lhs.impl.QuadPart) - ticks) });
    }
    return_none();
#elif plat_based_unix
    if (lhs.impl.tv_sec < 0) { return_none(); }
    let rhs_secs = as$(time_t)(rhs.secs);
    var secs = lhs.impl.tv_sec - rhs_secs;
    var nanos = lhs.impl.tv_nsec - as$(long)(rhs.nanos);
    if (nanos < 0) {
        secs -= 1;
        nanos += as$(long)(time_Inst_nanos_per_sec);
    }
    if (secs < 0) { return_none(); }
    return_some({ .impl = { .tv_sec = secs, .tv_nsec = nanos } });
#else
    let_ignore = lhs;
    let_ignore = rhs;
    return_none();
#endif
} $unscoped(fn);

/* --- Comparison Operations --- */

cmp_fn_ord$((time_Inst)(lhs, rhs)) {
#if plat_is_windows
    if (lhs.impl.QuadPart < rhs.impl.QuadPart) { return cmp_Ord_lt; }
    if (lhs.impl.QuadPart > rhs.impl.QuadPart) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
#elif plat_based_unix
    if (lhs.impl.tv_sec < rhs.impl.tv_sec) { return cmp_Ord_lt; }
    if (lhs.impl.tv_sec > rhs.impl.tv_sec) { return cmp_Ord_gt; }
    if (lhs.impl.tv_nsec < rhs.impl.tv_nsec) { return cmp_Ord_lt; }
    if (lhs.impl.tv_nsec > rhs.impl.tv_nsec) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
#else
    let_ignore = lhs;
    let_ignore = rhs;
    return cmp_Ord_eq;
#endif
};
cmp_fn_eq_default$((time_Inst)(lhs, rhs));
cmp_fn_ne_default$((time_Inst)(lhs, rhs));
cmp_fn_lt_default$((time_Inst)(lhs, rhs));
cmp_fn_gt_default$((time_Inst)(lhs, rhs));
cmp_fn_le_default$((time_Inst)(lhs, rhs));
cmp_fn_ge_default$((time_Inst)(lhs, rhs));
cmp_fn_ordCtx$((time_Inst)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_ord$(time_Inst)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Inst)(lhs, rhs, ctx));

cmp_fn_eql$((time_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Inst)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_Inst)(lhs, rhs));
cmp_fn_eqlCtx$((time_Inst)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_eql$(time_Inst)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_Inst)(lhs, rhs, ctx));

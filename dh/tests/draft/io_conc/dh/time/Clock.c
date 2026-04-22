#include "Clock.h"

#if plat_based_unix
#include <time.h>
#endif /* plat_based_unix */

/*========== Internal Definitions ===========================================*/

#if plat_is_windows
$attr($inline_always)
$static fn_((time_Clock__windows_filetimeToU64(FILETIME ft))(u64)) {
    return (as$(u64)(ft.dwHighDateTime) << 32ull) | as$(u64)(ft.dwLowDateTime);
};

$attr($inline_always)
$static fn_((time_Clock__windows_u64ToFiletime(u64 val))(FILETIME)) {
    return (FILETIME){
        .dwLowDateTime = as$(DWORD)(val & 0xFFFFFFFFull),
        .dwHighDateTime = as$(DWORD)(val >> 32ull),
    };
};
#endif /* plat_is_windows */

/*========== External Definitions ===========================================*/

/* --- Operations --- */

fn_((time_Clock_now(time_Self time))(time_Clock)) {
    claim_assert_nonnull(time.ctx);
    claim_assert_nonnull(time.vtbl);
    claim_assert_nonnull(time.vtbl->nowClockFn);
    return time.vtbl->nowClockFn(time.ctx);
};

fn_((time_Clock_elapsed(time_Clock self, time_Self time))(time_Dur)) {
    return time_Clock_durSince(time_Clock_now(time), self);
};

fn_((time_Clock_durSince(time_Clock later, time_Clock earlier))(time_Dur)) {
    return unwrap_(time_Clock_durSinceChkd(later, earlier));
};

fn_((time_Clock_durSinceChkd(time_Clock later, time_Clock earlier))(O$time_Dur) $scope) {
    if (cmp_lt$(time_Clock)(later, earlier)) {
        return_none();
    }
#if plat_is_windows
    let later_val = time_Clock__windows_filetimeToU64(later.impl);
    let earlier_val = time_Clock__windows_filetimeToU64(earlier.impl);
    let diff_intervals = later_val - earlier_val;
    if_some((u64_mulChkd(diff_intervals, 100ull))(nanos)) {
        return_some(time_Dur_fromNanos(nanos));
    }
    return_none();
#elif plat_based_unix
    var secs = later.impl.tv_sec - earlier.impl.tv_sec;
    var nanos = later.impl.tv_nsec - earlier.impl.tv_nsec;
    if (nanos < 0) {
        secs -= 1;
        nanos += as$(long)(time_nanos_per_sec);
    }
    return_some(time_Dur_from(as$(u64)(secs), as$(u32)(nanos)));
#else
    let_ignore = later;
    let_ignore = earlier;
    return_none();
#endif
} $unscoped(fn);

/* --- Arithmetic Operations --- */

op_fn_addWith$(((time_Clock, time_Dur)(lhs, rhs))(time_Clock)) {
    return unwrap_(time_Clock_addChkdDur(lhs, rhs));
};

op_fn_addAsgWith$(((time_Clock, time_Dur)(lhs, rhs))(time_Clock*)) {
    return *lhs = unwrap_(time_Clock_addChkdDur(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_Clock, time_Dur)(lhs, rhs))(time_Clock)) {
    return unwrap_(time_Clock_subChkdDur(lhs, rhs));
};

op_fn_subAsgWith$(((time_Clock, time_Dur)(lhs, rhs))(time_Clock*)) {
    return *lhs = unwrap_(time_Clock_subChkdDur(*lhs, rhs)), lhs;
};

fn_((time_Clock_addChkdDur(time_Clock lhs, time_Dur rhs))(O$time_Clock) $scope) {
#if plat_is_windows
    let lhs_val = time_Clock__windows_filetimeToU64(lhs.impl);
    if_some((u64_mulChkd(rhs.secs, time_Clock_intervals_per_sec))(secs_intervals)) {
        if_some((u64_addChkd(secs_intervals, rhs.nanos / 100ull))(intervals)) {
            if (intervals <= (u64_limit_max - lhs_val)) {
                return_some({ .impl = time_Clock__windows_u64ToFiletime(lhs_val + intervals) });
            }
        }
    }
    return_none();
#elif plat_based_unix
    var result = lhs.impl;
    result.tv_sec += as$(time_t)(rhs.secs);
    result.tv_nsec += as$(long)(rhs.nanos);
    if (result.tv_nsec >= as$(long)(time_nanos_per_sec)) {
        result.tv_sec += 1;
        result.tv_nsec -= as$(long)(time_nanos_per_sec);
    }
    return_some({ .impl = result });
#else
    let_ignore = lhs;
    let_ignore = rhs;
    return_none();
#endif
} $unscoped(fn);

fn_((time_Clock_subChkdDur(time_Clock lhs, time_Dur rhs))(O$time_Clock) $scope) {
#if plat_is_windows
    let lhs_val = time_Clock__windows_filetimeToU64(lhs.impl);
    if_some((u64_mulChkd(rhs.secs, time_Clock_intervals_per_sec))(secs_intervals)) {
        if_some((u64_addChkd(secs_intervals, rhs.nanos / 100ull))(intervals)) {
            if (intervals <= lhs_val) {
                return_some({ .impl = time_Clock__windows_u64ToFiletime(lhs_val - intervals) });
            }
        }
    }
    return_none();
#elif plat_based_unix
    var result = lhs.impl;
    result.tv_sec -= as$(time_t)(rhs.secs);
    result.tv_nsec -= as$(long)(rhs.nanos);
    if (result.tv_nsec < 0) {
        result.tv_sec -= 1;
        result.tv_nsec += as$(long)(time_nanos_per_sec);
    }
    if (result.tv_sec < 0) {
        return_none();
    }
    return_some({ .impl = result });
#else
    let_ignore = lhs;
    let_ignore = rhs;
    return_none();
#endif
} $unscoped(fn);

/* --- Unix Epoch Conversion --- */

fn_((time_Clock_fromUnixEpoch(u64 secs))(time_Clock)) {
#if plat_is_windows
    let intervals = time_Clock_intervals_to_unix_epoch + (secs * time_Clock_intervals_per_sec);
    return (time_Clock){ .impl = time_Clock__windows_u64ToFiletime(intervals) };
#elif plat_based_unix
    return (time_Clock){
        .impl = {
            .tv_sec = as$(time_t)(secs),
            .tv_nsec = 0,
        },
    };
#else
    let_ignore = secs;
    return (time_Clock){ .impl = cleared() };
#endif
};

fn_((time_Clock_toUnixEpoch(time_Clock self))(u64)) {
#if plat_is_windows
    let val = time_Clock__windows_filetimeToU64(self.impl);
    debug_assert_fmt(val >= time_Clock_intervals_to_unix_epoch, "Clock is before Unix epoch");
    let intervals_since_epoch = val - time_Clock_intervals_to_unix_epoch;
    return intervals_since_epoch / time_Clock_intervals_per_sec;
#elif plat_based_unix
    debug_assert_fmt(self.impl.tv_sec >= 0, "Clock is before Unix epoch");
    return as$(u64)(self.impl.tv_sec);
#else
    let_ignore = self;
    return 0;
#endif
};

fn_((time_Clock_durSinceUnixEpoch(time_Clock self))(time_Dur)) {
    return time_Clock_durSince(self, time_Clock_unix_epoch);
};

/* --- Comparison Operations --- */

cmp_fn_ord$((time_Clock)(lhs, rhs)) {
#if plat_is_windows
    let lhs_val = time_Clock__windows_filetimeToU64(lhs.impl);
    let rhs_val = time_Clock__windows_filetimeToU64(rhs.impl);
    if (lhs_val < rhs_val) { return cmp_Ord_lt; }
    if (lhs_val > rhs_val) { return cmp_Ord_gt; }
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
cmp_fn_eq_default$((time_Clock)(lhs, rhs));
cmp_fn_ne_default$((time_Clock)(lhs, rhs));
cmp_fn_lt_default$((time_Clock)(lhs, rhs));
cmp_fn_gt_default$((time_Clock)(lhs, rhs));
cmp_fn_le_default$((time_Clock)(lhs, rhs));
cmp_fn_ge_default$((time_Clock)(lhs, rhs));
cmp_fn_ordCtx$((time_Clock)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_ord$(time_Clock)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_Clock)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Clock)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Clock)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Clock)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Clock)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Clock)(lhs, rhs, ctx));

cmp_fn_eql$((time_Clock)(lhs, rhs)) {
    return cmp_ord$(time_Clock)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_Clock)(lhs, rhs));
cmp_fn_eqlCtx$((time_Clock)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_eql$(time_Clock)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_Clock)(lhs, rhs, ctx));

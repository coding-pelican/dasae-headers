#include "dh/time/SysTime.h"
#if plat_is_posix
#include "dh/sys/posix.h"
#endif

/*========== Internal Declarations ==========================================*/

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $maybe_unused)
    $static fn_((time_SysTime__unsupported_now(void))(time_SysTime));
    $attr($inline_always $maybe_unused)
    $static fn_((time_SysTime__unsupported_durSinceChkd(time_SysTime later, time_SysTime earlier))(O$time_Dur));
    $attr($inline_always $maybe_unused)
    $static fn_((time_SysTime__unsupported_addChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime));
    $attr($inline_always $maybe_unused)
    $static fn_((time_SysTime__unsupported_subChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime));
    $attr($inline_always $maybe_unused)
    $static fn_((time_SysTime__unsupported_fromUnixEpoch(u64 secs))(time_SysTime));
    $attr($inline_always $maybe_unused)
    $static fn_((time_SysTime__unsupported_toUnixEpoch(time_SysTime self))(u64));
    $attr($inline_always $maybe_unused)
    $static fn_((time_SysTime__unsupported_ord(time_SysTime lhs, time_SysTime rhs))(cmp_Ord));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always)
    $static fn_((time_SysTime__windows_now(void))(time_SysTime));
    $attr($inline_always)
    $static fn_((time_SysTime__windows_durSinceChkd(time_SysTime later, time_SysTime earlier))(O$time_Dur));
    $attr($inline_always)
    $static fn_((time_SysTime__windows_addChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime));
    $attr($inline_always)
    $static fn_((time_SysTime__windows_subChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime));
    $attr($inline_always)
    $static fn_((time_SysTime__windows_fromUnixEpoch(u64 secs))(time_SysTime));
    $attr($inline_always)
    $static fn_((time_SysTime__windows_toUnixEpoch(time_SysTime self))(u64));
    $attr($inline_always)
    $static fn_((time_SysTime__windows_ord(time_SysTime lhs, time_SysTime rhs))(cmp_Ord));
));
pp_if_(plat_is_posix)(pp_then_(
    $attr($inline_always)
    $static fn_((time_SysTime__unix_now(void))(time_SysTime));
    $attr($inline_always)
    $static fn_((time_SysTime__unix_durSinceChkd(time_SysTime later, time_SysTime earlier))(O$time_Dur));
    $attr($inline_always)
    $static fn_((time_SysTime__unix_addChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime));
    $attr($inline_always)
    $static fn_((time_SysTime__unix_subChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime));
    $attr($inline_always)
    $static fn_((time_SysTime__unix_fromUnixEpoch(u64 secs))(time_SysTime));
    $attr($inline_always)
    $static fn_((time_SysTime__unix_toUnixEpoch(time_SysTime self))(u64));
    $attr($inline_always)
    $static fn_((time_SysTime__unix_ord(time_SysTime lhs, time_SysTime rhs))(cmp_Ord));
));

$static let time_SysTime__now = pp_if_(plat_is_windows)(
    pp_then_(time_SysTime__windows_now),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(time_SysTime__unix_now),
        pp_else_(time_SysTime__unsupported_now)
    )));
$static let time_SysTime__durSinceChkd = pp_if_(plat_is_windows)(
    pp_then_(time_SysTime__windows_durSinceChkd),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(time_SysTime__unix_durSinceChkd),
        pp_else_(time_SysTime__unsupported_durSinceChkd)
    )));
$static let time_SysTime__addChkdDur = pp_if_(plat_is_windows)(
    pp_then_(time_SysTime__windows_addChkdDur),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(time_SysTime__unix_addChkdDur),
        pp_else_(time_SysTime__unsupported_addChkdDur)
    )));
$static let time_SysTime__subChkdDur = pp_if_(plat_is_windows)(
    pp_then_(time_SysTime__windows_subChkdDur),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(time_SysTime__unix_subChkdDur),
        pp_else_(time_SysTime__unsupported_subChkdDur)
    )));
$static let time_SysTime__fromUnixEpoch = pp_if_(plat_is_windows)(
    pp_then_(time_SysTime__windows_fromUnixEpoch),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(time_SysTime__unix_fromUnixEpoch),
        pp_else_(time_SysTime__unsupported_fromUnixEpoch)
    )));
$static let time_SysTime__toUnixEpoch = pp_if_(plat_is_windows)(
    pp_then_(time_SysTime__windows_toUnixEpoch),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(time_SysTime__unix_toUnixEpoch),
        pp_else_(time_SysTime__unsupported_toUnixEpoch)
    )));
$static let time_SysTime__ord = pp_if_(plat_is_windows)(
    pp_then_(time_SysTime__windows_ord),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(time_SysTime__unix_ord),
        pp_else_(time_SysTime__unsupported_ord)
    )));

/*========== External Definitions ===========================================*/

/* --- Operations --- */

fn_((time_SysTime_now(void))(time_SysTime)) {
    return time_SysTime__now();
};

fn_((time_SysTime_elapsed(time_SysTime self))(time_Dur)) {
    return time_SysTime_durSince(time_SysTime_now(), self);
};

fn_((time_SysTime_durSince(time_SysTime later, time_SysTime earlier))(time_Dur)) {
    return unwrap_(time_SysTime_durSinceChkd(later, earlier));
};

fn_((time_SysTime_durSinceChkd(time_SysTime later, time_SysTime earlier))(O$time_Dur)) {
    return time_SysTime__durSinceChkd(later, earlier);
};

/* --- Arithmetic Operations --- */

op_fn_addWith$(((time_SysTime, time_Dur)(lhs, rhs))(time_SysTime)) {
    return unwrap_(time_SysTime_addChkdDur(lhs, rhs));
};

op_fn_addAsgWith$(((time_SysTime, time_Dur)(lhs, rhs))(time_SysTime*)) {
    return *lhs = unwrap_(time_SysTime_addChkdDur(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_SysTime, time_Dur)(lhs, rhs))(time_SysTime)) {
    return unwrap_(time_SysTime_subChkdDur(lhs, rhs));
};

op_fn_subAsgWith$(((time_SysTime, time_Dur)(lhs, rhs))(time_SysTime*)) {
    return *lhs = unwrap_(time_SysTime_subChkdDur(*lhs, rhs)), lhs;
};

fn_((time_SysTime_addChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime)) {
    return time_SysTime__addChkdDur(lhs, rhs);
};

fn_((time_SysTime_subChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime)) {
    return time_SysTime__subChkdDur(lhs, rhs);
};

/* --- Unix Epoch Conversion --- */

fn_((time_SysTime_fromUnixEpoch(u64 secs))(time_SysTime)) {
    return time_SysTime__fromUnixEpoch(secs);
};

fn_((time_SysTime_toUnixEpoch(time_SysTime self))(u64)) {
    return time_SysTime__toUnixEpoch(self);
};

fn_((time_SysTime_durSinceUnixEpoch(time_SysTime self))(time_Dur)) {
    return time_SysTime_durSince(self, time_SysTime_unix_epoch);
};

/* --- Comparison Operations --- */

cmp_fn_ord$((time_SysTime)(lhs, rhs)) {
    return time_SysTime__ord(lhs, rhs);
};
cmp_fn_eq_default$((time_SysTime)(lhs, rhs));
cmp_fn_ne_default$((time_SysTime)(lhs, rhs));
cmp_fn_lt_default$((time_SysTime)(lhs, rhs));
cmp_fn_gt_default$((time_SysTime)(lhs, rhs));
cmp_fn_le_default$((time_SysTime)(lhs, rhs));
cmp_fn_ge_default$((time_SysTime)(lhs, rhs));
cmp_fn_ordCtx$((time_SysTime)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_ord$(time_SysTime)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_SysTime)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_SysTime)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_SysTime)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_SysTime)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_SysTime)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_SysTime)(lhs, rhs, ctx));

cmp_fn_eql$((time_SysTime)(lhs, rhs)) {
    return cmp_ord$(time_SysTime)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_SysTime)(lhs, rhs));
cmp_fn_eqlCtx$((time_SysTime)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_eql$(time_SysTime)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_SysTime)(lhs, rhs, ctx));

/*========== Internal Definitions ===========================================*/

/* --- Unsupported --- */

fn_((time_SysTime__unsupported_now(void))(time_SysTime)) {
    return (time_SysTime){ .impl = cleared() };
};

fn_((time_SysTime__unsupported_durSinceChkd(time_SysTime later, time_SysTime earlier))(O$time_Dur) $scope) {
    let_ignore = later;
    let_ignore = earlier;
    return_none();
} $unscoped(fn);

fn_((time_SysTime__unsupported_addChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime) $scope) {
    let_ignore = lhs;
    let_ignore = rhs;
    return_none();
} $unscoped(fn);

fn_((time_SysTime__unsupported_subChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime) $scope) {
    let_ignore = lhs;
    let_ignore = rhs;
    return_none();
} $unscoped(fn);

fn_((time_SysTime__unsupported_fromUnixEpoch(u64 secs))(time_SysTime)) {
    let_ignore = secs;
    return (time_SysTime){ .impl = cleared() };
};

fn_((time_SysTime__unsupported_toUnixEpoch(time_SysTime self))(u64)) {
    let_ignore = self;
    return 0;
};

fn_((time_SysTime__unsupported_ord(time_SysTime lhs, time_SysTime rhs))(cmp_Ord)) {
    let_ignore = lhs;
    let_ignore = rhs;
    return cmp_Ord_eq;
};

/* --- Windows --- */

#if plat_is_windows
/// Convert FILETIME to 64-bit value (100-nanosecond intervals since 1601).
$attr($inline_always)
$static fn_((time_SysTime__windows_filetimeToU64(FILETIME ft))(u64)) {
    return (as$(u64)(ft.dwHighDateTime) << 32ull) | as$(u64)(ft.dwLowDateTime);
};

$attr($inline_always)
$static fn_((time_SysTime__windows_u64ToFiletime(u64 val))(FILETIME)) {
    return (FILETIME){
        .dwLowDateTime = as$(DWORD)(val & 0xFFFFFFFFull),
        .dwHighDateTime = as$(DWORD)(val >> 32ull),
    };
};

fn_((time_SysTime__windows_now(void))(time_SysTime)) {
    var ft = l0$((FILETIME));
    GetSystemTimeAsFileTime(&ft);
    return (time_SysTime){ .impl = ft };
};

fn_((time_SysTime__windows_durSinceChkd(time_SysTime later, time_SysTime earlier))(O$time_Dur) $scope) {
    if (time_SysTime__windows_ord(later, earlier) == cmp_Ord_lt) {
        return_none();
    }
    let later_val = time_SysTime__windows_filetimeToU64(later.impl);
    let earlier_val = time_SysTime__windows_filetimeToU64(earlier.impl);
    let diff_intervals = later_val - earlier_val;
    let nanos = diff_intervals * 100ull;
    return_some(time_Dur_fromNanos(nanos));
} $unscoped(fn);

fn_((time_SysTime__windows_addChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime) $scope) {
    let lhs_val = time_SysTime__windows_filetimeToU64(lhs.impl);
    let intervals = (rhs.secs * time_SysTime_intervals_per_sec) + (rhs.nanos / 100ull);
    if (intervals > (u64_limit_max - lhs_val)) {
        return_none();
    }
    return_some({ .impl = time_SysTime__windows_u64ToFiletime(lhs_val + intervals) });
} $unscoped(fn);

fn_((time_SysTime__windows_subChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime) $scope) {
    let lhs_val = time_SysTime__windows_filetimeToU64(lhs.impl);
    let intervals = (rhs.secs * time_SysTime_intervals_per_sec) + (rhs.nanos / 100ull);
    if (intervals > lhs_val) {
        return_none();
    }
    return_some({ .impl = time_SysTime__windows_u64ToFiletime(lhs_val - intervals) });
} $unscoped(fn);

fn_((time_SysTime__windows_fromUnixEpoch(u64 secs))(time_SysTime)) {
    let intervals = time_SysTime_intervals_to_unix_epoch + (secs * time_SysTime_intervals_per_sec);
    return (time_SysTime){ .impl = time_SysTime__windows_u64ToFiletime(intervals) };
};

fn_((time_SysTime__windows_toUnixEpoch(time_SysTime self))(u64)) {
    let val = time_SysTime__windows_filetimeToU64(self.impl);
    debug_assert_fmt(val >= time_SysTime_intervals_to_unix_epoch, "SysTime is before Unix epoch");
    let intervals_since_epoch = val - time_SysTime_intervals_to_unix_epoch;
    return intervals_since_epoch / time_SysTime_intervals_per_sec;
};

fn_((time_SysTime__windows_ord(time_SysTime lhs, time_SysTime rhs))(cmp_Ord)) {
    let lhs_val = time_SysTime__windows_filetimeToU64(lhs.impl);
    let rhs_val = time_SysTime__windows_filetimeToU64(rhs.impl);
    if (lhs_val < rhs_val) { return cmp_Ord_lt; }
    if (lhs_val > rhs_val) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
};
#endif /* plat_is_windows */

/* --- POSIX --- */

#if plat_is_posix
fn_((time_SysTime__unix_now(void))(time_SysTime)) {
    var ts = l0$((time_SysTimePlatform));
    let_ignore = sys_posix_clock_gettime(sys_posix_CLOCK_REALTIME, &ts);
    return (time_SysTime){ .impl = ts };
};

fn_((time_SysTime__unix_durSinceChkd(time_SysTime later, time_SysTime earlier))(O$time_Dur) $scope) {
    if (time_SysTime__unix_ord(later, earlier) == cmp_Ord_lt) {
        return_none();
    }
    var diff_sec = later.impl.tv_sec - earlier.impl.tv_sec;
    var diff_nsec = later.impl.tv_nsec - earlier.impl.tv_nsec;
    if (diff_nsec < 0) {
        diff_sec--;
        diff_nsec += time_nanos_per_sec;
    }
    let nanos = as$(u64)(diff_sec)*time_nanos_per_sec + as$(u64)(diff_nsec);
    return_some(time_Dur_fromNanos(nanos));
} $unscoped(fn);

fn_((time_SysTime__unix_addChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime) $scope) {
    var result = lhs.impl;
    result.tv_sec += as$(TypeOf(result.tv_sec))(rhs.secs);
    result.tv_nsec += as$(TypeOf(result.tv_nsec))(rhs.nanos);
    if (result.tv_nsec >= as$(TypeOf(result.tv_nsec))(time_nanos_per_sec)) {
        result.tv_sec++;
        result.tv_nsec -= as$(TypeOf(result.tv_nsec))(time_nanos_per_sec);
    }
    return_some({ .impl = result });
} $unscoped(fn);

fn_((time_SysTime__unix_subChkdDur(time_SysTime lhs, time_Dur rhs))(O$time_SysTime) $scope) {
    var result = lhs.impl;
    result.tv_sec -= as$(TypeOf(result.tv_sec))(rhs.secs);
    result.tv_nsec -= as$(TypeOf(result.tv_nsec))(rhs.nanos);
    if (result.tv_nsec < 0) {
        result.tv_sec--;
        result.tv_nsec += as$(TypeOf(result.tv_nsec))(time_nanos_per_sec);
    }
    if (result.tv_sec < 0) {
        return_none();
    }
    return_some({ .impl = result });
} $unscoped(fn);

fn_((time_SysTime__unix_fromUnixEpoch(u64 secs))(time_SysTime)) {
    var impl = l0$((time_SysTimePlatform));
    impl.tv_sec = as$(TypeOf(impl.tv_sec))(secs);
    return (time_SysTime){ .impl = impl };
};

fn_((time_SysTime__unix_toUnixEpoch(time_SysTime self))(u64)) {
    debug_assert_fmt(self.impl.tv_sec >= 0, "SysTime is before Unix epoch");
    return as$(u64)(self.impl.tv_sec);
};

fn_((time_SysTime__unix_ord(time_SysTime lhs, time_SysTime rhs))(cmp_Ord)) {
    if (lhs.impl.tv_sec < rhs.impl.tv_sec) { return cmp_Ord_lt; }
    if (lhs.impl.tv_sec > rhs.impl.tv_sec) { return cmp_Ord_gt; }
    if (lhs.impl.tv_nsec < rhs.impl.tv_nsec) { return cmp_Ord_lt; }
    if (lhs.impl.tv_nsec > rhs.impl.tv_nsec) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
};
#endif /* plat_is_posix */

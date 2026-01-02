#include "dh/time/Instant.h"
#include "dh/time/Duration.h"

/*========== Internal Declarations ==========================================*/

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $maybe_unused)
    $static fn_((time_Instant__unsupported_freq(void))(time_Instant));
    $attr($inline_always $maybe_unused)
    $static fn_((time_Instant__unsupported_freqInv(void))(f64));
    $attr($inline_always $maybe_unused)
    $static fn_((time_Instant__unsupported_offset(void))(time_Instant));
    $attr($inline_always $maybe_unused)
    $static fn_((time_Instant__unsupported_now(void))(time_Instant));
    $attr($inline_always $maybe_unused)
    $static fn_((time_Instant__unsupported_ticks(time_Instant self))(u64));
    $attr($inline_always $maybe_unused)
    $static fn_((time_Instant__unsupported_durationSinceChkd(time_Instant later, time_Instant earlier))(O$time_Duration));
    $attr($inline_always $maybe_unused)
    $static fn_((time_Instant__unsupported_addChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant));
    $attr($inline_always $maybe_unused)
    $static fn_((time_Instant__unsupported_subChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant));
    $attr($inline_always $maybe_unused)
    $static fn_((time_Instant__unsupported_ord(time_Instant lhs, time_Instant rhs))(cmp_Ord));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always)
    $static fn_((time_Instant__windows_freq(void))(time_Instant));
    $attr($inline_always)
    $static fn_((time_Instant__windows_freqInv(void))(f64));
    $attr($inline_always)
    $static fn_((time_Instant__windows_offset(void))(time_Instant));
    $attr($inline_always)
    $static fn_((time_Instant__windows_now(void))(time_Instant));
    $attr($inline_always)
    $static fn_((time_Instant__windows_ticks(time_Instant self))(u64));
    $attr($inline_always)
    $static fn_((time_Instant__windows_durationSinceChkd(time_Instant later, time_Instant earlier))(O$time_Duration));
    $attr($inline_always)
    $static fn_((time_Instant__windows_addChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant));
    $attr($inline_always)
    $static fn_((time_Instant__windows_subChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant));
    $attr($inline_always)
    $static fn_((time_Instant__windows_ord(time_Instant lhs, time_Instant rhs))(cmp_Ord));
));
pp_if_(pp_or(plat_is_linux, plat_is_darwin))(pp_then_(
    $attr($inline_always)
    $static fn_((time_Instant__unix_freq(void))(time_Instant));
    $attr($inline_always)
    $static fn_((time_Instant__unix_freqInv(void))(f64));
    $attr($inline_always)
    $static fn_((time_Instant__unix_offset(void))(time_Instant));
    $attr($inline_always)
    $static fn_((time_Instant__unix_now(void))(time_Instant));
    $attr($inline_always)
    $static fn_((time_Instant__unix_ticks(time_Instant self))(u64));
    $attr($inline_always)
    $static fn_((time_Instant__unix_durationSinceChkd(time_Instant later, time_Instant earlier))(O$time_Duration));
    $attr($inline_always)
    $static fn_((time_Instant__unix_addChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant));
    $attr($inline_always)
    $static fn_((time_Instant__unix_subChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant));
    $attr($inline_always)
    $static fn_((time_Instant__unix_ord(time_Instant lhs, time_Instant rhs))(cmp_Ord));
));

$static let time_Instant__freq = pp_if_(plat_is_windows)(
    pp_then_(time_Instant__windows_freq),
    pp_else_(pp_if_(pp_or(plat_is_linux, plat_is_darwin))(
        pp_then_(time_Instant__unix_freq),
        pp_else_(time_Instant__unsupported_freq)
    )));
$static let time_Instant__freqInv = pp_if_(plat_is_windows)(
    pp_then_(time_Instant__windows_freqInv),
    pp_else_(pp_if_(pp_or(plat_is_linux, plat_is_darwin))(
        pp_then_(time_Instant__unix_freqInv),
        pp_else_(time_Instant__unsupported_freqInv)
    )));
$static let time_Instant__offset = pp_if_(plat_is_windows)(
    pp_then_(time_Instant__windows_offset),
    pp_else_(pp_if_(pp_or(plat_is_linux, plat_is_darwin))(
        pp_then_(time_Instant__unix_offset),
        pp_else_(time_Instant__unsupported_offset)
    )));
$static let time_Instant__now = pp_if_(plat_is_windows)(
    pp_then_(time_Instant__windows_now),
    pp_else_(pp_if_(pp_or(plat_is_linux, plat_is_darwin))(
        pp_then_(time_Instant__unix_now),
        pp_else_(time_Instant__unsupported_now)
    )));
$static let time_Instant__ticks = pp_if_(plat_is_windows)(
    pp_then_(time_Instant__windows_ticks),
    pp_else_(pp_if_(pp_or(plat_is_linux, plat_is_darwin))(
        pp_then_(time_Instant__unix_ticks),
        pp_else_(time_Instant__unsupported_ticks)
    )));
$static let time_Instant__durationSinceChkd = pp_if_(plat_is_windows)(
    pp_then_(time_Instant__windows_durationSinceChkd),
    pp_else_(pp_if_(pp_or(plat_is_linux, plat_is_darwin))(
        pp_then_(time_Instant__unix_durationSinceChkd),
        pp_else_(time_Instant__unsupported_durationSinceChkd)
    )));
$static let time_Instant__addChkdDuration = pp_if_(plat_is_windows)(
    pp_then_(time_Instant__windows_addChkdDuration),
    pp_else_(pp_if_(pp_or(plat_is_linux, plat_is_darwin))(
        pp_then_(time_Instant__unix_addChkdDuration),
        pp_else_(time_Instant__unsupported_addChkdDuration)
    )));
$static let time_Instant__subChkdDuration = pp_if_(plat_is_windows)(
    pp_then_(time_Instant__windows_subChkdDuration),
    pp_else_(pp_if_(pp_or(plat_is_linux, plat_is_darwin))(
        pp_then_(time_Instant__unix_subChkdDuration),
        pp_else_(time_Instant__unsupported_subChkdDuration)
    )));
$static let time_Instant__ord = pp_if_(plat_is_windows)(
    pp_then_(time_Instant__windows_ord),
    pp_else_(pp_if_(pp_or(plat_is_linux, plat_is_darwin))(
        pp_then_(time_Instant__unix_ord),
        pp_else_(time_Instant__unsupported_ord)
    )));

/*========== External Definitions ===========================================*/

/* --- Accessors --- */

fn_((time_Instant_freq(void))(time_Instant)) { return time_Instant__freq(); };
fn_((time_Instant_freqInv(void))(f64)) { return time_Instant__freqInv(); };
fn_((time_Instant_offset(void))(time_Instant)) { return time_Instant__offset(); };
fn_((time_Instant_ticks(time_Instant self))(u64)) { return time_Instant__ticks(self); };

/* --- Operations --- */

fn_((time_Instant_now(void))(time_Instant)) {
    return time_Instant__now();
};

fn_((time_Instant_elapsed(time_Instant self))(time_Duration)) {
    return time_Instant_durationSince(time_Instant_now(), self);
};

fn_((time_Instant_durationSince(time_Instant later, time_Instant earlier))(time_Duration)) {
    return unwrap_(time_Instant_durationSinceChkd(later, earlier));
};

fn_((time_Instant_durationSinceChkd(time_Instant later, time_Instant earlier))(O$time_Duration)) {
    return time_Instant__durationSinceChkd(later, earlier);
};

/* --- Arithmetic Operations --- */

op_fn_addWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant)) {
    return unwrap_(time_Instant_addChkdDuration(lhs, rhs));
};

op_fn_addAsgWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant*)) {
    return *lhs = unwrap_(time_Instant_addChkdDuration(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant)) {
    return unwrap_(time_Instant_subChkdDuration(lhs, rhs));
};

op_fn_subAsgWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant*)) {
    return *lhs = unwrap_(time_Instant_subChkdDuration(*lhs, rhs)), lhs;
};

fn_((time_Instant_addChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant)) {
    return time_Instant__addChkdDuration(lhs, rhs);
};

fn_((time_Instant_subChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant)) {
    return time_Instant__subChkdDuration(lhs, rhs);
};

/* --- Comparison Operations --- */

cmp_fn_ord$((time_Instant)(lhs, rhs)) {
    return time_Instant__ord(lhs, rhs);
};
cmp_fn_eq_default$((time_Instant)(lhs, rhs));
cmp_fn_ne_default$((time_Instant)(lhs, rhs));
cmp_fn_lt_default$((time_Instant)(lhs, rhs));
cmp_fn_gt_default$((time_Instant)(lhs, rhs));
cmp_fn_le_default$((time_Instant)(lhs, rhs));
cmp_fn_ge_default$((time_Instant)(lhs, rhs));
cmp_fn_ordCtx$((time_Instant)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_ord$(time_Instant)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_Instant)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Instant)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Instant)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Instant)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Instant)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Instant)(lhs, rhs, ctx));

cmp_fn_eql$((time_Instant)(lhs, rhs)) {
    return cmp_ord$(time_Instant)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_Instant)(lhs, rhs));
cmp_fn_eqlCtx$((time_Instant)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_eql$(time_Instant)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_Instant)(lhs, rhs, ctx));

/*========== Internal Definitions ===========================================*/

/* --- Unsupported --- */

fn_((time_Instant__unsupported_freq(void))(time_Instant)) {
    return (time_Instant){ .impl = cleared() };
};

fn_((time_Instant__unsupported_freqInv(void))(f64)) {
    return f64_nan;
};

fn_((time_Instant__unsupported_offset(void))(time_Instant)) {
    return (time_Instant){ .impl = cleared() };
};

fn_((time_Instant__unsupported_now(void))(time_Instant)) {
    return (time_Instant){ .impl = cleared() };
};

fn_((time_Instant__unsupported_ticks(time_Instant self))(u64)) {
    let_ignore = self;
    return 0;
};

fn_((time_Instant__unsupported_durationSinceChkd(time_Instant later, time_Instant earlier))(O$time_Duration) $scope) {
    let_ignore = later;
    let_ignore = earlier;
    return_none();
} $unscoped_(fn);

fn_((time_Instant__unsupported_addChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant) $scope) {
    let_ignore = lhs;
    let_ignore = rhs;
    return_none();
} $unscoped_(fn);

fn_((time_Instant__unsupported_subChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant) $scope) {
    let_ignore = lhs;
    let_ignore = rhs;
    return_none();
} $unscoped_(fn);

fn_((time_Instant__unsupported_ord(time_Instant lhs, time_Instant rhs))(cmp_Ord)) {
    let_ignore = lhs;
    let_ignore = rhs;
    return cmp_Ord_eq;
};

/* --- Windows --- */

#if plat_is_windows
$static var_(s_windows_perf_freq, time_InstantPlatform) = cleared();
$static var_(s_windows_perf_freq_inv, f64) = f64_nan;
$static var_(s_windows_offset_value, time_InstantPlatform) = cleared();
$static var_(s_windows_initialized, bool) = false;

$attr($on_load)
$static fn_((time_Instant__windows_init(void))(void)) {
    if (s_windows_initialized) { return; }
    if (!QueryPerformanceFrequency(&s_windows_perf_freq)) {
        claim_unreachable_msg("Failed to query performance frequency");
    }
    s_windows_perf_freq_inv = 1.0 / as$(f64)(s_windows_perf_freq.QuadPart);
    QueryPerformanceCounter(&s_windows_offset_value);
    s_windows_initialized = true;
};

$attr($inline_always)
$static fn_((time_Instant__windows_ensureInit(void))(void)) {
    return time_Instant__windows_init(), claim_assert_fmt(s_windows_initialized, "time_Instant not initialized");
};

fn_((time_Instant__windows_freq(void))(time_Instant)) {
    return time_Instant__windows_ensureInit(), (time_Instant){ .impl = s_windows_perf_freq };
};

fn_((time_Instant__windows_freqInv(void))(f64)) {
    return time_Instant__windows_ensureInit(), s_windows_perf_freq_inv;
};

fn_((time_Instant__windows_offset(void))(time_Instant)) {
    return time_Instant__windows_ensureInit(), (time_Instant){ .impl = s_windows_offset_value };
};

fn_((time_Instant__windows_now(void))(time_Instant)) {
    time_Instant__windows_ensureInit();
    var current = lit0$((time_InstantPlatform));
    QueryPerformanceCounter(&current);
    return (time_Instant){ .impl = current };
};

fn_((time_Instant__windows_ticks(time_Instant self))(u64)) {
    return as$(u64)(self.impl.QuadPart);
};

fn_((time_Instant__windows_durationSinceChkd(time_Instant later, time_Instant earlier))(O$time_Duration) $scope) {
    if (time_Instant__windows_ord(later, earlier) == cmp_Ord_lt) {
        return_none();
    }
    let diff = as$(f64)(later.impl.QuadPart - earlier.impl.QuadPart);
    let nanos = as$(u64)(diff * time_Instant_nanos_per_sec * time_Instant__windows_freqInv());
    return_some(time_Duration_fromNanos(nanos));
} $unscoped_(fn);

fn_((time_Instant__windows_addChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant) $scope) {
    let ticks = (rhs.secs * time_Instant_intervals_per_sec) + (rhs.nanos / 100);
    if ((0 <= lhs.impl.QuadPart) && ticks <= (u64_limit_max - as$(u64)(lhs.impl.QuadPart))) {
        return_some({ .impl.QuadPart = as$(LONGLONG)(intCast$((u64)(lhs.impl.QuadPart)) + ticks) });
    }
    return_none();
} $unscoped_(fn);

fn_((time_Instant__windows_subChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant) $scope) {
    let ticks = (rhs.secs * time_Instant_intervals_per_sec) + (rhs.nanos / 100);
    if ((0 <= lhs.impl.QuadPart) && ticks <= (u64_limit_min + as$(u64)(lhs.impl.QuadPart))) {
        return_some({ .impl.QuadPart = as$(LONGLONG)(intCast$((u64)(lhs.impl.QuadPart)) - ticks) });
    }
    return_none();
} $unscoped_(fn);

fn_((time_Instant__windows_ord(time_Instant lhs, time_Instant rhs))(cmp_Ord)) {
    if (lhs.impl.QuadPart < rhs.impl.QuadPart) { return cmp_Ord_lt; }
    if (lhs.impl.QuadPart > rhs.impl.QuadPart) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
};
#endif /* plat_is_windows */

/* --- Unix (Linux, Darwin) --- */

#if plat_is_linux || plat_is_darwin
$static var_(time_InstantPlatform, s_unix_perf_freq) = cleared();
$static var_(f64, s_unix_perf_freq_inv) = f64_nan;
$static var_(time_InstantPlatform, s_unix_offset_value) = cleared();
$static var_(bool, s_unix_initialized) = false;

$attr($on_load)
$static fn_((time_Instant__unix_init(void))(void)) {
    if (s_unix_initialized) { return; }
    var value = lit0$((time_InstantPlatform));
    if (clock_gettime(CLOCK_MONOTONIC, &value) != 0) {
        claim_unreachable_msg("Failed to initialize high-resolution timer");
    }
    s_unix_perf_freq.tv_sec = 1;
    s_unix_perf_freq.tv_nsec = time_Instant_nanos_per_sec;
    s_unix_perf_freq_inv = 1.0 / as$(f64)(time_Instant_nanos_per_sec);
    s_unix_offset_value = value;
    s_unix_initialized = true;
};

$attr($inline_always)
$static fn_((time_Instant__unix_ensureInit(void))(void)) {
    return time_Instant__unix_init(), claim_assert_fmt(s_unix_initialized, "time_Instant not initialized");
};

fn_((time_Instant__unix_freq(void))(time_Instant)) {
    return time_Instant__unix_ensureInit(), (time_Instant){ .impl = s_unix_perf_freq };
};

fn_((time_Instant__unix_freqInv(void))(f64)) {
    return time_Instant__unix_ensureInit(), s_unix_perf_freq_inv;
};

fn_((time_Instant__unix_offset(void))(time_Instant)) {
    return time_Instant__unix_ensureInit(), (time_Instant){ .impl = s_unix_offset_value };
};

fn_((time_Instant__unix_now(void))(time_Instant)) {
    time_Instant__unix_ensureInit();
    var current = lit0$((time_InstantPlatform));
    clock_gettime(CLOCK_MONOTONIC, &current);
    return (time_Instant){ .impl = current };
};

fn_((time_Instant__unix_ticks(time_Instant self))(u64)) {
    return as$(u64)(self.impl.tv_sec) * time_Instant_nanos_per_sec + as$(u64)(self.impl.tv_nsec);
};

fn_((time_Instant__unix_durationSinceChkd(time_Instant later, time_Instant earlier))(O$time_Duration) $scope) {
    if (time_Instant__unix_ord(later, earlier) == cmp_Ord_lt) {
        return_none();
    }
    var diff = lit0$((time_InstantPlatform));
    diff.tv_sec = later.impl.tv_sec - earlier.impl.tv_sec;
    if (later.impl.tv_nsec < earlier.impl.tv_nsec) {
        diff.tv_sec--;
        diff.tv_nsec = time_Instant_nanos_per_sec + later.impl.tv_nsec - earlier.impl.tv_nsec;
    } else {
        diff.tv_nsec = later.impl.tv_nsec - earlier.impl.tv_nsec;
    }
    let nanos = as$(u64)((diff.tv_sec * time_Instant_nanos_per_sec) + diff.tv_nsec);
    return_some(time_Duration_fromNanos(nanos));
} $unscoped_(fn);

fn_((time_Instant__unix_addChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant) $scope) {
    let ticks = (rhs.secs * time_Instant_intervals_per_sec) + (rhs.nanos / 100);
    if ((0 <= lhs.impl.tv_sec) && ticks <= (u64_limit_max - as$(u64)(lhs.impl.tv_sec))) {
        return_some({
            .impl = {
                .tv_sec = lhs.impl.tv_sec + (ticks / time_Instant_nanos_per_sec),
                .tv_nsec = lhs.impl.tv_nsec + (ticks % time_Instant_nanos_per_sec),
            },
        });
    }
    return_none();
} $unscoped_(fn);

fn_((time_Instant__unix_subChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant) $scope) {
    let ticks = (rhs.secs * time_Instant_intervals_per_sec) + (rhs.nanos / 100);
    if ((0 <= lhs.impl.tv_sec) && ticks <= (u64_limit_max + as$(u64)(lhs.impl.tv_sec))) {
        return_some({
            .impl = {
                .tv_sec = lhs.impl.tv_sec - (ticks / time_Instant_nanos_per_sec),
                .tv_nsec = lhs.impl.tv_nsec - (ticks % time_Instant_nanos_per_sec),
            },
        });
    }
    return_none();
} $unscoped_(fn);

fn_((time_Instant__unix_ord(time_Instant lhs, time_Instant rhs))(cmp_Ord)) {
    if (lhs.impl.tv_sec < rhs.impl.tv_sec) { return cmp_Ord_lt; }
    if (lhs.impl.tv_sec > rhs.impl.tv_sec) { return cmp_Ord_gt; }
    if (lhs.impl.tv_nsec < rhs.impl.tv_nsec) { return cmp_Ord_lt; }
    if (lhs.impl.tv_nsec > rhs.impl.tv_nsec) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
};
#endif /* plat_is_linux || plat_is_darwin */

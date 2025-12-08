#include "dh/time/SysTime.h"
#include "dh/time/Instant.h"
#include "dh/time/Duration.h"

/*========== Internals ======================================================*/

/* Variables */
#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
/// Performance counter frequency in ticks per second.
static time_SysTimePlatform s_perf_freq = cleared();
/// Inverse of the performance counter frequency.
static f64 s_perf_freq_inv = f64_nan;
/// Offset between the QueryPerformanceFrequency and QueryPerformanceCounter
static time_SysTimePlatform s_pref_offset_value = cleared();
/// Whether the performance counter has been initialized.
static bool s_pref_initialized = false;
#else /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
static time_SysTimePlatform s_perf_freq = cleared();
static f64 s_perf_freq_inv = f64_nan;
static time_SysTimePlatform s_pref_offset_value = cleared();
static bool s_pref_initialized = false;
#endif

/* Initialization */
/// Initialize performance counter frequency and offset.
$attr($on_load)
$static fn_((init(void))(void)) {
    if (s_pref_initialized) { return; }
#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
    if (!QueryPerformanceFrequency(&s_perf_freq)) {
        claim_unreachable_msg("Failed to query performance frequency");
    }
    s_perf_freq_inv = 1.0 / as$(f64)(s_perf_freq.QuadPart);
    QueryPerformanceCounter(&s_pref_offset_value);
#else /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
    var value = makeCleared$(time_SysTimePlatform);
    if (clock_gettime(CLOCK_MONOTONIC, &value) != 0) {
        claim_unreachable_msg("Failed to initialize high-resolution timer");
    }
    // Assuming 1 second = 1 billion nanoseconds (nanoseconds per second)
    s_perf_freq.tv_sec = 1;                           // 1 second
    s_perf_freq.tv_nsec = time_SysTime_nanos_per_sec; // 1 billion nanoseconds (nanosecond precision)
    s_perf_freq_inv = 1.0 / as$(f64)(time_SysTime_nanos_per_sec);
    // Set the initial offset value to the current time
    s_pref_offset_value = value;
#endif
    s_pref_initialized = true;
}
$attr($inline_always)
void ensureInit(void) {
    return init(), debug_assert_fmt(s_pref_initialized, "SysTime not initialized");
}

/* Accessors */
$attr($inline_always)
time_SysTime freq(void) {
    return ensureInit(), (time_SysTime){ .impl_ = s_perf_freq };
}
$attr($inline_always)
f64 freqInv(void) {
    return ensureInit(), s_perf_freq_inv;
}
$attr($inline_always)
time_SysTime offset(void) {
    return ensureInit(), (time_SysTime){ .impl_ = s_pref_offset_value };
}
$attr($inline_always)
time_SysTime value(void) {
    return ensureInit(), (time_SysTime){
        .impl_ = blk({
            var current = make$((time_SysTimePlatform){});
#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
            QueryPerformanceCounter(&current);
#else /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
            clock_gettime(CLOCK_MONOTONIC, &current);
#endif
            blk_return current;
        })
    };
}

/*========== Accessors =====================================================*/

time_SysTime time_SysTime_freq(void) { return freq(); }
f64 time_SysTime_freqInv(void) { return freqInv(); }
time_SysTime time_SysTime_offset(void) { return offset(); }
time_SysTime time_SysTime_value(void) { return value(); }

/*========== Operations =====================================================*/

time_SysTime time_SysTime_now(void) {
    return value();
}

time_Duration time_SysTime_elapsed(time_SysTime self) {
    return time_SysTime_durationSince(time_SysTime_now(), self);
}

time_Duration time_SysTime_durationSince(time_SysTime later, time_SysTime earlier) {
    return unwrap_(time_SysTime_durationSinceChkd(later, earlier));
}

fn_((time_SysTime_durationSinceChkd(time_SysTime later, time_SysTime earlier))(O$time_Duration) $scope) {
    if (time_SysTime_ord(later, earlier) == cmp_Ord_lt) {
        return_none();
    }
#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
    // Calculate the difference in ticks
    let diff = as$(f64)(later.impl_.QuadPart - earlier.impl_.QuadPart);
    // Convert ticks to nanoseconds
    let nanos = as$(u64)(diff * time_SysTime_nanos_per_sec * freqInv());
#else  /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
    // Calculate the difference in seconds and nanoseconds
    var diff = makeCleared$(time_SysTimePlatform);
    diff.tv_sec = later.impl_.tv_sec - earlier.impl_.tv_sec;
    if (later.impl_.tv_nsec < earlier.impl_.tv_nsec) {
        diff.tv_sec--;
        diff.tv_nsec = time_SysTime_nanos_per_sec + later.impl_.tv_nsec - earlier.impl_.tv_nsec;
    } else {
        diff.tv_nsec = later.impl_.tv_nsec - earlier.impl_.tv_nsec;
    }
    let nanos = as$(u64)((diff.tv_sec * time_SysTime_nanos_per_sec) + diff.tv_nsec);
#endif /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
    return_some(time_Duration_fromNanos(nanos));
} $unscoped_(fn);

/*========== Arithmetic Operations ==========================================*/

op_fn_addWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime)) {
    return unwrap_(time_SysTime_addChkdDuration(lhs, rhs));
}

op_fn_addAsgWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime*)) {
    return *lhs = unwrap_(time_SysTime_addChkdDuration(*lhs, rhs)), lhs;
}

op_fn_subWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime)) {
    return unwrap_(time_SysTime_subChkdDuration(lhs, rhs));
}

op_fn_subAsgWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime*)) {
    return *lhs = unwrap_(time_SysTime_subChkdDuration(*lhs, rhs)), lhs;
}

fn_((time_SysTime_addChkdDuration(time_SysTime lhs, time_Duration rhs))(O$time_SysTime) $scope) {
    let ticks = (rhs.secs * time_SysTime_intervals_per_sec) + (rhs.nanos / 100);
#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
    if ((0 <= lhs.impl_.QuadPart) && ticks <= (u64_limit_max - as$(u64)(lhs.impl_.QuadPart))) {
        return_some({ .impl_.QuadPart = as$(LONGLONG)(lhs.impl_.QuadPart + ticks) });
    }
#else /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
    if ((0 <= lhs.impl_.tv_sec) && ticks <= (u64_limit_max - as$(u64)(lhs.impl_.tv_sec))) {
        return_some({
            .impl_ = {
                .tv_sec = lhs.impl_.tv_sec + (ticks / time_SysTime_nanos_per_sec),
                .tv_nsec = lhs.impl_.tv_nsec + (ticks % time_SysTime_nanos_per_sec),
            },
        });
    }
#endif
    return_none();
} $unscoped_(fn);

fn_((time_SysTime_subChkdDuration(time_SysTime lhs, time_Duration rhs))(O$time_SysTime) $scope) {
    let ticks = (rhs.secs * time_SysTime_intervals_per_sec) + (rhs.nanos / 100);
#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
    if ((0 <= lhs.impl_.QuadPart) && ticks <= (u64_limit_min + as$(u64)(lhs.impl_.QuadPart))) {
        return_some({ .impl_.QuadPart = as$(LONGLONG)(lhs.impl_.QuadPart - ticks) });
    }
#else /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
    if ((0 <= lhs.impl_.tv_sec) && ticks <= (u64_limit_max + as$(u64)(lhs.impl_.tv_sec))) {
        return_some({
            .impl_ = {
                .tv_sec = lhs.impl_.tv_sec - (ticks / time_SysTime_nanos_per_sec),
                .tv_nsec = lhs.impl_.tv_nsec - (ticks % time_SysTime_nanos_per_sec),
            },
        });
    }
#endif
    return_none();
} $unscoped_(fn);

/*========== Time Conversion to/from Unix Epoch =============================*/

time_SysTime time_SysTime_fromUnixEpoch(u64 secs) {
    // Convert seconds to intervals
    let intervals = secs * time_SysTime_intervals_per_sec;
    // Add the intervals to the Unix epoch's QuadPart to get the new time
    return (time_SysTime){
        .impl_ = {
#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
            .QuadPart = time_SysTime_unix_epoch.impl_.QuadPart + as$(LONGLONG)(intervals),
#else /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
            .tv_sec = time_SysTime_unix_epoch.impl_.tv_sec + (intervals / time_SysTime_nanos_per_sec),
            .tv_nsec = time_SysTime_unix_epoch.impl_.tv_nsec + (intervals % time_SysTime_nanos_per_sec),
#endif
        }
    };
}

u64 time_SysTime_toUnixEpoch(time_SysTime self) {
    // Calculate the difference in intervals from the Unix epoch
#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
    let diff = self.impl_.QuadPart - time_SysTime_unix_epoch.impl_.QuadPart;
#else /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
    let diff = self.impl_.tv_sec - time_SysTime_unix_epoch.impl_.tv_sec;
#endif
    // Convert the difference to seconds
    return as$(u64)(diff / time_SysTime_intervals_per_sec);
}

/*========== Comparison =====================================================*/

cmp_fn_ord$((time_SysTime)(self, other)) {
#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
    if (self.impl_.QuadPart < other.impl_.QuadPart) { return cmp_Ord_lt; }
    if (self.impl_.QuadPart > other.impl_.QuadPart) { return cmp_Ord_gt; }
#else /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
    if (self.impl_.tv_sec < other.impl_.tv_sec) { return cmp_Ord_lt; }
    if (self.impl_.tv_sec > other.impl_.tv_sec) { return cmp_Ord_gt; }
    if (self.impl_.tv_nsec < other.impl_.tv_nsec) { return cmp_Ord_lt; }
    if (self.impl_.tv_nsec > other.impl_.tv_nsec) { return cmp_Ord_gt; }
#endif
    return cmp_Ord_eq;
}
cmp_fn_eq_default$((time_SysTime)(lhs, rhs));
cmp_fn_ne_default$((time_SysTime)(lhs, rhs));
cmp_fn_lt_default$((time_SysTime)(lhs, rhs));
cmp_fn_gt_default$((time_SysTime)(lhs, rhs));
cmp_fn_le_default$((time_SysTime)(lhs, rhs));
cmp_fn_ge_default$((time_SysTime)(lhs, rhs));
cmp_fn_ordCtx$((time_SysTime)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_ord$(time_SysTime)(lhs, rhs);
}
cmp_fn_eqCtx_default$((time_SysTime)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_SysTime)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_SysTime)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_SysTime)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_SysTime)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_SysTime)(lhs, rhs, ctx));

cmp_fn_eql$((time_SysTime)(lhs, rhs)) {
    return cmp_ord$(time_SysTime)(lhs, rhs) == cmp_Ord_eq;
}
cmp_fn_neq_default$((time_SysTime)(lhs, rhs));
cmp_fn_eqlCtx$((time_SysTime)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_eql$(time_SysTime)(lhs, rhs);
}
cmp_fn_neqCtx_default$((time_SysTime)(lhs, rhs, ctx));

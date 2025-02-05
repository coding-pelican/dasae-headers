#include "dh/time/SysTime.h"

#include "dh/time/Instant.h"
#include "dh/time/Duration.h"

#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)

/*========== Static Variables ==============================================*/

static time_SysTimePlatform s_perf_freq     = cleared();
static f64                  s_perf_freq_inv = f64_nan;
static time_SysTimePlatform s_offset_value  = cleared();
static bool                 s_initialized   = false;

/*========== Initialization ==============================================*/

/// Initialize performance counter frequency and offset.
static void __attribute__((constructor)) time_SysTime_init(void) {
    if (!QueryPerformanceFrequency(&s_perf_freq)) {
        claim_unreachable_msg("Failed to query performance frequency");
    }
    s_perf_freq_inv = 1.0 / as$(f64, s_perf_freq.QuadPart);
    QueryPerformanceCounter(&s_offset_value);
    s_initialized = true;
}

/*========== Accessors =====================================================*/

time_SysTime time_SysTime_frequency(void) {
    debug_assert_fmt(s_initialized, "SysTime not initialized");
    return (time_SysTime){ .impl_ = s_perf_freq };
}

f64 time_SysTime_frequencyInv(void) {
    debug_assert_fmt(s_initialized, "SysTime not initialized");
    return s_perf_freq_inv;
}

time_SysTime time_SysTime_value(void) {
    debug_assert_fmt(s_initialized, "SysTime not initialized");
    time_SysTimePlatform current = cleared();
    QueryPerformanceCounter(&current);
    return (time_SysTime){ .impl_ = current };
}

time_SysTime time_SysTime_offset(void) {
    debug_assert_fmt(s_initialized, "SysTime not initialized");
    return (time_SysTime){ .impl_ = s_offset_value };
}

/*========== Operations =====================================================*/

time_SysTime time_SysTime_now(void) {
    return time_SysTime_value();
}

time_Duration time_SysTime_elapsed(time_SysTime self) {
    return time_SysTime_durationSince(time_SysTime_now(), self);
}

time_Duration time_SysTime_durationSince(time_SysTime self, time_SysTime earlier) {
    return unwrap(time_SysTime_durationSinceChecked(self, earlier));
}

Opt$time_Duration time_SysTime_durationSinceChecked(time_SysTime self, time_SysTime earlier) {
    reserveReturn(Opt$time_Duration);
    if (self.impl_.QuadPart < earlier.impl_.QuadPart) {
        return_none();
    }
    // Calculate the difference in ticks
    let diff  = as$(f64, self.impl_.QuadPart - earlier.impl_.QuadPart);
    // Convert ticks to nanoseconds
    let nanos = as$(u64, diff * time_SysTime_nanos_per_sec * s_perf_freq_inv);
    return_some(time_Duration_fromNanos(nanos));
}

/*========== Arithmetic Operations ==========================================*/

time_SysTime op_fnAddBy(time_SysTime, time_Duration) {
    return unwrap(time_SysTime_addDurationChecked(self, other));
}

time_SysTime op_fnSubBy(time_SysTime, time_Duration) {
    return unwrap(time_SysTime_subDurationChecked(self, other));
}

Opt$time_SysTime time_SysTime_addDurationChecked(time_SysTime lhs, time_Duration rhs) {
    reserveReturn(Opt$time_SysTime);
    let ticks = (rhs.secs * time_SysTime_intervals_per_sec) + (rhs.nanos / 100);
    if ((0 <= lhs.impl_.QuadPart) && ticks <= (u64_limit_max - as$(u64, lhs.impl_.QuadPart))) {
        return_some({ .impl_.QuadPart = as$(LONGLONG, lhs.impl_.QuadPart + ticks) });
    }
    return_none();
}

Opt$time_SysTime time_SysTime_subDurationChecked(time_SysTime lhs, time_Duration rhs) {
    reserveReturn(Opt$time_SysTime);
    let ticks = (rhs.secs * time_SysTime_intervals_per_sec) + (rhs.nanos / 100);
    if ((0 <= lhs.impl_.QuadPart) && ticks <= (u64_limit_min + as$(u64, lhs.impl_.QuadPart))) {
        return_some({ .impl_.QuadPart = as$(LONGLONG, lhs.impl_.QuadPart - ticks) });
    }
    return_none();
}

/*========== Time Conversion to/from Unix Epoch =============================*/

time_SysTime time_SysTime_fromUnixEpoch(u64 secs) {
    // Convert seconds to intervals
    let intervals = secs * time_SysTime_intervals_per_sec;
    // Add the intervals to the Unix epoch's QuadPart to get the new time
    return (time_SysTime){
        .impl_ = { .QuadPart = time_SysTime_unix_epoch.impl_.QuadPart + as$(LONGLONG, intervals) }
    };
}

u64 time_SysTime_toUnixEpoch(time_SysTime self) {
    // Calculate the difference in intervals from the Unix epoch
    let diff = self.impl_.QuadPart - time_SysTime_unix_epoch.impl_.QuadPart;
    // Convert the difference to seconds
    return as$(u64, diff / time_SysTime_intervals_per_sec);
}

/*========== Comparison =====================================================*/

cmp_fnCmp(time_SysTime) {
    if (self.impl_.QuadPart < other.impl_.QuadPart) { return cmp_Ord_lt; }
    if (self.impl_.QuadPart > other.impl_.QuadPart) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}

#endif /* bti_plat_windows && (bti_plat_32BIT || bti_plat_64BIT) */

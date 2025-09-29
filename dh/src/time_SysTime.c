#include "dh/time/SysTime.h"
#include <time.h>

#include "dh/time/Instant.h"
#include "dh/time/Duration.h"

#include "dh/fn.h"

/*========== Internals ======================================================*/

/* Variables */
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
/// Performance counter frequency in ticks per second.
static time_SysTimePlatform s_perf_freq         = cleared();
/// Inverse of the performance counter frequency.
static f64                  s_perf_freq_inv     = f64_nan;
/// Offset between the QueryPerformanceFrequency and QueryPerformanceCounter
static time_SysTimePlatform s_pref_offset_value = cleared();
/// Whether the performance counter has been initialized.
static bool                 s_pref_initialized  = false;
#else /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
static time_SysTimePlatform s_perf_freq         = cleared();
static f64                  s_perf_freq_inv     = f64_nan;
static time_SysTimePlatform s_pref_offset_value = cleared();
static bool                 s_pref_initialized  = false;
#endif

/* Initialization */
/// Initialize performance counter frequency and offset.
static $on_load fn_(init(void), void) {
    if (s_pref_initialized) { return; }
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
    if (!QueryPerformanceFrequency(&s_perf_freq)) {
        claim_unreachable_msg("Failed to query performance frequency");
    }
    s_perf_freq_inv = 1.0 / as$(f64, s_perf_freq.QuadPart);
    QueryPerformanceCounter(&s_pref_offset_value);
#else /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
    var value = makeCleared$(time_SysTimePlatform);
    if (clock_gettime(CLOCK_MONOTONIC, &value) != 0) {
        claim_unreachable_msg("Failed to initialize high-resolution timer");
    }
    // Assuming 1 second = 1 billion nanoseconds (nanoseconds per second)
    s_perf_freq.tv_sec  = 1;                          // 1 second
    s_perf_freq.tv_nsec = time_SysTime_nanos_per_sec; // 1 billion nanoseconds (nanosecond precision)
    s_perf_freq_inv     = 1.0 / as$(f64, time_SysTime_nanos_per_sec);
    // Set the initial offset value to the current time
    s_pref_offset_value = value;
#endif
    s_pref_initialized = true;
}
$inline_always void ensureInit(void) {
    return init(), debug_assert_fmt(s_pref_initialized, "SysTime not initialized");
}

/* Accessors */
$inline_always time_SysTime freq(void) {
    return ensureInit(), (time_SysTime){ .impl_ = s_perf_freq };
}
$inline_always f64 freqInv(void) {
    return ensureInit(), s_perf_freq_inv;
}
$inline_always time_SysTime offset(void) {
    return ensureInit(), (time_SysTime){ .impl_ = s_pref_offset_value };
}
$inline_always time_SysTime value(void) {
    return ensureInit(), (time_SysTime){
        .impl_ = eval({
            var current = makeCleared$(time_SysTimePlatform);
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
            QueryPerformanceCounter(&current);
#else /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
            clock_gettime(CLOCK_MONOTONIC, &current);
#endif
            eval_return current;
        })
    };
}

/*========== Accessors =====================================================*/

time_SysTime time_SysTime_freq(void) { return freq(); }
f64          time_SysTime_freqInv(void) { return freqInv(); }
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
    return unwrap(time_SysTime_chkdDurationSince(later, earlier));
}

fn_(time_SysTime_chkdDurationSince(time_SysTime later, time_SysTime earlier), Opt$time_Duration $scope) {
    if (time_SysTime_lt(later, earlier)) {
        return_none();
    }
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
    // Calculate the difference in ticks
    let diff  = as$(f64, later.impl_.QuadPart - earlier.impl_.QuadPart);
    // Convert ticks to nanoseconds
    let nanos = as$(u64, diff* time_SysTime_nanos_per_sec* freqInv());
#else  /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
    // Calculate the difference in seconds and nanoseconds
    var diff    = makeCleared$(time_SysTimePlatform);
    diff.tv_sec = later.impl_.tv_sec - earlier.impl_.tv_sec;
    if (later.impl_.tv_nsec < earlier.impl_.tv_nsec) {
        diff.tv_sec--;
        diff.tv_nsec = time_SysTime_nanos_per_sec + later.impl_.tv_nsec - earlier.impl_.tv_nsec;
    } else {
        diff.tv_nsec = later.impl_.tv_nsec - earlier.impl_.tv_nsec;
    }
    let nanos = as$(u64, diff.tv_sec* time_SysTime_nanos_per_sec) + diff.tv_nsec;
#endif /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
    return_some(time_Duration_fromNanos(nanos));
} $unscoped;

/*========== Arithmetic Operations ==========================================*/

time_SysTime op_fnAddBy(time_SysTime, time_Duration) {
    return unwrap(time_SysTime_chkdAddDuration(self, other));
}

time_SysTime op_fnAddAsgBy(time_SysTime, time_Duration) {
    return *self = op_addBy(time_SysTime, time_Duration)(*self, other);
}

time_SysTime op_fnSubBy(time_SysTime, time_Duration) {
    return unwrap(time_SysTime_chkdSubDuration(self, other));
}

time_SysTime op_fnSubAsgBy(time_SysTime, time_Duration) {
    return *self = op_subBy(time_SysTime, time_Duration)(*self, other);
}

fn_(time_SysTime_chkdAddDuration(time_SysTime lhs, time_Duration rhs), Opt$time_SysTime $scope) {
    let ticks = (rhs.secs * time_SysTime_intervals_per_sec) + (rhs.nanos / 100);
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
    if ((0 <= lhs.impl_.QuadPart) && ticks <= (u64_limit_max - as$(u64, lhs.impl_.QuadPart))) {
        return_some({ .impl_.QuadPart = as$(LONGLONG, lhs.impl_.QuadPart + ticks) });
    }
#else /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
    if ((0 <= lhs.impl_.tv_sec) && ticks <= (u64_limit_max - as$(u64, lhs.impl_.tv_sec))) {
        return_some({
            .impl_ = {
                .tv_sec  = lhs.impl_.tv_sec + (ticks / time_SysTime_nanos_per_sec),
                .tv_nsec = lhs.impl_.tv_nsec + (ticks % time_SysTime_nanos_per_sec),
            },
        });
    }
#endif
    return_none();
} $unscoped;

fn_(time_SysTime_chkdSubDuration(time_SysTime lhs, time_Duration rhs), Opt$time_SysTime $scope) {
    let ticks = (rhs.secs * time_SysTime_intervals_per_sec) + (rhs.nanos / 100);
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
    if ((0 <= lhs.impl_.QuadPart) && ticks <= (u64_limit_min + as$(u64, lhs.impl_.QuadPart))) {
        return_some({ .impl_.QuadPart = as$(LONGLONG, lhs.impl_.QuadPart - ticks) });
    }
#else /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
    if ((0 <= lhs.impl_.tv_sec) && ticks <= (u64_limit_max + as$(u64, lhs.impl_.tv_sec))) {
        return_some({
            .impl_ = {
                .tv_sec  = lhs.impl_.tv_sec - (ticks / time_SysTime_nanos_per_sec),
                .tv_nsec = lhs.impl_.tv_nsec - (ticks % time_SysTime_nanos_per_sec),
            },
        });
    }
#endif
    return_none();
} $unscoped;

/*========== Time Conversion to/from Unix Epoch =============================*/

time_SysTime time_SysTime_fromUnixEpoch(u64 secs) {
    // Convert seconds to intervals
    let intervals = secs * time_SysTime_intervals_per_sec;
    // Add the intervals to the Unix epoch's QuadPart to get the new time
    return (time_SysTime){
        .impl_ = {
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
            .QuadPart = time_SysTime_unix_epoch.impl_.QuadPart + as$(LONGLONG, intervals),
#else /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
            .tv_sec  = time_SysTime_unix_epoch.impl_.tv_sec + (intervals / time_SysTime_nanos_per_sec),
            .tv_nsec = time_SysTime_unix_epoch.impl_.tv_nsec + (intervals % time_SysTime_nanos_per_sec),
#endif
        }
    };
}

u64 time_SysTime_toUnixEpoch(time_SysTime self) {
    // Calculate the difference in intervals from the Unix epoch
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
    let diff = self.impl_.QuadPart - time_SysTime_unix_epoch.impl_.QuadPart;
#else /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
    let diff = self.impl_.tv_sec - time_SysTime_unix_epoch.impl_.tv_sec;
#endif
    // Convert the difference to seconds
    return as$(u64, diff / time_SysTime_intervals_per_sec);
}

/*========== Comparison =====================================================*/

cmp_fnCmp(time_SysTime) {
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
    if (self.impl_.QuadPart < other.impl_.QuadPart) { return cmp_Ord_lt; }
    if (self.impl_.QuadPart > other.impl_.QuadPart) { return cmp_Ord_gt; }
#else /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
    if (self.impl_.tv_sec < other.impl_.tv_sec) { return cmp_Ord_lt; }
    if (self.impl_.tv_sec > other.impl_.tv_sec) { return cmp_Ord_gt; }
    if (self.impl_.tv_nsec < other.impl_.tv_nsec) { return cmp_Ord_lt; }
    if (self.impl_.tv_nsec > other.impl_.tv_nsec) { return cmp_Ord_gt; }
#endif
    return cmp_Ord_eq;
}

#include "dh/time.h"

#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)

#define intervals_per_sec       (10000000ULL)
#define secs_to_unix_epoch      (11644473600ULL)
#define intervals_to_unix_epoch (secs_to_unix_epoch * intervals_per_sec)
const time_SysTime time_SysTime_unix_epoch = { .QuadPart = intervals_to_unix_epoch };

/* Static variables for performance counter */
static time_SysTime time_SysTime_s_performance_frequency = make$(time_SysTime);
static f64          time_SysTime_s_frequency_inverse     = 0.0;
static time_SysTime time_SysTime_s_offset_value          = make$(time_SysTime);
static bool         time_SysTime_s_initialized           = false;

/* Initialize performance counter frequency */
static void __attribute__((constructor)) time_SysTime_init(void) {
    if (!QueryPerformanceFrequency(&time_SysTime_s_performance_frequency)) {
        claim_unreachable_msg("Failed to query performance frequency");
    }
    time_SysTime_s_frequency_inverse = 1.0 / as$(f64, time_SysTime_s_performance_frequency.QuadPart);
    QueryPerformanceCounter(&time_SysTime_s_offset_value);
    time_SysTime_s_initialized = true;
}

/*========== Core Functions ============================================*/

time_SysTime time_SysTime_frequency(void) {
    debug_assert_fmt(time_SysTime_s_initialized, "SysTime not initialized");
    return time_SysTime_s_performance_frequency;
}

f64 time_SysTime_frequencyInv(void) {
    debug_assert_fmt(time_SysTime_s_initialized, "SysTime not initialized");
    return time_SysTime_s_frequency_inverse;
}

time_SysTime time_SysTime_value(void) {
    time_SysTime current = cleared();
    debug_assert_fmt(time_SysTime_s_initialized, "SysTime not initialized");
    QueryPerformanceCounter(&current);
    return current;
}

time_SysTime time_SysTime_offset(void) {
    debug_assert_fmt(time_SysTime_s_initialized, "SysTime not initialized");
    return time_SysTime_s_offset_value;
}

time_SysTime time_SysTime_now(void) {
    time_SysTime current = cleared();
    debug_assert_fmt(time_SysTime_s_initialized, "SysTime not initialized");
    QueryPerformanceCounter(&current);
    return current;
}

/*========== Duration Calculations =====================================*/

time_Duration time_SysTime_elapsed(time_SysTime self) {
    time_SysTime current = cleared();
    debug_assert_fmt(time_SysTime_s_initialized, "SysTime not initialized");
    QueryPerformanceCounter(&current);

    u64 diff = current.QuadPart - self.QuadPart;
    return time_Duration_fromNanos(
        as$(u64, as$(f64, diff) * as$(f64, time_nanos_per_sec) * time_SysTime_s_frequency_inverse)
    );
}

time_Duration time_SysTime_durationSince(time_SysTime self, time_SysTime earlier) {
    debug_assert_fmt(time_SysTime_s_initialized, "SysTime not initialized");

    // Calculate epsilon (1 tick) as in Rust implementation
    time_Duration epsilon = time_Duration_fromNanos(
        time_nanos_per_sec / time_SysTime_s_performance_frequency.QuadPart
    );

    if (self.QuadPart < earlier.QuadPart
        && as$(u64, earlier.QuadPart - self.QuadPart)
               <= as$(u64, epsilon.nanos / time_SysTime_s_frequency_inverse)) {
        return time_Duration_zero;
    }

    u64 diff = self.QuadPart - earlier.QuadPart;
    return time_Duration_fromNanos(
        as$(u64, as$(f64, diff) * as$(f64, time_nanos_per_sec) * time_SysTime_s_frequency_inverse)
    );
}

/*========== Safe Arithmetic Operations ================================*/

Opt$time_SysTime time_SysTime_addDurationChecked(time_SysTime lhs, time_Duration rhs) {
    reserveReturn(Opt$time_SysTime);
    let nanos = rhs.secs * time_nanos_per_sec + rhs.nanos;
    let ticks = as$(u64, as$(f64, nanos) * time_SysTime_s_frequency_inverse);
    if (ticks <= (u64_limit - lhs.QuadPart)) {
        return_some((time_SysTime){ .QuadPart = (LONGLONG)(lhs.QuadPart + ticks) });
    }
    return_none();
}

Opt$time_SysTime time_SysTime_subDurationChecked(time_SysTime lhs, time_Duration rhs) {
    reserveReturn(Opt$time_SysTime);
    let nanos = rhs.secs * time_nanos_per_sec + rhs.nanos;
    let ticks = as$(u64, as$(f64, nanos) * time_SysTime_s_frequency_inverse);
    if (ticks <= as$(u64, lhs.QuadPart)) {
        return_some((time_SysTime){ .QuadPart = (LONGLONG)(lhs.QuadPart - ticks) });
    }
    return_none();
}

/*========== Unsafe Arithmetic Operations ==============================*/

time_SysTime op_fnAddBy(time_SysTime, time_Duration) {
    return unwrap(time_SysTime_addDurationChecked(self, other));
}

time_SysTime op_fnSubBy(time_SysTime, time_Duration) {
    return unwrap(time_SysTime_subDurationChecked(self, other));
}

/*========== Comparison Functions =====================================*/

cmp_fnCmp(time_SysTime) {
    if (self.QuadPart < other.QuadPart) { return cmp_Ord_lt; }
    if (self.QuadPart > other.QuadPart) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}

#endif /* bti_plat_windows && (bti_plat_32BIT || bti_plat_64BIT) */

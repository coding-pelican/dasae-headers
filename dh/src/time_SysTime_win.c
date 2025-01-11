#include "dh/time.h"

#if BUILTIN_PLTF_WINDOWS && (BUILTIN_PLTF_32BIT || BUILTIN_PLTF_64BIT)

#define intervals_per_sec       (10000000ULL)
#define secs_to_unix_epoch      (11644473600ULL)
#define intervals_to_unix_epoch (secs_to_unix_epoch * intervals_per_sec)
const time_SysTime time_SysTime_unix_epoch = { .QuadPart = intervals_to_unix_epoch };

/* Static variables for performance counter */
static time_SysTime s_performance_frequency = make(time_SysTime);
static f64          s_frequency_inverse     = 0.0;
static time_SysTime s_offset_value          = make(time_SysTime);
static bool         s_initialized           = false;

/* Initialize performance counter frequency */
static void __attribute__((constructor)) init(void) {
    if (!QueryPerformanceFrequency(&s_performance_frequency)) {
        return claim_unreachable();
    }
    s_frequency_inverse = 1.0 / as(f64, s_performance_frequency.QuadPart);
    QueryPerformanceCounter(&s_offset_value);
    s_initialized = true;
}

/*========== Core Functions ============================================*/

time_SysTime time_SysTime_frequency(void) {
    debug_assert_fmt(s_initialized, "SysTime not initialized");
    return s_performance_frequency;
}

f64 time_SysTime_frequencyInv(void) {
    debug_assert_fmt(s_initialized, "SysTime not initialized");
    return s_frequency_inverse;
}

time_SysTime time_SysTime_value(void) {
    time_SysTime current = cleared();
    debug_assert_fmt(s_initialized, "SysTime not initialized");
    QueryPerformanceCounter(&current);
    return current;
}

time_SysTime time_SysTime_offset(void) {
    debug_assert_fmt(s_initialized, "SysTime not initialized");
    return s_offset_value;
}

time_SysTime time_SysTime_now(void) {
    time_SysTime current = cleared();
    debug_assert_fmt(s_initialized, "SysTime not initialized");
    QueryPerformanceCounter(&current);
    return current;
}

/*========== Duration Calculations =====================================*/

time_Duration time_SysTime_elapsed(time_SysTime self) {
    time_SysTime current = cleared();
    debug_assert_fmt(s_initialized, "SysTime not initialized");
    QueryPerformanceCounter(&current);

    u64 diff = current.QuadPart - self.QuadPart;
    return time_Duration_fromNanos(
        as(u64, as(f64, diff) * as(f64, time_nanos_per_sec) * s_frequency_inverse)
    );
}

time_Duration time_SysTime_durationSince(time_SysTime self, time_SysTime earlier) {
    debug_assert_fmt(s_initialized, "SysTime not initialized");

    // Calculate epsilon (1 tick) as in Rust implementation
    time_Duration epsilon = time_Duration_fromNanos(
        time_nanos_per_sec / s_performance_frequency.QuadPart
    );

    if (earlier.QuadPart > self.QuadPart && as(u64, earlier.QuadPart - self.QuadPart) <= as(u64, epsilon.nanos_ / s_frequency_inverse)) {
        return time_Duration_zero;
    }

    u64 diff = self.QuadPart - earlier.QuadPart;
    return time_Duration_fromNanos(
        as(u64, as(f64, diff) * as(f64, time_nanos_per_sec) * s_frequency_inverse)
    );
}

/*========== Safe Arithmetic Operations ================================*/

Opt$time_SysTime time_SysTime_addDurationChecked(time_SysTime self, time_Duration other) {
    reserveReturn(Opt$time_SysTime);
    u64 nanos = other.secs_ * time_nanos_per_sec + other.nanos_;
    u64 ticks = as(u64, as(f64, nanos) * s_frequency_inverse);
    if (ticks > (u64_limit - self.QuadPart)) {
        return_none();
    }
    return_some(make(time_SysTime, .QuadPart = (LONGLONG)(self.QuadPart + ticks)));
}

Opt$time_SysTime time_SysTime_subDurationChecked(time_SysTime self, time_Duration other) {
    reserveReturn(Opt$time_SysTime);
    u64 nanos = other.secs_ * time_nanos_per_sec + other.nanos_;
    u64 ticks = as(u64, as(f64, nanos) * s_frequency_inverse);
    if (ticks > as(u64, self.QuadPart)) {
        return_none();
    }
    return_some(make(time_SysTime, .QuadPart = (LONGLONG)(self.QuadPart - ticks)));
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
    if (self.QuadPart < other.QuadPart) { return cmp_Ord_less; }
    if (self.QuadPart > other.QuadPart) { return cmp_Ord_greater; }
    return cmp_Ord_equal;
}

#endif /* BUILTIN_PLTF_WINDOWS && (BUILTIN_PLTF_32BIT || BUILTIN_PLTF_64BIT) */

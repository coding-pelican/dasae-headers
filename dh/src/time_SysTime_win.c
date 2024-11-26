#if defined(_WIN32) || defined(_WIN64)

#include "dh/core/ops.h"
#include "dh/core/prim/struct.h"
#include "dh/time/cfg.h"
#include "dh/time/common.h"
#include "dh/time/SysTime.h"
#include "dh/time/Instant.h"
#include "dh/time/Duration.h"
#include "dh/debug/assert.h"
#include "dh/claim/unreachable.h"

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
    s_frequency_inverse = 1.0 / (f64)s_performance_frequency.QuadPart;
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
        (u64)((f64)diff * (f64)time_nanos_per_sec * s_frequency_inverse)
    );
}

time_Duration time_SysTime_durationSince(time_SysTime self, time_SysTime earlier) {
    debug_assert_fmt(s_initialized, "SysTime not initialized");

    // Calculate epsilon (1 tick) as in Rust implementation
    time_Duration epsilon = time_Duration_fromNanos(
        time_nanos_per_sec / s_performance_frequency.QuadPart
    );

    if (earlier.QuadPart > self.QuadPart && (u64)(earlier.QuadPart - self.QuadPart) <= (u64)(epsilon.nanos_ / s_frequency_inverse)) {
        return time_Duration_zero;
    }

    u64 diff = self.QuadPart - earlier.QuadPart;
    return time_Duration_fromNanos(
        (u64)((f64)diff * (f64)time_nanos_per_sec * s_frequency_inverse)
    );
}

/*========== Safe Arithmetic Operations ================================*/
bool ops_try_add_other(time_SysTime, time_Duration, time_SysTime) {
    debug_assert_fmt(out != null, "Output parameter cannot be null");

    u64 nanos = other.secs_ * time_nanos_per_sec + other.nanos_;
    u64 ticks = (u64)((f64)nanos * s_frequency_inverse);
    if (ticks > (UINT64_MAX - self.QuadPart)) {
        return false;
    }

    out->QuadPart = (LONGLONG)(self.QuadPart + ticks);
    return true;
}

bool ops_try_sub_other(time_SysTime, time_Duration, time_SysTime) {
    debug_assert_fmt(out != null, "Output parameter cannot be null");

    u64 nanos = other.secs_ * time_nanos_per_sec + other.nanos_;
    u64 ticks = (u64)((f64)nanos * s_frequency_inverse);

    if (ticks > (u64)self.QuadPart) {
        return false;
    }

    out->QuadPart = (LONGLONG)(self.QuadPart - ticks);
    return true;
}

/*========== Unsafe Arithmetic Operations ==============================*/

time_SysTime ops_add_other(time_SysTime, time_Duration) {
    time_SysTime result  = cleared();
    bool         success = time_SysTime_try_add_time_Duration(self, other, &result);
    claim_assert_fmt(success, "Arithmetic overflow in time_SysTime addition");
    return result;
}

time_SysTime ops_sub_other(time_SysTime, time_Duration) {
    time_SysTime result  = cleared();
    bool         success = time_SysTime_try_sub_time_Duration(self, other, &result);
    claim_assert_fmt(success, "Arithmetic underflow in time_SysTime subtraction");
    return result;
}

/*========== Comparison Functions =====================================*/

cmp_Ord time_SysTime_cmp(time_SysTime self, time_SysTime other) {
    if (self.QuadPart < other.QuadPart) { return cmp_Ord_less; }
    if (self.QuadPart > other.QuadPart) { return cmp_Ord_greater; }
    return cmp_Ord_equal;
}

/*========== Sleep Functions ==========================================*/

void time_SysTime_sleep(time_Duration duration) {
    HANDLE timer = CreateWaitableTimerExW(
        null, null, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS
    );

    if (timer == null) {
        Sleep((DWORD)((duration.secs_ * time_millis_per_sec) + (duration.nanos_ / time_nanos_per_milli)));
        return;
    }

    LARGE_INTEGER li = make(LARGE_INTEGER);
    // Convert to negative 100-nanosecond intervals for relative time
    li.QuadPart      = -((LONGLONG)((ULONGLONG)duration.secs_ * intervals_per_sec + (duration.nanos_ / 100)));

    if (SetWaitableTimer(timer, &li, 0, null, null, false)) {
        WaitForSingleObject(timer, INFINITE);
    } else {
        Sleep((DWORD)((duration.secs_ * time_millis_per_sec) + (duration.nanos_ / time_nanos_per_milli)));
    }

    CloseHandle(timer);
}

void time_SysTime_sleepSecs(u64 secs) {
    time_SysTime_sleep(time_Duration_fromSecs(secs));
}

void time_SysTime_sleepMillis(u64 millis) {
    time_SysTime_sleep(time_Duration_fromMillis(millis));
}

void time_SysTime_sleepMicros(u64 micros) {
    time_SysTime_sleep(time_Duration_fromMicros(micros));
}

void time_SysTime_sleepNanos(u32 nanos) {
    time_SysTime_sleep(time_Duration_fromNanos(nanos));
}

#endif /* defined(_WIN32) || defined(_WIN64) */


// #define SUPPORT_BUSY_WAIT_LOOP
// // Wait for some time (stop program execution)
// // NOTE: Sleep() granularity could be around 10 ms, it means, Sleep() could
// // take longer than expected... for that reason we use the busy wait loop
// // Ref: http://stackoverflow.com/questions/43057578/c-programming-win32-games-sleep-taking-longer-than-expected
// // Ref: http://www.geisswerks.com/ryan/FAQS/timing.html --> All about timing on Win32!
// void time_SysTime_sleep_s_f64(f64 secs) {
//     if (secs < 0) { return; /* Security check */ }

// #if defined(SUPPORT_BUSY_WAIT_LOOP) || defined(SUPPORT_PARTIAL_BUSY_WAIT_LOOP)
//     f64 dest_time = time_SysTime_now_f64() + secs;
// #endif

// #if defined(SUPPORT_BUSY_WAIT_LOOP)
//     while (time_SysTime_now_f64() < dest_time) {}
// #else
// #if defined(SUPPORT_PARTIAL_BUSY_WAIT_LOOP)
//     f64 sleep_secs = secs - secs * 0.05; // NOTE: We reserve a percentage of the time for busy waiting
// #else
//     f64 sleep_secs = secs;
// #endif

// // System halt functions
// #if defined(_WIN32)
//     Sleep((u16)(sleep_secs * time_millis_per_sec));
// #endif
// #if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__EMSCRIPTEN__)
//     struct timespec req  = { 0 };
//     time_t          sec  = sleep_secs;
//     long            nsec = (sleep_secs - sec) * 1000000000L;
//     req.tv_sec           = sec;
//     req.tv_nsec          = nsec;

//     // NOTE: Use nanosleep() on Unix platforms... usleep() it's deprecated
//     while (nanosleep(&req, &req) == -1) {
//         continue;
//     }
// #endif
// #if defined(__APPLE__)
//     usleep(sleep_secs * 1000000.0);
// #endif

// #if defined(SUPPORT_PARTIAL_BUSY_WAIT_LOOP)
//     while (time_SysTime_now_f64() < dest_time) {}
// #endif
// #endif
// }

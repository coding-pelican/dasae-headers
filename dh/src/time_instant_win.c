#if defined(_WIN32) || defined(_WIN64)

#include "dh/time/cfg.h"
#include "dh/time/system.h"
#include "dh/time/instant.h"
#include "dh/time/duration.h"


static time_SystemTime time_SystemTime__s_performance_frequency  = make(time_SystemTime);
static f64             time_SystemTime__s_frequency_inverse      = 0.0;
static time_SystemTime time_SystemTime__s_offset_value_frequency = make(time_SystemTime);
static bool            time_SystemTime__s_frequency_initialized  = false;

static void time_SystemTime__init(void) {
    if (time_SystemTime__s_frequency_initialized) { return; }

    time_SystemTime__s_performance_frequency = make(time_SystemTime);
    QueryPerformanceFrequency(&time_SystemTime__s_performance_frequency);

    time_SystemTime__s_frequency_inverse      = 1.0 / (f64)time_SystemTime__s_performance_frequency.QuadPart;
    time_SystemTime__s_frequency_initialized  = true;
    time_SystemTime__s_offset_value_frequency = time_SystemTime_value();
}

time_SystemTime time_SystemTime_frequency(void) {
    return time_SystemTime__s_performance_frequency;
}

f64 time_SystemTime_frequency_inv(void) {
    time_SystemTime__init();
    return time_SystemTime__s_frequency_inverse;
}

time_SystemTime time_SystemTime_value(void) {
    time_SystemTime sys_time = make(time_SystemTime);
    QueryPerformanceCounter(&sys_time);
    return sys_time;
}

time_SystemTime time_SystemTime_offset(void) {
    time_SystemTime__init();
    return time_SystemTime__s_offset_value_frequency;
}

time_SystemTime time_SystemTime_now(void) {
    time_SystemTime__init();
    time_SystemTime time   = time_SystemTime_value();
    time_SystemTime result = make(time_SystemTime);
    result.QuadPart        = (LONGLONG)((f64)(time.QuadPart - time_SystemTime_offset().QuadPart) * time_SystemTime__s_frequency_inverse);
    return result;
}

f64 time_SystemTime_now_f64(void) {
    time_SystemTime__init();
    time_SystemTime time = time_SystemTime_value();
    return ((f64)(time.QuadPart - time_SystemTime_offset().QuadPart) * time_SystemTime__s_frequency_inverse);
}

void time_SystemTime_sleep(time_Duration duration) {
    time_SystemTime_sleep_s_f64(time_Duration_asSecs_f64(duration));
}

void time_SystemTime_sleep_s(u64 secs) {
    time_SystemTime_sleep(time_Duration_from(secs, 0));
}

#define SUPPORT_BUSY_WAIT_LOOP
// Wait for some time (stop program execution)
// NOTE: Sleep() granularity could be around 10 ms, it means, Sleep() could
// take longer than expected... for that reason we use the busy wait loop
// Ref: http://stackoverflow.com/questions/43057578/c-programming-win32-games-sleep-taking-longer-than-expected
// Ref: http://www.geisswerks.com/ryan/FAQS/timing.html --> All about timing on Win32!
void time_SystemTime_sleep_s_f64(f64 secs) {
    if (secs < 0) { return; /* Security check */ }

#if defined(SUPPORT_BUSY_WAIT_LOOP) || defined(SUPPORT_PARTIAL_BUSY_WAIT_LOOP)
    f64 dest_time = time_SystemTime_now_f64() + secs;
#endif

#if defined(SUPPORT_BUSY_WAIT_LOOP)
    while (time_SystemTime_now_f64() < dest_time) {}
#else
#if defined(SUPPORT_PARTIAL_BUSY_WAIT_LOOP)
    f64 sleep_secs = secs - secs * 0.05; // NOTE: We reserve a percentage of the time for busy waiting
#else
    f64 sleep_secs = secs;
#endif

// System halt functions
#if defined(_WIN32)
    Sleep((u16)(sleep_secs * time_millis_per_sec));
#endif
#if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__EMSCRIPTEN__)
    struct timespec req  = { 0 };
    time_t          sec  = sleep_secs;
    long            nsec = (sleep_secs - sec) * 1000000000L;
    req.tv_sec           = sec;
    req.tv_nsec          = nsec;

    // NOTE: Use nanosleep() on Unix platforms... usleep() it's deprecated
    while (nanosleep(&req, &req) == -1) {
        continue;
    }
#endif
#if defined(__APPLE__)
    usleep(sleep_secs * 1000000.0);
#endif

#if defined(SUPPORT_PARTIAL_BUSY_WAIT_LOOP)
    while (time_SystemTime_now_f64() < dest_time) {}
#endif
#endif
}

void time_SystemTime_sleep_ms(u64 millis) {
    time_SystemTime_sleep(time_Duration_fromMillis(millis));
}

void time_SystemTime_sleep_us(u64 micros) {
    time_SystemTime_sleep(time_Duration_fromMicros(micros));
}

void time_SystemTime_sleep_ns(u32 nanos) {
    time_SystemTime_sleep(time_Duration_fromNanos(nanos));
}

time_Instant time_Instant_now(void) {
    time_SystemTime current;
    QueryPerformanceCounter(&current);
    return makeWith(time_Instant, .time_ = current);
}

time_Duration time_Instant_elapsed(time_Instant start) {
    return time_Instant_durationSince(time_Instant_now(), start);
}

time_Duration time_Instant_durationSince(time_Instant start, time_Instant earlier) {
    time_SystemTime__init();
    u64 diff = start.time_.QuadPart - earlier.time_.QuadPart;

    f64 secs_f = (f64)diff * time_SystemTime__s_frequency_inverse;
    u64 secs   = (u64)secs_f;
    u32 nanos  = (u32)((secs_f - (f64)secs) * (f64)time_nanos_per_sec);

    return time_Duration_from(secs, nanos);
}

u64 time_Instant_toNanos(time_Instant ins) {
    return (u64)((f64)ins.time_.QuadPart * time_SystemTime__s_frequency_inverse);
}

bool time_Instant_eq(time_Instant lhs, time_Instant rhs) {
    return lhs.time_.QuadPart == rhs.time_.QuadPart;
}

bool time_Instant_ne(time_Instant lhs, time_Instant rhs) {
    return lhs.time_.QuadPart != rhs.time_.QuadPart;
}

bool time_Instant_lt(time_Instant lhs, time_Instant rhs) {
    return lhs.time_.QuadPart < rhs.time_.QuadPart;
}

bool time_Instant_le(time_Instant lhs, time_Instant rhs) {
    return lhs.time_.QuadPart <= rhs.time_.QuadPart;
}

bool time_Instant_gt(time_Instant lhs, time_Instant rhs) {
    return lhs.time_.QuadPart > rhs.time_.QuadPart;
}

bool time_Instant_ge(time_Instant lhs, time_Instant rhs) {
    return lhs.time_.QuadPart >= rhs.time_.QuadPart;
}

bool time_Instant_isValid(time_Instant i) {
    return 0 < i.time_.QuadPart;
}

#endif /* defined(_WIN32) || defined(_WIN64) */


// void time_SystemTime_sleep_ns(u64 nanos) {
//     // Add threshold for very small durations
//     if (nanos < 100) {
//         return; // Too small to sleep meaningfully
//     }

//     time_SystemTime start_time = time_SystemTime_now();
//     u64        target     = start_time + ((nanos * time_SystemTime__s_performance_frequency) / time_nanos_per_sec);

//     // Increase Sleep threshold for better CPU utilization
//     u64 ms_remaining = nanos / time_nanos_per_milli;
//     if (5 < ms_remaining) {               // Increased threshold
//         Sleep((DWORD)(ms_remaining - 1)); // Sleep slightly less to account for overhead
//     }

//     // Use _mm_pause() for shorter spin-waits on x86
//     time_SystemTime current_time = make(time_SystemTime);
//     do {
// #if defined(_M_IX86) || defined(_M_X64)
//         _mm_pause(); // Reduce power consumption during spin-wait
// #endif
//         QueryPerformanceCounter(&current_time);
//     } while ((u64)current_time < target);
// }

// void time_SystemTime_sleep_ns(u64 nanos) {
//     time_SystemTime start_time = time_SystemTime_now();
//     u64             target     = start_time + ((nanos * time_SystemTime__s_performance_frequency) / time_nanos_per_sec);

//     // Use larger sleep for most of the duration
//     u64 ms_remaining = nanos / time_nanos_per_milli;
//     if (ms_remaining > 1) {
//         Sleep((DWORD)(ms_remaining - 1));
//     }

//     // Spin wait for precise timing
//     time_SystemTime current_time = make(time_SystemTime);
//     while (true) {
//         QueryPerformanceCounter(&current_time);
//         if (target <= (u64)current_time) {
//             break;
//         }
// #if defined(_M_IX86) || defined(_M_X64)
//         _mm_pause(); // Reduce power usage during spin
// #endif
//     }
// }

// static inline BOOLEAN precise_nanosleep(LONGLONG nanoseconds) {
//     HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
//     if (!timer) {
//         return FALSE;
//     }

//     // Convert to 100ns units and negate for relative time
//     LARGE_INTEGER due_time;
//     due_time = -(nanoseconds / 100);

//     if (SetWaitableTimer(timer, &due_time, 0, NULL, NULL, FALSE)) {
//         WaitForSingleObject(timer, INFINITE);
//         CloseHandle(timer);
//         return TRUE;
//     }

//     CloseHandle(timer);
//     return FALSE;
// }

// void time_SystemTime_sleep_ns(u64 nanos) {
//     bool result = precise_nanosleep((LONGLONG)nanos);
//     debug_assert(result);
// }

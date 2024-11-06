#if defined(_WIN32) || defined(_WIN64)

#include "../include/dh/time/time.h"


static SystemTime SystemTime__s_performance_frequency = { 0 };
static bool       SystemTime__s_frequency_initialized = false;
static void       SystemTime__initFrequency(void) {
    if (SystemTime__s_frequency_initialized) { return; }
    QueryPerformanceFrequency(&SystemTime__s_performance_frequency);
    SystemTime__s_frequency_initialized = true;
}

SystemTime SystemTime_now(void) {
    SystemTime sys_time = make(SystemTime);
    SystemTime__initFrequency();
    QueryPerformanceCounter(&sys_time);
    return sys_time;
}

f64 SystemTime_toSecs(SystemTime time) {
    SystemTime__initFrequency();
    return (f64)time.QuadPart / (f64)SystemTime__s_performance_frequency.QuadPart;
}

f64 SystemTime_toMillis(SystemTime time) {
    SystemTime__initFrequency();
    return (f64)time.QuadPart * (f64)Time_millis_per_sec / (f64)SystemTime__s_performance_frequency.QuadPart;
}

f64 SystemTime_toMicros(SystemTime time) {
    SystemTime__initFrequency();
    return (f64)time.QuadPart * (f64)Time_micros_per_sec / (f64)SystemTime__s_performance_frequency.QuadPart;
}

u64 SystemTime_toNanos(SystemTime time) {
    SystemTime__initFrequency();
    return (u64)time.QuadPart * (u64)Time_nanos_per_sec / (u64)SystemTime__s_performance_frequency.QuadPart;
}

void SystemTime_sleep(Duration duration) {
    SystemTime_sleep_ns((duration.secs * Time_nanos_per_sec) + duration.nanos);
}

void SystemTime_sleep_s(f64 secs) {
    SystemTime_sleep_ns((u64)(secs * Time_nanos_per_sec));
}

void SystemTime_sleep_ms(f64 millis) {
    SystemTime_sleep_ns((u64)(millis * Time_nanos_per_milli));
}

void SystemTime_sleep_us(f64 micros) {
    SystemTime_sleep_ns((u64)(micros * Time_nanos_per_micro));
}

// void SystemTime_sleep_ns(u64 nanos) {
//     // Add threshold for very small durations
//     if (nanos < 100) {
//         return; // Too small to sleep meaningfully
//     }

//     SystemTime start_time = SystemTime_now();
//     u64        target     = start_time.QuadPart + ((nanos * SystemTime__s_performance_frequency.QuadPart) / Time_nanos_per_sec);

//     // Increase Sleep threshold for better CPU utilization
//     u64 ms_remaining = nanos / Time_nanos_per_milli;
//     if (5 < ms_remaining) {               // Increased threshold
//         Sleep((DWORD)(ms_remaining - 1)); // Sleep slightly less to account for overhead
//     }

//     // Use _mm_pause() for shorter spin-waits on x86
//     SystemTime current_time = make(SystemTime);
//     do {
// #if defined(_M_IX86) || defined(_M_X64)
//         _mm_pause(); // Reduce power consumption during spin-wait
// #endif
//         QueryPerformanceCounter(&current_time);
//     } while ((u64)current_time.QuadPart < target);
// }

void SystemTime_sleep_ns(u64 nanos) {
    SystemTime start_time = SystemTime_now();
    u64        target     = start_time.QuadPart + ((nanos * SystemTime__s_performance_frequency.QuadPart) / Time_nanos_per_sec);

    // Use larger sleep for most of the duration
    u64 ms_remaining = nanos / Time_nanos_per_milli;
    if (ms_remaining > 1) {
        Sleep((DWORD)(ms_remaining - 1));
    }

    // Spin wait for precise timing
    SystemTime current_time = make(SystemTime);
    while (true) {
        QueryPerformanceCounter(&current_time);
        if (target <= (u64)current_time.QuadPart) {
            break;
        }
#if defined(_M_IX86) || defined(_M_X64)
        _mm_pause(); // Reduce power usage during spin
#endif
    }
}

// static inline BOOLEAN precise_nanosleep(LONGLONG nanoseconds) {
//     HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
//     if (!timer) {
//         return FALSE;
//     }

//     // Convert to 100ns units and negate for relative time
//     LARGE_INTEGER due_time;
//     due_time.QuadPart = -(nanoseconds / 100);

//     if (SetWaitableTimer(timer, &due_time, 0, NULL, NULL, FALSE)) {
//         WaitForSingleObject(timer, INFINITE);
//         CloseHandle(timer);
//         return TRUE;
//     }

//     CloseHandle(timer);
//     return FALSE;
// }

// void SystemTime_sleep_ns(u64 nanos) {
//     bool result = precise_nanosleep((LONGLONG)nanos);
//     debug_assert(result);
// }

Instant Instant_now(void) {
    return makeWith(Instant, .time_ = SystemTime_now());
}

Duration Instant_elapsed(Instant start) {
    return Instant_durationSince(Instant_now(), start);
}

Duration Instant_durationSince(Instant start, Instant earlier) {
    SystemTime__initFrequency();
    u64 diff      = start.time_.QuadPart - earlier.time_.QuadPart;
    u64 secs      = diff / SystemTime__s_performance_frequency.QuadPart;
    u64 remainder = diff % SystemTime__s_performance_frequency.QuadPart;
    u32 nanos     = (u32)((remainder * Time_nanos_per_sec) / SystemTime__s_performance_frequency.QuadPart);
    return Duration_from(secs, nanos);
}

bool Instant_eq(Instant lhs, Instant rhs) {
    return lhs.time_.QuadPart == rhs.time_.QuadPart;
}

bool Instant_ne(Instant lhs, Instant rhs) {
    return lhs.time_.QuadPart != rhs.time_.QuadPart;
}

bool Instant_lt(Instant lhs, Instant rhs) {
    return lhs.time_.QuadPart < rhs.time_.QuadPart;
}

bool Instant_le(Instant lhs, Instant rhs) {
    return lhs.time_.QuadPart <= rhs.time_.QuadPart;
}

bool Instant_gt(Instant lhs, Instant rhs) {
    return lhs.time_.QuadPart > rhs.time_.QuadPart;
}

bool Instant_ge(Instant lhs, Instant rhs) {
    return lhs.time_.QuadPart >= rhs.time_.QuadPart;
}

bool Instant_isValid(Instant i) {
    return 0 < i.time_.QuadPart;
}

#endif /* defined(_WIN32) || defined(_WIN64) */

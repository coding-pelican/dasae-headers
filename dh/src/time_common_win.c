#include "dh/time.h"

#if BUILTIN_PLTF_WINDOWS && (BUILTIN_PLTF_32BIT || BUILTIN_PLTF_64BIT)

#define time_intervals_per_sec (10000000ULL)

void time_sleep(time_Duration duration) {
    HANDLE timer = CreateWaitableTimerExW(
        null,
        null,
        CREATE_WAITABLE_TIMER_HIGH_RESOLUTION,
        TIMER_ALL_ACCESS
    );
    if (timer == null) {
        Sleep((DWORD)((duration.secs_ * time_millis_per_sec) + (duration.nanos_ / time_nanos_per_milli)));
        return;
    }

    LARGE_INTEGER li = cleared();
    // Convert to negative 100-nanosecond intervals for relative time
    li.QuadPart      = -((LONGLONG)((ULONGLONG)duration.secs_ * time_intervals_per_sec + (duration.nanos_ / 100)));

    if (SetWaitableTimer(timer, &li, 0, null, null, false)) {
        WaitForSingleObject(timer, INFINITE);
    } else {
        Sleep((DWORD)((duration.secs_ * time_millis_per_sec) + (duration.nanos_ / time_nanos_per_milli)));
    }

    CloseHandle(timer);
}

void time_sleepSecs(u64 secs) {
    time_sleep(time_Duration_fromSecs(secs));
}

void time_sleepMillis(u64 millis) {
    time_sleep(time_Duration_fromMillis(millis));
}

void time_sleepMicros(u64 micros) {
    time_sleep(time_Duration_fromMicros(micros));
}

void time_sleepNanos(u32 nanos) {
    time_sleep(time_Duration_fromNanos(nanos));
}

#endif /* BUILTIN_PLTF_WINDOWS && (BUILTIN_PLTF_32BIT || BUILTIN_PLTF_64BIT) */

// #define SUPPORT_BUSY_WAIT_LOOP
// // Wait for some time (stop program execution)
// // NOTE: Sleep() granularity could be around 10 ms, it means, Sleep() could
// // take longer than expected... for that reason we use the busy wait loop
// // Ref: http://stackoverflow.com/questions/43057578/c-programming-win32-games-sleep-taking-longer-than-expected
// // Ref: http://www.geisswerks.com/ryan/FAQS/timing.html --> All about timing on Win32!
// void time_sleep_s_f64(f64 secs) {
//     if (secs < 0) { return; /* Security check */ }

// #if defined(SUPPORT_BUSY_WAIT_LOOP) || defined(SUPPORT_PARTIAL_BUSY_WAIT_LOOP)
//     f64 dest_time = time_now_f64() + secs;
// #endif

// #if defined(SUPPORT_BUSY_WAIT_LOOP)
//     while (time_now_f64() < dest_time) {}
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
//     while (time_now_f64() < dest_time) {}
// #endif
// #endif
// }

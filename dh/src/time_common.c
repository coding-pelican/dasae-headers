#include "dh/time.h"

fn_(time_sleep(time_Duration duration), void) {
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
    HANDLE timer = CreateWaitableTimerExW(
        null,
        null,
        CREATE_WAITABLE_TIMER_HIGH_RESOLUTION,
        TIMER_ALL_ACCESS
    );
    if (timer == null) {
        Sleep((DWORD)((duration.secs * time_millis_per_sec) + (duration.nanos / time_nanos_per_milli)));
        return;
    }

    LARGE_INTEGER li = cleared();
    // Convert to negative 100-nanosecond intervals for relative time
    li.QuadPart      = -((LONGLONG)((ULONGLONG)duration.secs * time_SysTime_intervals_per_sec + (duration.nanos / 100)));

    if (SetWaitableTimer(timer, &li, 0, null, null, false)) {
        WaitForSingleObject(timer, INFINITE);
    } else {
        Sleep((DWORD)((duration.secs * time_millis_per_sec) + (duration.nanos / time_nanos_per_milli)));
    }

    CloseHandle(timer);
#else  /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
    struct timespec req = {
        .tv_sec  = duration.secs,
        .tv_nsec = duration.nanos
    };
    struct timespec rem = {};

    // Use nanosleep for Unix platforms
    while (nanosleep(&req, &rem) == -1) {
        // If interrupted by signal, retry with remaining time
        req = rem;
    }
#endif /* bti_plat_windows && (bti_plat_32bit || bti_plat_64bit) */
}

fn_(time_sleepSecs(u64 secs), void) {
    time_sleep(time_Duration_fromSecs(secs));
}

fn_(time_sleepMillis(u64 millis), void) {
    time_sleep(time_Duration_fromMillis(millis));
}

fn_(time_sleepMicros(u64 micros), void) {
    time_sleep(time_Duration_fromMicros(micros));
}

fn_(time_sleepNanos(u32 nanos), void) {
    time_sleep(time_Duration_fromNanos(nanos));
}

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

//     // NOTE: Use nanosleep() on Unix platforms... usleep() it's $deprecated
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

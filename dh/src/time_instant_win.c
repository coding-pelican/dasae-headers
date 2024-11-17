// #if defined(_WIN32) || defined(_WIN64)

// #include "dh/time/cfg.h"
// #include "dh/time/common.h"
// #include "dh/time/system_time.h"
// #include "dh/time/instant.h"
// #include "dh/time/duration.h"


// time_Instant time_Instant_now(void) {
//     return makeWith(time_Instant, time_SysTime_now());
// }

// time_Duration time_Instant_elapsed(time_Instant ins) {
//     return time_Instant_durationSince(time_Instant_now(), ins);
// }

// time_Duration time_Instant_durationSince(time_Instant ins, time_Instant earlier) {
//     u64 diff = ins.time_.QuadPart - earlier.time_.QuadPart;

//     f64 secs_f = (f64)diff * time_SysTime_frequency_inv();
//     u64 secs   = (u64)secs_f;
//     u32 nanos  = (u32)((secs_f - (f64)secs) * (f64)time_NANOS_PER_SEC);

//     return time_Duration_from(secs, nanos);
// }

// u64 time_Instant_toNanos(time_Instant ins) {
//     return (u64)((f64)ins.time_.QuadPart * time_SysTime_frequency_inv());
// }

// bool time_Instant_eq(time_Instant lhs, time_Instant rhs) {
//     return lhs.time_.QuadPart == rhs.time_.QuadPart;
// }

// bool time_Instant_ne(time_Instant lhs, time_Instant rhs) {
//     return lhs.time_.QuadPart != rhs.time_.QuadPart;
// }

// bool time_Instant_lt(time_Instant lhs, time_Instant rhs) {
//     return lhs.time_.QuadPart < rhs.time_.QuadPart;
// }

// bool time_Instant_le(time_Instant lhs, time_Instant rhs) {
//     return lhs.time_.QuadPart <= rhs.time_.QuadPart;
// }

// bool time_Instant_gt(time_Instant lhs, time_Instant rhs) {
//     return lhs.time_.QuadPart > rhs.time_.QuadPart;
// }

// bool time_Instant_ge(time_Instant lhs, time_Instant rhs) {
//     return lhs.time_.QuadPart >= rhs.time_.QuadPart;
// }

// bool time_Instant_isValid(time_Instant i) {
//     return 0 < i.time_.QuadPart;
// }

// #endif /* defined(_WIN32) || defined(_WIN64) */


// // void time_SysTime_sleep_ns(u64 nanos) {
// //     // Add threshold for very small durations
// //     if (nanos < 100) {
// //         return; // Too small to sleep meaningfully
// //     }

// //     time_SysTime start_time = time_SysTime_now();
// //     u64        target     = start_time + ((nanos * time_SysTime__s_performance_frequency) / time_NANOS_PER_SEC);

// //     // Increase Sleep threshold for better CPU utilization
// //     u64 ms_remaining = nanos / time_NANOS_PER_MILLI;
// //     if (5 < ms_remaining) {               // Increased threshold
// //         Sleep((DWORD)(ms_remaining - 1)); // Sleep slightly less to account for overhead
// //     }

// //     // Use _mm_pause() for shorter spin-waits on x86
// //     time_SysTime current_time = make(time_SysTime);
// //     do {
// // #if defined(_M_IX86) || defined(_M_X64)
// //         _mm_pause(); // Reduce power consumption during spin-wait
// // #endif
// //         QueryPerformanceCounter(&current_time);
// //     } while ((u64)current_time < target);
// // }

// // void time_SysTime_sleep_ns(u64 nanos) {
// //     time_SysTime start_time = time_SysTime_now();
// //     u64             target     = start_time + ((nanos * time_SysTime__s_performance_frequency) / time_NANOS_PER_SEC);

// //     // Use larger sleep for most of the duration
// //     u64 ms_remaining = nanos / time_NANOS_PER_MILLI;
// //     if (ms_remaining > 1) {
// //         Sleep((DWORD)(ms_remaining - 1));
// //     }

// //     // Spin wait for precise timing
// //     time_SysTime current_time = make(time_SysTime);
// //     while (true) {
// //         QueryPerformanceCounter(&current_time);
// //         if (target <= (u64)current_time) {
// //             break;
// //         }
// // #if defined(_M_IX86) || defined(_M_X64)
// //         _mm_pause(); // Reduce power usage during spin
// // #endif
// //     }
// // }

// // static inline BOOLEAN precise_nanosleep(LONGLONG nanoseconds) {
// //     HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
// //     if (!timer) {
// //         return FALSE;
// //     }

// //     // Convert to 100ns units and negate for relative time
// //     LARGE_INTEGER due_time;
// //     due_time = -(nanoseconds / 100);

// //     if (SetWaitableTimer(timer, &due_time, 0, NULL, NULL, FALSE)) {
// //         WaitForSingleObject(timer, INFINITE);
// //         CloseHandle(timer);
// //         return TRUE;
// //     }

// //     CloseHandle(timer);
// //     return FALSE;
// // }

// // void time_SysTime_sleep_ns(u64 nanos) {
// //     bool result = precise_nanosleep((LONGLONG)nanos);
// //     debug_assert(result);
// // }

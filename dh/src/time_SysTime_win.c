// #if defined(_WIN32) || defined(_WIN64)

// #include "dh/time/cfg.h"
// #include "dh/time/common.h"
// #include "dh/time/system_time.h"
// #include "dh/time/instant.h"
// #include "dh/time/duration.h"
// #include "dh/cmp.h"

// static time_SysTime time_SysTime__s_performance_frequency     = make(time_SysTime);
// static f64             time_SysTime__s_frequency_inverse         = 0.0;
// static time_SysTime time_SysTime__s_offset_frequency_to_value = make(time_SysTime);
// static bool            time_SysTime__s_frequency_initialized     = false;

// static void __attribute__((constructor)) time_SysTime__init(void) {
//     if (time_SysTime__s_frequency_initialized) { return; }
//     QueryPerformanceFrequency(&time_SysTime__s_performance_frequency);
//     time_SysTime__s_frequency_inverse         = 1.0 / (f64)time_SysTime__s_performance_frequency.QuadPart;
//     time_SysTime__s_frequency_initialized     = true;
//     time_SysTime__s_offset_frequency_to_value = time_SysTime_value();
// }

// static_inline bool time_SysTime_checkedCmp(time_SysTime lhs, time_SysTime rhs, EOrd* out_result) {
//     if (!out_result) { return false; }
//     *out_result = time_SysTime_cmp(&lhs, &rhs);
//     return true;
// }

// static_inline bool time_SysTime_checkedDurationCalc(time_SysTime lhs, time_SysTime rhs, time_Duration* out_duration, bool allow_negative) {
//     time_SysTime__init();
//     time_SysTime diff = make(time_SysTime);
//     diff.QuadPart        = lhs.QuadPart - rhs.QuadPart;

//     if (!allow_negative && diff.QuadPart < 0) { return false; }
//     if (out_duration) {
//         out_duration->nanos = (u64)((f64)(diff.QuadPart * (LONGLONG)time_NANOS_PER_SEC)
//                                     * time_SysTime_frequency_inv());
//     }
//     return true;
// }

// static_inline bool time_SysTime_checkedArith(time_SysTime t, time_Duration d, time_SysTime* out_time, bool is_add) {
//     time_SysTime__init();
//     u64 ticks = (u64)(d.nanos * time_SysTime_frequency_inv());

//     if (is_add) {
//         if (t.QuadPart > LLONG_MAX - (LONGLONG)ticks) { return false; }
//         out_time->QuadPart = t.QuadPart + (LONGLONG)ticks;
//     } else {
//         if (t.QuadPart < (LONGLONG)ticks) { return false; }
//         out_time->QuadPart = t.QuadPart - (LONGLONG)ticks;
//     }
//     return true;
// }

// EOrd time_SysTime_cmp(const time_SysTime* lhs, const time_SysTime* rhs) {
//     if (lhs->QuadPart < rhs->QuadPart) { return EOrd_less; }
//     if (lhs->QuadPart > rhs->QuadPart) { return EOrd_greater; }
//     return EOrd_equal;
// }

// time_SysTime time_SysTime_frequency(void) {
//     return time_SysTime__s_performance_frequency;
// }

// f64 time_SysTime_frequency_inv(void) {
//     return time_SysTime__s_frequency_inverse;
// }

// time_SysTime time_SysTime_value(void) {
//     time_SysTime value = make(time_SysTime);
//     QueryPerformanceCounter(&value);
//     return value;
// }

// time_SysTime time_SysTime_offset(void) {
//     return time_SysTime__s_offset_frequency_to_value;
// }

// time_SysTime time_SysTime_now(void) {
//     time_SysTime time   = time_SysTime_value();
//     time_SysTime result = make(time_SysTime);
//     result.QuadPart        = (LONGLONG)((f64)(time.QuadPart - time_SysTime_offset().QuadPart) * time_SysTime__s_frequency_inverse);
//     return result;
// }

// f64 time_SysTime_now_f64(void) {
//     time_SysTime time = time_SysTime_value();
//     return ((f64)(time.QuadPart - time_SysTime_offset().QuadPart) * time_SysTime__s_frequency_inverse);
// }

// time_Duration time_SysTime_elapsed(time_SysTime t) {
//     return time_SysTime_durationSince(time_SysTime_now(), t);
// }

// bool time_SysTime_checkedElapsed(time_SysTime t, time_Duration* out_duration) {
//     return time_SysTime_checkedDurationSince(time_SysTime_now(), t, out_duration);
// }

// time_Duration time_SysTime_durationSince(time_SysTime t, time_SysTime earlier) {
//     time_Duration duration = make(time_Duration);
//     time_SysTime_checkedDurationCalc(t, earlier, &duration, false);
//     return duration;
// }

// bool time_SysTime_checkedDurationSince(time_SysTime t, time_SysTime earlier, time_Duration* const out_duration) {
//     return time_SysTime_checkedDurationCalc(t, earlier, out_duration, false);
// }

// // Time arithmetic functions
// time_SysTime time_SysTime_addDuration(time_SysTime t, time_Duration d) {
//     time_SysTime result = make(time_SysTime);
//     time_SysTime_checkedArith(t, d, &result, true);
//     return result;
// }

// bool time_SysTime_checkedAddDuration(time_SysTime t, time_Duration d, time_SysTime* out_time) {
//     return time_SysTime_checkedArith(t, d, out_time, true);
// }

// time_SysTime time_SysTime_subDuration(time_SysTime t, time_Duration d) {
//     time_SysTime result = make(time_SysTime);
//     time_SysTime_checkedArith(t, d, &result, false);
//     return result;
// }

// bool time_SysTime_checkedSubDuration(time_SysTime t, time_Duration d, time_SysTime* out_time) {
//     return time_SysTime_checkedArith(t, d, out_time, false);
// }

// // Comparison functions
// bool time_SysTime_eq(time_SysTime lhs, time_SysTime rhs) {
//     EOrd result = -1;
//     time_SysTime_checkedCmp(lhs, rhs, &result);
//     return result == EOrd_equal;
// }

// bool time_SysTime_lt(time_SysTime lhs, time_SysTime rhs) {
//     EOrd result = -1;
//     time_SysTime_checkedCmp(lhs, rhs, &result);
//     return result == EOrd_less;
// }

// bool time_SysTime_le(time_SysTime lhs, time_SysTime rhs) {
//     return time_SysTime_lt(lhs, rhs) || time_SysTime_eq(lhs, rhs);
// }

// bool time_SysTime_gt(time_SysTime lhs, time_SysTime rhs) {
//     EOrd result = -1;
//     time_SysTime_checkedCmp(lhs, rhs, &result);
//     return result == EOrd_greater;
// }

// bool time_SysTime_ge(time_SysTime lhs, time_SysTime rhs) {
//     return time_SysTime_gt(lhs, rhs) || time_SysTime_eq(lhs, rhs);
// }

// void time_SysTime_sleep(time_Duration duration) {
//     time_SysTime_sleep_s_f64(time_Duration_asSecs_f64(duration));
// }

// void time_SysTime_sleep_s(u64 secs) {
//     time_SysTime_sleep(time_Duration_from(secs, 0));
// }

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
//     Sleep((u16)(sleep_secs * time_MILLIS_PER_SEC));
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

// void time_SysTime_sleep_ms(u64 millis) {
//     time_SysTime_sleep(time_Duration_fromMillis(millis));
// }

// void time_SysTime_sleep_us(u64 micros) {
//     time_SysTime_sleep(time_Duration_fromMicros(micros));
// }

// void time_SysTime_sleep_ns(u32 nanos) {
//     time_SysTime_sleep(time_Duration_fromNanos(nanos));
// }

// #endif /* defined(_WIN32) || defined(_WIN64) */

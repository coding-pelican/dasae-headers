#include "dh/time.h"

/*========== Internal Declarations ==========================================*/

/// 100-nanosecond intervals per second (for Windows waitable timer).
#define time__intervals_per_sec (lit_n$(u64)(10, 000, 000ull))

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $maybe_unused)
    $static fn_((time__unsupported_sleep(time_Duration duration))(void));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always)
    $static fn_((time__windows_sleep(time_Duration duration))(void));
));
pp_if_(pp_or(plat_is_linux, plat_is_darwin))(pp_then_(
    $attr($inline_always)
    $static fn_((time__unix_sleep(time_Duration duration))(void));
));

$static let time__sleep = pp_if_(plat_is_windows)(
    pp_then_(time__windows_sleep),
    pp_else_(pp_if_(pp_or(plat_is_linux, plat_is_darwin))(
        pp_then_(time__unix_sleep),
        pp_else_(time__unsupported_sleep)
    )));

/*========== External Definitions ===========================================*/

fn_((time_sleep(time_Duration duration))(void)) {
    time__sleep(duration);
};

fn_((time_sleepSecs(u64 secs))(void)) {
    time_sleep(time_Duration_fromSecs(secs));
};

fn_((time_sleepMillis(u64 millis))(void)) {
    time_sleep(time_Duration_fromMillis(millis));
};

fn_((time_sleepMicros(u64 micros))(void)) {
    time_sleep(time_Duration_fromMicros(micros));
};

fn_((time_sleepNanos(u32 nanos))(void)) {
    time_sleep(time_Duration_fromNanos(nanos));
};

/*========== Internal Definitions ===========================================*/

/* --- Unsupported --- */

fn_((time__unsupported_sleep(time_Duration duration))(void)) {
    let_ignore = duration;
};

/* --- Windows --- */

#if plat_is_windows
#include "dh/os/windows/handle.h"
#include "dh/os/windows/sync.h"

fn_((time__windows_sleep(time_Duration duration))(void) $guard) {
    let timer = CreateWaitableTimerExW(
        null, null,
        CREATE_WAITABLE_TIMER_HIGH_RESOLUTION,
        TIMER_ALL_ACCESS
    );
    if (timer == null) {
        // Fallback to Sleep() if high-resolution timer unavailable
        let ms = as$(DWORD)(duration.secs * time_millis_per_sec + duration.nanos / time_nanos_per_milli);
        Sleep(ms);
        return;
    }
    defer_(CloseHandle(timer));

    // Convert to negative 100-nanosecond intervals for relative time
    LARGE_INTEGER li = cleared();
    li.QuadPart = -as$(LONGLONG)(duration.secs * time__intervals_per_sec + duration.nanos / 100);

    if (SetWaitableTimer(timer, &li, 0, null, null, false)) {
        WaitForSingleObject(timer, INFINITE);
    } else {
        let ms = as$(DWORD)(duration.secs * time_millis_per_sec + duration.nanos / time_nanos_per_milli);
        Sleep(ms);
    }
} $unguarded_(fn);
#endif /* plat_is_windows */

/* --- Unix (Linux, Darwin) --- */

#if plat_is_linux || plat_is_darwin
fn_((time__unix_sleep(time_Duration duration))(void)) {
    struct timespec req = {
        .tv_sec = duration.secs,
        .tv_nsec = duration.nanos,
    };
    struct timespec rem = cleared();
    while (nanosleep(&req, &rem) == -1) {
        req = rem;
    }
};
#endif /* plat_is_linux || plat_is_darwin */

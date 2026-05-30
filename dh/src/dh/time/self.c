#include "dh/time.h"
#if plat_is_linux
#include "dh/sys/call/linux.h"
#endif

/*========== Internal Declarations ==========================================*/

pp_if_(pp_true)(pp_then_(
    $attr($must_check $inline_always $maybe_unused)
    $static fn_((time__unsupported_sleep(time_Dur duration))(time_sleep_E$void));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($must_check $inline_always)
    $static fn_((time__windows_sleep(time_Dur duration))(time_sleep_E$void));
));
pp_if_(plat_is_linux)(pp_then_(
    $attr($must_check $inline_always)
    $static fn_((time__linux_sleep(time_Dur duration))(time_sleep_E$void));
));

$static let time__sleep = pp_if_(plat_is_windows)(
    pp_then_(time__windows_sleep),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(time__linux_sleep),
        pp_else_(time__unsupported_sleep)
    )));

/*========== External Definitions ===========================================*/

fn_((time_sleep(time_Dur duration))(time_sleep_E$void)) {
    return time__sleep(duration);
};

fn_((time_sleepSecs(u64 secs))(time_sleep_E$void)) {
    return time_sleep(time_Dur_fromSecs(secs));
};

fn_((time_sleepMillis(u64 millis))(time_sleep_E$void)) {
    return time_sleep(time_Dur_fromMillis(millis));
};

fn_((time_sleepMicros(u64 micros))(time_sleep_E$void)) {
    return time_sleep(time_Dur_fromMicros(micros));
};

fn_((time_sleepNanos(u32 nanos))(time_sleep_E$void)) {
    return time_sleep(time_Dur_fromNanos(nanos));
};

/*========== Internal Definitions ===========================================*/

/* --- Unsupported --- */

fn_((time__unsupported_sleep(time_Dur duration))(time_sleep_E$void) $scope) {
    let_ignore = duration;
    return_err(E_cause$time_Unsupported());
} $unscoped(fn);

/* --- Windows --- */

#if plat_is_windows
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/sync.h"

/// 100-nanosecond intervals per second (for waitable timer)
#define time__windows_intervals_per_sec (n$(u64)(10, 000, 000ull))

/* NOTE: should we support low-resolution timer version? */
fn_((time__windows_sleep(time_Dur duration))(time_sleep_E$void) $guard) {
    let timer = CreateWaitableTimerExW(
        null, null,
        CREATE_WAITABLE_TIMER_HIGH_RESOLUTION,
        TIMER_ALL_ACCESS
    );
    if (timer == null) {
        // Fallback to Sleep() if high-resolution timer unavailable
        let ms = as$(DWORD)(duration.secs * time_millis_per_sec + duration.nanos / time_nanos_per_milli);
        return_ok_void(Sleep(ms));
    }
    defer_(CloseHandle(timer));

    // Convert to negative 100-nanosecond intervals for relative time
    LARGE_INTEGER li = cleared();
    li.QuadPart = -as$(LONGLONG)(duration.secs * time__windows_intervals_per_sec + duration.nanos / 100);

    if (SetWaitableTimer(timer, &li, 0, null, null, false)) {
        WaitForSingleObject(timer, INFINITE);
    } else {
        let ms = as$(DWORD)(duration.secs * time_millis_per_sec + duration.nanos / time_nanos_per_milli);
        Sleep(ms);
    }
    return_ok({});
} $unguarded(fn);
#endif /* plat_is_windows */

/* --- Linux --- */

#if plat_is_linux
fn_((time__linux_sleep(time_Dur duration))(time_sleep_E$void) $scope) {
    time_UnixTimespec req = cleared();
    req.tv_sec = as$(TypeOf(req.tv_sec))(duration.secs);
    req.tv_nsec = as$(TypeOf(req.tv_nsec))(duration.nanos);
    time_UnixTimespec rem = cleared();
    while (sys_call_linux_nanosleep(&req, &rem) == -sys_call_linux_EINTR) {
        req = rem;
    }
    return_ok({});
} $unscoped(fn);
#endif /* plat_is_linux */

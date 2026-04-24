#include "internal.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((time__direct__unsupported_sleep(time_Dur dur))(Sched_Cancelable$void));
pp_if_(plat_is_windows)(pp_then_(
    $static fn_((time__direct__windows_sleep(time_Dur dur))(Sched_Cancelable$void));
));
pp_if_(plat_based_unix)(pp_then_(
    $static fn_((time__direct__unix_sleep(time_Dur dur))(Sched_Cancelable$void));
));

$static let time__direct__sleep = pp_if_(plat_is_windows)(
    pp_then_(time__direct__windows_sleep),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(time__direct__unix_sleep),
        pp_else_(time__direct__unsupported_sleep)
    ))
);

/*========== External Definitions ===========================================*/

pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always)
    $static fn_((time__windows_fileTimeToU64(FILETIME ft))(u64)) {
        return (as$(u64)(ft.dwHighDateTime) << 32ull) | as$(u64)(ft.dwLowDateTime);
    };

    fn_((time__windows_fromUnixFileTime(FILETIME ft))(time_Inst)) {
        let total = time__windows_fileTimeToU64(ft);
        debug_assert_fmt(total >= time__windows_intervals_to_unix_epoch, "FILETIME is before Unix epoch");
        let unix_intervals = total - time__windows_intervals_to_unix_epoch;
        return time_Inst_from(
            unix_intervals / time__windows_intervals_per_sec,
            as$(u32)((unix_intervals % time__windows_intervals_per_sec) * 100ull)
        );
    };

    fn_((time__windows_fromFileTime(FILETIME ft))(time_Inst)) {
        let total = time__windows_fileTimeToU64(ft);
        return time_Inst_from(
            total / time__windows_intervals_per_sec,
            as$(u32)((total % time__windows_intervals_per_sec) * 100ull)
        );
    };

    fn_((time__windows_qpcFreq(void))(u64)) {
        var_(freq, LARGE_INTEGER) = cleared();
        if (!QueryPerformanceFrequency(&freq)) {
            claim_unreachable_msg("Failed to query performance frequency");
        }
        return as$(u64)(freq.QuadPart);
    };
));
pp_if_(plat_based_unix)(pp_then_(
    fn_((time__unix_fromTimespec(struct timespec ts))(time_Inst)) {
        return time_Inst_from(as$(u64)(ts.tv_sec), as$(u32)(ts.tv_nsec));
    };
));

fn_((time__direct_sleep(time_Dur dur))(Sched_Cancelable$void)) {
    return time__direct__sleep(dur);
};

/*========== Internal Definitions ===========================================*/

fn_((time__direct__unsupported_sleep(time_Dur dur))(Sched_Cancelable$void) $scope) {
    let_ignore = dur;
    return_err(Sched_Cancelable_Canceled());
} $unscoped(fn);

pp_if_(plat_is_windows)(pp_then_(
    fn_((time__direct__windows_sleep(time_Dur dur))(Sched_Cancelable$void) $scope) {
        let millis = dur.secs * time_millis_per_sec + dur.nanos / time_nanos_per_milli;
        Sleep(as$(DWORD)(millis));
        return_ok({});
    } $unscoped(fn);
));

pp_if_(plat_based_unix)(pp_then_(
    fn_((time__direct__unix_sleep(time_Dur dur))(Sched_Cancelable$void) $scope) {
        var_(req, struct timespec) = {
            .tv_sec = as$(time_t)(dur.secs),
            .tv_nsec = as$(long)(dur.nanos),
        };
        var_(rem, struct timespec) = cleared();
        while (nanosleep(&req, &rem) == -1) req = rem;
        return_ok({});
    } $unscoped(fn);
));

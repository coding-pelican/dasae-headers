#include "dh/Thrd/Ftx.h"
#include "dh/time/Instant.h"

/*========== Internal Declarations ==========================================*/

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((Thrd_Ftx__unsupported_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void));
    $attr($inline_always $maybe_unused)
    $static fn_((Thrd_Ftx__unsupported_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(Thrd_Ftx_use_pthread)(pp_then_(
    $attr($inline_always)
    $static fn_((Thrd_Ftx__pthread_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void));
    $attr($inline_always)
    $static fn_((Thrd_Ftx__pthread_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((Thrd_Ftx__windows_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void));
    $attr($inline_always)
    $static fn_((Thrd_Ftx__windows_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_linux)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((Thrd_Ftx__linux_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void));
    $attr($inline_always)
    $static fn_((Thrd_Ftx__linux_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_darwin)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((Thrd_Ftx__darwin_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void));
    $attr($inline_always)
    $static fn_((Thrd_Ftx__darwin_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_wasi)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((Thrd_Ftx__wasi_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void));
    $attr($inline_always)
    $static fn_((Thrd_Ftx__wasi_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));

$static let Thrd_Ftx__wait = pp_if_(Thrd_Ftx_use_pthread)(
    pp_then_(Thrd_Ftx__pthread_wait),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd_Ftx__windows_wait),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd_Ftx__linux_wait),
            pp_else_(pp_if_(plat_is_darwin)(
                pp_then_(Thrd_Ftx__darwin_wait),
                pp_else_(pp_if_(plat_is_wasi)(
                    pp_then_(Thrd_Ftx__wasi_wait),
                    pp_else_(Thrd_Ftx__unsupported_wait)
                ))
            ))
        ))
    )));
$static let Thrd_Ftx__wake = pp_if_(Thrd_Ftx_use_pthread)(
    pp_then_(Thrd_Ftx__pthread_wake),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd_Ftx__windows_wake),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd_Ftx__linux_wake),
            pp_else_(pp_if_(plat_is_darwin)(
                pp_then_(Thrd_Ftx__darwin_wake),
                pp_else_(pp_if_(plat_is_wasi)(
                    pp_then_(Thrd_Ftx__wasi_wake),
                    pp_else_(Thrd_Ftx__unsupported_wake)
                ))
            ))
        ))
    )));

/*========== External Definitions ===========================================*/

$attr($branch_cold)
fn_((Thrd_Ftx_wait(const atom_V$u32* ptr, u32 expect))(void) $scope) {
    return_void(catch_((Thrd_Ftx__wait(ptr, expect, none$((O$time_Duration))))($ignore, claim_unreachable)));
} $unscoped_(fn);

$attr($branch_cold)
fn_((Thrd_Ftx_timedWait(const atom_V$u32* ptr, u32 expect, time_Duration timeout))(Thrd_Ftx_Err$void) $scope) {
    if (time_Duration_isZero(timeout)) {
        if (atom_V_load(ptr, atom_MemOrd_seq_cst) != expect) { return_ok({}); }
        return_err(Thrd_Ftx_Err_Timeout());
    }
    return_(Thrd_Ftx__wait(ptr, expect, some$((O$time_Duration)(timeout))));
} $unscoped_(fn);

$attr($branch_cold)
fn_((Thrd_Ftx_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    if (max_waiters == 0) { return; }
    return Thrd_Ftx__wake(ptr, max_waiters);
};

fn_((Thrd_Ftx_Deadline_init(O$time_Duration expires))(Thrd_Ftx_Deadline)) {
    var deadline = lit0$((Thrd_Ftx_Deadline));
    deadline.timeout = expires;
    if (isSome(expires)) { deadline.started = time_Instant_now(); }
    return deadline;
};

$attr($branch_cold)
fn_((Thrd_Ftx_Deadline_wait(Thrd_Ftx_Deadline* self, const atom_V$u32* ptr, u32 expect))(Thrd_Ftx_Err$void) $scope) {
    let timeout = orelse_((self->timeout)(return_ok_void(Thrd_Ftx_wait(ptr, expect))));
    let elapsed = time_Instant_elapsed(self->started);
    let until_timeout = orelse_((time_Duration_subChkd(timeout, elapsed))(time_Duration_zero));
    return_(Thrd_Ftx_timedWait(ptr, expect, until_timeout));
} $unscoped_(fn);

/*========== Internal Definitions ===========================================*/

/* --- Unsupported --- */

fn_((Thrd_Ftx__unsupported_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void) $scope) {
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd_Ftx__unsupported_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    let_ignore = ptr;
    let_ignore = max_waiters;
};

/* --- Pthreads --- */

#if Thrd_Ftx_use_pthread
fn_((Thrd_Ftx__pthread_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void) $scope) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd_Ftx__pthread_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = max_waiters;
};
#endif /* Thrd_Ftx_use_pthread */

/* --- Windows --- */

#if plat_is_windows
fn_((Thrd_Ftx__windows_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void) $scope) {
    claim_assert_static(TypeInfoPacked_eq(packTypeInfo$(DWORD), packTypeInfo$(u32)));
    let timeout_ms = expr_(u32 $scope)(if_some((timeout)(delay)) {
        let delay_ms = delay.secs * lit_num(1, 000) + delay.nanos / lit_num(1, 000, 000);
        $break_(u32_limit_max < delay_ms ? u32_limit_max : as$(u32)(delay_ms));
    } else_none {
        $break_(u32_limit_max);
    }) $unscoped_(expr);
    let rc = WaitOnAddress(
        ptrQualCast$((volatile P$raw)(&ptr->raw)),
        &expect, sizeOf$(u32),
        timeout_ms
    );
    if (!rc && GetLastError() == ERROR_TIMEOUT) {
        claim_assert(isSome(timeout));
        return_err(Thrd_Ftx_Err_Timeout());
    }
    return_ok({});
} $unscoped_(fn);

fn_((Thrd_Ftx__windows_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    let addr = ptrQualCast$((volatile P$raw)(&ptr->raw));
    claim_assert(max_waiters != 0);
    if (max_waiters == 1) {
        WakeByAddressSingle(addr);
    } else {
        WakeByAddressAll(addr);
    }
};
#endif /* plat_is_windows */

/* --- Linux --- */

#if plat_is_linux
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>

fn_((Thrd_Ftx__linux_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void) $scope) {
    struct timespec ts = { 0 };
    struct timespec* ts_ptr = null;
    if_some((timeout)(delay)) {
        ts.tv_sec = as$(time_t)(delay.secs);
        ts.tv_nsec = as$(long)(delay.nanos);
        ts_ptr = &ts;
    }
    let rc = syscall(SYS_futex, &ptr->raw, FUTEX_WAIT | FUTEX_PRIVATE_FLAG, expect, ts_ptr, null, 0);
    if (rc == -1 && errno == ETIMEDOUT) {
        claim_assert(isSome(timeout));
        return_err(Thrd_Ftx_Err_Timeout());
    }
    return_ok({});
} $unscoped_(fn);

fn_((Thrd_Ftx__linux_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    claim_assert(max_waiters != 0);
    let waiters = (max_waiters < as$(u32)(i32_limit_max)) ? as$(i32)(max_waiters) : i32_limit_max;
    let_ignore = syscall(SYS_futex, &ptr->raw, FUTEX_WAKE | FUTEX_PRIVATE_FLAG, waiters, null, null, 0);
};
#endif /* plat_is_linux */

/* --- Darwin --- */

#if plat_is_darwin
#include <errno.h>
#define UL_COMPARE_AND_WAIT 1
#define ULF_NO_ERRNO 0x01000000
#define ULF_WAKE_ALL 0x00000100
$extern fn_((__ulock_wait(u32 operation, P$raw addr, u64 value, u32 timeout_us))(i32));
$extern fn_((__ulock_wait2(u32 operation, P$raw addr, u64 value, u64 timeout_ns, u64 value2))(i32));
$extern fn_((__ulock_wake(u32 operation, P$raw addr, u64 wake_value))(i32));

fn_((Thrd_Ftx__darwin_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void) $scope) {
    u64 timeout_ns = 0;
    if_some((timeout)(delay)) { timeout_ns = delay.secs * time_nanos_per_sec + delay.nanos; }
    let flags = UL_COMPARE_AND_WAIT | ULF_NO_ERRNO;
    let addr = as$(P$raw)(&ptr->raw);
#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 110001
    let status = __ulock_wait2(flags, addr, as$(u64)(expect), timeout_ns, 0);
#else
    u32 timeout_us = 0;
    bool overflowed = false;
    if (timeout_ns != 0) {
        let us64 = timeout_ns / 1000;
        if (us64 > u32_limit_max) {
            overflowed = true;
            timeout_us = u32_limit_max;
        } else {
            timeout_us = as$(u32)(us64);
        }
    }
    let status = __ulock_wait(flags, addr, as$(u64)(expect), timeout_us);
#endif
    if (status < 0 && (-status) == ETIMEDOUT) {
#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 110001
        return_err(Thrd_Ftx_Err_Timeout());
#else
        if (!overflowed) { return_err(Thrd_Ftx_Err_Timeout()); }
#endif
    }
    return_ok({});
} $unscoped_(fn);

fn_((Thrd_Ftx__darwin_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    let flags = UL_COMPARE_AND_WAIT | ULF_NO_ERRNO | (max_waiters > 1 ? ULF_WAKE_ALL : 0);
    let addr = as$(P$raw)(&ptr->raw);
    while (true) {
        let status = __ulock_wake(flags, addr, 0);
        if (status >= 0 || (-status) != EINTR) { return; }
    }
};
#endif /* plat_is_darwin */

/* --- WASI --- */

#if plat_is_wasi
fn_((Thrd_Ftx__wasi_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void) $scope) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    return_err(Err_Unsupported());
} $unscoped_(fn);

fn_((Thrd_Ftx__wasi_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = max_waiters;
};
#endif /* plat_is_wasi */

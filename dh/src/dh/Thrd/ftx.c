#include "dh/Thrd/ftx.h"
#include "dh/time/Instant.h"
#include "dh/sys/libc/darwin/sync.h"

/*========== Internal Declarations ==========================================*/

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((Thrd_ftx__unsupported_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void));
    $attr($inline_always $maybe_unused)
    $static fn_((Thrd_ftx__unsupported_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(Thrd_ftx_use_pthread)(pp_then_(
    $attr($inline_always)
    $static fn_((Thrd_ftx__pthread_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void));
    $attr($inline_always)
    $static fn_((Thrd_ftx__pthread_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((Thrd_ftx__windows_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void));
    $attr($inline_always)
    $static fn_((Thrd_ftx__windows_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_linux)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((Thrd_ftx__linux_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void));
    $attr($inline_always)
    $static fn_((Thrd_ftx__linux_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_darwin)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((Thrd_ftx__darwin_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void));
    $attr($inline_always)
    $static fn_((Thrd_ftx__darwin_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_wasi)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((Thrd_ftx__wasi_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void));
    $attr($inline_always)
    $static fn_((Thrd_ftx__wasi_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));

$static let Thrd_ftx__wait = pp_if_(Thrd_ftx_use_pthread)(
    pp_then_(Thrd_ftx__pthread_wait),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd_ftx__windows_wait),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd_ftx__linux_wait),
            pp_else_(pp_if_(plat_is_darwin)(
                pp_then_(Thrd_ftx__darwin_wait),
                pp_else_(pp_if_(plat_is_wasi)(
                    pp_then_(Thrd_ftx__wasi_wait),
                    pp_else_(Thrd_ftx__unsupported_wait)
                ))
            ))
        ))
    )));
$static let Thrd_ftx__wake = pp_if_(Thrd_ftx_use_pthread)(
    pp_then_(Thrd_ftx__pthread_wake),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(Thrd_ftx__windows_wake),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(Thrd_ftx__linux_wake),
            pp_else_(pp_if_(plat_is_darwin)(
                pp_then_(Thrd_ftx__darwin_wake),
                pp_else_(pp_if_(plat_is_wasi)(
                    pp_then_(Thrd_ftx__wasi_wake),
                    pp_else_(Thrd_ftx__unsupported_wake)
                ))
            ))
        ))
    )));

/*========== External Definitions ===========================================*/

$attr($branch_cold)
fn_((Thrd_ftx_wait(const atom_V$u32* ptr, u32 expect))(void) $scope) {
    return_void(catch_((Thrd_ftx__wait(ptr, expect, none$((O$time_Duration))))($ignore, claim_unreachable)));
} $unscoped(fn);

$attr($branch_cold)
fn_((Thrd_ftx_timedWait(const atom_V$u32* ptr, u32 expect, time_Duration timeout))(Thrd_ftx_E$void) $scope) {
    if (time_Duration_isZero(timeout)) {
        if (atom_V_load(ptr, atom_MemOrd_seq_cst) != expect) { return_ok({}); }
        return_err(E_cause$ThrdTimeout());
    }
    return_(Thrd_ftx__wait(ptr, expect, some$((O$time_Duration)(timeout))));
} $unscoped(fn);

$attr($branch_cold)
fn_((Thrd_ftx_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    if (max_waiters == 0) { return; }
    return Thrd_ftx__wake(ptr, max_waiters);
};

fn_((Thrd_ftx_Deadline_init(O$time_Duration expires))(Thrd_ftx_Deadline)) {
    var deadline = l0$((Thrd_ftx_Deadline));
    deadline.timeout = expires;
    if (isSome(expires)) { deadline.started = time_Instant_now(); }
    return deadline;
};

$attr($branch_cold)
fn_((Thrd_ftx_Deadline_wait(Thrd_ftx_Deadline* self, const atom_V$u32* ptr, u32 expect))(Thrd_ftx_E$void) $scope) {
    let timeout = orelse_((self->timeout)(return_ok_void(Thrd_ftx_wait(ptr, expect))));
    let elapsed = time_Instant_elapsed(self->started);
    let until_timeout = orelse_((time_Duration_subChkd(timeout, elapsed))(time_Duration_zero));
    return_(Thrd_ftx_timedWait(ptr, expect, until_timeout));
} $unscoped(fn);

/*========== Internal Definitions ===========================================*/

/* --- Unsupported --- */

fn_((Thrd_ftx__unsupported_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void) $scope) {
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    return_err(E_cause$UnsupportedFtx());
} $unscoped(fn);

fn_((Thrd_ftx__unsupported_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    let_ignore = ptr;
    let_ignore = max_waiters;
};

/* --- Pthreads --- */

#if Thrd_ftx_use_pthread
fn_((Thrd_ftx__pthread_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void) $scope) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    return_err(E_cause$UnsupportedFtx());
} $unscoped(fn);

fn_((Thrd_ftx__pthread_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = max_waiters;
};
#endif /* Thrd_ftx_use_pthread */

/* --- Windows --- */

#if plat_is_windows
fn_((Thrd_ftx__windows_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void) $scope) {
    claim_assert_static(TypeInfoPacked_eql(packTypeInfo$(DWORD), packTypeInfo$(u32)));
    let timeout_ms = expr_(u32 $scope)(if_some((timeout)(delay)) {
        let delay_ms = delay.secs * n_(1, 000) + delay.nanos / n_(1, 000, 000);
        $break_(u32_limit_max < delay_ms ? u32_limit_max : as$(u32)(delay_ms));
    } else_none {
        $break_(u32_limit_max);
    }) $unscoped(expr);
    let rc = WaitOnAddress(
        ptrQualCast$((volatile P$raw)(&ptr->raw)), &expect, sizeOf$(u32), timeout_ms);
    if (!rc && GetLastError() == ERROR_TIMEOUT) {
        claim_assert(isSome(timeout));
        return_err(E_cause$ThrdTimeout());
    }
    return_ok({});
} $unscoped(fn);

fn_((Thrd_ftx__windows_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
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
#include "dh/sys/call/linux.h"

fn_((Thrd_ftx__linux_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void) $scope) {
    sys_call_linux_timespec ts = cleared();
    sys_call_linux_timespec* ts_ptr = null;
    if_some((timeout)(delay)) {
        ts.tv_sec = as$(TypeOf(ts.tv_sec))(delay.secs);
        ts.tv_nsec = as$(TypeOf(ts.tv_nsec))(delay.nanos);
        ts_ptr = &ts;
    }
    let rc = sys_call_linux_futex(ptrQualCast$((P$raw)(&ptr->raw)), sys_call_linux_FUTEX_WAIT | sys_call_linux_FUTEX_PRIVATE_FLAG, as$(sys_call_linux_word)(expect), ts_ptr, null, 0);
    if (rc == -sys_call_linux_ETIMEDOUT) {
        claim_assert(isSome(timeout));
        return_err(E_cause$ThrdTimeout());
    }
    return_ok({});
} $unscoped(fn);

fn_((Thrd_ftx__linux_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    claim_assert(max_waiters != 0);
    let waiters = (max_waiters < as$(u32)(i32_limit_max)) ? as$(i32)(max_waiters) : i32_limit_max;
    let_ignore = sys_call_linux_futex(ptrQualCast$((P$raw)(&ptr->raw)), sys_call_linux_FUTEX_WAKE | sys_call_linux_FUTEX_PRIVATE_FLAG, waiters, null, null, 0);
};
#endif /* plat_is_linux */

/* --- Darwin --- */

#if plat_is_darwin
fn_((Thrd_ftx__darwin_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void) $scope) {
    u64 timeout_ns = 0;
    if_some((timeout)(delay)) { timeout_ns = delay.secs * time_nanos_per_sec + delay.nanos; }
    let flags = sys_libc_darwin_UL_make(
        sys_libc_darwin_UL_op_compare_and_wait,
        sys_libc_darwin_ULF_NO_ERRNO
    );
    let addr = ptrQualCast$((P$raw)(&ptr->raw));
#if sys_libc_darwin_has_ulock_wait2
    let status = sys_libc_darwin_ulock_wait2(flags, addr, as$(u64)(expect), timeout_ns, 0);
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
    let status = sys_libc_darwin_ulock_wait(flags, addr, as$(u64)(expect), timeout_us);
#endif
    if (status < 0 && (-status) == sys_libc_darwin_ETIMEDOUT) {
#if sys_libc_darwin_has_ulock_wait2
        return_err(E_cause$ThrdTimeout());
#else
        if (!overflowed) { return_err(E_cause$ThrdTimeout()); }
#endif
    }
    return_ok({});
} $unscoped(fn);

fn_((Thrd_ftx__darwin_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    let flags = sys_libc_darwin_UL_make(
        sys_libc_darwin_UL_op_compare_and_wait,
        sys_libc_darwin_ULF_NO_ERRNO | (max_waiters > 1 ? sys_libc_darwin_ULF_WAKE_ALL : u32_(0))
    );
    let addr = ptrQualCast$((P$raw)(&ptr->raw));
    while (true) {
        let status = sys_libc_darwin_ulock_wake(flags, addr, 0);
        if (status >= 0 || (-status) != sys_libc_darwin_EINTR) { return; }
    }
};
#endif /* plat_is_darwin */

/* --- WASI --- */

#if plat_is_wasi
fn_((Thrd_ftx__wasi_wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_ftx_E$void) $scope) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    return_err(E_cause$UnsupportedFtx());
} $unscoped(fn);

fn_((Thrd_ftx__wasi_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = max_waiters;
};
#endif /* plat_is_wasi */

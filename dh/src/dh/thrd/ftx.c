#include "dh/thrd/ftx.h"

/*========== Internal Declarations ==========================================*/

pp_if_(pp_true)(pp_then_(
    $attr($inline_always $must_check $maybe_unused)
    $static fn_((thrd_ftx__unsupported_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void));
    $attr($inline_always $maybe_unused)
    $static fn_((thrd_ftx__unsupported_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));

typedef struct thrd_ftx__CancelWake {
    var_(ptr, const atom_V$u32*);
} thrd_ftx__CancelWake;
$static fn_((thrd_ftx__wakeCanceled(P$raw ctx))(void));
$static fn_((thrd_ftx__waitForTimeout(const atom_V$u32* ptr, u32 expect, time_Dur timeout))(thrd_ftx_E$void));
pp_if_(thrd_ftx_use_pthread)(pp_then_(
    $attr($inline_always)
    $static fn_((thrd_ftx__pthread_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void));
    $attr($inline_always)
    $static fn_((thrd_ftx__pthread_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_windows)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((thrd_ftx__windows_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void));
    $attr($inline_always)
    $static fn_((thrd_ftx__windows_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_linux)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((thrd_ftx__linux_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void));
    $attr($inline_always)
    $static fn_((thrd_ftx__linux_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_darwin)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((thrd_ftx__darwin_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void));
    $attr($inline_always)
    $static fn_((thrd_ftx__darwin_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));
pp_if_(plat_is_wasi)(pp_then_(
    $attr($inline_always $must_check)
    $static fn_((thrd_ftx__wasi_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void));
    $attr($inline_always)
    $static fn_((thrd_ftx__wasi_wake(const atom_V$u32* ptr, u32 max_waiters))(void));
));

$static let thrd_ftx__wait = pp_if_(thrd_ftx_use_pthread)(
    pp_then_(thrd_ftx__pthread_wait),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd_ftx__windows_wait),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd_ftx__linux_wait),
            pp_else_(pp_if_(plat_is_darwin)(
                pp_then_(thrd_ftx__darwin_wait),
                pp_else_(pp_if_(plat_is_wasi)(
                    pp_then_(thrd_ftx__wasi_wait),
                    pp_else_(thrd_ftx__unsupported_wait)
                ))
            ))
        ))
    )));
$static let thrd_ftx__wake = pp_if_(thrd_ftx_use_pthread)(
    pp_then_(thrd_ftx__pthread_wake),
    pp_else_(pp_if_(plat_is_windows)(
        pp_then_(thrd_ftx__windows_wake),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(thrd_ftx__linux_wake),
            pp_else_(pp_if_(plat_is_darwin)(
                pp_then_(thrd_ftx__darwin_wake),
                pp_else_(pp_if_(plat_is_wasi)(
                    pp_then_(thrd_ftx__wasi_wake),
                    pp_else_(thrd_ftx__unsupported_wake)
                ))
            ))
        ))
    )));

/*========== External Definitions ===========================================*/

$attr($branch_cold)
fn_((thrd_ftx_wait(const atom_V$u32* ptr, u32 expect, thrd_Wakeable cancel_src))(thrd_ftx_E$void) $guard) {
    let src = thrd_Wakeable_ensureValid(cancel_src);
    if (thrd_Wakeable_ready(src)) {
        return_err(E_cause$Sched_Canceled());
    }

    var wake = (thrd_ftx__CancelWake){ .ptr = ptr };
    var link = thrd_wait_Link_from(
        thrd_Waker_ensureValid((thrd_Waker){
            .ctx = &wake,
            .wakeFn = thrd_ftx__wakeCanceled,
        }),
        0
    );
    if (thrd_Wakeable_link(src, &link)) {
        return_err(E_cause$Sched_Canceled());
    }
    defer_(thrd_Wakeable_unlink(src, &link));

    catch_((thrd_ftx__wait(ptr, expect, none$((O$time_Dur))))(err, return_err(err)));
    if (thrd_Wakeable_ready(src)) {
        return_err(E_cause$Sched_Canceled());
    }
    return_ok({});
} $unguarded(fn);
$attr($branch_cold)
fn_((thrd_ftx_waitFor(const atom_V$u32* ptr, u32 expect, thrd_Wakeable cancel_src, time_Dur timeout))(thrd_ftx_E$void) $guard) {
    let src = thrd_Wakeable_ensureValid(cancel_src);
    if (thrd_Wakeable_ready(src)) {
        return_err(E_cause$Sched_Canceled());
    }

    var wake = (thrd_ftx__CancelWake){ .ptr = ptr };
    var link = thrd_wait_Link_from(
        thrd_Waker_ensureValid((thrd_Waker){
            .ctx = &wake,
            .wakeFn = thrd_ftx__wakeCanceled,
        }),
        0
    );
    if (thrd_Wakeable_link(src, &link)) {
        return_err(E_cause$Sched_Canceled());
    }
    defer_(thrd_Wakeable_unlink(src, &link));

    catch_((thrd_ftx__waitForTimeout(ptr, expect, timeout))(err, {
        if (thrd_Wakeable_ready(src)) {
            return_err(E_cause$Sched_Canceled());
        }
        return_err(err);
    }));
    if (thrd_Wakeable_ready(src)) {
        return_err(E_cause$Sched_Canceled());
    }
    return_ok({});
} $unguarded(fn);
$attr($branch_cold)
fn_((thrd_ftx_waitProtcd(const atom_V$u32* ptr, u32 expect))(void) $scope) {
    return_void(catch_((thrd_ftx__wait(ptr, expect, none$((O$time_Dur))))(
        $ignore, claim_unreachable
    )));
} $unscoped(fn);
$attr($branch_cold)
fn_((thrd_ftx_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    if (max_waiters == 0) return;
    return thrd_ftx__wake(ptr, max_waiters);
};

fn_((thrd_ftx_Deadline_init(O$time_Dur expires))(thrd_ftx_Deadline)) {
    var_(deadline, thrd_ftx_Deadline) = {
        .timeout = expires,
        .started = cleared()
    };
    if (isSome(expires)) {
        deadline.started = time_Awake_now(catch_((time_Awake_direct())(
            $ignore, time_Awake_noop
        )));
    }
    return deadline;
};
$attr($branch_cold)
fn_((thrd_ftx_Deadline_wait(thrd_ftx_Deadline* self, const atom_V$u32* ptr, u32 expect, O$thrd_Wakeable cancel_src))(thrd_ftx_E$void) $scope) {
    let timeout = orelse_((self->timeout)({
        if_some((cancel_src)(src)) {
            return thrd_ftx_wait(ptr, expect, src);
        }
        return_ok_void(thrd_ftx_waitProtcd(ptr, expect));
    }));
    let elapsed = time_Awake_Inst_elapsed(
        self->started,
        catch_((time_Awake_direct())($ignore, time_Awake_noop))
    );
    let until_timeout = orelse_((time_Dur_subChkd(timeout, elapsed))(time_Dur_zero));
    if_some((cancel_src)(src)) {
        return_(thrd_ftx_waitFor(ptr, expect, src, until_timeout));
    }
    return_(thrd_ftx__waitForTimeout(ptr, expect, until_timeout));
} $unscoped(fn);

/*========== Internal Definitions ===========================================*/

fn_((thrd_ftx__wakeCanceled(P$raw ctx))(void)) {
    let wake = ptrAlignCast$((thrd_ftx__CancelWake*)(ctx));
    thrd_ftx_wake(wake->ptr, 1);
};

$attr($branch_cold)
fn_((thrd_ftx__waitForTimeout(const atom_V$u32* ptr, u32 expect, time_Dur timeout))(thrd_ftx_E$void) $scope) {
    if (time_Dur_isZero(timeout)) {
        if (atom_V_load(ptr, atom_MemOrd_seq_cst) != expect) return_ok({});
        return_err(E_cause$Sched_Timeout());
    }
    return_(thrd_ftx__wait(ptr, expect, some$((O$time_Dur)(timeout))));
} $unscoped(fn);

/* --- Unsupported --- */

fn_((thrd_ftx__unsupported_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void) $scope) {
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    return_err(E_cause$thrd_ftx_Unsupported());
} $unscoped(fn);

fn_((thrd_ftx__unsupported_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    let_ignore = ptr;
    let_ignore = max_waiters;
};

/* --- Pthreads --- */

#if thrd_ftx_use_pthread
fn_((thrd_ftx__pthread_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void) $scope) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    return_err(E_cause$thrd_ftx_Unsupported());
} $unscoped(fn);

fn_((thrd_ftx__pthread_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = max_waiters;
};
#endif /* thrd_ftx_use_pthread */

/* --- Windows --- */

#if plat_is_windows
fn_((thrd_ftx__windows_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void) $scope) {
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
        return_err(E_cause$Sched_Timeout());
    }
    return_ok({});
} $unscoped(fn);

fn_((thrd_ftx__windows_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
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

T_use_P$(sys_call_linux_timespec);
T_use_O$(P$sys_call_linux_timespec);

fn_((thrd_ftx__linux_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void) $scope) {
    var_(ts, sys_call_linux_timespec) = cleared();
    var_(ts_ptr, O$P$sys_call_linux_timespec) = none();
    if_some((timeout)(delay)) {
        ts.tv_sec = as$(FieldType$(sys_call_linux_timespec, tv_sec))(delay.secs);
        ts.tv_nsec = as$(FieldType$(sys_call_linux_timespec, tv_nsec))(delay.nanos);
        ts_ptr = some$((O$P$sys_call_linux_timespec)(&ts));
    }
    let rc = sys_call_linux_futex(
        (ptrQualCast$((void*)(&ptr->raw))),
        sys_call_linux_FUTEX_WAIT | sys_call_linux_FUTEX_PRIVATE_FLAG,
        expect,
        orelse_((ts_ptr)(null)),
        null,
        0
    );
    if (sys_call_linux_syscall_isErr(rc) && sys_call_linux_syscall_err(rc) == sys_call_linux_ETIMEDOUT) {
        claim_assert(isSome(timeout));
        return_err(E_cause$Sched_Timeout());
    }
    return_ok({});
} $unscoped(fn);

fn_((thrd_ftx__linux_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    claim_assert(max_waiters != 0);
    let waiters = (max_waiters < as$(u32)(i32_limit_max)) ? as$(i32)(max_waiters) : i32_limit_max;
    let_ignore = sys_call_linux_futex(
        (ptrQualCast$((void*)(&ptr->raw))),
        sys_call_linux_FUTEX_WAKE | sys_call_linux_FUTEX_PRIVATE_FLAG,
        waiters,
        null,
        null,
        0
    );
};
#endif /* plat_is_linux */

/* --- Darwin --- */

#if plat_is_darwin
#include "dh/sys/libc/darwin/sync.h"

fn_((thrd_ftx__darwin_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void) $scope) {
    u64 timeout_ns = 0;
    if_some((timeout)(delay)) { timeout_ns = delay.secs * time_nanos_per_sec + delay.nanos; }
    let flags = sys_libc_darwin_UL_make(
        sys_libc_darwin_UL_op_compare_and_wait,
        sys_libc_darwin_ULF_NO_ERRNO
    );
    let addr = as$(P$raw)(&ptr->raw);
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
        return_err(E_cause$Sched_Timeout());
#else
        if (!overflowed) { return_err(E_cause$Sched_Timeout()); }
#endif
    }
    return_ok({});
} $unscoped(fn);

fn_((thrd_ftx__darwin_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    let flags = sys_libc_darwin_UL_make(
        sys_libc_darwin_UL_op_compare_and_wait,
        sys_libc_darwin_ULF_NO_ERRNO | (max_waiters > 1 ? sys_libc_darwin_ULF_WAKE_ALL : 0)
    );
    let addr = as$(P$raw)(&ptr->raw);
    while (true) {
        let status = sys_libc_darwin_ulock_wake(flags, addr, 0);
        if (status >= 0 || (-status) != sys_libc_darwin_EINTR) { return; }
    }
};
#endif /* plat_is_darwin */

/* --- WASI --- */

#if plat_is_wasi
fn_((thrd_ftx__wasi_wait(const atom_V$u32* ptr, u32 expect, O$time_Dur timeout))(thrd_ftx_E$void) $scope) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    return_err(E_cause$thrd_ftx_Unsupported());
} $unscoped(fn);

fn_((thrd_ftx__wasi_wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    /* TODO: Implement */
    let_ignore = ptr;
    let_ignore = max_waiters;
};
#endif /* plat_is_wasi */

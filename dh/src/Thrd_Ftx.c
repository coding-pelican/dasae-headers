#include "dh/Thrd/Ftx.h"
#include "dh/time/Instant.h"

#if plat_type == plat_type_windows
#include "dh/os/windows.h"
#if comp_type == comp_type_msvc
#pragma comment(lib, "synchronization.lib")
#endif /* comp_type == comp_type_msvc */
#elif plat_type == plat_type_linux
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#elif plat_type == plat_type_darwin
#include <errno.h>
// Darwin ulock declarations (private API, but stable)
#define UL_COMPARE_AND_WAIT 1
#define ULF_NO_ERRNO 0x01000000
#define ULF_WAKE_ALL 0x00000100
$extern fn_((__ulock_wait(u32 operation, P$raw addr, u64 value, u32 timeout_us))(i32));
$extern fn_((__ulock_wait2(u32 operation, P$raw addr, u64 value, u64 timeout_ns, u64 value2))(i32));
$extern fn_((__ulock_wake(u32 operation, P$raw addr, u64 wake_value))(i32));
#endif

$attr($must_check)
$static fn_((Thrd_Ftx__wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void));
$static fn_((Thrd_Ftx__wake(const atom_V$u32* ptr, u32 max_waiters))(void));

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
    if (isSome(expires)) {
        deadline.started = time_Instant_now();
    }
    return deadline;
};

$attr($branch_cold)
fn_((Thrd_Ftx_Deadline_wait(Thrd_Ftx_Deadline* self, const atom_V$u32* ptr, u32 expect))(Thrd_Ftx_Err$void) $scope) {
    let timeout = orelse_((self->timeout)(return_ok_void(Thrd_Ftx_wait(ptr, expect))));
    let elapsed = time_Instant_elapsed(self->started);
    let until_timeout = orelse_((time_Duration_subChkd(timeout, elapsed))(time_Duration_zero));
    return_(Thrd_Ftx_timedWait(ptr, expect, until_timeout));
} $unscoped_(fn);

fn_((Thrd_Ftx__wait(const atom_V$u32* ptr, u32 expect, O$time_Duration timeout))(Thrd_Ftx_Err$void) $scope) {
#if plat_type == plat_type_windows
    // WaitOnAddress uses timeout in milliseconds
    let timeout_ms = expr_(DWORD $scope)(if_some((timeout)(delay)) {
        // Convert duration to milliseconds, capping at DWORD max
        let delay_ms = delay.secs * lit_num(1, 000) + delay.nanos / lit_num(1, 000, 000);
        $break_(u32_limit_max < delay_ms ? u32_limit_max : as$(DWORD)(delay_ms));
    } else_none {
        $break_(INFINITE);
    }) $unscoped_(expr);

    let rc = WaitOnAddress(
        (as$(volatile P$raw)(&ptr->raw)),
        &expect, sizeOf$(TypeOf(expect)),
        timeout_ms
    );

    switch (rc ? ERROR_SUCCESS : GetLastError()) {
    case_((ERROR_SUCCESS)) break $end(case);
    case_((ERROR_TIMEOUT)) {
        claim_assert(isSome(timeout));
        return_err(Thrd_Ftx_Err_Timeout());
    } $end(case);
    default_() claim_unreachable $end(default);
    };
    return_ok({});

#elif plat_type == plat_type_linux
    struct timespec ts = {};
    struct timespec* ts_ptr = null;
    if_some((timeout)(delay)) {
        ts.tv_sec = as$(time_t)(delay.secs);
        ts.tv_nsec = as$(long)(delay.nanos);
        ts_ptr = &ts;
    }

    let rc = syscall(SYS_futex, &ptr->raw, FUTEX_WAIT | FUTEX_PRIVATE_FLAG, expect, ts_ptr, null, 0);
    if (rc == -1) {
        switch (errno) {
        case ETIMEDOUT:
            claim_assert(isSome(timeout));
            return_err(Thrd_Ftx_Err_Timeout());
        case EINTR: // spurious wakeup
        case EAGAIN: // ptr->raw != expect
        case EINVAL: // possibly timeout overflow
            break;
        }
    }
    return_ok({});

#elif plat_type == plat_type_darwin
    // Darwin XNU 7195.50.7.100.1 introduced __ulock_wait2 (macOS 11.0.1+)
    // Use __ulock_wait2 for nanosecond precision, __ulock_wait for microsecond
    u64 timeout_ns = 0;
    if_some((timeout)(delay)) {
        timeout_ns = delay.secs * time_nanos_per_sec + delay.nanos;
    }

    let flags = UL_COMPARE_AND_WAIT | ULF_NO_ERRNO;
    let addr = as$(P$raw)(&ptr->raw);

    // Try using __ulock_wait2 for 64-bit nanosecond timeout
    // For older macOS, fall back to __ulock_wait with microsecond timeout
#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 110001
    let status = __ulock_wait2(flags, addr, as$(u64)(expect), timeout_ns, 0);
#else
    // __ulock_wait uses 32-bit microsecond timeout where 0 = INFINITE
    var timeout_us = as$(u32)(0);
    var timeout_overflowed = false;
    if (timeout_ns != 0) {
        let timeout_us_64 = timeout_ns / 1000;
        if (timeout_us_64 > u32_limit_max) {
            timeout_overflowed = true;
            timeout_us = u32_limit_max;
        } else {
            timeout_us = as$(u32)(timeout_us_64);
        }
    }
    let status = __ulock_wait(flags, addr, as$(u64)(expect), timeout_us);
#endif

    if (status < 0) {
        let err = -status;
        if (err == ETIMEDOUT) {
#if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED < 110001
            if (!timeout_overflowed)
#endif
            {
                claim_assert(isSome(timeout));
                return_err(Thrd_Ftx_Err_Timeout());
            }
        }
        // EINTR: spurious wake, EFAULT: address paged out - treat as spurious
    }
    return_ok({});

#elif plat_type == plat_type_wasi
    // WASI with atomics - not currently supported
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    claim_unreachable; // WASI futex not implemented
    return_ok({});

#else
    // Unsupported platform
    let_ignore = ptr;
    let_ignore = expect;
    let_ignore = timeout;
    claim_unreachable; // Futex not supported on this platform
    return_ok({});
#endif
} $unscoped_(fn);

fn_((Thrd_Ftx__wake(const atom_V$u32* ptr, u32 max_waiters))(void)) {
    claim_assert(max_waiters != 0);

#if plat_type == plat_type_windows
    let addr = as$(volatile P$raw)(&ptr->raw);
    if (max_waiters == 1) {
        WakeByAddressSingle(addr);
    } else {
        WakeByAddressAll(addr);
    }

#elif plat_type == plat_type_linux
    // Use min to avoid overflow when casting to i32
    let waiters_to_wake = max_waiters < as$(u32)(i32_limit_max) ? as$(i32)(max_waiters) : i32_limit_max;
    let_ignore = syscall(SYS_futex, &ptr->raw, FUTEX_WAKE | FUTEX_PRIVATE_FLAG, waiters_to_wake, null, null, 0);
    // Ignore return value - it's the number of waiters woken or an error we can't handle

#elif plat_type == plat_type_darwin
    let flags = UL_COMPARE_AND_WAIT | ULF_NO_ERRNO | (max_waiters > 1 ? ULF_WAKE_ALL : 0);
    let addr = as$(P$raw)(&ptr->raw);

    while (true) {
        let status = __ulock_wake(flags, addr, 0);
        if (status >= 0) { return; }

        let err = -status;
        if (err == EINTR) { continue; } // spurious wake, retry
        if (err == ENOENT) { return; } // nothing was woken up
        // EFAULT: shouldn't happen according to darwin pthread_cond_t
        return;
    }

#elif plat_type == plat_type_wasi
    // WASI with atomics - not currently supported
    let_ignore = ptr;
    let_ignore = max_waiters;
    claim_unreachable; // WASI futex not implemented

#else
    // Unsupported platform
    let_ignore = ptr;
    let_ignore = max_waiters;
    claim_unreachable; // Futex not supported on this platform
#endif
}

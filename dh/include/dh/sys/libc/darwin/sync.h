/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    sync.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-27 (date of creation)
 * @ingroup dasae-headers(dh)/sys/libc/darwin
 * @prefix  sys_libc_darwin
 */
#pragma once
#ifndef sys_libc_darwin_sync__included
#define sys_libc_darwin_sync__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#if on_analysis || plat_is_darwin
typedef struct sys_libc_darwin_unfair_lock {
    var_(os_unfair_lock_opaque_, u32);
} sys_libc_darwin_unfair_lock;
typedef sys_libc_darwin_unfair_lock* sys_libc_darwin_unfair_lock_t;

#define sys_libc_darwin_UNFAIR_LOCK_INIT \
    __val__sys_libc_darwin_UNFAIR_LOCK_INIT

typedef enum sys_libc_darwin_errno {
    sys_libc_darwin_EINTR = 4,
    sys_libc_darwin_ETIMEDOUT = 60,
} sys_libc_darwin_errno;

typedef u32 sys_libc_darwin_UL;

typedef enum sys_libc_darwin_UL_op {
    sys_libc_darwin_UL_op_compare_and_wait = 1,
    sys_libc_darwin_UL_op_unfair_lock = 2,
    sys_libc_darwin_UL_op_compare_and_wait_shared = 3,
    sys_libc_darwin_UL_op_unfair_lock64_shared = 4,
    sys_libc_darwin_UL_op_compare_and_wait64 = 5,
    sys_libc_darwin_UL_op_compare_and_wait64_shared = 6,
} sys_libc_darwin_UL_op;

typedef enum sys_libc_darwin_UL_const {
    sys_libc_darwin_UL_COMPARE_AND_WAIT = sys_libc_darwin_UL_op_compare_and_wait,
} sys_libc_darwin_UL_const;

$static let sys_libc_darwin_ULF_WAKE_ALL = u32_(0x00000100);
$static let sys_libc_darwin_ULF_WAKE_THREAD = u32_(0x00000200);
$static let sys_libc_darwin_ULF_WAIT_WORKQ_DATA_CONTENTION = u32_(0x00010000);
$static let sys_libc_darwin_ULF_WAIT_CANCEL_POINT = u32_(0x00020000);
$static let sys_libc_darwin_ULF_WAIT_ADAPTIVE_SPIN = u32_(0x00040000);
$static let sys_libc_darwin_ULF_NO_ERRNO = u32_(0x01000000);

$attr($inline_always $must_check)
$static fn_((sys_libc_darwin_UL_make(sys_libc_darwin_UL_op op, u32 flags))(sys_libc_darwin_UL));

$extern fn_((sys_libc_darwin_unfair_lock_lock(sys_libc_darwin_unfair_lock_t lock))(void));
$extern fn_((sys_libc_darwin_unfair_lock_trylock(sys_libc_darwin_unfair_lock_t lock))(bool));
$extern fn_((sys_libc_darwin_unfair_lock_unlock(sys_libc_darwin_unfair_lock_t lock))(void));
$extern fn_((sys_libc_darwin_unfair_lock_assert_owner(sys_libc_darwin_unfair_lock_t lock))(void));
$extern fn_((sys_libc_darwin_unfair_lock_assert_not_owner(sys_libc_darwin_unfair_lock_t lock))(void));

$extern fn_((sys_libc_darwin_ulock_wait(sys_libc_darwin_UL operation, P$raw addr, u64 value, u32 timeout_us))(i32));
$extern fn_((sys_libc_darwin_ulock_wait2(sys_libc_darwin_UL operation, P$raw addr, u64 value, u64 timeout_ns, u64 value2))(i32));
$extern fn_((sys_libc_darwin_ulock_wake(sys_libc_darwin_UL operation, P$raw addr, u64 wake_value))(i32));
#endif /* on_analysis || plat_is_darwin */

/*========== Macros and Definitions =========================================*/

#if on_analysis || plat_is_darwin
#define __val__sys_libc_darwin_UNFAIR_LOCK_INIT \
    l$((sys_libc_darwin_unfair_lock){ .os_unfair_lock_opaque_ = 0 })
#endif /* on_analysis || plat_is_darwin */

#if on_analysis_active_only || on_comptime && plat_is_darwin
fn_((sys_libc_darwin_UL_make(sys_libc_darwin_UL_op op, u32 flags))(sys_libc_darwin_UL)) {
    return as$(sys_libc_darwin_UL)(op) | flags;
};
#endif /* on_analysis_active_only || on_comptime && plat_is_darwin */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_libc_darwin_sync__included */

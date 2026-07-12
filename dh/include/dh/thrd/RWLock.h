/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    RWLock.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2026-05-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_RWLock
 *
 * @brief   Read-Write lock for thread management
 * @details Defines read-write lock for thread management.
 */
#pragma once
#ifndef thrd_RWLock__included
#define thrd_RWLock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Mtx.h"
#include "Cond.h"

/*========== Macros and Declarations ========================================*/

#if !defined(thrd_RWLock_use_pthread)
#define thrd_RWLock_use_pthread __comp_bool__thrd_RWLock_use_pthread
#endif /* !defined(thrd_RWLock_use_pthread) */
#define __comp_bool__thrd_RWLock_use_pthread thrd_RWLock__use_pthread_default

#define thrd_RWLock__use_pthread_default __comp_bool__thrd_RWLock__use_pthread_default
#define __comp_bool__thrd_RWLock__use_pthread_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_default_(pp_false) \
    ) pp_end \
)

struct thrd_RWLock__Impl pp_if_(thrd_RWLock_use_pthread)(
    pp_then_({
        var_(_unused, Void);
    }),
    pp_else_({
        var_(state, usize);
        var_(mtx, thrd_Mtx);
        var_(cond, thrd_Cond);
    }));
struct thrd_RWLock pp_if_(thrd_RWLock_use_pthread)(
    pp_then_({ var_(impl, pthread_rwlock_t); }),
    pp_else_({ var_(impl, thrd_RWLock__Impl); }));
$extern fn_((thrd_RWLock_init(void))(thrd_RWLock));
$extern fn_((thrd_RWLock_fini(thrd_RWLock* self))(void));

$extern fn_((thrd_RWLock_tryLock(thrd_RWLock* self))(bool));
$attr($must_check)
$extern fn_((thrd_RWLock_lock(thrd_RWLock* self, thrd_wait_Src cancel_src))(Sched_Cancelable$void));
$extern fn_((thrd_RWLock_lockProtcd(thrd_RWLock* self))(void));
$extern fn_((thrd_RWLock_unlock(thrd_RWLock* self))(void));

$extern fn_((thrd_RWLock_tryLockShared(thrd_RWLock* self))(bool));
$attr($must_check)
$extern fn_((thrd_RWLock_lockShared(thrd_RWLock* self, thrd_wait_Src cancel_src))(Sched_Cancelable$void));
$extern fn_((thrd_RWLock_lockSharedProtcd(thrd_RWLock* self))(void));
$extern fn_((thrd_RWLock_unlockShared(thrd_RWLock* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_RWLock__included */

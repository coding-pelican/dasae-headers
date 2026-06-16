/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Mtx.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2026-05-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Mtx
 *
 * @brief   Mutex for thread management
 * @details Defines mutex for thread management.
 */
#pragma once
#ifndef thrd_Mtx__included
#define thrd_Mtx__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "dh/sys/libc/darwin/sync.h"

/*========== Macros and Declarations ========================================*/

#if !defined(thrd_Mtx_use_pthread)
#define thrd_Mtx_use_pthread __comp_bool__thrd_Mtx_use_pthread
#endif /* !defined(thrd_Mtx_use_pthread) */
#define __comp_bool__thrd_Mtx_use_pthread thrd_Mtx__use_pthread_default

#define thrd_Mtx__use_pthread_default __comp_bool__thrd_Mtx__use_pthread_default
#define __comp_bool__thrd_Mtx__use_pthread_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_false)), \
        pp_default_(thrd_use_pthread) \
    ) pp_end \
)

#if !defined(thrd_Mtx_has_specialized)
#define thrd_Mtx_has_specialized __comp_bool__thrd_Mtx_has_specialized
#endif /* !defined(thrd_Mtx_has_specialized) */
#define __comp_bool__thrd_Mtx_has_specialized thrd_Mtx__has_specialized_default

#define thrd_Mtx__has_specialized_default __comp_bool__thrd_Mtx__has_specialized_default
#define __comp_bool__thrd_Mtx__has_specialized_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_true)), \
        pp_case_((plat_type_darwin)(sys_libc_darwin_has_unfair_lock)), \
        pp_default_(pp_false) \
    ) pp_end \
)

struct thrd_Mtx__Impl pp_if_(thrd_Mtx_use_pthread)(
    pp_then_({
        var_(unused_, Void);
    }),
    pp_else_(pp_if_(thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)({
                    var_(inner, SRWLOCK);
                })),
                pp_case_((plat_type_darwin)({
                    var_(inner, sys_libc_darwin_unfair_lock);
                }))
            ) pp_end
        )),
        pp_else_({
            var_(state, atom_V$u32);
        })
    )));
struct thrd_Mtx pp_if_(thrd_Mtx_use_pthread)(
    pp_then_({ var_(impl, pthread_mutex_t); }),
    pp_else_({ var_(impl, thrd_Mtx__Impl); }));
/// @brief Initializes a mutex
/// @return A new mutex
$extern fn_((thrd_Mtx_init(void))(thrd_Mtx));
/// @brief Finalizes a mutex
/// @param self Pointer to the mutex to finalize
$extern fn_((thrd_Mtx_fini(thrd_Mtx* self))(void));
/// @brief Locks a mutex, blocking if the mutex is already locked
/// @param self Pointer to the mutex to lock
$extern fn_((thrd_Mtx_lock(thrd_Mtx* self))(void));
/// @brief Attempts to lock a mutex without blocking
/// @param self Pointer to the mutex to try locking
/// @return true if the mutex was locked, false if it was already locked
$extern fn_((thrd_Mtx_tryLock(thrd_Mtx* self))(bool));
/// @brief Unlocks a mutex
/// @param self Pointer to the mutex to unlock
$extern fn_((thrd_Mtx_unlock(thrd_Mtx* self))(void));

struct thrd_Mtx_Recur {
    var_(inner, thrd_Mtx);
    var_(thrd_id, thrd_Id);
    var_(lock_count, usize);
};
$extern fn_((thrd_Mtx_Recur_init(void))(thrd_Mtx_Recur));
$extern fn_((thrd_Mtx_Recur_fini(thrd_Mtx_Recur* self))(void));
$extern fn_((thrd_Mtx_Recur_lock(thrd_Mtx_Recur* self))(void));
$extern fn_((thrd_Mtx_Recur_tryLock(thrd_Mtx_Recur* self))(bool));
$extern fn_((thrd_Mtx_Recur_unlock(thrd_Mtx_Recur* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Mtx__included */

/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Mtx.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-12-20 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd_Mtx
 *
 * @brief   Mutex for thread management
 * @details Defines mutex for thread management.
 */
#ifndef Thrd_Mtx__included
#define Thrd_Mtx__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"

/*========== Macros and Declarations ========================================*/

#define Thrd_Mtx__use_pthread __comp_bool__Thrd_Mtx__use_pthread
#define __comp_bool__Thrd_Mtx__use_pthread pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_false)), \
        pp_case_((plat_type_linux)(pp_false)), \
        pp_case_((plat_type_darwin)(pp_false)), \
        pp_default_(pp_true) \
    ) pp_end \
)

struct Thrd_Mtx__Impl pp_if_(plat_is_linux)(
    pp_then_({ var_(state, atom_V$u32); }),
    pp_else_({ var_(unused_, Void); })
);
struct Thrd_Mtx pp_switch_((plat_type)(
    pp_case_((plat_type_windows)({ var_(impl, SRWLOCK); })),
    pp_case_((plat_type_linux)({ var_(impl, Thrd_Mtx__Impl); })),
    pp_case_((plat_type_darwin)({ var_(impl, os_unfair_lock); })),
    pp_default_({ var_(impl, pthread_mutex_t); })
));
/// @brief Initializes a mutex
/// @return A new mutex
$extern fn_((Thrd_Mtx_init(void))(Thrd_Mtx));
/// @brief Finalizes a mutex
/// @param self Pointer to the mutex to finalize
$extern fn_((Thrd_Mtx_fini(Thrd_Mtx* self))(void));
/// @brief Locks a mutex, blocking if the mutex is already locked
/// @param self Pointer to the mutex to lock
$extern fn_((Thrd_Mtx_lock(Thrd_Mtx* self))(void));
/// @brief Attempts to lock a mutex without blocking
/// @param self Pointer to the mutex to try locking
/// @return true if the mutex was locked, false if it was already locked
$extern fn_((Thrd_Mtx_tryLock(Thrd_Mtx* self))(bool));
/// @brief Unlocks a mutex
/// @param self Pointer to the mutex to unlock
$extern fn_((Thrd_Mtx_unlock(Thrd_Mtx* self))(void));

struct Thrd_Mtx_Recur {
    var_(inner, Thrd_Mtx);
    var_(thrd_id, Thrd_Id);
    var_(lock_count, usize);
};
$extern fn_((Thrd_Mtx_Recur_init(void))(Thrd_Mtx_Recur));
$extern fn_((Thrd_Mtx_Recur_fini(Thrd_Mtx_Recur* self))(void));
$extern fn_((Thrd_Mtx_Recur_lock(Thrd_Mtx_Recur* self))(void));
$extern fn_((Thrd_Mtx_Recur_tryLock(Thrd_Mtx_Recur* self))(bool));
$extern fn_((Thrd_Mtx_Recur_unlock(Thrd_Mtx_Recur* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Thrd_Mtx__included */

/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Cond.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-12-20 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd_Cond
 *
 * @brief   Condition variable for thread management
 * @details Defines condition variable for thread management.
 */
#ifndef Thrd_Cond__included
#define Thrd_Cond__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"

/*========== Macros and Declarations ========================================*/

#if !defined(Thrd_Cond_use_pthread)
#define Thrd_Cond_use_pthread __comp_bool__Thrd_Cond_use_pthread
#endif /* !defined(Thrd_Cond_use_pthread) */
#define __comp_bool__Thrd_Cond_use_pthread Thrd_Cond__use_pthread_default

#define Thrd_Cond__use_pthread_default __comp_bool__Thrd_Cond__use_pthread_default
#define __comp_bool__Thrd_Cond__use_pthread_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_false)), \
        pp_default_(Thrd_use_pthread) \
    ) pp_end \
)

#if !defined(Thrd_Cond_has_specialized)
#define Thrd_Cond_has_specialized __comp_bool__Thrd_Cond_has_specialized
#endif /* !defined(Thrd_Cond_has_specialized) */
#define __comp_bool__Thrd_Cond_has_specialized Thrd_Cond__has_specialized_default

#define Thrd_Cond__has_specialized_default __comp_bool__Thrd_Cond__has_specialized_default
#define __comp_bool__Thrd_Cond__has_specialized_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_true)), \
        pp_default_(pp_false) \
    ) pp_end \
)

errset_((Thrd_Cond_Err)(Timeout));
struct Thrd_Cond__Impl pp_if_(Thrd_Cond_use_pthread)(
    pp_then_({
        var_(unused_, Void);
    }),
    pp_else_(pp_if_(Thrd_Cond_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)({
                    var_(inner, CONDITION_VARIABLE);
                }))
            ) pp_end
        )),
        pp_else_({
            var_(state, atom_V$u32);
            var_(epoch, atom_V$u32);
        })
    )));
struct Thrd_Cond pp_if_(Thrd_Cond_use_pthread)(
    pp_then_({ var_(impl, pthread_cond_t); }),
    pp_else_({ var_(impl, Thrd_Cond__Impl); }));
/// @brief Initializes a condition variable
/// @return A new condition variable
$extern fn_((Thrd_Cond_init(void))(Thrd_Cond));
/// @brief Finalizes a condition variable
/// @param self Pointer to the condition variable to finalize
$extern fn_((Thrd_Cond_fini(Thrd_Cond* self))(void));
/// @brief Waits for a condition variable to be signaled
/// @param self Pointer to the condition variable to wait on
/// @param mtx Pointer to the mutex to unlock while waiting
$extern fn_((Thrd_Cond_wait(Thrd_Cond* self, Thrd_Mtx* mtx))(void));
/// @brief Waits for a condition variable to be signaled with a timeout
/// @param self Pointer to the condition variable to wait on
/// @param mtx Pointer to the mutex to unlock while waiting
/// @param timeout Maximum time to wait
/// @return Error if the wait timed out
$attr($must_check)
$extern fn_((Thrd_Cond_timedWait(Thrd_Cond* self, Thrd_Mtx* mtx, time_Duration timeout))(Thrd_Cond_Err$void));
/// @brief Signals one waiting thread on a condition variable
/// @param self Pointer to the condition variable to signal
$extern fn_((Thrd_Cond_signal(Thrd_Cond* self))(void));
/// @brief Signals all waiting threads on a condition variable
/// @param self Pointer to the condition variable to broadcast to
$extern fn_((Thrd_Cond_broadcast(Thrd_Cond* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Thrd_Cond__included */

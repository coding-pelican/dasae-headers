/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Cond.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-12-20 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd
 *
 * @brief   Condition variable for thread management
 * @details Defines condition variable for thread management.
 */
#pragma once
#ifndef thrd_Cond__included
#define thrd_Cond__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

#if !defined(thrd_Cond_use_pthread)
#define thrd_Cond_use_pthread __comp_bool__thrd_Cond_use_pthread
#endif /* !defined(thrd_Cond_use_pthread) */
#define __comp_bool__thrd_Cond_use_pthread thrd_Cond__use_pthread_default

#define thrd_Cond__use_pthread_default __comp_bool__thrd_Cond__use_pthread_default
#define __comp_bool__thrd_Cond__use_pthread_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_false)), \
        pp_default_(thrd_use_pthread) \
    ) pp_end \
)

#if !defined(thrd_Cond_has_specialized)
#define thrd_Cond_has_specialized __comp_bool__thrd_Cond_has_specialized
#endif /* !defined(thrd_Cond_has_specialized) */
#define __comp_bool__thrd_Cond_has_specialized thrd_Cond__has_specialized_default

#define thrd_Cond__has_specialized_default __comp_bool__thrd_Cond__has_specialized_default
#define __comp_bool__thrd_Cond__has_specialized_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_true)), \
        pp_default_(pp_false) \
    ) pp_end \
)

struct thrd_Cond__Impl pp_if_(thrd_Cond_use_pthread)(
    pp_then_({
        var_(unused_, Void);
    }),
    pp_else_(pp_if_(thrd_Cond_has_specialized)(
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
struct thrd_Cond pp_if_(thrd_Cond_use_pthread)(
    pp_then_({ var_(impl, pthread_cond_t); }),
    pp_else_({ var_(impl, thrd_Cond__Impl); }));
#if !thrd_Cond_use_pthread
#if thrd_Cond_has_specialized && plat_is_windows
#define thrd_Cond_init_static() \
    { .impl.inner = CONDITION_VARIABLE_INIT }
#else
#define thrd_Cond_init_static() \
    { \
        .impl = {.state = atom_V_init(0u), \
                 .epoch = atom_V_init(0u) } \
    }
#endif
#endif /* !thrd_Cond_use_pthread */
/// @brief Initializes a condition variable
/// @return A new condition variable
$extern fn_((thrd_Cond_init(void))(thrd_Cond));
/// @brief Finalizes a condition variable
/// @param self Pointer to the condition variable to finalize
$extern fn_((thrd_Cond_fini(thrd_Cond* self))(void));
/// @brief Waits for a condition variable to be signaled
/// @param self Pointer to the condition variable to wait on
/// @param mtx Pointer to the mutex to unlock while waiting
$extern fn_((thrd_Cond_wait(thrd_Cond* self, thrd_Mtx* mtx))(void));
/// @brief Waits for a condition variable to be signaled with a timeout
/// @param self Pointer to the condition variable to wait on
/// @param mtx Pointer to the mutex to unlock while waiting
/// @param timeout Maximum time to wait
/// @return Error if the wait timed out
$attr($must_check)
$extern fn_((thrd_Cond_timedWait(thrd_Cond* self, thrd_Mtx* mtx, time_Dur timeout))(Sched_TimeoutE$void));
/// @brief Signals one waiting thread on a condition variable
/// @param self Pointer to the condition variable to signal
$extern fn_((thrd_Cond_signal(thrd_Cond* self))(void));
/// @brief Signals all waiting threads on a condition variable
/// @param self Pointer to the condition variable to broadcast to
$extern fn_((thrd_Cond_broadcast(thrd_Cond* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Cond__included */

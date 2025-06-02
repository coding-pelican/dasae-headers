/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Cond.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-05-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd_Cond
 *
 * @brief   Condition variable for thread management
 * @details Defines condition variable for thread management.
 */

#ifndef THRD_COND_INCLUDED
#define THRD_COND_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"

/*========== Macros and Definitions =========================================*/

struct Thrd_Cond {
    Thrd_CondImpl impl;
};
/// @brief Initializes a condition variable
/// @return A new condition variable
extern fn_(Thrd_Cond_init(void), Thrd_Cond);
/// @brief Finalizes a condition variable
/// @param self Pointer to the condition variable to finalize
extern fn_(Thrd_Cond_fini(Thrd_Cond* self), void);
/// @brief Waits for a condition variable to be signaled
/// @param self Pointer to the condition variable to wait on
/// @param mtx Pointer to the mutex to unlock while waiting
extern fn_(Thrd_Cond_wait(Thrd_Cond* self, Thrd_Mtx* mtx), void);
/// @brief Waits for a condition variable to be signaled with a timeout
/// @param self Pointer to the condition variable to wait on
/// @param mtx Pointer to the mutex to unlock while waiting
/// @param duration Maximum time to wait
/// @return true if the condition was signaled, false if the wait timed out
extern fn_(Thrd_Cond_timedWait(Thrd_Cond* self, Thrd_Mtx* mtx, time_Duration duration), bool);
/// @brief Signals one waiting thread on a condition variable
/// @param self Pointer to the condition variable to signal
extern fn_(Thrd_Cond_signal(Thrd_Cond* self), void);
/// @brief Signals all waiting threads on a condition variable
/// @param self Pointer to the condition variable to broadcast to
extern fn_(Thrd_Cond_broadcast(Thrd_Cond* self), void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_MTX_INCLUDED */

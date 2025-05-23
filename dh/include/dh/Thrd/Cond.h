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
extern fn_(Thrd_Cond_init(void), Thrd_Cond);
extern fn_(Thrd_Cond_fini(Thrd_Cond* self), void);
extern fn_(Thrd_Cond_wait(Thrd_Cond* self, Thrd_Mtx* mtx), void);
extern fn_(Thrd_Cond_timedWait(Thrd_Cond* self, Thrd_Mtx* mtx, time_Duration duration), bool);
extern fn_(Thrd_Cond_signal(Thrd_Cond* self), void);
extern fn_(Thrd_Cond_broadcast(Thrd_Cond* self), void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_MTX_INCLUDED */

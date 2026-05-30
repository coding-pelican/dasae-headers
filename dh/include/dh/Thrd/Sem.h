/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Sem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2026-05-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Sem
 *
 * @brief   Semaphore for thread management
 * @details Defines semaphore for thread management.
 */
#ifndef thrd_Sem__included
#define thrd_Sem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Mtx.h"
#include "Cond.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_Sem {
    var_(mtx, thrd_Mtx);
    var_(cond, thrd_Cond);
    var_(permits, usize);
} thrd_Sem;
$extern fn_((thrd_Sem_init(void))(thrd_Sem));
$extern fn_((thrd_Sem_fini(thrd_Sem* self))(void));
$extern fn_((thrd_Sem_wait(thrd_Sem* self))(void));
$attr($must_check)
$extern fn_((thrd_Sem_timedWait(thrd_Sem* self, time_Dur timeout))(Sched_TimeoutE$void));
$extern fn_((thrd_Sem_post(thrd_Sem* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Sem__included */

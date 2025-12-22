/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Sem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2025-12-20 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd_Sem
 *
 * @brief   Semaphore for thread management
 * @details Defines semaphore for thread management.
 */
#ifndef Thrd_Sem__included
#define Thrd_Sem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "Mtx.h"
#include "Cond.h"

/*========== Macros and Declarations ========================================*/

errset_((Thrd_Sem_Err)(Timeout));
typedef struct Thrd_Sem {
    var_(mtx, Thrd_Mtx);
    var_(cond, Thrd_Cond);
    var_(permits, usize);
} Thrd_Sem;
$extern fn_((Thrd_Sem_init(void))(Thrd_Sem));
$extern fn_((Thrd_Sem_fini(Thrd_Sem* self))(void));
$extern fn_((Thrd_Sem_wait(Thrd_Sem* self))(void));
$attr($must_check)
$extern fn_((Thrd_Sem_timedWait(Thrd_Sem* self, time_Duration timeout))(Thrd_Sem_Err$void));
$extern fn_((Thrd_Sem_post(Thrd_Sem* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Thrd_Sem__included */

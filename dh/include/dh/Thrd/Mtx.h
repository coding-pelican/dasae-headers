/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Mtx.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-05-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd_Mtx
 *
 * @brief   Mutex for thread management
 * @details Defines mutex for thread management.
 */

#ifndef THRD_MTX_INCLUDED
#define THRD_MTX_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"

/*========== Macros and Definitions =========================================*/

struct Thrd_Mtx {
    Thrd_MtxImpl impl;
};
extern fn_(Thrd_Mtx_init(void), Thrd_Mtx);
extern fn_(Thrd_Mtx_fini(Thrd_Mtx* self), void);
extern fn_(Thrd_Mtx_lock(Thrd_Mtx* self), void);
extern fn_(Thrd_Mtx_tryLock(Thrd_Mtx* self), bool);
extern fn_(Thrd_Mtx_unlock(Thrd_Mtx* self), void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_MTX_INCLUDED */

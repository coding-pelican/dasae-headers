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
/// @brief Initializes a mutex
/// @return A new mutex
extern fn_(Thrd_Mtx_init(void), Thrd_Mtx);
/// @brief Finalizes a mutex
/// @param self Pointer to the mutex to finalize
extern fn_(Thrd_Mtx_fini(Thrd_Mtx* self), void);
/// @brief Locks a mutex, blocking if the mutex is already locked
/// @param self Pointer to the mutex to lock
extern fn_(Thrd_Mtx_lock(Thrd_Mtx* self), void);
/// @brief Attempts to lock a mutex without blocking
/// @param self Pointer to the mutex to try locking
/// @return true if the mutex was locked, false if it was already locked
extern fn_(Thrd_Mtx_tryLock(Thrd_Mtx* self), bool);
/// @brief Unlocks a mutex
/// @param self Pointer to the mutex to unlock
extern fn_(Thrd_Mtx_unlock(Thrd_Mtx* self), void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_MTX_INCLUDED */

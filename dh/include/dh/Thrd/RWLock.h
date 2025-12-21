/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    RWLock.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2025-12-20 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd_RWLock
 *
 * @brief   Read-Write lock for thread management
 * @details Defines read-write lock for thread management.
 */
#ifndef Thrd_RWLock__included
#define Thrd_RWLock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"

/*========== Macros and Declarations ========================================*/

struct Thrd_RWLock {
    Thrd_RWLock_Impl impl;
};
$extern fn_((Thrd_RWLock_init(void))(Thrd_RWLock));
$extern fn_((Thrd_RWLock_fini(Thrd_RWLock* self))(void));
$extern fn_((Thrd_RWLock_lock(Thrd_RWLock* self))(void));
$extern fn_((Thrd_RWLock_tryLock(Thrd_RWLock* self))(bool));
$extern fn_((Thrd_RWLock_unlock(Thrd_RWLock* self))(void));
$extern fn_((Thrd_RWLock_lockShared(Thrd_RWLock* self))(void));
$extern fn_((Thrd_RWLock_tryLockShared(Thrd_RWLock* self))(bool));
$extern fn_((Thrd_RWLock_unlockShared(Thrd_RWLock* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Thrd_RWLock__included */

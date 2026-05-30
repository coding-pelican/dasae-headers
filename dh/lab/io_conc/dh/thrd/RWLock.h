/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    RWLock.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-20 (date of creation)
 * @updated 2025-12-20 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_RWLock
 *
 * @brief   Read-Write lock for thread management
 * @details Defines read-write lock for thread management.
 */
#ifndef thrd_RWLock__included
#define thrd_RWLock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Sem.h"

/*========== Macros and Declarations ========================================*/

#if !defined(thrd_RWLock_use_pthread)
#define thrd_RWLock_use_pthread __comp_bool__thrd_RWLock_use_pthread
#endif /* !defined(thrd_RWLock_use_pthread) */
#define __comp_bool__thrd_RWLock_use_pthread thrd_RWLock__use_pthread_default

#define thrd_RWLock__use_pthread_default __comp_bool__thrd_RWLock__use_pthread_default
#define __comp_bool__thrd_RWLock__use_pthread_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_default_(thrd_use_pthread) \
    ) pp_end \
)

struct thrd_RWLock__Impl pp_if_(thrd_RWLock_use_pthread)(
    pp_then_({
        var_(unused_, Void);
    }),
    pp_else_({
        var_(state, usize);
        var_(mtx, thrd_Mtx);
        var_(sem, thrd_Sem);
    }));
struct thrd_RWLock pp_if_(thrd_RWLock_use_pthread)(
    pp_then_({ var_(impl, pthread_rwlock_t); }),
    pp_else_({ var_(impl, thrd_RWLock__Impl); }));
#if thrd_RWLock_use_pthread
#define thrd_RWLock_init_static() \
    { .impl = PTHREAD_RWLOCK_INITIALIZER }
#elif !thrd_Cond_use_pthread
#define thrd_RWLock_init_static() \
    { \
        .impl = {.state = 0, \
                 .mtx = thrd_Mtx_init_static(), \
                 .sem = thrd_Sem_init_static() } \
    }
#endif
$extern fn_((thrd_RWLock_init(void))(thrd_RWLock));
$extern fn_((thrd_RWLock_fini(thrd_RWLock* self))(void));
$extern fn_((thrd_RWLock_lock(thrd_RWLock* self))(void));
$extern fn_((thrd_RWLock_tryLock(thrd_RWLock* self))(bool));
$extern fn_((thrd_RWLock_unlock(thrd_RWLock* self))(void));
$extern fn_((thrd_RWLock_lockShared(thrd_RWLock* self))(void));
$extern fn_((thrd_RWLock_tryLockShared(thrd_RWLock* self))(bool));
$extern fn_((thrd_RWLock_unlockShared(thrd_RWLock* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_RWLock__included */

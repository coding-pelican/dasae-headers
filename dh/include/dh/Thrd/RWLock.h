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
#include "Mtx.h"
#include "Sem.h"

/*========== Macros and Declarations ========================================*/

#define Thrd_RWLock__use_pthread __comp_bool__Thrd_RWLock__use_pthread
#define __comp_bool__Thrd_RWLock__use_pthread pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_false)), \
        pp_case_((plat_type_linux)(pp_false)), \
        pp_case_((plat_type_darwin)(pp_false)), \
        pp_default_(pp_true) \
    ) pp_end \
)
struct Thrd_RWLock__Impl pp_if_(pp_not(Thrd_RWLock__use_pthread))(
    pp_then_({
        var_(state, usize);
        var_(mtx, Thrd_Mtx);
        var_(sem, Thrd_Sem);
    }),
    pp_else_({
        var_(unused_, Void);
    })
);
struct Thrd_RWLock pp_switch_((plat_type)(
    pp_case_((plat_type_windows)({ var_(impl, Thrd_RWLock__Impl); })),
    pp_case_((plat_type_linux)({ var_(impl, Thrd_RWLock__Impl); })),
    pp_case_((plat_type_darwin)({ var_(impl, Thrd_RWLock__Impl); })),
    pp_default_({ var_(impl, pthread_rwlock_t); })
));
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

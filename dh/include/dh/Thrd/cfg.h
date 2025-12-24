/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-06-04 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd
 *
 * @brief   Configuration settings for thread management
 * @details Defines compile-time options and configuration parameters
 *          for thread management.
 */
#ifndef THRD_CFG_INCLUDED
#define THRD_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/atom.h"

/*========== Macros and Definitions =========================================*/

#define Thrd__use_pthread __comp_bool__Thrd_use_pthread
#define __comp_bool__Thrd_use_pthread pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_false)), \
        pp_case_((plat_type_linux)(pp_false)), \
        pp_case_((plat_type_darwin)(pp_true)), \
        pp_default_(pp_true) \
    ) pp_end \
)

#if plat_is_windows
#include "dh/os/windows.h"
#endif /* plat_is_windows */
#if plat_is_darwin
#include <os/lock.h>
#endif /* plat_is_darwin */
#if Thrd__use_pthread
#include <pthread.h>
#endif /* Thrd__use_pthread */

typedef usize Thrd_Id__Impl;
#define Thrd_invalid_id usize_limit_max
typedef pp_if_(Thrd__use_pthread)(
    pp_then_(pthread_t),
    pp_else_(pp_switch_((plat_type)(
        pp_case_((plat_type_windows)(HANDLE)),
        pp_case_((plat_type_linux)(i32)),
        pp_case_((plat_type_darwin)(i32)),
        pp_default_(Void)
    )))
) Thrd_Handle__Impl;
#define Thrd_max_name_len (15)
T_use_atom_V$(u32); /* for Thrd_Ftx, Thrd_Mtx */
typedef struct Thrd_Mtx__Impl Thrd_Mtx__Impl;
typedef struct Thrd_Cond__Impl Thrd_Cond__Impl;
typedef struct Thrd_RWLock__Impl Thrd_RWLock__Impl;
T_use_atom_V$(usize); /* for Thrd_WaitGroup */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_CFG_INCLUDED */

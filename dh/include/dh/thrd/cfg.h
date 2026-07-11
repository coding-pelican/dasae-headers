/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2026-05-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd
 *
 * @brief   Configuration settings for thread management
 * @details Defines compile-time options and configuration parameters
 *          for thread management.
 */
#pragma once
#ifndef thrd_cfg__included
#define thrd_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/atom.h"

/*========== Macros and Definitions =========================================*/

#if !defined(thrd_use_pthread)
#define thrd_use_pthread __comp_bool__thrd_use_pthread
#endif /* !defined(thrd_use_pthread) */
#define __comp_bool__thrd_use_pthread thrd__use_pthread_default

#define thrd__use_pthread_default __comp_bool__thrd__use_pthread_default
#define __comp_bool__thrd__use_pthread_default pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_false)), \
        pp_case_((plat_type_linux)(pp_false)), \
        pp_case_((plat_type_darwin)(pp_false)), \
        pp_case_((plat_type_wasi)(pp_false)), \
        pp_default_(pp_false) \
    ) pp_end \
)

#if thrd_use_pthread
#include <pthread.h>
#endif /* thrd_use_pthread */
#if plat_is_windows
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/thrd.h"
#include "dh/sys/api/windows/sync.h"
#endif /* plat_is_windows */
typedef usize thrd_Id__Impl;
#define thrd_invalid_id __comp_const__thrd_invalid_id
#define __comp_const__thrd_invalid_id usize_limit_max
typedef pp_if_(thrd_use_pthread)(
    pp_then_(pthread_t),
    pp_else_(pp_switch_((plat_type)(
        pp_case_((plat_type_windows)(HANDLE)),
        pp_case_((plat_type_linux)(i32)),
        pp_case_((plat_type_darwin)(i32)),
        pp_case_((plat_type_wasi)(i32)),
        pp_default_(Void)
    )))) thrd_Handle__Impl;
#define thrd_max_name_len __comp_const__thrd_max_name_len
#define __comp_const__thrd_max_name_len (15)
typedef struct thrd_Mtx__Impl thrd_Mtx__Impl;
typedef struct thrd_Cond__Impl thrd_Cond__Impl;
typedef struct thrd_RWLock__Impl thrd_RWLock__Impl;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_cfg__included */

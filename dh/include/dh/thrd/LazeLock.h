/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    LazeLock.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_LazeLock
 *
 * @brief   Lazily initialized storage for thread management
 */
#pragma once
#ifndef thrd_LazeLock__included
#define thrd_LazeLock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "OnceLock.h"

/*========== Macros and Declarations ========================================*/

#define thrd_LazeLock$(_T...) tpl$(thrd_LazeLock, _T)
#define thrd_LazeLock$$(_T...) \
    struct { \
        var_(once, thrd_Once); \
        var_(value, _T); \
    }
#define T_decl_thrd_LazeLock$(_T...) \
    $maybe_unused typedef struct thrd_LazeLock$(_T) thrd_LazeLock$(_T);
#define T_impl_thrd_LazeLock$(_T...) \
    struct thrd_LazeLock$(_T) { \
        var_(once, thrd_Once); \
        var_(value, _T); \
    }
#define T_use_thrd_LazeLock$(_T...) \
    T_decl_thrd_LazeLock$(_T); \
    T_impl_thrd_LazeLock$(_T)

#define thrd_LazeLock_init$(_T...) ____thrd_LazeLock_init$(_T)
#define thrd_LazeLock_isSet(_p_self) thrd_Once_isDone(&(_p_self)->once)
#define thrd_LazeLock_get(_p_self) (&(_p_self)->value)
#define thrd_LazeLock_getOrInit(_p_self, _init_expr) __step__thrd_LazeLock_getOrInit(_p_self, _init_expr)

/*========== Macros and Definitions =========================================*/

#define ____thrd_LazeLock_init$(_T...) l$((thrd_LazeLock$(_T)){ \
    .once = thrd_Once_init_static(), \
    .value = cleared(), \
})
#define __step__thrd_LazeLock_getOrInit(_p_self, _init_expr) ({ \
    if (thrd_Once_tryBegin(&(_p_self)->once)) { \
        (_p_self)->value = (_init_expr); \
        thrd_Once_finish(&(_p_self)->once); \
    } else if (!thrd_Once_isDone(&(_p_self)->once)) { \
        thrd_Once_wait(&(_p_self)->once); \
    } \
    &(_p_self)->value; \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_LazeLock__included */

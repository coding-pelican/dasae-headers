/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    OnceLock.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_OnceLock
 *
 * @brief   Once-initialized storage for thread management
 */
#pragma once
#ifndef thrd_OnceLock__included
#define thrd_OnceLock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Once.h"

/*========== Macros and Declarations ========================================*/

#define thrd_OnceLock$(_T...) tpl$(thrd_OnceLock, _T)
#define thrd_OnceLock$$(_T...) \
    struct { \
        var_(once, thrd_Once); \
        var_(value, _T); \
    }
#define T_decl_thrd_OnceLock$(_T...) \
    $maybe_unused typedef struct thrd_OnceLock$(_T) thrd_OnceLock$(_T);
#define T_impl_thrd_OnceLock$(_T...) \
    struct thrd_OnceLock$(_T) { \
        var_(once, thrd_Once); \
        var_(value, _T); \
    }
#define T_use_thrd_OnceLock$(_T...) \
    T_decl_thrd_OnceLock$(_T); \
    T_impl_thrd_OnceLock$(_T)

#define thrd_OnceLock_init$(_T...) \
    ____thrd_OnceLock_init$(_T)
#define thrd_OnceLock_isSet(_p_self) thrd_Once_isDone(&(_p_self)->once)
#define thrd_OnceLock_wait(_p_self) thrd_Once_wait(&(_p_self)->once)
#define thrd_OnceLock_get(_p_self) (&(_p_self)->value)
#define thrd_OnceLock_trySet(_p_self, _val) __step__thrd_OnceLock_trySet(_p_self, _val)

/*========== Macros and Definitions =========================================*/

#define ____thrd_OnceLock_init$(_T...) l$((thrd_OnceLock$(_T)){ \
    .once = thrd_Once_init_static(), \
    .value = cleared(), \
})
#define __step__thrd_OnceLock_trySet(_p_self, _val) ({ \
    let_(__ok, bool) = thrd_Once_tryBegin(&(_p_self)->once); \
    if (__ok) { \
        (_p_self)->value = (_val); \
        thrd_Once_finish(&(_p_self)->once); \
    } \
    __ok; \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_OnceLock__included */

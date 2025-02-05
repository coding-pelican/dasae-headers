/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    assert_static.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/claim
 * @prefix  claim_assert_static
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef CLAIM_ASSERT_STATIC_INCLUDED
#define CLAIM_ASSERT_STATIC_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define claim_assert_static(_Expr) \
    FUNC__claim_assert_static(_Expr)

#define claim_assert_static_msg(_Expr, _msg) \
    FUNC__claim_assert_static_msg(_Expr, _msg)

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus) && (__cplusplus >= 201103L)
/* C++11 or later - static_assert is available */
#define FUNC__claim_assert_static(_Expr)           static_assert(_Expr, "Failed assertion")
#define FUNC__claim_assert_static_msg(_Expr, _msg) static_assert(_Expr, _msg)
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
/* C11 or later - _Static_assert is available */
#define FUNC__claim_assert_static(_Expr)           _Static_assert(_Expr, "Failed assertion")
#define FUNC__claim_assert_static_msg(_Expr, _msg) _Static_assert(_Expr, _msg)
#else
/* Older versions - emulate static assert */
#include "dh/core/pp.h"

#ifdef __COUNTER__
#define FUNC__claim_assert_static(_Expr) \
    typedef int pp_cat(claim_assert_static_, __COUNTER__)[(_Expr) ? 1 : -1]
#define FUNC__claim_assert_static_msg(_Expr, _msg) \
    typedef int pp_cat(claim_assert_static_msg_, __COUNTER__)[(_Expr) ? 1 : -1]
#else
#define FUNC__claim_assert_static(_Expr) \
    typedef int pp_cat(claim_assert_static_, __LINE__)[(_Expr) ? 1 : -1]
#define FUNC__claim_assert_static_msg(_Expr, _msg) \
    typedef int pp_cat(claim_assert_static_msg_, __LINE__)[(_Expr) ? 1 : -1]
#endif
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_ASSERT_STATIC_INCLUDED */

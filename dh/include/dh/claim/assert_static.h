/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    assert_static.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2024-11-06 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/claim
 * @prefix  claim_assert_static_msg
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef CLAIM_ASSERT_STATIC_INCLUDED
#define CLAIM_ASSERT_STATIC_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

/*========== Macros and Definitions =========================================*/

#define claim_assert_static(_Expr) \
    IMPL_claim_assert_static(_Expr)

#define claim_assert_static_msg(_Expr, _msg) \
    IMPL_claim_assert_static_msg(_Expr, _msg)

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus) && (__cplusplus >= 201103L)
/* C++11 or later - static_assert is available */
#define IMPL_claim_assert_static(_Expr)           static_assert(_Expr, "Failed assertion")
#define IMPL_claim_assert_static_msg(_Expr, _msg) static_assert(_Expr, _msg)
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
/* C11 or later - _Static_assert is available */
#define IMPL_claim_assert_static(_Expr)           _Static_assert(_Expr, "Failed assertion")
#define IMPL_claim_assert_static_msg(_Expr, _msg) _Static_assert(_Expr, _msg)
#else
/* Older versions - emulate static assert */

#include "../core/pp.h"
#include "../core/prim.h"

#ifdef __COUNTER__
#define IMPL_claim_assert_static(_Expr) \
    typedef i8 pp_concat(claim_assert_static_, __COUNTER__)[(_Expr) ? 1 : -1]
#define IMPL_claim_assert_static_msg(_Expr, _msg) \
    typedef i8 pp_concat(claim_assert_static_msg_, __COUNTER__)[(_Expr) ? 1 : -1]
#else
#define IMPL_claim_assert_static(_Expr) \
    typedef i8 pp_concat(claim_assert_static_, __LINE__)[(_Expr) ? 1 : -1]
#define IMPL_claim_assert_static_msg(_Expr, _msg) \
    typedef i8 pp_concat(claim_assert_static_msg_, __LINE__)[(_Expr) ? 1 : -1]
#endif

#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_ASSERT_STATIC_INCLUDED */

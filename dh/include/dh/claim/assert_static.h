/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    assert_static.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2024-11-06 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/claim
 * @prefix  claim
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

#ifndef claim_assertStatic
#define claim_assertStatic(EXPR, MSG) \
    IMPL_claim_assertStatic(EXPR, MSG)
#endif /* static_assert */

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus) && (__cplusplus >= 201103L)
/* C++11 or later - static_assert is available */
#define IMPL_claim_assertStatic(EXPR, MSG) static_assert(EXPR, MSG)
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
/* C11 or later - _Static_assert is available */
#define IMPL_claim_assertStatic(EXPR, MSG) _Static_assert(EXPR, MSG)
#else
/* Older versions - emulate static assert */

#include "../core/pp.h"
#include "../core/prim.h"

#ifdef __COUNTER__
#define IMPL_claim_assertStatic(EXPR, MSG) \
    typedef i8 pp_concat(claim_assertStatic_, __COUNTER__)[(EXPR) ? 1 : -1]
#else
#define IMPL_claim_assertStatic(EXPR, MSG) \
    typedef i8 pp_concat(claim_assertStatic_, __LINE__)[(EXPR) ? 1 : -1]
#endif

#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_ASSERT_STATIC_INCLUDED */

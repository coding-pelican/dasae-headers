/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    claim_assert_static.h
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
#define claim_assertStatic(_expr, _msg) \
    RETURN_claim_assertStatic(_expr, _msg)
#endif /* static_assert */

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus) && (__cplusplus >= 201103L)
/* C++11 or later - static_assert is available */
#define RETURN_claim_assertStatic(_expr, _msg) static_assert(_expr, _msg)
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
/* C11 or later - _Static_assert is available */
#define RETURN_claim_assertStatic(_expr, _msg) _Static_assert(_expr, _msg)
#else
/* Older versions - emulate static assert */

#include "../core/core_pp.h"
#include "../core/prim/prim.h"

#ifdef __COUNTER__
#define RETURN_claim_assertStatic(_expr, _msg) \
    typedef i8 pp_concat(claim_assertStatic_, __COUNTER__)[(_expr) ? 1 : -1]
#else
#define RETURN_claim_assertStatic(_expr, _msg) \
    typedef i8 pp_concat(claim_assertStatic_, __LINE__)[(_expr) ? 1 : -1]
#endif

#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_ASSERT_STATIC_INCLUDED */

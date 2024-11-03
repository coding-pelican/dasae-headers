/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    static_assert.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2024-11-02 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/static
 * @prefix  static
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef STATIC_ASSERT_INCLUDED
#define STATIC_ASSERT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

/*========== Macros and Definitions =========================================*/

#ifndef static_assert
#define static_assert(_expr, _msg) \
    RETURN_static_assert(_expr, _msg)
#endif /* static_assert */

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus) && (__cplusplus >= 201103L)
/* C++11 or later - static_assert is available */
#define RETURN_static_assert(_expr, _msg) static_assert(_expr, _msg)
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
/* C11 or later - _Static_assert is available */
#define RETURN_static_assert(_expr, _msg) _Static_assert(_expr, _msg)
#else
/* Older versions - emulate static assert */

#include "../preprocessor.h"
#include "../types.h"

#ifdef __COUNTER__
#define RETURN_static_assert(_expr, _msg) \
    typedef i8 pp_concat(static_assertion_, __COUNTER__)[(_expr) ? 1 : -1]
#else
#define RETURN_static_assert(_expr, _msg) \
    typedef i8 pp_concat(static_assertion_, __LINE__)[(_expr) ? 1 : -1]
#endif

#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* STATIC_ASSERT_INCLUDED */

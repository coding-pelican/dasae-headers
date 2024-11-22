/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    bool.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-11-10 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  bool
 *
 * @brief   Header for boolean type and operations
 * @details Provides boolean type definitions and operations for C/C++ compatibility
 */

#ifndef CORE_PRIM_BOOL_INCLUDED
#define CORE_PRIM_BOOL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

/* Boolean type definition */
#if !defined(__cplusplus)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
// #include <stdbool.h>
#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L
/* C2x compatibility */
#else
/* C99 compatibility */
#define bool  _Bool
#define true  (1)
#define false (0)
#endif
#else
/* Pre-C99 compatibility */
typedef unsigned char _Bool;
#define bool  _Bool
#define true  (1)
#define false (0)
#endif
#endif

/* Boolean operations */
#define bool_and(_lhs, _rhs) ((_lhs) && (_rhs))
#define bool_or(_lhs, _rhs)  ((_lhs) || (_rhs))
#define bool_not(_val)       (!(_val))
#define bool_xor(_lhs, _rhs) ((bool)((_lhs) != (_rhs)))

/* Type conversion */
#define bool_from(_val)      ((bool)(!!(_val)))
#define bool_toInt(_T, _val) ((_T)(_val))
#define bool_const(_val)     ((bool)(!!(_val)))

/* Size verification */
claim_assert_static_msg(sizeof(bool) == 1, "bool must be 1 byte");

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* CORE_PRIM_BOOL_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    prim_bool.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef PRIM_BOOL_INCLUDED
#define PRIM_BOOL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "prim_cfg.h"

/*========== Macros and Definitions =========================================*/

/* Boolean type definition */
#if !defined(__cplusplus)
/* C99 or later - use _Bool */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <stdbool.h>
#else
/* Pre-C99 - define bool manually */
#define bool  _Bool
#define true  (1)
#define false (0)
typedef unsigned char _Bool;
#endif
#endif /* !defined(__cplusplus) */

/* Boolean operations that work identically in C and C++ */
#define bool_and(_lhs, _rhs) ((_lhs) && (_rhs))
#define bool_or(_lhs, _rhs)  ((_lhs) || (_rhs))
#define bool_not(_x)         (!(_x))
#define bool_xor(_lhs, _rhs) ((bool)((_lhs) != (_rhs)))

/* Boolean conversion macros */
#define bool_from(_x)      ((bool)(!!(_x)))
#define bool_toInt(_T, _x) ((_T)(_x))

#define bool_const(_x)                           \
    /* Compile-time boolean constant creation */ \
    ((bool)(!!(_x)))

// Ensure bool is always exactly 1 byte
// typedef char ensure_bool_size[(sizeof(bool) == 1) ? 1 : -1];
claim_assertStatic(sizeof(bool) == 1, "bool must be 1 byte");


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* PRIM_BOOL_INCLUDED */

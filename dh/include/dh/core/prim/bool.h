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


#ifndef PRIM_BOOL_INCLUDED
#define PRIM_BOOL_INCLUDED (1)
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
#define bool_and(LHS_VAL, RHS_VAL) ((LHS_VAL) && (RHS_VAL))
#define bool_or(LHS_VAL, RHS_VAL)  ((LHS_VAL) || (RHS_VAL))
#define bool_not(VAL)              (!(VAL))
#define bool_xor(LHS_VAL, RHS_VAL) ((bool)((LHS_VAL) != (RHS_VAL)))

/* Type conversion */
#define bool_from(VAL)        ((bool)(!!(VAL)))
#define bool_toInt(TYPE, VAL) ((TYPE)(VAL))
#define bool_const(VAL)       ((bool)(!!(VAL)))

/* Size verification */
claim_assertStatic(sizeof(bool) == 1, "bool must be 1 byte");


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* PRIM_BOOL_INCLUDED */

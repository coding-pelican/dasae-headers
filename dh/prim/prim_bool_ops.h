/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    prim_bool_op.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef PRIM_BOOL_OPS_INCLUDED
#define PRIM_BOOL_OPS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "prim_bool.h"

/*========== Macros and Definitions =========================================*/

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


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* PRIM_BOOL_OPS_INCLUDED */

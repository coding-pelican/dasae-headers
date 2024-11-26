/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ops.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-24 (date of creation)
 * @updated 2024-11-24 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  ops
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef CORE_OPS_INCLUDED
#define CORE_OPS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin.h"
#include "dh/core/prim/ptr.h"
#include "dh/core/pp.h"

/*========== Macros and Definitions =========================================*/

// #define op_add(TSelf) pp_join(_, TSelf, add)
// #define op_sub(TSelf) pp_join(_, TSelf, sub)
// #define op_mul(TSelf) pp_join(_, TSelf, mul)
// #define op_div(TSelf) pp_join(_, TSelf, div)
// #define op_rem(TSelf) pp_join(_, TSelf, rem)

// #define op_add_(TSelf, TOther) pp_join3(_, TSelf, add, TOther)
// #define op_sub_(TSelf, TOther) pp_join3(_, TSelf, sub, TOther)
// #define op_mul_(TSelf, TOther) pp_join3(_, TSelf, mul, TOther)
// #define op_div_(TSelf, TOther) pp_join3(_, TSelf, div, TOther)
// #define op_rem_(TSelf, TOther) pp_join3(_, TSelf, rem, TOther)

// #define try_add(TSelf) pp_join(_, TSelf, try_add)
// #define try_sub(TSelf) pp_join(_, TSelf, try_sub)
// #define try_mul(TSelf) pp_join(_, TSelf, try_mul)
// #define try_div(TSelf) pp_join(_, TSelf, try_div)
// #define try_rem(TSelf) pp_join(_, TSelf, try_rem)

// #define try_add_(TSelf, TOther) pp_join3(_, TSelf, try_add, TOther)
// #define try_sud_(TSelf, TOther) pp_join3(_, TSelf, try_sub, TOther)
// #define try_mud_(TSelf, TOther) pp_join3(_, TSelf, try_mul, TOther)
// #define try_did_(TSelf, TOther) pp_join3(_, TSelf, try_div, TOther)
// #define try_red_(TSelf, TOther) pp_join3(_, TSelf, try_rem, TOther)

/* Function-like macros */
#define ops_fn(TRet, fnOps...) static_inline TRet fnOps

/* Binary operators */
#define ops_add(TSelf)               pp_join(_, TSelf, add)(TSelf self, TSelf other)
#define ops_add_other(TSelf, TOther) pp_join3(_, TSelf, add, TOther)(TSelf self, TOther other)

#define ops_sub(TSelf)               pp_join(_, TSelf, sub)(TSelf self, TSelf other)
#define ops_sub_other(TSelf, TOther) pp_join3(_, TSelf, sub, TOther)(TSelf self, TOther other)

#define ops_mul(TSelf)               pp_join(_, TSelf, mul)(TSelf self, TSelf other)
#define ops_mul_other(TSelf, TOther) pp_join3(_, TSelf, mul, TOther)(TSelf self, TOther other)

#define ops_div(TSelf)               pp_join(_, TSelf, div)(TSelf self, TSelf other)
#define ops_div_other(TSelf, TOther) pp_join3(_, TSelf, div, TOther)(TSelf self, TOther other)

#define ops_rem(TSelf)               pp_join(_, TSelf, rem)(TSelf self, TSelf other)
#define ops_rem_other(TSelf, TOther) pp_join3(_, TSelf, rem, TOther)(TSelf self, TOther other)

/* Try Binary operators */
#define ops_try_add(TSelf, TOut)               pp_join(_, TSelf, try_add)(TSelf self, TSelf other, rawptr(TOut) const out)
#define ops_try_add_other(TSelf, TOther, TOut) pp_join3(_, TSelf, try_add, TOther)(TSelf self, TOther other, rawptr(TOut) const out)

#define ops_try_sub(TSelf, TOut)               pp_join(_, TSelf, try_sub)(TSelf self, TSelf other, rawptr(TOut) const out)
#define ops_try_sub_other(TSelf, TOther, TOut) pp_join3(_, TSelf, try_sub, TOther)(TSelf self, TOther other, rawptr(TOut) const out)

#define ops_try_mul(TSelf, TOut)               pp_join(_, TSelf, try_mul)(TSelf self, TSelf other, rawptr(TOut) const out)
#define ops_try_mul_other(TSelf, TOther, TOut) pp_join3(_, TSelf, try_mul, TOther)(TSelf self, TOther other, rawptr(TOut) const out)

#define ops_try_div(TSelf, TOut)               pp_join(_, TSelf, try_div)(TSelf self, TSelf other, rawptr(TOut) const out)
#define ops_try_div_other(TSelf, TOther, TOut) pp_join3(_, TSelf, try_div, TOther)(TSelf self, TOther other, rawptr(TOut) const out)

#define ops_try_rem(TSelf, TOut)               pp_join(_, TSelf, try_rem)(TSelf self, TSelf other, rawptr(TOut) const out)
#define ops_try_rem_other(TSelf, TOther, TOut) pp_join3(_, TSelf, try_rem, TOther)(TSelf self, TOther other, rawptr(TOut) const out)

/* Unary operators */
#define ops_neg(TSelf) pp_join(_, TSelf, neg)(TSelf self)

/* Try Unary operators */
#define ops_try_neg(TSelf, TOut) pp_join(_, TSelf, try_neg)(TSelf self, rawptr(TOut) const out)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
ops_fn(int, ops_add(int)) { return self + other; }
ops_fn(int, ops_rem(int)) { return self % other; }
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_OPS_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    op.h
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

/* Unary operators */
#define op_neg(TSelf) pp_join(_, TSelf, neg)

/* Binary operators */
#define op_add(TSelf) pp_join(_, TSelf, add)
#define op_sub(TSelf) pp_join(_, TSelf, sub)
#define op_mul(TSelf) pp_join(_, TSelf, mul)
#define op_div(TSelf) pp_join(_, TSelf, div)
#define op_rem(TSelf) pp_join(_, TSelf, rem)

#define op_addBy(TSelf, TOther) pp_join3(_, TSelf, add, TOther)
#define op_subBy(TSelf, TOther) pp_join3(_, TSelf, sub, TOther)
#define op_mulBy(TSelf, TOther) pp_join3(_, TSelf, mul, TOther)
#define op_divBy(TSelf, TOther) pp_join3(_, TSelf, div, TOther)
#define op_remBy(TSelf, TOther) pp_join3(_, TSelf, rem, TOther)

/* Function-like macros */
#define op_fnUn(fnOps, TSelf)            pp_join(_, op, fnOps)(TSelf)(TSelf self)
#define op_fnBin(fnOps, TSelf)           pp_join(_, op, fnOps)(TSelf)(TSelf self, TSelf other)
#define op_fnBinBy(fnOps, TSelf, TOther) pp_join(_, op, fnOps)(TSelf, TOther)(TSelf self, TOther other)
#define op_fnNeg(TSelf)                  op_fnUn(neg, TSelf)
#define op_fnAdd(TSelf)                  op_fnBin(add, TSelf)
#define op_fnSub(TSelf)                  op_fnBin(sub, TSelf)
#define op_fnMul(TSelf)                  op_fnBin(mul, TSelf)
#define op_fnDiv(TSelf)                  op_fnBin(div, TSelf)
#define op_fnRem(TSelf)                  op_fnBin(rem, TSelf)
#define op_fnAddBy(TSelf, TOther)        op_fnBinBy(addBy, TSelf, TOther)
#define op_fnSubBy(TSelf, TOther)        op_fnBinBy(subBy, TSelf, TOther)
#define op_fnMulBy(TSelf, TOther)        op_fnBinBy(mulBy, TSelf, TOther)
#define op_fnDivBy(TSelf, TOther)        op_fnBinBy(divBy, TSelf, TOther)
#define op_fnRemBy(TSelf, TOther)        op_fnBinBy(remBy, TSelf, TOther)

#define op_fnWrapAddBy(fnName, TSelf, TOther) \
    pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return op_addBy(TSelf, TOther)(self, other); }
#define op_fnWrapSubBy(fnName, TSelf, TOther) \
    pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return op_subBy(TSelf, TOther)(self, other); }
#define op_fnWrapMulBy(fnName, TSelf, TOther) \
    pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return op_mulBy(TSelf, TOther)(self, other); }
#define op_fnWrapDivBy(fnName, TSelf, TOther) \
    pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return op_divBy(TSelf, TOther)(self, other); }
#define op_fnWrapRemBy(fnName, TSelf, TOther) \
    pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return op_remBy(TSelf, TOther)(self, other); }

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
/* Built-in types */
force_inline i8 op_fnUn(neg, i8) { return (i8)(-self); }
force_inline i8 op_fnBin(add, i8) { return (i8)(self + other); }
force_inline i8 op_fnBin(sub, i8) { return (i8)(self - other); }
force_inline i8 op_fnBin(mul, i8) { return (i8)(self * other); }
force_inline i8 op_fnBin(div, i8) { return (i8)(self / other); }
force_inline i8 op_fnBin(rem, i8) { return (i8)(self % other); }

force_inline i32 op_fnAdd(i32) { return self + other; }
force_inline i32 op_fnSub(i32) { return self - other; }
force_inline i32 op_fnMul(i32) { return self * other; }
force_inline i32 op_fnDiv(i32) { return self / other; }
force_inline i32 op_fnRem(i32) { return self % other; }
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_OPS_INCLUDED */

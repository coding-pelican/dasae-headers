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

/* Unary operators */
#define ops_neg(TSelf) pp_join(_, TSelf, neg)

/* Binary operators */
#define ops_add(TSelf) pp_join(_, TSelf, add)
#define ops_sub(TSelf) pp_join(_, TSelf, sub)
#define ops_mul(TSelf) pp_join(_, TSelf, mul)
#define ops_div(TSelf) pp_join(_, TSelf, div)
#define ops_rem(TSelf) pp_join(_, TSelf, rem)

#define ops_addBy(TSelf, TOther) pp_join3(_, TSelf, add, TOther)
#define ops_subBy(TSelf, TOther) pp_join3(_, TSelf, sub, TOther)
#define ops_mulBy(TSelf, TOther) pp_join3(_, TSelf, mul, TOther)
#define ops_divBy(TSelf, TOther) pp_join3(_, TSelf, div, TOther)
#define ops_remBy(TSelf, TOther) pp_join3(_, TSelf, rem, TOther)

/* Function-like macros */
#define ops_fnUn(fnOps, TSelf)            pp_join(_, ops, fnOps)(TSelf)(TSelf self)
#define ops_fnBin(fnOps, TSelf)           pp_join(_, ops, fnOps)(TSelf)(TSelf self, TSelf other)
#define ops_fnBinBy(fnOps, TSelf, TOther) pp_join(_, ops, fnOps)(TSelf, TOther)(TSelf self, TOther other)
#define ops_fnNeg(TSelf)                  ops_fnUn(neg, TSelf)
#define ops_fnAdd(TSelf)                  ops_fnBin(add, TSelf)
#define ops_fnSub(TSelf)                  ops_fnBin(sub, TSelf)
#define ops_fnMul(TSelf)                  ops_fnBin(mul, TSelf)
#define ops_fnDiv(TSelf)                  ops_fnBin(div, TSelf)
#define ops_fnRem(TSelf)                  ops_fnBin(rem, TSelf)
#define ops_fnAddBy(TSelf, TOther)        ops_fnBinBy(addBy, TSelf, TOther)
#define ops_fnSubBy(TSelf, TOther)        ops_fnBinBy(subBy, TSelf, TOther)
#define ops_fnMulBy(TSelf, TOther)        ops_fnBinBy(mulBy, TSelf, TOther)
#define ops_fnDivBy(TSelf, TOther)        ops_fnBinBy(divBy, TSelf, TOther)
#define ops_fnRemBy(TSelf, TOther)        ops_fnBinBy(remBy, TSelf, TOther)

#define ops_fnWrapAddBy(fnName, TSelf, TOther) \
    pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return ops_addBy(TSelf, TOther)(self, other); }
#define ops_fnWrapSubBy(fnName, TSelf, TOther) \
    pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return ops_subBy(TSelf, TOther)(self, other); }
#define ops_fnWrapMulBy(fnName, TSelf, TOther) \
    pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return ops_mulBy(TSelf, TOther)(self, other); }
#define ops_fnWrapDivBy(fnName, TSelf, TOther) \
    pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return ops_divBy(TSelf, TOther)(self, other); }
#define ops_fnWrapRemBy(fnName, TSelf, TOther) \
    pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return ops_remBy(TSelf, TOther)(self, other); }

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
/* Built-in types */
force_inline i8 ops_fnUn(neg, i8) { return (i8)(-self); }
force_inline i8 ops_fnBin(add, i8) { return (i8)(self + other); }
force_inline i8 ops_fnBin(sub, i8) { return (i8)(self - other); }
force_inline i8 ops_fnBin(mul, i8) { return (i8)(self * other); }
force_inline i8 ops_fnBin(div, i8) { return (i8)(self / other); }
force_inline i8 ops_fnBin(rem, i8) { return (i8)(self % other); }

force_inline i32 ops_fnAdd(i32) { return self + other; }
force_inline i32 ops_fnSub(i32) { return self - other; }
force_inline i32 ops_fnMul(i32) { return self * other; }
force_inline i32 ops_fnDiv(i32) { return self / other; }
force_inline i32 ops_fnRem(i32) { return self % other; }
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_OPS_INCLUDED */

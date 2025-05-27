/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    op.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-24 (date of creation)
 * @updated 2024-11-24 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  ops
 *
 * @brief   Operator utilities
 * @details Provides operator utilities and functions for:
 *          - Unary operators
 *          - Binary operators
 *          - Function-like macros
 *          - Wrapper macros
 *          - Externalized static functions
 */

#ifndef CORE_OPS_INCLUDED
#define CORE_OPS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

/* Unary operators */
#define op_neg(TSelf) pp_join(_, TSelf, neg)
#define op_abs(TSelf) pp_join(_, TSelf, abs)

/* Binary operators */
#define op_add(TSelf)           pp_join(_, TSelf, add)
#define op_sub(TSelf)           pp_join(_, TSelf, sub)
#define op_mul(TSelf)           pp_join(_, TSelf, mul)
#define op_div(TSelf)           pp_join(_, TSelf, div)
#define op_rem(TSelf)           pp_join(_, TSelf, rem)
#define op_addBy(TSelf, TOther) pp_join3(_, TSelf, add, TOther)
#define op_subBy(TSelf, TOther) pp_join3(_, TSelf, sub, TOther)
#define op_mulBy(TSelf, TOther) pp_join3(_, TSelf, mul, TOther)
#define op_divBy(TSelf, TOther) pp_join3(_, TSelf, div, TOther)
#define op_remBy(TSelf, TOther) pp_join3(_, TSelf, rem, TOther)

#define op_asg(TSelf)              pp_join(_, TSelf, asg)
#define op_addAsg(TSelf)           pp_join(_, TSelf, addAsg)
#define op_subAsg(TSelf)           pp_join(_, TSelf, subAsg)
#define op_mulAsg(TSelf)           pp_join(_, TSelf, mulAsg)
#define op_divAsg(TSelf)           pp_join(_, TSelf, divAsg)
#define op_remAsg(TSelf)           pp_join(_, TSelf, remAsg)
#define op_addAsgBy(TSelf, TOther) pp_join3(_, TSelf, addAsg, TOther)
#define op_subAsgBy(TSelf, TOther) pp_join3(_, TSelf, subAsg, TOther)
#define op_mulAsgBy(TSelf, TOther) pp_join3(_, TSelf, mulAsg, TOther)
#define op_divAsgBy(TSelf, TOther) pp_join3(_, TSelf, divAsg, TOther)
#define op_remAsgBy(TSelf, TOther) pp_join3(_, TSelf, remAsg, TOther)

/* Function-like macros */
#define op_fnUn(fnOps, TSelf)               pp_join(_, op, fnOps)(TSelf)(TSelf self)
#define op_fnBin(fnOps, TSelf)              pp_join(_, op, fnOps)(TSelf)(TSelf self, TSelf other)
#define op_fnBinBy(fnOps, TSelf, TOther)    pp_join(_, op, fnOps)(TSelf, TOther)(TSelf self, TOther other)
#define op_fnBinAsg(fnOps, TSelf)           pp_join(_, op, fnOps)(TSelf)(rawptr$(TSelf) self, TSelf other)
#define op_fnBinAsgBy(fnOps, TSelf, TOther) pp_join(_, op, fnOps)(TSelf, TOther)(rawptr$(TSelf) self, TOther other)

#define op_fnNeg(TSelf)           op_fnUn(neg, TSelf)
#define op_fnAdd(TSelf)           op_fnBin(add, TSelf)
#define op_fnSub(TSelf)           op_fnBin(sub, TSelf)
#define op_fnMul(TSelf)           op_fnBin(mul, TSelf)
#define op_fnDiv(TSelf)           op_fnBin(div, TSelf)
#define op_fnRem(TSelf)           op_fnBin(rem, TSelf)
#define op_fnAddBy(TSelf, TOther) op_fnBinBy(addBy, TSelf, TOther)
#define op_fnSubBy(TSelf, TOther) op_fnBinBy(subBy, TSelf, TOther)
#define op_fnMulBy(TSelf, TOther) op_fnBinBy(mulBy, TSelf, TOther)
#define op_fnDivBy(TSelf, TOther) op_fnBinBy(divBy, TSelf, TOther)
#define op_fnRemBy(TSelf, TOther) op_fnBinBy(remBy, TSelf, TOther)

#define op_fnAsg(TSelf)              op_fnBinAsg(asg, TSelf)
#define op_fnAddAsg(TSelf)           op_fnBinAsg(addAsg, TSelf)
#define op_fnSubAsg(TSelf)           op_fnBinAsg(subAsg, TSelf)
#define op_fnMulAsg(TSelf)           op_fnBinAsg(mulAsg, TSelf)
#define op_fnDivAsg(TSelf)           op_fnBinAsg(divAsg, TSelf)
#define op_fnRemAsg(TSelf)           op_fnBinAsg(remAsg, TSelf)
#define op_fnAddAsgBy(TSelf, TOther) op_fnBinAsgBy(addAsgBy, TSelf, TOther)
#define op_fnSubAsgBy(TSelf, TOther) op_fnBinAsgBy(subAsgBy, TSelf, TOther)
#define op_fnMulAsgBy(TSelf, TOther) op_fnBinAsgBy(mulAsgBy, TSelf, TOther)
#define op_fnDivAsgBy(TSelf, TOther) op_fnBinAsgBy(divAsgBy, TSelf, TOther)
#define op_fnRemAsgBy(TSelf, TOther) op_fnBinAsgBy(remAsgBy, TSelf, TOther)

#if !COMP_TIME
#define op_fnWrapAddBy(fnName, TSelf, TOther, TRet) \
    TRet pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return op_addBy(TSelf, TOther)(self, other); }
#define op_fnWrapSubBy(fnName, TSelf, TOther, TRet) \
    TRet pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return op_subBy(TSelf, TOther)(self, other); }
#define op_fnWrapMulBy(fnName, TSelf, TOther, TRet) \
    TRet pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return op_mulBy(TSelf, TOther)(self, other); }
#define op_fnWrapDivBy(fnName, TSelf, TOther, TRet) \
    TRet pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return op_divBy(TSelf, TOther)(self, other); }
#define op_fnWrapRemBy(fnName, TSelf, TOther, TRet) \
    TRet pp_join(_, TSelf, fnName)(TSelf self, TOther other) { return op_remBy(TSelf, TOther)(self, other); }

#define op_fnWrapAddAsgBy(fnName, TSelf, TOther, TRet) \
    TRet pp_join(_, TSelf, fnName)(rawptr$(TSelf) self, TOther other) { return op_addAsgBy(TSelf, TOther)(self, other); }
#define op_fnWrapSubAsgBy(fnName, TSelf, TOther, TRet) \
    TRet pp_join(_, TSelf, fnName)(rawptr$(TSelf) self, TOther other) { return op_subAsgBy(TSelf, TOther)(self, other); }
#define op_fnWrapMulAsgBy(fnName, TSelf, TOther, TRet) \
    TRet pp_join(_, TSelf, fnName)(rawptr$(TSelf) self, TOther other) { return op_mulAsgBy(TSelf, TOther)(self, other); }
#define op_fnWrapDivAsgBy(fnName, TSelf, TOther, TRet) \
    TRet pp_join(_, TSelf, fnName)(rawptr$(TSelf) self, TOther other) { return op_divAsgBy(TSelf, TOther)(self, other); }
#define op_fnWrapRemAsgBy(fnName, TSelf, TOther, TRet) \
    TRet pp_join(_, TSelf, fnName)(rawptr$(TSelf) self, TOther other) { return op_remAsgBy(TSelf, TOther)(self, other); }
#else /* COMP_TIME */
#define op_fnWrapAddBy(fnName, TSelf, TOther, TRet) \
    let pp_join(_, TSelf, fnName) = op_addBy(TSelf, TOther)
#define op_fnWrapSubBy(fnName, TSelf, TOther, TRet) \
    let pp_join(_, TSelf, fnName) = op_subBy(TSelf, TOther)
#define op_fnWrapMulBy(fnName, TSelf, TOther, TRet) \
    let pp_join(_, TSelf, fnName) = op_mulBy(TSelf, TOther)
#define op_fnWrapDivBy(fnName, TSelf, TOther, TRet) \
    let pp_join(_, TSelf, fnName) = op_divBy(TSelf, TOther)
#define op_fnWrapRemBy(fnName, TSelf, TOther, TRet) \
    let pp_join(_, TSelf, fnName) = op_remBy(TSelf, TOther)

#define op_fnWrapAddAsgBy(fnName, TSelf, TOther, TRet) \
    let pp_join(_, TSelf, fnName) = op_addAsgBy(TSelf, TOther)
#define op_fnWrapSubAsgBy(fnName, TSelf, TOther, TRet) \
    let pp_join(_, TSelf, fnName) = op_subAsgBy(TSelf, TOther)
#define op_fnWrapMulAsgBy(fnName, TSelf, TOther, TRet) \
    let pp_join(_, TSelf, fnName) = op_mulAsgBy(TSelf, TOther)
#define op_fnWrapDivAsgBy(fnName, TSelf, TOther, TRet) \
    let pp_join(_, TSelf, fnName) = op_divAsgBy(TSelf, TOther)
#define op_fnWrapRemAsgBy(fnName, TSelf, TOther, TRet) \
    let pp_join(_, TSelf, fnName) = op_remAsgBy(TSelf, TOther)
#endif /* COMP_TIME */

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#if EXAMPLE_USAGE
$inline_always i8 op_fnUn(neg, i8) { return (i8)(-self); }
$inline_always i8 op_fnBin(add, i8) { return (i8)(self + other); }
$inline_always i8 op_fnBin(sub, i8) { return (i8)(self - other); }
$inline_always i8 op_fnBin(mul, i8) { return (i8)(self * other); }
$inline_always i8 op_fnBin(div, i8) { return (i8)(self / other); }
$inline_always i8 op_fnBin(rem, i8) { return (i8)(self % other); }

$inline_always i32 op_fnAdd(i32) { return self + other; }
$inline_always i32 op_fnSub(i32) { return self - other; }
$inline_always i32 op_fnMul(i32) { return self * other; }
$inline_always i32 op_fnDiv(i32) { return self / other; }
$inline_always i32 op_fnRem(i32) { return self % other; }

$inline_always i32* op_fnAsg(i32) { return *self = other, self; }
$inline_always i32* op_fnAddAsg(i32) { return *self = op_add(i32)(*self, other), self; }
$inline_always i32* op_fnSubAsg(i32) { return *self = op_sub(i32)(*self, other), self; }
$inline_always i32* op_fnMulAsg(i32) { return *self = op_mul(i32)(*self, other), self; }
$inline_always i32* op_fnDivAsg(i32) { return *self = op_div(i32)(*self, other), self; }
$inline_always i32* op_fnRemAsg(i32) { return *self = op_rem(i32)(*self, other), self; }
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_OPS_INCLUDED */

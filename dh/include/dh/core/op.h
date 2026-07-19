/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    op.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-24 (date of creation)
 * @updated 2024-11-24 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 *
 * @brief   Operator utilities
 * @details Provides operator utilities and functions for:
 *          - Unary operators
 *          - Binary operators
 *          - Function-like macros
 *          - Wrapper macros
 *          - Externalized static functions
 */
#pragma once
#ifndef core_op__included
#define core_op__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pri.h"

/*========== Macros and Declarations ========================================*/

/*--- Operators ---*/

#define op_add$(_$T) tpl_(_$T, add)
#define op_sub$(_$T) tpl_(_$T, sub)
#define op_mul$(_$T) tpl_(_$T, mul)
#define op_div$(_$T) tpl_(_$T, div)
#define op_rem$(_$T) tpl_(_$T, rem)

#define op_addAsg$(_$T) tpl_(_$T, addAsg)
#define op_subAsg$(_$T) tpl_(_$T, subAsg)
#define op_mulAsg$(_$T) tpl_(_$T, mulAsg)
#define op_divAsg$(_$T) tpl_(_$T, divAsg)
#define op_remAsg$(_$T) tpl_(_$T, remAsg)

#define op_addWith$(_$T, _$U) tpl$(tpl_(_$T, add), _$U)
#define op_subWith$(_$T, _$U) tpl$(tpl_(_$T, sub), _$U)
#define op_mulWith$(_$T, _$U) tpl$(tpl_(_$T, mul), _$U)
#define op_divWith$(_$T, _$U) tpl$(tpl_(_$T, div), _$U)
#define op_remWith$(_$T, _$U) tpl$(tpl_(_$T, rem), _$U)

#define op_addAsgWith$(_$T, _$U) tpl$(tpl_(_$T, addAsg), _$U)
#define op_subAsgWith$(_$T, _$U) tpl$(tpl_(_$T, subAsg), _$U)
#define op_mulAsgWith$(_$T, _$U) tpl$(tpl_(_$T, mulAsg), _$U)
#define op_divAsgWith$(_$T, _$U) tpl$(tpl_(_$T, divAsg), _$U)
#define op_remAsgWith$(_$T, _$U) tpl$(tpl_(_$T, remAsg), _$U)

#define op_neg$(_$T) tpl_(_$T, neg)
#define op_not$(_$T) tpl_(_$T, not)

#define op_shl$(_$T) tpl_(_$T, shl)
#define op_shr$(_$T) tpl_(_$T, shr)
#define op_and$(_$T) tpl_(_$T, and)
#define op_xor$(_$T) tpl_(_$T, xor)
#define op_or$(_$T) tpl_(_$T, or)

#define op_shlAsg$(_$T) tpl_(_$T, shlAsg)
#define op_shrAsg$(_$T) tpl_(_$T, shrAsg)
#define op_andAsg$(_$T) tpl_(_$T, andAsg)
#define op_xorAsg$(_$T) tpl_(_$T, xorAsg)
#define op_orAsg$(_$T) tpl_(_$T, orAsg)

#define op_shlWith$(_$T, _$U) tpl$(tpl_(_$T, shl), _$U)
#define op_shrWith$(_$T, _$U) tpl$(tpl_(_$T, shr), _$U)
#define op_andWith$(_$T, _$U) tpl$(tpl_(_$T, and), _$U)
#define op_xorWith$(_$T, _$U) tpl$(tpl_(_$T, xor), _$U)
#define op_orWith$(_$T, _$U) tpl$(tpl_(_$T, or), _$U)

#define op_shlAsgWith$(_$T, _$U) tpl$(tpl_(_$T, shlAsg), _$U)
#define op_shrAsgWith$(_$T, _$U) tpl$(tpl_(_$T, shrAsg), _$U)
#define op_andAsgWith$(_$T, _$U) tpl$(tpl_(_$T, andAsg), _$U)
#define op_xorAsgWith$(_$T, _$U) tpl$(tpl_(_$T, xorAsg), _$U)
#define op_orAsgWith$(_$T, _$U) tpl$(tpl_(_$T, orAsg), _$U)

/*---- Function-like Macros ----*/

#define op_fn_add$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_bin$(add, __VA_ARGS__)
#define op_fn_sub$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_bin$(sub, __VA_ARGS__)
#define op_fn_mul$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_bin$(mul, __VA_ARGS__)
#define op_fn_div$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_bin$(div, __VA_ARGS__)
#define op_fn_rem$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_bin$(rem, __VA_ARGS__)

#define op_fn_addAsg$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsg$(add, __VA_ARGS__)
#define op_fn_subAsg$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsg$(sub, __VA_ARGS__)
#define op_fn_mulAsg$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsg$(mul, __VA_ARGS__)
#define op_fn_divAsg$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsg$(div, __VA_ARGS__)
#define op_fn_remAsg$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsg$(rem, __VA_ARGS__)

#define op_fn_addWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binWith$(add, __VA_ARGS__)
#define op_fn_subWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binWith$(sub, __VA_ARGS__)
#define op_fn_mulWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binWith$(mul, __VA_ARGS__)
#define op_fn_divWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binWith$(div, __VA_ARGS__)
#define op_fn_remWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binWith$(rem, __VA_ARGS__)

#define op_fn_addAsgWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsgWith$(add, __VA_ARGS__)
#define op_fn_subAsgWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsgWith$(sub, __VA_ARGS__)
#define op_fn_mulAsgWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsgWith$(mul, __VA_ARGS__)
#define op_fn_divAsgWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsgWith$(div, __VA_ARGS__)
#define op_fn_remAsgWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsgWith$(rem, __VA_ARGS__)

#define op_fn_neg$(/*((_$T)(_$val))(_$TRet)*/...) op__fn_un$(neg, __VA_ARGS__)
#define op_fn_not$(/*((_$T)(_$val))(_$TRet)*/...) op__fn_un$(not, __VA_ARGS__)

#define op_fn_shl$(/*((_$T)(_$val))(_$TRet)*/...) op__fn_un$(shl, __VA_ARGS__)
#define op_fn_shr$(/*((_$T)(_$val))(_$TRet)*/...) op__fn_un$(shr, __VA_ARGS__)
#define op_fn_and$(/*((_$T)(_$val))(_$TRet)*/...) op__fn_un$(and, __VA_ARGS__)
#define op_fn_xor$(/*((_$T)(_$val))(_$TRet)*/...) op__fn_un$(xor, __VA_ARGS__)
#define op_fn_or$(/*((_$T)(_$val))(_$TRet)*/...) op__fn_un$(or, __VA_ARGS__)

#define op_fn_shlAsg$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsg$(shl, __VA_ARGS__)
#define op_fn_shrAsg$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsg$(shr, __VA_ARGS__)
#define op_fn_andAsg$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsg$(and, __VA_ARGS__)
#define op_fn_xorAsg$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsg$(xor, __VA_ARGS__)
#define op_fn_orAsg$(/*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsg$(or, __VA_ARGS__)

#define op_fn_shlWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binWith$(shl, __VA_ARGS__)
#define op_fn_shrWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binWith$(shr, __VA_ARGS__)
#define op_fn_andWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binWith$(and, __VA_ARGS__)
#define op_fn_xorWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binWith$(xor, __VA_ARGS__)
#define op_fn_orWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binWith$(or, __VA_ARGS__)

#define op_fn_shlAsgWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsgWith$(shl, __VA_ARGS__)
#define op_fn_shrAsgWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsgWith$(shr, __VA_ARGS__)
#define op_fn_andAsgWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsgWith$(and, __VA_ARGS__)
#define op_fn_xorAsgWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsgWith$(xor, __VA_ARGS__)
#define op_fn_orAsgWith$(/*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) op__fn_binAsgWith$(or, __VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define op__fn_un$(_$ident, /*((_$T)(_$val))(_$TRet)*/...) __step__op__fn_un$(_$ident, __step__op__fn_un$__parse __VA_ARGS__)
#define __step__op__fn_un$(_$ident, ...) __step__op__fn_un$__emit(_$ident, __VA_ARGS__)
#define __step__op__fn_un$__parse(...) __step__op__fn_un$__parseNested __VA_ARGS__,
#define __step__op__fn_un$__parseNested(_$T...) _$T, __step__op__fn_un$__parseNestedNext
#define __step__op__fn_un$__parseNestedNext(_$val...) _$val
#define __step__op__fn_un$__emit(_$ident, ...) __op__fn_un$(_$ident, __VA_ARGS__)
#define __op__fn_un$(_$ident, _$T, _$val, _$TRet...) \
    fn_((pp_cat3(op_, _$ident, $)(_$T)(_$T _$val))_$TRet)

#define op__fn_bin$(_$ident, /*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) __step__op__fn_bin$(_$ident, __step__op__fn_bin$__parse __VA_ARGS__)
#define __step__op__fn_bin$(_$ident, ...) __step__op__fn_bin$__emit(_$ident, __VA_ARGS__)
#define __step__op__fn_bin$__parse(...) __step__op__fn_bin$__parseNested __VA_ARGS__,
#define __step__op__fn_bin$__parseNested(_$T...) _$T, __step__op__fn_bin$__parseNestedNext
#define __step__op__fn_bin$__parseNestedNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __step__op__fn_bin$__emit(_$ident, ...) __op__fn_bin$(_$ident, __VA_ARGS__)
#define __op__fn_bin$(_$ident, _$T, _$lhs, _$rhs, _$TRet...) \
    fn_((pp_cat3(op_, _$ident, $)(_$T)(_$T _$lhs, _$T _$rhs))_$TRet)

#define op__fn_binAsg$(_$ident, /*((_$T)(_$id_lhs, _$id_rhs))(_$TRet)*/...) __step__op__fn_binAsg$(_$ident, __step__op__fn_binAsg$__parse __VA_ARGS__)
#define __step__op__fn_binAsg$(_$ident, ...) __step__op__fn_binAsg$__emit(_$ident, __VA_ARGS__)
#define __step__op__fn_binAsg$__parse(...) __step__op__fn_binAsg$__parseNested __VA_ARGS__,
#define __step__op__fn_binAsg$__parseNested(_$T...) _$T, __step__op__fn_binAsg$__parseNestedNext
#define __step__op__fn_binAsg$__parseNestedNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __step__op__fn_binAsg$__emit(_$ident, ...) __op__fn_binAsg$(_$ident, __VA_ARGS__)
#define __op__fn_binAsg$(_$ident, _$T, _$lhs, _$rhs, _$TRet...) \
    fn_((pp_cat3(op_, _$ident, Asg$)(_$T)($P$(_$T) _$lhs, _$T _$rhs))_$TRet)

#define op__fn_binWith$(_$ident, /*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) pp_overload(__op__fn_binWith$, __VA_ARGS__)(_$ident, __VA_ARGS__)
#define __op__fn_binWith$_1(_$ident, /*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) __step__op__fn_binWith$(_$ident, __step__op__fn_binWith$__parse __VA_ARGS__)
#define __step__op__fn_binWith$(_$ident, ...) __step__op__fn_binWith$__emit(_$ident, __VA_ARGS__)
#define __step__op__fn_binWith$__parse(...) __step__op__fn_binWith$__parseNested __VA_ARGS__,
#define __step__op__fn_binWith$__parseNested(_$T, _$U...) _$T, _$U, __step__op__fn_binWith$__parseNestedNext
#define __step__op__fn_binWith$__parseNestedNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __step__op__fn_binWith$__emit(_$ident, ...) __op__fn_binWith$(_$ident, __VA_ARGS__)
#define __op__fn_binWith$(_$ident, _$T, _$U, _$lhs, _$rhs, _$TRet...) \
    fn_((pp_cat3(op_, _$ident, With$)(_$T, _$U)(_$T _$lhs, _$U _$rhs))_$TRet)
#define __op__fn_binWith$_2(_$ident, _$rename, /*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) __step__op__fn_binWith$__rename(_$ident, _$rename, __step__op__fn_binWith$__parse __VA_ARGS__)
#define __step__op__fn_binWith$__rename(_$ident, _$rename, ...) __step__op__fn_binWith$__rename__emit(_$ident, _$rename, __VA_ARGS__)
#define __step__op__fn_binWith$__rename__emit(_$ident, _$rename, ...) __op__fn_binWith$__rename(_$ident, _$rename, __VA_ARGS__)
#if !in_comptime
#define __op__fn_binWith$__rename(_$ident, _$rename, _$T, _$U, _$lhs, _$rhs, _$TRet...) \
    $inline_always fn_((tpl_(_$T, _$rename)(_$T _$lhs, _$U _$rhs))_$TRet) { return pp_cat3(op_, _$ident, With$)(_$T, _$U)(_$lhs, _$rhs); }
#else /* in_comptime */
#define __op__fn_binWith$__rename(_$ident, _$rename, _$T, _$U, _$lhs, _$rhs, _$TRet...) \
    let tpl_(_$T, _$rename) = pp_cat3(op_, _$ident, With$)(_$T, _$U)
#endif

#define op__fn_binAsgWith$(_$ident, /*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) pp_overload(__op__fn_binAsgWith$, __VA_ARGS__)(_$ident, __VA_ARGS__)
#define __op__fn_binAsgWith$_1(_$ident, /*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) __step__op__fn_binAsgWith$(_$ident, __step__op__fn_binAsgWith$__parse __VA_ARGS__)
#define __step__op__fn_binAsgWith$(_$ident, ...) __step__op__fn_binAsgWith$__emit(_$ident, __VA_ARGS__)
#define __step__op__fn_binAsgWith$__parse(...) __step__op__fn_binAsgWith$__parseNested __VA_ARGS__,
#define __step__op__fn_binAsgWith$__parseNested(_$T, _$U...) _$T, _$U, __step__op__fn_binAsgWith$__parseNestedNext
#define __step__op__fn_binAsgWith$__parseNestedNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __step__op__fn_binAsgWith$__emit(_$ident, ...) __op__fn_binAsgWith$(_$ident, __VA_ARGS__)
#define __op__fn_binAsgWith$(_$ident, _$T, _$U, _$lhs, _$rhs, _$TRet...) \
    fn_((pp_cat3(op_, _$ident, AsgWith$)(_$T, _$U)($P$(_$T) _$lhs, _$U _$rhs))_$TRet)
#define __op__fn_binAsgWith$_2(_$ident, _$rename, /*((_$T, _$U)(_$id_lhs, _$id_rhs))(_$TRet)*/...) __step__op__fn_binAsgWith$__rename(_$ident, _$rename, __step__op__fn_binAsgWith$__parse __VA_ARGS__)
#define __step__op__fn_binAsgWith$__rename(_$ident, _$rename, ...) __step__op__fn_binAsgWith$__rename__emit(_$ident, _$rename, __VA_ARGS__)
#define __step__op__fn_binAsgWith$__rename__emit(_$ident, _$rename, ...) __op__fn_binAsgWith$__rename(_$ident, _$rename, __VA_ARGS__)
#if !in_comptime
#define __op__fn_binAsgWith$__rename(_$ident, _$rename, _$T, _$U, _$lhs, _$rhs, _$TRet...) \
    $inline_always fn_((tpl_(_$T, _$rename)($P$(_$T) _$lhs, _$U _$rhs))_$TRet) { return pp_cat3(op_, _$ident, AsgWith$)(_$T, _$U)(_$lhs, _$rhs); }
#else /* in_comptime */
#define __op__fn_binAsgWith$__rename(_$ident, _$rename, _$T, _$U, _$lhs, _$rhs, _$TRet...) \
    let tpl_(_$T, _$rename) = pp_cat3(op_, _$ident, AsgWith$)(_$T, _$U)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_op__included */

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
#ifndef core_op__included
#define core_op__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim.h"

/*========== Macros and Declarations ========================================*/

/* --- Operators --- */

#define op_add$(_T) pp_join(_, _T, add)
#define op_sub$(_T) pp_join(_, _T, sub)
#define op_mul$(_T) pp_join(_, _T, mul)
#define op_div$(_T) pp_join(_, _T, div)
#define op_rem$(_T) pp_join(_, _T, rem)

#define op_addAsg$(_T) pp_join(_, _T, addAsg)
#define op_subAsg$(_T) pp_join(_, _T, subAsg)
#define op_mulAsg$(_T) pp_join(_, _T, mulAsg)
#define op_divAsg$(_T) pp_join(_, _T, divAsg)
#define op_remAsg$(_T) pp_join(_, _T, remAsg)

#define op_addWith$(_T, _U) pp_join($, pp_join(_, _T, add), _U)
#define op_subWith$(_T, _U) pp_join($, pp_join(_, _T, sub), _U)
#define op_mulWith$(_T, _U) pp_join($, pp_join(_, _T, mul), _U)
#define op_divWith$(_T, _U) pp_join($, pp_join(_, _T, div), _U)
#define op_remWith$(_T, _U) pp_join($, pp_join(_, _T, rem), _U)

#define op_addAsgWith$(_T, _U) pp_join($, pp_join(_, _T, addAsg), _U)
#define op_subAsgWith$(_T, _U) pp_join($, pp_join(_, _T, subAsg), _U)
#define op_mulAsgWith$(_T, _U) pp_join($, pp_join(_, _T, mulAsg), _U)
#define op_divAsgWith$(_T, _U) pp_join($, pp_join(_, _T, divAsg), _U)
#define op_remAsgWith$(_T, _U) pp_join($, pp_join(_, _T, remAsg), _U)

#define op_neg$(_T) pp_join(_, _T, neg)
#define op_not$(_T) pp_join(_, _T, not)

#define op_shl$(_T) pp_join(_, _T, shl)
#define op_shr$(_T) pp_join(_, _T, shr)
#define op_and$(_T) pp_join(_, _T, and)
#define op_xor$(_T) pp_join(_, _T, xor)
#define op_or$(_T) pp_join(_, _T, or)

#define op_shlAsg$(_T) pp_join(_, _T, shlAsg)
#define op_shrAsg$(_T) pp_join(_, _T, shrAsg)
#define op_andAsg$(_T) pp_join(_, _T, andAsg)
#define op_xorAsg$(_T) pp_join(_, _T, xorAsg)
#define op_orAsg$(_T) pp_join(_, _T, orAsg)

#define op_shlWith$(_T, _U) pp_join($, pp_join(_, _T, shl), _U)
#define op_shrWith$(_T, _U) pp_join($, pp_join(_, _T, shr), _U)
#define op_andWith$(_T, _U) pp_join($, pp_join(_, _T, and), _U)
#define op_xorWith$(_T, _U) pp_join($, pp_join(_, _T, xor), _U)
#define op_orWith$(_T, _U) pp_join($, pp_join(_, _T, or), _U)

#define op_shlAsgWith$(_T, _U) pp_join($, pp_join(_, _T, shlAsg), _U)
#define op_shrAsgWith$(_T, _U) pp_join($, pp_join(_, _T, shrAsg), _U)
#define op_andAsgWith$(_T, _U) pp_join($, pp_join(_, _T, andAsg), _U)
#define op_xorAsgWith$(_T, _U) pp_join($, pp_join(_, _T, xorAsg), _U)
#define op_orAsgWith$(_T, _U) pp_join($, pp_join(_, _T, orAsg), _U)

/* ---- Function-like Macros ---- */

#define op_fn_add$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_bin$(add, __VA_ARGS__)
#define op_fn_sub$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_bin$(sub, __VA_ARGS__)
#define op_fn_mul$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_bin$(mul, __VA_ARGS__)
#define op_fn_div$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_bin$(div, __VA_ARGS__)
#define op_fn_rem$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_bin$(rem, __VA_ARGS__)

#define op_fn_addAsg$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsg$(add, __VA_ARGS__)
#define op_fn_subAsg$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsg$(sub, __VA_ARGS__)
#define op_fn_mulAsg$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsg$(mul, __VA_ARGS__)
#define op_fn_divAsg$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsg$(div, __VA_ARGS__)
#define op_fn_remAsg$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsg$(rem, __VA_ARGS__)

#define op_fn_addWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binWith$(add, __VA_ARGS__)
#define op_fn_subWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binWith$(sub, __VA_ARGS__)
#define op_fn_mulWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binWith$(mul, __VA_ARGS__)
#define op_fn_divWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binWith$(div, __VA_ARGS__)
#define op_fn_remWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binWith$(rem, __VA_ARGS__)

#define op_fn_addAsgWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsgWith$(add, __VA_ARGS__)
#define op_fn_subAsgWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsgWith$(sub, __VA_ARGS__)
#define op_fn_mulAsgWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsgWith$(mul, __VA_ARGS__)
#define op_fn_divAsgWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsgWith$(div, __VA_ARGS__)
#define op_fn_remAsgWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsgWith$(rem, __VA_ARGS__)

#define op_fn_neg$(/*((_T)(_val))(_TRet)*/...) op__fn_un$(neg, __VA_ARGS__)
#define op_fn_not$(/*((_T)(_val))(_TRet)*/...) op__fn_un$(not, __VA_ARGS__)

#define op_fn_shl$(/*((_T)(_val))(_TRet)*/...) op__fn_un$(shl, __VA_ARGS__)
#define op_fn_shr$(/*((_T)(_val))(_TRet)*/...) op__fn_un$(shr, __VA_ARGS__)
#define op_fn_and$(/*((_T)(_val))(_TRet)*/...) op__fn_un$(and, __VA_ARGS__)
#define op_fn_xor$(/*((_T)(_val))(_TRet)*/...) op__fn_un$(xor, __VA_ARGS__)
#define op_fn_or$(/*((_T)(_val))(_TRet)*/...) op__fn_un$(or, __VA_ARGS__)

#define op_fn_shlAsg$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsg$(shl, __VA_ARGS__)
#define op_fn_shrAsg$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsg$(shr, __VA_ARGS__)
#define op_fn_andAsg$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsg$(and, __VA_ARGS__)
#define op_fn_xorAsg$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsg$(xor, __VA_ARGS__)
#define op_fn_orAsg$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsg$(or, __VA_ARGS__)

#define op_fn_shlWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binWith$(shl, __VA_ARGS__)
#define op_fn_shrWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binWith$(shr, __VA_ARGS__)
#define op_fn_andWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binWith$(and, __VA_ARGS__)
#define op_fn_xorWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binWith$(xor, __VA_ARGS__)
#define op_fn_orWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binWith$(or, __VA_ARGS__)

#define op_fn_shlAsgWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsgWith$(shl, __VA_ARGS__)
#define op_fn_shrAsgWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsgWith$(shr, __VA_ARGS__)
#define op_fn_andAsgWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsgWith$(and, __VA_ARGS__)
#define op_fn_xorAsgWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsgWith$(xor, __VA_ARGS__)
#define op_fn_orAsgWith$(/*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) op__fn_binAsgWith$(or, __VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define op__fn_un$(_ident, /*((_T)(_val))(_TRet)*/...) __step__op__fn_un$(_ident, __step__op__fn_un$__parse __VA_ARGS__)
#define __step__op__fn_un$(_ident, ...) __step__op__fn_un$__emit(_ident, __VA_ARGS__)
#define __step__op__fn_un$__parse(...) __step__op__fn_un$__parseNested __VA_ARGS__,
#define __step__op__fn_un$__parseNested(_T...) _T, __step__op__fn_un$__parseNestedNext
#define __step__op__fn_un$__parseNestedNext(_val...) _val
#define __step__op__fn_un$__emit(_ident, ...) __op__fn_un$(_ident, __VA_ARGS__)
#define __op__fn_un$(_ident, _T, _val, _TRet...) \
    fn_((pp_cat3(op_, _ident, $)(_T)(_T _val))_TRet)

#define op__fn_bin$(_ident, /*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) __step__op__fn_bin$(_ident, __step__op__fn_bin$__parse __VA_ARGS__)
#define __step__op__fn_bin$(_ident, ...) __step__op__fn_bin$__emit(_ident, __VA_ARGS__)
#define __step__op__fn_bin$__parse(...) __step__op__fn_bin$__parseNested __VA_ARGS__,
#define __step__op__fn_bin$__parseNested(_T...) _T, __step__op__fn_bin$__parseNestedNext
#define __step__op__fn_bin$__parseNestedNext(_lhs, _rhs...) _lhs, _rhs
#define __step__op__fn_bin$__emit(_ident, ...) __op__fn_bin$(_ident, __VA_ARGS__)
#define __op__fn_bin$(_ident, _T, _lhs, _rhs, _TRet...) \
    fn_((pp_cat3(op_, _ident, $)(_T)(_T _lhs, _T _rhs))_TRet)

#define op__fn_binAsg$(_ident, /*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) __step__op__fn_binAsg$(_ident, __step__op__fn_binAsg$__parse __VA_ARGS__)
#define __step__op__fn_binAsg$(_ident, ...) __step__op__fn_binAsg$__emit(_ident, __VA_ARGS__)
#define __step__op__fn_binAsg$__parse(...) __step__op__fn_binAsg$__parseNested __VA_ARGS__,
#define __step__op__fn_binAsg$__parseNested(_T...) _T, __step__op__fn_binAsg$__parseNestedNext
#define __step__op__fn_binAsg$__parseNestedNext(_lhs, _rhs...) _lhs, _rhs
#define __step__op__fn_binAsg$__emit(_ident, ...) __op__fn_binAsg$(_ident, __VA_ARGS__)
#define __op__fn_binAsg$(_ident, _T, _lhs, _rhs, _TRet...) \
    fn_((pp_cat3(op_, _ident, Asg$)(_T)($P$(_T) _lhs, _T _rhs))_TRet)

#define op__fn_binWith$(_ident, /*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) pp_overload(__op__fn_binWith$, __VA_ARGS__)(_ident, __VA_ARGS__)
#define __op__fn_binWith$_1(_ident, /*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) __step__op__fn_binWith$(_ident, __step__op__fn_binWith$__parse __VA_ARGS__)
#define __step__op__fn_binWith$(_ident, ...) __step__op__fn_binWith$__emit(_ident, __VA_ARGS__)
#define __step__op__fn_binWith$__parse(...) __step__op__fn_binWith$__parseNested __VA_ARGS__,
#define __step__op__fn_binWith$__parseNested(_T, _U...) _T, _U, __step__op__fn_binWith$__parseNestedNext
#define __step__op__fn_binWith$__parseNestedNext(_lhs, _rhs...) _lhs, _rhs
#define __step__op__fn_binWith$__emit(_ident, ...) __op__fn_binWith$(_ident, __VA_ARGS__)
#define __op__fn_binWith$(_ident, _T, _U, _lhs, _rhs, _TRet...) \
    fn_((pp_cat3(op_, _ident, With$)(_T, _U)(_T _lhs, _U _rhs))_TRet)
#define __op__fn_binWith$_2(_ident, _rename, /*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) __step__op__fn_binWith$__rename(_ident, _rename, __step__op__fn_binWith$__parse __VA_ARGS__)
#define __step__op__fn_binWith$__rename(_ident, _rename, ...) __step__op__fn_binWith$__rename__emit(_ident, _rename, __VA_ARGS__)
#define __step__op__fn_binWith$__rename__emit(_ident, _rename, ...) __op__fn_binWith$__rename(_ident, _rename, __VA_ARGS__)
#if !on_comptime
#define __op__fn_binWith$__rename(_ident, _rename, _T, _U, _lhs, _rhs, _TRet...) \
    $inline_always fn_((pp_join(_, _T, _rename)(_T _lhs, _U _rhs))_TRet) { return pp_cat3(op_, _ident, With$)(_T, _U)(_lhs, _rhs); }
#else /* on_comptime */
#define __op__fn_binWith$__rename(_ident, _rename, _T, _U, _lhs, _rhs, _TRet...) \
    let pp_join(_, _T, _rename) = pp_cat3(op_, _ident, With$)(_T, _U)
#endif

#define op__fn_binAsgWith$(_ident, /*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) pp_overload(__op__fn_binAsgWith$, __VA_ARGS__)(_ident, __VA_ARGS__)
#define __op__fn_binAsgWith$_1(_ident, /*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) __step__op__fn_binAsgWith$(_ident, __step__op__fn_binAsgWith$__parse __VA_ARGS__)
#define __step__op__fn_binAsgWith$(_ident, ...) __step__op__fn_binAsgWith$__emit(_ident, __VA_ARGS__)
#define __step__op__fn_binAsgWith$__parse(...) __step__op__fn_binAsgWith$__parseNested __VA_ARGS__,
#define __step__op__fn_binAsgWith$__parseNested(_T, _U...) _T, _U, __step__op__fn_binAsgWith$__parseNestedNext
#define __step__op__fn_binAsgWith$__parseNestedNext(_lhs, _rhs...) _lhs, _rhs
#define __step__op__fn_binAsgWith$__emit(_ident, ...) __op__fn_binAsgWith$(_ident, __VA_ARGS__)
#define __op__fn_binAsgWith$(_ident, _T, _U, _lhs, _rhs, _TRet...) \
    fn_((pp_cat3(op_, _ident, AsgWith$)(_T, _U)($P$(_T) _lhs, _U _rhs))_TRet)
#define __op__fn_binAsgWith$_2(_ident, _rename, /*((_T, _U)(_id_lhs, _id_rhs))(_TRet)*/...) __step__op__fn_binAsgWith$__rename(_ident, _rename, __step__op__fn_binAsgWith$__parse __VA_ARGS__)
#define __step__op__fn_binAsgWith$__rename(_ident, _rename, ...) __step__op__fn_binAsgWith$__rename__emit(_ident, _rename, __VA_ARGS__)
#define __step__op__fn_binAsgWith$__rename__emit(_ident, _rename, ...) __op__fn_binAsgWith$__rename(_ident, _rename, __VA_ARGS__)
#if !on_comptime
#define __op__fn_binAsgWith$__rename(_ident, _rename, _T, _U, _lhs, _rhs, _TRet...) \
    $inline_always fn_((pp_join(_, _T, _rename)($P$(_T) _lhs, _U _rhs))_TRet) { return pp_cat3(op_, _ident, AsgWith$)(_T, _U)(_lhs, _rhs); }
#else /* on_comptime */
#define __op__fn_binAsgWith$__rename(_ident, _rename, _T, _U, _lhs, _rhs, _TRet...) \
    let pp_join(_, _T, _rename) = pp_cat3(op_, _ident, AsgWith$)(_T, _U)
#endif

#if UNUSED_CODE
#define op_fn_add$(/*((_T)(_id_lhs, _id_rhs))(_TRet)*/...) __step__op_fn_add$(__step__op_fn_add$__parse __VA_ARGS__)
#define __step__op_fn_add$(...) __step__op_fn_add$__emit(__VA_ARGS__)
#define __step__op_fn_add$__parse(...) __step__op_fn_add$__parseNested __VA_ARGS__,
#define __step__op_fn_add$__parseNested(_T...) _T, __step__op_fn_add$__parseNestedNext
#define __step__op_fn_add$__parseNestedNext(_lhs, _rhs...) _lhs, _rhs
#define __step__op_fn_add$__emit(...) __op_fn_add$(__VA_ARGS__)
#define __op_fn_add$(_T, _lhs, _rhs, _TRet...) \
    fn_((op_add$(_T)(_T _lhs, _T _rhs))_TRet)
op_fn_addWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant)) {
}
#endif /* UNUSED_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_op__included */

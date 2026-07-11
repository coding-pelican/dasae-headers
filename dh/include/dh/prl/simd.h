#pragma once
#ifndef simd_V__included
#define simd_V__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/simd.h"
#include "common.h"

/*========== Type and Shape Utilities =======================================*/

#define $simd_V$(_$n /*: usize*/, _$T...) __type__$simd_V$(_$n, _$T)
#define simd_V$$(_$n /*: usize*/, _$T...) __type__simd_V$$(_$n, _$T)
#define simd_V$(_$n /*: usize*/, _$T...) __alias__simd_V$(_$n, _$T)
#define T_decl_simd_V$(_$n /*: usize*/, _$T...) __stmt__T_decl_simd_V$(_$n, _$T)
#define T_impl_simd_V$(_$n /*: usize*/, _$T...) __stmt__T_impl_simd_V$(_$n, _$T)
#define T_use_simd_V$(_$n /*: usize*/, _$T...) __stmt__T_use_simd_V$(_$n, _$T)

#define simd_V_n$(_$T /*: simd_V(n: usize, T)*/... /*(usize)*/) __val__simd_V_n$(_$T)
#define simd_V_T$(_$T /*: simd_V(n: usize, T)*/... /*(T)*/) __type__simd_V_T$(_$T)
#define simd_V_TUnqual$(_$T /*: simd_V(n: usize, T)*/... /*(Unqual(T))*/) __type__simd_V_TUnqual$(_$T)
#define simd_V_isBool$(_$T /*: simd_V(n: usize, T)*/... /*(bool)*/) isBool$(simd_V_TUnqual$(_$T))
#define simd_V_isUInt$(_$T /*: simd_V(n: usize, T)*/... /*(bool)*/) isUInt$(simd_V_TUnqual$(_$T))
#define simd_V_isIInt$(_$T /*: simd_V(n: usize, T)*/... /*(bool)*/) isIInt$(simd_V_TUnqual$(_$T))
#define simd_V_isInt$(_$T /*: simd_V(n: usize, T)*/... /*(bool)*/) isInt$(simd_V_TUnqual$(_$T))
#define simd_V_isFlt$(_$T /*: simd_V(n: usize, T)*/... /*(bool)*/) isFlt$(simd_V_TUnqual$(_$T))

/*========== Vector Construction ============================================*/

#define simd_V_zero$(/*(_$T: simd_V(n: usize, T))*/... /*(_$T)*/) __op__simd_V_zero$(__VA_ARGS__)
#define simd_V_zero() __op__simd_V_zero()
#define simd_V_init$(/*(_$T: simd_V(n: usize, T)){ _$initial... }*/... /*(_$T)*/) __op__simd_V_init$(__VA_ARGS__)
#define simd_V_init(/*{ _$initial... }*/...) __op__simd_V_init(__VA_ARGS__)
#define simd_V_splat$(/*(_$T: simd_V(n: usize, T))(_$x: T)*/... /*(_$T)*/) __op__simd_V_splat$(__VA_ARGS__)
#define simd_V_splat(_$v /*: simd_V(n: usize, T)*/, _$x /*: T*/... /*(simd_V(n, T))*/) __op__simd_V_splat(_$v, _$x)
#define simd_V_from$(/*(_$T){ _$initial... }*/... /*(simd_V(n: usize, _$T))*/) __op__simd_V_from$(__VA_ARGS__)
#define simd_V_fromA$(/*(_$T: simd_V(n: usize, T))(_$a: A(n: usize, T))*/... /*(_$T)*/) __op__simd_V_fromA$(__VA_ARGS__)
#define simd_V_fromA(_$a /*: A(n: usize, T)*/... /*(simd_V(n: usize, T))*/) __op__simd_V_fromA(_$a)
#define simd_V_toA$(/*(_$T: A(n: usize, T))(_$v: simd_V(n: usize, T))*/... /*(_$T)*/) __op__simd_V_toA$(__VA_ARGS__)
#define simd_V_toA(_$v /*: simd_V(n: usize, T)*/... /*(A(n: usize, T))*/) __op__simd_V_toA(_$v)

#define simd_V_cat(_$lhs, _$rhs...) __op__simd_V_cat(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), pp_uniqTok(i), _$lhs, _$rhs)
#define simd_V_repeat$(/*(_$T: simd_V(n: usize, T))(_$v: simd_V(m: usize, T))*/... /*(_$T)*/) __op__simd_V_repeat$(__VA_ARGS__)
#define simd_V_extract$(/*(_$T: simd_V(n: usize, T))(_$v: simd_V(m: usize, T), _$first: usize)*/... /*(_$T)*/) __op__simd_V_extract$(__VA_ARGS__)

#define simd_V_val(_$v /*: simd_V(n: usize, T)*/... /*($simd_V$(n: usize, T))*/) __op__simd_V_val(_$v)
#define simd_V_len$(_$T /*: simd_V(n: usize, T)*/... /*(usize)*/) __op__simd_V_len$(_$T)
#define simd_V_len(_$v /*: simd_V(n: usize, T)*/... /*(usize)*/) __op__simd_V_len(_$v)
#define simd_V_at(/*(_$v: simd_V(n: usize, T))[_$idx: usize]*/... /*(P(T))*/) __op__simd_V_at(__VA_ARGS__)

/*========== Memory Operations ==============================================*/

#define simd_V_load$(/*(_$T: simd_V(n: usize, T))(_$ptr: P(T))*/... /*(_$T)*/) __op__simd_V_load$(__VA_ARGS__)
#define simd_V_load(_$v /*: simd_V(n: usize, T)*/, _$ptr /*: P(T)*/... /*(simd_V(n, T))*/) __op__simd_V_load(_$v, _$ptr)
#define simd_V_store(_$ptr /*: P(T)*/, _$v /*: simd_V(n: usize, T)*/... /*(void)*/) __op__simd_V_store(_$ptr, _$v)

/*========== Type Conversion ================================================*/

#define simd_V_as$(/*(_$T: simd_V(n: usize, T))(_$v: simd_V(n: usize, U))*/... /*(_$T)*/) __op__simd_V_as$(__VA_ARGS__)
#define simd_V_boolToInt$(/*(_$T: simd_V(n: usize, IntType))(_$v: simd_V(n: usize, MaskType))*/... /*(_$T)*/) __op__simd_V_boolToInt$(__VA_ARGS__)
#define simd_V_intFromBool$ simd_V_boolToInt$
#define simd_V_intCast$(/*(_$T: simd_V(n: usize, IntType))(_$v: simd_V(n: usize, IntType))*/... /*(_$T)*/) __op__simd_V_intCast$(__VA_ARGS__)
#define simd_V_intToFlt$(/*(_$T: simd_V(n: usize, FltType))(_$v: simd_V(n: usize, IntType))*/... /*(_$T)*/) __op__simd_V_intToFlt$(__VA_ARGS__)
#define simd_V_fltFromInt$ simd_V_intToFlt$
#define simd_V_fltToInt$(/*(_$T: simd_V(n: usize, IntType))(_$v: simd_V(n: usize, FltType))*/... /*(_$T)*/) __op__simd_V_fltToInt$(__VA_ARGS__)
#define simd_V_intFromFlt$ simd_V_fltToInt$
#define simd_V_fltCast$(/*(_$T: simd_V(n: usize, FltType))(_$v: simd_V(n: usize, FltType))*/... /*(_$T)*/) __op__simd_V_fltCast$(__VA_ARGS__)

/*========== Functional and Lane Operations ================================*/

#define simd_V_map1(_$x, _$op...) __op__simd_V_map1(pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane), _$x, _$op)
#define simd_V_map1$(/*(_$T: simd_V(n: usize, T))(_$x: simd_V(n: usize, U), _$op)*/... /*(_$T)*/) __op__simd_V_map1$(__VA_ARGS__)
#define simd_V_map2(_$lhs, _$rhs, _$op...) __op__simd_V_map2(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lhs_lane), pp_uniqTok(rhs_lane), _$lhs, _$rhs, _$op)
#define simd_V_map2$(/*(_$T: simd_V(n: usize, T))(_$lhs: simd_V(n, U), _$rhs: simd_V(n, V), _$op)*/... /*(_$T)*/) __op__simd_V_map2$(__VA_ARGS__)
#define simd_V_map3(_$x, _$y, _$z, _$op...) __op__simd_V_map3(pp_uniqTok(x), pp_uniqTok(y), pp_uniqTok(z), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(x_lane), pp_uniqTok(y_lane), pp_uniqTok(z_lane), _$x, _$y, _$z, _$op)
#define simd_V_reduce(_$x, _$initial, _$op...) __op__simd_V_reduce(pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane), _$x, _$initial, _$op)

#define simd_V_select(_$mask, _$if_true, _$if_false...) __op__simd_V_select(pp_uniqTok(mask), pp_uniqTok(if_true), pp_uniqTok(if_false), pp_uniqTok(ret), pp_uniqTok(i), _$mask, _$if_true, _$if_false)
#define simd_V_shuffle(_$lhs, _$rhs, _$idxs...) __op__simd_V_shuffle(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(idxs), pp_uniqTok(ret), pp_uniqTok(lhs_len), pp_uniqTok(rhs_len), pp_uniqTok(i), pp_uniqTok(idx), _$lhs, _$rhs, _$idxs)

/*========== Arithmetic Operations ==========================================*/

#define simd_V_isZero(_$x...) __op__simd_V_isZero(_$x)
#define simd_V_isNonzero(_$x...) __op__simd_V_isNonzero(_$x)

#define simd_V_add(_$lhs, _$rhs...) __op__simd_V_add(_$lhs, _$rhs)
#define simd_V_sub(_$lhs, _$rhs...) __op__simd_V_sub(_$lhs, _$rhs)
#define simd_V_mul(_$lhs, _$rhs...) __op__simd_V_mul(_$lhs, _$rhs)
#define simd_V_mulAdd(_$x, _$y, _$z...) __op__simd_V_mulAdd(_$x, _$y, _$z)
#define simd_V_div(_$lhs, _$rhs...) __op__simd_V_div(_$lhs, _$rhs)
#define simd_V_rem(_$x, _$n...) __op__simd_V_rem(_$x, _$n)

#define simd_V_neg(_$x...) __op__simd_V_neg(_$x)
#define simd_V_abs(_$x...) __op__simd_V_abs__step(_$x)
#define simd_V_sgn(_$x...) __op__simd_V_sgn__step(_$x)

#define simd_V_not(_$x...) __op__simd_V_not(_$x)
#define simd_V_and(_$x, _$y...) __op__simd_V_and(_$x, _$y)
#define simd_V_or(_$x, _$y...) __op__simd_V_or(_$x, _$y)

#define simd_V_eql(_$lhs, _$rhs...) __op__simd_V_eql(_$lhs, _$rhs)
#define simd_V_neq(_$lhs, _$rhs...) __op__simd_V_neq(_$lhs, _$rhs)

#define simd_V_ord(_$lhs, _$rhs...) __op__simd_V_ord__step(_$lhs, _$rhs)
#define simd_V_eq(_$lhs, _$rhs...) __op__simd_V_eq(_$lhs, _$rhs)
#define simd_V_ne(_$lhs, _$rhs...) __op__simd_V_ne(_$lhs, _$rhs)
#define simd_V_lt(_$lhs, _$rhs...) __op__simd_V_lt(_$lhs, _$rhs)
#define simd_V_gt(_$lhs, _$rhs...) __op__simd_V_gt(_$lhs, _$rhs)
#define simd_V_le(_$lhs, _$rhs...) __op__simd_V_le(_$lhs, _$rhs)
#define simd_V_ge(_$lhs, _$rhs...) __op__simd_V_ge(_$lhs, _$rhs)

#define simd_V_min(_$lhs, _$rhs...) __op__simd_V_min2__step(_$lhs, _$rhs)
#define simd_V_min2(_$lhs, _$rhs...) __op__simd_V_min2__step(_$lhs, _$rhs)
#define simd_V_min3(_$1st, _$2nd, _$3rd...) __op__simd_V_min3__step(_$1st, _$2nd, _$3rd)
#define simd_V_min4(_$1st, _$2nd, _$3rd, _$4th...) __op__simd_V_min4__step(_$1st, _$2nd, _$3rd, _$4th)
#define simd_V_findMin(_$vals...) __op__simd_V_findMin__step(_$vals)

#define simd_V_max(_$lhs, _$rhs...) __op__simd_V_max2__step(_$lhs, _$rhs)
#define simd_V_max2(_$lhs, _$rhs...) __op__simd_V_max2__step(_$lhs, _$rhs)
#define simd_V_max3(_$1st, _$2nd, _$3rd...) __op__simd_V_max3__step(_$1st, _$2nd, _$3rd)
#define simd_V_max4(_$1st, _$2nd, _$3rd, _$4th...) __op__simd_V_max3__step(_$1st, _$2nd, _$3rd, _$4th)
#define simd_V_findMax(_$vals...) __op__simd_V_findMax__step(_$vals)

#define simd_V_clamp(_$x, _$lo /*$incl*/, _$hi /*$incl*/...) __op__simd_V_clamp__step(_$x, _$lo, _$hi)

/*========== Vector Arithmetic Operations ===================================*/

#define simd_V_dot(_$lhs, _$rhs...) __op__simd_V_dot(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define simd_V_cross3(_$lhs, _$rhs...) __op__simd_V_cross3(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), _$lhs, _$rhs)

/*========== Boolean Operations =============================================*/

#define simd_V_bool_any(_$x...) __op__simd_V_bool_any(_$x)
#define simd_V_bool_all(_$x...) __op__simd_V_bool_all(_$x)
#define simd_V_bool_none(_$x...) __op__simd_V_bool_none(_$x)
#define simd_V_bool_bitMask(_$x...) __op__simd_V_bool_bitMask(pp_uniqTok(x), pp_uniqTok(normalized), pp_uniqTok(raw), pp_uniqTok(bit_mask), pp_uniqTok(masked), pp_uniqTok(paired), pp_uniqTok(ret), pp_uniqTok(i), _$x)

/*========== Integer Classification Operations =============================*/

#define simd_V_int_isZero(_$x...) simd_V_map1(_$x, int_isZero)
#define simd_V_int_isNonzero(_$x...) simd_V_map1(_$x, int_isNonzero)
#define simd_V_int_isOdd(_$x...) simd_V_map1(_$x, int_isOdd)
#define simd_V_int_isEven(_$x...) simd_V_map1(_$x, int_isEven)
#define simd_V_int_isPow2(_$x...) simd_V_map1(_$x, int_isPow2)
#define simd_V_int_hasSglBit(_$x...) simd_V_map1(_$x, int_hasSglBit)

/*========== Integer Arithmetic Operations ==================================*/

#define simd_V_int_add(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_add)
#define simd_V_int_addOverflow(_$lhs, _$rhs, _p_out...) __op__simd_V_int_addOverflow(_$lhs, _$rhs, _p_out)
#define simd_V_int_addWrap(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_addWrap)
#define simd_V_int_addSat(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_addSat)
#define simd_V_uint_addCarry(_$lhs, _$rhs, _carry_in, _p_carry_out...) __op__simd_V_uint_addCarry(_$lhs, _$rhs, _carry_in, _p_carry_out)
#define simd_V_int_sub(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_sub)
#define simd_V_int_subOverflow(_$lhs, _$rhs, _p_out...) __op__simd_V_int_subOverflow(_$lhs, _$rhs, _p_out)
#define simd_V_int_subWrap(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_subWrap)
#define simd_V_int_subSat(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_subSat)
#define simd_V_uint_subBorrow(_$lhs, _$rhs, _borrow_in, _p_borrow_out...) __op__simd_V_uint_subBorrow(_$lhs, _$rhs, _borrow_in, _p_borrow_out)
#define simd_V_int_mul(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_mul)
#define simd_V_int_mulOverflow(_$lhs, _$rhs, _p_out...) __op__simd_V_int_mulOverflow(_$lhs, _$rhs, _p_out)
#define simd_V_int_mulWrap(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_mulWrap)
#define simd_V_int_mulSat(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_mulSat)
#define simd_V_int_mulAdd(_$x, _$y, _$z...) simd_V_int_add(simd_V_int_mul(_$x, _$y), _$z)
#define simd_V_int_div(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_div)
#define simd_V_int_divTrunc(_$lhs, _$rhs...) simd_V_int_div(_$lhs, _$rhs)
#define simd_V_int_divRound(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_divRound)
#define simd_V_iint_divFloor(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, iint_divFloor)
#define simd_V_iint_divEuclid(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, iint_divEuclid)
#define simd_V_int_divCeil(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_divCeil)
#define simd_V_int_rem(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_rem)
#define simd_V_int_remRound(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_remRound)
#define simd_V_iint_mod(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, iint_mod)
#define simd_V_iint_modEuclid(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, iint_modEuclid)
#define simd_V_int_modCeil(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, int_modCeil)

#define simd_V_iint_neg(_$x...) simd_V_map1(_$x, iint_neg)
#define simd_V_iint_negOverflow(_$x, _p_out...) __op__simd_V_iint_negOverflow(_$x, _p_out)
#define simd_V_iint_negWrap(_$x...) simd_V_map1(_$x, iint_negWrap)
#define simd_V_iint_negSat(_$x...) simd_V_map1(_$x, iint_negSat)
#define simd_V_iint_abs(_$x...) simd_V_map1(_$x, iint_abs)
#define simd_V_iint_absOverflow(_$x, _p_out...) __op__simd_V_iint_absOverflow(_$x, _p_out)
#define simd_V_iint_absSat(_$x...) simd_V_map1(_$x, iint_absSat)
#define simd_V_iint_sgn(_$x...) simd_V_map1(_$x, iint_sgn)
#define simd_V_iint_sgnBit(_$x...) simd_V_map1(_$x, iint_sgnBit)

/*========== Integer Comparison Operations ==================================*/

#define simd_V_int_eql(_$lhs, _$rhs...) simd_V_eql(_$lhs, _$rhs)
#define simd_V_int_neq(_$lhs, _$rhs...) simd_V_neq(_$lhs, _$rhs)
#define simd_V_int_ord(_$lhs, _$rhs...) simd_V_ord(_$lhs, _$rhs)
#define simd_V_int_eq(_$lhs, _$rhs...) simd_V_eq(_$lhs, _$rhs)
#define simd_V_int_ne(_$lhs, _$rhs...) simd_V_ne(_$lhs, _$rhs)
#define simd_V_int_lt(_$lhs, _$rhs...) simd_V_lt(_$lhs, _$rhs)
#define simd_V_int_gt(_$lhs, _$rhs...) simd_V_gt(_$lhs, _$rhs)
#define simd_V_int_le(_$lhs, _$rhs...) simd_V_le(_$lhs, _$rhs)
#define simd_V_int_ge(_$lhs, _$rhs...) simd_V_ge(_$lhs, _$rhs)

/*========== Integer Min/Max/Clamp Operations ===============================*/

#define simd_V_int_min(_$lhs, _$rhs...) simd_V_min(_$lhs, _$rhs)
#define simd_V_int_max(_$lhs, _$rhs...) simd_V_max(_$lhs, _$rhs)
#define simd_V_int_clamp(_$x, _$lo, _$hi...) simd_V_clamp(_$x, _$lo, _$hi)
#define simd_V_int_wrap(_$x, _$lo, _$hi...) simd_V_map3(_$x, _$lo, _$hi, int_wrap)

/*========== Bitwise Operations =============================================*/

#define simd_V_int_not(_$x...) __op__simd_V_int_not(_$x)
#define simd_V_int_shl(_$x, _$y...) __op__simd_V_int_shl(_$x, _$y)
#define simd_V_int_shr(_$x, _$y...) __op__simd_V_int_shr(_$x, _$y)
#define simd_V_int_and(_$x, _$y...) __op__simd_V_int_and(_$x, _$y)
#define simd_V_int_xor(_$x, _$y...) __op__simd_V_int_xor(_$x, _$y)
#define simd_V_int_or(_$x, _$y...) __op__simd_V_int_or(_$x, _$y)
#define simd_V_int_setBit(_$x, _$bit...) __op__simd_V_int_setBit(_$x, _$bit)
#define simd_V_int_resetBit(_$x, _$bit...) __op__simd_V_int_resetBit(_$x, _$bit)
#define simd_V_int_toggleBit(_$x, _$bit...) __op__simd_V_int_toggleBit(_$x, _$bit)
#define simd_V_int_maskLo$(...) __op__simd_V_uintTypedMap$(int_maskLo, __VA_ARGS__)
#define simd_V_int_maskHi$(...) __op__simd_V_uintTypedMap$(int_maskHi, __VA_ARGS__)
#define simd_V_int_mask$(...) __op__simd_V_intTypedMap2$(int_mask, __VA_ARGS__)

/*========== Bit Manipulation Operations ====================================*/

#define simd_V_int_countOnes(_$x...) __op__simd_V_map1U32(_$x, int_countOnes)
#define simd_V_int_countZeros(_$x...) __op__simd_V_map1U32(_$x, int_countZeros)
#define simd_V_iint_leadingRedundantSgnBits(_$x...) __op__simd_V_map1U32(_$x, iint_leadingRedundantSgnBits)
#define simd_V_int_leadingOnes(_$x...) __op__simd_V_map1U32(_$x, int_leadingOnes)
#define simd_V_int_leadingZeros(_$x...) __op__simd_V_map1U32(_$x, int_leadingZeros)
#define simd_V_int_trailingOnes(_$x...) __op__simd_V_map1U32(_$x, int_trailingOnes)
#define simd_V_int_trailingZeros(_$x...) __op__simd_V_map1U32(_$x, int_trailingZeros)
#define simd_V_int_firstSetBit(_$x...) __op__simd_V_map1U32(_$x, int_firstSetBit)
#define simd_V_int_lastSetBit(_$x...) __op__simd_V_map1U32(_$x, int_lastSetBit)
#define simd_V_int_parity(_$x...) simd_V_map1(_$x, int_parity)
#define simd_V_int_bitWidth(_$x...) __op__simd_V_map1U32(_$x, int_bitWidth)
#define simd_V_int_bitFloor(_$x...) simd_V_map1(_$x, int_bitFloor)
#define simd_V_int_bitCeil(_$x...) simd_V_map1(_$x, int_bitCeil)
#define simd_V_int_rotateLeft(_$x, _$n...) __op__simd_V_int_rotateLeft(_$x, _$n)
#define simd_V_int_rotateRight(_$x, _$n...) __op__simd_V_int_rotateRight(_$x, _$n)
#define simd_V_int_swapBytes(_$x...) simd_V_map1(_$x, int_swapBytes)
#define simd_V_int_reverseBits(_$x...) simd_V_map1(_$x, int_reverseBits)
#define simd_V_int_hasBit(_$x, _$bit...) __op__simd_V_int_hasBit(_$x, _$bit)
#define simd_V_int_extractBits(_$x, _$off, _$len...) __op__simd_V_int_extractBits(_$x, _$off, _$len)
#define simd_V_int_replaceBits(_$x, _$off, _$len, _$bits...) __op__simd_V_int_replaceBits(_$x, _$off, _$len, _$bits)

/*========== Integer Exponential and Logarithm Operations ===================*/

#define simd_V_uint_exp10$(...) __op__simd_V_uintTypedMap$(uint_exp10, __VA_ARGS__)
#define simd_V_uint_exp2$(...) __op__simd_V_uintTypedMap$(uint_exp2, __VA_ARGS__)
#define simd_V_uint_pow2$(...) __op__simd_V_uintTypedMap$(uint_pow2, __VA_ARGS__)
#define simd_V_uint_pow2Round$(...) __op__simd_V_uintTypedMap$(uint_pow2Round, __VA_ARGS__)
#define simd_V_uint_pow2Floor$(...) __op__simd_V_uintTypedMap$(uint_pow2Floor, __VA_ARGS__)
#define simd_V_uint_pow2Ceil$(...) __op__simd_V_uintTypedMap$(uint_pow2Ceil, __VA_ARGS__)
#define simd_V_uint_log2(_$x...) __op__simd_V_map1U32(_$x, uint_log2)
#define simd_V_uint_log10Round(_$x...) __op__simd_V_map1U32(_$x, uint_log10Round)
#define simd_V_uint_log10Floor(_$x...) __op__simd_V_map1U32(_$x, uint_log10Floor)
#define simd_V_uint_log10Ceil(_$x...) __op__simd_V_map1U32(_$x, uint_log10Ceil)

/*========== Floating-Point Classification ==================================*/

#define simd_V_flt_isNaN(_$x...) simd_V_map1(_$x, flt_isNaN)
#define simd_V_flt_isInf(_$x...) simd_V_map1(_$x, flt_isInf)
#define simd_V_flt_isFinite(_$x...) simd_V_map1(_$x, flt_isFinite)
#define simd_V_flt_isNormal(_$x...) simd_V_map1(_$x, flt_isNormal)
#define simd_V_flt_isSubnormal(_$x...) simd_V_map1(_$x, flt_isSubnormal)
#define simd_V_flt_isZero(_$x...) simd_V_map1(_$x, flt_isZero)
#define simd_V_flt_isNonzero(_$x...) simd_V_map1(_$x, flt_isNonzero)
#define simd_V_flt_isIntegral(_$x...) simd_V_map1(_$x, flt_isIntegral)

/*========== Floating-Point Arithmetic Operations ===========================*/

#define simd_V_flt_add(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_add)
#define simd_V_flt_sub(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_sub)
#define simd_V_flt_mul(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_mul)
#define simd_V_flt_mulAdd(_$x, _$y, _$z...) simd_V_map3(_$x, _$y, _$z, flt_mulAdd)
#define simd_V_flt_div(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_div)
#define simd_V_flt_divTrunc(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_divTrunc)
#define simd_V_flt_divRound(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_divRound)
#define simd_V_flt_divFloor(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_divFloor)
#define simd_V_flt_divEuclid(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_divEuclid)
#define simd_V_flt_divCeil(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_divCeil)
#define simd_V_flt_rem(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_rem)
#define simd_V_flt_remRound(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_remRound)
#define simd_V_flt_mod(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_mod)
#define simd_V_flt_modEuclid(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_modEuclid)
#define simd_V_flt_modCeil(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_modCeil)
#define simd_V_flt_neg(_$x...) simd_V_map1(_$x, flt_neg)
#define simd_V_flt_abs(_$x...) simd_V_map1(_$x, flt_abs)
#define simd_V_flt_sgn(_$x...) simd_V_map1(_$x, flt_sgn)
#define simd_V_flt_sgnBit(_$x...) simd_V_map1(_$x, flt_sgnBit)
#define simd_V_flt_inv(_$x...) simd_V_map1(_$x, flt_inv)

/*========== Floating-Point Comparison Operations ===========================*/

#define simd_V_flt_eql(_$lhs, _$rhs...) simd_V_eql(_$lhs, _$rhs)
#define simd_V_flt_neq(_$lhs, _$rhs...) simd_V_neq(_$lhs, _$rhs)
#define simd_V_flt_ord(_$lhs, _$rhs...) simd_V_ord(_$lhs, _$rhs)
#define simd_V_flt_ordApx(_$lhs, _$rhs, _$threshold, _$mode...) __op__simd_V_flt_ordApx( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(threshold), pp_uniqTok(mode), \
    pp_uniqTok(lane_ord), pp_uniqTok(ret), pp_uniqTok(i), \
    _$lhs, _$rhs, _$threshold, _$mode \
)
#define simd_V_flt_ordApxAbs(_$lhs, _$rhs, _$threshold...) \
    simd_V_flt_ordApx(_$lhs, _$rhs, _$threshold, cmp_ApxMode_abs)
#define simd_V_flt_ordApxRel(_$lhs, _$rhs, _$threshold...) \
    simd_V_flt_ordApx(_$lhs, _$rhs, _$threshold, cmp_ApxMode_rel)
#define simd_V_flt_eq(_$lhs, _$rhs...) simd_V_eq(_$lhs, _$rhs)
#define simd_V_flt_ne(_$lhs, _$rhs...) simd_V_ne(_$lhs, _$rhs)
#define simd_V_flt_lt(_$lhs, _$rhs...) simd_V_lt(_$lhs, _$rhs)
#define simd_V_flt_gt(_$lhs, _$rhs...) simd_V_gt(_$lhs, _$rhs)
#define simd_V_flt_le(_$lhs, _$rhs...) simd_V_le(_$lhs, _$rhs)
#define simd_V_flt_ge(_$lhs, _$rhs...) simd_V_ge(_$lhs, _$rhs)

/*========== Floating-Point Min/Max/Clamp Operations ========================*/

#define simd_V_flt_min(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_min)
#define simd_V_flt_max(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_max)
#define simd_V_flt_clamp(_$x, _$lo, _$hi...) simd_V_map3(_$x, _$lo, _$hi, flt_clamp)
#define simd_V_flt_clamp01(_$x...) simd_V_map1(_$x, flt_clamp01)
#define simd_V_flt_wrap(_$x, _$lo, _$hi...) simd_V_map3(_$x, _$lo, _$hi, flt_wrap)
#define simd_V_flt_wrap01(_$x...) simd_V_map1(_$x, flt_wrap01)

/*========== Floating-Point Rounding Operations =============================*/

#define simd_V_flt_trunc(_$x...) simd_V_map1(_$x, flt_trunc)
#define simd_V_flt_round(_$x...) simd_V_map1(_$x, flt_round)
#define simd_V_flt_floor(_$x...) simd_V_map1(_$x, flt_floor)
#define simd_V_flt_ceil(_$x...) simd_V_map1(_$x, flt_ceil)
#define simd_V_flt_nearbyint(_$x...) simd_V_map1(_$x, flt_nearbyint)
#define simd_V_flt_rint(_$x...) simd_V_map1(_$x, flt_rint)
#define simd_V_flt_lround(_$x...) __op__simd_V_map1To(ilong, _$x, flt_lround)
#define simd_V_flt_llround(_$x...) __op__simd_V_map1To(i64, _$x, flt_llround)
#define simd_V_flt_lrint(_$x...) __op__simd_V_map1To(ilong, _$x, flt_lrint)
#define simd_V_flt_llrint(_$x...) __op__simd_V_map1To(i64, _$x, flt_llrint)

/*========== Floating-Point Power and Root Operations =======================*/

#define simd_V_flt_sqrt(_$x...) simd_V_map1(_$x, flt_sqrt)
#define simd_V_flt_cbrt(_$x...) simd_V_map1(_$x, flt_cbrt)
#define simd_V_flt_sq(_$x...) simd_V_map1(_$x, flt_sq)
#define simd_V_flt_cb(_$x...) simd_V_map1(_$x, flt_cb)
#define simd_V_flt_hypot(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_hypot)

/*========== Floating-Point Exponential and Logarithm =======================*/

#define simd_V_flt_exp(_$x...) simd_V_map1(_$x, flt_exp)
#define simd_V_flt_exp2(_$x...) simd_V_map1(_$x, flt_exp2)
#define simd_V_flt_exp10(_$x...) simd_V_map1(_$x, flt_exp10)
#define simd_V_flt_powI(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_powI)
#define simd_V_flt_powF(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_powF)
#define simd_V_flt_ln(_$x...) simd_V_map1(_$x, flt_ln)
#define simd_V_flt_log2(_$x...) simd_V_map1(_$x, flt_log2)
#define simd_V_flt_log10(_$x...) simd_V_map1(_$x, flt_log10)
#define simd_V_flt_log1p(_$x...) simd_V_map1(_$x, flt_log1p)
#define simd_V_flt_expm1(_$x...) simd_V_map1(_$x, flt_expm1)

/*========== Floating-Point Trigonometric Functions =========================*/

#define simd_V_flt_sin(_$x...) simd_V_map1(_$x, flt_sin)
#define simd_V_flt_cos(_$x...) simd_V_map1(_$x, flt_cos)
#define simd_V_flt_tan(_$x...) simd_V_map1(_$x, flt_tan)
#define simd_V_flt_asin(_$x...) simd_V_map1(_$x, flt_asin)
#define simd_V_flt_acos(_$x...) simd_V_map1(_$x, flt_acos)
#define simd_V_flt_atan(_$x...) simd_V_map1(_$x, flt_atan)
#define simd_V_flt_atan2(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, flt_atan2)

/*========== Floating-Point Hyperbolic Functions ============================*/

#define simd_V_flt_sinh(_$x...) simd_V_map1(_$x, flt_sinh)
#define simd_V_flt_cosh(_$x...) simd_V_map1(_$x, flt_cosh)
#define simd_V_flt_tanh(_$x...) simd_V_map1(_$x, flt_tanh)
#define simd_V_flt_asinh(_$x...) simd_V_map1(_$x, flt_asinh)
#define simd_V_flt_acosh(_$x...) simd_V_map1(_$x, flt_acosh)
#define simd_V_flt_atanh(_$x...) simd_V_map1(_$x, flt_atanh)

/*========== Floating-Point IEEE 754 Operations =============================*/

#define simd_V_flt_copySgn(_$x, _$y...) simd_V_map2(_$x, _$y, flt_copySgn)
#define simd_V_flt_nextAfter(_$x, _$y...) simd_V_map2(_$x, _$y, flt_nextAfter)
#define simd_V_flt_nextUp(_$x...) simd_V_map1(_$x, flt_nextUp)
#define simd_V_flt_nextDown(_$x...) simd_V_map1(_$x, flt_nextDown)
#define simd_V_flt_scalb(_$x, _$n...) simd_V_map2(_$x, _$n, flt_scalb)
#define simd_V_flt_scalbn(_$x, _$n...) simd_V_map2(_$x, _$n, flt_scalbn)
#define simd_V_flt_scalbln(_$x, _$n...) simd_V_map2(_$x, _$n, flt_scalbln)
#define simd_V_flt_ilogb(_$x...) __op__simd_V_map1To(i32, _$x, flt_ilogb)
#define simd_V_flt_fdim(_$x, _$y...) simd_V_map2(_$x, _$y, flt_fdim)
#define simd_V_flt_frexp(_$x, _$p_exp...) __op__simd_V_flt_frexp(_$x, _$p_exp)
#define simd_V_flt_modf(_$x, _$p_int...) __op__simd_V_flt_modf(_$x, _$p_int)
#define simd_V_flt_remquo(_$x, _$y, _$p_quo...) __op__simd_V_flt_remquo(_$x, _$y, _$p_quo)
#define simd_V_flt_fract(_$x...) simd_V_map1(_$x, flt_fract)

/*========== Macros and Definitions =========================================*/

#define __type__$simd_V$(_$n, _$T...) \
    TypeOf(_$T __attribute__((vector_size(_$n * sizeOf$(_$T)))))
#define __type__simd_V$$(_$n, _$T...) \
    union { \
        var_(val, $simd_V$(_$n, _$T)); \
        var_(as_arr, A$$(_$n, _$T)) $like_ref; \
    }
#define __alias__simd_V$(_$n, _$T...) \
    tpl$(simd_V, _$n, _$T)
#define __stmt__T_decl_simd_V$(_$n, _$T...) \
    T_decl_A$(_$n, _$T); \
    $maybe_unused typedef union simd_V$(_$n, _$T) simd_V$(_$n, _$T)
#define __stmt__T_impl_simd_V$(_$n, _$T...) \
    T_impl_A$(_$n, _$T); \
    union simd_V$(_$n, _$T) { \
        var_(val, $simd_V$(_$n, _$T)); \
        var_(as_arr, A$(_$n, _$T)) $like_ref; \
    }
#define __stmt__T_use_simd_V$(_$n, _$T...) \
    T_decl_simd_V$(_$n, _$T); \
    T_impl_simd_V$(_$n, _$T)

#define __val__simd_V_n$(_$T...) simd_V_len$(_$T)
#define __type__simd_V_T$(_$T...) TypeOf(null$(_$T*)->val[0])
#define __type__simd_V_TUnqual$(_$T...) TypeOfUnqual(null$(_$T*)->val[0])

#define __op__simd_V_zero$(_$T...) l0$(_$T)
#define __op__simd_V_zero() cleared()
#define __op__simd_V_init$(...) \
    __step_inline__simd_V_init$(pp_defer(__emit_inline__simd_V_init$)(__param_parse__simd_V_init$ __VA_ARGS__))
#define __step_inline__simd_V_init$(...) __VA_ARGS__
#define __param_parse__simd_V_init$(...) __VA_ARGS__,
#define __emit_inline__simd_V_init$(_$T, _$initial...) l$((_$T)simd_V_init(_$initial))
#define __op__simd_V_init(_$initial...) { .val = _$initial }
#define __op__simd_V_splat$(...) __inline__simd_V_splat$(__inline__simd_V_splat$__parseT __VA_ARGS__)
#define __inline__simd_V_splat$__parseT(_$T...) _$T,
#define __inline__simd_V_splat$(_$T, _$x...) ({ \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { __ret.val[__i] = as$(simd_V_TUnqual$(_$T))(_$x); } \
    __ret; \
})
#define __op__simd_V_splat(_$v, _$x...) ({ \
    let __base = _$v; \
    var_(__ret, TypeOfUnqual(__base)) = simd_V_zero$((TypeOfUnqual(__base))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { __ret.val[__i] = as$(simd_V_TUnqual$(TypeOf(__base)))(_$x); } \
    __ret; \
})
#define __op__simd_V_from$(...) \
    __step_inline__simd_V_from$(pp_defer(__emit_inline__simd_V_from$)(__param_parse__simd_V_from$ __VA_ARGS__))
#define __step_inline__simd_V_from$(...) __VA_ARGS__
#define __param_parse__simd_V_from$(_$T...) _$T,
#define __emit_inline__simd_V_from$(_$T, _$initial...) \
    ({ \
        let __arr = A_from$((_$T)_$initial); \
        typedef TypeOf(__arr) __simd_V_from__Arr; \
        var_(__ret, simd_V$$(A_n$(__simd_V_from__Arr), _$T)) = {}; \
        loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { __ret.val[__i] = __arr.val[__i]; } \
        __ret; \
    })
#define __op__simd_V_fromA$(...) __inline__simd_V_fromA$(__inline__simd_V_fromA$__parseT __VA_ARGS__)
#define __inline__simd_V_fromA$__parseT(_$T...) _$T,
#define __inline__simd_V_fromA$(_$T, _$a...) l$((_$T){ .as_arr = { _$a } })
#define __op__simd_V_fromA(_$a...) \
    l$((simd_V$$(A_n$(TypeOf(_$a)), A_T$(TypeOf(_$a)))){ .as_arr = { _$a } })
#define __op__simd_V_toA$(...) __inline__simd_V_toA$(__inline__simd_V_toA$__parseT __VA_ARGS__)
#define __inline__simd_V_toA$__parseT(_$T...) _$T,
#define __inline__simd_V_toA$(_$T, _$v...) l$((_$T){ .val = (*(_$v).as_arr).val })
#define __op__simd_V_toA(_$v...) (*(_$v).as_arr)

#define __op__simd_V_cat(__lhs, __rhs, __ret, __i, _$lhs, _$rhs...) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    var_(__ret, simd_V$$((simd_V_len(__lhs) + simd_V_len(__rhs)), simd_V_TUnqual$(TypeOf(__lhs)))) = simd_V_zero(); \
    claim_assert(eqlType$(simd_V_TUnqual$(TypeOf(__lhs)), simd_V_TUnqual$(TypeOf(__rhs)))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { *simd_V_at((__ret)[__i]) = *simd_V_at((__lhs)[__i]); } \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__rhs); ++__i) { *simd_V_at((__ret)[simd_V_len(__lhs) + __i]) = *simd_V_at((__rhs)[__i]); } \
    __ret; \
})
#define __op__simd_V_repeat$(...) \
    __op__simd_V_repeat$__step(__op__simd_V_repeat$__parseT __VA_ARGS__)
#define __op__simd_V_repeat$__parseT(_$T...) \
    _$T, __op__simd_V_repeat$__parseV
#define __op__simd_V_repeat$__parseV(_$v...) \
    pp_uniqTok(v), pp_uniqTok(ret), pp_uniqTok(i), _$v
#define __op__simd_V_repeat$__step(...) \
    __op__simd_V_repeat$__emit(__VA_ARGS__)
#define __op__simd_V_repeat$__emit(_$T, __x, __ret, __i, _$x...) ({ \
    let __x = _$x; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__x) > 0); \
    claim_assert(eqlType$(simd_V_TUnqual$(_$T), simd_V_TUnqual$(TypeOf(__x)))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = *simd_V_at((__x)[__i % simd_V_len(__x)]); \
    } \
    __ret; \
})
#define __op__simd_V_extract$(...) \
    __op__simd_V_extract$__step(__op__simd_V_extract$__parseT __VA_ARGS__)
#define __op__simd_V_extract$__parseT(_$T...) \
    _$T, __op__simd_V_extract$__parseV
#define __op__simd_V_extract$__parseV(_$v, _$first...) \
    pp_uniqTok(v), pp_uniqTok(first), pp_uniqTok(ret), pp_uniqTok(i), _$v, _$first
#define __op__simd_V_extract$__step(...) \
    __op__simd_V_extract$__emit(__VA_ARGS__)
#define __op__simd_V_extract$__emit(_$T, __x, __first, __ret, __i, _$x, _$first...) ({ \
    let __x = _$x; \
    let_(__first, usize) = _$first; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(eqlType$(simd_V_TUnqual$(_$T), simd_V_TUnqual$(TypeOf(__x)))); \
    claim_assert(__first + simd_V_len(__ret) <= simd_V_len(__x)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { *simd_V_at((__ret)[__i]) = *simd_V_at((__x)[__first + __i]); } \
    __ret; \
})

#define __op__simd_V_val(_$v...) ((_$v).val)
#define __op__simd_V_len$(_$T...) simd_V_len(*null$(_$T*))
#define __op__simd_V_len(_$v...) A_len(*(_$v).as_arr)
#define __op__simd_V_at(...) __op__simd_V_at__step(__op__simd_V_at__parse __VA_ARGS__)
#define __op__simd_V_at__parse(_$v...) pp_uniqTok(v), _$v, pp_uniqTok(idx),
#define __op__simd_V_at__step(...) __op__simd_V_at__emit(__VA_ARGS__)
#define __op__simd_V_at__emit(__v, _$v, __idx, _$idx...) ({ \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _$idx)); \
    let_(__v, TypeOf(&(_$v))) = &(_$v); \
    claim_assert_fmt(__idx < simd_V_len(*__v), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, simd_V_len(*__v)); \
    &(*__v->as_arr).val[__idx]; \
})

#define __op__simd_V_load$(...) \
    __step_inline__simd_V_load$(pp_defer(__emit_inline__simd_V_load$)(__param_parse__simd_V_load$ __VA_ARGS__))
#define __step_inline__simd_V_load$(...) __VA_ARGS__
#define __param_parse__simd_V_load$(_$T...) _$T,
#define __emit_inline__simd_V_load$(_$T, _$ptr...) ({ \
    let __ptr = _$ptr; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    typedef TypeOfUnqual(__ret.val) __raw_vec_t __attribute__((aligned(1), may_alias)); \
    __ret.val = *as$(const __raw_vec_t*)(__ptr); \
    __ret; \
})
#define __op__simd_V_load(_$v, _$ptr...) ({ \
    let __base = _$v; \
    let __ptr = _$ptr; \
    var_(__ret, TypeOfUnqual(__base)) = simd_V_zero$((TypeOfUnqual(__base))); \
    typedef TypeOfUnqual(__ret.val) __raw_vec_t __attribute__((aligned(1), may_alias)); \
    __ret.val = *as$(const __raw_vec_t*)(__ptr); \
    __ret; \
})
#define __op__simd_V_store(_$ptr, _$v...) ({ \
    let __v = _$v; \
    let __ptr = _$ptr; \
    typedef TypeOfUnqual(__v.val) __raw_vec_t __attribute__((aligned(1), may_alias)); \
    *as$(__raw_vec_t*)(__ptr) = __v.val; \
})

#define __op__simd_V_as$(...) \
    __step_inline__simd_V_as$(pp_defer(__emit_inline__simd_V_as$)(__param_parse__simd_V_as$ __VA_ARGS__))
#define __step_inline__simd_V_as$(...) __VA_ARGS__
#define __param_parse__simd_V_as$(_$T...) _$T,
#define __emit_inline__simd_V_as$(_$T, _$v...) ({ \
    let __v = _$v; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(_$T))(*simd_V_at((__v)[__i])); \
    } \
    __ret; \
})
#define __op__simd_V_boolToInt$(...) \
    __step_inline__simd_V_boolToInt$(pp_defer(__emit_inline__simd_V_boolToInt$)(__param_parse__simd_V_boolToInt$ __VA_ARGS__))
#define __step_inline__simd_V_boolToInt$(...) __VA_ARGS__
#define __param_parse__simd_V_boolToInt$(_$T...) _$T,
#define __emit_inline__simd_V_boolToInt$(_$T, _$v...) ({ \
    let __v = _$v; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(_$T))(boolToInt(bool_(*simd_V_at((__v)[__i]) != 0))); \
    } \
    __ret; \
})
#define __op__simd_V_intCast$(...) \
    __step_inline__simd_V_intCast$(pp_defer(__emit_inline__simd_V_intCast$)(__param_parse__simd_V_intCast$ __VA_ARGS__))
#define __step_inline__simd_V_intCast$(...) __VA_ARGS__
#define __param_parse__simd_V_intCast$(_$T...) _$T,
#define __emit_inline__simd_V_intCast$(_$T, _$v...) ({ \
    let __v = _$v; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = intCast$((simd_V_TUnqual$(_$T))(*simd_V_at((__v)[__i]))); \
    } \
    __ret; \
})
#define __op__simd_V_intToFlt$(...) \
    __step_inline__simd_V_intToFlt$(pp_defer(__emit_inline__simd_V_intToFlt$)(__param_parse__simd_V_intToFlt$ __VA_ARGS__))
#define __step_inline__simd_V_intToFlt$(...) __VA_ARGS__
#define __param_parse__simd_V_intToFlt$(_$T...) _$T,
#define __emit_inline__simd_V_intToFlt$(_$T, _$v...) ({ \
    let __v = _$v; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = intToFlt$((simd_V_TUnqual$(_$T))(*simd_V_at((__v)[__i]))); \
    } \
    __ret; \
})
#define __op__simd_V_fltToInt$(...) \
    __step_inline__simd_V_fltToInt$(pp_defer(__emit_inline__simd_V_fltToInt$)(__param_parse__simd_V_fltToInt$ __VA_ARGS__))
#define __step_inline__simd_V_fltToInt$(...) __VA_ARGS__
#define __param_parse__simd_V_fltToInt$(_$T...) _$T,
#define __emit_inline__simd_V_fltToInt$(_$T, _$v...) ({ \
    let __v = _$v; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = fltToInt$((simd_V_TUnqual$(_$T))(*simd_V_at((__v)[__i]))); \
    } \
    __ret; \
})
#define __op__simd_V_fltCast$(...) \
    __step_inline__simd_V_fltCast$(pp_defer(__emit_inline__simd_V_fltCast$)(__param_parse__simd_V_fltCast$ __VA_ARGS__))
#define __step_inline__simd_V_fltCast$(...) __VA_ARGS__
#define __param_parse__simd_V_fltCast$(_$T...) _$T,
#define __emit_inline__simd_V_fltCast$(_$T, _$v...) ({ \
    let __v = _$v; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = fltCast$((simd_V_TUnqual$(_$T))(*simd_V_at((__v)[__i]))); \
    } \
    __ret; \
})

#define __op__simd_V_uintTypedMap$(_$op, ...) \
    __step_inline__simd_V_uintTypedMap$(pp_defer(__emit_inline__simd_V_uintTypedMap$)(_$op, __param_parse__simd_V_uintTypedMap$ __VA_ARGS__))
#define __step_inline__simd_V_uintTypedMap$(...) __VA_ARGS__
#define __param_parse__simd_V_uintTypedMap$(_$T...) _$T, pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane),
#define __emit_inline__simd_V_uintTypedMap$(_$op, _$T, __x, __ret, __i, __lane, _$x...) ({ \
    let __x = _$x; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__x)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let_(__lane, simd_V_TUnqual$(TypeOf(__x))) = *simd_V_at((__x)[__i]); \
        *simd_V_at((__ret)[__i]) = pp_cat(_$op, $)((simd_V_TUnqual$(_$T))(__lane)); \
    } \
    __ret; \
})
#define __op__simd_V_intTypedMap2$(_$op, ...) \
    __step_inline__simd_V_intTypedMap2$(pp_defer(__emit_inline__simd_V_intTypedMap2$)(_$op, __param_parse__simd_V_intTypedMap2$ __VA_ARGS__))
#define __step_inline__simd_V_intTypedMap2$(...) __VA_ARGS__
#define __param_parse__simd_V_intTypedMap2$(_$T...) _$T, pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lhs_lane), pp_uniqTok(rhs_lane), __param_parse__simd_V_intTypedMap2$__operands
#define __param_parse__simd_V_intTypedMap2$__operands(_$lhs, _$rhs...) _$lhs, _$rhs
#define __emit_inline__simd_V_intTypedMap2$(_$op, _$T, __lhs, __rhs, __ret, __i, __lhs_lane, __rhs_lane, _$lhs, _$rhs...) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__lhs)); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        $maybe_unused let_(__lhs_lane, simd_V_TUnqual$(TypeOf(__lhs))) = *simd_V_at((__lhs)[__i]); \
        $maybe_unused let_(__rhs_lane, simd_V_TUnqual$(TypeOf(__rhs))) = *simd_V_at((__rhs)[__i]); \
        *simd_V_at((__ret)[__i]) = pp_cat(_$op, $)((simd_V_TUnqual$(_$T))(__lhs_lane, __rhs_lane)); \
    } \
    __ret; \
})
#define __op__simd_V_map1U32(_$x, _$op...) \
    __op__simd_V_map1U32__emit(pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane), _$x, _$op)
#define __op__simd_V_map1U32__emit(__x, __ret, __i, __lane, _$x, _$op...) ({ \
    let __x = _$x; \
    var_(__ret, simd_V$$((simd_V_n$(TypeOf(__x))), u32)) = simd_V_zero(); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let_(__lane, simd_V_TUnqual$(TypeOf(__x))) = *simd_V_at((__x)[__i]); \
        *simd_V_at((__ret)[__i]) = _$op(__lane); \
    } \
    __ret; \
})
#define __op__simd_V_map1To(_$T, _$x, _$op...) \
    __op__simd_V_map1To__emit(_$T, pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane), _$x, _$op)
#define __op__simd_V_map1To__emit(_$T, __x, __ret, __i, __lane, _$x, _$op...) ({ \
    let __x = _$x; \
    var_(__ret, simd_V$$((simd_V_n$(TypeOf(__x))), _$T)) = simd_V_zero(); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let_(__lane, simd_V_TUnqual$(TypeOf(__x))) = *simd_V_at((__x)[__i]); \
        *simd_V_at((__ret)[__i]) = _$op(__lane); \
    } \
    __ret; \
})

#define __op__simd_V_map1(__x, __ret, __i, __lane, _$x, _$op...) ({ \
    let __x = _$x; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let __lane = *simd_V_at((__x)[__i]); \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(TypeOf(__ret)))(_$op(__lane)); \
    } \
    __ret; \
})
#define __op__simd_V_map1$(...) \
    __step_inline__simd_V_map1$(pp_defer(__emit_inline__simd_V_map1$)(__param_parse__simd_V_map1$ __VA_ARGS__))
#define __step_inline__simd_V_map1$(...) __VA_ARGS__
#define __param_parse__simd_V_map1$(_$T...) _$T, pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane),
#define __emit_inline__simd_V_map1$(_$T, __x, __ret, __i, __lane, _$x, _$op...) ({ \
    let __x = _$x; \
    var_(__ret, _$T) = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__x)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let __lane = *simd_V_at((__x)[__i]); \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(_$T))(_$op(__lane)); \
    } \
    __ret; \
})
#define __op__simd_V_map2(__lhs, __rhs, __ret, __i, __lhs_lane, __rhs_lane, _$lhs, _$rhs, _$op...) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    var __ret = simd_V_zero$((TypeOfUnqual(__lhs))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        $maybe_unused let __lhs_lane = *simd_V_at((__lhs)[__i]); \
        $maybe_unused let __rhs_lane = *simd_V_at((__rhs)[__i]); \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(TypeOf(__ret)))(_$op(__lhs_lane, __rhs_lane)); \
    } \
    __ret; \
})
#define __op__simd_V_map2$(...) \
    __step_inline__simd_V_map2$(pp_defer(__emit_inline__simd_V_map2$)(__param_parse__simd_V_map2$ __VA_ARGS__))
#define __step_inline__simd_V_map2$(...) __VA_ARGS__
#define __param_parse__simd_V_map2$(_$T...) _$T, pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lhs_lane), pp_uniqTok(rhs_lane),
#define __emit_inline__simd_V_map2$(_$T, __lhs, __rhs, __ret, __i, __lhs_lane, __rhs_lane, _$lhs, _$rhs, _$op...) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    var __ret = simd_V_zero$((_$T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__lhs)); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        $maybe_unused let __lhs_lane = *simd_V_at((__lhs)[__i]); \
        $maybe_unused let __rhs_lane = *simd_V_at((__rhs)[__i]); \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(_$T))(_$op(__lhs_lane, __rhs_lane)); \
    } \
    __ret; \
})
#define __op__simd_V_map3(__x, __y, __z, __ret, __i, __x_lane, __y_lane, __z_lane, _$x, _$y, _$z, _$op...) ({ \
    let __x = _$x; \
    let __y = _$y; \
    let __z = _$z; \
    claim_assert(simd_V_len(__x) == simd_V_len(__y)); \
    claim_assert(simd_V_len(__x) == simd_V_len(__z)); \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        $maybe_unused let_(__x_lane, simd_V_TUnqual$(TypeOf(__x))) = *simd_V_at((__x)[__i]); \
        $maybe_unused let_(__y_lane, simd_V_TUnqual$(TypeOf(__y))) = *simd_V_at((__y)[__i]); \
        $maybe_unused let_(__z_lane, simd_V_TUnqual$(TypeOf(__z))) = *simd_V_at((__z)[__i]); \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(TypeOf(__ret)))(_$op(__x_lane, __y_lane, __z_lane)); \
    } \
    __ret; \
})
#define __op__simd_V_reduce(__x, __ret, __i, __lane, _$x, _$initial, _$op...) ({ \
    let __x = _$x; \
    var __ret = _$initial; \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let __lane = *simd_V_at((__x)[__i]); \
        __ret = _$op(__ret, __lane); \
    } \
    __ret; \
})

#define __op__simd_V_select(__mask, __if_true, __if_false, __ret, __i, _$mask, _$if_true, _$if_false...) ({ \
    let __mask = _$mask; \
    let __if_true = _$if_true; \
    let __if_false = _$if_false; \
    var __ret = simd_V_zero$((TypeOfUnqual(__if_false))); \
    claim_assert(simd_V_len(__mask) == simd_V_len(__ret)); \
    claim_assert(simd_V_len(__if_false) == simd_V_len(__if_true)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = *simd_V_at((__mask)[__i]) ? *simd_V_at((__if_true)[__i]) : *simd_V_at((__if_false)[__i]); \
    } \
    __ret; \
})
#define __op__simd_V_shuffle(__lhs, __rhs, __idxs, __ret, __lhs_len, __rhs_len, __i, __idx, _$lhs, _$rhs, _$idxs...) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    let __idxs = A_from$((usize){ _$idxs }); \
    var __ret = simd_V_zero$((simd_V$$((A_len(__idxs)), simd_V_TUnqual$(TypeOf(__lhs))))); \
    let __lhs_len = simd_V_len(__lhs); \
    let __rhs_len = simd_V_len(__rhs); \
    claim_assert(__lhs_len == __rhs_len); \
    __ret.val = __builtin_shufflevector(__lhs.val, __rhs.val, _$idxs); \
    __ret; \
})

#define __op__simd_V_isZero(_$x...) simd_V_bool_none(_$x)
#define __op__simd_V_isNonzero(_$x...) simd_V_bool_any(_$x)

#define __op__simd_V_add(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, +)
#define __op__simd_V_sub(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, -)
#define __op__simd_V_mul(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, *)
#define __op__simd_V_mulAdd(_$x, _$y, _$z...) ({ \
    let __x = _$x; \
    let __y = _$y; \
    let __z = _$z; \
    l$((TypeOfUnqual(__x)){ .val = (__x.val * __y.val) + __z.val }); \
})
#define __op__simd_V_div(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, /)
#define __op__simd_V_rem(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, %)
#define __op__simd_V_native2(_$lhs, _$rhs, _$op) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    l$((TypeOfUnqual(__lhs)){ .val = __lhs.val _$op __rhs.val }); \
})

#define __op__simd_V_neg(_$x...) ({ \
    let __x = _$x; \
    l$((TypeOfUnqual(__x)){ .val = -__x.val }); \
})
#define __op__simd_V_abs__step(_$x...) simd_V_map1(_$x, pri_abs)
#define __op__simd_V_sgn__step(_$x...) simd_V_map1(_$x, pri_sgn)

#define __op__simd_V_not(_$x...) ({ \
    let __x = _$x; \
    l$((TypeOfUnqual(__x)){ .val = __x.val == simd_V_zero$((TypeOfUnqual(__x))).val }); \
})
#define __op__simd_V_and(_$x, _$y...) ({ \
    let __x = _$x; \
    let __y = _$y; \
    let __zero = simd_V_zero$((TypeOfUnqual(__x))); \
    l$((TypeOfUnqual(__x)){ .val = (__x.val != __zero.val) & (__y.val != __zero.val) }); \
})
#define __op__simd_V_or(_$x, _$y...) ({ \
    let __x = _$x; \
    let __y = _$y; \
    let __zero = simd_V_zero$((TypeOfUnqual(__x))); \
    l$((TypeOfUnqual(__x)){ .val = (__x.val != __zero.val) | (__y.val != __zero.val) }); \
})

#define __op__simd_V_eql(_$lhs, _$rhs...) simd_V_bool_all(simd_V_eq(_$lhs, _$rhs))
#define __op__simd_V_neq(_$lhs, _$rhs...) bool_(!simd_V_eql(_$lhs, _$rhs))
#define __op__simd_V_ord__step(_$lhs, _$rhs...) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    var_(__ret, cmp_Ord) = cmp_Ord_eq; \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        if (__lhs.val[__i] < __rhs.val[__i]) { \
            __ret = cmp_Ord_lt; \
            break; \
        } \
        if (__lhs.val[__i] > __rhs.val[__i]) { \
            __ret = cmp_Ord_gt; \
            break; \
        } \
    } \
    __ret; \
})
#define __op__simd_V_eq(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, ==)
#define __op__simd_V_ne(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, !=)
#define __op__simd_V_lt(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, <)
#define __op__simd_V_gt(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, >)
#define __op__simd_V_le(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, <=)
#define __op__simd_V_ge(_$lhs, _$rhs...) __op__simd_V_native2(_$lhs, _$rhs, >=)

#define __op__simd_V_min2__step(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, pri_min)
#define __op__simd_V_min3__step(_$1st, _$2nd, _$3rd...) simd_V_min(simd_V_min(_$1st, _$2nd), _$3rd)
#define __op__simd_V_min4__step(_$1st, _$2nd, _$3rd, _$4th...) simd_V_min(simd_V_min(_$1st, _$2nd), simd_V_min(_$3rd, _$4th))
#define __op__simd_V_findMin__step(_$vals...) __op__simd_V_findMin__emit(pp_uniqTok(vals), _$vals)
#define __op__simd_V_findMin__emit(__vals, _$vals...) ({ \
    let __vals = _$vals; \
    var __ret = *simd_V_at((__vals)[0]); \
    loop_inline_(for) (usize __i = 1; __i < simd_V_len(__vals); ++__i) { \
        __ret = pri_min(__ret, *simd_V_at((__vals)[__i])); \
    } \
    __ret; \
})

#define __op__simd_V_max2__step(_$lhs, _$rhs...) simd_V_map2(_$lhs, _$rhs, pri_max)
#define __op__simd_V_max3__step(_$1st, _$2nd, _$3rd...) simd_V_max(simd_V_max(_$1st, _$2nd), _$3rd)
#define __op__simd_V_max4__step(_$1st, _$2nd, _$3rd, _$4th...) simd_V_max(simd_V_max(_$1st, _$2nd), simd_V_max(_$3rd, _$4th))
#define __op__simd_V_findMax__step(_$vals...) __op__simd_V_findMax__emit(pp_uniqTok(vals), _$vals)
#define __op__simd_V_findMax__emit(__vals, _$vals...) ({ \
    let __vals = _$vals; \
    var __ret = *simd_V_at((__vals)[0]); \
    loop_inline_(for) (usize __i = 1; __i < simd_V_len(__vals); ++__i) { \
        __ret = pri_max(__ret, *simd_V_at((__vals)[__i])); \
    } \
    __ret; \
})
#define __op__simd_V_clamp__step(_$x, _$lo, _$hi...) simd_V_map3(_$x, _$lo, _$hi, pri_clamp)

#define __op__simd_V_dot(__lhs, __rhs, _$lhs, _$rhs...) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    simd_V_reduce(simd_V_mul(__lhs, __rhs), as$(simd_V_TUnqual$(TypeOf(__lhs)))(0), pri_add); \
})
#define __op__simd_V_cross3(__lhs, __rhs, __ret, _$lhs, _$rhs...) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    claim_assert(simd_V_len(__lhs) >= 3 && simd_V_len(__lhs) == simd_V_len(__rhs)); \
    var_(__ret, TypeOfUnqual(__lhs)) = simd_V_zero(); \
    __ret.val[0] = __lhs.val[1] * __rhs.val[2] - __lhs.val[2] * __rhs.val[1]; \
    __ret.val[1] = __lhs.val[2] * __rhs.val[0] - __lhs.val[0] * __rhs.val[2]; \
    __ret.val[2] = __lhs.val[0] * __rhs.val[1] - __lhs.val[1] * __rhs.val[0]; \
    __ret; \
})

#define __op__simd_V_bool_or(_acc, _lane...) bool_((_acc) || ((_lane) != 0))
#define __op__simd_V_bool_and(_acc, _lane...) bool_((_acc) && ((_lane) != 0))
#define __op__simd_V_bool_any(_$x...) ({ \
    simd_V_reduce(_$x, false, __op__simd_V_bool_or); \
})
#define __op__simd_V_bool_all(_$x...) ({ \
    simd_V_reduce(_$x, true, __op__simd_V_bool_and); \
})
#define __op__simd_V_bool_none(_$x...) (!simd_V_bool_any(_$x))
#define __op__simd_V_bool_bitMask_fallback(__x, __ret, __i) \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        __ret |= as$(usize)(*simd_V_at((__x)[__i]) != 0) << __i; \
    }
#if simd_V_has_sse2
#define __op__simd_V_bool_bitMask(__x, __normalized, __raw, __bit_mask, __masked, __paired, __ret, __i, _$x...) ({ \
    let __x = _$x; \
    claim_assert(simd_V_len(__x) <= sizeOf$(usize) * arch_bits_per_byte); \
    var_(__ret, usize) = 0; \
    if (sizeof(__x.val) == 16 && sizeof(__x.val[0]) == 1) { \
        let __normalized = simd_V_ne(__x, simd_V_zero$((TypeOfUnqual(__x)))); \
        __m128i __raw = _mm_setzero_si128(); \
        __builtin_memcpy(&__raw, &__normalized.val, pri_min(sizeof(__raw), sizeof(__normalized.val))); \
        __ret = as$(usize)(_mm_movemask_epi8(__raw)); \
    } else { \
        __op__simd_V_bool_bitMask_fallback(__x, __ret, __i); \
    } \
    __ret; \
})
#elif simd_V_has_neon
#define __op__simd_V_bool_bitMask(__x, __normalized, __raw, __bit_mask, __masked, __paired, __ret, __i, _$x...) ({ \
    let __x = _$x; \
    claim_assert(simd_V_len(__x) <= sizeOf$(usize) * arch_bits_per_byte); \
    var_(__ret, usize) = 0; \
    if (sizeof(__x.val) == 16 && sizeof(__x.val[0]) == 1) { \
        let __normalized = simd_V_ne(__x, simd_V_zero$((TypeOfUnqual(__x)))); \
        uint8x16_t __raw = vdupq_n_u8(0); \
        __builtin_memcpy(&__raw, &__normalized.val, pri_min(sizeof(__raw), sizeof(__normalized.val))); \
        const uint8x16_t __bit_mask = { 1, 2, 4, 8, 1, 2, 4, 8, 1, 2, 4, 8, 1, 2, 4, 8 }; \
        let __masked = vandq_u8(__raw, __bit_mask); \
        uint8x8_t __paired = vpadd_u8(vget_low_u8(__masked), vget_high_u8(__masked)); \
        __paired = vpadd_u8(__paired, __paired); \
        __paired = vpadd_u8(__paired, __paired); \
        __ret = as$(usize)(vget_lane_u16(vreinterpret_u16_u8(__paired), 0)); \
    } else { \
        __op__simd_V_bool_bitMask_fallback(__x, __ret, __i); \
    } \
    __ret; \
})
#elif simd_V_has_wasm_simd128
#define __op__simd_V_bool_bitMask(__x, __normalized, __raw, __bit_mask, __masked, __paired, __ret, __i, _$x...) ({ \
    let __x = _$x; \
    claim_assert(simd_V_len(__x) <= sizeOf$(usize) * arch_bits_per_byte); \
    var_(__ret, usize) = 0; \
    if (sizeof(__x.val) == 16 && sizeof(__x.val[0]) == 1) { \
        let __normalized = simd_V_ne(__x, simd_V_zero$((TypeOfUnqual(__x)))); \
        v128_t __raw = wasm_i8x16_splat(0); \
        __builtin_memcpy(&__raw, &__normalized.val, pri_min(sizeof(__raw), sizeof(__normalized.val))); \
        __ret = as$(usize)(wasm_i8x16_bitmask(__raw)); \
    } else { \
        __op__simd_V_bool_bitMask_fallback(__x, __ret, __i); \
    } \
    __ret; \
})
#else
#define __op__simd_V_bool_bitMask(__x, __normalized, __raw, __bit_mask, __masked, __paired, __ret, __i, _$x...) ({ \
    let __x = _$x; \
    claim_assert(simd_V_len(__x) <= sizeOf$(usize) * arch_bits_per_byte); \
    var_(__ret, usize) = 0; \
    __op__simd_V_bool_bitMask_fallback(__x, __ret, __i); \
    __ret; \
})
#endif

#define __op__simd_V_int_addOverflow(_$lhs, _$rhs, _p_out...) \
    __op__simd_V_int_overflow2(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), pp_uniqTok(mask), pp_uniqTok(i), pp_uniqTok(lane_out), _$lhs, _$rhs, _p_out, int_addOverflow)
#define __op__simd_V_int_subOverflow(_$lhs, _$rhs, _p_out...) \
    __op__simd_V_int_overflow2(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), pp_uniqTok(mask), pp_uniqTok(i), pp_uniqTok(lane_out), _$lhs, _$rhs, _p_out, int_subOverflow)
#define __op__simd_V_int_mulOverflow(_$lhs, _$rhs, _p_out...) \
    __op__simd_V_int_overflow2(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), pp_uniqTok(mask), pp_uniqTok(i), pp_uniqTok(lane_out), _$lhs, _$rhs, _p_out, int_mulOverflow)
#define __op__simd_V_int_overflow2(__lhs, __rhs, __out, __mask, __i, __lane_out, _$lhs, _$rhs, _p_out, _$op...) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    let_(__out, TypeOf(_p_out)) = _p_out; \
    var_(__mask, TypeOfUnqual(__lhs)) = simd_V_zero$((TypeOfUnqual(__lhs))); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        var_(__lane_out, simd_V_TUnqual$(TypeOf(__lhs))) = 0; \
        *simd_V_at((*__out)[__i]) = __lane_out; \
        *simd_V_at((__mask)[__i]) = as$(simd_V_TUnqual$(TypeOf(__mask)))(boolToInt(_$op(*simd_V_at((__lhs)[__i]), *simd_V_at((__rhs)[__i]), simd_V_at((*__out)[__i])))); \
    } \
    __mask; \
})
#define __op__simd_V_uint_addCarry(_$lhs, _$rhs, _carry_in, _p_carry_out...) \
    __op__simd_V_uint_carry3(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(carry_in), pp_uniqTok(carry_out), pp_uniqTok(ret), pp_uniqTok(i), _$lhs, _$rhs, _carry_in, _p_carry_out, uint_addCarry)
#define __op__simd_V_uint_subBorrow(_$lhs, _$rhs, _borrow_in, _p_borrow_out...) \
    __op__simd_V_uint_carry3(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(carry_in), pp_uniqTok(carry_out), pp_uniqTok(ret), pp_uniqTok(i), _$lhs, _$rhs, _borrow_in, _p_borrow_out, uint_subBorrow)
#define __op__simd_V_uint_carry3(__lhs, __rhs, __carry_in, __p_carry_out, __ret, __i, _$lhs, _$rhs, _carry_in, _p_carry_out, _$op...) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    let __carry_in = _carry_in; \
    $maybe_unused let_(__p_carry_out, TypeOf(_p_carry_out)) = _p_carry_out; \
    var_(__ret, TypeOfUnqual(__lhs)) = simd_V_zero$((TypeOfUnqual(__lhs))); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__carry_in)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        *simd_V_at((__ret)[__i]) = _$op(*simd_V_at((__lhs)[__i]), *simd_V_at((__rhs)[__i]), *simd_V_at((__carry_in)[__i]), simd_V_at((*__p_carry_out)[__i])); \
    } \
    __ret; \
})
#define __op__simd_V_iint_negOverflow(_$x, _p_out...) \
    __op__simd_V_iint_overflow1(pp_uniqTok(x), pp_uniqTok(out), pp_uniqTok(mask), pp_uniqTok(i), _$x, _p_out, iint_negOverflow)
#define __op__simd_V_iint_absOverflow(_$x, _p_out...) \
    __op__simd_V_iint_overflow1(pp_uniqTok(x), pp_uniqTok(out), pp_uniqTok(mask), pp_uniqTok(i), _$x, _p_out, iint_absOverflow)
#define __op__simd_V_iint_overflow1(__x, __out, __mask, __i, _$x, _p_out, _$op...) ({ \
    let __x = _$x; \
    let_(__out, TypeOf(_p_out)) = _p_out; \
    var_(__mask, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__mask)[__i]) = as$(simd_V_TUnqual$(TypeOf(__mask)))(boolToInt(_$op(*simd_V_at((__x)[__i]), simd_V_at((*__out)[__i])))); \
    } \
    __mask; \
})

#define __op__simd_V_int_not(_$x...) ({ \
    let __x = _$x; \
    l$((TypeOfUnqual(__x)){ .val = ~__x.val }); \
})
#define __op__simd_V_int_shl(_$x, _$y...) ({ \
    let __x = _$x; \
    let __y = _$y; \
    l$((TypeOfUnqual(__x)){ .val = __x.val << __y }); \
})
#define __op__simd_V_int_shr(_$x, _$y...) ({ \
    let __x = _$x; \
    let __y = _$y; \
    l$((TypeOfUnqual(__x)){ .val = __x.val >> __y }); \
})
#define __op__simd_V_int_and(_$x, _$y...) __op__simd_V_native2(_$x, _$y, &)
#define __op__simd_V_int_xor(_$x, _$y...) __op__simd_V_native2(_$x, _$y, ^)
#define __op__simd_V_int_or(_$x, _$y...) __op__simd_V_native2(_$x, _$y, |)
#define __op__simd_V_int_setBit(_$x, _$bit...) simd_V_int_or(_$x, simd_V_splat(_$x, as$(simd_V_TUnqual$(TypeOf(_$x)))(1) << (_$bit)))
#define __op__simd_V_int_resetBit(_$x, _$bit...) simd_V_int_and(_$x, simd_V_splat(_$x, ~(as$(simd_V_TUnqual$(TypeOf(_$x)))(1) << (_$bit))))
#define __op__simd_V_int_toggleBit(_$x, _$bit...) simd_V_int_xor(_$x, simd_V_splat(_$x, as$(simd_V_TUnqual$(TypeOf(_$x)))(1) << (_$bit)))

#define __op__simd_V_int_rotateLeft(_$x, _$n...) ({ \
    let __x = _$x; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = int_rotateLeft(*simd_V_at((__x)[__i]), _$n); \
    } \
    __ret; \
})
#define __op__simd_V_int_rotateRight(_$x, _$n...) ({ \
    let __x = _$x; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = int_rotateRight(*simd_V_at((__x)[__i]), _$n); \
    } \
    __ret; \
})

#define __op__simd_V_int_hasBit(_$x, _$bit...) \
    __op__simd_V_int_hasBit__emit(pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), _$x, _$bit)
#define __op__simd_V_int_hasBit__emit(__x, __ret, __i, _$x, _$bit...) ({ \
    let __x = _$x; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(TypeOf(__ret)))(boolToInt(int_hasBit(*simd_V_at((__x)[__i]), _$bit))); \
    } \
    __ret; \
})
#define __op__simd_V_int_extractBits(_$x, _$off, _$len...) \
    __op__simd_V_int_extractBits__emit(pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), _$x, _$off, _$len)
#define __op__simd_V_int_extractBits__emit(__x, __ret, __i, _$x, _$off, _$len...) ({ \
    let __x = _$x; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = int_extractBits(*simd_V_at((__x)[__i]), _$off, _$len); \
    } \
    __ret; \
})
#define __op__simd_V_int_replaceBits(_$x, _$off, _$len, _$bits...) \
    __op__simd_V_int_replaceBits__emit(pp_uniqTok(x), pp_uniqTok(bits), pp_uniqTok(ret), pp_uniqTok(i), _$x, _$off, _$len, _$bits)
#define __op__simd_V_int_replaceBits__emit(__x, __bits, __ret, __i, _$x, _$off, _$len, _$bits...) ({ \
    let __x = _$x; \
    let __bits = _$bits; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    claim_assert(simd_V_len(__x) == simd_V_len(__bits)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = int_replaceBits(*simd_V_at((__x)[__i]), _$off, _$len, *simd_V_at((__bits)[__i])); \
    } \
    __ret; \
})

#define __op__simd_V_flt_ordApx( \
    __lhs, __rhs, __threshold, __mode, __lane_ord, __ret, __i, \
    _$lhs, _$rhs, _$threshold, _$mode... \
) ({ \
    let __lhs = _$lhs; \
    let __rhs = _$rhs; \
    let __threshold = _$threshold; \
    let_(__mode, cmp_ApxMode) = _$mode; \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__threshold)); \
    var_(__ret, cmp_Ord) = cmp_Ord_eq; \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        let_(__lane_ord, cmp_Ord) = flt_ordApx( \
            *simd_V_at((__lhs)[__i]), \
            *simd_V_at((__rhs)[__i]), \
            *simd_V_at((__threshold)[__i]), \
            __mode \
        ); \
        if (__lane_ord != cmp_Ord_eq) { \
            __ret = __lane_ord; \
            break; \
        } \
    } \
    __ret; \
})

#define __op__simd_V_flt_frexp(_$x, _$p_exp...) \
    __op__simd_V_flt_frexp__emit(pp_uniqTok(x), pp_uniqTok(exp), pp_uniqTok(ret), pp_uniqTok(i), _$x, _$p_exp)
#define __op__simd_V_flt_frexp__emit(__x, __exp, __ret, __i, _$x, _$p_exp...) ({ \
    let __x = _$x; \
    let_(__exp, TypeOf(_$p_exp)) = _$p_exp; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    claim_assert(simd_V_len(__x) == simd_V_len(*__exp)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = flt_frexp(*simd_V_at((__x)[__i]), simd_V_at((*__exp)[__i])); \
    } \
    __ret; \
})
#define __op__simd_V_flt_modf(_$x, _$p_int...) \
    __op__simd_V_flt_modf__emit(pp_uniqTok(x), pp_uniqTok(int_part), pp_uniqTok(ret), pp_uniqTok(i), _$x, _$p_int)
#define __op__simd_V_flt_modf__emit(__x, __int_part, __ret, __i, _$x, _$p_int...) ({ \
    let __x = _$x; \
    let_(__int_part, TypeOf(_$p_int)) = _$p_int; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    claim_assert(simd_V_len(__x) == simd_V_len(*__int_part)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = flt_modf(*simd_V_at((__x)[__i]), simd_V_at((*__int_part)[__i])); \
    } \
    __ret; \
})
#define __op__simd_V_flt_remquo(_$x, _$y, _$p_quo...) \
    __op__simd_V_flt_remquo__emit(pp_uniqTok(x), pp_uniqTok(y), pp_uniqTok(quo), pp_uniqTok(ret), pp_uniqTok(i), _$x, _$y, _$p_quo)
#define __op__simd_V_flt_remquo__emit(__x, __y, __quo, __ret, __i, _$x, _$y, _$p_quo...) ({ \
    let __x = _$x; \
    let __y = _$y; \
    let_(__quo, TypeOf(_$p_quo)) = _$p_quo; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    claim_assert(simd_V_len(__x) == simd_V_len(__y)); \
    claim_assert(simd_V_len(__x) == simd_V_len(*__quo)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = flt_remquo(*simd_V_at((__x)[__i]), *simd_V_at((__y)[__i]), simd_V_at((*__quo)[__i])); \
    } \
    __ret; \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* simd_V__included */

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

#define $simd_V$(_n /*: usize*/, _T...) __type__$simd_V$(_n, _T)
#define simd_V$$(_n /*: usize*/, _T...) __type__simd_V$$(_n, _T)
#define simd_V$(_n /*: usize*/, _T...) __alias__simd_V$(_n, _T)
#define T_decl_simd_V$(_n /*: usize*/, _T...) __stmt__T_decl_simd_V$(_n, _T)
#define T_impl_simd_V$(_n /*: usize*/, _T...) __stmt__T_impl_simd_V$(_n, _T)
#define T_use_simd_V$(_n /*: usize*/, _T...) __stmt__T_use_simd_V$(_n, _T)

#define simd_V_n$(_T /*: simd_V(n: usize, T)*/... /*(usize)*/) __val__simd_V_n$(_T)
#define simd_V_T$(_T /*: simd_V(n: usize, T)*/... /*(T)*/) __type__simd_V_T$(_T)
#define simd_V_TUnqual$(_T /*: simd_V(n: usize, T)*/... /*(Unqual(T))*/) __type__simd_V_TUnqual$(_T)
#define simd_V_isBool$(_T /*: simd_V(n: usize, T)*/... /*(bool)*/) isBool$(simd_V_TUnqual$(_T))
#define simd_V_isUInt$(_T /*: simd_V(n: usize, T)*/... /*(bool)*/) isUInt$(simd_V_TUnqual$(_T))
#define simd_V_isIInt$(_T /*: simd_V(n: usize, T)*/... /*(bool)*/) isIInt$(simd_V_TUnqual$(_T))
#define simd_V_isInt$(_T /*: simd_V(n: usize, T)*/... /*(bool)*/) isInt$(simd_V_TUnqual$(_T))
#define simd_V_isFlt$(_T /*: simd_V(n: usize, T)*/... /*(bool)*/) isFlt$(simd_V_TUnqual$(_T))

/*========== Vector Construction ============================================*/

#define simd_V_zero$(/*(_T: simd_V(n: usize, T))*/... /*(_T)*/) __op__simd_V_zero$(__VA_ARGS__)
#define simd_V_zero() __op__simd_V_zero()
#define simd_V_init$(/*(_T: simd_V(n: usize, T)){ _initial... }*/... /*(_T)*/) __op__simd_V_init$(__VA_ARGS__)
#define simd_V_init(/*{ _initial... }*/...) __op__simd_V_init(__VA_ARGS__)
#define simd_V_splat$(/*(_T: simd_V(n: usize, T))(_x: T)*/... /*(_T)*/) __op__simd_V_splat$(__VA_ARGS__)
#define simd_V_splat(_v /*: simd_V(n: usize, T)*/, _x /*: T*/... /*(simd_V(n, T))*/) __op__simd_V_splat(_v, _x)
#define simd_V_from$(/*(_T){ _initial... }*/... /*(simd_V(n: usize, _T))*/) __op__simd_V_from$(__VA_ARGS__)
#define simd_V_fromA$(/*(_T: simd_V(n: usize, T))(_a: A(n: usize, T))*/... /*(_T)*/) __op__simd_V_fromA$(__VA_ARGS__)
#define simd_V_fromA(_a /*: A(n: usize, T)*/... /*(simd_V(n: usize, T))*/) __op__simd_V_fromA(_a)
#define simd_V_toA$(/*(_T: A(n: usize, T))(_v: simd_V(n: usize, T))*/... /*(_T)*/) __op__simd_V_toA$(__VA_ARGS__)
#define simd_V_toA(_v /*: simd_V(n: usize, T)*/... /*(A(n: usize, T))*/) __op__simd_V_toA(_v)

#define simd_V_cat(_lhs, _rhs...) __op__simd_V_cat(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), pp_uniqTok(i), _lhs, _rhs)
#define simd_V_repeat$(/*(_T: simd_V(n: usize, T))(_v: simd_V(m: usize, T))*/... /*(_T)*/) __op__simd_V_repeat$(__VA_ARGS__)
#define simd_V_extract$(/*(_T: simd_V(n: usize, T))(_v: simd_V(m: usize, T), _first: usize)*/... /*(_T)*/) __op__simd_V_extract$(__VA_ARGS__)

#define simd_V_val(_v /*: simd_V(n: usize, T)*/... /*($simd_V$(n: usize, T))*/) __op__simd_V_val(_v)
#define simd_V_len$(_T /*: simd_V(n: usize, T)*/... /*(usize)*/) __op__simd_V_len$(_T)
#define simd_V_len(_v /*: simd_V(n: usize, T)*/... /*(usize)*/) __op__simd_V_len(_v)
#define simd_V_at(/*(_v: simd_V(n: usize, T))[_idx: usize]*/... /*(P(T))*/) __op__simd_V_at(__VA_ARGS__)

/*========== Memory Operations ==============================================*/

#define simd_V_load$(/*(_T: simd_V(n: usize, T))(_ptr: P(T))*/... /*(_T)*/) __op__simd_V_load$(__VA_ARGS__)
#define simd_V_load(_v /*: simd_V(n: usize, T)*/, _ptr /*: P(T)*/... /*(simd_V(n, T))*/) __op__simd_V_load(_v, _ptr)
#define simd_V_store(_ptr /*: P(T)*/, _v /*: simd_V(n: usize, T)*/... /*(void)*/) __op__simd_V_store(_ptr, _v)

/*========== Type Conversion ================================================*/

#define simd_V_as$(/*(_T: simd_V(n: usize, T))(_v: simd_V(n: usize, U))*/... /*(_T)*/) __op__simd_V_as$(__VA_ARGS__)
#define simd_V_boolToInt$(/*(_T: simd_V(n: usize, IntType))(_v: simd_V(n: usize, MaskType))*/... /*(_T)*/) __op__simd_V_boolToInt$(__VA_ARGS__)
#define simd_V_intFromBool$ simd_V_boolToInt$
#define simd_V_intCast$(/*(_T: simd_V(n: usize, IntType))(_v: simd_V(n: usize, IntType))*/... /*(_T)*/) __op__simd_V_intCast$(__VA_ARGS__)
#define simd_V_intToFlt$(/*(_T: simd_V(n: usize, FltType))(_v: simd_V(n: usize, IntType))*/... /*(_T)*/) __op__simd_V_intToFlt$(__VA_ARGS__)
#define simd_V_fltFromInt$ simd_V_intToFlt$
#define simd_V_fltToInt$(/*(_T: simd_V(n: usize, IntType))(_v: simd_V(n: usize, FltType))*/... /*(_T)*/) __op__simd_V_fltToInt$(__VA_ARGS__)
#define simd_V_intFromFlt$ simd_V_fltToInt$
#define simd_V_fltCast$(/*(_T: simd_V(n: usize, FltType))(_v: simd_V(n: usize, FltType))*/... /*(_T)*/) __op__simd_V_fltCast$(__VA_ARGS__)

/*========== Functional and Lane Operations ================================*/

#define simd_V_map1(_x, _op...) __op__simd_V_map1(pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane), _x, _op)
#define simd_V_map1$(/*(_T: simd_V(n: usize, T))(_x: simd_V(n: usize, U), _op)*/... /*(_T)*/) __op__simd_V_map1$(__VA_ARGS__)
#define simd_V_map2(_lhs, _rhs, _op...) __op__simd_V_map2(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lhs_lane), pp_uniqTok(rhs_lane), _lhs, _rhs, _op)
#define simd_V_map2$(/*(_T: simd_V(n: usize, T))(_lhs: simd_V(n, U), _rhs: simd_V(n, V), _op)*/... /*(_T)*/) __op__simd_V_map2$(__VA_ARGS__)
#define simd_V_map3(_x, _y, _z, _op...) __op__simd_V_map3(pp_uniqTok(x), pp_uniqTok(y), pp_uniqTok(z), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(x_lane), pp_uniqTok(y_lane), pp_uniqTok(z_lane), _x, _y, _z, _op)
#define simd_V_reduce(_x, _initial, _op...) __op__simd_V_reduce(pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane), _x, _initial, _op)

#define simd_V_select(_mask, _if_true, _if_false...) __op__simd_V_select(pp_uniqTok(mask), pp_uniqTok(if_true), pp_uniqTok(if_false), pp_uniqTok(ret), pp_uniqTok(i), _mask, _if_true, _if_false)
#define simd_V_shuffle(_lhs, _rhs, _idxs...) __op__simd_V_shuffle(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(idxs), pp_uniqTok(ret), pp_uniqTok(lhs_len), pp_uniqTok(rhs_len), pp_uniqTok(i), pp_uniqTok(idx), _lhs, _rhs, _idxs)

/*========== Arithmetic Operations ==========================================*/

#define simd_V_isZero(_x...) __op__simd_V_isZero(_x)
#define simd_V_isNonzero(_x...) __op__simd_V_isNonzero(_x)

#define simd_V_add(_lhs, _rhs...) __op__simd_V_add(_lhs, _rhs)
#define simd_V_sub(_lhs, _rhs...) __op__simd_V_sub(_lhs, _rhs)
#define simd_V_mul(_lhs, _rhs...) __op__simd_V_mul(_lhs, _rhs)
#define simd_V_mulAdd(_x, _y, _z...) __op__simd_V_mulAdd(_x, _y, _z)
#define simd_V_div(_lhs, _rhs...) __op__simd_V_div(_lhs, _rhs)
#define simd_V_rem(_x, _n...) __op__simd_V_rem(_x, _n)

#define simd_V_neg(_x...) __op__simd_V_neg(_x)
#define simd_V_abs(_x...) __op__simd_V_abs__step(_x)
#define simd_V_sgn(_x...) __op__simd_V_sgn__step(_x)

#define simd_V_not(_x...) __op__simd_V_not(_x)
#define simd_V_and(_x, _y...) __op__simd_V_and(_x, _y)
#define simd_V_or(_x, _y...) __op__simd_V_or(_x, _y)

#define simd_V_eql(_lhs, _rhs...) __op__simd_V_eql(_lhs, _rhs)
#define simd_V_neq(_lhs, _rhs...) __op__simd_V_neq(_lhs, _rhs)

#define simd_V_ord(_lhs, _rhs...) __op__simd_V_ord__step(_lhs, _rhs)
#define simd_V_eq(_lhs, _rhs...) __op__simd_V_eq(_lhs, _rhs)
#define simd_V_ne(_lhs, _rhs...) __op__simd_V_ne(_lhs, _rhs)
#define simd_V_lt(_lhs, _rhs...) __op__simd_V_lt(_lhs, _rhs)
#define simd_V_gt(_lhs, _rhs...) __op__simd_V_gt(_lhs, _rhs)
#define simd_V_le(_lhs, _rhs...) __op__simd_V_le(_lhs, _rhs)
#define simd_V_ge(_lhs, _rhs...) __op__simd_V_ge(_lhs, _rhs)

#define simd_V_min(_lhs, _rhs...) __op__simd_V_min2__step(_lhs, _rhs)
#define simd_V_min2(_lhs, _rhs...) __op__simd_V_min2__step(_lhs, _rhs)
#define simd_V_min3(_1st, _2nd, _3rd...) __op__simd_V_min3__step(_1st, _2nd, _3rd)
#define simd_V_min4(_1st, _2nd, _3rd, _4th...) __op__simd_V_min4__step(_1st, _2nd, _3rd, _4th)
#define simd_V_findMin(_vals...) __op__simd_V_findMin__step(_vals)

#define simd_V_max(_lhs, _rhs...) __op__simd_V_max2__step(_lhs, _rhs)
#define simd_V_max2(_lhs, _rhs...) __op__simd_V_max2__step(_lhs, _rhs)
#define simd_V_max3(_1st, _2nd, _3rd...) __op__simd_V_max3__step(_1st, _2nd, _3rd)
#define simd_V_max4(_1st, _2nd, _3rd, _4th...) __op__simd_V_max3__step(_1st, _2nd, _3rd, _4th)
#define simd_V_findMax(_vals...) __op__simd_V_findMax__step(_vals)

#define simd_V_clamp(_x, _lo /*$incl*/, _hi /*$incl*/...) __op__simd_V_clamp__step(_x, _lo, _hi)

/*========== Vector Arithmetic Operations ===================================*/

#define simd_V_dot(_lhs, _rhs...) __op__simd_V_dot(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define simd_V_cross3(_lhs, _rhs...) __op__simd_V_cross3(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), _lhs, _rhs)

/*========== Boolean Operations =============================================*/

#define simd_V_bool_any(_x...) __op__simd_V_bool_any(_x)
#define simd_V_bool_all(_x...) __op__simd_V_bool_all(_x)
#define simd_V_bool_none(_x...) __op__simd_V_bool_none(_x)
#define simd_V_bool_bitMask(_x...) __op__simd_V_bool_bitMask(pp_uniqTok(x), pp_uniqTok(normalized), pp_uniqTok(raw), pp_uniqTok(bit_mask), pp_uniqTok(masked), pp_uniqTok(paired), pp_uniqTok(ret), pp_uniqTok(i), _x)

/*========== Integer Classification Operations =============================*/

#define simd_V_int_isZero(_x...) simd_V_map1(_x, int_isZero)
#define simd_V_int_isNonzero(_x...) simd_V_map1(_x, int_isNonzero)
#define simd_V_int_isOdd(_x...) simd_V_map1(_x, int_isOdd)
#define simd_V_int_isEven(_x...) simd_V_map1(_x, int_isEven)
#define simd_V_int_isPow2(_x...) simd_V_map1(_x, int_isPow2)
#define simd_V_int_hasSglBit(_x...) simd_V_map1(_x, int_hasSglBit)

/*========== Integer Arithmetic Operations ==================================*/

#define simd_V_int_add(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_add)
#define simd_V_int_addOverflow(_lhs, _rhs, _p_out...) __op__simd_V_int_addOverflow(_lhs, _rhs, _p_out)
#define simd_V_int_addWrap(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_addWrap)
#define simd_V_int_addSat(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_addSat)
#define simd_V_uint_addCarry(_lhs, _rhs, _carry_in, _p_carry_out...) __op__simd_V_uint_addCarry(_lhs, _rhs, _carry_in, _p_carry_out)
#define simd_V_int_sub(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_sub)
#define simd_V_int_subOverflow(_lhs, _rhs, _p_out...) __op__simd_V_int_subOverflow(_lhs, _rhs, _p_out)
#define simd_V_int_subWrap(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_subWrap)
#define simd_V_int_subSat(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_subSat)
#define simd_V_uint_subBorrow(_lhs, _rhs, _borrow_in, _p_borrow_out...) __op__simd_V_uint_subBorrow(_lhs, _rhs, _borrow_in, _p_borrow_out)
#define simd_V_int_mul(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_mul)
#define simd_V_int_mulOverflow(_lhs, _rhs, _p_out...) __op__simd_V_int_mulOverflow(_lhs, _rhs, _p_out)
#define simd_V_int_mulWrap(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_mulWrap)
#define simd_V_int_mulSat(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_mulSat)
#define simd_V_int_mulAdd(_x, _y, _z...) simd_V_int_add(simd_V_int_mul(_x, _y), _z)
#define simd_V_int_div(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_div)
#define simd_V_int_divTrunc(_lhs, _rhs...) simd_V_int_div(_lhs, _rhs)
#define simd_V_int_divRound(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_divRound)
#define simd_V_iint_divFloor(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, iint_divFloor)
#define simd_V_iint_divEuclid(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, iint_divEuclid)
#define simd_V_int_divCeil(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_divCeil)
#define simd_V_int_rem(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_rem)
#define simd_V_int_remRound(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_remRound)
#define simd_V_iint_mod(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, iint_mod)
#define simd_V_iint_modEuclid(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, iint_modEuclid)
#define simd_V_int_modCeil(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, int_modCeil)

#define simd_V_iint_neg(_x...) simd_V_map1(_x, iint_neg)
#define simd_V_iint_negOverflow(_x, _p_out...) __op__simd_V_iint_negOverflow(_x, _p_out)
#define simd_V_iint_negWrap(_x...) simd_V_map1(_x, iint_negWrap)
#define simd_V_iint_negSat(_x...) simd_V_map1(_x, iint_negSat)
#define simd_V_iint_abs(_x...) simd_V_map1(_x, iint_abs)
#define simd_V_iint_absOverflow(_x, _p_out...) __op__simd_V_iint_absOverflow(_x, _p_out)
#define simd_V_iint_absSat(_x...) simd_V_map1(_x, iint_absSat)
#define simd_V_iint_sgn(_x...) simd_V_map1(_x, iint_sgn)
#define simd_V_iint_sgnBit(_x...) simd_V_map1(_x, iint_sgnBit)

/*========== Integer Comparison Operations ==================================*/

#define simd_V_int_eql(_lhs, _rhs...) simd_V_eql(_lhs, _rhs)
#define simd_V_int_neq(_lhs, _rhs...) simd_V_neq(_lhs, _rhs)
#define simd_V_int_ord(_lhs, _rhs...) simd_V_ord(_lhs, _rhs)
#define simd_V_int_eq(_lhs, _rhs...) simd_V_eq(_lhs, _rhs)
#define simd_V_int_ne(_lhs, _rhs...) simd_V_ne(_lhs, _rhs)
#define simd_V_int_lt(_lhs, _rhs...) simd_V_lt(_lhs, _rhs)
#define simd_V_int_gt(_lhs, _rhs...) simd_V_gt(_lhs, _rhs)
#define simd_V_int_le(_lhs, _rhs...) simd_V_le(_lhs, _rhs)
#define simd_V_int_ge(_lhs, _rhs...) simd_V_ge(_lhs, _rhs)

/*========== Integer Min/Max/Clamp Operations ===============================*/

#define simd_V_int_min(_lhs, _rhs...) simd_V_min(_lhs, _rhs)
#define simd_V_int_max(_lhs, _rhs...) simd_V_max(_lhs, _rhs)
#define simd_V_int_clamp(_x, _lo, _hi...) simd_V_clamp(_x, _lo, _hi)
#define simd_V_int_wrap(_x, _lo, _hi...) simd_V_map3(_x, _lo, _hi, int_wrap)

/*========== Bitwise Operations =============================================*/

#define simd_V_int_not(_x...) __op__simd_V_int_not(_x)
#define simd_V_int_shl(_x, _y...) __op__simd_V_int_shl(_x, _y)
#define simd_V_int_shr(_x, _y...) __op__simd_V_int_shr(_x, _y)
#define simd_V_int_and(_x, _y...) __op__simd_V_int_and(_x, _y)
#define simd_V_int_xor(_x, _y...) __op__simd_V_int_xor(_x, _y)
#define simd_V_int_or(_x, _y...) __op__simd_V_int_or(_x, _y)
#define simd_V_int_setBit(_x, _bit...) __op__simd_V_int_setBit(_x, _bit)
#define simd_V_int_resetBit(_x, _bit...) __op__simd_V_int_resetBit(_x, _bit)
#define simd_V_int_toggleBit(_x, _bit...) __op__simd_V_int_toggleBit(_x, _bit)
#define simd_V_int_maskLo$(...) __op__simd_V_uintTypedMap$(int_maskLo, __VA_ARGS__)
#define simd_V_int_maskHi$(...) __op__simd_V_uintTypedMap$(int_maskHi, __VA_ARGS__)
#define simd_V_int_mask$(...) __op__simd_V_intTypedMap2$(int_mask, __VA_ARGS__)

/*========== Bit Manipulation Operations ====================================*/

#define simd_V_int_countOnes(_x...) __op__simd_V_map1U32(_x, int_countOnes)
#define simd_V_int_countZeros(_x...) __op__simd_V_map1U32(_x, int_countZeros)
#define simd_V_iint_leadingRedundantSgnBits(_x...) __op__simd_V_map1U32(_x, iint_leadingRedundantSgnBits)
#define simd_V_int_leadingOnes(_x...) __op__simd_V_map1U32(_x, int_leadingOnes)
#define simd_V_int_leadingZeros(_x...) __op__simd_V_map1U32(_x, int_leadingZeros)
#define simd_V_int_trailingOnes(_x...) __op__simd_V_map1U32(_x, int_trailingOnes)
#define simd_V_int_trailingZeros(_x...) __op__simd_V_map1U32(_x, int_trailingZeros)
#define simd_V_int_firstSetBit(_x...) __op__simd_V_map1U32(_x, int_firstSetBit)
#define simd_V_int_lastSetBit(_x...) __op__simd_V_map1U32(_x, int_lastSetBit)
#define simd_V_int_parity(_x...) simd_V_map1(_x, int_parity)
#define simd_V_int_bitWidth(_x...) __op__simd_V_map1U32(_x, int_bitWidth)
#define simd_V_int_bitFloor(_x...) simd_V_map1(_x, int_bitFloor)
#define simd_V_int_bitCeil(_x...) simd_V_map1(_x, int_bitCeil)
#define simd_V_int_rotateLeft(_x, _n...) __op__simd_V_int_rotateLeft(_x, _n)
#define simd_V_int_rotateRight(_x, _n...) __op__simd_V_int_rotateRight(_x, _n)
#define simd_V_int_swapBytes(_x...) simd_V_map1(_x, int_swapBytes)
#define simd_V_int_reverseBits(_x...) simd_V_map1(_x, int_reverseBits)
#define simd_V_int_hasBit(_x, _bit...) __op__simd_V_int_hasBit(_x, _bit)
#define simd_V_int_extractBits(_x, _off, _len...) __op__simd_V_int_extractBits(_x, _off, _len)
#define simd_V_int_replaceBits(_x, _off, _len, _bits...) __op__simd_V_int_replaceBits(_x, _off, _len, _bits)

/*========== Integer Exponential and Logarithm Operations ===================*/

#define simd_V_uint_exp10$(...) __op__simd_V_uintTypedMap$(uint_exp10, __VA_ARGS__)
#define simd_V_uint_exp2$(...) __op__simd_V_uintTypedMap$(uint_exp2, __VA_ARGS__)
#define simd_V_uint_pow2$(...) __op__simd_V_uintTypedMap$(uint_pow2, __VA_ARGS__)
#define simd_V_uint_pow2Round$(...) __op__simd_V_uintTypedMap$(uint_pow2Round, __VA_ARGS__)
#define simd_V_uint_pow2Floor$(...) __op__simd_V_uintTypedMap$(uint_pow2Floor, __VA_ARGS__)
#define simd_V_uint_pow2Ceil$(...) __op__simd_V_uintTypedMap$(uint_pow2Ceil, __VA_ARGS__)
#define simd_V_uint_log2(_x...) __op__simd_V_map1U32(_x, uint_log2)
#define simd_V_uint_log10Round(_x...) __op__simd_V_map1U32(_x, uint_log10Round)
#define simd_V_uint_log10Floor(_x...) __op__simd_V_map1U32(_x, uint_log10Floor)
#define simd_V_uint_log10Ceil(_x...) __op__simd_V_map1U32(_x, uint_log10Ceil)

/*========== Floating-Point Classification ==================================*/

#define simd_V_flt_isNaN(_x...) simd_V_map1(_x, flt_isNaN)
#define simd_V_flt_isInf(_x...) simd_V_map1(_x, flt_isInf)
#define simd_V_flt_isFinite(_x...) simd_V_map1(_x, flt_isFinite)
#define simd_V_flt_isNormal(_x...) simd_V_map1(_x, flt_isNormal)
#define simd_V_flt_isSubnormal(_x...) simd_V_map1(_x, flt_isSubnormal)
#define simd_V_flt_isZero(_x...) simd_V_map1(_x, flt_isZero)
#define simd_V_flt_isNonzero(_x...) simd_V_map1(_x, flt_isNonzero)
#define simd_V_flt_isIntegral(_x...) simd_V_map1(_x, flt_isIntegral)

/*========== Floating-Point Arithmetic Operations ===========================*/

#define simd_V_flt_add(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_add)
#define simd_V_flt_sub(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_sub)
#define simd_V_flt_mul(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_mul)
#define simd_V_flt_mulAdd(_x, _y, _z...) simd_V_map3(_x, _y, _z, flt_mulAdd)
#define simd_V_flt_div(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_div)
#define simd_V_flt_divTrunc(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_divTrunc)
#define simd_V_flt_divRound(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_divRound)
#define simd_V_flt_divFloor(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_divFloor)
#define simd_V_flt_divEuclid(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_divEuclid)
#define simd_V_flt_divCeil(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_divCeil)
#define simd_V_flt_rem(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_rem)
#define simd_V_flt_remRound(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_remRound)
#define simd_V_flt_mod(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_mod)
#define simd_V_flt_modEuclid(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_modEuclid)
#define simd_V_flt_modCeil(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_modCeil)
#define simd_V_flt_neg(_x...) simd_V_map1(_x, flt_neg)
#define simd_V_flt_abs(_x...) simd_V_map1(_x, flt_abs)
#define simd_V_flt_sgn(_x...) simd_V_map1(_x, flt_sgn)
#define simd_V_flt_sgnBit(_x...) simd_V_map1(_x, flt_sgnBit)
#define simd_V_flt_inv(_x...) simd_V_map1(_x, flt_inv)

/*========== Floating-Point Comparison Operations ===========================*/

#define simd_V_flt_eql(_lhs, _rhs...) simd_V_eql(_lhs, _rhs)
#define simd_V_flt_neq(_lhs, _rhs...) simd_V_neq(_lhs, _rhs)
#define simd_V_flt_ord(_lhs, _rhs...) simd_V_ord(_lhs, _rhs)
#define simd_V_flt_ordApx(_lhs, _rhs, _threshold, _mode...) __op__simd_V_flt_ordApx( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(threshold), pp_uniqTok(mode), \
    pp_uniqTok(lane_ord), pp_uniqTok(ret), pp_uniqTok(i), \
    _lhs, _rhs, _threshold, _mode \
)
#define simd_V_flt_ordApxAbs(_lhs, _rhs, _threshold...) \
    simd_V_flt_ordApx(_lhs, _rhs, _threshold, cmp_ApxMode_abs)
#define simd_V_flt_ordApxRel(_lhs, _rhs, _threshold...) \
    simd_V_flt_ordApx(_lhs, _rhs, _threshold, cmp_ApxMode_rel)
#define simd_V_flt_eq(_lhs, _rhs...) simd_V_eq(_lhs, _rhs)
#define simd_V_flt_ne(_lhs, _rhs...) simd_V_ne(_lhs, _rhs)
#define simd_V_flt_lt(_lhs, _rhs...) simd_V_lt(_lhs, _rhs)
#define simd_V_flt_gt(_lhs, _rhs...) simd_V_gt(_lhs, _rhs)
#define simd_V_flt_le(_lhs, _rhs...) simd_V_le(_lhs, _rhs)
#define simd_V_flt_ge(_lhs, _rhs...) simd_V_ge(_lhs, _rhs)

/*========== Floating-Point Min/Max/Clamp Operations ========================*/

#define simd_V_flt_min(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_min)
#define simd_V_flt_max(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_max)
#define simd_V_flt_clamp(_x, _lo, _hi...) simd_V_map3(_x, _lo, _hi, flt_clamp)
#define simd_V_flt_clamp01(_x...) simd_V_map1(_x, flt_clamp01)
#define simd_V_flt_wrap(_x, _lo, _hi...) simd_V_map3(_x, _lo, _hi, flt_wrap)
#define simd_V_flt_wrap01(_x...) simd_V_map1(_x, flt_wrap01)

/*========== Floating-Point Rounding Operations =============================*/

#define simd_V_flt_trunc(_x...) simd_V_map1(_x, flt_trunc)
#define simd_V_flt_round(_x...) simd_V_map1(_x, flt_round)
#define simd_V_flt_floor(_x...) simd_V_map1(_x, flt_floor)
#define simd_V_flt_ceil(_x...) simd_V_map1(_x, flt_ceil)
#define simd_V_flt_nearbyint(_x...) simd_V_map1(_x, flt_nearbyint)
#define simd_V_flt_rint(_x...) simd_V_map1(_x, flt_rint)
#define simd_V_flt_lround(_x...) __op__simd_V_map1To(ilong, _x, flt_lround)
#define simd_V_flt_llround(_x...) __op__simd_V_map1To(i64, _x, flt_llround)
#define simd_V_flt_lrint(_x...) __op__simd_V_map1To(ilong, _x, flt_lrint)
#define simd_V_flt_llrint(_x...) __op__simd_V_map1To(i64, _x, flt_llrint)

/*========== Floating-Point Power and Root Operations =======================*/

#define simd_V_flt_sqrt(_x...) simd_V_map1(_x, flt_sqrt)
#define simd_V_flt_cbrt(_x...) simd_V_map1(_x, flt_cbrt)
#define simd_V_flt_sq(_x...) simd_V_map1(_x, flt_sq)
#define simd_V_flt_cb(_x...) simd_V_map1(_x, flt_cb)
#define simd_V_flt_hypot(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_hypot)

/*========== Floating-Point Exponential and Logarithm =======================*/

#define simd_V_flt_exp(_x...) simd_V_map1(_x, flt_exp)
#define simd_V_flt_exp2(_x...) simd_V_map1(_x, flt_exp2)
#define simd_V_flt_exp10(_x...) simd_V_map1(_x, flt_exp10)
#define simd_V_flt_powI(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_powI)
#define simd_V_flt_powF(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_powF)
#define simd_V_flt_ln(_x...) simd_V_map1(_x, flt_ln)
#define simd_V_flt_log2(_x...) simd_V_map1(_x, flt_log2)
#define simd_V_flt_log10(_x...) simd_V_map1(_x, flt_log10)
#define simd_V_flt_log1p(_x...) simd_V_map1(_x, flt_log1p)
#define simd_V_flt_expm1(_x...) simd_V_map1(_x, flt_expm1)

/*========== Floating-Point Trigonometric Functions =========================*/

#define simd_V_flt_sin(_x...) simd_V_map1(_x, flt_sin)
#define simd_V_flt_cos(_x...) simd_V_map1(_x, flt_cos)
#define simd_V_flt_tan(_x...) simd_V_map1(_x, flt_tan)
#define simd_V_flt_asin(_x...) simd_V_map1(_x, flt_asin)
#define simd_V_flt_acos(_x...) simd_V_map1(_x, flt_acos)
#define simd_V_flt_atan(_x...) simd_V_map1(_x, flt_atan)
#define simd_V_flt_atan2(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, flt_atan2)

/*========== Floating-Point Hyperbolic Functions ============================*/

#define simd_V_flt_sinh(_x...) simd_V_map1(_x, flt_sinh)
#define simd_V_flt_cosh(_x...) simd_V_map1(_x, flt_cosh)
#define simd_V_flt_tanh(_x...) simd_V_map1(_x, flt_tanh)
#define simd_V_flt_asinh(_x...) simd_V_map1(_x, flt_asinh)
#define simd_V_flt_acosh(_x...) simd_V_map1(_x, flt_acosh)
#define simd_V_flt_atanh(_x...) simd_V_map1(_x, flt_atanh)

/*========== Floating-Point IEEE 754 Operations =============================*/

#define simd_V_flt_copySgn(_x, _y...) simd_V_map2(_x, _y, flt_copySgn)
#define simd_V_flt_nextAfter(_x, _y...) simd_V_map2(_x, _y, flt_nextAfter)
#define simd_V_flt_nextUp(_x...) simd_V_map1(_x, flt_nextUp)
#define simd_V_flt_nextDown(_x...) simd_V_map1(_x, flt_nextDown)
#define simd_V_flt_scalb(_x, _n...) simd_V_map2(_x, _n, flt_scalb)
#define simd_V_flt_scalbn(_x, _n...) simd_V_map2(_x, _n, flt_scalbn)
#define simd_V_flt_scalbln(_x, _n...) simd_V_map2(_x, _n, flt_scalbln)
#define simd_V_flt_ilogb(_x...) __op__simd_V_map1To(i32, _x, flt_ilogb)
#define simd_V_flt_fdim(_x, _y...) simd_V_map2(_x, _y, flt_fdim)
#define simd_V_flt_frexp(_x, _p_exp...) __op__simd_V_flt_frexp(_x, _p_exp)
#define simd_V_flt_modf(_x, _p_int...) __op__simd_V_flt_modf(_x, _p_int)
#define simd_V_flt_remquo(_x, _y, _p_quo...) __op__simd_V_flt_remquo(_x, _y, _p_quo)
#define simd_V_flt_fract(_x...) simd_V_map1(_x, flt_fract)

/*========== Macros and Definitions =========================================*/

#define __type__$simd_V$(_n, _T...) \
    TypeOf(_T __attribute__((vector_size(_n * sizeOf$(_T)))))
#define __type__simd_V$$(_n, _T...) \
    union { \
        var_(val, $simd_V$(_n, _T)); \
        var_(as_arr, A$$(_n, _T)) $like_ref; \
    }
#define __alias__simd_V$(_n, _T...) \
    tpl$(simd_V, _n, _T)
#define __stmt__T_decl_simd_V$(_n, _T...) \
    T_decl_A$(_n, _T); \
    $maybe_unused typedef union simd_V$(_n, _T) simd_V$(_n, _T)
#define __stmt__T_impl_simd_V$(_n, _T...) \
    T_impl_A$(_n, _T); \
    union simd_V$(_n, _T) { \
        var_(val, $simd_V$(_n, _T)); \
        var_(as_arr, A$(_n, _T)) $like_ref; \
    }
#define __stmt__T_use_simd_V$(_n, _T...) \
    T_decl_simd_V$(_n, _T); \
    T_impl_simd_V$(_n, _T)

#define __val__simd_V_n$(_T...) simd_V_len$(_T)
#define __type__simd_V_T$(_T...) TypeOf(null$(_T*)->val[0])
#define __type__simd_V_TUnqual$(_T...) TypeOfUnqual(null$(_T*)->val[0])

#define __op__simd_V_zero$(_T...) l0$(_T)
#define __op__simd_V_zero() cleared()
#define __op__simd_V_init$(...) \
    __step_inline__simd_V_init$(pp_defer(__emit_inline__simd_V_init$)(__param_parse__simd_V_init$ __VA_ARGS__))
#define __step_inline__simd_V_init$(...) __VA_ARGS__
#define __param_parse__simd_V_init$(...) __VA_ARGS__,
#define __emit_inline__simd_V_init$(_T, _initial...) l$((_T)simd_V_init(_initial))
#define __op__simd_V_init(_initial...) { .val = _initial }
#define __op__simd_V_splat$(...) __inline__simd_V_splat$(__inline__simd_V_splat$__parseT __VA_ARGS__)
#define __inline__simd_V_splat$__parseT(_T...) _T,
#define __inline__simd_V_splat$(_T, _x...) ({ \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { __ret.val[__i] = as$(simd_V_TUnqual$(_T))(_x); } \
    __ret; \
})
#define __op__simd_V_splat(_v, _x...) ({ \
    let __base = _v; \
    var_(__ret, TypeOfUnqual(__base)) = simd_V_zero$((TypeOfUnqual(__base))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { __ret.val[__i] = as$(simd_V_TUnqual$(TypeOf(__base)))(_x); } \
    __ret; \
})
#define __op__simd_V_from$(...) \
    __step_inline__simd_V_from$(pp_defer(__emit_inline__simd_V_from$)(__param_parse__simd_V_from$ __VA_ARGS__))
#define __step_inline__simd_V_from$(...) __VA_ARGS__
#define __param_parse__simd_V_from$(_T...) _T,
#define __emit_inline__simd_V_from$(_T, _initial...) \
    ({ \
        let __arr = A_from$((_T)_initial); \
        typedef TypeOf(__arr) __simd_V_from__Arr; \
        var_(__ret, simd_V$$(A_n$(__simd_V_from__Arr), _T)) = {}; \
        loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { __ret.val[__i] = __arr.val[__i]; } \
        __ret; \
    })
#define __op__simd_V_fromA$(...) __inline__simd_V_fromA$(__inline__simd_V_fromA$__parseT __VA_ARGS__)
#define __inline__simd_V_fromA$__parseT(_T...) _T,
#define __inline__simd_V_fromA$(_T, _a...) l$((_T){ .as_arr = { _a } })
#define __op__simd_V_fromA(_a...) \
    l$((simd_V$$(A_n$(TypeOf(_a)), A_T$(TypeOf(_a)))){ .as_arr = { _a } })
#define __op__simd_V_toA$(...) __inline__simd_V_toA$(__inline__simd_V_toA$__parseT __VA_ARGS__)
#define __inline__simd_V_toA$__parseT(_T...) _T,
#define __inline__simd_V_toA$(_T, _v...) l$((_T){ .val = (*(_v).as_arr).val })
#define __op__simd_V_toA(_v...) (*(_v).as_arr)

#define __op__simd_V_cat(__lhs, __rhs, __ret, __i, _lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    var_(__ret, simd_V$$((simd_V_len(__lhs) + simd_V_len(__rhs)), simd_V_TUnqual$(TypeOf(__lhs)))) = simd_V_zero(); \
    claim_assert(eqlType$(simd_V_TUnqual$(TypeOf(__lhs)), simd_V_TUnqual$(TypeOf(__rhs)))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { *simd_V_at((__ret)[__i]) = *simd_V_at((__lhs)[__i]); } \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__rhs); ++__i) { *simd_V_at((__ret)[simd_V_len(__lhs) + __i]) = *simd_V_at((__rhs)[__i]); } \
    __ret; \
})
#define __op__simd_V_repeat$(...) \
    __op__simd_V_repeat$__step(__op__simd_V_repeat$__parseT __VA_ARGS__)
#define __op__simd_V_repeat$__parseT(_T...) \
    _T, __op__simd_V_repeat$__parseV
#define __op__simd_V_repeat$__parseV(_v...) \
    pp_uniqTok(v), pp_uniqTok(ret), pp_uniqTok(i), _v
#define __op__simd_V_repeat$__step(...) \
    __op__simd_V_repeat$__emit(__VA_ARGS__)
#define __op__simd_V_repeat$__emit(_T, __x, __ret, __i, _x...) ({ \
    let __x = _x; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__x) > 0); \
    claim_assert(eqlType$(simd_V_TUnqual$(_T), simd_V_TUnqual$(TypeOf(__x)))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = *simd_V_at((__x)[__i % simd_V_len(__x)]); \
    } \
    __ret; \
})
#define __op__simd_V_extract$(...) \
    __op__simd_V_extract$__step(__op__simd_V_extract$__parseT __VA_ARGS__)
#define __op__simd_V_extract$__parseT(_T...) \
    _T, __op__simd_V_extract$__parseV
#define __op__simd_V_extract$__parseV(_v, _first...) \
    pp_uniqTok(v), pp_uniqTok(first), pp_uniqTok(ret), pp_uniqTok(i), _v, _first
#define __op__simd_V_extract$__step(...) \
    __op__simd_V_extract$__emit(__VA_ARGS__)
#define __op__simd_V_extract$__emit(_T, __x, __first, __ret, __i, _x, _first...) ({ \
    let __x = _x; \
    let_(__first, usize) = _first; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(eqlType$(simd_V_TUnqual$(_T), simd_V_TUnqual$(TypeOf(__x)))); \
    claim_assert(__first + simd_V_len(__ret) <= simd_V_len(__x)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { *simd_V_at((__ret)[__i]) = *simd_V_at((__x)[__first + __i]); } \
    __ret; \
})

#define __op__simd_V_val(_v...) ((_v).val)
#define __op__simd_V_len$(_T...) simd_V_len(*null$(_T*))
#define __op__simd_V_len(_v...) A_len(*(_v).as_arr)
#define __op__simd_V_at(...) __op__simd_V_at__step(__op__simd_V_at__parse __VA_ARGS__)
#define __op__simd_V_at__parse(_v...) pp_uniqTok(v), _v, pp_uniqTok(idx),
#define __op__simd_V_at__step(...) __op__simd_V_at__emit(__VA_ARGS__)
#define __op__simd_V_at__emit(__v, _v, __idx, _idx...) ({ \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _idx)); \
    let_(__v, TypeOf(&(_v))) = &(_v); \
    claim_assert_fmt(__idx < simd_V_len(*__v), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, simd_V_len(*__v)); \
    &(*__v->as_arr).val[__idx]; \
})

#define __op__simd_V_load$(...) \
    __step_inline__simd_V_load$(pp_defer(__emit_inline__simd_V_load$)(__param_parse__simd_V_load$ __VA_ARGS__))
#define __step_inline__simd_V_load$(...) __VA_ARGS__
#define __param_parse__simd_V_load$(_T...) _T,
#define __emit_inline__simd_V_load$(_T, _ptr...) ({ \
    let __ptr = _ptr; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    typedef TypeOfUnqual(__ret.val) __raw_vec_t __attribute__((aligned(1), may_alias)); \
    __ret.val = *as$(const __raw_vec_t*)(__ptr); \
    __ret; \
})
#define __op__simd_V_load(_v, _ptr...) ({ \
    let __base = _v; \
    let __ptr = _ptr; \
    var_(__ret, TypeOfUnqual(__base)) = simd_V_zero$((TypeOfUnqual(__base))); \
    typedef TypeOfUnqual(__ret.val) __raw_vec_t __attribute__((aligned(1), may_alias)); \
    __ret.val = *as$(const __raw_vec_t*)(__ptr); \
    __ret; \
})
#define __op__simd_V_store(_ptr, _v...) ({ \
    let __v = _v; \
    let __ptr = _ptr; \
    typedef TypeOfUnqual(__v.val) __raw_vec_t __attribute__((aligned(1), may_alias)); \
    *as$(__raw_vec_t*)(__ptr) = __v.val; \
})

#define __op__simd_V_as$(...) \
    __step_inline__simd_V_as$(pp_defer(__emit_inline__simd_V_as$)(__param_parse__simd_V_as$ __VA_ARGS__))
#define __step_inline__simd_V_as$(...) __VA_ARGS__
#define __param_parse__simd_V_as$(_T...) _T,
#define __emit_inline__simd_V_as$(_T, _v...) ({ \
    let __v = _v; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(_T))(*simd_V_at((__v)[__i])); \
    } \
    __ret; \
})
#define __op__simd_V_boolToInt$(...) \
    __step_inline__simd_V_boolToInt$(pp_defer(__emit_inline__simd_V_boolToInt$)(__param_parse__simd_V_boolToInt$ __VA_ARGS__))
#define __step_inline__simd_V_boolToInt$(...) __VA_ARGS__
#define __param_parse__simd_V_boolToInt$(_T...) _T,
#define __emit_inline__simd_V_boolToInt$(_T, _v...) ({ \
    let __v = _v; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(_T))(boolToInt(bool_(*simd_V_at((__v)[__i]) != 0))); \
    } \
    __ret; \
})
#define __op__simd_V_intCast$(...) \
    __step_inline__simd_V_intCast$(pp_defer(__emit_inline__simd_V_intCast$)(__param_parse__simd_V_intCast$ __VA_ARGS__))
#define __step_inline__simd_V_intCast$(...) __VA_ARGS__
#define __param_parse__simd_V_intCast$(_T...) _T,
#define __emit_inline__simd_V_intCast$(_T, _v...) ({ \
    let __v = _v; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = intCast$((simd_V_TUnqual$(_T))(*simd_V_at((__v)[__i]))); \
    } \
    __ret; \
})
#define __op__simd_V_intToFlt$(...) \
    __step_inline__simd_V_intToFlt$(pp_defer(__emit_inline__simd_V_intToFlt$)(__param_parse__simd_V_intToFlt$ __VA_ARGS__))
#define __step_inline__simd_V_intToFlt$(...) __VA_ARGS__
#define __param_parse__simd_V_intToFlt$(_T...) _T,
#define __emit_inline__simd_V_intToFlt$(_T, _v...) ({ \
    let __v = _v; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = intToFlt$((simd_V_TUnqual$(_T))(*simd_V_at((__v)[__i]))); \
    } \
    __ret; \
})
#define __op__simd_V_fltToInt$(...) \
    __step_inline__simd_V_fltToInt$(pp_defer(__emit_inline__simd_V_fltToInt$)(__param_parse__simd_V_fltToInt$ __VA_ARGS__))
#define __step_inline__simd_V_fltToInt$(...) __VA_ARGS__
#define __param_parse__simd_V_fltToInt$(_T...) _T,
#define __emit_inline__simd_V_fltToInt$(_T, _v...) ({ \
    let __v = _v; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = fltToInt$((simd_V_TUnqual$(_T))(*simd_V_at((__v)[__i]))); \
    } \
    __ret; \
})
#define __op__simd_V_fltCast$(...) \
    __step_inline__simd_V_fltCast$(pp_defer(__emit_inline__simd_V_fltCast$)(__param_parse__simd_V_fltCast$ __VA_ARGS__))
#define __step_inline__simd_V_fltCast$(...) __VA_ARGS__
#define __param_parse__simd_V_fltCast$(_T...) _T,
#define __emit_inline__simd_V_fltCast$(_T, _v...) ({ \
    let __v = _v; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = fltCast$((simd_V_TUnqual$(_T))(*simd_V_at((__v)[__i]))); \
    } \
    __ret; \
})

#define __op__simd_V_uintTypedMap$(_op, ...) \
    __step_inline__simd_V_uintTypedMap$(pp_defer(__emit_inline__simd_V_uintTypedMap$)(_op, __param_parse__simd_V_uintTypedMap$ __VA_ARGS__))
#define __step_inline__simd_V_uintTypedMap$(...) __VA_ARGS__
#define __param_parse__simd_V_uintTypedMap$(_T...) _T, pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane),
#define __emit_inline__simd_V_uintTypedMap$(_op, _T, __x, __ret, __i, __lane, _x...) ({ \
    let __x = _x; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__x)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let_(__lane, simd_V_TUnqual$(TypeOf(__x))) = *simd_V_at((__x)[__i]); \
        *simd_V_at((__ret)[__i]) = pp_cat(_op, $)((simd_V_TUnqual$(_T))(__lane)); \
    } \
    __ret; \
})
#define __op__simd_V_intTypedMap2$(_op, ...) \
    __step_inline__simd_V_intTypedMap2$(pp_defer(__emit_inline__simd_V_intTypedMap2$)(_op, __param_parse__simd_V_intTypedMap2$ __VA_ARGS__))
#define __step_inline__simd_V_intTypedMap2$(...) __VA_ARGS__
#define __param_parse__simd_V_intTypedMap2$(_T...) _T, pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lhs_lane), pp_uniqTok(rhs_lane), __param_parse__simd_V_intTypedMap2$__operands
#define __param_parse__simd_V_intTypedMap2$__operands(_lhs, _rhs...) _lhs, _rhs
#define __emit_inline__simd_V_intTypedMap2$(_op, _T, __lhs, __rhs, __ret, __i, __lhs_lane, __rhs_lane, _lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__lhs)); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        $maybe_unused let_(__lhs_lane, simd_V_TUnqual$(TypeOf(__lhs))) = *simd_V_at((__lhs)[__i]); \
        $maybe_unused let_(__rhs_lane, simd_V_TUnqual$(TypeOf(__rhs))) = *simd_V_at((__rhs)[__i]); \
        *simd_V_at((__ret)[__i]) = pp_cat(_op, $)((simd_V_TUnqual$(_T))(__lhs_lane, __rhs_lane)); \
    } \
    __ret; \
})
#define __op__simd_V_map1U32(_x, _op...) \
    __op__simd_V_map1U32__emit(pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane), _x, _op)
#define __op__simd_V_map1U32__emit(__x, __ret, __i, __lane, _x, _op...) ({ \
    let __x = _x; \
    var_(__ret, simd_V$$((simd_V_n$(TypeOf(__x))), u32)) = simd_V_zero(); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let_(__lane, simd_V_TUnqual$(TypeOf(__x))) = *simd_V_at((__x)[__i]); \
        *simd_V_at((__ret)[__i]) = _op(__lane); \
    } \
    __ret; \
})
#define __op__simd_V_map1To(_T, _x, _op...) \
    __op__simd_V_map1To__emit(_T, pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane), _x, _op)
#define __op__simd_V_map1To__emit(_T, __x, __ret, __i, __lane, _x, _op...) ({ \
    let __x = _x; \
    var_(__ret, simd_V$$((simd_V_n$(TypeOf(__x))), _T)) = simd_V_zero(); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let_(__lane, simd_V_TUnqual$(TypeOf(__x))) = *simd_V_at((__x)[__i]); \
        *simd_V_at((__ret)[__i]) = _op(__lane); \
    } \
    __ret; \
})

#define __op__simd_V_map1(__x, __ret, __i, __lane, _x, _op...) ({ \
    let __x = _x; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let __lane = *simd_V_at((__x)[__i]); \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(TypeOf(__ret)))(_op(__lane)); \
    } \
    __ret; \
})
#define __op__simd_V_map1$(...) \
    __step_inline__simd_V_map1$(pp_defer(__emit_inline__simd_V_map1$)(__param_parse__simd_V_map1$ __VA_ARGS__))
#define __step_inline__simd_V_map1$(...) __VA_ARGS__
#define __param_parse__simd_V_map1$(_T...) _T, pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lane),
#define __emit_inline__simd_V_map1$(_T, __x, __ret, __i, __lane, _x, _op...) ({ \
    let __x = _x; \
    var_(__ret, _T) = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__x)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let __lane = *simd_V_at((__x)[__i]); \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(_T))(_op(__lane)); \
    } \
    __ret; \
})
#define __op__simd_V_map2(__lhs, __rhs, __ret, __i, __lhs_lane, __rhs_lane, _lhs, _rhs, _op...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    var __ret = simd_V_zero$((TypeOfUnqual(__lhs))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        $maybe_unused let __lhs_lane = *simd_V_at((__lhs)[__i]); \
        $maybe_unused let __rhs_lane = *simd_V_at((__rhs)[__i]); \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(TypeOf(__ret)))(_op(__lhs_lane, __rhs_lane)); \
    } \
    __ret; \
})
#define __op__simd_V_map2$(...) \
    __step_inline__simd_V_map2$(pp_defer(__emit_inline__simd_V_map2$)(__param_parse__simd_V_map2$ __VA_ARGS__))
#define __step_inline__simd_V_map2$(...) __VA_ARGS__
#define __param_parse__simd_V_map2$(_T...) _T, pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), pp_uniqTok(i), pp_uniqTok(lhs_lane), pp_uniqTok(rhs_lane),
#define __emit_inline__simd_V_map2$(_T, __lhs, __rhs, __ret, __i, __lhs_lane, __rhs_lane, _lhs, _rhs, _op...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    var __ret = simd_V_zero$((_T)); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__lhs)); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        $maybe_unused let __lhs_lane = *simd_V_at((__lhs)[__i]); \
        $maybe_unused let __rhs_lane = *simd_V_at((__rhs)[__i]); \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(_T))(_op(__lhs_lane, __rhs_lane)); \
    } \
    __ret; \
})
#define __op__simd_V_map3(__x, __y, __z, __ret, __i, __x_lane, __y_lane, __z_lane, _x, _y, _z, _op...) ({ \
    let __x = _x; \
    let __y = _y; \
    let __z = _z; \
    claim_assert(simd_V_len(__x) == simd_V_len(__y)); \
    claim_assert(simd_V_len(__x) == simd_V_len(__z)); \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        $maybe_unused let_(__x_lane, simd_V_TUnqual$(TypeOf(__x))) = *simd_V_at((__x)[__i]); \
        $maybe_unused let_(__y_lane, simd_V_TUnqual$(TypeOf(__y))) = *simd_V_at((__y)[__i]); \
        $maybe_unused let_(__z_lane, simd_V_TUnqual$(TypeOf(__z))) = *simd_V_at((__z)[__i]); \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(TypeOf(__ret)))(_op(__x_lane, __y_lane, __z_lane)); \
    } \
    __ret; \
})
#define __op__simd_V_reduce(__x, __ret, __i, __lane, _x, _initial, _op...) ({ \
    let __x = _x; \
    var __ret = _initial; \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        let __lane = *simd_V_at((__x)[__i]); \
        __ret = _op(__ret, __lane); \
    } \
    __ret; \
})

#define __op__simd_V_select(__mask, __if_true, __if_false, __ret, __i, _mask, _if_true, _if_false...) ({ \
    let __mask = _mask; \
    let __if_true = _if_true; \
    let __if_false = _if_false; \
    var __ret = simd_V_zero$((TypeOfUnqual(__if_false))); \
    claim_assert(simd_V_len(__mask) == simd_V_len(__ret)); \
    claim_assert(simd_V_len(__if_false) == simd_V_len(__if_true)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = *simd_V_at((__mask)[__i]) ? *simd_V_at((__if_true)[__i]) : *simd_V_at((__if_false)[__i]); \
    } \
    __ret; \
})
#define __op__simd_V_shuffle(__lhs, __rhs, __idxs, __ret, __lhs_len, __rhs_len, __i, __idx, _lhs, _rhs, _idxs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    let __idxs = A_from$((usize){ _idxs }); \
    var __ret = simd_V_zero$((simd_V$$((A_len(__idxs)), simd_V_TUnqual$(TypeOf(__lhs))))); \
    let __lhs_len = simd_V_len(__lhs); \
    let __rhs_len = simd_V_len(__rhs); \
    claim_assert(__lhs_len == __rhs_len); \
    __ret.val = __builtin_shufflevector(__lhs.val, __rhs.val, _idxs); \
    __ret; \
})

#define __op__simd_V_isZero(_x...) simd_V_bool_none(_x)
#define __op__simd_V_isNonzero(_x...) simd_V_bool_any(_x)

#define __op__simd_V_add(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, +)
#define __op__simd_V_sub(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, -)
#define __op__simd_V_mul(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, *)
#define __op__simd_V_mulAdd(_x, _y, _z...) ({ \
    let __x = _x; \
    let __y = _y; \
    let __z = _z; \
    l$((TypeOfUnqual(__x)){ .val = (__x.val * __y.val) + __z.val }); \
})
#define __op__simd_V_div(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, /)
#define __op__simd_V_rem(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, %)
#define __op__simd_V_native2(_lhs, _rhs, _op) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    l$((TypeOfUnqual(__lhs)){ .val = __lhs.val _op __rhs.val }); \
})

#define __op__simd_V_neg(_x...) ({ \
    let __x = _x; \
    l$((TypeOfUnqual(__x)){ .val = -__x.val }); \
})
#define __op__simd_V_abs__step(_x...) simd_V_map1(_x, pri_abs)
#define __op__simd_V_sgn__step(_x...) simd_V_map1(_x, pri_sgn)

#define __op__simd_V_not(_x...) ({ \
    let __x = _x; \
    l$((TypeOfUnqual(__x)){ .val = __x.val == simd_V_zero$((TypeOfUnqual(__x))).val }); \
})
#define __op__simd_V_and(_x, _y...) ({ \
    let __x = _x; \
    let __y = _y; \
    let __zero = simd_V_zero$((TypeOfUnqual(__x))); \
    l$((TypeOfUnqual(__x)){ .val = (__x.val != __zero.val) & (__y.val != __zero.val) }); \
})
#define __op__simd_V_or(_x, _y...) ({ \
    let __x = _x; \
    let __y = _y; \
    let __zero = simd_V_zero$((TypeOfUnqual(__x))); \
    l$((TypeOfUnqual(__x)){ .val = (__x.val != __zero.val) | (__y.val != __zero.val) }); \
})

#define __op__simd_V_eql(_lhs, _rhs...) simd_V_bool_all(simd_V_eq(_lhs, _rhs))
#define __op__simd_V_neq(_lhs, _rhs...) bool_(!simd_V_eql(_lhs, _rhs))
#define __op__simd_V_ord__step(_lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
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
#define __op__simd_V_eq(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, ==)
#define __op__simd_V_ne(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, !=)
#define __op__simd_V_lt(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, <)
#define __op__simd_V_gt(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, >)
#define __op__simd_V_le(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, <=)
#define __op__simd_V_ge(_lhs, _rhs...) __op__simd_V_native2(_lhs, _rhs, >=)

#define __op__simd_V_min2__step(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, pri_min)
#define __op__simd_V_min3__step(_1st, _2nd, _3rd...) simd_V_min(simd_V_min(_1st, _2nd), _3rd)
#define __op__simd_V_min4__step(_1st, _2nd, _3rd, _4th...) simd_V_min(simd_V_min(_1st, _2nd), simd_V_min(_3rd, _4th))
#define __op__simd_V_findMin__step(_vals...) __op__simd_V_findMin__emit(pp_uniqTok(vals), _vals)
#define __op__simd_V_findMin__emit(__vals, _vals...) ({ \
    let __vals = _vals; \
    var __ret = *simd_V_at((__vals)[0]); \
    loop_inline_(for) (usize __i = 1; __i < simd_V_len(__vals); ++__i) { \
        __ret = pri_min(__ret, *simd_V_at((__vals)[__i])); \
    } \
    __ret; \
})

#define __op__simd_V_max2__step(_lhs, _rhs...) simd_V_map2(_lhs, _rhs, pri_max)
#define __op__simd_V_max3__step(_1st, _2nd, _3rd...) simd_V_max(simd_V_max(_1st, _2nd), _3rd)
#define __op__simd_V_max4__step(_1st, _2nd, _3rd, _4th...) simd_V_max(simd_V_max(_1st, _2nd), simd_V_max(_3rd, _4th))
#define __op__simd_V_findMax__step(_vals...) __op__simd_V_findMax__emit(pp_uniqTok(vals), _vals)
#define __op__simd_V_findMax__emit(__vals, _vals...) ({ \
    let __vals = _vals; \
    var __ret = *simd_V_at((__vals)[0]); \
    loop_inline_(for) (usize __i = 1; __i < simd_V_len(__vals); ++__i) { \
        __ret = pri_max(__ret, *simd_V_at((__vals)[__i])); \
    } \
    __ret; \
})
#define __op__simd_V_clamp__step(_x, _lo, _hi...) simd_V_map3(_x, _lo, _hi, pri_clamp)

#define __op__simd_V_dot(__lhs, __rhs, _lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    simd_V_reduce(simd_V_mul(__lhs, __rhs), as$(simd_V_TUnqual$(TypeOf(__lhs)))(0), pri_add); \
})
#define __op__simd_V_cross3(__lhs, __rhs, __ret, _lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    claim_assert(simd_V_len(__lhs) >= 3 && simd_V_len(__lhs) == simd_V_len(__rhs)); \
    var_(__ret, TypeOfUnqual(__lhs)) = simd_V_zero(); \
    __ret.val[0] = __lhs.val[1] * __rhs.val[2] - __lhs.val[2] * __rhs.val[1]; \
    __ret.val[1] = __lhs.val[2] * __rhs.val[0] - __lhs.val[0] * __rhs.val[2]; \
    __ret.val[2] = __lhs.val[0] * __rhs.val[1] - __lhs.val[1] * __rhs.val[0]; \
    __ret; \
})

#define __op__simd_V_bool_or(_acc, _lane...) bool_((_acc) || ((_lane) != 0))
#define __op__simd_V_bool_and(_acc, _lane...) bool_((_acc) && ((_lane) != 0))
#define __op__simd_V_bool_any(_x...) ({ \
    simd_V_reduce(_x, false, __op__simd_V_bool_or); \
})
#define __op__simd_V_bool_all(_x...) ({ \
    simd_V_reduce(_x, true, __op__simd_V_bool_and); \
})
#define __op__simd_V_bool_none(_x...) (!simd_V_bool_any(_x))
#define __op__simd_V_bool_bitMask_fallback(__x, __ret, __i) \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        __ret |= as$(usize)(*simd_V_at((__x)[__i]) != 0) << __i; \
    }
#if simd_V_has_sse2
#define __op__simd_V_bool_bitMask(__x, __normalized, __raw, __bit_mask, __masked, __paired, __ret, __i, _x...) ({ \
    let __x = _x; \
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
#define __op__simd_V_bool_bitMask(__x, __normalized, __raw, __bit_mask, __masked, __paired, __ret, __i, _x...) ({ \
    let __x = _x; \
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
#define __op__simd_V_bool_bitMask(__x, __normalized, __raw, __bit_mask, __masked, __paired, __ret, __i, _x...) ({ \
    let __x = _x; \
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
#define __op__simd_V_bool_bitMask(__x, __normalized, __raw, __bit_mask, __masked, __paired, __ret, __i, _x...) ({ \
    let __x = _x; \
    claim_assert(simd_V_len(__x) <= sizeOf$(usize) * arch_bits_per_byte); \
    var_(__ret, usize) = 0; \
    __op__simd_V_bool_bitMask_fallback(__x, __ret, __i); \
    __ret; \
})
#endif

#define __op__simd_V_int_addOverflow(_lhs, _rhs, _p_out...) \
    __op__simd_V_int_overflow2(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), pp_uniqTok(mask), pp_uniqTok(i), pp_uniqTok(lane_out), _lhs, _rhs, _p_out, int_addOverflow)
#define __op__simd_V_int_subOverflow(_lhs, _rhs, _p_out...) \
    __op__simd_V_int_overflow2(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), pp_uniqTok(mask), pp_uniqTok(i), pp_uniqTok(lane_out), _lhs, _rhs, _p_out, int_subOverflow)
#define __op__simd_V_int_mulOverflow(_lhs, _rhs, _p_out...) \
    __op__simd_V_int_overflow2(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), pp_uniqTok(mask), pp_uniqTok(i), pp_uniqTok(lane_out), _lhs, _rhs, _p_out, int_mulOverflow)
#define __op__simd_V_int_overflow2(__lhs, __rhs, __out, __mask, __i, __lane_out, _lhs, _rhs, _p_out, _op...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    let_(__out, TypeOf(_p_out)) = _p_out; \
    var_(__mask, TypeOfUnqual(__lhs)) = simd_V_zero$((TypeOfUnqual(__lhs))); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        var_(__lane_out, simd_V_TUnqual$(TypeOf(__lhs))) = 0; \
        *simd_V_at((*__out)[__i]) = __lane_out; \
        *simd_V_at((__mask)[__i]) = as$(simd_V_TUnqual$(TypeOf(__mask)))(boolToInt(_op(*simd_V_at((__lhs)[__i]), *simd_V_at((__rhs)[__i]), simd_V_at((*__out)[__i])))); \
    } \
    __mask; \
})
#define __op__simd_V_uint_addCarry(_lhs, _rhs, _carry_in, _p_carry_out...) \
    __op__simd_V_uint_carry3(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(carry_in), pp_uniqTok(carry_out), pp_uniqTok(ret), pp_uniqTok(i), _lhs, _rhs, _carry_in, _p_carry_out, uint_addCarry)
#define __op__simd_V_uint_subBorrow(_lhs, _rhs, _borrow_in, _p_borrow_out...) \
    __op__simd_V_uint_carry3(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(carry_in), pp_uniqTok(carry_out), pp_uniqTok(ret), pp_uniqTok(i), _lhs, _rhs, _borrow_in, _p_borrow_out, uint_subBorrow)
#define __op__simd_V_uint_carry3(__lhs, __rhs, __carry_in, __p_carry_out, __ret, __i, _lhs, _rhs, _carry_in, _p_carry_out, _op...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    let __carry_in = _carry_in; \
    $maybe_unused let_(__p_carry_out, TypeOf(_p_carry_out)) = _p_carry_out; \
    var_(__ret, TypeOfUnqual(__lhs)) = simd_V_zero$((TypeOfUnqual(__lhs))); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__rhs)); \
    claim_assert(simd_V_len(__lhs) == simd_V_len(__carry_in)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__lhs); ++__i) { \
        *simd_V_at((__ret)[__i]) = _op(*simd_V_at((__lhs)[__i]), *simd_V_at((__rhs)[__i]), *simd_V_at((__carry_in)[__i]), simd_V_at((*__p_carry_out)[__i])); \
    } \
    __ret; \
})
#define __op__simd_V_iint_negOverflow(_x, _p_out...) \
    __op__simd_V_iint_overflow1(pp_uniqTok(x), pp_uniqTok(out), pp_uniqTok(mask), pp_uniqTok(i), _x, _p_out, iint_negOverflow)
#define __op__simd_V_iint_absOverflow(_x, _p_out...) \
    __op__simd_V_iint_overflow1(pp_uniqTok(x), pp_uniqTok(out), pp_uniqTok(mask), pp_uniqTok(i), _x, _p_out, iint_absOverflow)
#define __op__simd_V_iint_overflow1(__x, __out, __mask, __i, _x, _p_out, _op...) ({ \
    let __x = _x; \
    let_(__out, TypeOf(_p_out)) = _p_out; \
    var_(__mask, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__mask)[__i]) = as$(simd_V_TUnqual$(TypeOf(__mask)))(boolToInt(_op(*simd_V_at((__x)[__i]), simd_V_at((*__out)[__i])))); \
    } \
    __mask; \
})

#define __op__simd_V_int_not(_x...) ({ \
    let __x = _x; \
    l$((TypeOfUnqual(__x)){ .val = ~__x.val }); \
})
#define __op__simd_V_int_shl(_x, _y...) ({ \
    let __x = _x; \
    let __y = _y; \
    l$((TypeOfUnqual(__x)){ .val = __x.val << __y }); \
})
#define __op__simd_V_int_shr(_x, _y...) ({ \
    let __x = _x; \
    let __y = _y; \
    l$((TypeOfUnqual(__x)){ .val = __x.val >> __y }); \
})
#define __op__simd_V_int_and(_x, _y...) __op__simd_V_native2(_x, _y, &)
#define __op__simd_V_int_xor(_x, _y...) __op__simd_V_native2(_x, _y, ^)
#define __op__simd_V_int_or(_x, _y...) __op__simd_V_native2(_x, _y, |)
#define __op__simd_V_int_setBit(_x, _bit...) simd_V_int_or(_x, simd_V_splat(_x, as$(simd_V_TUnqual$(TypeOf(_x)))(1) << (_bit)))
#define __op__simd_V_int_resetBit(_x, _bit...) simd_V_int_and(_x, simd_V_splat(_x, ~(as$(simd_V_TUnqual$(TypeOf(_x)))(1) << (_bit))))
#define __op__simd_V_int_toggleBit(_x, _bit...) simd_V_int_xor(_x, simd_V_splat(_x, as$(simd_V_TUnqual$(TypeOf(_x)))(1) << (_bit)))

#define __op__simd_V_int_rotateLeft(_x, _n...) ({ \
    let __x = _x; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = int_rotateLeft(*simd_V_at((__x)[__i]), _n); \
    } \
    __ret; \
})
#define __op__simd_V_int_rotateRight(_x, _n...) ({ \
    let __x = _x; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = int_rotateRight(*simd_V_at((__x)[__i]), _n); \
    } \
    __ret; \
})

#define __op__simd_V_int_hasBit(_x, _bit...) \
    __op__simd_V_int_hasBit__emit(pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), _x, _bit)
#define __op__simd_V_int_hasBit__emit(__x, __ret, __i, _x, _bit...) ({ \
    let __x = _x; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = as$(simd_V_TUnqual$(TypeOf(__ret)))(boolToInt(int_hasBit(*simd_V_at((__x)[__i]), _bit))); \
    } \
    __ret; \
})
#define __op__simd_V_int_extractBits(_x, _off, _len...) \
    __op__simd_V_int_extractBits__emit(pp_uniqTok(x), pp_uniqTok(ret), pp_uniqTok(i), _x, _off, _len)
#define __op__simd_V_int_extractBits__emit(__x, __ret, __i, _x, _off, _len...) ({ \
    let __x = _x; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = int_extractBits(*simd_V_at((__x)[__i]), _off, _len); \
    } \
    __ret; \
})
#define __op__simd_V_int_replaceBits(_x, _off, _len, _bits...) \
    __op__simd_V_int_replaceBits__emit(pp_uniqTok(x), pp_uniqTok(bits), pp_uniqTok(ret), pp_uniqTok(i), _x, _off, _len, _bits)
#define __op__simd_V_int_replaceBits__emit(__x, __bits, __ret, __i, _x, _off, _len, _bits...) ({ \
    let __x = _x; \
    let __bits = _bits; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    claim_assert(simd_V_len(__x) == simd_V_len(__bits)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = int_replaceBits(*simd_V_at((__x)[__i]), _off, _len, *simd_V_at((__bits)[__i])); \
    } \
    __ret; \
})

#define __op__simd_V_flt_ordApx( \
    __lhs, __rhs, __threshold, __mode, __lane_ord, __ret, __i, \
    _lhs, _rhs, _threshold, _mode... \
) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    let __threshold = _threshold; \
    let_(__mode, cmp_ApxMode) = _mode; \
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

#define __op__simd_V_flt_frexp(_x, _p_exp...) \
    __op__simd_V_flt_frexp__emit(pp_uniqTok(x), pp_uniqTok(exp), pp_uniqTok(ret), pp_uniqTok(i), _x, _p_exp)
#define __op__simd_V_flt_frexp__emit(__x, __exp, __ret, __i, _x, _p_exp...) ({ \
    let __x = _x; \
    let_(__exp, TypeOf(_p_exp)) = _p_exp; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    claim_assert(simd_V_len(__x) == simd_V_len(*__exp)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = flt_frexp(*simd_V_at((__x)[__i]), simd_V_at((*__exp)[__i])); \
    } \
    __ret; \
})
#define __op__simd_V_flt_modf(_x, _p_int...) \
    __op__simd_V_flt_modf__emit(pp_uniqTok(x), pp_uniqTok(int_part), pp_uniqTok(ret), pp_uniqTok(i), _x, _p_int)
#define __op__simd_V_flt_modf__emit(__x, __int_part, __ret, __i, _x, _p_int...) ({ \
    let __x = _x; \
    let_(__int_part, TypeOf(_p_int)) = _p_int; \
    var_(__ret, TypeOfUnqual(__x)) = simd_V_zero$((TypeOfUnqual(__x))); \
    claim_assert(simd_V_len(__x) == simd_V_len(*__int_part)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__x); ++__i) { \
        *simd_V_at((__ret)[__i]) = flt_modf(*simd_V_at((__x)[__i]), simd_V_at((*__int_part)[__i])); \
    } \
    __ret; \
})
#define __op__simd_V_flt_remquo(_x, _y, _p_quo...) \
    __op__simd_V_flt_remquo__emit(pp_uniqTok(x), pp_uniqTok(y), pp_uniqTok(quo), pp_uniqTok(ret), pp_uniqTok(i), _x, _y, _p_quo)
#define __op__simd_V_flt_remquo__emit(__x, __y, __quo, __ret, __i, _x, _y, _p_quo...) ({ \
    let __x = _x; \
    let __y = _y; \
    let_(__quo, TypeOf(_p_quo)) = _p_quo; \
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

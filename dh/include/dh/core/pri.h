/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    pri.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2026-01-03 (date of last update)
 * @ingroup dasae-headers(dh)/core/pri
 * @prefix  (pri)
 *
 * @brief   Primitive types and operations
 * @details Provides comprehensive primitive operations using zero-cost abstractions:
 *          - Integer arithmetic (with overflow detection in debug mode)
 *          - Floating-point arithmetic (IEEE 754 compliant)
 *          - Bitwise operations
 *          - Comparison operations
 *          - Bit manipulation (count, rotate, reverse)
 *          - Transcendental functions (sin, cos, exp, log, etc.)
 *          - IEEE 754 classification and decomposition
 *
 *          All operations use Clang built-ins for zero runtime cost.
 *          Debug assertions check for undefined behavior (overflow, division by zero).
 */
#ifndef core_pri__included
#define core_pri__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pri/cfg.h"
#include "pri/enum.h"
#include "pri/switch.h"

/*========== Type Utilities =================================================*/

#define isValidAlign_static(_align /*: usize*/... /*(bool)*/) ____isValidAlign_static(_align)
#define isValidAlign(_align /*: usize*/... /*(bool)*/) __step__isValidAlign(_align)

#define isAligned(_addr /*: usize|PtrType*/, _align /*: usize*/... /*(bool)*/) __step__isAligned(_addr, _align)
#define isAlignedLog2(_addr /*: usize|PtrType*/, _log2_align /*: u8*/... /*(bool)*/) __step__isAlignedLog2(_addr, _log2_align)

#define alignFwd(_addr /*: usize|PtrType*/, _align /*: usize*/... /*(usize)*/) __step__alignFwd(_addr, _align)
#define alignFwdLog2_(_addr /*: usize|PtrType*/, _log2_align /*: u8*/... /*(usize)*/) __step__alignFwdLog2(_addr, _log2_align)
#define alignBwd(_addr /*: usize|PtrType*/, _align /*: usize*/... /*(usize)*/) __step__alignBwd(_addr, _align)
#define alignBwdLog2_(_addr /*: usize|PtrType*/, _log2_align /*: u8*/... /*(usize)*/) __step__alignBwdLog2(_addr, _log2_align)

#define alignToLog2_static(_align /*: usize*/... /*(u8)*/) ____alignToLog2_static(_align)
#define alignToLog2(_align /*: usize*/... /*(u8)*/) __step__alignToLog2(_align)
#define log2ToAlign_static(_log2_align /*: u8*/... /*(usize)*/) ____log2ToAlign_static(_log2_align)
#define log2ToAlign(_log2_align /*: u8*/... /*(usize)*/) __step__log2ToAlign(_log2_align)

#define alignCast(/*(_log2_align: u8)(_ptr: PtrType)*/... /*(_T)*/) __step__alignCast(__VA_ARGS__)

#define bitCast$(/*(_T)(_val)*/... /*(_T)*/) __step__bitCast$(__VA_ARGS__)
#define intToBool(_val /*: IntType*/... /*(bool)*/) __step__intToBool(_val)
#define boolToInt(_val /*: bool*/... /*(u8)*/) ____boolToInt(_val)
#define intCast$(/*(_T: IntType)(_val: IntType)*/... /*(_T)*/) __step__intCast$(__VA_ARGS__)
#define intToFlt$(/*(_T: FltType)(_val: IntType)*/... /*(_T)*/) __step__intToFlt$(__VA_ARGS__)
#define fltToInt$(/*(_T: IntType)(_val: FltType)*/... /*(_T)*/) __step__fltToInt$(__VA_ARGS__)
#define fltCast$(/*(_T: FltType)(_val: FltType)*/... /*(_T)*/) __step__fltCast$(__VA_ARGS__)

/*========== Memory Operations ==============================================*/

#define pri_memset(_p_dst, _v_src...) __op__pri_memset(_p_dst, _v_src)
#define pri_memsetS(_s_dst, _v_src...) __op__pri_memsetS(_s_dst, _v_src)
#define pri_memset0(_s_dst...) __op__pri_memset0(_s_dst)
#define pri_memset0S(_s_dst...) __op__pri_memset0S(_s_dst)
#define pri_memcpy(_p_dst, _p_src...) __op__pri_memcpy(_p_dst, _p_src)
#define pri_memcpyS(_s_dst, _s_src...) __op__pri_memcpyS(_s_dst, _s_src)
#define pri_memmove(_p_dst, _p_src...) __op__pri_memmove(_p_dst, _p_src)
#define pri_memmoveS(_s_dst, _s_src...) __op__pri_memmoveS(_s_dst, _s_src)
#define pri_memeql(_p_lhs, _p_rhs...) __op__pri_memeql(_p_lhs, _p_rhs)
#define pri_memeqlS(_s_lhs, _s_rhs...) __op__pri_memeqlS(_s_lhs, _s_rhs)
#define pri_memord(_p_lhs, _p_rhs...) __op__pri_memord(_p_lhs, _p_rhs)
#define pri_memordS(_s_lhs, _s_rhs...) __op__pri_memordS(_s_lhs, _s_rhs)
#define pri_alloca(_len_bytes...) __op__pri_alloca(_len_bytes)
#define pri_allocaAlign(_len_bytes, _log2_align...) __op__pri_allocaAlign(_len_bytes, _log2_align)
#define pri_swap(_p_lhs, _p_rhs...) __step__pri_swap(_p_lhs, _p_rhs)
#define pri_swapS(_s_lhs, _s_rhs...) __step__pri_swapS(_s_lhs, _s_rhs)

/*========== Arithmetic Operations ==========================================*/

#define isZero(_x...) __op__isZero(_x)
#define isNonzero(_x...) __op__isNonzero(_x)

#define pri_add(_lhs, _rhs...) __op__pri_add(_lhs, _rhs)
#define pri_sub(_lhs, _rhs...) __op__pri_sub(_lhs, _rhs)
#define pri_mul(_lhs, _rhs...) __op__pri_mul(_lhs, _rhs)
#define pri_mulAdd(_x, _y, _z...) __op__pri_mulAdd(_x, _y, _z)
#define pri_div(_lhs, _rhs...) __op__pri_div(_lhs, _rhs)
#define pri_rem(_x, _n...) __op__pri_rem(_x, _n)

#define pri_neg(_x...) __op__pri_neg(_x)
#define pri_abs_static(_x...) __op__pri_abs_static(_x)
#define pri_abs(_x...) __op__pri_abs__step(_x)
#define pri_sgn_static(_x...) __op__pri_sgn_static(_x)
#define pri_sgn(_x...) __op__pri_sgn__step(_x)

#define pri_not(_x...) __op__pri_not(_x)
#define pri_and(_x, _y...) __op__pri_and(_x, _y)
#define pri_or(_x, _y...) __op__pri_or(_x, _y)

#define pri_eql(_lhs, _rhs... /*(bool)*/) __op__pri_eql(_lhs, _rhs)
#define pri_neq(_lhs, _rhs... /*(bool)*/) __op__pri_neq(_lhs, _rhs)

#define pri_ord_static(_lhs, _rhs... /*(cmp_Ord)*/) __op__pri_ord_static(_lhs, _rhs)
#define pri_ord(_lhs, _rhs... /*(cmp_Ord)*/) __op__pri_ord__step(_lhs, _rhs)
#define pri_eq(_lhs, _rhs... /*(bool)*/) __op__pri_eq(_lhs, _rhs)
#define pri_ne(_lhs, _rhs... /*(bool)*/) __op__pri_ne(_lhs, _rhs)
#define pri_lt(_lhs, _rhs... /*(bool)*/) __op__pri_lt(_lhs, _rhs)
#define pri_gt(_lhs, _rhs... /*(bool)*/) __op__pri_gt(_lhs, _rhs)
#define pri_le(_lhs, _rhs... /*(bool)*/) __op__pri_le(_lhs, _rhs)
#define pri_ge(_lhs, _rhs... /*(bool)*/) __op__pri_ge(_lhs, _rhs)

#define pri_min_static(_lhs, _rhs...) __op__pri_min2_static(_lhs, _rhs)
#define pri_min(_lhs, _rhs...) __op__pri_min2__step(_lhs, _rhs)
#define pri_min2_static(_lhs, _rhs...) __op__pri_min2_static(_lhs, _rhs)
#define pri_min2(_lhs, _rhs...) __op__pri_min2__step(_lhs, _rhs)
#define pri_min3_static(_1st, _2nd, _3rd...) __op__pri_min3_static(_1st, _2nd, _3rd)
#define pri_min3(_1st, _2nd, _3rd...) __op__pri_min3__step(_1st, _2nd, _3rd)
#define pri_min4_static(_1st, _2nd, _3rd, _4th...) __op__pri_min4_static(_1st, _2nd, _3rd, _4th)
#define pri_min4(_1st, _2nd, _3rd, _4th...) __op__pri_min4__step(_1st, _2nd, _3rd, _4th)
#define pri_findMin(_vals...) __op__pri_findMin__step(_vals)

#define pri_max_static(_lhs, _rhs...) __op__pri_max2_static(_lhs, _rhs)
#define pri_max(_lhs, _rhs...) __op__pri_max2__step(_lhs, _rhs)
#define pri_max2_static(_lhs, _rhs...) __op__pri_max2_static(_lhs, _rhs)
#define pri_max2(_lhs, _rhs...) __op__pri_max2__step(_lhs, _rhs)
#define pri_max3_static(_1st, _2nd, _3rd...) __op__pri_max3_static(_1st, _2nd, _3rd)
#define pri_max3(_1st, _2nd, _3rd...) __op__pri_max3__step(_1st, _2nd, _3rd)
#define pri_max4_static(_1st, _2nd, _3rd, _4th...) __op__pri_max4_static(_1st, _2nd, _3rd, _4th)
#define pri_max4(_1st, _2nd, _3rd, _4th...) __op__pri_max3__step(_1st, _2nd, _3rd, _4th)
#define pri_findMax(_vals...) __op__pri_findMax__step(_vals)

#define pri_clamp_static(_x, _lo /*$incl*/, _hi /*$incl*/...) __op__pri_clamp_static(_x, _lo, _hi)
#define pri_clamp(_x, _lo /*$incl*/, _hi /*$incl*/...) __op__pri_clamp__step(_x, _lo, _hi)

/*========== Integer Arithmetic Operations ==================================*/

#define int_add(_lhs, _rhs...) __op__int_add__step(_lhs, _rhs)
#define int_sub(_lhs, _rhs...) __op__int_sub__step(_lhs, _rhs)
#define int_mul(_lhs, _rhs...) __op__int_mul__step(_lhs, _rhs)
#define int_mulAdd(_x, _y, _z...) __op__int_mulAdd(_x, _y, _z)
#define int_div(_x, _n...) __op__int_div__step(_x, _n)
#define int_divTrunc(_x, _n...) __op__int_div__step(_x, _n)
#define int_divRound(_x, _n...) __op__int_divRound__step(_x, _n)
#define iint_divFloor(_x, _n...) __op__iint_divFloor__step(_x, _n)
#define iint_divEuclid(_x, _n...) __op__iint_divEuclid__step(_x, _n)
#define int_divCeil(_x, _n...) __op__int_divCeil__step(_x, _n)
#define int_rem_static(_x, _n...) __op__int_rem_static(_x, _n)
#define int_rem(_x, _n...) __op__int_rem__step(_x, _n)
#define int_remRound(_x, _n...) __op__int_remRound__step(_x, _n)
#define iint_mod(_x, _n...) __op__iint_mod__step(_x, _n)
#define iint_modEuclid(_x, _n...) __op__iint_modEuclid__step(_x, _n)
#define int_modCeil(_x, _n...) __op__int_modCeil__step(_x, _n)

#define iint_neg(_x...) __op__iint_neg__step(_x)
#define iint_abs(_x...) __op__iint_abs(_x)

/*========== Integer Comparison Operations ==================================*/

#define int_eq(_lhs, _rhs...) __op__int_eq(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define int_ne(_lhs, _rhs...) __op__int_ne(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define int_lt(_lhs, _rhs...) __op__int_lt(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define int_gt(_lhs, _rhs...) __op__int_gt(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define int_le(_lhs, _rhs...) __op__int_le(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define int_ge(_lhs, _rhs...) __op__int_ge(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)

/*========== Integer Min/Max/Clamp Operations ===============================*/

#define int_min(_lhs, _rhs...) __op__int_min(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define int_max(_lhs, _rhs...) __op__int_max(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define int_clamp(_x, _lo /*$incl*/, _hi /*$incl*/...) __op__int_clamp__step(_x, _lo, _hi)
#define int_wrap(_x, _lo /*$incl*/, _hi /*$excl*/...) __op__int_wrap__step(_x, _lo, _hi)

/*========== Bitwise Operations =============================================*/

#define int_not_static(_x /*: IntType*/... /*(IntType)*/) __op__int_not_static(_x)
#define int_not(_x /*: IntType*/... /*(IntType)*/) __step__int_not(_x)
#define int_shl_static(_x /*: IntType*/, _y /*: u32*/... /*(IntType)*/) __op__int_shl_static(_x, _y)
#define int_shl(_x /*: IntType*/, _n /*: u32*/... /*(IntType)*/) __step__int_shl(_x, _n)
#define int_shr_static(_x /*: IntType*/, _y /*: u32*/... /*(IntType)*/) __op__int_shr_static(_x, _y)
#define int_shr(_x /*: IntType*/, _n /*: u32*/... /*(IntType)*/) __step__int_shr(_x, _n)
#define int_and_static(_x /*: IntType*/, _y /*: IntType*/... /*(IntType)*/) __op__int_and_static(_x, _y)
#define int_and(_lhs /*: IntType*/, _rhs /*: IntType*/... /*(IntType)*/) __step__int_and(_lhs, _rhs)
#define int_xor_static(_x /*: IntType*/, _y /*: IntType*/... /*(IntType)*/) __op__int_xor_static(_x, _y)
#define int_xor(_lhs /*: IntType*/, _rhs /*: IntType*/... /*(IntType)*/) __step__int_xor(_lhs, _rhs)
#define int_or_static(_x /*: IntType*/, _y /*: IntType*/... /*(IntType)*/) __op__int_or_static(_x, _y)
#define int_or(_lhs /*: IntType*/, _rhs /*: IntType*/... /*(IntType)*/) __step__int_or(_lhs, _rhs)

#define int_setBit_static(_x /*: IntType*/, _bit /*: u32*/... /*(IntType)*/) __op__int_setBit_static(_x, _bit)
#define int_setBit(_x /*: IntType*/, _bit /*: u32*/... /*(IntType)*/) __op__int_setBit(_x, _bit)
#define int_resetBit_static(_x /*: IntType*/, _bit /*: u32*/... /*(IntType)*/) __op__int_resetBit_static(_x, _bit)
#define int_resetBit(_x /*: IntType*/, _bit /*: u32*/... /*(IntType)*/) __op__int_resetBit(_x, _bit)
#define int_toggleBit_static(_x /*: IntType*/, _bit /*: u32*/... /*(IntType)*/) __op__int_toggleBit_static(_x, _bit)
#define int_toggleBit(_x /*: IntType*/, _bit /*: u32*/... /*(IntType)*/) __op__int_toggleBit(_x, _bit)

/// Lower N bits `[0,N)`: (1 << n) - 1
/// Usage: bits_maskLo((u16)(4)) -> 0x000F
#define int_maskLo_static$(/*(_T: IntType)(_n)*/... /*(_T)*/) __step__int_maskLo_static$(__VA_ARGS__)
#define int_maskLo$(/*(_T: IntType)(_n)*/... /*(_T)*/) __step__int_maskLo$(__VA_ARGS__)
/// Upper N bits `[Width-N,Width)`: (~0) << (width - n)
/// Usage: bits_maskHi((u16)(4)) -> 0xF000
#define int_maskHi_static$(/*(_T: IntType)(_n)*/... /*(_T)*/) __step__int_maskHi_static$(__VA_ARGS__)
#define int_maskHi$(/*(_T: IntType)(_n)*/... /*(_T)*/) __step__int_maskHi$(__VA_ARGS__)
/// Arbitrary range [Offset,Offset+Length): maskLo(len) << off
/// Usage: bits_mask((u16)(4, 4)) -> 0x00F0 (offset 4, length 4)
#define int_mask_static$(/*(_T: IntType)(_off, _len)*/... /*(_T)*/) __step__int_mask_static$(__VA_ARGS__)
#define int_mask$(/*(_T: IntType)(_off, _len)*/... /*(_T)*/) __step__int_mask$(__VA_ARGS__)

/*========== Bit Manipulation Operations ====================================*/

#define int_countOnes_static(_x... /*(u32)*/) ____int_countOnes_static(_x)
#define int_countOnes(_x... /*(u32)*/) __step__int_countOnes(_x)
#define int_countZeros_static(_x... /*(u32)*/) ____int_countZeros_static(_x)
#define int_countZeros(_x... /*(u32)*/) __step__int_countZeros(_x)
#define int_leadingOnes_static(_x... /*(u32)*/) ____int_leadingOnes_static(_x)
#define int_leadingOnes(_x... /*(u32)*/) __step__int_leadingOnes(_x)
#define int_leadingZeros_static(_x... /*(u32)*/) ____int_leadingZeros_static(_x)
#define int_leadingZeros(_x... /*(u32)*/) __step__int_leadingZeros(_x)
#define int_trailingOnes_static(_x... /*(u32)*/) ____int_trailingOnes_static(_x)
#define int_trailingOnes(_x... /*(u32)*/) __step__int_trailingOnes(_x)
#define int_trailingZeros_static(_x... /*(u32)*/) ____int_trailingZeros_static(_x)
#define int_trailingZeros(_x... /*(u32)*/) __step__int_trailingZeros(_x)
#define int_rotateLeft_static(_x /*: IntType*/, _y /*: IntType*/... /*(IntType)*/) ____int_rotateLeft_static(_x, _y)
#define int_rotateLeft(_x /*: IntType*/, _y /*: IntType*/... /*(IntType)*/) __step__int_rotateLeft(_x, _y)
#define int_rotateRight_static(_x /*: IntType*/, _y /*: IntType*/... /*(IntType)*/) ____int_rotateRight_static(_x, _y)
#define int_rotateRight(_x /*: IntType*/, _y /*: IntType*/... /*(IntType)*/) __step__int_rotateRight(_x, _y)
#define int_swapBytes_static(_x /*: IntType*/... /*(IntType)*/) ____int_swapBytes_static(_x)
#define int_swapBytes(_x /*: IntType*/... /*(IntType)*/) __step__int_swapBytes(_x)
#define int_reverseBits_static(_x /*: IntType*/... /*(IntType)*/) ____int_reverseBits_static(_x)
#define int_reverseBits(_x /*: IntType*/... /*(IntType)*/) __step__int_reverseBits(_x)

/*========== Integer Exponential and Logarithm Operations ===================*/

#define uint_exp10_static$(/*(_T)(_n: UIntType)*/... /*(_T)*/) \
    /* \
     * calculate 10^(_n) \
     */ \
    __step__uint_exp10_static$(__VA_ARGS__)
#define uint_exp10$(/*(_T)(_n: UIntType)*/... /*(_T)*/) __step__uint_exp10$(__VA_ARGS__)
#define uint_exp2_static$(/*(_T)(_n: UIntType)*/... /*(_T)*/) \
    /* \
     * calculate 2^(_n) \
     */ \
    __step__uint_exp2_static$(__VA_ARGS__)
#define uint_exp2$(/*(_T)(_n: UIntType)*/... /*(_T)*/) __step__uint_exp2$(__VA_ARGS__)
#define uint_pow2_static$(/*(_T)(_n: UIntType)*/... /*(_T)*/) \
    /* \
     * calculate 2^(_n) \
     */ \
    __step__uint_pow2_static$(__VA_ARGS__)
#define uint_pow2$(/*(_T)(_n: UIntType)*/... /*(_T)*/) __step__uint_pow2$(__VA_ARGS__)
#define uint_pow2Round_static$(/*(_T)(_n: UIntType)*/... /*(_T)*/) \
    /* \
     * calculate round(2^(_n)) \
     */ \
    __step__uint_pow2Round_static$(__VA_ARGS__)
#define uint_pow2Round$(/*(_T)(_n: UIntType)*/... /*(_T)*/) __step__uint_pow2Round$(__VA_ARGS__)
#define uint_pow2Floor_static$(/*(_T)(_n: UIntType)*/... /*(_T)*/) \
    /* \
     * calculate floor(2^(_n)) \
     */ \
    __step__uint_pow2Floor_static$(__VA_ARGS__)
#define uint_pow2Floor$(/*(_T)(_n: UIntType)*/... /*(_T)*/) __step__uint_pow2Floor$(__VA_ARGS__)
#define uint_pow2Ceil_static$(/*(_T)(_n: UIntType)*/... /*(_T)*/) \
    /* \
     * calculate ceil(2^(_n)) \
     */ \
    __step__uint_pow2Ceil_static$(__VA_ARGS__)
#define uint_pow2Ceil$(/*(_T)(_n: UIntType)*/... /*(_T)*/) __step__uint_pow2Ceil$(__VA_ARGS__)
#define uint_log2_static(_n /*: UIntType*/... /*(u32)*/) \
    /* \
     * calculate log2(_n) \
     */ \
    __step__uint_log2_static(_n)
#define uint_log2(_n /*: UIntType*/... /*(u32)*/) __step__uint_log2(_n)
#define uint_log10Round_static(_n /*: UIntType*/... /*(u32)*/) \
    /* \
     * calculate round(log10(_n)) \
     * \
     * Thresholds are at 10^(k.5). \
     * sqrt(10) ~ 3.16227766 \
     */ \
    __step__uint_log10Round_static(_n)
#define uint_log10Round(_n /*: UIntType*/... /*(u32)*/) __step__uint_log10Round(_n)
#define uint_log10Floor_static(_n /*: UIntType*/... /*(u32)*/) \
    /* \
     * calculate floor(log10(_n)) \
     */ \
    __step__uint_log10Floor_static(_n)
#define uint_log10Floor(_n /*: UIntType*/... /*(u32)*/) __step__uint_log10Floor(_n)
#define uint_log10Ceil_static(_n /*: UIntType*/... /*(u32)*/) \
    /* \
     * calculate ceil(log10(_n)) \
     */ \
    __step__uint_log10Ceil_static(_n)
#define uint_log10Ceil(_n /*: UIntType*/... /*(u32)*/) __step__uint_log10Ceil(_n)

/*========== Integer Query Operations =======================================*/

#define int_isOdd(_x...) __op__int_isOdd(_x)
#define int_isEven(_x...) __op__int_isEven(_x)
#define int_isPow2(_x...) __op__int_isPow2(pp_uniqTok(x), _x)
#define int_hasSglBit(_x...) __op__int_hasSglBit(_x)

/*========== Floating-Point Arithmetic Operations ===========================*/

#define flt_add(_lhs, _rhs...) __op__flt_add__step(_lhs, _rhs)
#define flt_sub(_lhs, _rhs...) __op__flt_sub__step(_lhs, _rhs)
#define flt_mul(_lhs, _rhs...) __op__flt_mul__step(_lhs, _rhs)
#define flt_mulAdd(_x, _y, _z...) __op__flt_mulAdd__step(_x, _y, _z)
#define flt_div(_x, _n...) __step__flt_div(_x, _n)
#define flt_divTrunc(_x, _n...) __op__flt_divTrunc(_x, _n)
#define flt_divRound(_x, _n...) __op__flt_divRound(_x, _n)
#define flt_divFloor(_x, _n...) __op__flt_divFloor(_x, _n)
#define flt_divEuclid(_x, _n...) __op__flt_divEuclid__step(_x, _n)
#define flt_divCeil(_x, _n...) __op__flt_divCeil(_x, _n)
#define flt_rem(_x, _n...) __op__flt_rem__step(_x, _n)
#define flt_remRound(_x, _n...) __op__flt_remRound__step(_x, _n)
#define flt_mod(_x, _n...) __op__flt_mod__step(_x, _n)
#define flt_modEuclid(_x, _n...) __op__flt_modEuclid__step(_x, _n)
#define flt_modCeil(_x, _n...) __op__flt_modCeil__step(_x, _n)

#define flt_neg(_x...) __step__flt_neg(_x)
#define flt_abs(_x...) ____flt_abs(_x)
#define flt_inv_static(_x...) ____flt_inv_static(_x)
#define flt_inv(_x...) __step__flt_inv(_x)

/*========== Floating-Point Comparison Operations ===========================*/

#define flt_eq(_lhs, _rhs...) __op__flt_eq(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define flt_ne(_lhs, _rhs...) __op__flt_ne(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define flt_lt(_lhs, _rhs...) __op__flt_lt(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define flt_gt(_lhs, _rhs...) __op__flt_gt(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define flt_le(_lhs, _rhs...) __op__flt_le(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define flt_ge(_lhs, _rhs...) __op__flt_ge(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)

/*========== Floating-Point Min/Max/Clamp Operations ========================*/

#define flt_min(_lhs, _rhs...) __op__flt_min(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define flt_max(_lhs, _rhs...) __op__flt_max(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define flt_clamp(_x, _lo /*$incl*/, _hi /*$incl*/...) __op__flt_clamp__step(_x, _lo, _hi)
#define flt_clamp01(_x...) __op__flt_clamp01(_x)
#define flt_wrap(_x, _lo /*$incl*/, _hi /*$excl*/...) __op__flt_wrap__step(_x, _lo, _hi)
#define flt_wrap01(_x...) __op__flt_wrap01(_x)

/*========== Floating-Point Classification ==================================*/

#define flt_isNan(_x...) ____flt_isNan(_x)
#define flt_isInf(_x...) ____flt_isInf(_x)
#define flt_isFinite(_x...) ____flt_isFinite(_x)
#define flt_isNormal(_x...) ____flt_isNormal(_x)

/*========== Floating-Point Rounding Operations =============================*/

#define flt_trunc(_x...) __step__flt_trunc(_x)
#define flt_round(_x...) __step__flt_round(_x)
#define flt_floor(_x...) __step__flt_floor(_x)
#define flt_ceil(_x...) __step__flt_ceil(_x)

/*========== Floating-Point Power and Root Operations =======================*/

#define flt_sqrt(_x...) __step__flt_sqrt(_x)
#define flt_cbrt(_x...) __step__flt_cbrt(_x)
#define flt_sq(_x...) __step__flt_sq(_x)
#define flt_cb(_x...) __step__flt_cb(_x)
#define flt_hypot(_x, _y...) __step__flt_hypot(_x, _y)

/*========== Floating-Point Exponential and Logarithm =======================*/

#define flt_exp(_x...) ____flt_exp(_x)
#define flt_exp2(_x...) ____flt_exp2(_x)
#define flt_powI(_x, _y...) __step__flt_powI(_x, _y)
#define flt_powF(_x, _y...) __step__flt_powF(_x, _y)
#define flt_ln(_x...) __step__flt_ln(_x)
#define flt_log2(_x...) __step__flt_log2(_x)
#define flt_log10(_x...) __step__flt_log10(_x)

/*========== Floating-Point Trigonometric Functions =========================*/

#define flt_sin(_x...) ____flt_sin(_x)
#define flt_cos(_x...) ____flt_cos(_x)
#define flt_tan(_x...) ____flt_tan(_x)
#define flt_asin(_x...) __step__flt_asin(_x)
#define flt_acos(_x...) __step__flt_acos(_x)
#define flt_atan(_x...) ____flt_atan(_x)
#define flt_atan2(_y, _x...) __step__flt_atan2(_y, _x)

/*========== Floating-Point Hyperbolic Functions ============================*/

#define flt_sinh(_x...) ____flt_sinh(_x)
#define flt_cosh(_x...) ____flt_cosh(_x)
#define flt_tanh(_x...) ____flt_tanh(_x)
#define flt_asinh(_x...) ____flt_asinh(_x)
#define flt_acosh(_x...) __step__flt_acosh(_x)
#define flt_atanh(_x...) __step__flt_atanh(_x)

/*========== Floating-Point IEEE 754 Operations =============================*/

#define flt_copySgn(_x, _y...) __op__flt_copySgn(pp_uniqTok(x), pp_uniqTok(y), _x, _y)
#define flt_nextAfter(_x, _y...) __op__flt_nextAfter(pp_uniqTok(x), pp_uniqTok(y), _x, _y)
#define flt_scalb(_x, _n...) __op__flt_scalb(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define flt_ilogb(_x...) ____flt_ilogb(_x)

/*===========================================================================*/
/*                          IMPLEMENTATION SECTION                           */
/*===========================================================================*/

/*========== Memory Operations Implementation ===============================*/

#if UNUSED_CODE
$inline_always
    $static P$raw pri__memset(P$raw ptr, u8 val, usize len) {
    if (len == 0) { return ptr; }
    return raw_memset(ptr, val, len);
}
$inline_always
    $static P$raw pri__memcpy(P$raw dst, P_const$raw src, usize len) {
    if (len == 0 || dst == src) { return dst; }
    return raw_memcpy(dst, src, len);
}
$inline_always
    $static P$raw pri__memmove(P$raw dst, P_const$raw src, usize len) {
    if (len == 0 || dst == src) { return dst; }
    return raw_memmove(dst, src, len);
}
$inline_always
    $static u8 pri__memcmp(P_const$raw lhs, P_const$raw rhs, usize len) {
    if (len == 0 || lhs == rhs) { return 0; }
    return raw_memcmp(lhs, rhs, len);
}
#endif /* UNUSED_CODE */

#define __op__pri_memset(_p_dst, _v_src...) u_castP$((TypeOf(_p_dst))(u_memset(u_anyP(_p_dst), u_anyP(from$((TypeOf(*(_p_dst)))_v_src)))))
#define __op__pri_memsetS(_s_dst, _v_src...) u_castS$((TypeOf(_s_dst))(u_memsetS(u_anyS(_s_dst), u_anyV(from$((TypeOf(*(_s_dst).ptr))_v_src)))))
#define __op__pri_memcpy(_p_dst, _p_src...) u_castP$((TypeOf(_p_dst))(u_memcpy(u_anyP(_p_dst), u_anyP(from$((TypeOf(*(_p_dst)))_p_src)))))
#define __op__pri_memcpyS(_s_dst, _s_src...) u_castS$((TypeOf(_s_dst))(u_memcpyS(u_anyS(_s_dst), u_anyS(from$((TypeOf(_s_dst.as_const))_s_src)))))
#define __op__pri_memmove(_p_dst, _p_src...) u_castP$((TypeOf(_p_dst))(u_memmove(u_anyP(_p_dst), u_anyP(from$((TypeOf(*(_p_dst)))_p_src)))))
#define __op__pri_memmoveS(_s_dst, _s_src...) u_castS$((TypeOf(_s_dst))(u_memmoveS(u_anyS(_s_dst), u_anyS(from$((TypeOf(_s_dst.as_const))_s_src)))))
#define __op__pri_memeql(_p_lhs, _p_rhs...) u_castP$((TypeOf(_p_lhs))(u_memeql(u_anyP(_p_lhs), u_anyP(from$((TypeOf(_p_lhs))_p_rhs)))))
#define __op__pri_memeqlS(_s_lhs, _s_rhs...) u_castS$((TypeOf(_s_lhs))(u_memeqlS(u_anyS(_s_lhs), u_anyS(from$((TypeOf(_s_lhs.as_const))_s_rhs)))))
#define __op__pri_memord(_p_lhs, _p_rhs...) u_castP$((TypeOf(_p_lhs))(u_memord(u_anyP(_p_lhs), u_anyP(from$((TypeOf(_p_lhs))_p_rhs)))))
#define __op__pri_memordS(_s_lhs, _s_rhs...) u_castS$((TypeOf(_s_lhs))(u_memordS(u_anyS(_s_lhs), u_anyS(from$((TypeOf(_s_lhs.as_const))_s_rhs)))))
#define __op__pri_alloca(_len_bytes...) raw_alloca(_len_bytes)
#define __op__pri_allocaAlign(_len_bytes, _log2_align...) raw_allocaAlign(_len_bytes, _log2_align)
#define __step__pri_swap(_p_lhs, _p_rhs...) ____pri_swap( \
    pp_uniqTok(tmp), pp_uniqTok(p_lhs), _p_lhs, pp_uniqTok(p_rhs), _p_rhs \
)
#define ____pri_swap(__tmp, __p_lhs, _p_lhs, __p_rhs, _p_rhs...) ({ \
    let __p_lhs = _p_lhs; \
    let __p_rhs = _p_rhs; \
    let __tmp = *__p_lhs; \
    *__p_lhs = *__p_rhs; \
    *__p_rhs = __tmp; \
    {}; \
})

#define __op__pri_add(_lhs, _rhs...) ((_lhs) + (_rhs))
#define __op__pri_sub(_lhs, _rhs...) ((_lhs) - (_rhs))
#define __op__pri_mul(_lhs, _rhs...) ((_lhs) * (_rhs))
#define __op__pri_mulAdd(_x, _y, _z...) ((__x) * (__y) + (__z))
#define __op__pri_div(_lhs, _rhs...) ((_lhs) / (_rhs))
#define __op__pri_rem(_x, _n...) ((_x) % (_n))

#define __op__int_divRound__step(_x, _n...) __op__int_divRound( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(q), pp_uniqTok(r), pp_uniqTok(half), _x, _n \
)
#define __op__int_divRound(__x, __n, __q, __r, __half, _x, _n...) ({ \
    let __x = _x; \
    let __n = _n; \
    let __q = __x / __n; \
    let __r = __x % __n; \
    let __half = __n / 2; \
    (__r > __half || (__r == __half && (__q & 1))) \
        ? (__n > 0 ? __q + 1 : __q - 1) \
    : (__r < -__half || (__r == -__half && (__q & 1))) \
        ? (__n > 0 ? __q - 1 : __q + 1) \
        : __q; \
})
#define __op__iint_divFloor__step(_x, _n...) __op__iint_divFloor( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(q), pp_uniqTok(r), pp_uniqTok(has_r), pp_uniqTok(diff_sgn), _x, _n \
)
#define __op__iint_divFloor(__x, __n, __q, __r, __has_r, __diff_sgn, _x, _n...) ({ \
    let __x = _x; \
    let __n = _n; \
    let __q = int_div(__x, __n); \
    let __r = int_rem_static(__x, __n); \
    let __has_r = __r != 0; \
    let __diff_sgn = __r ^ __n < 0; \
    __has_r&& __diff_sgn ? __q - 1 : __q; \
})
#define __op__iint_divEuclid__step(_x, _n...) __op__iint_divEuclid( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(q), pp_uniqTok(r), _x, _n \
)
#define __op__iint_divEuclid(__x, __n, __q, __r, _x, _n...) ({ \
    let __x = _x; \
    let __n = _n; \
    let __q = int_div(__x, __n); \
    let __r = int_rem_static(__x, __n); \
    __r < 0 ? (__n > 0 ? __q - 1 : __q + 1) : __q; \
})
#define __op__int_divCeil__step(_x, _n...) __op__int_divCeil( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(q), pp_uniqTok(r), pp_uniqTok(has_r), pp_uniqTok(same_sgn), _x, _n \
)
#define __op__int_divCeil(__x, __n, __q, __r, __has_r, __same_sgn, _x, _n...) ({ \
    let __x = _x; \
    let __n = _n; \
    let __q = int_div(__x, __n); \
    let __r = int_rem_static(__x, __n); \
    let __has_r = __r != 0; \
    let __same_sgn = __r ^ __n >= 0; \
    __has_r&& __same_sgn ? __q + 1 : __q; \
})

#define __op__flt_divTrunc(_x, _n...) flt_trunc(flt_div(_x, _n))
#define __op__flt_divRound(_x, _n...) flt_round(flt_div(_x, _n))
#define __op__flt_divFloor(_x, _n...) flt_floor(flt_div(_x, _n))
#define __op__flt_divEuclid__step(_x, _n...) __op__flt_divEuclid(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__flt_divEuclid(__x, __n, _x, _n...) ({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__n, FltType) = _n; \
    let_(__q, FltType) = flt_divTrunc(__x, __n); \
    let_(__r, FltType) = __x - __q * __n; \
    __r < 0.0 ? (__n > 0.0 ? __q - 1.0 : __q + 1.0) : __q; \
})
#define __op__flt_divCeil(_x, _n...) flt_ceil(flt_div(__x, __n))

#define __op__int_rem_static(_x, _n...) ((_x) % (_n))
#define __op__int_rem__step(_x, _n...) __op__int_rem(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__int_rem(__x, __n, _x, _n...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__n, IntType) = _n; \
    claim_assert(__n != 0); \
    claim_assert(isIInt$(IntType) ? !(__x == int_limit_min$(IntType) && __n == as$(IntType)(-1)) : true); \
    as$(IntType)(__x % __n); \
})
#define __op__int_remRound__step(_x, _n...) __op__int_remRound(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__int_remRound(__x, __n, _x, _n...) ({ \
    let __x = _x; \
    let __n = _n; \
    __x - int_divRound(__x, __n) * __n; \
})
#define __op__iint_mod__step(_x, _n...) __op__iint_mod( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(r), pp_uniqTok(has_r), pp_uniqTok(diff_sgn), _x, _n \
)
#define __op__iint_mod(__x, __n, __r, __has_r, __diff_sgn, _x, _n...) ({ \
    let __x = _x; \
    let __n = _n; \
    let __r = int_rem(__x, __n); \
    let __has_r = __r != 0; \
    let __diff_sgn = (__r ^ __n) < 0; \
    __has_r&& __diff_sgn ? __r + __n : __r; \
})
#define __op__int_modEuclid__step(_x, _n...) __op__int_modEuclid( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(r), _x, _n \
)
#define __op__int_modEuclid(__x, __n, __r, _x, _n...) ({ \
    let __x = _x; \
    let __n = _n; \
    let __r = int_rem(__x, __n); \
    __r < 0 ? __r + pri_abs(__n) : __r; \
})
#define __op__int_modCeil__step(_x, _n...) __op__int_modCeil( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(r), pp_uniqTok(has_r), pp_uniqTok(same_sgn), _x, _n \
)
#define __op__int_modCeil(__x, __n, __r, __has_r, __same_sgn, _x, _n...) ({ \
    let __x = _x; \
    let __n = _n; \
    let __r = int_rem(__x, __n); \
    let __has_r = __r != 0; \
    let __same_sgn = __r ^ __n >= 0; \
    __has_r&& __same_sgn ? __r - __n : __r; \
})

#define __op__flt_rem__step(_x, _n...) __op__flt_rem(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__flt_rem(__x, __n, _x, _n...) ({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__n, FltType) = _n; \
    claim_assert(__n != 0.0); \
    T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_fmodf(as$(f32)(__x), as$(f32)(__n)))), \
        T_case$((f64)(__builtin_fmod(as$(f64)(__x), as$(f64)(__n)))) \
    )); \
})
#define __op__flt_remRound__step(_x, _n...) __op__flt_remRound(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__flt_remRound(__x, __n, _x, _n...) ({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__n, FltType) = _n; \
    claim_assert(__n != 0.0); \
    T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_remainderf(as$(f32)(__x), as$(f32)(__n)))), \
        T_case$((f64)(__builtin_remainder(as$(f64)(__x), as$(f64)(__n)))) \
    )); \
})
#define __op__flt_mod__step(_x, _n...) ____flt_mod(pp_uniqTok(x), _x, pp_uniqTok(n), _n)
#define ____flt_mod(__x, _x, __n, _n...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    let_(__n, FltType) = _n; \
    __x - flt_divFloor(__x, __n) * __n; \
})
#define __op__flt_modEuclid__step(_x, _n...) __op__flt_modEuclid( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(r), _x, _n \
)
#define __op__flt_modEuclid(__x, __n, __r, _x, _n...) ({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__n, FltType) = _n; \
    let_(__r, FltType) = flt_rem(__x, __n); \
    __r < 0.0 ? __r + flt_abs(__n) : __r; \
})
#define __op__flt_modCeil__step(_x, _n...) __op__flt_modCeil(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__flt_modCeil(__x, __n, _x, _n...) ({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__n, FltType) = _n; \
    __x - flt_divCeil(__x, __n) * __n; \
})

#define __op__pri_neg(_x...) (as$(TypeOf(_x))(-(_x)))
#define __op__pri_abs_static(_x...) (as$(TypeOf(_x))(((_x) < 0) ? -(_x) : (_x)))
#define __op__pri_abs__step(_x...) __op__pri_abs(pp_uniqTok(x), _x)
#define __op__pri_abs(__x, _x...) ({ \
    let __x = _x; \
    pri_abs_static(__x); \
})
#define __op__pri_sgn_static(_x...) (as$(cmp_Sgn)( \
    ((_x) < 0)   ? cmp_Sgn_ngtv \
    : (0 < (_x)) ? cmp_Sgn_pstv \
                 : cmp_Sgn_zero \
))
#define __op__pri_sgn__step(_x...) __op__pri_sgn(pp_uniqTok(x), _x)
#define __op__pri_sgn(__x, _x...) local_({ \
    let __x = _x; \
    local_return_(pri_sgn_static(__x)); \
})

#define __op__isZero(_x...) pri_eql(_x, 0)
#define __op__isNonzero(_x...) pri_neq(_x, 0)

#define __op__pri_not(_x...) bool_(!(_x))
#define __op__pri_and(_x, _y...) bool_((_x) && (_y))
#define __op__pri_or(_x, _y...) bool_((_x) || (_y))

#define __op__pri_eql(_lhs, _rhs...) bool_((_lhs) == (_rhs))
#define __op__pri_neq(_lhs, _rhs...) bool_((_lhs) != (_rhs))
#define __op__pri_ord_static(_lhs, _rhs...) (as$(cmp_Ord)( \
    ((_lhs) < (_rhs))   ? cmp_Ord_lt \
    : ((_lhs) > (_rhs)) ? cmp_Ord_gt \
                        : cmp_Ord_eq \
))
#define __op__pri_ord__step(_lhs, _rhs...) __op__pri_ord(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__pri_ord(__lhs, __rhs, _lhs, _rhs...) local_({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    local_return_(pri_ord_static(__lhs, __rhs)); \
})
#define __op__pri_eq(_lhs, _rhs...) bool_((_lhs) == (_rhs))
#define __op__pri_ne(_lhs, _rhs...) bool_((_lhs) != (_rhs))
#define __op__pri_lt(_lhs, _rhs...) bool_((_lhs) < (_rhs))
#define __op__pri_gt(_lhs, _rhs...) bool_((_lhs) > (_rhs))
#define __op__pri_le(_lhs, _rhs...) bool_((_lhs) <= (_rhs))
#define __op__pri_ge(_lhs, _rhs...) bool_((_lhs) >= (_rhs))

#define __op__pri_min2_static(_lhs, _rhs...) (as$(TypeOf(_lhs))((_rhs) < (_lhs) ? (_rhs) : (_lhs)))
#define __op__pri_min2__step(_lhs, _rhs...) __op__pri_min2( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs \
)
#define __op__pri_min2(__lhs, __rhs, _lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    pri_min2_static(__lhs, __rhs); \
})
#define __op__pri_min3_static(_1st, _2nd, _3rd...) \
    pri_min2_static(pri_min2_static(_1st, _2nd), _3rd)
#define __op__pri_min3__step(_1st, _2nd, _3rd...) __op__pri_min3( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), _1st, _2nd, _3rd \
)
#define __op__pri_min3(__1st, __2nd, __3rd, _1st, _2nd, _3rd...) ({ \
    let __1st = _1st; \
    let __2nd = _2nd; \
    let __3rd = _3rd; \
    pri_min3_static(__1st, __2nd, __3rd); \
})
#define __op__pri_min4_static(_1st, _2nd, _3rd, _4th...) \
    pri_min2_static(pri_min3_static(_1st, _2nd, _3rd), _4th)
#define __op__pri_min4__step(_1st, _2nd, _3rd, _4th...) __op__pri_min4( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), pp_uniqTok(4th), _1st, _2nd, _3rd, _4th \
)
#define __op__pri_min4(__1st, __2nd, __3rd, __4th, _1st, _2nd, _3rd, _4th...) ({ \
    let __1st = _1st; \
    let __2nd = _2nd; \
    let __3rd = _3rd; \
    let __4th = _4th; \
    pri_min4_static(__1st, __2nd, __3rd, __4th); \
})
#define __op__pri_findMin__step(__best, __vals...) __op__pri_findMin( \
    pp_uniqTok(best), pp_uniqTok(vals), pp_uniqTok(val), __best, __vals \
)
#define __op__pri_findMin(__best, __vals, __val, _best, _vals...) ({ \
    var __best = _best; \
    let __vals = A_from$((TypeOf(__best)){ _vals }); \
    for_(($s(A_ref(__vals)))(__val)) { \
        __best = pri_min2_static(__best, *__val); \
    } $end(for); \
    __best; \
})
#define __op__pri_max2_static(_lhs, _rhs...) (as$(TypeOf(_lhs))((_rhs) > (_lhs) ? (_rhs) : (_lhs)))
#define __op__pri_max2__step(_lhs, _rhs...) __op__pri_max2( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs \
)
#define __op__pri_max2(__lhs, __rhs, _lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    pri_max2_static(__lhs, __rhs); \
})
#define __op__pri_max3_static(_1st, _2nd, _3rd...) pri_max2_static(pri_max2_static(_1st, _2nd), _3rd)
#define __op__pri_max3__step(_1st, _2nd, _3rd...) __op__pri_max3( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), _1st, _2nd, _3rd \
)
#define __op__pri_max3(__1st, __2nd, __3rd, _1st, _2nd, _3rd...) ({ \
    let __1st = _1st; \
    let __2nd = _2nd; \
    let __3rd = _3rd; \
    pri_max3_static(__1st, __2nd, __3rd); \
})
#define __op__pri_max4_static(_1st, _2nd, _3rd, _4th...) pri_max2_static(pri_max3_static(_1st, _2nd, _3rd), _4th)
#define __op__pri_max4__step(_1st, _2nd, _3rd, _4th...) __op__pri_max4( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), pp_uniqTok(4th), _1st, _2nd, _3rd, _4th \
)
#define __op__pri_max4(__1st, __2nd, __3rd, __4th, _1st, _2nd, _3rd, _4th...) ({ \
    let __1st = _1st; \
    let __2nd = _2nd; \
    let __3rd = _3rd; \
    let __4th = _4th; \
    pri_max4_static(__1st, __2nd, __3rd, __4th); \
})
#define __op__pri_findMax__step(__best, __vals...) __op__pri_findMax( \
    pp_uniqTok(best), pp_uniqTok(vals), pp_uniqTok(val), __best, __vals \
)
#define __op__pri_findMax(__best, __vals, __val, _best, _vals...) ({ \
    var __best = _best; \
    let __vals = A_from$((TypeOf(__best)){ _vals }); \
    for_(($s(A_ref(__vals)))(__val)) { \
        __best = pri_max2_static(__best, *__val); \
    } $end(for); \
    __best; \
})

#define __op__pri_clamp_static(_x, _lo, _hi...) pri_min2_static(pri_max2_static(_lo, _x), _hi)
#define __op__pri_clamp__step(_x, _lo, _hi...) __op__pri_clamp(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _x, _lo, _hi)
#define __op__pri_clamp(__x, __lo, __hi, _x, _lo, _hi...) ({ \
    let __x = _x; \
    let __lo = _lo; \
    let __hi = _hi; \
    claim_assert(__lo <= __hi); \
    pri_clamp_static(__x, __lo, __hi); \
})
#define __op__int_wrap__step(_x, _lo, _hi...) __op__int_wrap(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _x, _lo, _hi)
#define __op__int_wrap(__x, __lo, __hi, _x, _lo, _hi...) ({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__lo, IntType) = _lo; \
    let_(__hi, IntType) = _hi; \
    claim_assert(__lo < __hi); \
    __lo + int_modEuclid(__x - __lo, __hi - __lo); \
})

#define __op__flt_wrap__step(_x, _lo, _hi...) __op__flt_wrap(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _x, _lo, _hi)
#define __op__flt_wrap(__x, __lo, __hi, _x, _lo, _hi...) ({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__lo, FltType) = _lo; \
    let_(__hi, FltType) = _hi; \
    claim_assert(__lo < _hi); \
    __lo + flt_modEuclid(__x - __lo, __hi - __lo); \
})
#define __op__flt_wrap01(_x...) flt_wrap(_x, 0, 1)

#define ____isValidAlign_static(_align...) as$(bool)(0 < _align && (_align & (_align - 1)) == 0)
#define __step__isValidAlign(_align...) ____isValidAlign(pp_uniqTok(align), _align)
#define ____isValidAlign(__align, _align...) ({ \
    let_(__align, usize) = _align; \
    as$(bool)(0 < __align && (__align & (__align - 1)) == 0); \
})

#define __step__isAligned(_addr, _align...) ____isAligned( \
    pp_uniqTok(addr), _addr, pp_uniqTok(align), _align \
)
#define ____isAligned(__addr, _addr, __align, _align...) ({ \
    let_(__addr, usize) = ptrToInt(_addr); \
    let_(__align, usize) = _align; \
    claim_assert(isValidAlign(__align)); \
    as$(bool)((__addr & (__align - 1)) == 0); \
})
#define __step__isAlignedLog2(_addr, _log2_align...) ____isAlignedLog2( \
    pp_uniqTok(addr), _addr, pp_uniqTok(log2_align), _log2_align \
)
#define ____isAlignedLog2(__addr, _addr, __log2_align, _log2_align...) ({ \
    let_(__addr, usize) = ptrToInt(_addr); \
    let_(__log2_align, u8) = _log2_align; \
    as$(bool)(int_trailingZeros(__addr) >= __log2_align); \
})

#define __step__alignFwd(_addr, _align...) ____alignFwd(pp_uniqTok(addr), _addr, pp_uniqTok(align), _align)
#define ____alignFwd(__addr, _addr, __align, _align...) ({ \
    let_(__addr, usize) = ptrToInt(_addr); \
    let_(__align, usize) = _align; \
    claim_assert(isValidAlign(__align)); \
    (__addr + (__align - 1)) & ~(__align - 1); \
})
#define __step__alignFwdLog2(_addr, _log2_align...) ____alignFwdLog2(pp_uniqTok(addr), _addr, pp_uniqTok(log2_align), _log2_align)
#define ____alignFwdLog2(__addr, _addr, __log2_align, _log2_align...) ({ \
    let_(__addr, usize) = ptrToInt(_addr); \
    let_(__log2_align, u8) = _log2_align; \
    claim_assert(isValidAlign(__log2_align)); \
    (__addr + (__log2_align - 1)) & ~(__log2_align - 1); \
})
#define __step__alignBwd(_addr, _align...) ____alignBwd(pp_uniqTok(addr), _addr, pp_uniqTok(align), _align)
#define ____alignBwd(__addr, _addr, __align, _align...) ({ \
    let_(__addr, usize) = ptrToInt(_addr); \
    let_(__align, usize) = _align; \
    claim_assert(isValidAlign(__align)); \
    (__addr & ~(__align - 1)); \
})
#define __step__alignBwdLog2(_addr, _log2_align...) ____alignBwdLog2(pp_uniqTok(addr), _addr, pp_uniqTok(log2_align), _log2_align)
#define ____alignBwdLog2(__addr, _addr, __log2_align, _log2_align...) ({ \
    let_(__addr, usize) = ptrToInt(_addr); \
    let_(__log2_align, u8) = _log2_align; \
    claim_assert(isValidAlign(__log2_align)); \
    (__addr & ~(__log2_align - 1)); \
})

#define ____alignToLog2_static(_align...) (as$(u8)(usize_bits - 1u) - as$(u8)(raw_trailingZeros64(as$(u64)(_align))))
#define __step__alignToLog2(_align...) ____alignToLog2(pp_uniqTok(align), _align)
#define ____alignToLog2(__align, _align...) ({ \
    let_(__align, usize) = _align; \
    claim_assert(isValidAlign(__align)); \
    intCast$((u8)(usize_bits - 1u)) - intCast$((u8)(raw_trailingZeros64(as$(u64)(__align)))); \
})
#define ____log2ToAlign_static(_log2_align...) (as$(usize)(1) << _log2_align)
#define __step__log2ToAlign(_log2_align...) ____log2ToAlign(pp_uniqTok(log2_align), _log2_align)
#define ____log2ToAlign(__log2_align, _log2_align...) ({ \
    let_(__log2_align, u8) = _log2_align; \
    as$(usize)(1) << __log2_align; \
})

#define __step__alignCast(...) __step__alignCast__emit(__step__alignCast__parse __VA_ARGS__)
#define __step__alignCast__parse(_log2_align...) pp_uniqTok(log2_align), _log2_align, pp_uniqTok(ptr),
#define __step__alignCast__emit(...) ____alignCast(__VA_ARGS__)
#define ____alignCast(__log2_align, _log2_align, __ptr, _ptr...) local_({ \
    let_(__log2_align, u8) = _log2_align; \
    let __ptr = _ptr; \
    claim_assert(isAlignedLog2(__ptr, __log2_align)); \
    local_return_(__ptr); \
})

#if UNUSED_CODE
#define __step__bitCast$(...) __step__bitCast$__emit(__step__bitCast$__parse __VA_ARGS__)
#define __step__bitCast$__parse(_T...) _T, pp_uniqTok(val),
#define __step__bitCast$__emit(...) ____bitCast$(__VA_ARGS__)
#define ____bitCast$(_T, __val, _val...) ({ \
    let_(__val, TypeOf(_val)) = _val; \
    claim_assert_static(sizeOf$(_T) == sizeOf$(TypeOf(__val))); \
    *ptrCast$((_T*)(raw_memcpy(&l0$((_T)), &__val, sizeOf$(TypeOf(__val))))); \
})
#endif /* UNUSED_CODE */
#define __step__bitCast$(...) __step__bitCast$__emit(__step__bitCast$__parse __VA_ARGS__)
#define __step__bitCast$__parse(_T...) pp_uniqTok(dst), _T, pp_uniqTok(val),
#define __step__bitCast$__emit(...) ____bitCast$(__VA_ARGS__)
#define ____bitCast$(__dst, _T, __val, _val...) ({ \
    var __dst = l0$((_T)); \
    let_(__val, TypeOf(_val)) = _val; \
    claim_assert_static(sizeOf$(_T) == sizeOf$(TypeOf(__val))); \
    raw_memcpy(&__dst, &__val, sizeOf$(TypeOf(__val))); \
    __dst; \
})
#define __step__intToBool(_val...) ____intToBool(pp_uniqTok(val), _val)
#define ____intToBool(__val, _val...) ({ \
    typedef TypeOf(_val) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__val, IntType) = _val; \
    claim_assert(__val == 0 || __val == 1); \
    as$(bool)(__val); \
})
#define ____boolToInt(_val...) T_switch$((TypeOf(_val))( \
    T_case$((bool)(as$(u8)(_val))) \
))
#define __step__intCast$(...) __step__intCast$__emit(__step__intCast$__parse __VA_ARGS__)
#define __step__intCast$__parse(_T...) \
    _T, pp_uniqTok(val), pp_uniqTok(min), pp_uniqTok(max), \
        pp_uniqTok(dst_is_signed), pp_uniqTok(src_is_signed),
#define __step__intCast$__emit(...) ____intCast$(__VA_ARGS__)
#define ____intCast$(_T, __val, __min, __max, __dst_is_signed, __src_is_signed, _val...) $supress_implicit_int_conversion( \
    ({ \
        typedef _T DstType; \
        typedef TypeOf(_val) SrcType; \
        claim_assert_static(isInt$(SrcType)); \
        let_(__val, SrcType) = _val; \
        let_(__min, DstType) = int_limit_min$(DstType); \
        let_(__max, DstType) = int_limit_max$(DstType); \
        let_(__dst_is_signed, bool) = isIInt$(DstType); \
        let_(__src_is_signed, bool) = isIInt$(SrcType); \
        /* Lower bound check: ensure value >= target minimum */ \
        claim_assert( \
            __dst_is_signed \
                ? (!__src_is_signed ? true : as$(i64)(__val) >= as$(i64)(__min)) \
                : (!__src_is_signed ? true : __val >= 0) \
        ); \
        /* Upper bound check: ensure value <= target maximum */ \
        claim_assert( \
            (__src_is_signed && __val < 0) \
                ? true \
                : as$(u64)(__val) <= as$(u64)(__max) \
        ); \
        as$(_T)(__val); \
    }) \
)
#if UNUSED_CODE
#define ____intCast$(_T, __val, __min, __max, _val...) ({ \
    typedef TypeOf(_T) DstType; \
    typedef TypeOf(_val) SrcType; \
    claim_assert_static(isInt$(SrcType)); \
    let_(__val, SrcType) = _val; \
    let_(__max, DstType) = int_limit_max$(DstType); \
    if (isIInt$(SrcType)) { \
        /* Min Check: Dest is unsigned then 0 or more, Signed then Min or more */ \
        /* Dest is signed then Min variable is needed, so use lazy or ternary operator */ \
        if (isIInt$(DstType)) { \
            let_(__min, DstType) = int_limit_min$(DstType); \
            claim_assert(__val >= __min); \
        } else /* isUInt$(DstType) */ { \
            claim_assert(__val >= 0); \
        } \
        /* Max Check: only check when positive (negative is handled above) */ \
        if (__val >= 0) { claim_assert(as$(u64)(__val) <= as$(u64)(__max)); } \
    } else /* isUInt$(SrcType) */ { \
        /* Min Check: unnecessary (Unsigned is always 0 or more) */ \
        /* Max Check: simply compare with u64 */ \
        claim_assert(as$(u64)(__val) <= as$(u64)(__max)); \
    } \
    as$(_T)(__val); \
})
#endif /* UNUSED_CODE */
#define __step__intToFlt$(...) __step__intToFlt$__emit(__step__intToFlt$__parse __VA_ARGS__)
#define __step__intToFlt$__parse(_T...) _T, pp_uniqTok(val),
#define __step__intToFlt$__emit(...) ____intToFlt$(__VA_ARGS__)
#define ____intToFlt$(_T, __val, _val...) ({ \
    typedef TypeOf(_val) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__val, IntType) = _val; \
    as$(_T)(__val); \
})
#define __step__fltCast$(...) __step__fltCast$__emit(__step__fltCast$__parse __VA_ARGS__)
#define __step__fltCast$__parse(_T...) _T, pp_uniqTok(val), pp_uniqTok(min), pp_uniqTok(max),
#define __step__fltCast$__emit(...) ____fltCast$(__VA_ARGS__)
#define ____fltCast$(_T, __val, __min, __max, _val...) ({ \
    typedef _T DstType; \
    typedef TypeOf(_val) SrcType; \
    claim_assert_static(isFlt$(SrcType)); \
    let_(__val, SrcType) = _val; \
    let_(__min, DstType) = flt_limit_min$(DstType); \
    let_(__max, DstType) = flt_limit_max$(DstType); \
    claim_assert(__min <= __max); \
    claim_assert(__min <= __val); \
    claim_assert(__val <= __max); \
    as$(_T)(__val); \
})
/* Check: (min - 1) < x < max, where max = 2^(N-1) for signed, 2^N for unsigned */
/* This correctly handles cases like -2147483648.9 → trunc → -2147483648 (valid i32) */
#define __step__fltToInt$(...) __step__fltToInt$__emit(__step__fltToInt$__parse __VA_ARGS__)
#define __step__fltToInt$__parse(_T...) _T, pp_uniqTok(val),
#define __step__fltToInt$__emit(...) ____fltToInt$(__VA_ARGS__)
#define ____fltToInt$(_T, __val, _val...) ({ \
    typedef TypeOf(_val) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__val, FltType) = _val; \
    claim_assert(flt_isFinite(__val)); \
    claim_assert(as$(FltType)(int_limit_flt_min_bound_excl$(_T)) < __val); \
    claim_assert(__val < as$(FltType)(int_limit_flt_max_bound_excl$(_T))); \
    as$(_T)(__val); \
})

/*========== Integer Arithmetic Implementation ==============================*/

#define __op__int_add__step(_lhs, _rhs...) __op__int_add(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__int_add(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    claim_assert(!__builtin_add_overflow(__lhs, __rhs, &l0$((IntType)))); \
    local_return_(as$(IntType)(__lhs + __rhs)); \
})
#define __op__int_sub__step(_lhs, _rhs...) __op__int_sub(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__int_sub(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    claim_assert(!__builtin_sub_overflow(__lhs, __rhs, &l0$((IntType)))); \
    local_return_(as$(IntType)(__lhs - __rhs)); \
})
#define __op__int_mul__step(_lhs, _rhs...) __op__int_mul(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__int_mul(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    claim_assert(!__builtin_mul_overflow(__lhs, __rhs, &l0$((IntType)))); \
    local_return_(as$(IntType)(__lhs * __rhs)); \
})
#define __op__int_mulAdd(_x, _y, _z...) int_add(int_mul(_x, _y), _z)
#define __op__int_div__step(_x, _n...) __op__int_div(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__int_div(__x, __n, _x, _n...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__n, IntType) = _n; \
    claim_assert(__n != 0); \
    claim_assert(isIInt$(IntType) ? !(__x == int_limit_min$(IntType) && __n == as$(IntType)(-1)) : true); \
    local_return_(as$(IntType)(__x / __n)); \
})

#define __op__iint_neg__step(_x...) __op__iint_neg(pp_uniqTok(x), _x)
#define __op__iint_neg(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _x; \
    claim_assert(__x != int_limit_min$(IIntType)); \
    local_return_(as$(IIntType)(-__x)); \
})
#define __op__iint_abs(_x...) local_({ \
    typedef TypeOfUnqual(_x) IIntType; \
    let_(__x, IIntType) = _x; \
    claim_assert(__x != int_limit_min$(IIntType)); \
    local_return_(T_switch$((TypeOf(IIntType))( \
        T_case$((i8)(__builtin_abs(as$(i8)(_x)))), \
        T_case$((i16)(__builtin_abs(as$(i16)(_x)))), \
        T_case$((i32)(__builtin_abs(as$(i32)(_x)))), \
        T_case$((i64)(__builtin_llabs(as$(i64)(_x)))), \
    ))); \
})

/*========== Integer Comparison Implementation ==============================*/

#define __op__int_eq(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    local_return_(pri_eq(__lhs, __rhs)); \
})
#define __op__int_ne(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    local_return_(pri_ne(__lhs, __rhs)); \
})
#define __op__int_lt(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    local_return_(pri_lt(__lhs, __rhs)); \
})
#define __op__int_gt(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    local_return_(pri_gt(__lhs, __rhs)); \
})
#define __op__int_le(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    local_return_(pri_le(__lhs, __rhs)); \
})
#define __op__int_ge(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    local_return_(pri_ge(__lhs, __rhs)); \
})

/*========== Integer Min/Max/Clamp Implementation ===========================*/

#define __op__int_min(__lhs, __rhs, _lhs, _rhs...) ({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    as$(IntType)(pri_min_static(__lhs, __rhs)); \
})
#define __op__int_max(__lhs, __rhs, _lhs, _rhs...) ({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    as$(IntType)(pri_max_static(__lhs, __rhs)); \
})
#define __op__int_clamp__step(_x, _lo, _hi...) __op__int_clamp(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _x, _lo, _hi)
#define __op__int_clamp(__x, __lo, __hi, _x, _lo, _hi...) ({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    let_(__lo, IntType) = _lo; \
    let_(__hi, IntType) = _hi; \
    claim_assert(__lo <= __hi); \
    int_min(int_max(__lo, __x), __hi); \
})

/*========== Bitwise Operations Implementation ==============================*/

#define __op__int_not_static(_x...) (as$(TypeOf(_x))((~(_x))))
#define __step__int_not(_x...) __op__int_not(pp_uniqTok(x), _x)
#define __op__int_not(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    local_return_(as$(IntType)(~__x)); \
})
#define __op__int_shl_static(_x, _y...) (as$(TypeOf(_x))((_x) << (_y)))
#define __step__int_shl(_x, _n...) __op__int_shl(pp_uniqTok(x), _x, pp_uniqTok(n), _n)
#define __op__int_shl(__x, _x, __n, _n...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    let_(__n, u32) = _n; \
    claim_assert(__n < int_bits$(IntType)); \
    local_return_(as$(IntType)(__x << __n)); \
})
#define __op__int_shr_static(_x, _y...) (as$(TypeOf(_x))((_x) >> (_y)))
#define __step__int_shr(_x, _n...) __op__int_shr(pp_uniqTok(x), _x, pp_uniqTok(n), _n)
#define __op__int_shr(__x, _x, __n, _n...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    let_(__n, u32) = _n; \
    claim_assert(__n < int_bits$(IntType)); \
    local_return_(as$(IntType)(__x >> __n)); \
})
#define __op__int_and_static(_x, _y...) (as$(TypeOf(_x))((_x) & (_y)))
#define __step__int_and(_lhs, _rhs...) __op__int_and(pp_uniqTok(lhs), _lhs, pp_uniqTok(rhs), _rhs)
#define __op__int_and(__lhs, _lhs, __rhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    local_return_(as$(IntType)(__lhs & __rhs)); \
})
#define __op__int_xor_static(_x, _y...) (as$(TypeOf(_x))((_x) ^ (_y)))
#define __step__int_xor(_lhs, _rhs...) __op__int_xor(pp_uniqTok(lhs), _lhs, pp_uniqTok(rhs), _rhs)
#define __op__int_xor(__lhs, _lhs, __rhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    local_return_(as$(IntType)(__lhs ^ __rhs)); \
})
#define __op__int_or_static(_x, _y...) (as$(TypeOf(_x))((_x) | (_y)))
#define __step__int_or(_lhs, _rhs...) __op__int_or(pp_uniqTok(lhs), _lhs, pp_uniqTok(rhs), _rhs)
#define __op__int_or(__lhs, _lhs, __rhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    local_return_(as$(IntType)(__lhs | __rhs)); \
})

#define __op__int_setBit_static(_x, _bit...) (as$(TypeOf(_x))((_x) | (as$(TypeOf(_x))(1) << (_bit))))
#define __op__int_setBit(_x, _bit...) ({ \
    typedef TypeOf(_x) IntType; \
    as$(IntType)(_x | int_shl(as$(IntType)(1), _bit)); \
})
#define __op__int_resetBit_static(_x, _bit...) (as$(TypeOf(_x))((_x) & ~(as$(TypeOf(_x))(1) << (_bit))))
#define __op__int_resetBit(_x, _bit...) ({ \
    typedef TypeOf(_x) IntType; \
    as$(IntType)(_x & ~int_shl(as$(IntType)(1), _bit)); \
})
#define __op__int_toggleBit_static(_x, _bit...) (as$(TypeOf(_x))((_x) ^ (as$(TypeOf(_x))(1) << (_bit))))
#define __op__int_toggleBit(_x, _bit...) ({ \
    typedef TypeOf(_x) IntType; \
    as$(IntType)(_x ^ int_shl(as$(IntType)(1), _bit)); \
})

#define __step__int_maskLo_static$(...) __step__int_maskLo_static$__emit(__step__int_maskLo_static$__parse __VA_ARGS__)
#define __step__int_maskLo_static$__parse(_T...) _T,
#define __step__int_maskLo_static$__emit(...) ____int_maskLo_static$(__VA_ARGS__)
#define ____int_maskLo_static$(_T, _n...) ( \
    as$(_T)((_n == int_bits$(_T)) ? as$(_T)(~0) : (as$(_T)(1) << _n) - as$(_T)(1)) \
)
#define __step__int_maskLo$(...) __step__int_maskLo$__emit(__step__int_maskLo$__parse __VA_ARGS__)
#define __step__int_maskLo$__parse(_T...) _T, pp_uniqTok(n),
#define __step__int_maskLo$__emit(...) ____int_maskLo$(__VA_ARGS__)
#define ____int_maskLo$(_T, __n, _n...) (as$(_T)(local_({ \
    let_(__n, u32) = _n; \
    claim_assert(__n <= int_bits$(_T)); \
    local_return_((__n == int_bits$(_T)) ? as$(_T)(~0) : (as$(_T)(1) << __n) - as$(_T)(1)); \
})))
#define __step__int_maskHi_static$(...) __step__int_maskHi_static$__emit(__step__int_maskHi_static$__parse __VA_ARGS__)
#define __step__int_maskHi_static$__parse(_T...) _T,
#define __step__int_maskHi_static$__emit(...) ____int_maskHi_static$(__VA_ARGS__)
#define ____int_maskHi_static$(_T, _n...) ( \
    as$(_T)((_n == 0) ? as$(_T)(0) : as$(_T)(as$(_T)(~0) << (int_bits$(_T) - _n))) \
)
#define __step__int_maskHi$(...) __step__int_maskHi$__emit(__step__int_maskHi$__parse __VA_ARGS__)
#define __step__int_maskHi$__parse(_T...) _T, pp_uniqTok(n),
#define __step__int_maskHi$__emit(...) ____int_maskHi$(__VA_ARGS__)
#define ____int_maskHi$(_T, __n, _n...) (as$(_T)(local_({ \
    let_(__n, u32) = _n; \
    claim_assert(__n <= int_bits$(_T)); \
    local_return_((__n == 0) ? as$(_T)(0) : as$(_T)(as$(_T)(~0) << (int_bits$(_T) - __n))); \
})))
#define __step__int_mask_static$(...) __step__int_mask_static$__emit(__step__int_mask_static$__parse __VA_ARGS__)
#define __step__int_mask_static$__parse(_T...) _T, __step__int_mask_static$__parseNext
#define __step__int_mask_static$__parseNext(_off, _len...) _off, _len
#define __step__int_mask_static$__emit(...) ____int_mask_static$(__VA_ARGS__)
#define ____int_mask_static$(_T, _off, _len...) ( \
    as$(_T)(int_maskLo_static$((_T)(_len)) << _off) \
)
#define __step__int_mask$(...) __step__int_mask$__emit(__step__int_mask$__parse __VA_ARGS__)
#define __step__int_mask$__parse(_T...) _T, __step__int_mask$__parseNext
#define __step__int_mask$__parseNext(_off, _len...) pp_uniqTok(off), _off, pp_uniqTok(len), _len
#define __step__int_mask$__emit(...) ____int_mask$(__VA_ARGS__)
#define ____int_mask$(_T, __off, _off, __len, _len...) (as$(_T)(local_({ \
    let_(__off, u32) = _off; \
    let_(__len, u32) = _len; \
    claim_assert((__off + __len) <= int_bits$(_T)); \
    local_return_(int_maskLo$((_T)(__len)) << __off); \
})))

/*========== Bit Manipulation Operations Implementation =====================*/

#define ____int_countOnes_static(_x /*: IntType */... /*(u32)*/) (as$(u32)( \
    _x == 0 \
        ? 0 \
        : T_switch$((TypeOf(_x))( \
              T_case$((u8)(raw_countOnes8(as$(u8) _x))), \
              T_case$((u16)(raw_countOnes16(as$(u16) _x))), \
              T_case$((u32)(raw_countOnes32(as$(u32) _x))), \
              T_case$((u64)(raw_countOnes64(as$(u64) _x))), \
              T_case$((i8)(raw_countOnes8(as$(i8) _x))), \
              T_case$((i16)(raw_countOnes16(as$(i16) _x))), \
              T_case$((i32)(raw_countOnes32(as$(i32) _x))), \
              T_case$((i64)(raw_countOnes64(as$(i64) _x))) \
          )) \
))
#define __step__int_countOnes(_x...) ____int_countOnes(pp_uniqTok(x), _x)
#define ____int_countOnes(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    local_return_(int_countOnes_static(__x)); \
})

#define ____int_countZeros_static(_x /*: IntType */... /*(u32)*/) ( \
    int_bits$(TypeOf(_x)) - int_countOnes_static(_x) \
)
#define __step__int_countZeros(_x...) ____int_countZeros(pp_uniqTok(x), _x)
#define ____int_countZeros(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    local_return_(int_countZeros_static(__x)); \
})

#define ____int_trailingOnes_static(_x /*: IntType */... /*(u32)*/) \
    int_trailingZeros_static(as$(TypeOf(_x))(~(_x)))
#define __step__int_trailingOnes(_x...) ____int_trailingOnes(pp_uniqTok(x), _x)
#define ____int_trailingOnes(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    local_return_(int_trailingOnes_static(__x)); \
})

#define ____int_trailingZeros_static(_x /*: IntType */... /*(u32)*/) (as$(u32)( \
    _x == 0 \
        ? int_bits$(TypeOf(_x)) \
        : T_switch$((TypeOf(_x))( \
              T_case$((u8)(raw_trailingZeros8(as$(u8)(_x)))), \
              T_case$((u16)(raw_trailingZeros16(as$(u16)(_x)))), \
              T_case$((u32)(raw_trailingZeros32(as$(u32)(_x)))), \
              T_case$((u64)(raw_trailingZeros64(as$(u64)(_x)))), \
              T_case$((i8)(raw_trailingZeros8(as$(i8)(_x)))), \
              T_case$((i16)(raw_trailingZeros16(as$(i16)(_x)))), \
              T_case$((i32)(raw_trailingZeros32(as$(i32)(_x)))), \
              T_case$((i64)(raw_trailingZeros64(as$(i64)(_x)))) \
          )) \
))
#define __step__int_trailingZeros(_x...) ____int_trailingZeros(pp_uniqTok(x), _x)
#define ____int_trailingZeros(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    local_return_(int_trailingZeros_static(__x)); \
})

#define ____int_leadingOnes_static(_x /*: IntType */... /*(u32)*/) \
    int_leadingZeros_static(as$(TypeOf(_x))(~(_x)))
#define __step__int_leadingOnes(_x...) ____int_leadingOnes(pp_uniqTok(x), _x)
#define ____int_leadingOnes(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    local_return_(int_leadingOnes_static(__x)); \
})

#define ____int_leadingZeros_static(_x /*: IntType */... /*(u32)*/) (as$(u32)( \
    _x == 0 \
        ? int_bits$(TypeOf(_x)) \
        : T_switch$((TypeOf(_x))( \
              T_case$((u8)(raw_leadingZeros8(as$(u8)(_x)))), \
              T_case$((u16)(raw_leadingZeros16(as$(u16)(_x)))), \
              T_case$((u32)(raw_leadingZeros32(as$(u32)(_x)))), \
              T_case$((u64)(raw_leadingZeros64(as$(u64)(_x)))), \
              T_case$((i8)(raw_leadingZeros8(as$(i8)(_x)))), \
              T_case$((i16)(raw_leadingZeros16(as$(i16)(_x)))), \
              T_case$((i32)(raw_leadingZeros32(as$(i32)(_x)))), \
              T_case$((i64)(raw_leadingZeros64(as$(i64)(_x)))) \
          )) \
))
#define __step__int_leadingZeros(_x...) ____int_leadingZeros(pp_uniqTok(x), _x)
#define ____int_leadingZeros(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    local_return_(int_leadingZeros_static(__x)); \
})

#define ____int_rotateLeft_static(_x, _y...) (as$(TypeOf(_x))( \
    _x == 0 \
        ? 0 \
        : T_switch$((TypeOf(_x))( \
              T_case$((u8)(raw_rotateLeft8(as$(u8)(_x), as$(u8)(_y)))), \
              T_case$((u16)(raw_rotateLeft16(as$(u16)(_x), as$(u16)(_y)))), \
              T_case$((u32)(raw_rotateLeft32(as$(u32)(_x), as$(u32)(_y)))), \
              T_case$((u64)(raw_rotateLeft64(as$(u64)(_x), as$(u64)(_y)))), \
              T_case$((i8)(raw_rotateLeft8(as$(i8)(_x), as$(i8)(_y)))), \
              T_case$((i16)(raw_rotateLeft16(as$(i16)(_x), as$(i16)(_y)))), \
              T_case$((i32)(raw_rotateLeft32(as$(i32)(_x), as$(i32)(_y)))), \
              T_case$((i64)(raw_rotateLeft64(as$(i64)(_x), as$(i64)(_y)))) \
          )) \
))
#define __step__int_rotateLeft(_x, _y...) ____int_rotateLeft(pp_uniqTok(x), pp_uniqTok(y), _x, _y)
#define ____int_rotateLeft(__x, __y, _x, _y...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    let_(__y, IntType) = _y; \
    local_return_(int_rotateLeft_static(__x, __y)); \
})

#define ____int_rotateRight_static(_x, _y...) (as$(TypeOf(_x))( \
    _x == 0 \
        ? 0 \
        : T_switch$((TypeOf(_x))( \
              T_case$((u8)(raw_rotateRight8(as$(u8)(_x), as$(u8)(_y)))), \
              T_case$((u16)(raw_rotateRight16(as$(u16)(_x), as$(u16)(_y)))), \
              T_case$((u32)(raw_rotateRight32(as$(u32)(_x), as$(u32)(_y)))), \
              T_case$((u64)(raw_rotateRight64(as$(u64)(_x), as$(u64)(_y)))), \
              T_case$((i8)(raw_rotateRight8(as$(i8)(_x), as$(i8)(_y)))), \
              T_case$((i16)(raw_rotateRight16(as$(i16)(_x), as$(i16)(_y)))), \
              T_case$((i32)(raw_rotateRight32(as$(i32)(_x), as$(i32)(_y)))), \
              T_case$((i64)(raw_rotateRight64(as$(i64)(_x), as$(i64)(_y)))) \
          )) \
))
#define __step__int_rotateRight(_x, _y...) ____int_rotateRight(pp_uniqTok(x), pp_uniqTok(y), _x, _y)
#define ____int_rotateRight(__x, __y, _x, _y...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    let_(__y, IntType) = _y; \
    local_return_(int_rotateRight_static(__x, __y)); \
})

#define ____int_swapBytes_static(_x...) (as$(TypeOf(_x))( \
    _x == 0 \
        ? 0 \
        : T_switch$((TypeOf(_x))( \
              T_case$((u8)(as$(u8)(_x))), \
              T_case$((u16)(raw_swapBytes16(as$(u16)(_x)))), \
              T_case$((u32)(raw_swapBytes32(as$(u32)(_x)))), \
              T_case$((u64)(raw_swapBytes64(as$(u64)(_x)))), \
              T_case$((i8)(as$(i8)(_x))), \
              T_case$((i16)(raw_swapBytes16(as$(i16)(_x)))), \
              T_case$((i32)(raw_swapBytes32(as$(i32)(_x)))), \
              T_case$((i64)(raw_swapBytes64(as$(i64)(_x)))) \
          )) \
))
#define __step__int_swapBytes(_x...) ____int_swapBytes(pp_uniqTok(x), _x)
#define ____int_swapBytes(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    local_return_(int_swapBytes_static(__x)); \
})

#define ____int_reverseBits_static(_x...) (as$(TypeOf(_x))( \
    _x == 0 \
        ? 0 \
        : T_switch$((TypeOf(_x))( \
              T_case$((u8)(raw_reverseBits8(as$(u8)(_x)))), \
              T_case$((u16)(raw_reverseBits16(as$(u16)(_x)))), \
              T_case$((u32)(raw_reverseBits32(as$(u32)(_x)))), \
              T_case$((u64)(raw_reverseBits64(as$(u64)(_x)))), \
              T_case$((i8)(raw_reverseBits8(as$(i8)(_x)))), \
              T_case$((i16)(raw_reverseBits16(as$(i16)(_x)))), \
              T_case$((i32)(raw_reverseBits32(as$(i32)(_x)))), \
              T_case$((i64)(raw_reverseBits64(as$(i64)(_x)))) \
          )) \
))
#define __step__int_reverseBits(_x...) ____int_reverseBits(pp_uniqTok(x), _x)
#define ____int_reverseBits(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    let_(__x, IntType) = _x; \
    local_return_(int_reverseBits_static(__x)); \
})

/*========== Bit Manipulation Implementation ================================*/

#if UNUSED_CODE
#define __op__int_rotateLeft(__x, __n, _x, _n...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__n, u32) = _n; \
    const u32 __mask = sizeof(IntType) * 8 - 1; \
    __n &= __mask; \
    as$(IntType)((__x << __n) | (__x >> ((-__n) & __mask))); \
})
#define __op__int_rotateRight(__x, __n, _x, _n...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__n, u32) = _n; \
    const u32 __mask = sizeof(IntType) * 8 - 1; \
    __n &= __mask; \
    as$(IntType)((__x >> __n) | (__x << ((-__n) & __mask))); \
})
#define __op__int_swapBytes(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    as$(IntType)(_Generic(sizeof(IntType), 1: __x, 2: __builtin_bswap16((u16)__x), 4: __builtin_bswap32((u32)__x), 8: __builtin_bswap64((u64)__x))); \
})
#define __op__int_reverseBits(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    as$(IntType)(_Generic(sizeof(IntType), 1: __builtin_bitreverse8((u8)__x), 2: __builtin_bitreverse16((u16)__x), 4: __builtin_bitreverse32((u32)__x), 8: __builtin_bitreverse64((u64)__x))); \
})
#endif /* UNUSED_CODE */

/*========== Integer Exponential and Logarithm Implementation ===================*/

#define __step__uint_exp10_static$(...) __step__uint_exp10_static$__emit(__step__uint_exp10_static$__parse __VA_ARGS__)
#define __step__uint_exp10_static$__parse(_T...) _T,
#define __step__uint_exp10_static$__emit(...) ____uint_exp10_static$(__VA_ARGS__)
/* clang-format off */
#define ____uint_exp10_static$(_T, _n...) (as$(_T)( \
    (_n) < 10 ? ____uint_exp10_static$__values0to9(_n) : \
    (_n) < 20 ? ____uint_exp10_static$__values10to19(_n) : 0u /* Overflow (> 10^19) for u64 */ \
))
#define ____uint_exp10_static$__values0to9(_n) ( \
    (_n) == 0 ? n_(1,u) : \
    (_n) == 1 ? n_(10,u) : \
    (_n) == 2 ? n_(100,u) : \
    (_n) == 3 ? n_(1,000,u) : \
    (_n) == 4 ? n_(10,000,u) : \
    (_n) == 5 ? n_(100,000,u) : \
    (_n) == 6 ? n_(1,000,000,u) : \
    (_n) == 7 ? n_(10,000,000,u) : \
    (_n) == 8 ? n_(100,000,000,u) : \
                n_(1,000,000,000,u)/* 9 */ \
)
#define ____uint_exp10_static$__values10to19(_n) ( \
    (_n) == 10 ? n_(10,000,000,000,ull) : \
    (_n) == 11 ? n_(100,000,000,000,ull) : \
    (_n) == 12 ? n_(1,000,000,000,000,ull) : \
    (_n) == 13 ? n_(10,000,000,000,000,ull) : \
    (_n) == 14 ? n_(100,000,000,000,000,ull) : \
    (_n) == 15 ? n_(1,000,000,000,000,000,ull) : \
    (_n) == 16 ? n_(10,000,000,000,000,000,ull) : \
    (_n) == 17 ? n_(100,000,000,000,000,000,ull) : \
    (_n) == 18 ? n_(1,000,000,000,000,000,000,ull) : \
                 n_(10,000,000,000,000,000,000,ull) /* 19 (Max for u64) */ \
)
/* clang-format on */
#define __step__uint_exp10$(...) __step__uint_exp10$(__step__uint_exp10$__parse __VA_ARGS__)
#define __step__uint_exp10$__parse(_T...) _T, pp_uniqTok(n),
#define __step__uint_exp10$__emit(...) ____uint_exp10$(__VA_ARGS__)
#define ____uint_exp10$(_T, __n, _n...) local_({ \
    typedef TypeOfUnqual(_n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _n; \
    claim_assert(0 < __n); \
    local_return_(uint_exp10_static$((_T)(__n))); \
})

#define __step__uint_exp2_static$(...) __step__uint_exp2_static$__emit(__step__uint_exp2_static$__parse __VA_ARGS__)
#define __step__uint_exp2_static$__parse(_T...) _T,
#define __step__uint_exp2_static$__emit(...) ____uint_exp2_static$(__VA_ARGS__)
#define ____uint_exp2_static$(_T, _n...) ( \
    as$(_T)(1) << (_n) \
)
#define __step__uint_exp2$(...) __step__uint_exp2$__emit(__step__uint_exp2$__parse __VA_ARGS__)
#define __step__uint_exp2$__parse(_T...) _T, pp_uniqTok(n),
#define __step__uint_exp2$__emit(...) ____uint_exp2$(__VA_ARGS__)
#define ____uint_exp2$(_T, __n, _n...) local_({ \
    typedef TypeOfUnqual(_n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _n; \
    claim_assert(0 <= __n); \
    local_return_(int_shl(as$(_T)(1), intCast$((u32)(__n)))); \
})

#define __step__uint_pow2_static$(...) uint_exp2_static$(__VA_ARGS__)
#define __step__uint_pow2$(...) uint_exp2$(__VA_ARGS__)

#define __step__uint_pow2Round_static$(...) \
    __step__uint_pow2Round_static$__emit(__step__uint_pow2Round_static$__parse __VA_ARGS__)
#define __step__uint_pow2Round_static$__parse(_T...) _T,
#define __step__uint_pow2Round_static$__emit(...) ____uint_pow2Round_static$(__VA_ARGS__)
#define ____uint_pow2Round_static$(_T, _n...) (as$(_T)( \
    (_n) <= 1 \
        ? as$(TypeOf(_n))(_n) \
        : ((((_n) - uint_pow2Floor_static$((TypeOf(_n))(_n))) < (uint_pow2Ceil_static$((TypeOf(_n))(_n)) - (_n))) \
               ? uint_pow2Floor_static$((TypeOf(_n))(_n)) \
               : uint_pow2Ceil_static$((TypeOf(_n))(_n))) \
))
#define __step__uint_pow2Round$(...) __step__uint_pow2Round$__emit(__step__uint_pow2Round$__parse __VA_ARGS__)
#define __step__uint_pow2Round$__parse(_T...) _T, pp_uniqTok(n),
#define __step__uint_pow2Round$__emit(...) ____uint_pow2Round$(__VA_ARGS__)
#define ____uint_pow2Round$(_T, __n, _n...) local_({ \
    typedef TypeOfUnqual(_n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _n; \
    local_return_(uint_pow2Round_static$((_T)(__n))); \
})

#define __step__uint_pow2Floor_static$(...) \
    __step__uint_pow2Floor_static$__emit(__step__uint_pow2Floor_static$__parse __VA_ARGS__)
#define __step__uint_pow2Floor_static$__parse(_T...) _T,
#define __step__uint_pow2Floor_static$__emit(...) ____uint_pow2Floor_static$(__VA_ARGS__)
#define ____uint_pow2Floor_static$(_T, _n...) (as$(_T)( \
    (_n) == 0 ? 0 : uint_exp2_static$((TypeOf(_n))(uint_log2_static(_n))) \
))
#define __step__uint_pow2Floor$(...) __step__uint_pow2Floor$__emit(__step__uint_pow2Floor$__parse __VA_ARGS__)
#define __step__uint_pow2Floor$__parse(_T...) _T, pp_uniqTok(n),
#define __step__uint_pow2Floor$__emit(...) ____uint_pow2Floor$(__VA_ARGS__)
#define ____uint_pow2Floor$(_T, __n, _n...) local_({ \
    typedef TypeOfUnqual(_n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _n; \
    local_return_(uint_pow2Floor_static$((_T)(__n))); \
})

#define __step__uint_pow2Ceil_static$(...) \
    __step__uint_pow2Ceil_static$__emit(__step__uint_pow2Ceil_static$__parse __VA_ARGS__)
#define __step__uint_pow2Ceil_static$__parse(_T...) _T,
#define __step__uint_pow2Ceil_static$__emit(...) ____uint_pow2Ceil_static$(__VA_ARGS__)
#define ____uint_pow2Ceil_static$(_T, _n...) (as$(_T)( \
    (_n) <= 1 \
        ? as$(TypeOf(_n))(1) \
        : uint_exp2_static$((TypeOf(_n))( \
              as$(u32)(int_bits$(TypeOf(_n))) \
              - int_leadingZeros_static(as$(TypeOf(_n))((_n) - 1)) \
          )) \
))
#define __step__uint_pow2Ceil$(...) __step__uint_pow2Ceil$__emit(__step__uint_pow2Ceil$__parse __VA_ARGS__)
#define __step__uint_pow2Ceil$__parse(_T...) _T, pp_uniqTok(n),
#define __step__uint_pow2Ceil$__emit(...) ____uint_pow2Ceil$(__VA_ARGS__)
#define ____uint_pow2Ceil$(_T, __n, _n...) local_({ \
    typedef TypeOfUnqual(_n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _n; \
    local_return_(uint_pow2Ceil_static$((_T)(__n))); \
})

#define __step__uint_log2_static(_n...) ( \
    (u32_(64u) - u32_(1u)) - int_leadingZeros_static(as$(u64)(_n)) \
)
#define __step__uint_log2(_n...) ____uint_log2(pp_uniqTok(n), _n)
#define ____uint_log2(__n, _n...) local_({ \
    typedef TypeOfUnqual(_n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _n; \
    claim_assert(0 < __n); \
    local_return_(uint_log2_static(__n)); \
})

/* clang-format off */
#define __step__uint_log10Round_static(_n...) ( \
    (_n) < n_(31,622,776,602,ull) \
        ? ____uint_log10Round_static__small(_n) \
        : ____uint_log10Round_static__large(_n) \
)
#define ____uint_log10Round_static__small(_n...) \
    /* Small value thresholds */ ( \
    (_n) < n_(4,u)             ? 0u : \
    (_n) < n_(32,u)            ? 1u : \
    (_n) < n_(317,u)           ? 2u : \
    (_n) < n_(3,163,u)         ? 3u : \
    (_n) < n_(31,623,u)        ? 4u : \
    (_n) < n_(316,228,u)       ? 5u : \
    (_n) < n_(3,162,278,u)     ? 6u : \
    (_n) < n_(31,622,777,u)    ? 7u : \
    (_n) < n_(316,227,767,u)   ? 8u : \
    (_n) < n_(3,162,277,661,u) ? 9u : 10u \
)
#define ____uint_log10Round_static__large(_n...) \
    /* Large value thresholds */ ( \
    (_n) < n_(316,227,766,017,ull)           ? 11u : \
    (_n) < n_(3,162,277,660,169,ull)         ? 12u : \
    (_n) < n_(31,622,776,601,684,ull)        ? 13u : \
    (_n) < n_(316,227,766,016,838,ull)       ? 14u : \
    (_n) < n_(3,162,277,660,168,380,ull)     ? 15u : \
    (_n) < n_(31,622,776,601,683,794,ull)    ? 16u : \
    (_n) < n_(316,227,766,016,837,934,ull)   ? 17u : \
    (_n) < n_(3,162,277,660,168,379,332,ull) ? 18u : \
    19u /* roughly up to max u64 */ \
)
/* clang-format on */
#define __step__uint_log10Round(_n...) ____uint_log10Round(pp_uniqTok(n), _n)
#define ____uint_log10Round(__n, _n...) local_({ \
    typedef TypeOfUnqual(_n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _n; \
    claim_assert(0 < __n); \
    local_return_(uint_log10Round_static(__n)); \
})

/* clang-format off */
#define __step__uint_log10Floor_static(_n...) ( \
    (_n) < n_(10,000,000,000,ull) \
        ? ____uint_log10Floor_static__small(_n) \
        : ____uint_log10Floor_static__large(_n) \
)
#define ____uint_log10Floor_static__small(_n...) \
    /* Small value (0 - 10^10-1): 0 to 9 */ ( \
    (_n) < n_(10,u)            ? 0u : \
    (_n) < n_(100,u)           ? 1u : \
    (_n) < n_(1,000,u)         ? 2u : \
    (_n) < n_(10,000,u)        ? 3u : \
    (_n) < n_(100,000,u)       ? 4u : \
    (_n) < n_(1,000,000,u)     ? 5u : \
    (_n) < n_(10,000,000,u)    ? 6u : \
    (_n) < n_(100,000,000,u)   ? 7u : \
    (_n) < n_(1,000,000,000,u) ? 8u : 9u \
)
#define ____uint_log10Floor_static__large(_n...) \
    /* Large value (10^10+): 10 to 19 */ ( \
    (_n) < n_(100,000,000,000,ull)            ? 10u : \
    (_n) < n_(1,000,000,000,000,ull)          ? 11u : \
    (_n) < n_(10,000,000,000,000,ull)         ? 12u : \
    (_n) < n_(100,000,000,000,000,ull)        ? 13u : \
    (_n) < n_(1,000,000,000,000,000,ull)      ? 14u : \
    (_n) < n_(10,000,000,000,000,000,ull)     ? 15u : \
    (_n) < n_(100,000,000,000,000,000,ull)    ? 16u : \
    (_n) < n_(1,000,000,000,000,000,000,ull)  ? 17u : \
    (_n) < n_(10,000,000,000,000,000,000,ull) ? 18u : 19u \
)
/* clang-format on */
#define __step__uint_log10Floor(_n...) ____uint_log10Floor(pp_uniqTok(n), _n)
#define ____uint_log10Floor(__n, _n...) local_({ \
    typedef TypeOfUnqual(_n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _n; \
    claim_assert(0 < __n); \
    local_return_(uint_log10Floor_static(__n)); \
})

/* clang-format off */
#define __step__uint_log10Ceil_static(_n...) ( \
    (_n) <= n_(10,000,000,000,ull) \
        ? ____uint_log10Ceil_static__small(_n) \
        : ____uint_log10Ceil_static__large(_n) \
)
#define ____uint_log10Ceil_static__small(_n...) \
    /* Small value (0 - 10^10) */ ( \
    (_n) <= n_(1,u)              ? 0u : \
    (_n) <= n_(10,u)             ? 1u : \
    (_n) <= n_(100,u)            ? 2u : \
    (_n) <= n_(1,000,u)          ? 3u : \
    (_n) <= n_(10,000,u)         ? 4u : \
    (_n) <= n_(100,000,u)        ? 5u : \
    (_n) <= n_(1,000,000,u)      ? 6u : \
    (_n) <= n_(10,000,000,u)     ? 7u : \
    (_n) <= n_(100,000,000,u)    ? 8u : \
    (_n) <= n_(1,000,000,000,u)  ? 9u : 10u \
)
#define ____uint_log10Ceil_static__large(_n...) \
    /* Large value (10^10+ - max) */ ( \
    (_n) <= n_(100,000,000,000,ull)            ? 11u : \
    (_n) <= n_(1,000,000,000,000,ull)          ? 12u : \
    (_n) <= n_(10,000,000,000,000,ull)         ? 13u : \
    (_n) <= n_(100,000,000,000,000,ull)        ? 14u : \
    (_n) <= n_(1,000,000,000,000,000,ull)      ? 15u : \
    (_n) <= n_(10,000,000,000,000,000,ull)     ? 16u : \
    (_n) <= n_(100,000,000,000,000,000,ull)    ? 17u : \
    (_n) <= n_(1,000,000,000,000,000,000,ull)  ? 18u : \
    (_n) <= n_(10,000,000,000,000,000,000,ull) ? 19u : 20u \
)
/* clang-format on */
#define __step__uint_log10Ceil(_n...) ____uint_log10Ceil(pp_uniqTok(n), _n)
#define ____uint_log10Ceil(__n, _n...) local_({ \
    typedef TypeOfUnqual(_n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _n; \
    claim_assert(0 < __n); \
    local_return_(uint_log10Ceil_static(__n)); \
})

/*========== Integer Query Implementation ===================================*/

#define __op__int_isOdd(_x...) ((_x & 1) != 0)
#define __op__int_isEven(_x...) ((_x & 1) == 0)
#define __op__int_isPow2(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    local_return_(__x > 0 && (__x & (__x - 1)) == 0); \
})
#define __op__int_hasSglBit(_x...) int_hasSglBit(_x)

/*========== Floating-Point Arithmetic Implementation =======================*/

#define __op__flt_add__step(_lhs, _rhs...) __op__flt_add(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__flt_add(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(as$(FltType)(__lhs + __rhs)); \
})
#define __op__flt_sub__step(_lhs, _rhs...) __op__flt_sub(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__flt_sub(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(as$(FltType)(__lhs - __rhs)); \
})
#define __op__flt_mul__step(_lhs, _rhs...) __op__flt_mul(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__flt_mul(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(as$(FltType)(__lhs * __rhs)); \
})
#define __op__flt_mulAdd__step(_x, _y, _z...) __op__flt_mulAdd(pp_uniqTok(x), pp_uniqTok(y), pp_uniqTok(z), _x, _y, _z)
#define __op__flt_mulAdd(__x, __y, __z, _x, _y, _z...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    let_(__y, FltType) = _y; \
    let_(__z, FltType) = _z; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_fmaf(as$(f32)(__x), as$(f32)(__y), as$(f32)(__z)))), \
        T_case$((f64)(__builtin_fma(as$(f64)(__x), as$(f64)(__y), as$(f64)(__z)))) \
    ))); \
})
#define __step__flt_div(_x, _n...) ____flt_div(pp_uniqTok(x), _x, pp_uniqTok(n), _n)
#define ____flt_div(__x, _x, __n, _n...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__n, FltType) = _n; \
    claim_assert(__n != 0); \
    local_return_(as$(FltType)(__x / __n)); \
})

#define __step__flt_neg(_x...) ____flt_neg(pp_uniqTok(x), _x)
#define ____flt_neg(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    local_return_(as$(FltType)(-__x)); \
})
#define ____flt_abs(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_fabsf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_fabs(as$(f64)(_x)))) \
))
#define ____flt_inv_static(_x...) (as$(TypeOf(_x))(1) / (_x))
#define __step__flt_inv(_x...) ____flt_inv(pp_uniqTok(x), _x)
#define ____flt_inv(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    local_return_(flt_inv_static(__x)); \
})

/*========== Floating-Point Comparison Implementation =======================*/

#define __op__flt_eq(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(pri_eq(__lhs, __rhs)); \
})
#define __op__flt_ne(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(pri_ne(__lhs, __rhs)); \
})
#define __op__flt_lt(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(pri_lt(__lhs, __rhs)); \
})
#define __op__flt_gt(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(pri_gt(__lhs, __rhs)); \
})
#define __op__flt_le(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(pri_le(__lhs, __rhs)); \
})
#define __op__flt_ge(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(pri_ge(__lhs, __rhs)); \
})

/*========== Floating-Point Min/Max/Clamp Implementation ====================*/

#define __op__flt_min(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_fminf(as$(f32)(__lhs), as$(f32)(__rhs)))), \
        T_case$((f64)(__builtin_fmin(as$(f64)(__lhs), as$(f64)(__rhs)))) \
    ))); \
})
#define __op__flt_max(__lhs, __rhs, _lhs, _rhs...) local_({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_fmaxf(as$(f32)(__lhs), as$(f32)(__rhs)))), \
        T_case$((f64)(__builtin_fmax(as$(f64)(__lhs), as$(f64)(__rhs)))) \
    ))); \
})
#define __op__flt_clamp__step(_x, _lo, _hi...) __op__flt_clamp(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _x, _lo, _hi)
#define __op__flt_clamp(__x, __lo, __hi, _x, _lo, _hi...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    let_(__lo, FltType) = _lo; \
    let_(__hi, FltType) = _hi; \
    claim_assert(__lo <= __hi); \
    local_return_(flt_min(flt_max(__lo, __x), __hi)); \
})
#define __op__flt_clamp01(_x...) flt_clamp(_x, 0, 1)

/*========== Floating-Point Classification Implementation ===================*/

#define ____flt_isNan(_x...) __builtin_isnan(_x)
#define ____flt_isInf(_x...) __builtin_isinf(_x)
#define ____flt_isFinite(_x...) __builtin_isfinite(_x)
#define ____flt_isNormal(_x...) __builtin_isnormal(_x)

/*========== Floating-Point Rounding Implementation =========================*/

#define __step__flt_trunc(_x...) ____flt_trunc(pp_uniqTok(x), _x)
#define ____flt_trunc(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_truncf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_trunc(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_round(_x...) ____flt_round(pp_uniqTok(x), _x)
#define ____flt_round(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_roundf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_round(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_ceil(_x...) ____flt_ceil(pp_uniqTok(x), _x)
#define ____flt_ceil(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_ceilf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_ceil(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_floor(_x...) ____flt_floor(pp_uniqTok(x), _x)
#define ____flt_floor(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_floorf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_floor(as$(f64)(__x)))) \
    ))); \
})

/*========== Floating-Point Power and Root Implementation ===================*/

#define __step__flt_sqrt(_x...) ____flt_sqrt(pp_uniqTok(x), _x)
#define ____flt_sqrt(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    claim_assert(__x >= 0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_sqrtf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_sqrt(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_cbrt(_x...) ____flt_cbrt(pp_uniqTok(x), _x)
#define ____flt_cbrt(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    claim_assert(__x >= 0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_cbrtf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_cbrt(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_sq(_x...) ____flt_sq(pp_uniqTok(x), _x)
#define ____flt_sq(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    local_return_(as$(FltType)(__x * __x)); \
})
#define __step__flt_cb(_x...) ____flt_cb(pp_uniqTok(x), _x)
#define ____flt_cb(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    local_return_(as$(FltType)(__x * __x * __x)); \
})
#define __step__flt_hypot(_x, _y...) ____flt_hypot(pp_uniqTok(x), _x, pp_uniqTok(y), _y)
#define ____flt_hypot(__x, _x, __y, _y...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    let_(__y, FltType) = _y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_hypotf(as$(f32)(__x), as$(f32)(__y)))), \
        T_case$((f64)(__builtin_hypot(as$(f64)(__x), as$(f64)(__y)))) \
    ))); \
})

/*========== Floating-Point Exponential and Logarithm Implementation ========*/

#define ____flt_exp(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_expf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_exp(as$(f64)(_x)))) \
))
#define ____flt_exp2(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_exp2f(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_exp2(as$(f64)(_x)))) \
))
#define __step__flt_powI(_x, _y...) ____flt_powI(pp_uniqTok(x), _x, pp_uniqTok(y), _y)
#define ____flt_powI(__x, _x, __y, _y...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    let_(__y, i32) = _y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_powif(as$(f32)(__x), __y))), \
        T_case$((f64)(__builtin_powi(as$(f64)(__x), __y))) \
    ))); \
})
#define __step__flt_powF(_x, _y...) ____flt_powF(pp_uniqTok(x), _x, pp_uniqTok(y), _y)
#define ____flt_powF(__x, _x, __y, _y...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    let_(__y, FltType) = _y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_powf(as$(f32)(__x), as$(f32)(__y)))), \
        T_case$((f64)(__builtin_pow(as$(f64)(__x), as$(f64)(__y)))) \
    ))); \
})
#define __step__flt_ln(_x) ____flt_ln(pp_uniqTok(x), _x)
#define ____flt_ln(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    claim_assert(__x > 0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_logf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_log(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_log2(_x) ____flt_log2(pp_uniqTok(x), _x)
#define ____flt_log2(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    claim_assert(__x > 0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_logf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_log(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_log10(_x) ____flt_log10(pp_uniqTok(x), _x)
#define ____flt_log10(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    claim_assert(__x > 0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_log10f(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_log10(as$(f64)(__x)))) \
    ))); \
})

/*========== Floating-Point Trigonometric Implementation ====================*/

#define ____flt_sin(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_sinf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_sin(as$(f64)(_x)))) \
))
#define ____flt_cos(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_cosf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_cos(as$(f64)(_x)))) \
))
#define ____flt_tan(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_tanf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_tan(as$(f64)(_x)))) \
))
#define __step__flt_asin(_x...) ____flt_asin(pp_uniqTok(x), _x)
#define ____flt_asin(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    /* Domain: [-1, 1] */ \
    claim_assert(flt_abs(__x) <= 1); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_asinf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_asin(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_acos(_x...) ____flt_acos(pp_uniqTok(x), _x)
#define ____flt_acos(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    /* Domain: [-1, 1] */ \
    claim_assert(flt_abs(__x) <= 1); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_acosf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_acos(as$(f64)(__x)))) \
    ))); \
})
#define ____flt_atan(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_atanf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_atan(as$(f64)(_x)))) \
))
#define __step__flt_atan2(_y, _x...) ____flt_atan2(pp_uniqTok(y), _y, pp_uniqTok(x), _x)
#define ____flt_atan2(__y, _y, __x, _x...) local_({ \
    typedef TypeOfUnqual(_y) FltType; \
    let_(__y, FltType) = _y; \
    let_(__x, FltType) = _x; \
    /* Domain: Non-Origin */ \
    claim_assert(isNonzero(__y) || isNonzero(__x)); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_atan2f(as$(f32)(__y), as$(f32)(__x)))), \
        T_case$((f64)(__builtin_atan2(as$(f64)(__y), as$(f64)(__x)))) \
    ))); \
})

/*========== Floating-Point Hyperbolic Implementation =======================*/

#define ____flt_sinh(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_sinhf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_sinh(as$(f64)(_x)))) \
))
#define ____flt_cosh(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_coshf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_cosh(as$(f64)(_x)))) \
))
#define ____flt_tanh(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_tanhf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_tanh(as$(f64)(_x)))) \
))
#define ____flt_asinh(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_asinhf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_asinh(as$(f64)(_x)))) \
))
#define __step__flt_acosh(_x...) ____flt_acosh(pp_uniqTok(x), _x)
#define ____flt_acosh(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    /* Domain: [1, Inf) */ \
    claim_assert(__x >= 1.0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_acoshf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_acosh(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_atanh(_x...) ____flt_atanh(pp_uniqTok(x), _x)
#define ____flt_atanh(__x, _x...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    /* Domain: (-1, 1) */ \
    claim_assert(flt_abs(__x) < 1.0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_atanhf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_atanh(as$(f64)(__x)))) \
    ))); \
})

/*========== Floating-Point IEEE 754 Implementation =========================*/

#define __op__flt_copySgn(__x, __y, _x, _y...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    let_(__y, FltType) = _y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_copysignf(as$(f32)(__x), as$(f32)(__y)))), \
        T_case$((f64)(__builtin_copysign(as$(f64)(__x), as$(f64)(__y)))) \
    ))); \
})
#define __op__flt_nextAfter(__x, __y, _x, _y...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    let_(__y, FltType) = _y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_nextafterf(as$(f32)(__x), as$(f32)(__y)))), \
        T_case$((f64)(__builtin_nextafter(as$(f64)(__x), as$(f64)(__y)))) \
    ))); \
})
#define __op__flt_scalb(__x, __n, _x, _n...) local_({ \
    typedef TypeOfUnqual(_x) FltType; \
    let_(__x, FltType) = _x; \
    let_(__n, i32) = _n; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_ldexpf(as$(f32)(__x), __n))), \
        T_case$((f64)(__builtin_ldexp(as$(f64)(__x), __n))) \
    ))); \
})
#define ____flt_ilogb(_x...) T_switch$((TypeOf(_x))( \
    T_case$((f32)(__builtin_ilogbf(as$(f32)(_x)))), \
    T_case$((f64)(__builtin_ilogb(as$(f64)(_x)))) \
))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_pri__included */

/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    prim.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2025-01-09 (date of last update)
 * @version v0.1-alpha.3
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  prim
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
#ifndef core_prim__included
#define core_prim__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim/cfg.h"

#include "prim/bool.h"
#include "prim/int.h"
#include "prim/flt.h"
#include "prim/ptr.h"
#include "prim/null.h"
#include "prim/wchar.h"

#include "prim/struct.h"
#include "prim/enum.h"
#include "prim/union.h"
#include "prim/switch.h"

/*========== Memory Operations ==============================================*/

typedef struct Void {
    u8 _bytes_zero[0];
} Void;

#define prim_memset(_ptr, _val, _len...) __op__prim_memset(_ptr, _val, _len)
#define prim_memsetS(_s_dst, _val...) __op__prim_memsetS(_s_dst, _val)
#define prim_memcpy(_p_dst, _p_src, _len...) __op__prim_memcpy(_p_dst, _p_src, _len)
#define prim_memcpyS(_s_dst, _s_src...) __op__prim_memcpyS(_s_dst, _s_src)
#define prim_memmove(_p_dst, _p_src, _len...) __op__prim_memmove(_p_dst, _p_src, _len)
#define prim_memmoveS(_s_dst, _s_src...) __op__prim_memmoveS(_s_dst, _s_src)
#define prim_memeql(_p_lhs, _p_rhs...) __op__prim_memeql(_p_lhs, _p_rhs)
#define prim_memeqlS(_s_lhs, _s_rhs...) __op__prim_memeqlS(_s_lhs, _s_rhs)
#define prim_memord(_p_lhs, _p_rhs...) __op__prim_memord(_p_lhs, _p_rhs)
#define prim_memordS(_s_lhs, _s_rhs...) __op__prim_memordS(_s_lhs, _s_rhs)
#define prim_memcmp(_p_lhs, _p_rhs, _len...) __op__prim_memcmp(_p_lhs, _p_rhs, _len)
#define prim_alloca(_bytes...) __op__prim_alloca(_bytes)
#define prim_swap(_p_lhs, _p_rhs...) __op__prim_swap__step(_p_lhs, _p_rhs)

#define u8_n(_literal...) lit$((u8){ _literal })
#define u16_n(_literal...) lit$((u16){ _literal })
#define u32_n(_literal...) lit$((u32){ _literal })
#define u64_n(_literal...) lit$((u64){ _literal })
#define usize_n(_literal...) lit$((u128){ _literal })

#define i8_n(_literal...) lit$((i8){ _literal })
#define i16_n(_literal...) lit$((i16){ _literal })
#define i32_n(_literal...) lit$((i32){ _literal })
#define i64_n(_literal...) lit$((i64){ _literal })
#define isize_n(_literal...) lit$((i128){ _literal })

#define f32_n(_literal...) lit$((f32){ _literal })
#define f64_n(_literal...) lit$((f64){ _literal })

/*========== Arithmetic Operations ==========================================*/

#define prim_add(_lhs, _rhs...) __op__prim_add(_lhs, _rhs)
#define prim_sub(_lhs, _rhs...) __op__prim_sub(_lhs, _rhs)
#define prim_mul(_lhs, _rhs...) __op__prim_mul(_lhs, _rhs)
#define prim_mulAdd(_x, _y, _z...) __op__prim_mulAdd(_x, _y, _z)
#define prim_div(_lhs, _rhs...) __op__prim_div(_lhs, _rhs)

#define int_divTrunc(_x, _n...) __op__int_div__step(_x, _n)
#define int_divRound(_x, _n...) __op__int_divRound__step(_x, _n)
#define iint_divFloor(_x, _n...) __op__iint_divFloor__step(_x, _n)
#define iint_divEuclid(_x, _n...) __op__iint_divEuclid__step(_x, _n)
#define int_divCeil(_x, _n...) __op__int_divCeil__step(_x, _n)

#define flt_divTrunc(_x, _n...) __op__flt_divTrunc(_x, _n)
#define flt_divRound(_x, _n...) __op__flt_divRound(_x, _n)
#define flt_divFloor(_x, _n...) __op__flt_divFloor(_x, _n)
#define flt_divEuclid(_x, _n...) __op__flt_divEuclid__step(_x, _n)
#define flt_divCeil(_x, _n...) __op__flt_divCeil(_x, _n)

#define int_rem_static(_x, _n...) __op__int_rem_static(_x, _n)
#define int_rem(_x, _n...) __op__int_rem__step(_x, _n)
#define int_remRound(_x, _n...) __op__int_remRound__step(_x, _n)
#define iint_mod(_x, _n...) __op__iint_mod__step(_x, _n)
#define int_modEuclid(_x, _n...) __op__int_modEuclid__step(_x, _n)
#define int_modCeil(_x, _n...) __op__int_modCeil__step(_x, _n)

#define flt_rem(_x, _n...) __op__flt_rem__step(_x, _n)
#define flt_remRound(_x, _n...) __op__flt_remRound__step(_x, _n)
#define flt_mod(_x, _n...) __op__flt_mod__step(_x, _n)
#define flt_modEuclid(_x, _n...) __op__flt_modEuclid__step(_x, _n)
#define flt_modCeil(_x, _n...) __op__flt_modCeil__step(_x, _n)

#define prim_neg(_x...) __op__prim_neg(_x)
#define prim_abs_static(_x...) __op__prim_abs_static(_x)
#define prim_abs(_x...) __op__prim_abs__step(_x)
#define prim_sgn_static(_x...) __op__prim_sgn_static(_x)
#define prim_sgn(_x...) __op__prim_sgn__step(_x)

#define prim_not(_x...) __op__prim_not(_x)
#define prim_shl(_x, _y...) __op__prim_shl(_x, _y)
#define prim_shr(_x, _y...) __op__prim_shr(_x, _y)
#define prim_and(_x, _y...) __op__prim_and(_x, _y)
#define prim_xor(_x, _y...) __op__prim_xor(_x, _y)
#define prim_or(_x, _y...) __op__prim_or(_x, _y)

#define prim_eql(_lhs, _rhs... /*(bool)*/) __op__prim_eql(_lhs, _rhs)
#define prim_ord_static(_lhs, _rhs...) __op__prim_ord_static(_lhs, _rhs)
#define prim_ord(_lhs, _rhs... /*(cmp_Ord)*/) __op__prim_ord__step(_lhs, _rhs)
#define prim_eq(_lhs, _rhs... /*(bool)*/) __op__prim_eq(_lhs, _rhs)
#define prim_ne(_lhs, _rhs... /*(bool)*/) __op__prim_ne(_lhs, _rhs)
#define prim_lt(_lhs, _rhs... /*(bool)*/) __op__prim_lt(_lhs, _rhs)
#define prim_gt(_lhs, _rhs... /*(bool)*/) __op__prim_gt(_lhs, _rhs)
#define prim_le(_lhs, _rhs... /*(bool)*/) __op__prim_le(_lhs, _rhs)
#define prim_ge(_lhs, _rhs... /*(bool)*/) __op__prim_ge(_lhs, _rhs)

#define prim_min_static(_lhs, _rhs...) __op__prim_min2_static(_lhs, _rhs)
#define prim_min(_lhs, _rhs...) __op__prim_min2__step(_lhs, _rhs)
#define prim_min2_static(_lhs, _rhs...) __op__prim_min2_static(_lhs, _rhs)
#define prim_min2(_lhs, _rhs...) __op__prim_min2__step(_lhs, _rhs)
#define prim_min3_static(_1st, _2nd, _3rd...) __op__prim_min3_static(_1st, _2nd, _3rd)
#define prim_min3(_1st, _2nd, _3rd...) __op__prim_min3__step(_1st, _2nd, _3rd)
#define prim_min4_static(_1st, _2nd, _3rd, _4th...) __op__prim_min4_static(_1st, _2nd, _3rd, _4th)
#define prim_min4(_1st, _2nd, _3rd, _4th...) __op__prim_min4__step(_1st, _2nd, _3rd, _4th)
#define prim_findMin(_vals...) __op__prim_findMin(_vals)
#define prim_max_static(_lhs, _rhs...) __op__prim_max2_static(_lhs, _rhs)
#define prim_max(_lhs, _rhs...) __op__prim_max2__step(_lhs, _rhs)
#define prim_max2_static(_lhs, _rhs...) __op__prim_max2_static(_lhs, _rhs)
#define prim_max2(_lhs, _rhs...) __op__prim_max2__step(_lhs, _rhs)
#define prim_max3_static(_1st, _2nd, _3rd...) __op__prim_max3_static(_1st, _2nd, _3rd)
#define prim_max3(_1st, _2nd, _3rd...) __op__prim_max3__step(_1st, _2nd, _3rd)
#define prim_max4_static(_1st, _2nd, _3rd, _4th...) __op__prim_max4_static(_1st, _2nd, _3rd, _4th)
#define prim_max4(_1st, _2nd, _3rd, _4th...) __op__prim_max3__step(_1st, _2nd, _3rd, _4th)
#define prim_findMax(_vals...) __op__prim_findMax(_vals)

#define prim_clamp_static(_x, _lo /*$incl*/, _hi /*$incl*/...) __op__prim_clamp_static(_x, _lo, _hi)
#define prim_clamp(_x, _lo /*$incl*/, _hi /*$incl*/...) __op__prim_clamp__step(_x, _lo, _hi)
#define int_wrap(_x, _lo /*$incl*/, _hi /*$excl*/...) __op__int_wrap__step(_x, _lo, _hi)
#define flt_wrap(_x, _lo /*$incl*/, _hi /*$excl*/...) __op__flt_wrap__step(_x, _lo, _hi)

/*========== Type Predicates ================================================*/

#define isBool$(_T...) __op__isBool$(_T)
#define isUInt$(_T...) __op__isUInt$(_T)
#define isIInt$(_T...) __op__isIInt$(_T)
#define isInt$(_T...) __op__isInt$(_T)
#define isFlt$(_T...) __op__isFlt$(_T)

/*========== Type Limits ====================================================*/

#define uint_limit$(_T...) __op__uint_limit$(_T)
#define int_limitMin$(_T...) __op__int_limitMin$(_T)
#define int_limitMax$(_T...) __op__int_limitMax$(_T)
#define iint_limitMaxNgtv$(_T...) __op__iint_limitMaxNgtv$(_T)
#define iint_limitMinNgtv$(_T...) __op__iint_limitMinNgtv$(_T)
#define iint_limitMinPstv$(_T...) __op__iint_limitMinPstv$(_T)
#define iint_limitMaxPstv$(_T...) __op__iint_limitMaxPstv$(_T)

#define int_bits$(_T...) __op__int_bits$(_T)
#define int_bytes$(_T...) __op__int_bytes$(_T)

#define flt_limitMin$(_T...) __op__flt_limitMin$(_T)
#define flt_limitMax$(_T...) __op__flt_limitMax$(_T)
#define flt_limitMaxNgtv$(_T...) __op__flt_limitMaxNgtv$(_T)
#define flt_limitMinNgtv$(_T...) __op__flt_limitMinNgtv$(_T)
#define flt_limitMinPstv$(_T...) __op__flt_limitMinPstv$(_T)
#define flt_limitMaxPstv$(_T...) __op__flt_limitMaxPstv$(_T)

#define flt_bits$(_T...) __op__flt_bits$(_T)
#define flt_bytes$(_T...) __op__flt_bytes$(_T)

#define flt_eps$(_T...) __op__flt_eps$(_T)
#define flt_nan$(_T...) __op__flt_nan$(_T)
#define flt_inf$(_T...) __op__flt_inf$(_T)
#define flt_infNgtv$(_T...) __op__flt_infNgtv$(_T)
#define flt_infPstv$(_T...) __op__flt_infPstv$(_T)

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

#define int_not(_x...) __op__int_not(pp_uniqTok(x), _x)
#define int_shl(_x, _n...) __op__int_shl(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define int_shr(_x, _n...) __op__int_shr(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define int_and(_lhs, _rhs...) __op__int_and(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define int_xor(_lhs, _rhs...) __op__int_xor(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define int_or(_lhs, _rhs...) __op__int_or(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)

/*========== Bit Manipulation Operations ====================================*/

#define int_countOnes(_x...) __op__int_countOnes(pp_uniqTok(x), _x)
#define int_countZeros(_x...) __op__int_countZeros(pp_uniqTok(x), _x)
#define int_countLeadingZeros(_x...) __op__int_countLeadingZeros(pp_uniqTok(x), _x)
#define int_countTrailingZeros(_x...) __op__int_countTrailingZeros(pp_uniqTok(x), _x)
#define int_rotateL(_x, _n...) __op__int_rotateL(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define int_rotateR(_x, _n...) __op__int_rotateR(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define int_swapBytes(_x...) __op__int_swapBytes(pp_uniqTok(x), _x)
#define int_reverseBits(_x...) __op__int_reverseBits(pp_uniqTok(x), _x)

/*========== Integer Exponential and Logarithm Operations ===================*/

#define int_exp2(_x...) __op__int_exp2(_x)
#define int_log2(_x...) __op__int_log2(_x)

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
#define flt_div(_x, _n...) __op__flt_div__step(_x, _n)
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

#define flt_neg(_x...) __op__flt_neg(_x)
#define flt_abs(_x...) __op__flt_abs(_x)

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
#define flt_wrap(_x, _lo /*$incl*/, _hi /*$excl*/...) __op__flt_wrap__step(_x, _lo, _hi)

/*========== Floating-Point Classification ==================================*/

#define flt_isNan(_x...) __op__flt_isNan(_x)
#define flt_isInf(_x...) __op__flt_isInf(_x)
#define flt_isFinite(_x...) __op__flt_isFinite(_x)
#define flt_isNormal(_x...) __op__flt_isNormal(_x)

/*========== Floating-Point Rounding Operations =============================*/

#define flt_trunc(_x...) __op__flt_trunc(pp_uniqTok(x), _x)
#define flt_round(_x...) __op__flt_round(pp_uniqTok(x), _x)
#define flt_floor(_x...) __op__flt_floor(pp_uniqTok(x), _x)
#define flt_ceil(_x...) __op__flt_ceil(pp_uniqTok(x), _x)

/*========== Floating-Point Power and Root Operations =======================*/

#define flt_sqrt(_x...) __op__flt_sqrt(pp_uniqTok(x), _x)
#define flt_cbrt(_x...) __op__flt_cbrt(pp_uniqTok(x), _x)
#define flt_sq(_x...) __op__flt_sq(pp_uniqTok(x), _x)
#define flt_cb(_x...) __op__flt_cb(pp_uniqTok(x), _x)
#define flt_hypot(_x, _y...) __op__flt_hypot(pp_uniqTok(x), pp_uniqTok(y), _x, _y)

/*========== Floating-Point Exponential and Logarithm =======================*/

#define flt_exp(_x...) __op__flt_exp(pp_uniqTok(x), _x)
#define flt_exp2(_x...) __op__flt_exp2(pp_uniqTok(x), _x)
#define flt_powI(_x, _y...) __op__flt_powI(pp_uniqTok(x), pp_uniqTok(y), _x, _y)
#define flt_powF(_x, _y...) __op__flt_powF(pp_uniqTok(x), pp_uniqTok(y), _x, _y)
#define flt_ln(_x...) __op__flt_ln(pp_uniqTok(x), _x)
#define flt_log2(_x...) __op__flt_log2(pp_uniqTok(x), _x)
#define flt_log10(_x...) __op__flt_log10(pp_uniqTok(x), _x)

/*========== Floating-Point Trigonometric Functions =========================*/

#define flt_sin(_x...) __op__flt_sin(pp_uniqTok(x), _x)
#define flt_cos(_x...) __op__flt_cos(pp_uniqTok(x), _x)
#define flt_tan(_x...) __op__flt_tan(pp_uniqTok(x), _x)
#define flt_asin(_x...) __op__flt_asin(pp_uniqTok(x), _x)
#define flt_acos(_x...) __op__flt_acos(pp_uniqTok(x), _x)
#define flt_atan(_x...) __op__flt_atan(pp_uniqTok(x), _x)
#define flt_atan2(_y, _x...) __op__flt_atan2(pp_uniqTok(y), pp_uniqTok(x), _y, _x)

/*========== Floating-Point Hyperbolic Functions ============================*/

#define flt_sinh(_x...) __op__flt_sinh(pp_uniqTok(x), _x)
#define flt_cosh(_x...) __op__flt_cosh(pp_uniqTok(x), _x)
#define flt_tanh(_x...) __op__flt_tanh(pp_uniqTok(x), _x)
#define flt_asinh(_x...) __op__flt_asinh(pp_uniqTok(x), _x)
#define flt_acosh(_x...) __op__flt_acosh(pp_uniqTok(x), _x)
#define flt_atanh(_x...) __op__flt_atanh(pp_uniqTok(x), _x)

/*========== Floating-Point IEEE 754 Operations =============================*/

#define flt_copySgn(_x, _y...) __op__flt_copySgn(pp_uniqTok(x), pp_uniqTok(y), _x, _y)
#define flt_nextAfter(_x, _y...) __op__flt_nextAfter(pp_uniqTok(x), pp_uniqTok(y), _x, _y)
#define flt_scalb(_x, _n...) __op__flt_scalb(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define flt_ilogb(_x...) __op__flt_ilogb(pp_uniqTok(x), _x)

/*===========================================================================*/
/*                          IMPLEMENTATION SECTION                           */
/*===========================================================================*/

/*========== Memory Operations Implementation ===============================*/

#if UNUSED_CODE
$inline_always
$static P$raw prim__memset(P$raw ptr, u8 val, usize len) {
    if (len == 0) { return ptr; }
    return memset(ptr, val, len);
}
$inline_always
$static P$raw prim__memcpy(P$raw dst, P_const$raw src, usize len) {
    if (len == 0 || dst == src) { return dst; }
    return memcpy(dst, src, len);
}
$inline_always
$static P$raw prim__memmove(P$raw dst, P_const$raw src, usize len) {
    if (len == 0 || dst == src) { return dst; }
    return memmove(dst, src, len);
}
$inline_always
$static u8 prim__memcmp(P_const$raw lhs, P_const$raw rhs, usize len) {
    if (len == 0 || lhs == rhs) { return 0; }
    return memcmp(lhs, rhs, len);
}
#endif /* UNUSED_CODE */

#define __op__prim_memset(_ptr, _val, _len...) memset(_ptr, _val, _len)
#define __op__prim_memsetS(_s_dst, _val...) u_castS$((TypeOf(_s_dst))(u_memsetS(u_anyS(_s_dst), u_anyV(make$((TypeOf(*(_s_dst).ptr))_val)))))
#define __op__prim_memcpy(_p_dst, _p_src, _len...) memcpy(_p_dst, _p_src, _len)
#define __op__prim_memcpyS(_s_dst, _s_src...) u_castS$((TypeOf(_s_dst))(u_memcpyS(u_anyS(_s_dst), u_anyS(make$((TypeOf(_s_dst.as_const))_s_src)))))
#define __op__prim_memmove(_p_dst, _p_src, _len...) memmove(_p_dst, _p_src, _len)
#define __op__prim_memmoveS(_s_dst, _s_src...) u_castS$((TypeOf(_s_dst))(u_memmoveS(u_anyS(_s_dst), u_anyS(make$((TypeOf(_s_dst.as_const))_s_src)))))
#define __op__prim_memeql(_p_lhs, _p_rhs...) u_castP$((TypeOf(_p_lhs))(u_memeql(u_anyP(_p_lhs), u_anyP(make$((TypeOf(_p_lhs))_p_rhs)))))
#define __op__prim_memeqlS(_s_lhs, _s_rhs...) u_castS$((TypeOf(_s_lhs))(u_memeqlS(u_anyS(_s_lhs), u_anyS(make$((TypeOf(_s_lhs.as_const))_s_rhs)))))
#define __op__prim_memord(_p_lhs, _p_rhs...) u_castP$((TypeOf(_p_lhs))(u_memord(u_anyP(_p_lhs), u_anyP(make$((TypeOf(_p_lhs))_p_rhs)))))
#define __op__prim_memordS(_s_lhs, _s_rhs...) u_castS$((TypeOf(_s_lhs))(u_memordS(u_anyS(_s_lhs), u_anyS(make$((TypeOf(_s_lhs.as_const))_s_rhs)))))
#define __op__prim_memcmp(_p_lhs, _p_rhs, _len...) memcmp(_p_lhs, _p_rhs, _len)
#define __op__prim_alloca(_bytes...) alloca(_bytes)
#define __op__prim_swap__step(_p_lhs, _p_rhs...) __op__prim_swap( \
    pp_uniqTok(p_lhs), pp_uniqTok(p_rhs), pp_uniqTok(tmp), _p_lhs, _p_rhs \
)
#define __op__prim_swap(__p_lhs, __p_rhs, __tmp, _p_lhs, _p_rhs...) ({ \
    let __p_lhs = _p_lhs; \
    let __p_rhs = _p_rhs; \
    let __tmp = *__p_lhs; \
    *__p_lhs = *__p_rhs; \
    *__p_rhs = __tmp; \
    {}; \
})

#define __op__prim_add(_lhs, _rhs...) ((_lhs) + (_rhs))
#define __op__prim_sub(_lhs, _rhs...) ((_lhs) - (_rhs))
#define __op__prim_mul(_lhs, _rhs...) ((_lhs) * (_rhs))
#define __op__prim_mulAdd(_x, _y, _z...) ((__x) * (__y) + (__z))
#define __op__prim_div(_lhs, _rhs...) ((_lhs) / (_rhs))

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

#define __op__flt_divTrunc(_x, _n...) flt_trunc(flt_div(__x, __n))
#define __op__flt_divRound(_x, _n...) flt_round(flt_div(__x, __n))
#define __op__flt_divFloor(_x, _n...) flt_floor(flt_div(__x, __n))
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
#define __op__int_rem(__x, __n, _x, _n...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__n, IntType) = _n; \
    debug_assert(__n != 0); \
    debug_assert(isIInt$(IntType) ? !(__x == int_limitMin$(IntType) && __n == as$(IntType)(-1)) : true); \
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
    __r < 0 ? __r + prim_abs(__n) : __r; \
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
    debug_assert(__n != 0.0); \
    _Generic( \
        FltType, \
        f32: __builtin_fmodf( \
            as$(f32)(__x), \
            as$(f32)(__n) \
        ), \
        f64: __builtin_fmod( \
            as$(f64)(__x), \
            as$(f64)(__n) \
        ) \
    ); \
})
#define __op__flt_remRound__step(_x, _n...) __op__flt_remRound(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__flt_remRound(__x, __n, _x, _n...) ({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__n, FltType) = _n; \
    debug_assert(__n != 0.0); \
    _Generic( \
        FltType, \
        f32: __builtin_remainderf( \
            as$(f32)(__x), \
            as$(f32)(__n) \
        ), \
        f64: __builtin_remainder( \
            as$(f64)(__x), \
            as$(f64)(__n) \
        ) \
    ); \
})
#define __op__flt_mod__step(_x, _n...) __op__flt_mod(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__flt_mod(__x, __n, _x, _n...) ({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
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

#define __op__prim_neg(_x...) (-(_x))
#define __op__prim_abs_static(_x...) (((_x) < 0) ? -(_x) : (_x))
#define __op__prim_abs__step(_x...) __op__prim_abs(pp_uniqTok(x), _x)
#define __op__prim_abs(__x, _x...) ({ \
    let __x = _x; \
    prim_abs_static(__x); \
})
#define __op__prim_sgn_static(_x...) ( \
    ((_x) < 0)   ? -1 \
    : (0 < (_x)) ? 1 \
                 : 0 \
)
#define __op__prim_sgn__step(_x...) __op__prim_sgn(pp_uniqTok(x), _x)
#define __op__prim_sgn(__x, _x...) ({ \
    let __x = _x; \
    prim_sgn_static(__x); \
})

#define __op__prim_not(_x...) (~(_x))
#define __op__prim_shl(_x, _y...) ((_x) << (_y))
#define __op__prim_shr(_x, _y...) ((_x) >> (_y))
#define __op__prim_and(_x, _y...) ((_x) & (_y))
#define __op__prim_xor(_x, _y...) ((_x) ^ (_y))
#define __op__prim_or(_x, _y...) ((_x) | (_y))

#define __op__prim_eql(_lhs, _rhs...) ((_lhs) == (_rhs))
#define __op__prim_ord_static(_lhs, _rhs...) ( \
    ((_lhs) < (_rhs))   ? cmp_Ord_lt \
    : ((_lhs) > (_rhs)) ? cmp_Ord_gt \
                        : cmp_Ord_eq \
)
#define __op__prim_ord__step(_lhs, _rhs...) __op__prim_ord(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__prim_ord(__lhs, __rhs, _lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    prim_ord_static(__lhs, __rhs); \
})
#define __op__prim_eq(_lhs, _rhs...) ((_lhs) == (_rhs))
#define __op__prim_ne(_lhs, _rhs...) ((_lhs) != (_rhs))
#define __op__prim_lt(_lhs, _rhs...) ((_lhs) < (_rhs))
#define __op__prim_gt(_lhs, _rhs...) ((_lhs) > (_rhs))
#define __op__prim_le(_lhs, _rhs...) ((_lhs) <= (_rhs))
#define __op__prim_ge(_lhs, _rhs...) ((_lhs) >= (_rhs))

#define __op__prim_min2_static(_lhs, _rhs...) ((_rhs) < (_lhs) ? (_rhs) : (_lhs))
#define __op__prim_min2__step(_lhs, _rhs...) __op__prim_min2( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs \
)
#define __op__prim_min2(__lhs, __rhs, _lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    prim_min2_static(__lhs, __rhs); \
})
#define __op__prim_min3_static(_1st, _2nd, _3rd...) \
    prim_min2_static(prim_min2_static(_1st, _2nd), _3rd)
#define __op__prim_min3__step(_1st, _2nd, _3rd...) __op__prim_min3( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), _1st, _2nd, _3rd \
)
#define __op__prim_min3(__1st, __2nd, __3rd, _1st, _2nd, _3rd...) ({ \
    let __1st = _1st; \
    let __2nd = _2nd; \
    let __3rd = _3rd; \
    prim_min3_static(__1st, __2nd, __3rd); \
})
#define __op__prim_min4_static(_1st, _2nd, _3rd, _4th...) \
    prim_min2_static(prim_min3_static(_1st, _2nd, _3rd), _4th)
#define __op__prim_min4__step(_1st, _2nd, _3rd, _4th...) __op__prim_min4( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), pp_uniqTok(4th), _1st, _2nd, _3rd, _4th \
)
#define __op__prim_min4(__1st, __2nd, __3rd, __4th, _1st, _2nd, _3rd, _4th...) ({ \
    let __1st = _1st; \
    let __2nd = _2nd; \
    let __3rd = _3rd; \
    let __4th = _4th; \
    prim_min4_static(__1st, __2nd, __3rd, __4th); \
})
#define __op__prim_findMin__step(__best, __vals...) __op__prim_findMin( \
    pp_uniqTok(best), pp_uniqTok(vals), pp_uniqTok(val), __best, __vals \
)
#define __op__prim_findMin(__best, __vals, __val, _best, _vals...) ({ \
    var __best = _best; \
    let __vals = A_from$((TypeOf(__best)){ _vals }); \
    for_(($s(A_ref(__vals)))(__val) { \
        __best = prim_min2_static(__best, *__val); \
    }); \
    __best; \
})
#define __op__prim_max2_static(_lhs, _rhs...) ((_rhs) > (_lhs) ? (_rhs) : (_lhs))
#define __op__prim_max2__step(_lhs, _rhs...) __op__prim_max2( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs \
)
#define __op__prim_max2(__lhs, __rhs, _lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    prim_max2_static(__lhs, __rhs); \
})
#define __op__prim_max3_static(_1st, _2nd, _3rd...) prim_max2_static(prim_max2_static(_1st, _2nd), _3rd)
#define __op__prim_max3__step(_1st, _2nd, _3rd...) __op__prim_max3( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), _1st, _2nd, _3rd \
)
#define __op__prim_max3(__1st, __2nd, __3rd, _1st, _2nd, _3rd...) ({ \
    let __1st = _1st; \
    let __2nd = _2nd; \
    let __3rd = _3rd; \
    prim_max3_static(__1st, __2nd, __3rd); \
})
#define __op__prim_max4_static(_1st, _2nd, _3rd, _4th...) prim_max2_static(prim_max3_static(_1st, _2nd, _3rd), _4th)
#define __op__prim_max4__step(_1st, _2nd, _3rd, _4th...) __op__prim_max4( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), pp_uniqTok(4th), _1st, _2nd, _3rd, _4th \
)
#define __op__prim_max4(__1st, __2nd, __3rd, __4th, _1st, _2nd, _3rd, _4th...) ({ \
    let __1st = _1st; \
    let __2nd = _2nd; \
    let __3rd = _3rd; \
    let __4th = _4th; \
    prim_max4_static(__1st, __2nd, __3rd, __4th); \
})
#define __op__prim_findMax__step(__best, __vals...) __op__prim_findMax( \
    pp_uniqTok(best), pp_uniqTok(vals), pp_uniqTok(val), __best, __vals \
)
#define __op__prim_findMax(__best, __vals, __val, _best, _vals...) ({ \
    var __best = _best; \
    let __vals = A_from$((TypeOf(__best)){ _vals }); \
    for_(($s(A_ref(__vals)))(__val) { \
        __best = prim_max2_static(__best, *__val); \
    }); \
    __best; \
})

#define __op__prim_clamp_static(_x, _lo, _hi...) prim_min2_static(prim_max2_static(_lo, _x), _hi)
#define __op__prim_clamp__step(_x, _lo, _hi...) __op__prim_clamp(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _x, _lo, _hi)
#define __op__prim_clamp(__x, __lo, __hi, _x, _lo, _hi...) ({ \
    let __x = _x; \
    let __lo = _lo; \
    let __hi = _hi; \
    claim_assert(__lo <= __hi); \
    prim_clamp_static(__x, __lo, __hi); \
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

#define __op__isBool$(_T...) _Generic(_T, bool: true, default: false)
#define __op__isUInt$(_T...) _Generic(_T, u8: true, u16: true, u32: true, u64: true, default: false)
#define __op__isIInt$(_T...) _Generic(_T, i8: true, i16: true, i32: true, i64: true, default: false)
#define __op__isInt$(_T...) (isUInt$(_T) || isIInt$(_T))
#define __op__isFlt$(_T...) _Generic(_T, f32: true, f64: true, default: false)

#define __op__uint_limit$(_T...) _Generic( \
    _T, \
    u8: u8_limit, \
    u16: u16_limit, \
    u32: u32_limit, \
    u64: u64_limit \
)
#define __op__int_limitMin$(_T...) _Generic( \
    _T, \
    u8: u8_limit_min, \
    u16: u16_limit_min, \
    u32: u32_limit_min, \
    u64: u64_limit_min, \
    i8: i8_limit_min, \
    i16: i16_limit_min, \
    i32: i32_limit_min, \
    i64: i64_limit_min \
)
#define __op__int_limitMax$(_T...) _Generic( \
    _T, \
    u8: u8_limit_max, \
    u16: u16_limit_max, \
    u32: u32_limit_max, \
    u64: u64_limit_max, \
    i8: i8_limit_max, \
    i16: i16_limit_max, \
    i32: i32_limit_max, \
    i64: i64_limit_max \
)
#define __op__iint_limitMaxNgtv$(_T...) _Generic( \
    _T, \
    i8: i8_limit_max_ngtv, \
    i16: i16_limit_max_ngtv, \
    i32: i32_limit_max_ngtv, \
    i64: i64_limit_max_ngtv \
)
#define __op__iint_limitMinNgtv$(_T...) _Generic( \
    _T, \
    i8: i8_limit_min_ngtv, \
    i16: i16_limit_min_ngtv, \
    i32: i32_limit_min_ngtv, \
    i64: i64_limit_min_ngtv \
)
#define __op__iint_limitMinPstv$(_T...) _Generic( \
    _T, \
    i8: i8_limit_min_pstv, \
    i16: i16_limit_min_pstv, \
    i32: i32_limit_min_pstv, \
    i64: i64_limit_min_pstv \
)
#define __op__iint_limitMaxPstv$(_T...) _Generic( \
    _T, \
    i8: i8_limit_max_pstv, \
    i16: i16_limit_max_pstv, \
    i32: i32_limit_max_pstv, \
    i64: i64_limit_max_pstv \
)

#define __op__int_bits$(_T...) _Generic( \
    _T, \
    u8: u8_bits, \
    u16: u16_bits, \
    u32: u32_bits, \
    u64: u64_bits \
)
#define __op__int_bytes$(_T...) _Generic( \
    _T, \
    u8: u8_bytes, \
    u16: u16_bytes, \
    u32: u32_bytes, \
    u64: u64_bytes \
)

#define __op__flt_limitMin$(_T...) _Generic(_T, f32: f32_limit_min, f64: f64_limit_min)
#define __op__flt_limitMax$(_T...) _Generic(_T, f32: f32_limit_max, f64: f64_limit_max)
#define __op__flt_limitMaxNgtv$(_T...) _Generic(_T, f32: f32_limit_max_ngtv, f64: f64_limit_max_ngtv)
#define __op__flt_limitMinNgtv$(_T...) _Generic(_T, f32: f32_limit_min_ngtv, f64: f64_limit_min_ngtv)
#define __op__flt_limitMinPstv$(_T...) _Generic(_T, f32: f32_limit_min_pstv, f64: f64_limit_min_pstv)
#define __op__flt_limitMaxPstv$(_T...) _Generic(_T, f32: f32_limit_max_pstv, f64: f64_limit_max_pstv)

#define __op__flt_bits$(_T...) _Generic(_T, f32: f32_bits, f64: f64_bits)
#define __op__flt_bytes$(_T...) _Generic(_T, f32: f32_bytes, f64: f64_bytes)

#define __op__flt_eps$(_T...) _Generic(_T, f32: f32_eps, f64: f64_eps)
#define __op__flt_nan$(_T...) _Generic(_T, f32: f32_nan, f64: f64_nan)
#define __op__flt_inf$(_T...) _Generic(_T, f32: f32_inf, f64: f64_inf)
#define __op__flt_infNgtv$(_T...) _Generic(_T, f32: f32_inf_ngtv, f64: f64_inf_ngtv)
#define __op__flt_infPstv$(_T...) _Generic(_T, f32: f32_inf_pstv, f64: f64_inf_pstv)

/*========== Integer Arithmetic Implementation ==============================*/

#define __op__int_add__step(_lhs, _rhs...) __op__int_add(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__int_add(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    debug_assert(!__builtin_add_overflow(__lhs, __rhs, &lit0$((IntType)))); \
    as$(IntType)(__lhs + __rhs); \
})
#define __op__int_sub__step(_lhs, _rhs...) __op__int_sub(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__int_sub(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    debug_assert(!__builtin_sub_overflow(__lhs, __rhs, &lit0$((IntType)))); \
    as$(IntType)(__lhs - __rhs); \
})
#define __op__int_mul__step(_lhs, _rhs...) __op__int_mul(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__int_mul(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    debug_assert(!__builtin_mul_overflow(__lhs, __rhs, &lit0$((IntType)))); \
    as$(IntType)(__lhs * __rhs); \
})
#define __op__int_mulAdd(_x, _y, _z...) int_add(int_mul(_x, _y), _z)
#define __op__int_div__step(_x, _n...) __op__int_div(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__int_div(__x, __n, _x, _n...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__n, IntType) = _n; \
    debug_assert(__n != 0); \
    debug_assert(isIInt$(IntType) ? !(__x == int_limitMin$(IntType) && __n == as$(IntType)(-1)) : true); \
    as$(IntType)(__x / __n); \
})

#define __op__iint_neg__step(_x...) __op__iint_neg(pp_uniqTok(x), _x)
#define __op__iint_neg(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _x; \
    debug_assert(__x != int_limitMin$(IIntType)); \
    as$(IIntType)(-__x); \
})
#define __op__iint_abs(_x...) _Generic( \
    TypeOfUnqual(_x), \
    i8: as$(i8)(__builtin_abs(as$(i8)(_x))), \
    i16: as$(i16)(__builtin_abs(as$(i16)(_x))), \
    i32: __builtin_abs(as$(i32)(_x)), \
    i64: __builtin_llabs(as$(i64)(_x)), \
)

/*========== Integer Comparison Implementation ==============================*/

#define __op__int_eq(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    (__lhs == __rhs); \
})
#define __op__int_ne(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    (__lhs != __rhs); \
})
#define __op__int_lt(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    (__lhs < __rhs); \
})
#define __op__int_gt(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    (__lhs > __rhs); \
})
#define __op__int_le(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    (__lhs <= __rhs); \
})
#define __op__int_ge(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    (__lhs >= __rhs); \
})

/*========== Integer Min/Max/Clamp Implementation ===========================*/

#define __op__int_min(__lhs, __rhs, _lhs, _rhs...) ({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    as$(IntType)(prim_min_static(__lhs, __rhs)); \
})
#define __op__int_max(__lhs, __rhs, _lhs, _rhs...) ({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    as$(IntType)(prim_max_static(__lhs, __rhs)); \
})
#define __op__int_clamp__step(_x, _lo, _hi...) __op__int_clamp(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _x, _lo, _hi)
#define __op__int_clamp(__x, __lo, __hi, _x, _lo, _hi...) ({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__lo, IntType) = _lo; \
    let_(__hi, IntType) = _hi; \
    claim_assert(__lo <= __hi); \
    int_min(int_max(__lo, __x), __hi); \
})


/*========== Bitwise Operations Implementation ==============================*/

#define __op__int_not(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    as$(IntType)(~__x); \
})
#define __op__int_shl(__x, __n, _x, _n...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__n, u32) = _n; \
    debug_assert(__n < int_bits$(IntType)); \
    as$(IntType)(__x << __n); \
})
#define __op__int_shr(__x, __n, _x, _n...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__n, u32) = _n; \
    debug_assert(__n < int_bits$(IntType)); \
    as$(IntType)(__x >> __n); \
})
#define __op__int_and(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    as$(IntType)(__lhs & __rhs); \
})
#define __op__int_xor(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    as$(IntType)(__lhs ^ __rhs); \
})
#define __op__int_or(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _lhs; \
    let_(__rhs, IntType) = _rhs; \
    as$(IntType)(__lhs | __rhs); \
})

/*========== Bit Manipulation Implementation ================================*/

#if UNUSED_CODE
#define __op__int_countOnes(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    _Generic(sizeof(IntType), 1: __builtin_popcount((u8)__x), 2: __builtin_popcount((u16)__x), 4: __builtin_popcount((u32)__x), 8: __builtin_popcountll((u64)__x)); \
})

#define __op__int_countZeros(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    (i32)(sizeof(IntType) * 8 - _Generic(sizeof(IntType), 1: __builtin_popcount((u8)__x), 2: __builtin_popcount((u16)__x), 4: __builtin_popcount((u32)__x), 8: __builtin_popcountll((u64)__x))); \
})

#define __op__int_countLeadingZeros(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    debug_assert(__x != 0); \
    _Generic(sizeof(IntType), 1: __builtin_clz((u32)(u8)__x) - 24, 2: __builtin_clz((u32)(u16)__x) - 16, 4: __builtin_clz((u32)__x), 8: __builtin_clzll((u64)__x)); \
})

#define __op__int_countTrailingZeros(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    debug_assert(__x != 0); \
    _Generic(sizeof(IntType), 1: __builtin_ctz((u32)(u8)__x), 2: __builtin_ctz((u32)(u16)__x), 4: __builtin_ctz((u32)__x), 8: __builtin_ctzll((u64)__x)); \
})

#define __op__int_rotateL(__x, __n, _x, _n...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__n, u32) = _n; \
    const u32 __mask = sizeof(IntType) * 8 - 1; \
    __n &= __mask; \
    as$(IntType)((__x << __n) | (__x >> ((-__n) & __mask))); \
})

#define __op__int_rotateR(__x, __n, _x, _n...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    let_(__n, u32) = _n; \
    const u32 __mask = sizeof(IntType) * 8 - 1; \
    __n &= __mask; \
    as$(IntType)((__x >> __n) | (__x << ((-__n) & __mask))); \
})

#define __op__int_swapBytes(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    as$(IntType)(_Generic(sizeof(IntType), 1: __x, 2: __builtin_bswap16((u16)__x), 4: __builtin_bswap32((u32)__x), 8: __builtin_bswap64((u64)__x))); \
})

#define __op__int_reverseBits(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    as$(IntType)(_Generic(sizeof(IntType), 1: __builtin_bitreverse8((u8)__x), 2: __builtin_bitreverse16((u16)__x), 4: __builtin_bitreverse32((u32)__x), 8: __builtin_bitreverse64((u64)__x))); \
})
#endif /* UNUSED_CODE */

/*========== Integer Exponential and Logarithm Implementation ===================*/

#define __op__int_exp2(_x...) (1ull << _x)
#define __op__int_log2(_x...) (as$(u32)((64u - 1u) - __builtin_clzll(_x)))

/*========== Integer Query Implementation ===================================*/

#define __op__int_isOdd(_x...) ((_x & 1) != 0)
#define __op__int_isEven(_x...) ((_x & 1) == 0)
#define __op__int_isPow2(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _x; \
    (__x > 0 && (__x & (__x - 1)) == 0); \
})
#define __op__int_hasSglBit(_x...) int_hasSglBit(_x)

/*========== Floating-Point Arithmetic Implementation =======================*/

#define __op__flt_add__step(_lhs, _rhs...) __op__flt_add(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__flt_add(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    as$(FltType)(__lhs + __rhs); \
})
#define __op__flt_sub__step(_lhs, _rhs...) __op__flt_sub(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__flt_sub(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    as$(FltType)(__lhs - __rhs); \
})
#define __op__flt_mul__step(_lhs, _rhs...) __op__flt_mul(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define __op__flt_mul(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    as$(FltType)(__lhs * __rhs); \
})
#define __op__flt_mulAdd__step(_x, _y, _z...) __op__flt_mulAdd(pp_uniqTok(x), pp_uniqTok(y), pp_uniqTok(z), _x, _y, _z)
#define __op__flt_mulAdd(__x, __y, __z, _x, _y, _z...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__y, FltType) = _y; \
    let_(__z, FltType) = _z; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_fmaf( \
                as$(f32)(__x), \
                as$(f32)(__y), \
                as$(f32)(__z) \
            ), \
            f64: __builtin_fma( \
                as$(f64)(__x), \
                as$(f64)(__y), \
                as$(f64)(__z) \
            ) \
        ) \
    ); \
})
#define __op__flt_div__step(_x, _n...) __op__flt_div(pp_uniqTok(x), pp_uniqTok(n), _x, _n)
#define __op__flt_div(__x, __n, _x, _n...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__n, FltType) = _n; \
    debug_assert(__n != 0); \
    as$(FltType)(__x / __n); \
})

#define __op__flt_neg(_x...) (-_x)
#define __op__flt_abs(_x...) _Generic( \
    TypeOfUnqual(_x), \
    f32: __builtin_fabsf(as$(f32)(_x)), \
    f64: __builtin_fabs(as$(f64)(_x)) \
)

/*========== Floating-Point Comparison Implementation =======================*/

#define __op__flt_eq(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    (__lhs == __rhs); \
})
#define __op__flt_ne(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    (__lhs != __rhs); \
})
#define __op__flt_lt(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    (__lhs < __rhs); \
})
#define __op__flt_gt(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    (__lhs > __rhs); \
})
#define __op__flt_le(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    (__lhs <= __rhs); \
})
#define __op__flt_ge(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    (__lhs >= __rhs); \
})

/*========== Floating-Point Min/Max/Clamp Implementation ====================*/

#define __op__flt_min(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_fminf( \
                as$(f32)(__lhs), \
                as$(f32)(__rhs) \
            ), \
            f64: __builtin_fmin( \
                as$(f64)(__lhs), \
                as$(f64)(__rhs) \
            ) \
        ) \
    ); \
})
#define __op__flt_max(__lhs, __rhs, _lhs, _rhs...) blk({ \
    typedef TypeOfUnqual(_lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _lhs; \
    let_(__rhs, FltType) = _rhs; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_fmaxf( \
                as$(f32)(__lhs), \
                as$(f32)(__rhs) \
            ), \
            f64: __builtin_fmax( \
                as$(f64)(__lhs), \
                as$(f64)(__rhs) \
            ) \
        ) \
    ); \
})
#define __op__flt_clamp__step(_x, _lo, _hi...) __op__flt_clamp(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _x, _lo, _hi)
#define __op__flt_clamp(__x, __lo, __hi, _x, _lo, _hi...) ({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__lo, FltType) = _lo; \
    let_(__hi, FltType) = _hi; \
    claim_assert(__lo <= __hi); \
    flt_min(flt_max(__lo, __x), __hi); \
})

/*========== Floating-Point Classification Implementation ===================*/

#define __op__flt_isNan(_x...) __builtin_isnan(_x)
#define __op__flt_isInf(_x...) __builtin_isinf(_x)
#define __op__flt_isFinite(_x...) __builtin_isfinite(_x)
#define __op__flt_isNormal(_x...) __builtin_isnormal(_x)

/*========== Floating-Point Rounding Implementation =========================*/

#define __op__flt_trunc(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_truncf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_trunc( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})
#define __op__flt_round(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_roundf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_round( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})
#define __op__flt_ceil(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_ceilf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_ceil( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})
#define __op__flt_floor(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_floorf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_floor( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

/*========== Floating-Point Power and Root Implementation ===================*/

#define __op__flt_sqrt(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_sqrtf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_sqrt( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_cbrt(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_cbrtf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_cbrt( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_sq(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)(__x * __x); \
})

#define __op__flt_cb(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)(__x * __x * __x); \
})

#define __op__flt_hypot(__x, __y, _x, _y...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__y, FltType) = _y; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_hypotf( \
                as$(f32)(__x), \
                as$(f32)(__y) \
            ), \
            f64: __builtin_hypot( \
                as$(f64)(__x), \
                as$(f64)(__y) \
            ) \
        ) \
    ); \
})

/*========== Floating-Point Exponential and Logarithm Implementation ========*/

#define __op__flt_exp(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_expf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_exp( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_exp2(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_exp2f( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_exp2( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_ln(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_logf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_log( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_log2(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_log2f( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_log2( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_log10(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_log10f( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_log10( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_powI(__x, __y, _x, _y...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__y, i32) = _y; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_powif( \
                as$(f32)(__x), \
                __y \
            ), \
            f64: __builtin_powi( \
                as$(f64)(__x), \
                __y \
            ) \
        ) \
    ); \
})

#define __op__flt_powF(__x, __y, _x, _y...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__y, FltType) = _y; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_powf( \
                as$(f32)(__x), \
                as$(f32)(__y) \
            ), \
            f64: __builtin_pow( \
                as$(f64)(__x), \
                as$(f64)(__y) \
            ) \
        ) \
    ); \
})

/*========== Floating-Point Trigonometric Implementation ====================*/

#define __op__flt_sin(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_sinf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_sin( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_cos(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_cosf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_cos( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_tan(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_tanf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_tan( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_asin(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_asinf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_asin( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_acos(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_acosf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_acos( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_atan(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_atanf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_atan( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_atan2(__y, __x, _y, _x...) blk({ \
    typedef TypeOfUnqual(_y) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__y, FltType) = _y; \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_atan2f( \
                as$(f32)(__y), \
                as$(f32)(__x) \
            ), \
            f64: __builtin_atan2( \
                as$(f64)(__y), \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

/*========== Floating-Point Hyperbolic Implementation =======================*/

#define __op__flt_sinh(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_sinhf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_sinh( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_cosh(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_coshf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_cosh( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_tanh(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_tanhf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_tanh( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_asinh(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_asinhf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_asinh( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_acosh(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_acoshf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_acosh( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#define __op__flt_atanh(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_atanhf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_atanh( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

/*========== Floating-Point IEEE 754 Implementation =========================*/

#define __op__flt_copySgn(__x, __y, _x, _y...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__y, FltType) = _y; \
    as$(FltType)(_Generic(FltType, f32: __builtin_copysignf(as$(f32)(__x), as$(f32)(__y)), f64: __builtin_copysign(as$(f64)(__x), as$(f64)(__y)))); \
})

#define __op__flt_nextAfter(__x, __y, _x, _y...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__y, FltType) = _y; \
    as$(FltType)(_Generic(FltType, f32: __builtin_nextafterf(as$(f32)(__x), as$(f32)(__y)), f64: __builtin_nextafter(as$(f64)(__x), as$(f64)(__y)))); \
})

#define __op__flt_scalb(__x, __n, _x, _n...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    let_(__n, i32) = _n; \
    as$(FltType)(_Generic(FltType, f32: __builtin_ldexpf(as$(f32)(__x), __n), f64: __builtin_ldexp(as$(f64)(__x), __n))); \
})

#define __op__flt_ilogb(__x, _x...) blk({ \
    typedef TypeOfUnqual(_x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _x; \
    as$(FltType)( \
        _Generic( \
            FltType, \
            f32: __builtin_ilogbf( \
                as$(f32)(__x) \
            ), \
            f64: __builtin_ilogb( \
                as$(f64)(__x) \
            ) \
        ) \
    ); \
})

#if NEXT_UPDATE
#define let_ignore \
    __attribute__((unused)) let pp_uniqTok(ignored)
#define let_ignore \
    __attribute__((unused)) pp_uniqTok(ignored) = (Void){}; \
    let_ignore
#define $ignore_void (void)

#define as$(/*(T)(Expr)*/...) pp_expand(pp_defer(block_inline__as$)(param_expand__as$ __VA_ARGS__))
#define param_expand__as$(...) __VA_ARGS__, pp_expand
#define block_inline__as$(_T, _Expr...) ((_T)(_Expr))

#define swap$(/*(T)(p_lhs, p_rhs)*/...) pp_expand(pp_defer(block_inline__swap$)(param_expand__swap$ __VA_ARGS__))
#define param_expand__swap$(...) __VA_ARGS__, pp_expand
#define block_inline__swap$(_T, _p_lhs, _p_rhs) ({ \
    p$(_T) __lhs = (__p_lhs); \
    p$(_T) __rhs = (__p_rhs); \
    _T __tmp = *__lhs; \
    *__lhs = *__rhs; \
    *__rhs = __tmp; \
    ({}); \
})

#define min$(/*(T)(lhs, rhs)*/...) pp_expand(pp_defer(block_inline__min$)(param_expand__min$ __VA_ARGS__))
#define param_expand__min$(...) __VA_ARGS__, pp_expand
#define block_inline__min$(_T, _lhs, _rhs) ({ \
    const _T __lhs = (_lhs); \
    const _T __rhs = (_rhs); \
    __rhs < __lhs ? __rhs : __lhs; \
})
#define max$(/*(T)(lhs, rhs)*/...) pp_expand(pp_defer(block_inline__max$)(param_expand__max$ __VA_ARGS__))
#define param_expand__max$(...) __VA_ARGS__, pp_expand
#define block_inline__max$(_T, _lhs, _rhs) ({ \
    const _T __lhs = (_lhs); \
    const _T __rhs = (_rhs); \
    __rhs > __lhs ? __rhs : __lhs; \
})
#endif /* NEXT_UPDATE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_prim__included */

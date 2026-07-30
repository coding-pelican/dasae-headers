/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    pri.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)/core/pri
 * @prefix  (none)
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
#pragma once
#ifndef core_pri__included
#define core_pri__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pri/cfg.h"
#include "pri/enum.h"
#include "pri/switch.h"
#include "pri/bitfield.h"

/*========== Type Utilities =================================================*/

#define isValidAlign_static(_$align /*: usize*/... /*(bool)*/) ____isValidAlign_static(_$align)
#define isValidAlign(_$align /*: usize*/... /*(bool)*/) __step__isValidAlign(_$align)

#define isAligned_static(_$addr /*: usize*/, _$align /*: usize*/... /*(bool)*/) ____isAligned_static(_$addr, _$align)
#define isAligned(_$addr /*: usize|PtrType*/, _$align /*: usize*/... /*(bool)*/) __step__isAligned(_$addr, _$align)
#define isAlignedLog2_static(_$addr /*: usize*/, _$log2_align /*: u8*/... /*(bool)*/) ____isAlignedLog2_static(_$addr, _$log2_align)
#define isAlignedLog2(_$addr /*: usize|PtrType*/, _$log2_align /*: u8*/... /*(bool)*/) __step__isAlignedLog2(_$addr, _$log2_align)

#define alignFwd_static(_$addr /*: usize*/, _$align /*: usize*/... /*(usize)*/) ____alignFwd_static(_$addr, _$align)
#define alignFwd(_$addr /*: usize|PtrType*/, _$align /*: usize*/... /*(usize)*/) __step__alignFwd(_$addr, _$align)
#define alignFwdLog2_static(_$addr /*: usize*/, _$log2_align /*: u8*/... /*(usize)*/) ____alignFwdLog2_static(_$addr, _$log2_align)
#define alignFwdLog2(_$addr /*: usize|PtrType*/, _$log2_align /*: u8*/... /*(usize)*/) __step__alignFwdLog2(_$addr, _$log2_align)
#define alignBwd_static(_$addr /*: usize*/, _$align /*: usize*/... /*(usize)*/) ____alignBwd_static(_$addr, _$align)
#define alignBwd(_$addr /*: usize|PtrType*/, _$align /*: usize*/... /*(usize)*/) __step__alignBwd(_$addr, _$align)
#define alignBwdLog2_static(_$addr /*: usize*/, _$log2_align /*: u8*/... /*(usize)*/) ____alignBwdLog2_static(_$addr, _$log2_align)
#define alignBwdLog2(_$addr /*: usize|PtrType*/, _$log2_align /*: u8*/... /*(usize)*/) __step__alignBwdLog2(_$addr, _$log2_align)

#define alignToLog2_static(_$align /*: usize*/... /*(u8)*/) ____alignToLog2_static(_$align)
#define log2FromAlign_static __alias__log2FromAlign_static
#define alignToLog2(_$align /*: usize*/... /*(u8)*/) __step__alignToLog2(_$align)
#define log2FromAlign __alias__log2FromAlign
#define log2ToAlign_static(_$log2_align /*: u8*/... /*(usize)*/) ____log2ToAlign_static(_$log2_align)
#define alignFromLog2_static __alias__alignFromLog2_static
#define log2ToAlign(_$log2_align /*: u8*/... /*(usize)*/) __step__log2ToAlign(_$log2_align)
#define alignFromLog2 __alias__alignFromLog2
#define alignCast(/*(_$log2_align: u8)(_ptr: PtrType)*/... /*(_$T)*/) __step__alignCast(__VA_ARGS__)

#define bitCast$(/*(_$T)(_$val)*/... /*(_$T)*/) __step__bitCast$(__VA_ARGS__)
#define intToBool(_$val /*: IntType*/... /*(bool)*/) __step__intToBool(_$val)
#define boolFromInt __alias__boolFromInt
#define boolToInt(_$val /*: bool*/... /*(u8)*/) ____boolToInt(_$val)
#define intFromBool __alias__intFromBool
#define intCast$(/*(_$T: IntType)(_$val: IntType)*/... /*(_$T)*/) __step__intCast$(__VA_ARGS__)
#define intToFlt$(/*(_$T: FltType)(_$val: IntType)*/... /*(_$T)*/) __step__intToFlt$(__VA_ARGS__)
#define fltFromInt$ __alias__fltFromInt$
#define fltToInt$(/*(_$T: IntType)(_$val: FltType)*/... /*(_$T)*/) __step__fltToInt$(__VA_ARGS__)
#define intFromFlt$ __alias__intFromFlt$
#define fltCast$(/*(_$T: FltType)(_$val: FltType)*/... /*(_$T)*/) __step__fltCast$(__VA_ARGS__)

/*========== Memory Operations ==============================================*/

#define pri_memset0(_$s_dst...) __op__pri_memset0(_$s_dst)
#define pri_memset0S(_$s_dst...) __op__pri_memset0S(_$s_dst)
#define pri_memset(_$p_dst, _$v_src...) __op__pri_memset(_$p_dst, _$v_src)
#define pri_memsetS(_$s_dst, _$v_src...) __op__pri_memsetS(_$s_dst, _$v_src)
#define pri_memcpy(_$p_dst, _$p_src...) __op__pri_memcpy(_$p_dst, _$p_src)
#define pri_memcpyS(_$s_dst, _$s_src...) __op__pri_memcpyS(_$s_dst, _$s_src)
#define pri_memmove(_$p_dst, _$p_src...) __op__pri_memmove(_$p_dst, _$p_src)
#define pri_memmoveS(_$s_dst, _$s_src...) __op__pri_memmoveS(_$s_dst, _$s_src)
#define pri_memeql(_$p_lhs, _$p_rhs...) __op__pri_memeql(_$p_lhs, _$p_rhs)
#define pri_memeqlS(_$s_lhs, _$s_rhs...) __op__pri_memeqlS(_$s_lhs, _$s_rhs)
#define pri_memord(_$p_lhs, _$p_rhs...) __op__pri_memord(_$p_lhs, _$p_rhs)
#define pri_memordS(_$s_lhs, _$s_rhs...) __op__pri_memordS(_$s_lhs, _$s_rhs)
#define pri_alloca(_$len_bytes...) __op__pri_alloca(_$len_bytes)
#define pri_allocaAlign(_$len_bytes, _$log2_align...) __op__pri_allocaAlign(_$len_bytes, _$log2_align)
#define pri_swap(_$p_lhs, _$p_rhs...) __step__pri_swap(_$p_lhs, _$p_rhs)
#define pri_swapS(_$s_lhs, _$s_rhs...) __step__pri_swapS(_$s_lhs, _$s_rhs)

/*========== Arithmetic Operations ==========================================*/

#define isZero(_$x...) __op__isZero(_$x)
#define isNonzero(_$x...) __op__isNonzero(_$x)

#define pri_add(_$lhs, _$rhs...) __op__pri_add(_$lhs, _$rhs)
#define pri_sub(_$lhs, _$rhs...) __op__pri_sub(_$lhs, _$rhs)
#define pri_mul(_$lhs, _$rhs...) __op__pri_mul(_$lhs, _$rhs)
#define pri_mulAdd(_$x, _$y, _$z...) __op__pri_mulAdd(_$x, _$y, _$z)
#define pri_div(_$lhs, _$rhs...) __op__pri_div(_$lhs, _$rhs)
#define pri_rem(_$x, _$n...) __op__pri_rem(_$x, _$n)

#define pri_neg(_$x...) __op__pri_neg(_$x)
#define pri_abs_static(_$x...) __op__pri_abs_static(_$x)
#define pri_abs(_$x...) __op__pri_abs__step(_$x)
#define pri_sgn_static(_$x...) __op__pri_sgn_static(_$x)
#define pri_sgn(_$x...) __op__pri_sgn__step(_$x)

#define pri_not(_$x...) __op__pri_not(_$x)
#define pri_and(_$x, _$y...) __op__pri_and(_$x, _$y)
#define pri_or(_$x, _$y...) __op__pri_or(_$x, _$y)

#define pri_eql(_$lhs, _$rhs... /*(bool)*/) __op__pri_eql(_$lhs, _$rhs)
#define pri_neq(_$lhs, _$rhs... /*(bool)*/) __op__pri_neq(_$lhs, _$rhs)

#define pri_ord_static(_$lhs, _$rhs... /*(cmp_Ord)*/) __op__pri_ord_static(_$lhs, _$rhs)
#define pri_ord(_$lhs, _$rhs... /*(cmp_Ord)*/) __op__pri_ord__step(_$lhs, _$rhs)
#define pri_eq(_$lhs, _$rhs... /*(bool)*/) __op__pri_eq(_$lhs, _$rhs)
#define pri_ne(_$lhs, _$rhs... /*(bool)*/) __op__pri_ne(_$lhs, _$rhs)
#define pri_lt(_$lhs, _$rhs... /*(bool)*/) __op__pri_lt(_$lhs, _$rhs)
#define pri_gt(_$lhs, _$rhs... /*(bool)*/) __op__pri_gt(_$lhs, _$rhs)
#define pri_le(_$lhs, _$rhs... /*(bool)*/) __op__pri_le(_$lhs, _$rhs)
#define pri_ge(_$lhs, _$rhs... /*(bool)*/) __op__pri_ge(_$lhs, _$rhs)

#define pri_min_static(_$lhs, _$rhs...) __op__pri_min2_static(_$lhs, _$rhs)
#define pri_min(_$lhs, _$rhs...) __op__pri_min2__step(_$lhs, _$rhs)
#define pri_min2_static(_$lhs, _$rhs...) __op__pri_min2_static(_$lhs, _$rhs)
#define pri_min2(_$lhs, _$rhs...) __op__pri_min2__step(_$lhs, _$rhs)
#define pri_min3_static(_$1st, _$2nd, _$3rd...) __op__pri_min3_static(_$1st, _$2nd, _$3rd)
#define pri_min3(_$1st, _$2nd, _$3rd...) __op__pri_min3__step(_$1st, _$2nd, _$3rd)
#define pri_min4_static(_$1st, _$2nd, _$3rd, _$4th...) __op__pri_min4_static(_$1st, _$2nd, _$3rd, _$4th)
#define pri_min4(_$1st, _$2nd, _$3rd, _$4th...) __op__pri_min4__step(_$1st, _$2nd, _$3rd, _$4th)
#define pri_min5_static(_$1st, _$2nd, _$3rd, _$4th, _$5th...) __op__pri_min5_static(_$1st, _$2nd, _$3rd, _$4th, _$5th)
#define pri_min5(_$1st, _$2nd, _$3rd, _$4th, _$5th...) __op__pri_min5__step(_$1st, _$2nd, _$3rd, _$4th, _$5th)
#define pri_findMin(_$vals...) __op__pri_findMin__step(_$vals)

#define pri_max_static(_$lhs, _$rhs...) __op__pri_max2_static(_$lhs, _$rhs)
#define pri_max(_$lhs, _$rhs...) __op__pri_max2__step(_$lhs, _$rhs)
#define pri_max2_static(_$lhs, _$rhs...) __op__pri_max2_static(_$lhs, _$rhs)
#define pri_max2(_$lhs, _$rhs...) __op__pri_max2__step(_$lhs, _$rhs)
#define pri_max3_static(_$1st, _$2nd, _$3rd...) __op__pri_max3_static(_$1st, _$2nd, _$3rd)
#define pri_max3(_$1st, _$2nd, _$3rd...) __op__pri_max3__step(_$1st, _$2nd, _$3rd)
#define pri_max4_static(_$1st, _$2nd, _$3rd, _$4th...) __op__pri_max4_static(_$1st, _$2nd, _$3rd, _$4th)
#define pri_max4(_$1st, _$2nd, _$3rd, _$4th...) __op__pri_max3__step(_$1st, _$2nd, _$3rd, _$4th)
#define pri_max5_static(_$1st, _$2nd, _$3rd, _$4th, _$5th...) __op__pri_max5_static(_$1st, _$2nd, _$3rd, _$4th, _$5th)
#define pri_max5(_$1st, _$2nd, _$3rd, _$4th, _$5th...) __op__pri_max5__step(_$1st, _$2nd, _$3rd, _$4th, _$5th)
#define pri_findMax(_$vals...) __op__pri_findMax__step(_$vals)

#define pri_clamp_static(_$x, _$lo /*$incl*/, _$hi /*$incl*/...) __op__pri_clamp_static(_$x, _$lo, _$hi)
#define pri_clamp(_$x, _$lo /*$incl*/, _$hi /*$incl*/...) __op__pri_clamp__step(_$x, _$lo, _$hi)

/*========== Boolean Operations =============================================*/

#define bool_any(_$vals...) __op__bool_any__step(_$vals)
#define bool_all(_$vals...) __op__bool_all__step(_$vals)
#define bool_none(_$vals...) __op__bool_none__step(_$vals)

/*========== Integer Classification Operations =============================*/

#define int_isZero(_$x...) __op__int_isZero(_$x)
#define int_isNonzero(_$x...) __op__int_isNonzero(_$x)
#define int_isOdd(_$x...) __op__int_isOdd(_$x)
#define int_isEven(_$x...) __op__int_isEven(_$x)
#define int_isPow2_static(_$x...) ____int_isPow2_static(_$x)
#define int_isPow2(_$x...) __step__int_isPow2(_$x)
#define int_hasSglBit_static(_$x...) ____int_hasSglBit_static(_$x)
#define int_hasSglBit(_$x...) __step__int_hasSglBit(_$x)

/*========== Integer Arithmetic Operations ==================================*/

#define int_add(_$lhs, _$rhs...) __op__int_add__step(_$lhs, _$rhs)
#define int_addOverflow(_$lhs, _$rhs, _$p_out...) __op__int_addOverflow( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), pp_uniqTok(overflow), _$lhs, _$rhs, _$p_out \
)
#define int_addWrap(_$lhs, _$rhs...) __op__int_addWrap(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), _$lhs, _$rhs)
#define int_addSat(_$lhs, _$rhs...) __op__int_addSat(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), _$lhs, _$rhs)
#define uint_addCarry(_$lhs, _$rhs, _carry_in, _p_carry_out...) __op__uint_addCarry( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(carry_in), _$lhs, _$rhs, _carry_in, _p_carry_out \
)
#define int_sub(_$lhs, _$rhs...) __op__int_sub__step(_$lhs, _$rhs)
#define int_subOverflow(_$lhs, _$rhs, _$p_out...) __op__int_subOverflow( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), pp_uniqTok(overflow), _$lhs, _$rhs, _$p_out \
)
#define int_subWrap(_$lhs, _$rhs...) __op__int_subWrap(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), _$lhs, _$rhs)
#define int_subSat(_$lhs, _$rhs...) __op__int_subSat(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), _$lhs, _$rhs)
#define uint_subBorrow(_$lhs, _$rhs, _borrow_in, _p_borrow_out...) __op__uint_subBorrow( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(borrow_in), _$lhs, _$rhs, _borrow_in, _p_borrow_out \
)
#define int_mul(_$lhs, _$rhs...) __op__int_mul__step(_$lhs, _$rhs)
#define int_mulOverflow(_$lhs, _$rhs, _$p_out...) __op__int_mulOverflow( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), pp_uniqTok(overflow), _$lhs, _$rhs, _$p_out \
)
#define int_mulWrap(_$lhs, _$rhs...) __op__int_mulWrap(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), _$lhs, _$rhs)
#define int_mulSat(_$lhs, _$rhs...) __op__int_mulSat(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(out), _$lhs, _$rhs)
#define int_mulAdd(_$x, _$y, _$z...) __op__int_mulAdd(_$x, _$y, _$z)
#define int_div(_$x, _$n...) __op__int_div__step(_$x, _$n)
#define int_divTrunc(_$x, _$n...) __op__int_div__step(_$x, _$n)
#define int_divRound(_$x, _$n...) __op__int_divRound__step(_$x, _$n)
#define iint_divFloor(_$x, _$n...) __op__iint_divFloor__step(_$x, _$n)
#define iint_divEuclid(_$x, _$n...) __op__iint_divEuclid__step(_$x, _$n)
#define int_divCeil(_$x, _$n...) __op__int_divCeil__step(_$x, _$n)
#define int_rem_static(_$x, _$n...) __op__int_rem_static(_$x, _$n)
#define int_rem(_$x, _$n...) __op__int_rem__step(_$x, _$n)
#define int_remRound(_$x, _$n...) __op__int_remRound__step(_$x, _$n)
#define iint_mod(_$x, _$n...) __op__iint_mod__step(_$x, _$n)
#define iint_modEuclid(_$x, _$n...) __op__iint_modEuclid__step(_$x, _$n)
#define int_modCeil(_$x, _$n...) __op__int_modCeil__step(_$x, _$n)

#define iint_neg(_$x...) __op__iint_neg__step(_$x)
#define iint_negOverflow(_$x, _$p_out...) __op__iint_negOverflow( \
    pp_uniqTok(x), pp_uniqTok(out), pp_uniqTok(overflow), _$x, _$p_out \
)
#define iint_negWrap(_$x...) __op__iint_negWrap(pp_uniqTok(x), pp_uniqTok(out), _$x)
#define iint_negSat(_$x...) __op__iint_negSat(pp_uniqTok(x), pp_uniqTok(out), _$x)
#define iint_abs(_$x...) __op__iint_abs(_$x)
#define iint_absOverflow(_$x, _$p_out...) __op__iint_absOverflow( \
    pp_uniqTok(x), pp_uniqTok(out), pp_uniqTok(overflow), _$x, _$p_out \
)
#define iint_absSat(_$x...) __op__iint_absSat(pp_uniqTok(x), pp_uniqTok(out), _$x)
#define iint_sgn(_$x...) ____iint_sgn(_$x)
#define iint_sgnBit_static(_$x...) ____iint_sgnBit_static(_$x)
#define iint_sgnBit(_$x...) __step__iint_sgnBit(_$x)

/*========== Integer Comparison Operations ==================================*/

#define int_eql(_$lhs, _$rhs...) __op__int_eql(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define int_neq(_$lhs, _$rhs...) __op__int_neq(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)

#define int_ord(_$lhs, _$rhs... /*(cmp_Ord)*/) __op__int_ord__step(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define int_eq(_$lhs, _$rhs...) __op__int_eq(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define int_ne(_$lhs, _$rhs...) __op__int_ne(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define int_lt(_$lhs, _$rhs...) __op__int_lt(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define int_gt(_$lhs, _$rhs...) __op__int_gt(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define int_le(_$lhs, _$rhs...) __op__int_le(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define int_ge(_$lhs, _$rhs...) __op__int_ge(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)

/*========== Integer Min/Max/Clamp Operations ===============================*/

#define int_min(_$lhs, _$rhs...) __op__int_min(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define int_max(_$lhs, _$rhs...) __op__int_max(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define int_clamp(_$x, _$lo /*$incl*/, _$hi /*$incl*/...) __op__int_clamp__step(_$x, _$lo, _$hi)
#define int_wrap(_$x, _$lo /*$incl*/, _$hi /*$excl*/...) __op__int_wrap__step(_$x, _$lo, _$hi)

/*========== Bitwise Operations =============================================*/

#define int_not_static(_$x /*: IntType*/... /*(IntType)*/) __op__int_not_static(_$x)
#define int_not(_$x /*: IntType*/... /*(IntType)*/) __step__int_not(_$x)
#define int_shl_static(_$x /*: IntType*/, _$y /*: u32*/... /*(IntType)*/) __op__int_shl_static(_$x, _$y)
#define int_shl(_$x /*: IntType*/, _$n /*: u32*/... /*(IntType)*/) __step__int_shl(_$x, _$n)
#define int_shr_static(_$x /*: IntType*/, _$y /*: u32*/... /*(IntType)*/) __op__int_shr_static(_$x, _$y)
#define int_shr(_$x /*: IntType*/, _$n /*: u32*/... /*(IntType)*/) __step__int_shr(_$x, _$n)
#define int_and_static(_$x /*: IntType*/, _$y /*: IntType*/... /*(IntType)*/) __op__int_and_static(_$x, _$y)
#define int_and(_$lhs /*: IntType*/, _$rhs /*: IntType*/... /*(IntType)*/) __step__int_and(_$lhs, _$rhs)
#define int_xor_static(_$x /*: IntType*/, _$y /*: IntType*/... /*(IntType)*/) __op__int_xor_static(_$x, _$y)
#define int_xor(_$lhs /*: IntType*/, _$rhs /*: IntType*/... /*(IntType)*/) __step__int_xor(_$lhs, _$rhs)
#define int_or_static(_$x /*: IntType*/, _$y /*: IntType*/... /*(IntType)*/) __op__int_or_static(_$x, _$y)
#define int_or(_$lhs /*: IntType*/, _$rhs /*: IntType*/... /*(IntType)*/) __step__int_or(_$lhs, _$rhs)

#define int_setBit_static(_$x /*: IntType*/, _$bit /*: u32*/... /*(IntType)*/) __op__int_setBit_static(_$x, _$bit)
#define int_setBit(_$x /*: IntType*/, _$bit /*: u32*/... /*(IntType)*/) __op__int_setBit(_$x, _$bit)
#define int_resetBit_static(_$x /*: IntType*/, _$bit /*: u32*/... /*(IntType)*/) __op__int_resetBit_static(_$x, _$bit)
#define int_resetBit(_$x /*: IntType*/, _$bit /*: u32*/... /*(IntType)*/) __op__int_resetBit(_$x, _$bit)
#define int_toggleBit_static(_$x /*: IntType*/, _$bit /*: u32*/... /*(IntType)*/) __op__int_toggleBit_static(_$x, _$bit)
#define int_toggleBit(_$x /*: IntType*/, _$bit /*: u32*/... /*(IntType)*/) __op__int_toggleBit(_$x, _$bit)

/// Lower N bits `[0,N)`: (1 << n) - 1
/// Usage: bits_maskLo((u16)(4)) -> 0x000F
#define int_maskLo_static$(/*(_$T: IntType)(_$n)*/... /*(_$T)*/) __step__int_maskLo_static$(__VA_ARGS__)
#define int_maskLo$(/*(_$T: IntType)(_$n)*/... /*(_$T)*/) __step__int_maskLo$(__VA_ARGS__)
/// Upper N bits `[Width-N,Width)`: (~0) << (width - n)
/// Usage: bits_maskHi((u16)(4)) -> 0xF000
#define int_maskHi_static$(/*(_$T: IntType)(_$n)*/... /*(_$T)*/) __step__int_maskHi_static$(__VA_ARGS__)
#define int_maskHi$(/*(_$T: IntType)(_$n)*/... /*(_$T)*/) __step__int_maskHi$(__VA_ARGS__)
/// Arbitrary range [Offset,Offset+Length): maskLo(len) << off
/// Usage: bits_mask((u16)(4, 4)) -> 0x00F0 (offset 4, length 4)
#define int_mask_static$(/*(_$T: IntType)(_$off, _$len)*/... /*(_$T)*/) __step__int_mask_static$(__VA_ARGS__)
#define int_mask$(/*(_$T: IntType)(_$off, _$len)*/... /*(_$T)*/) __step__int_mask$(__VA_ARGS__)

/*========== Bit Manipulation Operations ====================================*/

#define int_countOnes_static(_$x... /*(u32)*/) ____int_countOnes_static(_$x)
#define int_countOnes(_$x... /*(u32)*/) __step__int_countOnes(_$x)
#define int_countZeros_static(_$x... /*(u32)*/) ____int_countZeros_static(_$x)
#define int_countZeros(_$x... /*(u32)*/) __step__int_countZeros(_$x)

#define iint_leadingRedundantSgnBits_static(_$x... /*(u32)*/) ____iint_leadingRedundantSgnBits_static(_$x)
#define iint_leadingRedundantSgnBits(_$x... /*(u32)*/) __step__iint_leadingRedundantSgnBits(_$x)
#define int_leadingOnes_static(_$x... /*(u32)*/) ____int_leadingOnes_static(_$x)
#define int_leadingOnes(_$x... /*(u32)*/) __step__int_leadingOnes(_$x)
#define int_leadingZeros_static(_$x... /*(u32)*/) ____int_leadingZeros_static(_$x)
#define int_leadingZeros(_$x... /*(u32)*/) __step__int_leadingZeros(_$x)
#define int_trailingOnes_static(_$x... /*(u32)*/) ____int_trailingOnes_static(_$x)
#define int_trailingOnes(_$x... /*(u32)*/) __step__int_trailingOnes(_$x)
#define int_trailingZeros_static(_$x... /*(u32)*/) ____int_trailingZeros_static(_$x)
#define int_trailingZeros(_$x... /*(u32)*/) __step__int_trailingZeros(_$x)

#define int_firstSetBit_static(_$x... /*(u32)*/) ____int_firstSetBit_static(_$x)
#define int_firstSetBit(_$x... /*(u32)*/) __step__int_firstSetBit(_$x)
#define int_lastSetBit_static(_$x... /*(u32)*/) ____int_lastSetBit_static(_$x)
#define int_lastSetBit(_$x... /*(u32)*/) __step__int_lastSetBit(_$x)
#define int_parity_static(_$x... /*(bool)*/) ____int_parity_static(_$x)
#define int_parity(_$x... /*(bool)*/) __step__int_parity(_$x)

#define int_bitWidth_static(_$x... /*(u32)*/) ____int_bitWidth_static(_$x)
#define int_bitWidth(_$x... /*(u32)*/) __step__int_bitWidth(_$x)
#define int_bitFloor_static(_$x... /*(IntType)*/) ____int_bitFloor_static(_$x)
#define int_bitFloor(_$x... /*(IntType)*/) __step__int_bitFloor(_$x)
#define int_bitCeil_static(_$x... /*(IntType)*/) ____int_bitCeil_static(_$x)
#define int_bitCeil(_$x... /*(IntType)*/) __step__int_bitCeil(_$x)

#define int_swapBytes_static(_$x /*: IntType*/... /*(IntType)*/) ____int_swapBytes_static(_$x)
#define int_swapBytes(_$x /*: IntType*/... /*(IntType)*/) __step__int_swapBytes(_$x)
#define int_rotateLeft_static(_$x /*: IntType*/, _$y /*: IntType*/... /*(IntType)*/) ____int_rotateLeft_static(_$x, _$y)
#define int_rotateLeft(_$x /*: IntType*/, _$y /*: IntType*/... /*(IntType)*/) __step__int_rotateLeft(_$x, _$y)
#define int_rotateRight_static(_$x /*: IntType*/, _$y /*: IntType*/... /*(IntType)*/) ____int_rotateRight_static(_$x, _$y)
#define int_rotateRight(_$x /*: IntType*/, _$y /*: IntType*/... /*(IntType)*/) __step__int_rotateRight(_$x, _$y)
#define int_reverseBits_static(_$x /*: IntType*/... /*(IntType)*/) ____int_reverseBits_static(_$x)
#define int_reverseBits(_$x /*: IntType*/... /*(IntType)*/) __step__int_reverseBits(_$x)

#define int_hasBit(_$x, _$bit... /*(bool)*/) __op__int_hasBit(pp_uniqTok(x), pp_uniqTok(bit), _$x, _$bit)
#define int_extractBits(_$x, _$off, _$len... /*(IntType)*/) __op__int_extractBits( \
    pp_uniqTok(x), pp_uniqTok(off), pp_uniqTok(len), _$x, _$off, _$len \
)
#define int_replaceBits(_$x, _$off, _$len, _$bits... /*(IntType)*/) __op__int_replaceBits( \
    pp_uniqTok(x), pp_uniqTok(off), pp_uniqTok(len), pp_uniqTok(bits), _$x, _$off, _$len, _$bits \
)

/*========== Integer Exponential and Logarithm Operations ===================*/

#define uint_exp10_static$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) \
    /* \
     * calculate 10^(_$n) \
     */ \
    __step__uint_exp10_static$(__VA_ARGS__)
#define uint_exp10$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) __step__uint_exp10$(__VA_ARGS__)
#define uint_exp2_static$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) \
    /* \
     * calculate 2^(_$n) \
     */ \
    __step__uint_exp2_static$(__VA_ARGS__)
#define uint_exp2$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) __step__uint_exp2$(__VA_ARGS__)
#define uint_pow2_static$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) \
    /* \
     * calculate 2^(_$n) \
     */ \
    __step__uint_pow2_static$(__VA_ARGS__)
#define uint_pow2$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) __step__uint_pow2$(__VA_ARGS__)
#define uint_pow2Round_static$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) \
    /* \
     * calculate round(2^(_$n)) \
     */ \
    __step__uint_pow2Round_static$(__VA_ARGS__)
#define uint_pow2Round$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) __step__uint_pow2Round$(__VA_ARGS__)
#define uint_pow2Floor_static$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) \
    /* \
     * calculate floor(2^(_$n)) \
     */ \
    __step__uint_pow2Floor_static$(__VA_ARGS__)
#define uint_pow2Floor$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) __step__uint_pow2Floor$(__VA_ARGS__)
#define uint_pow2Ceil_static$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) \
    /* \
     * calculate ceil(2^(_$n)) \
     */ \
    __step__uint_pow2Ceil_static$(__VA_ARGS__)
#define uint_pow2Ceil$(/*(_$T)(_$n: UIntType)*/... /*(_$T)*/) __step__uint_pow2Ceil$(__VA_ARGS__)
#define uint_log2_static(_$n /*: UIntType*/... /*(u32)*/) \
    /* \
     * calculate log2(_$n) \
     */ \
    __step__uint_log2_static(_$n)
#define uint_log2(_$n /*: UIntType*/... /*(u32)*/) __step__uint_log2(_$n)
#define uint_log10Round_static(_$n /*: UIntType*/... /*(u32)*/) \
    /* \
     * calculate round(log10(_$n)) \
     * \
     * Thresholds are at 10^(k.5). \
     * sqrt(10) ~ 3.16227766 \
     */ \
    __step__uint_log10Round_static(_$n)
#define uint_log10Round(_$n /*: UIntType*/... /*(u32)*/) __step__uint_log10Round(_$n)
#define uint_log10Floor_static(_$n /*: UIntType*/... /*(u32)*/) \
    /* \
     * calculate floor(log10(_$n)) \
     */ \
    __step__uint_log10Floor_static(_$n)
#define uint_log10Floor(_$n /*: UIntType*/... /*(u32)*/) __step__uint_log10Floor(_$n)
#define uint_log10Ceil_static(_$n /*: UIntType*/... /*(u32)*/) \
    /* \
     * calculate ceil(log10(_$n)) \
     */ \
    __step__uint_log10Ceil_static(_$n)
#define uint_log10Ceil(_$n /*: UIntType*/... /*(u32)*/) __step__uint_log10Ceil(_$n)

/*========== Floating-Point Classification ==================================*/

#define flt_isNaN(_$x...) ____flt_isNaN(_$x)
#define flt_isInf(_$x...) ____flt_isInf(_$x)
#define flt_isFinite(_$x...) ____flt_isFinite(_$x)
#define flt_isNormal(_$x...) ____flt_isNormal(_$x)
#define flt_isSubnormal(_$x...) ____flt_isSubnormal(_$x)
#define flt_isZero(_$x...) ____flt_isZero(_$x)
#define flt_isNonzero(_$x...) ____flt_isNonzero(_$x)
#define flt_isIntegral(_$x...) __step__flt_isIntegral(_$x)

/*========== Floating-Point Arithmetic Operations ===========================*/

#define flt_add(_$lhs, _$rhs...) __op__flt_add__step(_$lhs, _$rhs)
#define flt_sub(_$lhs, _$rhs...) __op__flt_sub__step(_$lhs, _$rhs)
#define flt_mul(_$lhs, _$rhs...) __op__flt_mul__step(_$lhs, _$rhs)
#define flt_mulAdd(_$x, _$y, _$z...) __op__flt_mulAdd__step(_$x, _$y, _$z)
#define flt_div(_$x, _$n...) __step__flt_div(_$x, _$n)
#define flt_divTrunc(_$x, _$n...) __op__flt_divTrunc(_$x, _$n)
#define flt_divRound(_$x, _$n...) __op__flt_divRound(_$x, _$n)
#define flt_divFloor(_$x, _$n...) __op__flt_divFloor(_$x, _$n)
#define flt_divEuclid(_$x, _$n...) __op__flt_divEuclid__step(_$x, _$n)
#define flt_divCeil(_$x, _$n...) __op__flt_divCeil(_$x, _$n)
#define flt_rem(_$x, _$n...) __op__flt_rem__step(_$x, _$n)
#define flt_remRound(_$x, _$n...) __op__flt_remRound__step(_$x, _$n)
#define flt_mod(_$x, _$n...) __op__flt_mod__step(_$x, _$n)
#define flt_modEuclid(_$x, _$n...) __op__flt_modEuclid__step(_$x, _$n)
#define flt_modCeil(_$x, _$n...) __op__flt_modCeil__step(_$x, _$n)

#define flt_neg(_$x...) __step__flt_neg(_$x)
#define flt_abs(_$x...) ____flt_abs(_$x)
#define flt_sgn(_$x...) ____flt_sgn(_$x)
#define flt_sgnBit_static(_$x...) ____flt_sgnBit_static(_$x)
#define flt_sgnBit(_$x...) __step__flt_sgnBit(_$x)
#define flt_inv_static(_$x...) ____flt_inv_static(_$x)
#define flt_inv(_$x...) __step__flt_inv(_$x)

/*========== Floating-Point Comparison Operations ===========================*/

#define flt_eql(_$lhs, _$rhs...) __op__flt_eql(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define flt_neq(_$lhs, _$rhs...) __op__flt_neq(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)

#define flt_ord(_$lhs, _$rhs... /*(cmp_Ord)*/) __op__flt_ord__step(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define flt_ordApx(_$lhs, _$rhs, _$threshold, _$mode... /*(cmp_Ord)*/) __op__flt_ordApx( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(threshold), pp_uniqTok(mode), \
    pp_uniqTok(diff), pp_uniqTok(tolerance), pp_uniqTok(ret), \
    _$lhs, _$rhs, _$threshold, _$mode \
)
#define flt_ordApxAbs(_$lhs, _$rhs, _$threshold... /*(cmp_Ord)*/) flt_ordApx(_$lhs, _$rhs, _$threshold, cmp_ApxMode_abs)
#define flt_ordApxRel(_$lhs, _$rhs, _$threshold... /*(cmp_Ord)*/) flt_ordApx(_$lhs, _$rhs, _$threshold, cmp_ApxMode_rel)
#define flt_eq(_$lhs, _$rhs...) __op__flt_eq(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define flt_ne(_$lhs, _$rhs...) __op__flt_ne(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define flt_lt(_$lhs, _$rhs...) __op__flt_lt(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define flt_gt(_$lhs, _$rhs...) __op__flt_gt(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define flt_le(_$lhs, _$rhs...) __op__flt_le(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define flt_ge(_$lhs, _$rhs...) __op__flt_ge(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)

/*========== Floating-Point Min/Max/Clamp Operations ========================*/

#define flt_min(_$lhs, _$rhs...) __op__flt_min(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define flt_max(_$lhs, _$rhs...) __op__flt_max(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define flt_clamp(_$x, _$lo /*$incl*/, _$hi /*$incl*/...) __op__flt_clamp__step(_$x, _$lo, _$hi)
#define flt_clamp01(_$x...) __op__flt_clamp01(_$x)
#define flt_wrap(_$x, _$lo /*$incl*/, _$hi /*$excl*/...) __op__flt_wrap__step(_$x, _$lo, _$hi)
#define flt_wrap01(_$x...) __op__flt_wrap01(_$x)

/*========== Floating-Point Rounding Operations =============================*/

#define flt_trunc(_$x...) __step__flt_trunc(_$x)
#define flt_round(_$x...) __step__flt_round(_$x)
#define flt_floor(_$x...) __step__flt_floor(_$x)
#define flt_ceil(_$x...) __step__flt_ceil(_$x)
#define flt_nearbyint(_$x...) __step__flt_nearbyint(_$x)
#define flt_rint(_$x...) __step__flt_rint(_$x)
#define flt_lround(_$x...) __step__flt_lround(_$x)
#define flt_llround(_$x...) __step__flt_llround(_$x)
#define flt_lrint(_$x...) __step__flt_lrint(_$x)
#define flt_llrint(_$x...) __step__flt_llrint(_$x)

/*========== Floating-Point Power and Root Operations =======================*/

#define flt_sqrt(_$x...) __step__flt_sqrt(_$x)
#define flt_cbrt(_$x...) __step__flt_cbrt(_$x)
#define flt_sq(_$x...) __step__flt_sq(_$x)
#define flt_cb(_$x...) __step__flt_cb(_$x)
#define flt_hypot(_$x, _$y...) __step__flt_hypot(_$x, _$y)

/*========== Floating-Point Exponential and Logarithm =======================*/

#define flt_exp(_$x...) ____flt_exp(_$x)
#define flt_exp2(_$x...) ____flt_exp2(_$x)
#define flt_exp10(_$x...) ____flt_exp10(_$x)
#define flt_powI(_$x, _$y...) __step__flt_powI(_$x, _$y)
#define flt_powF(_$x, _$y...) __step__flt_powF(_$x, _$y)
#define flt_ln(_$x...) __step__flt_ln(_$x)
#define flt_log2(_$x...) __step__flt_log2(_$x)
#define flt_log10(_$x...) __step__flt_log10(_$x)
#define flt_log1p(_$x...) __step__flt_log1p(_$x)
#define flt_expm1(_$x...) __step__flt_expm1(_$x)

/*========== Floating-Point Trigonometric Functions =========================*/

#define flt_sin(_$x...) ____flt_sin(_$x)
#define flt_cos(_$x...) ____flt_cos(_$x)
#define flt_tan(_$x...) ____flt_tan(_$x)
#define flt_asin(_$x...) __step__flt_asin(_$x)
#define flt_acos(_$x...) __step__flt_acos(_$x)
#define flt_atan(_$x...) ____flt_atan(_$x)
#define flt_atan2(_$y, _$x...) __step__flt_atan2(_$y, _$x)

/*========== Floating-Point Hyperbolic Functions ============================*/

#define flt_sinh(_$x...) ____flt_sinh(_$x)
#define flt_cosh(_$x...) ____flt_cosh(_$x)
#define flt_tanh(_$x...) ____flt_tanh(_$x)
#define flt_asinh(_$x...) ____flt_asinh(_$x)
#define flt_acosh(_$x...) __step__flt_acosh(_$x)
#define flt_atanh(_$x...) __step__flt_atanh(_$x)

/*========== Floating-Point IEEE 754 Operations =============================*/

#define flt_copySgn(_$x, _$y...) __op__flt_copySgn(pp_uniqTok(x), pp_uniqTok(y), _$x, _$y)
#define flt_nextAfter(_$x, _$y...) __op__flt_nextAfter(pp_uniqTok(x), pp_uniqTok(y), _$x, _$y)
#define flt_nextUp(_$x...) __step__flt_nextUp(_$x)
#define flt_nextDown(_$x...) __step__flt_nextDown(_$x)
#define flt_scalb(_$x, _$n...) __op__flt_scalb(pp_uniqTok(x), pp_uniqTok(n), _$x, _$n)
#define flt_scalbn(_$x, _$n...) __op__flt_scalbn(pp_uniqTok(x), pp_uniqTok(n), _$x, _$n)
#define flt_scalbln(_$x, _$n...) __op__flt_scalbln(pp_uniqTok(x), pp_uniqTok(n), _$x, _$n)
#define flt_ilogb(_$x...) ____flt_ilogb(_$x)
#define flt_fdim(_$x, _$y...) __op__flt_fdim(pp_uniqTok(x), pp_uniqTok(y), _$x, _$y)
#define flt_frexp(_$x, _$p_exp...) __op__flt_frexp(pp_uniqTok(x), _$x, _$p_exp)
#define flt_modf(_$x, _$p_int...) __op__flt_modf(pp_uniqTok(x), _$x, _$p_int)
#define flt_remquo(_$x, _$y, _$p_quo...) __op__flt_remquo(pp_uniqTok(x), pp_uniqTok(y), _$x, _$y, _$p_quo)
#define flt_fract(_$x...) __step__flt_fract(_$x)

/*===========================================================================*/
/*                          IMPLEMENTATION SECTION                           */
/*===========================================================================*/

/*========== Memory Operations Implementation ===============================*/

#if UNUSED_CODE
$attr($inline_always)
$static P$raw pri__memset(P$raw ptr, u8 val, usize len) {
    if (len == 0) { return ptr; }
    return raw_memset(ptr, val, len);
}
$attr($inline_always)
$static P$raw pri__memcpy(P$raw dst, P_const$raw src, usize len) {
    if (len == 0 || dst == src) { return dst; }
    return raw_memcpy(dst, src, len);
}
$attr($inline_always)
$static P$raw pri__memmove(P$raw dst, P_const$raw src, usize len) {
    if (len == 0 || dst == src) { return dst; }
    return raw_memmove(dst, src, len);
}
$attr($inline_always)
$static u8 pri__memcmp(P_const$raw lhs, P_const$raw rhs, usize len) {
    if (len == 0 || lhs == rhs) { return 0; }
    return raw_memcmp(lhs, rhs, len);
}
#endif /* UNUSED_CODE */

#define __op__pri_memset0(_$s_dst...) u_castS$((TypeOf(_$s_dst))(u_memset0(u_anyS(_$s_dst))))
#define __op__pri_memset0S(_$s_dst...) u_castS$((TypeOf(_$s_dst))(u_memset0S(u_anyS(_$s_dst))))
#define __op__pri_memset(_$p_dst, _$v_src...) u_castP$((TypeOf(_$p_dst))(u_memset(u_anyP(_$p_dst), u_anyP(from$((TypeOf(*(_$p_dst)))_$v_src)))))
#define __op__pri_memsetS(_$s_dst, _$v_src...) u_castS$((TypeOf(_$s_dst))(u_memsetS(u_anyS(_$s_dst), u_anyV(from$((TypeOf(*(_$s_dst).ptr))_$v_src)))))
#define __op__pri_memcpy(_$p_dst, _$p_src...) u_castP$((TypeOf(_$p_dst))(u_memcpy(u_anyP(_$p_dst), u_anyP(from$((TypeOf(*(_$p_dst)))_$p_src)))))
#define __op__pri_memcpyS(_$s_dst, _$s_src...) u_castS$((TypeOf(_$s_dst))(u_memcpyS(u_anyS(_$s_dst), u_anyS(from$((TypeOf(_$s_dst.as_const))_$s_src)))))
#define __op__pri_memmove(_$p_dst, _$p_src...) u_castP$((TypeOf(_$p_dst))(u_memmove(u_anyP(_$p_dst), u_anyP(from$((TypeOf(*(_$p_dst)))_$p_src)))))
#define __op__pri_memmoveS(_$s_dst, _$s_src...) u_castS$((TypeOf(_$s_dst))(u_memmoveS(u_anyS(_$s_dst), u_anyS(from$((TypeOf(_$s_dst.as_const))_$s_src)))))
#define __op__pri_memeql(_$p_lhs, _$p_rhs...) u_castP$((TypeOf(_$p_lhs))(u_memeql(u_anyP(_$p_lhs), u_anyP(from$((TypeOf(_$p_lhs))_$p_rhs)))))
#define __op__pri_memeqlS(_$s_lhs, _$s_rhs...) u_castS$((TypeOf(_$s_lhs))(u_memeqlS(u_anyS(_$s_lhs), u_anyS(from$((TypeOf(_$s_lhs.as_const))_$s_rhs)))))
#define __op__pri_memord(_$p_lhs, _$p_rhs...) u_castP$((TypeOf(_$p_lhs))(u_memord(u_anyP(_$p_lhs), u_anyP(from$((TypeOf(_$p_lhs))_$p_rhs)))))
#define __op__pri_memordS(_$s_lhs, _$s_rhs...) u_castS$((TypeOf(_$s_lhs))(u_memordS(u_anyS(_$s_lhs), u_anyS(from$((TypeOf(_$s_lhs.as_const))_$s_rhs)))))
#define __op__pri_alloca(_$len_bytes...) raw_alloca(_$len_bytes)
#define __op__pri_allocaAlign(_$len_bytes, _$log2_align...) raw_allocaAlign(_$len_bytes, _$log2_align)
#define __step__pri_swap(_$p_lhs, _$p_rhs...) ____pri_swap( \
    pp_uniqTok(tmp), pp_uniqTok(p_lhs), _$p_lhs, pp_uniqTok(p_rhs), _$p_rhs \
)
#define ____pri_swap(__tmp, __p_lhs, _$p_lhs, __p_rhs, _$p_rhs...) ({ \
    let __p_lhs = _$p_lhs; \
    let __p_rhs = _$p_rhs; \
    let __tmp = *__p_lhs; \
    *__p_lhs = *__p_rhs; \
    *__p_rhs = __tmp; \
    {}; \
})

#define __op__pri_add(_$lhs, _$rhs...) (as$(TypeOf(_$lhs))((_$lhs) + (_$rhs)))
#define __op__pri_sub(_$lhs, _$rhs...) (as$(TypeOf(_$lhs))((_$lhs) - (_$rhs)))
#define __op__pri_mul(_$lhs, _$rhs...) (as$(TypeOf(_$lhs))((_$lhs) * (_$rhs)))
#define __op__pri_mulAdd(_$x, _$y, _$z...) (as$(TypeOf(_$x))((_$x) * (_$y) + (_$z)))
#define __op__pri_div(_$lhs, _$rhs...) (as$(TypeOf(_$lhs))((_$lhs) / (_$rhs)))
#define __op__pri_rem(_$x, _$n...) (as$(TypeOf(_$x))((_$x) % (_$n)))

#define __op__int_divRound__step(_$x, _$n...) __op__int_divRound( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(q), pp_uniqTok(r), pp_uniqTok(half), _$x, _$n \
)
#define __op__int_divRound(__x, __n, __q, __r, __half, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__n, IntType) = _$n; \
    let_(__q, IntType) = __x / __n; \
    let_(__r, IntType) = __x % __n; \
    let_(__half, IntType) = __n / 2; \
    as$(IntType)( \
        (__r > __half || (__r == __half && (__q & 1))) \
            ? (__n > 0 ? __q + 1 : __q - 1) \
        : (__r < -__half || (__r == -__half && (__q & 1))) \
            ? (__n > 0 ? __q - 1 : __q + 1) \
            : __q \
    ); \
})
#define __op__iint_divFloor__step(_$x, _$n...) __op__iint_divFloor( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(q), pp_uniqTok(r), pp_uniqTok(has_r), pp_uniqTok(diff_sgn), _$x, _$n \
)
#define __op__iint_divFloor(__x, __n, __q, __r, __has_r, __diff_sgn, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__n, IntType) = _$n; \
    let_(__q, IntType) = int_div(__x, __n); \
    let_(__r, IntType) = int_rem_static(__x, __n); \
    let_(__has_r, bool) = __r != 0; \
    let_(__diff_sgn, bool) = __r ^ __n < 0; \
    as$(IntType)(__has_r && __diff_sgn ? __q - 1 : __q); \
})
#define __op__iint_divEuclid__step(_$x, _$n...) __op__iint_divEuclid( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(q), pp_uniqTok(r), _$x, _$n \
)
#define __op__iint_divEuclid(__x, __n, __q, __r, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__n, IntType) = _$n; \
    let_(__q, IntType) = int_div(__x, __n); \
    let_(__r, IntType) = int_rem_static(__x, __n); \
    as$(IntType)(__r < 0 ? (__n > 0 ? __q - 1 : __q + 1) : __q); \
})
#define __op__int_divCeil__step(_$x, _$n...) __op__int_divCeil( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(q), pp_uniqTok(r), pp_uniqTok(has_r), pp_uniqTok(same_sgn), _$x, _$n \
)
#define __op__int_divCeil(__x, __n, __q, __r, __has_r, __same_sgn, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__n, IntType) = _$n; \
    let_(__q, IntType) = int_div(__x, __n); \
    let_(__r, IntType) = int_rem_static(__x, __n); \
    let_(__has_r, bool) = __r != 0; \
    let_(__same_sgn, bool) = __r ^ __n >= 0; \
    as$(IntType)(__has_r && __same_sgn ? __q + 1 : __q); \
})

#define __op__flt_divTrunc(_$x, _$n...) flt_trunc(flt_div(_$x, _$n))
#define __op__flt_divRound(_$x, _$n...) flt_round(flt_div(_$x, _$n))
#define __op__flt_divFloor(_$x, _$n...) flt_floor(flt_div(_$x, _$n))
#define __op__flt_divEuclid__step(_$x, _$n...) __op__flt_divEuclid(pp_uniqTok(x), pp_uniqTok(n), _$x, _$n)
#define __op__flt_divEuclid(__x, __n, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    let_(__n, FltType) = _$n; \
    let_(__q, FltType) = flt_divTrunc(__x, __n); \
    let_(__r, FltType) = __x - __q * __n; \
    as$(FltType)(__r < 0.0 ? (__n > 0.0 ? __q - 1.0 : __q + 1.0) : __q); \
})
#define __op__flt_divCeil(_$x, _$n...) flt_ceil(flt_div(__x, __n))

#define __op__int_rem_static(_$x, _$n...) ((_$x) % (_$n))
#define __op__int_rem__step(_$x, _$n...) __op__int_rem(pp_uniqTok(x), pp_uniqTok(n), _$x, _$n)
#define __op__int_rem(__x, __n, _$x, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _$x; \
    let_(__n, IntType) = _$n; \
    claim_assert(__n != 0); \
    claim_assert(isIInt$(IntType) ? !(__x == int_limit_min$(IntType) && __n == as$(IntType)(-1)) : true); \
    as$(IntType)(__x % __n); \
})
#define __op__int_remRound__step(_$x, _$n...) __op__int_remRound(pp_uniqTok(x), pp_uniqTok(n), _$x, _$n)
#define __op__int_remRound(__x, __n, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__n, IntType) = _$n; \
    as$(IntType)(__x - int_divRound(__x, __n) * __n); \
})
#define __op__iint_mod__step(_$x, _$n...) __op__iint_mod( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(r), pp_uniqTok(has_r), pp_uniqTok(diff_sgn), _$x, _$n \
)
#define __op__iint_mod(__x, __n, __r, __has_r, __diff_sgn, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__n, IntType) = _$n; \
    let_(__r, IntType) = int_rem(__x, __n); \
    let_(__has_r, bool) = __r != 0; \
    let_(__diff_sgn, bool) = (__r ^ __n) < 0; \
    as$(IntType)(__has_r && __diff_sgn ? __r + __n : __r); \
})
#define __op__iint_modEuclid__step(_$x, _$n...) __op__iint_modEuclid( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(r), _$x, _$n \
)
#define __op__iint_modEuclid(__x, __n, __r, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__n, IntType) = _$n; \
    let_(__r, IntType) = int_rem(__x, __n); \
    as$(IntType)(__r < 0 ? __r + pri_abs(__n) : __r); \
})
#define __op__int_modCeil__step(_$x, _$n...) __op__int_modCeil( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(r), pp_uniqTok(has_r), pp_uniqTok(same_sgn), _$x, _$n \
)
#define __op__int_modCeil(__x, __n, __r, __has_r, __same_sgn, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__n, IntType) = _$n; \
    let_(__r, IntType) = int_rem(__x, __n); \
    let_(__has_r, bool) = __r != 0; \
    let_(__same_sgn, bool) = __r ^ __n >= 0; \
    as$(IntType)(__has_r && __same_sgn ? __r - __n : __r); \
})

#define __op__flt_rem__step(_$x, _$n...) __op__flt_rem(pp_uniqTok(x), pp_uniqTok(n), _$x, _$n)
#define __op__flt_rem(__x, __n, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    let_(__n, FltType) = _$n; \
    claim_assert(__n != 0.0); \
    T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_fmodf(as$(f32)(__x), as$(f32)(__n)))), \
        T_case$((f64)(__builtin_fmod(as$(f64)(__x), as$(f64)(__n)))) \
    )); \
})
#define __op__flt_remRound__step(_$x, _$n...) __op__flt_remRound(pp_uniqTok(x), pp_uniqTok(n), _$x, _$n)
#define __op__flt_remRound(__x, __n, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    let_(__n, FltType) = _$n; \
    claim_assert(__n != 0.0); \
    T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_remainderf(as$(f32)(__x), as$(f32)(__n)))), \
        T_case$((f64)(__builtin_remainder(as$(f64)(__x), as$(f64)(__n)))) \
    )); \
})
#define __op__flt_mod__step(_$x, _$n...) ____flt_mod(pp_uniqTok(x), _$x, pp_uniqTok(n), _$n)
#define ____flt_mod(__x, _$x, __n, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__n, FltType) = _$n; \
    __x - flt_divFloor(__x, __n) * __n; \
})
#define __op__flt_modEuclid__step(_$x, _$n...) __op__flt_modEuclid( \
    pp_uniqTok(x), pp_uniqTok(n), pp_uniqTok(r), _$x, _$n \
)
#define __op__flt_modEuclid(__x, __n, __r, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    let_(__n, FltType) = _$n; \
    let_(__r, FltType) = flt_rem(__x, __n); \
    __r < 0.0 ? __r + flt_abs(__n) : __r; \
})
#define __op__flt_modCeil__step(_$x, _$n...) __op__flt_modCeil(pp_uniqTok(x), pp_uniqTok(n), _$x, _$n)
#define __op__flt_modCeil(__x, __n, _$x, _$n...) ({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    let_(__n, FltType) = _$n; \
    __x - flt_divCeil(__x, __n) * __n; \
})

#define __op__pri_neg(_$x...) (as$(TypeOf(_$x))(-(_$x)))
#define __op__pri_abs_static(_$x...) (as$(TypeOf(_$x))(((_$x) < 0) ? -(_$x) : (_$x)))
#define __op__pri_abs__step(_$x...) __op__pri_abs(pp_uniqTok(x), _$x)
#define __op__pri_abs(__x, _$x...) ({ \
    typedef TypeOfUnqual(_$x) ScalType; \
    let_(__x, ScalType) = _$x; \
    pri_abs_static(__x); \
})
#define __op__pri_sgn_static(_$x...) (as$(cmp_Sgn)( \
    ((_$x) < 0)   ? cmp_Sgn_ngtv \
    : (0 < (_$x)) ? cmp_Sgn_pstv \
                  : cmp_Sgn_zero \
))
#define __op__pri_sgn__step(_$x...) __op__pri_sgn(pp_uniqTok(x), _$x)
#define __op__pri_sgn(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) ScalType; \
    let_(__x, ScalType) = _$x; \
    local_return_(pri_sgn_static(__x)); \
})

#define __op__isZero(_$x...) pri_eql(_$x, 0)
#define __op__isNonzero(_$x...) pri_neq(_$x, 0)

#define __op__pri_not(_$x...) bool_(!(_$x))
#define __op__pri_and(_$x, _$y...) bool_((_$x) && (_$y))
#define __op__pri_or(_$x, _$y...) bool_((_$x) || (_$y))
#define __op__bool_any__step(_$vals...) __op__bool_any(pp_uniqTok(vals), pp_uniqTok(val), _$vals)
#define __op__bool_any(__vals, __val, _$vals...) ({ \
    let __vals = A_from$((bool){ _$vals }); \
    var __ret = false; \
    for_(($s(A_ref(__vals)))(__val)) { \
        __ret = pri_or(__ret, *__val); \
    } $end(for); \
    __ret; \
})
#define __op__bool_all__step(_$vals...) __op__bool_all(pp_uniqTok(vals), pp_uniqTok(val), _$vals)
#define __op__bool_all(__vals, __val, _$vals...) ({ \
    let __vals = A_from$((bool){ _$vals }); \
    var __ret = true; \
    for_(($s(A_ref(__vals)))(__val)) { \
        __ret = pri_and(__ret, *__val); \
    } $end(for); \
    __ret; \
})
#define __op__bool_none__step(_$vals...) bool_(!bool_any(_$vals))

#define __op__pri_eql(_$lhs, _$rhs...) bool_((_$lhs) == (_$rhs))
#define __op__pri_neq(_$lhs, _$rhs...) bool_((_$lhs) != (_$rhs))
#define __op__pri_ord_static(_$lhs, _$rhs...) (as$(cmp_Ord)( \
    ((_$lhs) < (_$rhs))   ? cmp_Ord_lt \
    : ((_$lhs) > (_$rhs)) ? cmp_Ord_gt \
                          : cmp_Ord_eq \
))
#define __op__pri_ord__step(_$lhs, _$rhs...) __op__pri_ord(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define __op__pri_ord(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) ScalType; \
    let_(__lhs, ScalType) = _$lhs; \
    let_(__rhs, ScalType) = _$rhs; \
    local_return_(pri_ord_static(__lhs, __rhs)); \
})
#define __op__pri_eq(_$lhs, _$rhs...) bool_((_$lhs) == (_$rhs))
#define __op__pri_ne(_$lhs, _$rhs...) bool_((_$lhs) != (_$rhs))
#define __op__pri_lt(_$lhs, _$rhs...) bool_((_$lhs) < (_$rhs))
#define __op__pri_gt(_$lhs, _$rhs...) bool_((_$lhs) > (_$rhs))
#define __op__pri_le(_$lhs, _$rhs...) bool_((_$lhs) <= (_$rhs))
#define __op__pri_ge(_$lhs, _$rhs...) bool_((_$lhs) >= (_$rhs))

#define __op__pri_min2_static(_$lhs, _$rhs...) (as$(TypeOf(_$lhs))((_$rhs) < (_$lhs) ? (_$rhs) : (_$lhs)))
#define __op__pri_min2__step(_$lhs, _$rhs...) __op__pri_min2( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs \
)
#define __op__pri_min2(__lhs, __rhs, _$lhs, _$rhs...) ({ \
    typedef TypeOfUnqual(_$lhs) ScalType; \
    let_(__lhs, ScalType) = _$lhs; \
    let_(__rhs, ScalType) = _$rhs; \
    pri_min2_static(__lhs, __rhs); \
})
#define __op__pri_min3_static(_$1st, _$2nd, _$3rd...) \
    pri_min2_static(pri_min2_static(_$1st, _$2nd), _$3rd)
#define __op__pri_min3__step(_$1st, _$2nd, _$3rd...) __op__pri_min3( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), _$1st, _$2nd, _$3rd \
)
#define __op__pri_min3(__1st, __2nd, __3rd, _$1st, _$2nd, _$3rd...) ({ \
    typedef TypeOfUnqual(_$1st) ScalType; \
    let_(__1st, ScalType) = _$1st; \
    let_(__2nd, ScalType) = _$2nd; \
    let_(__3rd, ScalType) = _$3rd; \
    pri_min3_static(__1st, __2nd, __3rd); \
})
#define __op__pri_min4_static(_$1st, _$2nd, _$3rd, _$4th...) \
    pri_min2_static(pri_min3_static(_$1st, _$2nd, _$3rd), _$4th)
#define __op__pri_min4__step(_$1st, _$2nd, _$3rd, _$4th...) __op__pri_min4( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), pp_uniqTok(4th), _$1st, _$2nd, _$3rd, _$4th \
)
#define __op__pri_min4(__1st, __2nd, __3rd, __4th, _$1st, _$2nd, _$3rd, _$4th...) ({ \
    typedef TypeOfUnqual(_$1st) ScalType; \
    let_(__1st, ScalType) = _$1st; \
    let_(__2nd, ScalType) = _$2nd; \
    let_(__3rd, ScalType) = _$3rd; \
    let_(__4th, ScalType) = _$4th; \
    pri_min4_static(__1st, __2nd, __3rd, __4th); \
})
#define __op__pri_min5_static(_$1st, _$2nd, _$3rd, _$4th, _$5th...) pri_min2_static(pri_min4_static(_$1st, _$2nd, _$3rd, _$4th), _$5th)
#define __op__pri_min5__step(_$1st, _$2nd, _$3rd, _$4th, _$5th...) __op__pri_min5( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), pp_uniqTok(4th), pp_uniqTok(5th), _$1st, _$2nd, _$3rd, _$4th, _$5th \
)
#define __op__pri_min5(__1st, __2nd, __3rd, __4th, __5th, _$1st, _$2nd, _$3rd, _$4th, _$5th...) ({ \
    typedef TypeOfUnqual(_$1st) ScalType; \
    let_(__1st, ScalType) = _$1st; \
    let_(__2nd, ScalType) = _$2nd; \
    let_(__3rd, ScalType) = _$3rd; \
    let_(__4th, ScalType) = _$4th; \
    let_(__5th, ScalType) = _$5th; \
    pri_min5_static(__1st, __2nd, __3rd, __4th, __5th); \
})
#define __op__pri_findMin__step(_$vals...) __op__pri_findMin( \
    pp_uniqTok(best), pp_uniqTok(vals), pp_uniqTok(val), _$vals \
)
#define __op__pri_findMin(__best, __vals, __val, _best, _$vals...) ({ \
    typedef TypeOfUnqual(_best) ScalType; \
    var_(__best, ScalType) = _best; \
    let __vals = A_from$((ScalType){ _$vals }); \
    for_(($s(A_ref(__vals)))(__val)) { \
        __best = pri_min2_static(__best, *__val); \
    } $end(for); \
    __best; \
})
#define __op__pri_max2_static(_$lhs, _$rhs...) (as$(TypeOf(_$lhs))((_$rhs) > (_$lhs) ? (_$rhs) : (_$lhs)))
#define __op__pri_max2__step(_$lhs, _$rhs...) __op__pri_max2( \
    pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs \
)
#define __op__pri_max2(__lhs, __rhs, _$lhs, _$rhs...) ({ \
    typedef TypeOfUnqual(_$lhs) ScalType; \
    let_(__lhs, ScalType) = _$lhs; \
    let_(__rhs, ScalType) = _$rhs; \
    pri_max2_static(__lhs, __rhs); \
})
#define __op__pri_max3_static(_$1st, _$2nd, _$3rd...) pri_max2_static(pri_max2_static(_$1st, _$2nd), _$3rd)
#define __op__pri_max3__step(_$1st, _$2nd, _$3rd...) __op__pri_max3( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), _$1st, _$2nd, _$3rd \
)
#define __op__pri_max3(__1st, __2nd, __3rd, _$1st, _$2nd, _$3rd...) ({ \
    typedef TypeOfUnqual(_$1st) ScalType; \
    let_(__1st, ScalType) = _$1st; \
    let_(__2nd, ScalType) = _$2nd; \
    let_(__3rd, ScalType) = _$3rd; \
    pri_max3_static(__1st, __2nd, __3rd); \
})
#define __op__pri_max4_static(_$1st, _$2nd, _$3rd, _$4th...) pri_max2_static(pri_max3_static(_$1st, _$2nd, _$3rd), _$4th)
#define __op__pri_max4__step(_$1st, _$2nd, _$3rd, _$4th...) __op__pri_max4( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), pp_uniqTok(4th), _$1st, _$2nd, _$3rd, _$4th \
)
#define __op__pri_max4(__1st, __2nd, __3rd, __4th, _$1st, _$2nd, _$3rd, _$4th...) ({ \
    typedef TypeOfUnqual(_$1st) ScalType; \
    let_(__1st, ScalType) = _$1st; \
    let_(__2nd, ScalType) = _$2nd; \
    let_(__3rd, ScalType) = _$3rd; \
    let_(__4th, ScalType) = _$4th; \
    pri_max4_static(__1st, __2nd, __3rd, __4th); \
})
#define __op__pri_max5_static(_$1st, _$2nd, _$3rd, _$4th, _$5th...) pri_max2_static(pri_max4_static(_$1st, _$2nd, _$3rd, _$4th), _$5th)
#define __op__pri_max5__step(_$1st, _$2nd, _$3rd, _$4th, _$5th...) __op__pri_max5( \
    pp_uniqTok(1st), pp_uniqTok(2nd), pp_uniqTok(3rd), pp_uniqTok(4th), pp_uniqTok(5th), _$1st, _$2nd, _$3rd, _$4th, _$5th \
)
#define __op__pri_max5(__1st, __2nd, __3rd, __4th, __5th, _$1st, _$2nd, _$3rd, _$4th, _$5th...) ({ \
    typedef TypeOfUnqual(_$1st) ScalType; \
    let_(__1st, ScalType) = _$1st; \
    let_(__2nd, ScalType) = _$2nd; \
    let_(__3rd, ScalType) = _$3rd; \
    let_(__4th, ScalType) = _$4th; \
    let_(__5th, ScalType) = _$5th; \
    pri_max5_static(__1st, __2nd, __3rd, __4th, __5th); \
})
#define __op__pri_findMax__step(_$vals...) __op__pri_findMax( \
    pp_uniqTok(best), pp_uniqTok(vals), pp_uniqTok(val), _$vals \
)
#define __op__pri_findMax(__best, __vals, __val, _best, _$vals...) ({ \
    typedef TypeOfUnqual(_best) ScalType; \
    var_(__best, ScalType) = _best; \
    let __vals = A_from$((ScalType){ _$vals }); \
    for_(($s(A_ref(__vals)))(__val)) { \
        __best = pri_max2_static(__best, *__val); \
    } $end(for); \
    __best; \
})

#define __op__pri_clamp_static(_$x, _$lo, _$hi...) pri_min2_static(pri_max2_static(_$lo, _$x), _$hi)
#define __op__pri_clamp__step(_$x, _$lo, _$hi...) __op__pri_clamp(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _$x, _$lo, _$hi)
#define __op__pri_clamp(__x, __lo, __hi, _$x, _$lo, _$hi...) ({ \
    typedef TypeOfUnqual(_$x) ScalType; \
    let_(__x, ScalType) = _$x; \
    let_(__lo, ScalType) = _$lo; \
    let_(__hi, ScalType) = _$hi; \
    claim_assert(__lo <= __hi); \
    pri_clamp_static(__x, __lo, __hi); \
})
#define __op__int_wrap__step(_$x, _$lo, _$hi...) __op__int_wrap(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _$x, _$lo, _$hi)
#define __op__int_wrap(__x, __lo, __hi, _$x, _$lo, _$hi...) ({ \
    typedef TypeOfUnqual(_$x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _$x; \
    let_(__lo, IntType) = _$lo; \
    let_(__hi, IntType) = _$hi; \
    claim_assert(__lo < __hi); \
    __lo + iint_modEuclid(__x - __lo, __hi - __lo); \
})

#define __op__flt_wrap__step(_$x, _$lo, _$hi...) __op__flt_wrap(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _$x, _$lo, _$hi)
#define __op__flt_wrap(__x, __lo, __hi, _$x, _$lo, _$hi...) ({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    let_(__lo, FltType) = _$lo; \
    let_(__hi, FltType) = _$hi; \
    claim_assert(__lo < _$hi); \
    __lo + flt_modEuclid(__x - __lo, __hi - __lo); \
})
#define __op__flt_wrap01(_$x...) flt_wrap(_$x, 0, 1)

#define ____isValidAlign_static(_$align...) bool_(0 < _$align && (_$align & (_$align - 1)) == 0)
#define __step__isValidAlign(_$align...) ____isValidAlign(pp_uniqTok(align), _$align)
#define ____isValidAlign(__align, _$align...) ({ \
    let_(__align, usize) = _$align; \
    isValidAlign_static(__align); \
})

#define ____isAligned_static(_$addr, _$align...) bool_((ptrToInt(_$addr) & (_$align - 1)) == 0)
#define __step__isAligned(_$addr, _$align...) ____isAligned( \
    pp_uniqTok(addr), _$addr, pp_uniqTok(align), _$align \
)
#define ____isAligned(__addr, _$addr, __align, _$align...) ({ \
    let_(__addr, usize) = ptrToInt(_$addr); \
    let_(__align, usize) = _$align; \
    claim_assert(isValidAlign(__align)); \
    bool_((__addr & (__align - 1)) == 0); \
})
#define ____isAlignedLog2_static(_$addr, _$log2_align...) bool_(int_trailingZeros_static(_$addr) >= _$log2_align)
#define __step__isAlignedLog2(_$addr, _$log2_align...) ____isAlignedLog2( \
    pp_uniqTok(addr), _$addr, pp_uniqTok(log2_align), _$log2_align \
)
#define ____isAlignedLog2(__addr, _$addr, __log2_align, _$log2_align...) ({ \
    let_(__addr, usize) = ptrToInt(_$addr); \
    let_(__log2_align, u8) = _$log2_align; \
    bool_(int_trailingZeros(__addr) >= __log2_align); \
})

#define ____alignFwd_static(_$addr, _$align...) ((ptrToInt(_$addr) + (_$align - 1)) & ~(_$align - 1))
#define __step__alignFwd(_$addr, _$align...) ____alignFwd(pp_uniqTok(addr), _$addr, pp_uniqTok(align), _$align)
#define ____alignFwd(__addr, _$addr, __align, _$align...) ({ \
    let_(__addr, usize) = ptrToInt(_$addr); \
    let_(__align, usize) = _$align; \
    claim_assert(isValidAlign(__align)); \
    (__addr + (__align - 1)) & ~(__align - 1); \
})
#define ____alignFwdLog2_static(_$addr, _$log2_align...) alignFwd_static(_$addr, log2ToAlign_static(_$log2_align))
#define __step__alignFwdLog2(_$addr, _$log2_align...) ____alignFwdLog2(pp_uniqTok(addr), _$addr, pp_uniqTok(log2_align), _$log2_align)
#define ____alignFwdLog2(__addr, _$addr, __log2_align, _$log2_align...) ({ \
    let_(__addr, usize) = ptrToInt(_$addr); \
    let_(__log2_align, u8) = _$log2_align; \
    alignFwd(__addr, log2ToAlign(__log2_align)); \
})
#define ____alignBwd_static(_$addr, _$align...) (ptrToInt(_$addr) & ~(_$align - 1))
#define __step__alignBwd(_$addr, _$align...) ____alignBwd(pp_uniqTok(addr), _$addr, pp_uniqTok(align), _$align)
#define ____alignBwd(__addr, _$addr, __align, _$align...) ({ \
    let_(__addr, usize) = ptrToInt(_$addr); \
    let_(__align, usize) = _$align; \
    claim_assert(isValidAlign(__align)); \
    (__addr & ~(__align - 1)); \
})
#define ____alignBwdLog2_static(_$addr, _$log2_align...) alignBwd_static(_$addr, log2ToAlign_static(_$log2_align))
#define __step__alignBwdLog2(_$addr, _$log2_align...) ____alignBwdLog2(pp_uniqTok(addr), _$addr, pp_uniqTok(log2_align), _$log2_align)
#define ____alignBwdLog2(__addr, _$addr, __log2_align, _$log2_align...) ({ \
    let_(__addr, usize) = ptrToInt(_$addr); \
    let_(__log2_align, u8) = _$log2_align; \
    alignBwd(__addr, log2ToAlign(__log2_align)); \
})

#define ____alignToLog2_static(_$align...) (as$(u8)(int_trailingZeros_static(_$align)))
#define __alias__log2FromAlign_static alignToLog2_static
#define __step__alignToLog2(_$align...) ____alignToLog2(pp_uniqTok(align), _$align)
#define ____alignToLog2(__align, _$align...) ({ \
    let_(__align, usize) = _$align; \
    claim_assert(isValidAlign(__align)); \
    intCast$((u8)(int_trailingZeros(__align))); \
})
#define __alias__log2FromAlign alignToLog2
#define ____log2ToAlign_static(_$log2_align...) (as$(usize)(1) << _$log2_align)
#define __alias__alignFromLog2_static log2ToAlign_static
#define __step__log2ToAlign(_$log2_align...) ____log2ToAlign(pp_uniqTok(log2_align), _$log2_align)
#define ____log2ToAlign(__log2_align, _$log2_align...) ({ \
    let_(__log2_align, u8) = _$log2_align; \
    usize_(1) << __log2_align; \
})
#define __alias__alignFromLog2 log2ToAlign

#define __step__alignCast(...) __step__alignCast__emit(__step__alignCast__parse __VA_ARGS__)
#define __step__alignCast__parse(_$log2_align...) pp_uniqTok(log2_align), _$log2_align, pp_uniqTok(ptr),
#define __step__alignCast__emit(...) ____alignCast(__VA_ARGS__)
#define ____alignCast(__log2_align, _$log2_align, __ptr, _ptr...) local_({ \
    let_(__log2_align, u8) = _$log2_align; \
    let __ptr = _ptr; \
    claim_assert(isAlignedLog2(__ptr, __log2_align)); \
    local_return_(__ptr); \
})

#if UNUSED_CODE
#define __step__bitCast$(...) __step__bitCast$__emit(__step__bitCast$__parse __VA_ARGS__)
#define __step__bitCast$__parse(_$T...) _$T, pp_uniqTok(val),
#define __step__bitCast$__emit(...) ____bitCast$(__VA_ARGS__)
#define ____bitCast$(_$T, __val, _$val...) ({ \
    let_(__val, TypeOf(_$val)) = _$val; \
    claim_assert_static(sizeOf$(_$T) == sizeOf$(TypeOf(__val))); \
    *ptrCast$((_$T*)(raw_memcpy(&l0$((_$T)), &__val, sizeOf$(TypeOf(__val))))); \
})
#endif /* UNUSED_CODE */
#define __step__bitCast$(...) __step__bitCast$__emit(__step__bitCast$__parse __VA_ARGS__)
#define __step__bitCast$__parse(_$T...) pp_uniqTok(dst), _$T, pp_uniqTok(val),
#define __step__bitCast$__emit(...) ____bitCast$(__VA_ARGS__)
#define ____bitCast$(__dst, _$T, __val, _$val...) ({ \
    var __dst = l0$((_$T)); \
    let_(__val, TypeOf(_$val)) = _$val; \
    claim_assert_static(sizeOf$(_$T) == sizeOf$(TypeOf(__val))); \
    raw_memcpy(&__dst, &__val, sizeOf$(TypeOf(__val))); \
    __dst; \
})

#define __step__intToBool(_$val...) ____intToBool(pp_uniqTok(val), _$val)
#define ____intToBool(__val, _$val...) ({ \
    typedef TypeOf(_$val) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__val, IntType) = _$val; \
    claim_assert(__val == 0 || __val == 1); \
    as$(bool)(__val); \
})
#define __alias__boolFromInt intToBool
#define ____boolToInt(_$val...) T_switch$((TypeOf(_$val))( \
    T_case$((bool)(as$(u8)(_$val))) \
))
#define __alias__intFromBool boolToInt

#define __step__intCast$(...) __step__intCast$__emit(__step__intCast$__parse __VA_ARGS__)
#define __step__intCast$__parse(_$T...) \
    _$T, pp_uniqTok(val), pp_uniqTok(min), pp_uniqTok(max), \
        pp_uniqTok(dst_is_signed), pp_uniqTok(src_is_signed),
#define __step__intCast$__emit(...) ____intCast$(__VA_ARGS__)
#define ____intCast$(_$T, __val, __min, __max, __dst_is_signed, __src_is_signed, _$val...) $suppress_implicit_int_conversion(({ \
    typedef _$T DstType; \
    typedef TypeOf(_$val) SrcType; \
    claim_assert_static(isInt$(SrcType)); \
    let_(__val, SrcType) = _$val; \
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
    as$(_$T)(__val); \
}))
#if UNUSED_CODE
#define ____intCast$(_$T, __val, __min, __max, _$val...) ({ \
    typedef TypeOf(_$T) DstType; \
    typedef TypeOf(_$val) SrcType; \
    claim_assert_static(isInt$(SrcType)); \
    let_(__val, SrcType) = _$val; \
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
    as$(_$T)(__val); \
})
#endif /* UNUSED_CODE */

#define __step__intToFlt$(...) __step__intToFlt$__emit(__step__intToFlt$__parse __VA_ARGS__)
#define __step__intToFlt$__parse(_$T...) _$T, pp_uniqTok(val),
#define __step__intToFlt$__emit(...) ____intToFlt$(__VA_ARGS__)
#define ____intToFlt$(_$T, __val, _$val...) ({ \
    typedef TypeOf(_$val) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__val, IntType) = _$val; \
    as$(_$T)(__val); \
})
#define __alias__fltFromInt$ intToFlt$
/* Check: (min - 1) < x < max, where max = 2^(N-1) for signed, 2^N for unsigned */
/* This correctly handles cases like -2147483648.9 → trunc → -2147483648 (valid i32) */
#define __step__fltToInt$(...) __step__fltToInt$__emit(__step__fltToInt$__parse __VA_ARGS__)
#define __step__fltToInt$__parse(_$T...) _$T, pp_uniqTok(val),
#define __step__fltToInt$__emit(...) ____fltToInt$(__VA_ARGS__)
#define ____fltToInt$(_$T, __val, _$val...) ({ \
    typedef TypeOf(_$val) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__val, FltType) = _$val; \
    claim_assert(flt_isFinite(__val)); \
    claim_assert(as$(FltType)(int_limit_flt_min_bound_excl$(_$T)) < __val); \
    claim_assert(__val < as$(FltType)(int_limit_flt_max_bound_excl$(_$T))); \
    as$(_$T)(__val); \
})
#define __alias__intFromFlt$ fltToInt$

#define __step__fltCast$(...) __step__fltCast$__emit(__step__fltCast$__parse __VA_ARGS__)
#define __step__fltCast$__parse(_$T...) _$T, pp_uniqTok(val), pp_uniqTok(min), pp_uniqTok(max),
#define __step__fltCast$__emit(...) ____fltCast$(__VA_ARGS__)
#define ____fltCast$(_$T, __val, __min, __max, _$val...) ({ \
    typedef _$T DstType; \
    typedef TypeOf(_$val) SrcType; \
    claim_assert_static(isFlt$(SrcType)); \
    let_(__val, SrcType) = _$val; \
    let_(__min, DstType) = flt_limit_min$(DstType); \
    let_(__max, DstType) = flt_limit_max$(DstType); \
    claim_assert(__min <= __max); \
    claim_assert(__min <= __val); \
    claim_assert(__val <= __max); \
    as$(_$T)(__val); \
})

/*========== Integer Arithmetic Implementation ==============================*/

#define __op__int_add__step(_$lhs, _$rhs...) __op__int_add(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define __op__int_add(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    claim_assert(!__builtin_add_overflow(__lhs, __rhs, &l0$((IntType)))); \
    local_return_(as$(IntType)(__lhs + __rhs)); \
})
#define __op__int_addOverflow(__lhs, __rhs, __out, __overflow, _$lhs, _$rhs, _$p_out...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    var_(__out, IntType) = 0; \
    let_(__overflow, bool) = bool_(__builtin_add_overflow(__lhs, __rhs, &__out)); \
    *(_$p_out) = as$(TypeOf(*(_$p_out)))(__out); \
    local_return_(__overflow); \
})
#define __op__int_addWrap(__lhs, __rhs, __out, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    var_(__out, IntType) = 0; \
    (void)__builtin_add_overflow(__lhs, __rhs, &__out); \
    local_return_(__out); \
})
#define __op__int_addSat(__lhs, __rhs, __out, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    var_(__out, IntType) = 0; \
    local_return_(__builtin_add_overflow(__lhs, __rhs, &__out) ? as$(IntType)(isUInt$(IntType) ? int_limit_max$(IntType) : (__lhs < 0 ? int_limit_min$(IntType) : int_limit_max$(IntType))) : __out); \
})
#define __op__uint_addCarry(__lhs, __rhs, __carry_in, _$lhs, _$rhs, _carry_in, _p_carry_out...) local_({ \
    typedef TypeOfUnqual(_$lhs) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__lhs, UIntType) = _$lhs; \
    let_(__rhs, UIntType) = _$rhs; \
    let_(__carry_in, UIntType) = _carry_in; \
    local_return_(T_switch$((TypeOf(UIntType))( \
        T_case$((u8)(____uint_addCarry8(__lhs, __rhs, __carry_in, _p_carry_out))), \
        T_case$((u16)(____uint_addCarry16(__lhs, __rhs, __carry_in, _p_carry_out))), \
        T_case$((u32)(____uint_addCarry32(__lhs, __rhs, __carry_in, _p_carry_out))), \
        pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
            T_case$((ulong)(____uint_addCarryLong(__lhs, __rhs, __carry_in, _p_carry_out))), \
        )) T_delim(), \
        T_case$((u64)(____uint_addCarry64(__lhs, __rhs, __carry_in, _p_carry_out))) \
    ))); \
})
#if UINT64_MAX == ULONG_MAX
#define ____uint_addCarry64(_$lhs, _$rhs, _carry_in, _p_carry_out...) local_({ \
    var_(__carry_out, ulong) = 0; \
    let_(__res, u64) = as$(u64)(__builtin_addcl( \
        as$(ulong)(_$lhs), as$(ulong)(_$rhs), as$(ulong)(_carry_in), &__carry_out \
    )); \
    *(_p_carry_out) = as$(TypeOf(*(_p_carry_out)))(__carry_out); \
    local_return_(__res); \
})
#elif UINT64_MAX == ULLONG_MAX
#define ____uint_addCarry64(_$lhs, _$rhs, _carry_in, _p_carry_out...) local_({ \
    var_(__carry_out, unsigned long long) = 0; \
    let_(__res, u64) = as$(u64)(__builtin_addcll( \
        as$(unsigned long long)(_$lhs), as$(unsigned long long)(_$rhs), as$(unsigned long long)(_carry_in), &__carry_out \
    )); \
    *(_p_carry_out) = as$(TypeOf(*(_p_carry_out)))(__carry_out); \
    local_return_(__res); \
})
#else
#define ____uint_addCarry64(_$lhs, _$rhs, _carry_in, _p_carry_out...) local_({ \
    let_(__lhs, u64) = as$(u64)(_$lhs); \
    let_(__rhs, u64) = as$(u64)(_$rhs); \
    let_(__cin, u64) = as$(u64)(_carry_in); \
    let_(__sum, u64) = as$(u64)(__lhs + __rhs); \
    let_(__res, u64) = as$(u64)(__sum + __cin); \
    *(_p_carry_out) = as$(TypeOf(*(_p_carry_out)))((__sum < __lhs) || (__res < __sum)); \
    local_return_(__res); \
})
#endif
#define ____uint_addCarryLong(_$lhs, _$rhs, _carry_in, _p_carry_out...) local_({ \
    var_(__carry_out, ulong) = 0; \
    let_(__res, ulong) = __builtin_addcl(as$(ulong)(_$lhs), as$(ulong)(_$rhs), as$(ulong)(_carry_in), &__carry_out); \
    *(_p_carry_out) = as$(TypeOf(*(_p_carry_out)))(__carry_out); \
    local_return_(__res); \
})
#define ____uint_addCarry32(_$lhs, _$rhs, _carry_in, _p_carry_out...) local_({ \
    var_(__carry_out, u32) = 0; \
    let_(__res, u32) = __builtin_addc(as$(u32)(_$lhs), as$(u32)(_$rhs), as$(u32)(_carry_in), &__carry_out); \
    *(_p_carry_out) = as$(TypeOf(*(_p_carry_out)))(__carry_out); \
    local_return_(__res); \
})
#define ____uint_addCarry16(_$lhs, _$rhs, _carry_in, _p_carry_out...) local_({ \
    let_(__sum, u32) = as$(u32)(as$(u16)(_$lhs)) + as$(u32)(as$(u16)(_$rhs)) + as$(u32)(as$(u16)(_carry_in)); \
    *(_p_carry_out) = as$(TypeOf(*(_p_carry_out)))(__sum >> u16_bits); \
    local_return_(as$(u16)(__sum)); \
})
#define ____uint_addCarry8(_$lhs, _$rhs, _carry_in, _p_carry_out...) local_({ \
    let_(__sum, u32) = as$(u32)(as$(u8)(_$lhs)) + as$(u32)(as$(u8)(_$rhs)) + as$(u32)(as$(u8)(_carry_in)); \
    *(_p_carry_out) = as$(TypeOf(*(_p_carry_out)))(__sum >> u8_bits); \
    local_return_(as$(u8)(__sum)); \
})
#define __op__int_sub__step(_$lhs, _$rhs...) __op__int_sub(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define __op__int_sub(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    claim_assert(!__builtin_sub_overflow(__lhs, __rhs, &l0$((IntType)))); \
    local_return_(as$(IntType)(__lhs - __rhs)); \
})
#define __op__int_subOverflow(__lhs, __rhs, __out, __overflow, _$lhs, _$rhs, _$p_out...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    var_(__out, IntType) = 0; \
    let_(__overflow, bool) = bool_(__builtin_sub_overflow(__lhs, __rhs, &__out)); \
    *(_$p_out) = as$(TypeOf(*(_$p_out)))(__out); \
    local_return_(__overflow); \
})
#define __op__int_subWrap(__lhs, __rhs, __out, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    var_(__out, IntType) = 0; \
    (void)__builtin_sub_overflow(__lhs, __rhs, &__out); \
    local_return_(__out); \
})
#define __op__int_subSat(__lhs, __rhs, __out, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    var_(__out, IntType) = 0; \
    local_return_(__builtin_sub_overflow(__lhs, __rhs, &__out) ? as$(IntType)(isUInt$(IntType) ? int_limit_min$(IntType) : (__rhs < 0 ? int_limit_max$(IntType) : int_limit_min$(IntType))) : __out); \
})
#define __op__uint_subBorrow(__lhs, __rhs, __borrow_in, _$lhs, _$rhs, _borrow_in, _p_borrow_out...) local_({ \
    typedef TypeOfUnqual(_$lhs) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__lhs, UIntType) = _$lhs; \
    let_(__rhs, UIntType) = _$rhs; \
    let_(__borrow_in, UIntType) = _borrow_in; \
    local_return_(T_switch$((TypeOf(UIntType))( \
        T_case$((u8)(____uint_subBorrow8(__lhs, __rhs, __borrow_in, _p_borrow_out))), \
        T_case$((u16)(____uint_subBorrow16(__lhs, __rhs, __borrow_in, _p_borrow_out))), \
        T_case$((u32)(____uint_subBorrow32(__lhs, __rhs, __borrow_in, _p_borrow_out))), \
        pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
            T_case$((ulong)(____uint_subBorrowLong(__lhs, __rhs, __borrow_in, _p_borrow_out))), \
        )) T_delim(), \
        T_case$((u64)(____uint_subBorrow64(__lhs, __rhs, __borrow_in, _p_borrow_out))) \
    ))); \
})
#if UINT64_MAX == ULONG_MAX
#define ____uint_subBorrow64(_$lhs, _$rhs, _borrow_in, _p_borrow_out...) local_({ \
    var_(__borrow_out, ulong) = 0; \
    let_(__res, u64) = as$(u64)(__builtin_subcl( \
        as$(ulong)(_$lhs), as$(ulong)(_$rhs), as$(ulong)(_borrow_in), &__borrow_out \
    )); \
    *(_p_borrow_out) = as$(TypeOf(*(_p_borrow_out)))(__borrow_out); \
    local_return_(__res); \
})
#elif UINT64_MAX == ULLONG_MAX
#define ____uint_subBorrow64(_$lhs, _$rhs, _borrow_in, _p_borrow_out...) local_({ \
    var_(__borrow_out, unsigned long long) = 0; \
    let_(__res, u64) = as$(u64)(__builtin_subcll( \
        as$(unsigned long long)(_$lhs), as$(unsigned long long)(_$rhs), as$(unsigned long long)(_borrow_in), &__borrow_out \
    )); \
    *(_p_borrow_out) = as$(TypeOf(*(_p_borrow_out)))(__borrow_out); \
    local_return_(__res); \
})
#else
#define ____uint_subBorrow64(_$lhs, _$rhs, _borrow_in, _p_borrow_out...) local_({ \
    let_(__lhs, u64) = as$(u64)(_$lhs); \
    let_(__rhs, u64) = as$(u64)(_$rhs); \
    let_(__bin, u64) = as$(u64)(_borrow_in); \
    let_(__sub, u64) = as$(u64)(__rhs + __bin); \
    let_(__res, u64) = as$(u64)(__lhs - __sub); \
    *(_p_borrow_out) = as$(TypeOf(*(_p_borrow_out)))((__sub < __rhs) || (__lhs < __sub)); \
    local_return_(__res); \
})
#endif
#define ____uint_subBorrowLong(_$lhs, _$rhs, _borrow_in, _p_borrow_out...) local_({ \
    var_(__borrow_out, ulong) = 0; \
    let_(__res, ulong) = __builtin_subcl(as$(ulong)(_$lhs), as$(ulong)(_$rhs), as$(ulong)(_borrow_in), &__borrow_out); \
    *(_p_borrow_out) = as$(TypeOf(*(_p_borrow_out)))(__borrow_out); \
    local_return_(__res); \
})
#define ____uint_subBorrow32(_$lhs, _$rhs, _borrow_in, _p_borrow_out...) local_({ \
    var_(__borrow_out, u32) = 0; \
    let_(__res, u32) = __builtin_subc(as$(u32)(_$lhs), as$(u32)(_$rhs), as$(u32)(_borrow_in), &__borrow_out); \
    *(_p_borrow_out) = as$(TypeOf(*(_p_borrow_out)))(__borrow_out); \
    local_return_(__res); \
})
#define ____uint_subBorrow16(_$lhs, _$rhs, _borrow_in, _p_borrow_out...) local_({ \
    let_(__lhs, u32) = as$(u32)(as$(u16)(_$lhs)); \
    let_(__sub, u32) = as$(u32)(as$(u16)(_$rhs)) + as$(u32)(as$(u16)(_borrow_in)); \
    *(_p_borrow_out) = as$(TypeOf(*(_p_borrow_out)))(__lhs < __sub); \
    local_return_(as$(u16)(__lhs - __sub)); \
})
#define ____uint_subBorrow8(_$lhs, _$rhs, _borrow_in, _p_borrow_out...) local_({ \
    let_(__lhs, u32) = as$(u32)(as$(u8)(_$lhs)); \
    let_(__sub, u32) = as$(u32)(as$(u8)(_$rhs)) + as$(u32)(as$(u8)(_borrow_in)); \
    *(_p_borrow_out) = as$(TypeOf(*(_p_borrow_out)))(__lhs < __sub); \
    local_return_(as$(u8)(__lhs - __sub)); \
})
#define __op__int_mul__step(_$lhs, _$rhs...) __op__int_mul(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define __op__int_mul(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    claim_assert(!__builtin_mul_overflow(__lhs, __rhs, &l0$((IntType)))); \
    local_return_(as$(IntType)(__lhs * __rhs)); \
})
#define __op__int_mulOverflow(__lhs, __rhs, __out, __overflow, _$lhs, _$rhs, _$p_out...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    var_(__out, IntType) = 0; \
    let_(__overflow, bool) = bool_(__builtin_mul_overflow(__lhs, __rhs, &__out)); \
    *(_$p_out) = as$(TypeOf(*(_$p_out)))(__out); \
    local_return_(__overflow); \
})
#define __op__int_mulWrap(__lhs, __rhs, __out, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    var_(__out, IntType) = 0; \
    (void)__builtin_mul_overflow(__lhs, __rhs, &__out); \
    local_return_(__out); \
})
#define __op__int_mulSat(__lhs, __rhs, __out, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    var_(__out, IntType) = 0; \
    local_return_(__builtin_mul_overflow(__lhs, __rhs, &__out) ? as$(IntType)(isUInt$(IntType) ? int_limit_max$(IntType) : (((__lhs < 0) != (__rhs < 0)) ? int_limit_min$(IntType) : int_limit_max$(IntType))) : __out); \
})
#define __op__int_mulAdd(_$x, _$y, _$z...) int_add(int_mul(_$x, _$y), _$z)
#define __op__int_div__step(_$x, _$n...) __op__int_div(pp_uniqTok(x), pp_uniqTok(n), _$x, _$n)
#define __op__int_div(__x, __n, _$x, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _$x; \
    let_(__n, IntType) = _$n; \
    claim_assert(__n != 0); \
    claim_assert(isIInt$(IntType) ? !(__x == int_limit_min$(IntType) && __n == as$(IntType)(-1)) : true); \
    local_return_(as$(IntType)(__x / __n)); \
})

#define __op__iint_neg__step(_$x...) __op__iint_neg(pp_uniqTok(x), _$x)
#define __op__iint_neg(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _$x; \
    claim_assert(__x != int_limit_min$(IIntType)); \
    local_return_(as$(IIntType)(-__x)); \
})
#define __op__iint_negOverflow(__x, __out, __overflow, _$x, _$p_out...) local_({ \
    typedef TypeOfUnqual(_$x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _$x; \
    var_(__out, IIntType) = 0; \
    let_(__overflow, bool) = bool_(__builtin_sub_overflow(as$(IIntType)(0), __x, &__out)); \
    *(_$p_out) = as$(TypeOf(*(_$p_out)))(__out); \
    local_return_(__overflow); \
})
#define __op__iint_negWrap(__x, __out, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _$x; \
    var_(__out, IIntType) = 0; \
    (void)__builtin_sub_overflow(as$(IIntType)(0), __x, &__out); \
    local_return_(__out); \
})
#define __op__iint_negSat(__x, __out, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _$x; \
    var_(__out, IIntType) = 0; \
    local_return_(__builtin_sub_overflow(as$(IIntType)(0), __x, &__out) ? int_limit_max$(IIntType) : __out); \
})
#define __op__iint_abs(_$x...) local_({ \
    typedef TypeOfUnqual(_$x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _$x; \
    claim_assert(__x != int_limit_min$(IIntType)); \
    local_return_(T_switch$((TypeOf(IIntType))( \
        T_case$((i8)(__builtin_abs(as$(i8)(__x)))), \
        T_case$((i16)(__builtin_abs(as$(i16)(__x)))), \
        T_case$((i32)(__builtin_abs(as$(i32)(__x)))), \
        pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
            T_case$((ilong)(__builtin_labs(as$(ilong)(__x)))), \
        )) T_delim(), \
        T_case$((i64)(__builtin_llabs(as$(i64)(__x)))) \
    ))); \
})
#define __op__iint_absOverflow(__x, __out, __overflow, _$x, _$p_out...) local_({ \
    typedef TypeOfUnqual(_$x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _$x; \
    var_(__out, IIntType) = __x; \
    let_(__overflow, bool) = bool_(__x < 0 && __builtin_sub_overflow(as$(IIntType)(0), __x, &__out)); \
    *(_$p_out) = as$(TypeOf(*(_$p_out)))(__out); \
    local_return_(__overflow); \
})
#define __op__iint_absSat(__x, __out, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _$x; \
    var_(__out, IIntType) = __x; \
    local_return_((__x < 0 && __builtin_sub_overflow(as$(IIntType)(0), __x, &__out)) ? int_limit_max$(IIntType) : __out); \
})

/*========== Integer Comparison Implementation ==============================*/

#define __op__int_eql(__lhs, __rhs, _$lhs, _$rhs...) __op__int_eq(__lhs, __rhs, _$lhs, _$rhs)
#define __op__int_neq(__lhs, __rhs, _$lhs, _$rhs...) __op__int_ne(__lhs, __rhs, _$lhs, _$rhs)
#define __op__int_ord__step(__lhs, __rhs, _$lhs, _$rhs...) __op__int_ord(__lhs, __rhs, _$lhs, _$rhs)
#define __op__int_ord(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    local_return_(pri_ord_static(__lhs, __rhs)); \
})
#define __op__int_eq(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    local_return_(pri_eq(__lhs, __rhs)); \
})
#define __op__int_ne(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    local_return_(pri_ne(__lhs, __rhs)); \
})
#define __op__int_lt(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    local_return_(pri_lt(__lhs, __rhs)); \
})
#define __op__int_gt(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    local_return_(pri_gt(__lhs, __rhs)); \
})
#define __op__int_le(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    local_return_(pri_le(__lhs, __rhs)); \
})
#define __op__int_ge(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    local_return_(pri_ge(__lhs, __rhs)); \
})

/*========== Integer Sign Implementation ===================================*/

#define ____iint_sgn(_$x...) local_({ \
    typedef TypeOfUnqual(_$x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _$x; \
    local_return_(pri_sgn_static(__x)); \
})
#define ____iint_sgnBit_static(_$x...) bool_((_$x) < 0)
#define __step__iint_sgnBit(_$x...) ____iint_sgnBit(pp_uniqTok(x), _$x)
#define ____iint_sgnBit(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _$x; \
    local_return_(iint_sgnBit_static(__x)); \
})

/*========== Integer Min/Max/Clamp Implementation ===========================*/

#define __op__int_min(__lhs, __rhs, _$lhs, _$rhs...) ({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    as$(IntType)(pri_min_static(__lhs, __rhs)); \
})
#define __op__int_max(__lhs, __rhs, _$lhs, _$rhs...) ({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    as$(IntType)(pri_max_static(__lhs, __rhs)); \
})
#define __op__int_clamp__step(_$x, _$lo, _$hi...) __op__int_clamp(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _$x, _$lo, _$hi)
#define __op__int_clamp(__x, __lo, __hi, _$x, _$lo, _$hi...) ({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__lo, IntType) = _$lo; \
    let_(__hi, IntType) = _$hi; \
    claim_assert(__lo <= __hi); \
    int_min(int_max(__lo, __x), __hi); \
})

/*========== Bitwise Operations Implementation ==============================*/

#define __op__int_not_static(_$x...) (as$(TypeOf(_$x))((~(_$x))))
#define __step__int_not(_$x...) __op__int_not(pp_uniqTok(x), _$x)
#define __op__int_not(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _$x; \
    local_return_(as$(IntType)(~__x)); \
})
#define __op__int_shl_static(_$x, _$y...) (as$(TypeOf(_$x))((_$x) << (_$y)))
#define __step__int_shl(_$x, _$n...) __op__int_shl(pp_uniqTok(x), _$x, pp_uniqTok(n), _$n)
#define __op__int_shl(__x, _$x, __n, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__n, u32) = _$n; \
    claim_assert(__n < int_bits$(IntType)); \
    local_return_(as$(IntType)(__x << __n)); \
})
#define __op__int_shr_static(_$x, _$y...) (as$(TypeOf(_$x))((_$x) >> (_$y)))
#define __step__int_shr(_$x, _$n...) __op__int_shr(pp_uniqTok(x), _$x, pp_uniqTok(n), _$n)
#define __op__int_shr(__x, _$x, __n, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__n, u32) = _$n; \
    claim_assert(__n < int_bits$(IntType)); \
    local_return_(as$(IntType)(__x >> __n)); \
})
#define __op__int_and_static(_$x, _$y...) (as$(TypeOf(_$x))((_$x) & (_$y)))
#define __step__int_and(_$lhs, _$rhs...) __op__int_and(pp_uniqTok(lhs), _$lhs, pp_uniqTok(rhs), _$rhs)
#define __op__int_and(__lhs, _$lhs, __rhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    local_return_(as$(IntType)(__lhs & __rhs)); \
})
#define __op__int_xor_static(_$x, _$y...) (as$(TypeOf(_$x))((_$x) ^ (_$y)))
#define __step__int_xor(_$lhs, _$rhs...) __op__int_xor(pp_uniqTok(lhs), _$lhs, pp_uniqTok(rhs), _$rhs)
#define __op__int_xor(__lhs, _$lhs, __rhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    local_return_(as$(IntType)(__lhs ^ __rhs)); \
})
#define __op__int_or_static(_$x, _$y...) (as$(TypeOf(_$x))((_$x) | (_$y)))
#define __step__int_or(_$lhs, _$rhs...) __op__int_or(pp_uniqTok(lhs), _$lhs, pp_uniqTok(rhs), _$rhs)
#define __op__int_or(__lhs, _$lhs, __rhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__lhs, IntType) = _$lhs; \
    let_(__rhs, IntType) = _$rhs; \
    local_return_(as$(IntType)(__lhs | __rhs)); \
})

#define __op__int_setBit_static(_$x, _$bit...) (as$(TypeOf(_$x))((_$x) | (as$(TypeOf(_$x))(1) << (_$bit))))
#define __op__int_setBit(_$x, _$bit...) ({ \
    typedef TypeOf(_$x) IntType; \
    as$(IntType)(_$x | int_shl(as$(IntType)(1), _$bit)); \
})
#define __op__int_resetBit_static(_$x, _$bit...) (as$(TypeOf(_$x))((_$x) & ~(as$(TypeOf(_$x))(1) << (_$bit))))
#define __op__int_resetBit(_$x, _$bit...) ({ \
    typedef TypeOf(_$x) IntType; \
    as$(IntType)(_$x & ~int_shl(as$(IntType)(1), _$bit)); \
})
#define __op__int_toggleBit_static(_$x, _$bit...) (as$(TypeOf(_$x))((_$x) ^ (as$(TypeOf(_$x))(1) << (_$bit))))
#define __op__int_toggleBit(_$x, _$bit...) ({ \
    typedef TypeOf(_$x) IntType; \
    as$(IntType)(_$x ^ int_shl(as$(IntType)(1), _$bit)); \
})

#define __step__int_maskLo_static$(...) __step__int_maskLo_static$__emit(__step__int_maskLo_static$__parse __VA_ARGS__)
#define __step__int_maskLo_static$__parse(_$T...) _$T,
#define __step__int_maskLo_static$__emit(...) ____int_maskLo_static$(__VA_ARGS__)
#define ____int_maskLo_static$(_$T, _$n...) ( \
    as$(_$T)((_$n == int_bits$(_$T)) ? as$(_$T)(~0) : (as$(_$T)(1) << _$n) - as$(_$T)(1)) \
)
#define __step__int_maskLo$(...) __step__int_maskLo$__emit(__step__int_maskLo$__parse __VA_ARGS__)
#define __step__int_maskLo$__parse(_$T...) _$T, pp_uniqTok(n),
#define __step__int_maskLo$__emit(...) ____int_maskLo$(__VA_ARGS__)
#define ____int_maskLo$(_$T, __n, _$n...) (as$(_$T)(local_({ \
    let_(__n, u32) = _$n; \
    claim_assert(__n <= int_bits$(_$T)); \
    local_return_((__n == int_bits$(_$T)) ? as$(_$T)(~0) : (as$(_$T)(1) << __n) - as$(_$T)(1)); \
})))
#define __step__int_maskHi_static$(...) __step__int_maskHi_static$__emit(__step__int_maskHi_static$__parse __VA_ARGS__)
#define __step__int_maskHi_static$__parse(_$T...) _$T,
#define __step__int_maskHi_static$__emit(...) ____int_maskHi_static$(__VA_ARGS__)
#define ____int_maskHi_static$(_$T, _$n...) ( \
    as$(_$T)((_$n == 0) ? as$(_$T)(0) : as$(_$T)(as$(_$T)(~0) << (int_bits$(_$T) - _$n))) \
)
#define __step__int_maskHi$(...) __step__int_maskHi$__emit(__step__int_maskHi$__parse __VA_ARGS__)
#define __step__int_maskHi$__parse(_$T...) _$T, pp_uniqTok(n),
#define __step__int_maskHi$__emit(...) ____int_maskHi$(__VA_ARGS__)
#define ____int_maskHi$(_$T, __n, _$n...) (as$(_$T)(local_({ \
    let_(__n, u32) = _$n; \
    claim_assert(__n <= int_bits$(_$T)); \
    local_return_((__n == 0) ? as$(_$T)(0) : as$(_$T)(as$(_$T)(~0) << (int_bits$(_$T) - __n))); \
})))
#define __step__int_mask_static$(...) __step__int_mask_static$__emit(__step__int_mask_static$__parse __VA_ARGS__)
#define __step__int_mask_static$__parse(_$T...) _$T, __step__int_mask_static$__parseNext
#define __step__int_mask_static$__parseNext(_$off, _$len...) _$off, _$len
#define __step__int_mask_static$__emit(...) ____int_mask_static$(__VA_ARGS__)
#define ____int_mask_static$(_$T, _$off, _$len...) ( \
    as$(_$T)(int_maskLo_static$((_$T)(_$len)) << _$off) \
)
#define __step__int_mask$(...) __step__int_mask$__emit(__step__int_mask$__parse __VA_ARGS__)
#define __step__int_mask$__parse(_$T...) _$T, __step__int_mask$__parseNext
#define __step__int_mask$__parseNext(_$off, _$len...) pp_uniqTok(off), _$off, pp_uniqTok(len), _$len
#define __step__int_mask$__emit(...) ____int_mask$(__VA_ARGS__)
#define ____int_mask$(_$T, __off, _$off, __len, _$len...) (as$(_$T)(local_({ \
    let_(__off, u32) = _$off; \
    let_(__len, u32) = _$len; \
    claim_assert((__off + __len) <= int_bits$(_$T)); \
    local_return_(int_maskLo$((_$T)(__len)) << __off); \
})))

/*========== Bit Manipulation Operations Implementation =====================*/

#define ____int_countOnes_static(_$x /*: IntType */... /*(u32)*/) (as$(u32)( \
    _$x == 0 \
        ? 0 \
        : T_switch$((TypeOf(_$x))( \
              T_case$((u8)(raw_countOnes8(as$(u8)(_$x)))), \
              T_case$((i8)(raw_countOnes8(as$(i8)(_$x)))), \
              T_case$((i16)(raw_countOnes16(as$(i16)(_$x)))), \
              T_case$((u16)(raw_countOnes16(as$(u16)(_$x)))), \
              T_case$((u32)(raw_countOnes32(as$(u32)(_$x)))), \
              T_case$((i32)(raw_countOnes32(as$(i32)(_$x)))), \
              pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
                  T_case$((ulong)(raw_countOnesLong(as$(ulong)(_$x)))), \
                  T_case$((ilong)(raw_countOnesLong(as$(ilong)(_$x)))), \
              )) T_delim(), \
              T_case$((u64)(raw_countOnes64(as$(u64)(_$x)))), \
              T_case$((i64)(raw_countOnes64(as$(i64)(_$x)))) \
          )) \
))
#define __step__int_countOnes(_$x...) ____int_countOnes(pp_uniqTok(x), _$x)
#define ____int_countOnes(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_countOnes_static(__x)); \
})

#define ____int_countZeros_static(_$x /*: IntType */... /*(u32)*/) ( \
    int_bits$(TypeOf(_$x)) - int_countOnes_static(_$x) \
)
#define __step__int_countZeros(_$x...) ____int_countZeros(pp_uniqTok(x), _$x)
#define ____int_countZeros(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_countZeros_static(__x)); \
})

#define ____iint_leadingRedundantSgnBits_static(_$x /*: IIntType */... /*(u32)*/) (as$(u32)( \
    T_switch$((TypeOf(_$x))( \
        T_case$((i8)(raw_leadingRedundantSgnBits8(as$(i8)(_$x)))), \
        T_case$((i16)(raw_leadingRedundantSgnBits16(as$(i16)(_$x)))), \
        T_case$((i32)(raw_leadingRedundantSgnBits32(as$(i32)(_$x)))), \
        pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
            T_case$((ilong)(raw_leadingRedundantSgnBitsLong(as$(ilong)(_$x)))), \
        )) T_delim(), \
        T_case$((i64)(raw_leadingRedundantSgnBits64(as$(i64)(_$x)))) \
    )) \
))
#define __step__iint_leadingRedundantSgnBits(_$x...) ____iint_leadingRedundantSgnBits(pp_uniqTok(x), _$x)
#define ____iint_leadingRedundantSgnBits(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IIntType; \
    claim_assert_static(isIInt$(IIntType)); \
    let_(__x, IIntType) = _$x; \
    local_return_(iint_leadingRedundantSgnBits_static(__x)); \
})

#define ____int_leadingOnes_static(_$x /*: IntType */... /*(u32)*/) \
    int_leadingZeros_static(as$(TypeOf(_$x))(~(_$x)))
#define __step__int_leadingOnes(_$x...) ____int_leadingOnes(pp_uniqTok(x), _$x)
#define ____int_leadingOnes(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_leadingOnes_static(__x)); \
})

#define ____int_leadingZeros_static(_$x /*: IntType */... /*(u32)*/) (as$(u32)( \
    _$x == 0 \
        ? int_bits$(TypeOf(_$x)) \
        : T_switch$((TypeOf(_$x))( \
              T_case$((u8)(raw_leadingZeros8(as$(u8)(_$x)))), \
              T_case$((i8)(raw_leadingZeros8(as$(i8)(_$x)))), \
              T_case$((u16)(raw_leadingZeros16(as$(u16)(_$x)))), \
              T_case$((i16)(raw_leadingZeros16(as$(i16)(_$x)))), \
              T_case$((u32)(raw_leadingZeros32(as$(u32)(_$x)))), \
              T_case$((i32)(raw_leadingZeros32(as$(i32)(_$x)))), \
              pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
                  T_case$((ulong)(raw_leadingZerosLong(as$(ulong)(_$x)))), \
                  T_case$((ilong)(raw_leadingZerosLong(as$(ilong)(_$x)))), \
              )) T_delim(), \
              T_case$((u64)(raw_leadingZeros64(as$(u64)(_$x)))), \
              T_case$((i64)(raw_leadingZeros64(as$(i64)(_$x)))) \
          )) \
))
#define __step__int_leadingZeros(_$x...) ____int_leadingZeros(pp_uniqTok(x), _$x)
#define ____int_leadingZeros(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_leadingZeros_static(__x)); \
})

#define ____int_trailingOnes_static(_$x /*: IntType */... /*(u32)*/) \
    int_trailingZeros_static(as$(TypeOf(_$x))(~(_$x)))
#define __step__int_trailingOnes(_$x...) ____int_trailingOnes(pp_uniqTok(x), _$x)
#define ____int_trailingOnes(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_trailingOnes_static(__x)); \
})

#define ____int_trailingZeros_static(_$x /*: IntType */... /*(u32)*/) (as$(u32)( \
    _$x == 0 \
        ? int_bits$(TypeOf(_$x)) \
        : T_switch$((TypeOf(_$x))( \
              T_case$((u8)(raw_trailingZeros8(as$(u8)(_$x)))), \
              T_case$((i8)(raw_trailingZeros8(as$(i8)(_$x)))), \
              T_case$((u16)(raw_trailingZeros16(as$(u16)(_$x)))), \
              T_case$((i16)(raw_trailingZeros16(as$(i16)(_$x)))), \
              T_case$((u32)(raw_trailingZeros32(as$(u32)(_$x)))), \
              T_case$((i32)(raw_trailingZeros32(as$(i32)(_$x)))), \
              pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
                  T_case$((ulong)(raw_trailingZerosLong(as$(ulong)(_$x)))), \
                  T_case$((ilong)(raw_trailingZerosLong(as$(ilong)(_$x)))), \
              )) T_delim(), \
              T_case$((u64)(raw_trailingZeros64(as$(u64)(_$x)))), \
              T_case$((i64)(raw_trailingZeros64(as$(i64)(_$x)))) \
          )) \
))
#define __step__int_trailingZeros(_$x...) ____int_trailingZeros(pp_uniqTok(x), _$x)
#define ____int_trailingZeros(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_trailingZeros_static(__x)); \
})

#define ____int_firstSetBit_static(_$x /*: IntType */... /*(u32)*/) (as$(u32)( \
    T_switch$((TypeOf(_$x))( \
        T_case$((u8)(raw_firstSetBit8(as$(u8)(_$x)))), \
        T_case$((i8)(raw_firstSetBit8(as$(i8)(_$x)))), \
        T_case$((u16)(raw_firstSetBit16(as$(u16)(_$x)))), \
        T_case$((i16)(raw_firstSetBit16(as$(i16)(_$x)))), \
        T_case$((u32)(raw_firstSetBit32(as$(u32)(_$x)))), \
        T_case$((i32)(raw_firstSetBit32(as$(i32)(_$x)))), \
        pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
            T_case$((ulong)(raw_firstSetBitLong(as$(ulong)(_$x)))), \
            T_case$((ilong)(raw_firstSetBitLong(as$(ilong)(_$x)))), \
        )) T_delim(), \
        T_case$((u64)(raw_firstSetBit64(as$(u64)(_$x)))), \
        T_case$((i64)(raw_firstSetBit64(as$(i64)(_$x)))) \
    )) \
))
#define __step__int_firstSetBit(_$x...) ____int_firstSetBit(pp_uniqTok(x), _$x)
#define ____int_firstSetBit(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_firstSetBit_static(__x)); \
})

#define ____int_lastSetBit_static(_$x /*: IntType */... /*(u32)*/) (as$(u32)( \
    (_$x) == 0 ? 0 : int_bits$(TypeOf(_$x)) - int_leadingZeros_static(_$x) \
))
#define __step__int_lastSetBit(_$x...) ____int_lastSetBit(pp_uniqTok(x), _$x)
#define ____int_lastSetBit(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_lastSetBit_static(__x)); \
})

#define ____int_parity_static(_$x /*: IntType */... /*(bool)*/) bool_( \
    T_switch$((TypeOf(_$x))( \
        T_case$((u8)(raw_parity8(as$(u8)(_$x)))), \
        T_case$((i8)(raw_parity8(as$(i8)(_$x)))), \
        T_case$((u16)(raw_parity16(as$(u16)(_$x)))), \
        T_case$((i16)(raw_parity16(as$(i16)(_$x)))), \
        T_case$((u32)(raw_parity32(as$(u32)(_$x)))), \
        T_case$((i32)(raw_parity32(as$(i32)(_$x)))), \
        pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
            T_case$((ulong)(raw_parityLong(as$(ulong)(_$x)))), \
            T_case$((ilong)(raw_parityLong(as$(ilong)(_$x)))), \
        )) T_delim(), \
        T_case$((u64)(raw_parity64(as$(u64)(_$x)))), \
        T_case$((i64)(raw_parity64(as$(i64)(_$x)))) \
    )) \
)
#define __step__int_parity(_$x...) ____int_parity(pp_uniqTok(x), _$x)
#define ____int_parity(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_parity_static(__x)); \
})

#define ____int_bitWidth_static(_$x /*: IntType */... /*(u32)*/) int_lastSetBit_static(_$x)
#define __step__int_bitWidth(_$x...) ____int_bitWidth(pp_uniqTok(x), _$x)
#define ____int_bitWidth(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_bitWidth_static(__x)); \
})

#define ____int_bitFloor_static(_$x /*: IntType */... /*(IntType)*/) (as$(TypeOf(_$x))( \
    (_$x) <= 0 ? 0 : int_shl_static(as$(TypeOf(_$x))(1), int_bitWidth_static(_$x) - 1) \
))
#define __step__int_bitFloor(_$x...) ____int_bitFloor(pp_uniqTok(x), _$x)
#define ____int_bitFloor(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    claim_assert(!isIInt$(IntType) || __x >= 0); \
    local_return_(int_bitFloor_static(__x)); \
})

#define ____int_bitCeil_static(_$x /*: IntType */... /*(IntType)*/) (as$(TypeOf(_$x))( \
    (_$x) <= 1 ? 1 : int_shl_static(as$(TypeOf(_$x))(1), int_bitWidth_static(as$(TypeOf(_$x))((_$x) - 1))) \
))
#define __step__int_bitCeil(_$x...) ____int_bitCeil(pp_uniqTok(x), _$x)
#define ____int_bitCeil(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    claim_assert(!isIInt$(IntType) || __x >= 0); \
    claim_assert(__x <= int_bitFloor_static(int_limit_max$(IntType))); \
    local_return_(int_bitCeil_static(__x)); \
})

#define ____int_swapBytes_static(_$x...) (as$(TypeOf(_$x))( \
    _$x == 0 \
        ? 0 \
        : T_switch$((TypeOf(_$x))( \
              T_case$((u8)(as$(u8)(_$x))), \
              T_case$((i8)(as$(i8)(_$x))), \
              T_case$((u16)(raw_swapBytes16(as$(u16)(_$x)))), \
              T_case$((i16)(raw_swapBytes16(as$(i16)(_$x)))), \
              T_case$((u32)(raw_swapBytes32(as$(u32)(_$x)))), \
              T_case$((i32)(raw_swapBytes32(as$(i32)(_$x)))), \
              pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
                  T_case$((ulong)(raw_swapBytesLong(as$(ulong)(_$x)))), \
                  T_case$((ilong)(raw_swapBytesLong(as$(ilong)(_$x)))), \
              )) T_delim(), \
              T_case$((u64)(raw_swapBytes64(as$(u64)(_$x)))), \
              T_case$((i64)(raw_swapBytes64(as$(i64)(_$x)))) \
          )) \
))
#define __step__int_swapBytes(_$x...) ____int_swapBytes(pp_uniqTok(x), _$x)
#define ____int_swapBytes(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_swapBytes_static(__x)); \
})

#define ____int_rotateLeft_static(_$x, _$y...) (as$(TypeOf(_$x))( \
    _$x == 0 \
        ? 0 \
        : T_switch$((TypeOf(_$x))( \
              T_case$((u8)(raw_rotateLeft8(as$(u8)(_$x), as$(u8)(_$y)))), \
              T_case$((i8)(raw_rotateLeft8(as$(i8)(_$x), as$(i8)(_$y)))), \
              T_case$((u16)(raw_rotateLeft16(as$(u16)(_$x), as$(u16)(_$y)))), \
              T_case$((i16)(raw_rotateLeft16(as$(i16)(_$x), as$(i16)(_$y)))), \
              T_case$((u32)(raw_rotateLeft32(as$(u32)(_$x), as$(u32)(_$y)))), \
              T_case$((i32)(raw_rotateLeft32(as$(i32)(_$x), as$(i32)(_$y)))), \
              pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
                  T_case$((ulong)(raw_rotateLeftLong(as$(ulong)(_$x), as$(ulong)(_$y)))), \
                  T_case$((ilong)(raw_rotateLeftLong(as$(ilong)(_$x), as$(ilong)(_$y)))), \
              )) T_delim(), \
              T_case$((u64)(raw_rotateLeft64(as$(u64)(_$x), as$(u64)(_$y)))), \
              T_case$((i64)(raw_rotateLeft64(as$(i64)(_$x), as$(i64)(_$y)))) \
          )) \
))
#define __step__int_rotateLeft(_$x, _$y...) ____int_rotateLeft(pp_uniqTok(x), pp_uniqTok(y), _$x, _$y)
#define ____int_rotateLeft(__x, __y, _$x, _$y...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__y, IntType) = _$y; \
    local_return_(int_rotateLeft_static(__x, __y)); \
})

#define ____int_rotateRight_static(_$x, _$y...) (as$(TypeOf(_$x))( \
    _$x == 0 \
        ? 0 \
        : T_switch$((TypeOf(_$x))( \
              T_case$((u8)(raw_rotateRight8(as$(u8)(_$x), as$(u8)(_$y)))), \
              T_case$((i8)(raw_rotateRight8(as$(i8)(_$x), as$(i8)(_$y)))), \
              T_case$((u16)(raw_rotateRight16(as$(u16)(_$x), as$(u16)(_$y)))), \
              T_case$((i16)(raw_rotateRight16(as$(i16)(_$x), as$(i16)(_$y)))), \
              T_case$((u32)(raw_rotateRight32(as$(u32)(_$x), as$(u32)(_$y)))), \
              T_case$((i32)(raw_rotateRight32(as$(i32)(_$x), as$(i32)(_$y)))), \
              pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
                  T_case$((ulong)(raw_rotateRightLong(as$(ulong)(_$x), as$(ulong)(_$y)))), \
                  T_case$((ilong)(raw_rotateRightLong(as$(ilong)(_$x), as$(ilong)(_$y)))), \
              )) T_delim(), \
              T_case$((u64)(raw_rotateRight64(as$(u64)(_$x), as$(u64)(_$y)))), \
              T_case$((i64)(raw_rotateRight64(as$(i64)(_$x), as$(i64)(_$y)))) \
          )) \
))
#define __step__int_rotateRight(_$x, _$y...) ____int_rotateRight(pp_uniqTok(x), pp_uniqTok(y), _$x, _$y)
#define ____int_rotateRight(__x, __y, _$x, _$y...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__y, IntType) = _$y; \
    local_return_(int_rotateRight_static(__x, __y)); \
})

#define ____int_reverseBits_static(_$x...) (as$(TypeOf(_$x))( \
    _$x == 0 \
        ? 0 \
        : T_switch$((TypeOf(_$x))( \
              T_case$((u8)(raw_reverseBits8(as$(u8)(_$x)))), \
              T_case$((i8)(raw_reverseBits8(as$(i8)(_$x)))), \
              T_case$((u16)(raw_reverseBits16(as$(u16)(_$x)))), \
              T_case$((i16)(raw_reverseBits16(as$(i16)(_$x)))), \
              T_case$((u32)(raw_reverseBits32(as$(u32)(_$x)))), \
              T_case$((i32)(raw_reverseBits32(as$(i32)(_$x)))), \
              pp_if_(abi_long_needs_distinct_int_cases)(pp_then_( \
                  T_case$((ulong)(raw_reverseBitsLong(as$(ulong)(_$x)))), \
                  T_case$((ilong)(raw_reverseBitsLong(as$(ilong)(_$x)))), \
              )) T_delim(), \
              T_case$((u64)(raw_reverseBits64(as$(u64)(_$x)))), \
              T_case$((i64)(raw_reverseBits64(as$(i64)(_$x)))) \
          )) \
))
#define __step__int_reverseBits(_$x...) ____int_reverseBits(pp_uniqTok(x), _$x)
#define ____int_reverseBits(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    local_return_(int_reverseBits_static(__x)); \
})

#define __op__int_hasBit(__x, __bit, _$x, _$bit...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__bit, u32) = _$bit; \
    claim_assert(__bit < int_bits$(IntType)); \
    local_return_(bool_((__x & int_shl(as$(IntType)(1), __bit)) != 0)); \
})
#define __op__int_extractBits(__x, __off, __len, _$x, _$off, _$len...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__off, u32) = _$off; \
    let_(__len, u32) = _$len; \
    claim_assert((__off + __len) <= int_bits$(IntType)); \
    local_return_(as$(IntType)((__x >> __off) & int_maskLo$((IntType)(__len)))); \
})
#define __op__int_replaceBits(__x, __off, __len, __bits, _$x, _$off, _$len, _$bits...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    let_(__x, IntType) = _$x; \
    let_(__off, u32) = _$off; \
    let_(__len, u32) = _$len; \
    let_(__bits, IntType) = _$bits; \
    let_(__mask, IntType) = int_mask$((IntType)(__off, __len)); \
    local_return_(as$(IntType)((__x & ~__mask) | ((int_shl(__bits, __off)) & __mask))); \
})

/*========== Bit Manipulation Implementation ================================*/

#if UNUSED_CODE
#define __op__int_rotateLeft(__x, __n, _$x, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _$x; \
    let_(__n, u32) = _$n; \
    const u32 __mask = sizeof(IntType) * 8 - 1; \
    __n &= __mask; \
    as$(IntType)((__x << __n) | (__x >> ((-__n) & __mask))); \
})
#define __op__int_rotateRight(__x, __n, _$x, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _$x; \
    let_(__n, u32) = _$n; \
    const u32 __mask = sizeof(IntType) * 8 - 1; \
    __n &= __mask; \
    as$(IntType)((__x >> __n) | (__x << ((-__n) & __mask))); \
})
#define __op__int_swapBytes(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _$x; \
    as$(IntType)(_Generic(sizeof(IntType), 1: __x, 2: __builtin_bswap16((u16)__x), 4: __builtin_bswap32((u32)__x), 8: __builtin_bswap64((u64)__x))); \
})
#define __op__int_reverseBits(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _$x; \
    as$(IntType)(_Generic(sizeof(IntType), 1: __builtin_bitreverse8((u8)__x), 2: __builtin_bitreverse16((u16)__x), 4: __builtin_bitreverse32((u32)__x), 8: __builtin_bitreverse64((u64)__x))); \
})
#endif /* UNUSED_CODE */

/*========== Integer Exponential and Logarithm Implementation ===================*/

#define __step__uint_exp10_static$(...) __step__uint_exp10_static$__emit(__step__uint_exp10_static$__parse __VA_ARGS__)
#define __step__uint_exp10_static$__parse(_$T...) _$T,
#define __step__uint_exp10_static$__emit(...) ____uint_exp10_static$(__VA_ARGS__)
/* clang-format off */
#define ____uint_exp10_static$(_$T, _$n...) (as$(_$T)( \
    (_$n) < 10 ? ____uint_exp10_static$__values0to9(_$n) : \
    (_$n) < 20 ? ____uint_exp10_static$__values10to19(_$n) : u32_(0) /* Overflow (> 10^19) for u64 */ \
))
#define ____uint_exp10_static$__values0to9(_$n) ( \
    (_$n) == 0 ? u32_(1u) : \
    (_$n) == 1 ? u32_(10u) : \
    (_$n) == 2 ? u32_(100u) : \
    (_$n) == 3 ? u32_(1,000u) : \
    (_$n) == 4 ? u32_(10,000u) : \
    (_$n) == 5 ? u32_(100,000u) : \
    (_$n) == 6 ? u32_(1,000,000u) : \
    (_$n) == 7 ? u32_(10,000,000u) : \
    (_$n) == 8 ? u32_(100,000,000u) : \
                u32_(1,000,000,000u)/* 9 */ \
)
#define ____uint_exp10_static$__values10to19(_$n) ( \
    (_$n) == 10 ? u64_(10,000,000,000ull) : \
    (_$n) == 11 ? u64_(100,000,000,000ull) : \
    (_$n) == 12 ? u64_(1,000,000,000,000ull) : \
    (_$n) == 13 ? u64_(10,000,000,000,000ull) : \
    (_$n) == 14 ? u64_(100,000,000,000,000ull) : \
    (_$n) == 15 ? u64_(1,000,000,000,000,000ull) : \
    (_$n) == 16 ? u64_(10,000,000,000,000,000ull) : \
    (_$n) == 17 ? u64_(100,000,000,000,000,000ull) : \
    (_$n) == 18 ? u64_(1,000,000,000,000,000,000ull) : \
                 u64_(10,000,000,000,000,000,000ull) /* 19 (Max for u64) */ \
)
/* clang-format on */
#define __step__uint_exp10$(...) __step__uint_exp10$(__step__uint_exp10$__parse __VA_ARGS__)
#define __step__uint_exp10$__parse(_$T...) _$T, pp_uniqTok(n),
#define __step__uint_exp10$__emit(...) ____uint_exp10$(__VA_ARGS__)
#define ____uint_exp10$(_$T, __n, _$n...) local_({ \
    typedef TypeOfUnqual(_$n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _$n; \
    claim_assert(0 < __n); \
    local_return_(uint_exp10_static$((_$T)(__n))); \
})

#define __step__uint_exp2_static$(...) __step__uint_exp2_static$__emit(__step__uint_exp2_static$__parse __VA_ARGS__)
#define __step__uint_exp2_static$__parse(_$T...) _$T,
#define __step__uint_exp2_static$__emit(...) ____uint_exp2_static$(__VA_ARGS__)
#define ____uint_exp2_static$(_$T, _$n...) ( \
    as$(_$T)(1) << (_$n) \
)
#define __step__uint_exp2$(...) __step__uint_exp2$__emit(__step__uint_exp2$__parse __VA_ARGS__)
#define __step__uint_exp2$__parse(_$T...) _$T, pp_uniqTok(n),
#define __step__uint_exp2$__emit(...) ____uint_exp2$(__VA_ARGS__)
#define ____uint_exp2$(_$T, __n, _$n...) local_({ \
    typedef TypeOfUnqual(_$n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _$n; \
    claim_assert(0 <= __n); \
    local_return_(int_shl(as$(_$T)(1), intCast$((u32)(__n)))); \
})

#define __step__uint_pow2_static$(...) uint_exp2_static$(__VA_ARGS__)
#define __step__uint_pow2$(...) uint_exp2$(__VA_ARGS__)

#define __step__uint_pow2Round_static$(...) \
    __step__uint_pow2Round_static$__emit(__step__uint_pow2Round_static$__parse __VA_ARGS__)
#define __step__uint_pow2Round_static$__parse(_$T...) _$T,
#define __step__uint_pow2Round_static$__emit(...) ____uint_pow2Round_static$(__VA_ARGS__)
#define ____uint_pow2Round_static$(_$T, _$n...) (as$(_$T)( \
    (_$n) <= 1 \
        ? as$(TypeOf(_$n))(_$n) \
        : ((((_$n) - uint_pow2Floor_static$((TypeOf(_$n))(_$n))) < (uint_pow2Ceil_static$((TypeOf(_$n))(_$n)) - (_$n))) \
               ? uint_pow2Floor_static$((TypeOf(_$n))(_$n)) \
               : uint_pow2Ceil_static$((TypeOf(_$n))(_$n))) \
))
#define __step__uint_pow2Round$(...) __step__uint_pow2Round$__emit(__step__uint_pow2Round$__parse __VA_ARGS__)
#define __step__uint_pow2Round$__parse(_$T...) _$T, pp_uniqTok(n),
#define __step__uint_pow2Round$__emit(...) ____uint_pow2Round$(__VA_ARGS__)
#define ____uint_pow2Round$(_$T, __n, _$n...) local_({ \
    typedef TypeOfUnqual(_$n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _$n; \
    local_return_(uint_pow2Round_static$((_$T)(__n))); \
})

#define __step__uint_pow2Floor_static$(...) \
    __step__uint_pow2Floor_static$__emit(__step__uint_pow2Floor_static$__parse __VA_ARGS__)
#define __step__uint_pow2Floor_static$__parse(_$T...) _$T,
#define __step__uint_pow2Floor_static$__emit(...) ____uint_pow2Floor_static$(__VA_ARGS__)
#define ____uint_pow2Floor_static$(_$T, _$n...) (as$(_$T)( \
    (_$n) == 0 ? 0 : uint_exp2_static$((TypeOf(_$n))(uint_log2_static(_$n))) \
))
#define __step__uint_pow2Floor$(...) __step__uint_pow2Floor$__emit(__step__uint_pow2Floor$__parse __VA_ARGS__)
#define __step__uint_pow2Floor$__parse(_$T...) _$T, pp_uniqTok(n),
#define __step__uint_pow2Floor$__emit(...) ____uint_pow2Floor$(__VA_ARGS__)
#define ____uint_pow2Floor$(_$T, __n, _$n...) local_({ \
    typedef TypeOfUnqual(_$n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _$n; \
    local_return_(uint_pow2Floor_static$((_$T)(__n))); \
})

#define __step__uint_pow2Ceil_static$(...) \
    __step__uint_pow2Ceil_static$__emit(__step__uint_pow2Ceil_static$__parse __VA_ARGS__)
#define __step__uint_pow2Ceil_static$__parse(_$T...) _$T,
#define __step__uint_pow2Ceil_static$__emit(...) ____uint_pow2Ceil_static$(__VA_ARGS__)
#define ____uint_pow2Ceil_static$(_$T, _$n...) (as$(_$T)( \
    (_$n) <= 1 \
        ? as$(TypeOf(_$n))(1) \
        : uint_exp2_static$((TypeOf(_$n))( \
              as$(u32)(int_bits$(TypeOf(_$n))) \
              - int_leadingZeros_static(as$(TypeOf(_$n))((_$n) - 1)) \
          )) \
))
#define __step__uint_pow2Ceil$(...) __step__uint_pow2Ceil$__emit(__step__uint_pow2Ceil$__parse __VA_ARGS__)
#define __step__uint_pow2Ceil$__parse(_$T...) _$T, pp_uniqTok(n),
#define __step__uint_pow2Ceil$__emit(...) ____uint_pow2Ceil$(__VA_ARGS__)
#define ____uint_pow2Ceil$(_$T, __n, _$n...) local_({ \
    typedef TypeOfUnqual(_$n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _$n; \
    local_return_(uint_pow2Ceil_static$((_$T)(__n))); \
})

#define __step__uint_log2_static(_$n...) ( \
    (u32_(64) - u32_(1)) - int_leadingZeros_static(as$(u64)(_$n)) \
)
#define __step__uint_log2(_$n...) ____uint_log2(pp_uniqTok(n), _$n)
#define ____uint_log2(__n, _$n...) local_({ \
    typedef TypeOfUnqual(_$n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _$n; \
    claim_assert(0 < __n); \
    local_return_(uint_log2_static(__n)); \
})

/* clang-format off */
#define __step__uint_log10Round_static(_$n...) ( \
    (_$n) < u64_(31,622,776,602) \
        ? ____uint_log10Round_static__small(_$n) \
        : ____uint_log10Round_static__large(_$n) \
)
#define ____uint_log10Round_static__small(_$n...) \
    /* Small value thresholds */ ( \
    (_$n) < u32_(4u)             ? u32_(0) : \
    (_$n) < u32_(32u)            ? u32_(1) : \
    (_$n) < u32_(317u)           ? u32_(2) : \
    (_$n) < u32_(3,163u)         ? u32_(3) : \
    (_$n) < u32_(31,623u)        ? u32_(4) : \
    (_$n) < u32_(316,228u)       ? u32_(5) : \
    (_$n) < u32_(3,162,278u)     ? u32_(6) : \
    (_$n) < u32_(31,622,777u)    ? u32_(7) : \
    (_$n) < u32_(316,227,767u)   ? u32_(8) : \
    (_$n) < u32_(3,162,277,661u) ? u32_(9) : u32_(10) \
)
#define ____uint_log10Round_static__large(_$n...) \
    /* Large value thresholds */ ( \
    (_$n) < u64_(316,227,766,017ull)           ? u32_(11) : \
    (_$n) < u64_(3,162,277,660,169ull)         ? u32_(12) : \
    (_$n) < u64_(31,622,776,601,684ull)        ? u32_(13) : \
    (_$n) < u64_(316,227,766,016,838ull)       ? u32_(14) : \
    (_$n) < u64_(3,162,277,660,168,380ull)     ? u32_(15) : \
    (_$n) < u64_(31,622,776,601,683,794ull)    ? u32_(16) : \
    (_$n) < u64_(316,227,766,016,837,934ull)   ? u32_(17) : \
    (_$n) < u64_(3,162,277,660,168,379,332ull) ? u32_(18) : \
    u32_(19) /* roughly up to max u64 */ \
)
/* clang-format on */
#define __step__uint_log10Round(_$n...) ____uint_log10Round(pp_uniqTok(n), _$n)
#define ____uint_log10Round(__n, _$n...) local_({ \
    typedef TypeOfUnqual(_$n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _$n; \
    claim_assert(0 < __n); \
    local_return_(uint_log10Round_static(__n)); \
})

/* clang-format off */
#define __step__uint_log10Floor_static(_$n...) ( \
    (_$n) < u64_(10,000,000,000) \
        ? ____uint_log10Floor_static__small(_$n) \
        : ____uint_log10Floor_static__large(_$n) \
)
#define ____uint_log10Floor_static__small(_$n...) \
    /* Small value (0 - 10^10-1): 0 to 9 */ ( \
    (_$n) < u32_(10u)            ? u32_(0) : \
    (_$n) < u32_(100u)           ? u32_(1) : \
    (_$n) < u32_(1,000u)         ? u32_(2) : \
    (_$n) < u32_(10,000u)        ? u32_(3) : \
    (_$n) < u32_(100,000u)       ? u32_(4) : \
    (_$n) < u32_(1,000,000u)     ? u32_(5) : \
    (_$n) < u32_(10,000,000u)    ? u32_(6) : \
    (_$n) < u32_(100,000,000u)   ? u32_(7) : \
    (_$n) < u32_(1,000,000,000u) ? u32_(8) : u32_(9) \
)
#define ____uint_log10Floor_static__large(_$n...) \
    /* Large value (10^10+): 10 to 19 */ ( \
    (_$n) < u64_(100,000,000,000ull)            ? u32_(10) : \
    (_$n) < u64_(1,000,000,000,000ull)          ? u32_(11) : \
    (_$n) < u64_(10,000,000,000,000ull)         ? u32_(12) : \
    (_$n) < u64_(100,000,000,000,000ull)        ? u32_(13) : \
    (_$n) < u64_(1,000,000,000,000,000ull)      ? u32_(14) : \
    (_$n) < u64_(10,000,000,000,000,000ull)     ? u32_(15) : \
    (_$n) < u64_(100,000,000,000,000,000ull)    ? u32_(16) : \
    (_$n) < u64_(1,000,000,000,000,000,000ull)  ? u32_(17) : \
    (_$n) < u64_(10,000,000,000,000,000,000ull) ? u32_(18) : u32_(19) \
)
/* clang-format on */
#define __step__uint_log10Floor(_$n...) ____uint_log10Floor(pp_uniqTok(n), _$n)
#define ____uint_log10Floor(__n, _$n...) local_({ \
    typedef TypeOfUnqual(_$n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _$n; \
    claim_assert(0 < __n); \
    local_return_(uint_log10Floor_static(__n)); \
})

/* clang-format off */
#define __step__uint_log10Ceil_static(_$n...) ( \
    (_$n) <= u64_(10,000,000,000ull) \
        ? ____uint_log10Ceil_static__small(_$n) \
        : ____uint_log10Ceil_static__large(_$n) \
)
#define ____uint_log10Ceil_static__small(_$n...) \
    /* Small value (0 - 10^10) */ ( \
    (_$n) <= u32_(1u)             ? u32_(0) : \
    (_$n) <= u32_(10u)            ? u32_(1) : \
    (_$n) <= u32_(100u)           ? u32_(2) : \
    (_$n) <= u32_(1,000u)         ? u32_(3) : \
    (_$n) <= u32_(10,000u)        ? u32_(4) : \
    (_$n) <= u32_(100,000u)       ? u32_(5) : \
    (_$n) <= u32_(1,000,000u)     ? u32_(6) : \
    (_$n) <= u32_(10,000,000u)    ? u32_(7) : \
    (_$n) <= u32_(100,000,000u)   ? u32_(8) : \
    (_$n) <= u32_(1,000,000,000u) ? u32_(9) : u32_(10) \
)
#define ____uint_log10Ceil_static__large(_$n...) \
    /* Large value (10^10+ - max) */ ( \
    (_$n) <= u64_(100,000,000,000ull)            ? u32_(11) : \
    (_$n) <= u64_(1,000,000,000,000ull)          ? u32_(12) : \
    (_$n) <= u64_(10,000,000,000,000ull)         ? u32_(13) : \
    (_$n) <= u64_(100,000,000,000,000ull)        ? u32_(14) : \
    (_$n) <= u64_(1,000,000,000,000,000ull)      ? u32_(15) : \
    (_$n) <= u64_(10,000,000,000,000,000ull)     ? u32_(16) : \
    (_$n) <= u64_(100,000,000,000,000,000ull)    ? u32_(17) : \
    (_$n) <= u64_(1,000,000,000,000,000,000ull)  ? u32_(18) : \
    (_$n) <= u64_(10,000,000,000,000,000,000ull) ? u32_(19) : u32_(20) \
)
/* clang-format on */
#define __step__uint_log10Ceil(_$n...) ____uint_log10Ceil(pp_uniqTok(n), _$n)
#define ____uint_log10Ceil(__n, _$n...) local_({ \
    typedef TypeOfUnqual(_$n) UIntType; \
    claim_assert_static(isUInt$(UIntType)); \
    let_(__n, UIntType) = _$n; \
    claim_assert(0 < __n); \
    local_return_(uint_log10Ceil_static(__n)); \
})

/*========== Integer Query Implementation ===================================*/

#define __op__int_isZero(_$x...) bool_((_$x) == 0)
#define __op__int_isNonzero(_$x...) bool_((_$x) != 0)
#define __op__int_isOdd(_$x...) bool_((_$x & 1) != 0)
#define __op__int_isEven(_$x...) bool_((_$x & 1) == 0)
#define ____int_isPow2_static(_$x...) bool_(((_$x) > 0) && (((_$x) & as$(TypeOf(_$x))((_$x) - 1)) == 0))
#define __step__int_isPow2(_$x...) __op__int_isPow2(pp_uniqTok(x), _$x)
#define __op__int_isPow2(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _$x; \
    local_return_(int_isPow2_static(__x)); \
})
#define ____int_hasSglBit_static(_$x...) bool_(int_countOnes_static(_$x) == 1)
#define __step__int_hasSglBit(_$x...) __op__int_hasSglBit(pp_uniqTok(x), _$x)
#define __op__int_hasSglBit(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) IntType; \
    claim_assert_static(isInt$(IntType)); \
    let_(__x, IntType) = _$x; \
    local_return_(int_hasSglBit_static(__x)); \
})

/*========== Floating-Point Arithmetic Implementation =======================*/

#define __op__flt_add__step(_$lhs, _$rhs...) __op__flt_add(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define __op__flt_add(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(as$(FltType)(__lhs + __rhs)); \
})
#define __op__flt_sub__step(_$lhs, _$rhs...) __op__flt_sub(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define __op__flt_sub(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(as$(FltType)(__lhs - __rhs)); \
})
#define __op__flt_mul__step(_$lhs, _$rhs...) __op__flt_mul(pp_uniqTok(lhs), pp_uniqTok(rhs), _$lhs, _$rhs)
#define __op__flt_mul(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(as$(FltType)(__lhs * __rhs)); \
})
#define __op__flt_mulAdd__step(_$x, _$y, _$z...) __op__flt_mulAdd(pp_uniqTok(x), pp_uniqTok(y), pp_uniqTok(z), _$x, _$y, _$z)
#define __op__flt_mulAdd(__x, __y, __z, _$x, _$y, _$z...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__y, FltType) = _$y; \
    let_(__z, FltType) = _$z; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_fmaf(as$(f32)(__x), as$(f32)(__y), as$(f32)(__z)))), \
        T_case$((f64)(__builtin_fma(as$(f64)(__x), as$(f64)(__y), as$(f64)(__z)))) \
    ))); \
})
#define __step__flt_div(_$x, _$n...) ____flt_div(pp_uniqTok(x), _$x, pp_uniqTok(n), _$n)
#define ____flt_div(__x, _$x, __n, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    let_(__n, FltType) = _$n; \
    claim_assert(__n != 0); \
    local_return_(as$(FltType)(__x / __n)); \
})

#define __step__flt_neg(_$x...) ____flt_neg(pp_uniqTok(x), _$x)
#define ____flt_neg(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    local_return_(as$(FltType)(-__x)); \
})
#define ____flt_abs(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_fabsf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_fabs(as$(f64)(_$x)))) \
))
#define ____flt_inv_static(_$x...) (as$(TypeOf(_$x))(1) / (_$x))
#define __step__flt_inv(_$x...) ____flt_inv(pp_uniqTok(x), _$x)
#define ____flt_inv(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    claim_assert(__x != 0); \
    local_return_(flt_inv_static(__x)); \
})

/*========== Floating-Point Comparison Implementation =======================*/

#define __op__flt_eql(__lhs, __rhs, _$lhs, _$rhs...) __op__flt_eq(__lhs, __rhs, _$lhs, _$rhs)
#define __op__flt_neq(__lhs, __rhs, _$lhs, _$rhs...) __op__flt_ne(__lhs, __rhs, _$lhs, _$rhs)
#define __op__flt_ord__step(__lhs, __rhs, _$lhs, _$rhs...) __op__flt_ord(__lhs, __rhs, _$lhs, _$rhs)
#define __op__flt_ord(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(pri_ord_static(__lhs, __rhs)); \
})
#define __op__flt_ordApx( \
    __lhs, __rhs, __threshold, __mode, __diff, __tolerance, __ret, \
    _$lhs, _$rhs, _$threshold, _$mode... \
) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    let_(__threshold, FltType) = _$threshold; \
    let_(__mode, cmp_ApxMode) = _$mode; \
    claim_assert(0 <= __threshold); \
    claim_assert(__mode == cmp_ApxMode_abs || __mode == cmp_ApxMode_rel); \
    claim_assert(!flt_isNaN(__lhs) && !flt_isNaN(__rhs)); \
    var_(__ret, cmp_Ord) = flt_ord(__lhs, __rhs); \
    if (__ret != cmp_Ord_eq && flt_isFinite(__lhs) && flt_isFinite(__rhs)) { \
        let_(__diff, FltType) = flt_abs(flt_sub(__lhs, __rhs)); \
        let_(__tolerance, FltType) = (__mode == cmp_ApxMode_abs) \
                                       ? __threshold \
                                       : flt_mul(__threshold, flt_max(flt_abs(__lhs), flt_abs(__rhs))); \
        if (flt_le(__diff, __tolerance)) { \
            __ret = cmp_Ord_eq; \
        } \
    } \
    local_return_(__ret); \
})
#define __op__flt_eq(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(pri_eq(__lhs, __rhs)); \
})
#define __op__flt_ne(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(pri_ne(__lhs, __rhs)); \
})
#define __op__flt_lt(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(pri_lt(__lhs, __rhs)); \
})
#define __op__flt_gt(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(pri_gt(__lhs, __rhs)); \
})
#define __op__flt_le(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(pri_le(__lhs, __rhs)); \
})
#define __op__flt_ge(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(pri_ge(__lhs, __rhs)); \
})

/*========== Floating-Point Min/Max/Clamp Implementation ====================*/

#define __op__flt_min(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_fminf(as$(f32)(__lhs), as$(f32)(__rhs)))), \
        T_case$((f64)(__builtin_fmin(as$(f64)(__lhs), as$(f64)(__rhs)))) \
    ))); \
})
#define __op__flt_max(__lhs, __rhs, _$lhs, _$rhs...) local_({ \
    typedef TypeOfUnqual(_$lhs) FltType; \
    let_(__lhs, FltType) = _$lhs; \
    let_(__rhs, FltType) = _$rhs; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_fmaxf(as$(f32)(__lhs), as$(f32)(__rhs)))), \
        T_case$((f64)(__builtin_fmax(as$(f64)(__lhs), as$(f64)(__rhs)))) \
    ))); \
})
#define __op__flt_clamp__step(_$x, _$lo, _$hi...) __op__flt_clamp(pp_uniqTok(x), pp_uniqTok(lo), pp_uniqTok(hi), _$x, _$lo, _$hi)
#define __op__flt_clamp(__x, __lo, __hi, _$x, _$lo, _$hi...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__lo, FltType) = _$lo; \
    let_(__hi, FltType) = _$hi; \
    claim_assert(__lo <= __hi); \
    local_return_(flt_min(flt_max(__lo, __x), __hi)); \
})
#define __op__flt_clamp01(_$x...) flt_clamp(_$x, 0, 1)

/*========== Floating-Point Classification Implementation ===================*/

#define ____flt_isNaN(_$x...) bool_(__builtin_isnan(_$x))
#define ____flt_isInf(_$x...) bool_(__builtin_isinf(_$x))
#define ____flt_isFinite(_$x...) bool_(__builtin_isfinite(_$x))
#define ____flt_isNormal(_$x...) bool_(__builtin_isnormal(_$x))
#define ____flt_isSubnormal(_$x...) bool_(__builtin_fpclassify(0, 1, 2, 3, 4, _$x) == 3)
#define ____flt_isZero(_$x...) bool_(T_switch$((TypeOf(_$x))( \
    T_case$((f32)(as$(f32)(_$x) == 0.0f)), \
    T_case$((f64)(as$(f64)(_$x) == 0.0)) \
)))
#define ____flt_isNonzero(_$x...) bool_(!flt_isZero(_$x))
#define ____flt_sgn(_$x...) pri_sgn_static(_$x)
#define ____flt_sgnBit_static(_$x...) bool_(T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_copysignf(1.0f, as$(f32)(_$x)) < 0.0f)), \
    T_case$((f64)(__builtin_copysign(1.0, as$(f64)(_$x)) < 0.0)) \
)))
#define __step__flt_sgnBit(_$x...) ____flt_sgnBit(pp_uniqTok(x), _$x)
#define ____flt_sgnBit(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    local_return_(flt_sgnBit_static(__x)); \
})
#define __step__flt_isIntegral(_$x...) ____flt_isIntegral(pp_uniqTok(x), _$x)
#define ____flt_isIntegral(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(bool_(flt_isFinite(__x) && __x == flt_trunc(__x))); \
})

/*========== Floating-Point Rounding Implementation =========================*/

#define __step__flt_trunc(_$x...) ____flt_trunc(pp_uniqTok(x), _$x)
#define ____flt_trunc(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_truncf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_trunc(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_round(_$x...) ____flt_round(pp_uniqTok(x), _$x)
#define ____flt_round(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_roundf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_round(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_ceil(_$x...) ____flt_ceil(pp_uniqTok(x), _$x)
#define ____flt_ceil(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_ceilf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_ceil(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_floor(_$x...) ____flt_floor(pp_uniqTok(x), _$x)
#define ____flt_floor(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_floorf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_floor(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_nearbyint(_$x...) ____flt_nearbyint(pp_uniqTok(x), _$x)
#define ____flt_nearbyint(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_nearbyintf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_nearbyint(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_rint(_$x...) ____flt_rint(pp_uniqTok(x), _$x)
#define ____flt_rint(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_rintf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_rint(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_lround(_$x...) ____flt_lround(pp_uniqTok(x), _$x)
#define ____flt_lround(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(as$(ilong)(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_lroundf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_lround(as$(f64)(__x)))) \
    )))); \
})
#define __step__flt_llround(_$x...) ____flt_llround(pp_uniqTok(x), _$x)
#define ____flt_llround(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(as$(i64)(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_llroundf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_llround(as$(f64)(__x)))) \
    )))); \
})
#define __step__flt_lrint(_$x...) ____flt_lrint(pp_uniqTok(x), _$x)
#define ____flt_lrint(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(as$(ilong)(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_lrintf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_lrint(as$(f64)(__x)))) \
    )))); \
})
#define __step__flt_llrint(_$x...) ____flt_llrint(pp_uniqTok(x), _$x)
#define ____flt_llrint(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(as$(i64)(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_llrintf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_llrint(as$(f64)(__x)))) \
    )))); \
})

/*========== Floating-Point Power and Root Implementation ===================*/

#define __step__flt_sqrt(_$x...) ____flt_sqrt(pp_uniqTok(x), _$x)
#define ____flt_sqrt(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    claim_assert(__x >= 0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_sqrtf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_sqrt(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_cbrt(_$x...) ____flt_cbrt(pp_uniqTok(x), _$x)
#define ____flt_cbrt(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    claim_assert(__x >= 0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_cbrtf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_cbrt(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_sq(_$x...) ____flt_sq(pp_uniqTok(x), _$x)
#define ____flt_sq(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    local_return_(as$(FltType)(__x * __x)); \
})
#define __step__flt_cb(_$x...) ____flt_cb(pp_uniqTok(x), _$x)
#define ____flt_cb(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    claim_assert_static(isFlt$(FltType)); \
    let_(__x, FltType) = _$x; \
    local_return_(as$(FltType)(__x * __x * __x)); \
})
#define __step__flt_hypot(_$x, _$y...) ____flt_hypot(pp_uniqTok(x), _$x, pp_uniqTok(y), _$y)
#define ____flt_hypot(__x, _$x, __y, _$y...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__y, FltType) = _$y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_hypotf(as$(f32)(__x), as$(f32)(__y)))), \
        T_case$((f64)(__builtin_hypot(as$(f64)(__x), as$(f64)(__y)))) \
    ))); \
})

/*========== Floating-Point Exponential and Logarithm Implementation ========*/

#define ____flt_exp(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_expf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_exp(as$(f64)(_$x)))) \
))
#define ____flt_exp2(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_exp2f(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_exp2(as$(f64)(_$x)))) \
))
#define ____flt_exp10(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_powf(10.0f, as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_pow(10.0, as$(f64)(_$x)))) \
))
#define __step__flt_powI(_$x, _$y...) ____flt_powI(pp_uniqTok(x), _$x, pp_uniqTok(y), _$y)
#define ____flt_powI(__x, _$x, __y, _$y...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__y, i32) = _$y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_powif(as$(f32)(__x), __y))), \
        T_case$((f64)(__builtin_powi(as$(f64)(__x), __y))) \
    ))); \
})
#define __step__flt_powF(_$x, _$y...) ____flt_powF(pp_uniqTok(x), _$x, pp_uniqTok(y), _$y)
#define ____flt_powF(__x, _$x, __y, _$y...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__y, FltType) = _$y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_powf(as$(f32)(__x), as$(f32)(__y)))), \
        T_case$((f64)(__builtin_pow(as$(f64)(__x), as$(f64)(__y)))) \
    ))); \
})
#define __step__flt_ln(_$x) ____flt_ln(pp_uniqTok(x), _$x)
#define ____flt_ln(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    claim_assert(__x > 0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_logf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_log(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_log2(_$x) ____flt_log2(pp_uniqTok(x), _$x)
#define ____flt_log2(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    claim_assert(__x > 0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_log2f(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_log2(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_log10(_$x) ____flt_log10(pp_uniqTok(x), _$x)
#define ____flt_log10(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    claim_assert(__x > 0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_log10f(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_log10(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_log1p(_$x) ____flt_log1p(pp_uniqTok(x), _$x)
#define ____flt_log1p(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    claim_assert(__x > -1); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_log1pf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_log1p(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_expm1(_$x) ____flt_expm1(pp_uniqTok(x), _$x)
#define ____flt_expm1(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_expm1f(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_expm1(as$(f64)(__x)))) \
    ))); \
})

/*========== Floating-Point Trigonometric Implementation ====================*/

#define ____flt_sin(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_sinf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_sin(as$(f64)(_$x)))) \
))
#define ____flt_cos(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_cosf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_cos(as$(f64)(_$x)))) \
))
#define ____flt_tan(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_tanf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_tan(as$(f64)(_$x)))) \
))
#define __step__flt_asin(_$x...) ____flt_asin(pp_uniqTok(x), _$x)
#define ____flt_asin(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    /* Domain: [-1, 1] */ \
    claim_assert(flt_abs(__x) <= 1); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_asinf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_asin(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_acos(_$x...) ____flt_acos(pp_uniqTok(x), _$x)
#define ____flt_acos(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    /* Domain: [-1, 1] */ \
    claim_assert(flt_abs(__x) <= 1); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_acosf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_acos(as$(f64)(__x)))) \
    ))); \
})
#define ____flt_atan(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_atanf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_atan(as$(f64)(_$x)))) \
))
#define __step__flt_atan2(_$y, _$x...) ____flt_atan2(pp_uniqTok(y), _$y, pp_uniqTok(x), _$x)
#define ____flt_atan2(__y, _$y, __x, _$x...) local_({ \
    typedef TypeOfUnqual(_$y) FltType; \
    let_(__y, FltType) = _$y; \
    let_(__x, FltType) = _$x; \
    /* Domain: Non-Origin */ \
    claim_assert(isNonzero(__y) || isNonzero(__x)); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_atan2f(as$(f32)(__y), as$(f32)(__x)))), \
        T_case$((f64)(__builtin_atan2(as$(f64)(__y), as$(f64)(__x)))) \
    ))); \
})

/*========== Floating-Point Hyperbolic Implementation =======================*/

#define ____flt_sinh(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_sinhf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_sinh(as$(f64)(_$x)))) \
))
#define ____flt_cosh(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_coshf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_cosh(as$(f64)(_$x)))) \
))
#define ____flt_tanh(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_tanhf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_tanh(as$(f64)(_$x)))) \
))
#define ____flt_asinh(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_asinhf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_asinh(as$(f64)(_$x)))) \
))
#define __step__flt_acosh(_$x...) ____flt_acosh(pp_uniqTok(x), _$x)
#define ____flt_acosh(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    /* Domain: [1, Inf) */ \
    claim_assert(__x >= 1.0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_acoshf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_acosh(as$(f64)(__x)))) \
    ))); \
})
#define __step__flt_atanh(_$x...) ____flt_atanh(pp_uniqTok(x), _$x)
#define ____flt_atanh(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    /* Domain: (-1, 1) */ \
    claim_assert(flt_abs(__x) < 1.0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_atanhf(as$(f32)(__x)))), \
        T_case$((f64)(__builtin_atanh(as$(f64)(__x)))) \
    ))); \
})

/*========== Floating-Point IEEE 754 Implementation =========================*/

#define __op__flt_copySgn(__x, __y, _$x, _$y...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__y, FltType) = _$y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_copysignf(as$(f32)(__x), as$(f32)(__y)))), \
        T_case$((f64)(__builtin_copysign(as$(f64)(__x), as$(f64)(__y)))) \
    ))); \
})
#define __op__flt_nextAfter(__x, __y, _$x, _$y...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__y, FltType) = _$y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_nextafterf(as$(f32)(__x), as$(f32)(__y)))), \
        T_case$((f64)(__builtin_nextafter(as$(f64)(__x), as$(f64)(__y)))) \
    ))); \
})
#define __step__flt_nextUp(_$x...) ____flt_nextUp(pp_uniqTok(x), _$x)
#define ____flt_nextUp(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(flt_nextAfter(__x, flt_inf_pstv$(FltType))); \
})
#define __step__flt_nextDown(_$x...) ____flt_nextDown(pp_uniqTok(x), _$x)
#define ____flt_nextDown(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(flt_nextAfter(__x, flt_inf_ngtv$(FltType))); \
})
#define __op__flt_scalb(__x, __n, _$x, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__n, i32) = _$n; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_ldexpf(as$(f32)(__x), __n))), \
        T_case$((f64)(__builtin_ldexp(as$(f64)(__x), __n))) \
    ))); \
})
#define __op__flt_scalbn(__x, __n, _$x, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__n, i32) = _$n; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_scalbnf(as$(f32)(__x), __n))), \
        T_case$((f64)(__builtin_scalbn(as$(f64)(__x), __n))) \
    ))); \
})
#define __op__flt_scalbln(__x, __n, _$x, _$n...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__n, ilong) = _$n; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_scalblnf(as$(f32)(__x), as$(long)(__n)))), \
        T_case$((f64)(__builtin_scalbln(as$(f64)(__x), as$(long)(__n)))) \
    ))); \
})
#define ____flt_ilogb(_$x...) T_switch$((TypeOf(_$x))( \
    T_case$((f32)(__builtin_ilogbf(as$(f32)(_$x)))), \
    T_case$((f64)(__builtin_ilogb(as$(f64)(_$x)))) \
))
#define __op__flt_fdim(__x, __y, _$x, _$y...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__y, FltType) = _$y; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(__builtin_fdimf(as$(f32)(__x), as$(f32)(__y)))), \
        T_case$((f64)(__builtin_fdim(as$(f64)(__x), as$(f64)(__y)))) \
    ))); \
})
#define __op__flt_frexp(__x, _$x, _$p_exp...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(____flt_frexp32(__x, _$p_exp))), \
        T_case$((f64)(____flt_frexp64(__x, _$p_exp))) \
    ))); \
})
#define ____flt_frexp32(_$x, _$p_exp...) local_({ \
    int __exp = 0; \
    let_(__res, f32) = __builtin_frexpf(as$(f32)(_$x), &__exp); \
    *(_$p_exp) = as$(TypeOf(*(_$p_exp)))(__exp); \
    local_return_(__res); \
})
#define ____flt_frexp64(_$x, _$p_exp...) local_({ \
    int __exp = 0; \
    let_(__res, f64) = __builtin_frexp(as$(f64)(_$x), &__exp); \
    *(_$p_exp) = as$(TypeOf(*(_$p_exp)))(__exp); \
    local_return_(__res); \
})
#define __op__flt_modf(__x, _$x, _$p_int...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(____flt_modf32(__x, _$p_int))), \
        T_case$((f64)(____flt_modf64(__x, _$p_int))) \
    ))); \
})
#define ____flt_modf32(_$x, _$p_int...) local_({ \
    f32 __int = 0.0f; \
    let_(__res, f32) = __builtin_modff(as$(f32)(_$x), &__int); \
    *(_$p_int) = as$(TypeOf(*(_$p_int)))(__int); \
    local_return_(__res); \
})
#define ____flt_modf64(_$x, _$p_int...) local_({ \
    f64 __int = 0.0; \
    let_(__res, f64) = __builtin_modf(as$(f64)(_$x), &__int); \
    *(_$p_int) = as$(TypeOf(*(_$p_int)))(__int); \
    local_return_(__res); \
})
#define __op__flt_remquo(__x, __y, _$x, _$y, _$p_quo...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    let_(__y, FltType) = _$y; \
    claim_assert(__y != 0.0); \
    local_return_(T_switch$((TypeOf(FltType))( \
        T_case$((f32)(____flt_remquo32(__x, __y, _$p_quo))), \
        T_case$((f64)(____flt_remquo64(__x, __y, _$p_quo))) \
    ))); \
})
#define ____flt_remquo32(_$x, _$y, _$p_quo...) local_({ \
    int __quo = 0; \
    let_(__res, f32) = __builtin_remquof(as$(f32)(_$x), as$(f32)(_$y), &__quo); \
    *(_$p_quo) = as$(TypeOf(*(_$p_quo)))(__quo); \
    local_return_(__res); \
})
#define ____flt_remquo64(_$x, _$y, _$p_quo...) local_({ \
    int __quo = 0; \
    let_(__res, f64) = __builtin_remquo(as$(f64)(_$x), as$(f64)(_$y), &__quo); \
    *(_$p_quo) = as$(TypeOf(*(_$p_quo)))(__quo); \
    local_return_(__res); \
})
#define __step__flt_fract(_$x...) ____flt_fract(pp_uniqTok(x), _$x)
#define ____flt_fract(__x, _$x...) local_({ \
    typedef TypeOfUnqual(_$x) FltType; \
    let_(__x, FltType) = _$x; \
    local_return_(as$(FltType)(__x - flt_trunc(__x))); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_pri__included */

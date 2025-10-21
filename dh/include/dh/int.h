/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    int.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-02 (date of creation)
 * @updated 2025-03-02 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Integer type overflow checking utilities
 * @details Provides utilities for checking integer overflows.
 */

#ifndef int__included
#define int__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== includes =======================================================*/

#include "dh/prl.h"
#include "dh/debug/assert.h"

/*========== Macros and Definitions =========================================*/

/// Unsigned integer basic arithmetic with debug assertions

$inline_always u8 u8_add(u8 lhs, u8 rhs);
$inline_always u8 u8_sub(u8 lhs, u8 rhs);
$inline_always u8 u8_mul(u8 lhs, u8 rhs);
$inline_always u8 u8_div(u8 lhs, u8 rhs);
$inline_always u8 u8_mod(u8 lhs, u8 rhs);
$inline_always u8 u8_pow(u8 base, u32 exp);
$inline_always u8 u8_shl(u8 x, u32 shift);
$inline_always u8 u8_shr(u8 x, u32 shift);

$inline_always u16 u16_add(u16 lhs, u16 rhs);
$inline_always u16 u16_sub(u16 lhs, u16 rhs);
$inline_always u16 u16_mul(u16 lhs, u16 rhs);
$inline_always u16 u16_div(u16 lhs, u16 rhs);
$inline_always u16 u16_mod(u16 lhs, u16 rhs);
$inline_always u16 u16_pow(u16 base, u32 exp);
$inline_always u16 u16_shl(u16 x, u32 shift);
$inline_always u16 u16_shr(u16 x, u32 shift);

$inline_always u32 u32_add(u32 lhs, u32 rhs);
$inline_always u32 u32_sub(u32 lhs, u32 rhs);
$inline_always u32 u32_mul(u32 lhs, u32 rhs);
$inline_always u32 u32_div(u32 lhs, u32 rhs);
$inline_always u32 u32_mod(u32 lhs, u32 rhs);
$inline_always u32 u32_pow(u32 base, u32 exp);
$inline_always u32 u32_shl(u32 x, u32 shift);
$inline_always u32 u32_shr(u32 x, u32 shift);

$inline_always u64 u64_add(u64 lhs, u64 rhs);
$inline_always u64 u64_sub(u64 lhs, u64 rhs);
$inline_always u64 u64_mul(u64 lhs, u64 rhs);
$inline_always u64 u64_div(u64 lhs, u64 rhs);
$inline_always u64 u64_mod(u64 lhs, u64 rhs);
$inline_always u64 u64_pow(u64 base, u32 exp);
$inline_always u64 u64_shl(u64 x, u32 shift);
$inline_always u64 u64_shr(u64 x, u32 shift);

$inline_always usize usize_add(usize lhs, usize rhs);
$inline_always usize usize_sub(usize lhs, usize rhs);
$inline_always usize usize_mul(usize lhs, usize rhs);
$inline_always usize usize_div(usize lhs, usize rhs);
$inline_always usize usize_mod(usize lhs, usize rhs);
$inline_always usize usize_pow(usize base, u32 exp);
$inline_always usize usize_shl(usize x, u32 shift);
$inline_always usize usize_shr(usize x, u32 shift);

/// Signed integer basic arithmetic with debug assertions

$inline_always i8 i8_add(i8 lhs, i8 rhs);
$inline_always i8 i8_sub(i8 lhs, i8 rhs);
$inline_always i8 i8_mul(i8 lhs, i8 rhs);
$inline_always i8 i8_div(i8 lhs, i8 rhs);
$inline_always i8 i8_mod(i8 lhs, i8 rhs);
$inline_always i8 i8_neg(i8 x);
$inline_always i8 i8_abs(i8 x);
$inline_always i8 i8_pow(i8 base, u32 exp);
$inline_always i8 i8_shl(i8 x, u32 shift);
$inline_always i8 i8_shr(i8 x, u32 shift);

$inline_always i16 i16_add(i16 lhs, i16 rhs);
$inline_always i16 i16_sub(i16 lhs, i16 rhs);
$inline_always i16 i16_mul(i16 lhs, i16 rhs);
$inline_always i16 i16_div(i16 lhs, i16 rhs);
$inline_always i16 i16_mod(i16 lhs, i16 rhs);
$inline_always i16 i16_neg(i16 x);
$inline_always i16 i16_abs(i16 x);
$inline_always i16 i16_pow(i16 base, u32 exp);
$inline_always i16 i16_shl(i16 x, u32 shift);
$inline_always i16 i16_shr(i16 x, u32 shift);

$inline_always i32 i32_add(i32 lhs, i32 rhs);
$inline_always i32 i32_sub(i32 lhs, i32 rhs);
$inline_always i32 i32_mul(i32 lhs, i32 rhs);
$inline_always i32 i32_div(i32 lhs, i32 rhs);
$inline_always i32 i32_mod(i32 lhs, i32 rhs);
$inline_always i32 i32_neg(i32 x);
$inline_always i32 i32_abs(i32 x);
$inline_always i32 i32_pow(i32 base, u32 exp);
$inline_always i32 i32_shl(i32 x, u32 shift);
$inline_always i32 i32_shr(i32 x, u32 shift);

$inline_always i64 i64_add(i64 lhs, i64 rhs);
$inline_always i64 i64_sub(i64 lhs, i64 rhs);
$inline_always i64 i64_mul(i64 lhs, i64 rhs);
$inline_always i64 i64_div(i64 lhs, i64 rhs);
$inline_always i64 i64_mod(i64 lhs, i64 rhs);
$inline_always i64 i64_neg(i64 x);
$inline_always i64 i64_abs(i64 x);
$inline_always i64 i64_pow(i64 base, u32 exp);
$inline_always i64 i64_shl(i64 x, u32 shift);
$inline_always i64 i64_shr(i64 x, u32 shift);

$inline_always isize isize_add(isize lhs, isize rhs);
$inline_always isize isize_sub(isize lhs, isize rhs);
$inline_always isize isize_mul(isize lhs, isize rhs);
$inline_always isize isize_div(isize lhs, isize rhs);
$inline_always isize isize_mod(isize lhs, isize rhs);
$inline_always isize isize_neg(isize x);
$inline_always isize isize_abs(isize x);
$inline_always isize isize_pow(isize base, u32 exp);
$inline_always isize isize_shl(isize x, u32 shift);
$inline_always isize isize_shr(isize x, u32 shift);

/// Unsigned integer overflow checking functions

$inline_always Opt$u8 u8_addChkd(u8 lhs, u8 rhs);
$inline_always Opt$u8 u8_subChkd(u8 lhs, u8 rhs);
$inline_always Opt$u8 u8_mulChkd(u8 lhs, u8 rhs);
$inline_always Opt$u8 u8_divChkd(u8 lhs, u8 rhs);
$inline_always Opt$u8 u8_modChkd(u8 lhs, u8 rhs);
$inline_always Opt$u8 u8_powChkd(u8 base, u32 exp);
$inline_always Opt$u8 u8_shlChkd(u8 x, u32 shift);
$inline_always Opt$u8 u8_shrChkd(u8 x, u32 shift);

$inline_always Opt$u16 u16_addChkd(u16 lhs, u16 rhs);
$inline_always Opt$u16 u16_subChkd(u16 lhs, u16 rhs);
$inline_always Opt$u16 u16_mulChkd(u16 lhs, u16 rhs);
$inline_always Opt$u16 u16_divChkd(u16 lhs, u16 rhs);
$inline_always Opt$u16 u16_modChkd(u16 lhs, u16 rhs);
$inline_always Opt$u16 u16_powChkd(u16 base, u32 exp);
$inline_always Opt$u16 u16_shlChkd(u16 x, u32 shift);
$inline_always Opt$u16 u16_shrChkd(u16 x, u32 shift);

$inline_always Opt$u32 u32_addChkd(u32 lhs, u32 rhs);
$inline_always Opt$u32 u32_subChkd(u32 lhs, u32 rhs);
$inline_always Opt$u32 u32_mulChkd(u32 lhs, u32 rhs);
$inline_always Opt$u32 u32_divChkd(u32 lhs, u32 rhs);
$inline_always Opt$u32 u32_modChkd(u32 lhs, u32 rhs);
$inline_always Opt$u32 u32_powChkd(u32 base, u32 exp);
$inline_always Opt$u32 u32_shlChkd(u32 x, u32 shift);
$inline_always Opt$u32 u32_shrChkd(u32 x, u32 shift);

$inline_always Opt$u64 u64_addChkd(u64 lhs, u64 rhs);
$inline_always Opt$u64 u64_subChkd(u64 lhs, u64 rhs);
$inline_always Opt$u64 u64_mulChkd(u64 lhs, u64 rhs);
$inline_always Opt$u64 u64_divChkd(u64 lhs, u64 rhs);
$inline_always Opt$u64 u64_modChkd(u64 lhs, u64 rhs);
$inline_always Opt$u64 u64_powChkd(u64 base, u32 exp);
$inline_always Opt$u64 u64_shlChkd(u64 x, u32 shift);
$inline_always Opt$u64 u64_shrChkd(u64 x, u32 shift);

$inline_always Opt$usize usize_addChkd(usize lhs, usize rhs);
$inline_always Opt$usize usize_subChkd(usize lhs, usize rhs);
$inline_always Opt$usize usize_mulChkd(usize lhs, usize rhs);
$inline_always Opt$usize usize_divChkd(usize lhs, usize rhs);
$inline_always Opt$usize usize_modChkd(usize lhs, usize rhs);
$inline_always Opt$usize usize_powChkd(usize base, u32 exp);
$inline_always Opt$usize usize_shlChkd(usize x, u32 shift);
$inline_always Opt$usize usize_shrChkd(usize x, u32 shift);

/// Signed integer overflow checking functions

$inline_always Opt$i8 i8_addChkd(i8 lhs, i8 rhs);
$inline_always Opt$i8 i8_subChkd(i8 lhs, i8 rhs);
$inline_always Opt$i8 i8_mulChkd(i8 lhs, i8 rhs);
$inline_always Opt$i8 i8_divChkd(i8 lhs, i8 rhs);
$inline_always Opt$i8 i8_modChkd(i8 lhs, i8 rhs);
$inline_always Opt$i8 i8_negChkd(i8 x);
$inline_always Opt$i8 i8_absChkd(i8 x);
$inline_always Opt$i8 i8_powChkd(i8 base, u32 exp);
$inline_always Opt$i8 i8_shlChkd(i8 x, u32 shift);
$inline_always Opt$i8 i8_shrChkd(i8 x, u32 shift);

$inline_always Opt$i16 i16_addChkd(i16 lhs, i16 rhs);
$inline_always Opt$i16 i16_subChkd(i16 lhs, i16 rhs);
$inline_always Opt$i16 i16_mulChkd(i16 lhs, i16 rhs);
$inline_always Opt$i16 i16_divChkd(i16 lhs, i16 rhs);
$inline_always Opt$i16 i16_modChkd(i16 lhs, i16 rhs);
$inline_always Opt$i16 i16_negChkd(i16 x);
$inline_always Opt$i16 i16_absChkd(i16 x);
$inline_always Opt$i16 i16_powChkd(i16 base, u32 exp);
$inline_always Opt$i16 i16_shlChkd(i16 x, u32 shift);
$inline_always Opt$i16 i16_shrChkd(i16 x, u32 shift);

$inline_always Opt$i32 i32_addChkd(i32 lhs, i32 rhs);
$inline_always Opt$i32 i32_subChkd(i32 lhs, i32 rhs);
$inline_always Opt$i32 i32_mulChkd(i32 lhs, i32 rhs);
$inline_always Opt$i32 i32_divChkd(i32 lhs, i32 rhs);
$inline_always Opt$i32 i32_modChkd(i32 lhs, i32 rhs);
$inline_always Opt$i32 i32_negChkd(i32 x);
$inline_always Opt$i32 i32_absChkd(i32 x);
$inline_always Opt$i32 i32_powChkd(i32 base, u32 exp);
$inline_always Opt$i32 i32_shlChkd(i32 x, u32 shift);
$inline_always Opt$i32 i32_shrChkd(i32 x, u32 shift);

$inline_always Opt$i64 i64_addChkd(i64 lhs, i64 rhs);
$inline_always Opt$i64 i64_subChkd(i64 lhs, i64 rhs);
$inline_always Opt$i64 i64_mulChkd(i64 lhs, i64 rhs);
$inline_always Opt$i64 i64_divChkd(i64 lhs, i64 rhs);
$inline_always Opt$i64 i64_modChkd(i64 lhs, i64 rhs);
$inline_always Opt$i64 i64_negChkd(i64 x);
$inline_always Opt$i64 i64_absChkd(i64 x);
$inline_always Opt$i64 i64_powChkd(i64 base, u32 exp);
$inline_always Opt$i64 i64_shlChkd(i64 x, u32 shift);
$inline_always Opt$i64 i64_shrChkd(i64 x, u32 shift);

$inline_always Opt$isize isize_addChkd(isize lhs, isize rhs);
$inline_always Opt$isize isize_subChkd(isize lhs, isize rhs);
$inline_always Opt$isize isize_mulChkd(isize lhs, isize rhs);
$inline_always Opt$isize isize_divChkd(isize lhs, isize rhs);
$inline_always Opt$isize isize_modChkd(isize lhs, isize rhs);
$inline_always Opt$isize isize_negChkd(isize x);
$inline_always Opt$isize isize_absChkd(isize x);
$inline_always Opt$isize isize_powChkd(isize base, u32 exp);
$inline_always Opt$isize isize_shlChkd(isize x, u32 shift);
$inline_always Opt$isize isize_shrChkd(isize x, u32 shift);

/// Unsigned integer wrapping arithmetic functions

$inline_always u8 u8_addWrap(u8 lhs, u8 rhs);
$inline_always u8 u8_subWrap(u8 lhs, u8 rhs);
$inline_always u8 u8_mulWrap(u8 lhs, u8 rhs);
$inline_always u8 u8_shlWrap(u8 x, u32 shift);
$inline_always u8 u8_shrWrap(u8 x, u32 shift);

$inline_always u16 u16_addWrap(u16 lhs, u16 rhs);
$inline_always u16 u16_subWrap(u16 lhs, u16 rhs);
$inline_always u16 u16_mulWrap(u16 lhs, u16 rhs);
$inline_always u16 u16_shlWrap(u16 x, u32 shift);
$inline_always u16 u16_shrWrap(u16 x, u32 shift);

$inline_always u32 u32_addWrap(u32 lhs, u32 rhs);
$inline_always u32 u32_subWrap(u32 lhs, u32 rhs);
$inline_always u32 u32_mulWrap(u32 lhs, u32 rhs);
$inline_always u32 u32_shlWrap(u32 x, u32 shift);
$inline_always u32 u32_shrWrap(u32 x, u32 shift);

$inline_always u64 u64_addWrap(u64 lhs, u64 rhs);
$inline_always u64 u64_subWrap(u64 lhs, u64 rhs);
$inline_always u64 u64_mulWrap(u64 lhs, u64 rhs);
$inline_always u64 u64_shlWrap(u64 x, u32 shift);
$inline_always u64 u64_shrWrap(u64 x, u32 shift);

$inline_always usize usize_addWrap(usize lhs, usize rhs);
$inline_always usize usize_subWrap(usize lhs, usize rhs);
$inline_always usize usize_mulWrap(usize lhs, usize rhs);
$inline_always usize usize_shlWrap(usize x, u32 shift);
$inline_always usize usize_shrWrap(usize x, u32 shift);

/// Signed integer wrapping arithmetic functions

$inline_always i8 i8_addWrap(i8 lhs, i8 rhs);
$inline_always i8 i8_subWrap(i8 lhs, i8 rhs);
$inline_always i8 i8_mulWrap(i8 lhs, i8 rhs);
$inline_always i8 i8_negWrap(i8 x);
$inline_always i8 i8_shlWrap(i8 x, u32 shift);
$inline_always i8 i8_shrWrap(i8 x, u32 shift);

$inline_always i16 i16_addWrap(i16 lhs, i16 rhs);
$inline_always i16 i16_subWrap(i16 lhs, i16 rhs);
$inline_always i16 i16_mulWrap(i16 lhs, i16 rhs);
$inline_always i16 i16_negWrap(i16 x);
$inline_always i16 i16_shlWrap(i16 x, u32 shift);
$inline_always i16 i16_shrWrap(i16 x, u32 shift);

$inline_always i32 i32_addWrap(i32 lhs, i32 rhs);
$inline_always i32 i32_subWrap(i32 lhs, i32 rhs);
$inline_always i32 i32_mulWrap(i32 lhs, i32 rhs);
$inline_always i32 i32_negWrap(i32 x);
$inline_always i32 i32_shlWrap(i32 x, u32 shift);
$inline_always i32 i32_shrWrap(i32 x, u32 shift);

$inline_always i64 i64_addWrap(i64 lhs, i64 rhs);
$inline_always i64 i64_subWrap(i64 lhs, i64 rhs);
$inline_always i64 i64_mulWrap(i64 lhs, i64 rhs);
$inline_always i64 i64_negWrap(i64 x);
$inline_always i64 i64_shlWrap(i64 x, u32 shift);
$inline_always i64 i64_shrWrap(i64 x, u32 shift);

$inline_always isize isize_addWrap(isize lhs, isize rhs);
$inline_always isize isize_subWrap(isize lhs, isize rhs);
$inline_always isize isize_mulWrap(isize lhs, isize rhs);
$inline_always isize isize_negWrap(isize x);
$inline_always isize isize_shlWrap(isize x, u32 shift);
$inline_always isize isize_shrWrap(isize x, u32 shift);

/// Unsigned integer saturating arithmetic functions

$inline_always u8 u8_addSat(u8 lhs, u8 rhs);
$inline_always u8 u8_subSat(u8 lhs, u8 rhs);
$inline_always u8 u8_mulSat(u8 lhs, u8 rhs);
$inline_always u8 u8_powSat(u8 base, u32 exp);

$inline_always u16 u16_addSat(u16 lhs, u16 rhs);
$inline_always u16 u16_subSat(u16 lhs, u16 rhs);
$inline_always u16 u16_mulSat(u16 lhs, u16 rhs);
$inline_always u16 u16_powSat(u16 base, u32 exp);

$inline_always u32 u32_addSat(u32 lhs, u32 rhs);
$inline_always u32 u32_subSat(u32 lhs, u32 rhs);
$inline_always u32 u32_mulSat(u32 lhs, u32 rhs);
$inline_always u32 u32_powSat(u32 base, u32 exp);

$inline_always u64 u64_addSat(u64 lhs, u64 rhs);
$inline_always u64 u64_subSat(u64 lhs, u64 rhs);
$inline_always u64 u64_mulSat(u64 lhs, u64 rhs);
$inline_always u64 u64_powSat(u64 base, u32 exp);

$inline_always usize usize_addSat(usize lhs, usize rhs);
$inline_always usize usize_subSat(usize lhs, usize rhs);
$inline_always usize usize_mulSat(usize lhs, usize rhs);
$inline_always usize usize_powSat(usize base, u32 exp);

/// Signed integer saturating arithmetic functions

$inline_always i8 i8_addSat(i8 lhs, i8 rhs);
$inline_always i8 i8_subSat(i8 lhs, i8 rhs);
$inline_always i8 i8_mulSat(i8 lhs, i8 rhs);
$inline_always i8 i8_divSat(i8 lhs, i8 rhs);
$inline_always i8 i8_negSat(i8 x);
$inline_always i8 i8_absSat(i8 x);
$inline_always i8 i8_powSat(i8 base, u32 exp);

$inline_always i16 i16_addSat(i16 lhs, i16 rhs);
$inline_always i16 i16_subSat(i16 lhs, i16 rhs);
$inline_always i16 i16_mulSat(i16 lhs, i16 rhs);
$inline_always i16 i16_divSat(i16 lhs, i16 rhs);
$inline_always i16 i16_negSat(i16 x);
$inline_always i16 i16_absSat(i16 x);
$inline_always i16 i16_powSat(i16 base, u32 exp);

$inline_always i32 i32_addSat(i32 lhs, i32 rhs);
$inline_always i32 i32_subSat(i32 lhs, i32 rhs);
$inline_always i32 i32_mulSat(i32 lhs, i32 rhs);
$inline_always i32 i32_divSat(i32 lhs, i32 rhs);
$inline_always i32 i32_negSat(i32 x);
$inline_always i32 i32_absSat(i32 x);
$inline_always i32 i32_powSat(i32 base, u32 exp);

$inline_always i64 i64_addSat(i64 lhs, i64 rhs);
$inline_always i64 i64_subSat(i64 lhs, i64 rhs);
$inline_always i64 i64_mulSat(i64 lhs, i64 rhs);
$inline_always i64 i64_divSat(i64 lhs, i64 rhs);
$inline_always i64 i64_negSat(i64 x);
$inline_always i64 i64_absSat(i64 x);
$inline_always i64 i64_powSat(i64 base, u32 exp);

$inline_always isize isize_addSat(isize lhs, isize rhs);
$inline_always isize isize_subSat(isize lhs, isize rhs);
$inline_always isize isize_mulSat(isize lhs, isize rhs);
$inline_always isize isize_divSat(isize lhs, isize rhs);
$inline_always isize isize_negSat(isize x);
$inline_always isize isize_absSat(isize x);
$inline_always isize isize_powSat(isize base, u32 exp);

/*========== Macros and Implementations =====================================*/

/// Unsigned integer basic arithmetic implementations

$inline_always u8 u8_add(u8 lhs, u8 rhs) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &(u8){})), as$(u8, lhs + rhs);
}
$inline_always u8 u8_sub(u8 lhs, u8 rhs) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &(u8){})), as$(u8, lhs - rhs);
}
$inline_always u8 u8_mul(u8 lhs, u8 rhs) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &(u8){})), as$(u8, lhs* rhs);
}
$inline_always u8 u8_div(u8 lhs, u8 rhs) {
    return debug_assert(rhs != 0), lhs / rhs;
}
$inline_always u8 u8_mod(u8 lhs, u8 rhs) {
    return debug_assert(rhs != 0), lhs % rhs;
}
$inline_always u8 u8_pow(u8 base, u32 exp) {
    if (exp == 0) { return 1; }
    u8 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &(u8){}));
        result = as$(u8, result* base);
    }
    return result;
}
$inline_always u8 u8_shl(u8 x, u32 shift) {
    return debug_assert(shift < 8), as$(u8, x << shift);
}
$inline_always u8 u8_shr(u8 x, u32 shift) {
    return debug_assert(shift < 8), as$(u8, x >> shift);
}

$inline_always u16 u16_add(u16 lhs, u16 rhs) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &(u16){})), as$(u16, lhs + rhs);
}
$inline_always u16 u16_sub(u16 lhs, u16 rhs) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &(u16){})), as$(u16, lhs - rhs);
}
$inline_always u16 u16_mul(u16 lhs, u16 rhs) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &(u16){})), as$(u16, lhs* rhs);
}
$inline_always u16 u16_div(u16 lhs, u16 rhs) {
    return debug_assert(rhs != 0), lhs / rhs;
}
$inline_always u16 u16_mod(u16 lhs, u16 rhs) {
    return debug_assert(rhs != 0), lhs % rhs;
}
$inline_always u16 u16_pow(u16 base, u32 exp) {
    if (exp == 0) { return 1; }
    u16 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &(u16){}));
        result = as$(u16, result* base);
    }
    return result;
}
$inline_always u16 u16_shl(u16 x, u32 shift) {
    return debug_assert(shift < 16), as$(u16, x << shift);
}
$inline_always u16 u16_shr(u16 x, u32 shift) {
    return debug_assert(shift < 16), as$(u16, x >> shift);
}

$inline_always u32 u32_add(u32 lhs, u32 rhs) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &(u32){})), lhs + rhs;
}
$inline_always u32 u32_sub(u32 lhs, u32 rhs) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &(u32){})), lhs - rhs;
}
$inline_always u32 u32_mul(u32 lhs, u32 rhs) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &(u32){})), lhs * rhs;
}
$inline_always u32 u32_div(u32 lhs, u32 rhs) {
    return debug_assert(rhs != 0), lhs / rhs;
}
$inline_always u32 u32_mod(u32 lhs, u32 rhs) {
    return debug_assert(rhs != 0), lhs % rhs;
}
$inline_always u32 u32_pow(u32 base, u32 exp) {
    if (exp == 0) { return 1; }
    u32 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &(u32){}));
        result *= base;
    }
    return result;
}
$inline_always u32 u32_shl(u32 x, u32 shift) {
    return debug_assert(shift < 32), x << shift;
}
$inline_always u32 u32_shr(u32 x, u32 shift) {
    return debug_assert(shift < 32), x >> shift;
}

$inline_always u64 u64_add(u64 lhs, u64 rhs) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &(u64){})), lhs + rhs;
}
$inline_always u64 u64_sub(u64 lhs, u64 rhs) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &(u64){})), lhs - rhs;
}
$inline_always u64 u64_mul(u64 lhs, u64 rhs) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &(u64){})), lhs * rhs;
}
$inline_always u64 u64_div(u64 lhs, u64 rhs) {
    return debug_assert(rhs != 0), lhs / rhs;
}
$inline_always u64 u64_mod(u64 lhs, u64 rhs) {
    return debug_assert(rhs != 0), lhs % rhs;
}
$inline_always u64 u64_pow(u64 base, u32 exp) {
    if (exp == 0) { return 1; }
    u64 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &(u64){}));
        result *= base;
    }
    return result;
}
$inline_always u64 u64_shl(u64 x, u32 shift) {
    return debug_assert(shift < 64), x << shift;
}
$inline_always u64 u64_shr(u64 x, u32 shift) {
    return debug_assert(shift < 64), x >> shift;
}

$inline_always usize usize_add(usize lhs, usize rhs) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &(usize){})), lhs + rhs;
}
$inline_always usize usize_sub(usize lhs, usize rhs) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &(usize){})), lhs - rhs;
}
$inline_always usize usize_mul(usize lhs, usize rhs) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &(usize){})), lhs * rhs;
}
$inline_always usize usize_div(usize lhs, usize rhs) {
    return debug_assert(rhs != 0), lhs / rhs;
}
$inline_always usize usize_mod(usize lhs, usize rhs) {
    return debug_assert(rhs != 0), lhs % rhs;
}
$inline_always usize usize_pow(usize base, u32 exp) {
    if (exp == 0) { return 1; }
    usize result = 1;
    for (u32 i = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &(usize){}));
        result *= base;
    }
    return result;
}
$inline_always usize usize_shl(usize x, u32 shift) {
    return debug_assert(shift < (sizeof(usize) * 8)), x << shift;
}
$inline_always usize usize_shr(usize x, u32 shift) {
    return debug_assert(shift < (sizeof(usize) * 8)), x >> shift;
}

/// Signed integer basic arithmetic implementations

$inline_always i8 i8_add(i8 lhs, i8 rhs) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &(i8){})), as$(i8, lhs + rhs);
}
$inline_always i8 i8_sub(i8 lhs, i8 rhs) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &(i8){})), as$(i8, lhs - rhs);
}
$inline_always i8 i8_mul(i8 lhs, i8 rhs) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &(i8){})), as$(i8, lhs* rhs);
}
$inline_always i8 i8_div(i8 lhs, i8 rhs) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i8_limit_min && rhs == -1)), as$(i8, lhs / rhs);
}
$inline_always i8 i8_mod(i8 lhs, i8 rhs) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i8_limit_min && rhs == -1)), as$(i8, lhs % rhs);
}
$inline_always i8 i8_neg(i8 x) {
    return debug_assert(x != i8_limit_min), as$(i8, -x);
}
$inline_always i8 i8_abs(i8 x) {
    return debug_assert(x != i8_limit_min), as$(i8, (x < 0) ? -x : x);
}
$inline_always i8 i8_pow(i8 base, u32 exp) {
    if (exp == 0) { return 1; }
    i8 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &(i8){}));
        result = as$(i8, result* base);
    }
    return result;
}
$inline_always i8 i8_shl(i8 x, u32 shift) {
    return debug_assert(shift < 8), debug_assert(x >= 0), as$(i8, x << shift);
}
$inline_always i8 i8_shr(i8 x, u32 shift) {
    return debug_assert(shift < 8), as$(i8, x >> shift);
}

$inline_always i16 i16_add(i16 lhs, i16 rhs) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &(i16){})), as$(i16, lhs + rhs);
}
$inline_always i16 i16_sub(i16 lhs, i16 rhs) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &(i16){})), as$(i16, lhs - rhs);
}
$inline_always i16 i16_mul(i16 lhs, i16 rhs) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &(i16){})), as$(i16, lhs* rhs);
}
$inline_always i16 i16_div(i16 lhs, i16 rhs) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i16_limit_min && rhs == -1)), as$(i16, lhs / rhs);
}
$inline_always i16 i16_mod(i16 lhs, i16 rhs) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i16_limit_min && rhs == -1)), as$(i16, lhs % rhs);
}
$inline_always i16 i16_neg(i16 x) {
    return debug_assert(x != i16_limit_min), as$(i16, -x);
}
$inline_always i16 i16_abs(i16 x) {
    return debug_assert(x != i16_limit_min), as$(i16, (x < 0) ? -x : x);
}
$inline_always i16 i16_pow(i16 base, u32 exp) {
    if (exp == 0) { return 1; }
    i16 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &(i16){}));
        result = as$(i16, result* base);
    }
    return result;
}
$inline_always i16 i16_shl(i16 x, u32 shift) {
    return debug_assert(shift < 16), debug_assert(x >= 0), as$(i16, x << shift);
}
$inline_always i16 i16_shr(i16 x, u32 shift) {
    return debug_assert(shift < 16), as$(i16, x >> shift);
}

$inline_always i32 i32_add(i32 lhs, i32 rhs) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &(i32){})), lhs + rhs;
}
$inline_always i32 i32_sub(i32 lhs, i32 rhs) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &(i32){})), lhs - rhs;
}
$inline_always i32 i32_mul(i32 lhs, i32 rhs) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &(i32){})), lhs * rhs;
}
$inline_always i32 i32_div(i32 lhs, i32 rhs) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i32_limit_min && rhs == -1)), lhs / rhs;
}
$inline_always i32 i32_mod(i32 lhs, i32 rhs) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i32_limit_min && rhs == -1)), lhs % rhs;
}
$inline_always i32 i32_neg(i32 x) {
    return debug_assert(x != i32_limit_min), -x;
}
$inline_always i32 i32_abs(i32 x) {
    return debug_assert(x != i32_limit_min), (x < 0) ? -x : x;
}
$inline_always i32 i32_pow(i32 base, u32 exp) {
    if (exp == 0) { return 1; }
    i32 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &(i32){}));
        result *= base;
    }
    return result;
}
$inline_always i32 i32_shl(i32 x, u32 shift) {
    return debug_assert(shift < 32), debug_assert(x >= 0), x << shift;
}
$inline_always i32 i32_shr(i32 x, u32 shift) {
    return debug_assert(shift < 32), x >> shift;
}

$inline_always i64 i64_add(i64 lhs, i64 rhs) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &(i64){})), lhs + rhs;
}
$inline_always i64 i64_sub(i64 lhs, i64 rhs) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &(i64){})), lhs - rhs;
}
$inline_always i64 i64_mul(i64 lhs, i64 rhs) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &(i64){})), lhs * rhs;
}
$inline_always i64 i64_div(i64 lhs, i64 rhs) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i64_limit_min && rhs == -1)), lhs / rhs;
}
$inline_always i64 i64_mod(i64 lhs, i64 rhs) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i64_limit_min && rhs == -1)), lhs % rhs;
}
$inline_always i64 i64_neg(i64 x) {
    return debug_assert(x != i64_limit_min), -x;
}
$inline_always i64 i64_abs(i64 x) {
    return debug_assert(x != i64_limit_min), (x < 0) ? -x : x;
}
$inline_always i64 i64_pow(i64 base, u32 exp) {
    if (exp == 0) { return 1; }
    i64 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &(i64){}));
        result *= base;
    }
    return result;
}
$inline_always i64 i64_shl(i64 x, u32 shift) {
    return debug_assert(shift < 64), debug_assert(x >= 0), x << shift;
}
$inline_always i64 i64_shr(i64 x, u32 shift) {
    return debug_assert(shift < 64), x >> shift;
}

$inline_always isize isize_add(isize lhs, isize rhs) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &(isize){})), lhs + rhs;
}
$inline_always isize isize_sub(isize lhs, isize rhs) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &(isize){})), lhs - rhs;
}
$inline_always isize isize_mul(isize lhs, isize rhs) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &(isize){})), lhs * rhs;
}
$inline_always isize isize_div(isize lhs, isize rhs) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == isize_limit_min && rhs == -1)), lhs / rhs;
}
$inline_always isize isize_mod(isize lhs, isize rhs) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == isize_limit_min && rhs == -1)), lhs % rhs;
}
$inline_always isize isize_neg(isize x) {
    return debug_assert(x != isize_limit_min), -x;
}
$inline_always isize isize_abs(isize x) {
    return debug_assert(x != isize_limit_min), (x < 0) ? -x : x;
}
$inline_always isize isize_pow(isize base, u32 exp) {
    if (exp == 0) { return 1; }
    isize result = 1;
    for (u32 i = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &(isize){}));
        result *= base;
    }
    return result;
}
$inline_always isize isize_shl(isize x, u32 shift) {
    return debug_assert(shift < (sizeof(isize) * 8)), debug_assert(x >= 0), x << shift;
}
$inline_always isize isize_shr(isize x, u32 shift) {
    return debug_assert(shift < (sizeof(isize) * 8)), x >> shift;
}

/*========== Checked Arithmetic Implementations =============================*/

/// Unsigned integer overflow checking implementations

$inline_always Opt$u8 u8_addChkd(u8 lhs, u8 rhs) {
    u8 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$u8); }
    return some$(Opt$u8, result);
}
$inline_always Opt$u8 u8_subChkd(u8 lhs, u8 rhs) {
    u8 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$u8); }
    return some$(Opt$u8, result);
}
$inline_always Opt$u8 u8_mulChkd(u8 lhs, u8 rhs) {
    u8 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$u8); }
    return some$(Opt$u8, result);
}
$inline_always Opt$u8 u8_divChkd(u8 lhs, u8 rhs) {
    if (rhs == 0) { return none$(Opt$u8); }
    return some$(Opt$u8, lhs / rhs);
}
$inline_always Opt$u8 u8_modChkd(u8 lhs, u8 rhs) {
    if (rhs == 0) { return none$(Opt$u8); }
    return some$(Opt$u8, lhs % rhs);
}
$inline_always Opt$u8 u8_powChkd(u8 base, u32 exp) {
    if (exp == 0) { return some$(Opt$u8, 1); }
    u8 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return none$(Opt$u8); }
    }
    return some$(Opt$u8, result);
}
$inline_always Opt$u8 u8_shlChkd(u8 x, u32 shift) {
    if (shift >= 8) { return none$(Opt$u8); }
    if (shift == 0) { return some$(Opt$u8, x); }
    if (x > (u8_limit_max >> shift)) { return none$(Opt$u8); }
    return some$(Opt$u8, as$(u8, x << shift));
}
$inline_always Opt$u8 u8_shrChkd(u8 x, u32 shift) {
    if (shift >= 8) { return none$(Opt$u8); }
    return some$(Opt$u8, as$(u8, x >> shift));
}

$inline_always Opt$u16 u16_addChkd(u16 lhs, u16 rhs) {
    u16 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$u16); }
    return some$(Opt$u16, result);
}
$inline_always Opt$u16 u16_subChkd(u16 lhs, u16 rhs) {
    u16 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$u16); }
    return some$(Opt$u16, result);
}
$inline_always Opt$u16 u16_mulChkd(u16 lhs, u16 rhs) {
    u16 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$u16); }
    return some$(Opt$u16, result);
}
$inline_always Opt$u16 u16_divChkd(u16 lhs, u16 rhs) {
    if (rhs == 0) { return none$(Opt$u16); }
    return some$(Opt$u16, lhs / rhs);
}
$inline_always Opt$u16 u16_modChkd(u16 lhs, u16 rhs) {
    if (rhs == 0) { return none$(Opt$u16); }
    return some$(Opt$u16, lhs % rhs);
}
$inline_always Opt$u16 u16_powChkd(u16 base, u32 exp) {
    if (exp == 0) { return some$(Opt$u16, 1); }
    u16 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return none$(Opt$u16); }
    }
    return some$(Opt$u16, result);
}
$inline_always Opt$u16 u16_shlChkd(u16 x, u32 shift) {
    if (shift >= 16) { return none$(Opt$u16); }
    if (shift == 0) { return some$(Opt$u16, x); }
    if (x > (u16_limit_max >> shift)) { return none$(Opt$u16); }
    return some$(Opt$u16, as$(u16, x << shift));
}
$inline_always Opt$u16 u16_shrChkd(u16 x, u32 shift) {
    if (shift >= 16) { return none$(Opt$u16); }
    return some$(Opt$u16, as$(u16, x >> shift));
}

$inline_always Opt$u32 u32_addChkd(u32 lhs, u32 rhs) {
    u32 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$u32); }
    return some$(Opt$u32, result);
}
$inline_always Opt$u32 u32_subChkd(u32 lhs, u32 rhs) {
    u32 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$u32); }
    return some$(Opt$u32, result);
}
$inline_always Opt$u32 u32_mulChkd(u32 lhs, u32 rhs) {
    u32 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$u32); }
    return some$(Opt$u32, result);
}
$inline_always Opt$u32 u32_divChkd(u32 lhs, u32 rhs) {
    if (rhs == 0) { return none$(Opt$u32); }
    return some$(Opt$u32, lhs / rhs);
}
$inline_always Opt$u32 u32_modChkd(u32 lhs, u32 rhs) {
    if (rhs == 0) { return none$(Opt$u32); }
    return some$(Opt$u32, lhs % rhs);
}
$inline_always Opt$u32 u32_powChkd(u32 base, u32 exp) {
    if (exp == 0) { return some$(Opt$u32, 1); }
    u32 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return none$(Opt$u32); }
    }
    return some$(Opt$u32, result);
}
$inline_always Opt$u32 u32_shlChkd(u32 x, u32 shift) {
    if (shift >= 32) { return none$(Opt$u32); }
    if (shift == 0) { return some$(Opt$u32, x); }
    if (x > (u32_limit_max >> shift)) { return none$(Opt$u32); }
    return some$(Opt$u32, x << shift);
}
$inline_always Opt$u32 u32_shrChkd(u32 x, u32 shift) {
    if (shift >= 32) { return none$(Opt$u32); }
    return some$(Opt$u32, x >> shift);
}

$inline_always Opt$u64 u64_addChkd(u64 lhs, u64 rhs) {
    u64 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$u64); }
    return some$(Opt$u64, result);
}
$inline_always Opt$u64 u64_subChkd(u64 lhs, u64 rhs) {
    u64 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$u64); }
    return some$(Opt$u64, result);
}
$inline_always Opt$u64 u64_mulChkd(u64 lhs, u64 rhs) {
    u64 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$u64); }
    return some$(Opt$u64, result);
}
$inline_always Opt$u64 u64_divChkd(u64 lhs, u64 rhs) {
    if (rhs == 0) { return none$(Opt$u64); }
    return some$(Opt$u64, lhs / rhs);
}
$inline_always Opt$u64 u64_modChkd(u64 lhs, u64 rhs) {
    if (rhs == 0) { return none$(Opt$u64); }
    return some$(Opt$u64, lhs % rhs);
}
$inline_always Opt$u64 u64_powChkd(u64 base, u32 exp) {
    if (exp == 0) { return some$(Opt$u64, 1); }
    u64 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return none$(Opt$u64); }
    }
    return some$(Opt$u64, result);
}
$inline_always Opt$u64 u64_shlChkd(u64 x, u32 shift) {
    if (shift >= 64) { return none$(Opt$u64); }
    if (shift == 0) { return some$(Opt$u64, x); }
    if (x > (u64_limit_max >> shift)) { return none$(Opt$u64); }
    return some$(Opt$u64, x << shift);
}
$inline_always Opt$u64 u64_shrChkd(u64 x, u32 shift) {
    if (shift >= 64) { return none$(Opt$u64); }
    return some$(Opt$u64, x >> shift);
}

$inline_always Opt$usize usize_addChkd(usize lhs, usize rhs) {
    usize result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$usize); }
    return some$(Opt$usize, result);
}
$inline_always Opt$usize usize_subChkd(usize lhs, usize rhs) {
    usize result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$usize); }
    return some$(Opt$usize, result);
}
$inline_always Opt$usize usize_mulChkd(usize lhs, usize rhs) {
    usize result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$usize); }
    return some$(Opt$usize, result);
}
$inline_always Opt$usize usize_divChkd(usize lhs, usize rhs) {
    if (rhs == 0) { return none$(Opt$usize); }
    return some$(Opt$usize, lhs / rhs);
}
$inline_always Opt$usize usize_modChkd(usize lhs, usize rhs) {
    if (rhs == 0) { return none$(Opt$usize); }
    return some$(Opt$usize, lhs % rhs);
}
$inline_always Opt$usize usize_powChkd(usize base, u32 exp) {
    if (exp == 0) { return some$(Opt$usize, 1); }
    usize result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return none$(Opt$usize); }
    }
    return some$(Opt$usize, result);
}
$inline_always Opt$usize usize_shlChkd(usize x, u32 shift) {
    if (shift >= (sizeof(usize) * 8)) { return none$(Opt$usize); }
    if (shift == 0) { return some$(Opt$usize, x); }
    if (x > (usize_limit_max >> shift)) { return none$(Opt$usize); }
    return some$(Opt$usize, x << shift);
}
$inline_always Opt$usize usize_shrChkd(usize x, u32 shift) {
    if (shift >= (sizeof(usize) * 8)) { return none$(Opt$usize); }
    return some$(Opt$usize, x >> shift);
}

/// Signed integer overflow checking implementations

$inline_always Opt$i8 i8_addChkd(i8 lhs, i8 rhs) {
    i8 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$i8); }
    return some$(Opt$i8, result);
}
$inline_always Opt$i8 i8_subChkd(i8 lhs, i8 rhs) {
    i8 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$i8); }
    return some$(Opt$i8, result);
}
$inline_always Opt$i8 i8_mulChkd(i8 lhs, i8 rhs) {
    i8 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$i8); }
    return some$(Opt$i8, result);
}
$inline_always Opt$i8 i8_divChkd(i8 lhs, i8 rhs) {
    if (rhs == 0 || (lhs == i8_limit_min && rhs == -1)) { return none$(Opt$i8); }
    return some$(Opt$i8, as$(i8, lhs / rhs));
}
$inline_always Opt$i8 i8_modChkd(i8 lhs, i8 rhs) {
    if (rhs == 0 || (lhs == i8_limit_min && rhs == -1)) { return none$(Opt$i8); }
    return some$(Opt$i8, as$(i8, lhs % rhs));
}
$inline_always Opt$i8 i8_negChkd(i8 x) {
    if (x == i8_limit_min) { return none$(Opt$i8); }
    return some$(Opt$i8, as$(i8, -x));
}
$inline_always Opt$i8 i8_absChkd(i8 x) {
    if (x == i8_limit_min) { return none$(Opt$i8); }
    return some$(Opt$i8, as$(i8, (x < 0) ? -x : x));
}
$inline_always Opt$i8 i8_powChkd(i8 base, u32 exp) {
    if (exp == 0) { return some$(Opt$i8, 1); }
    i8 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return none$(Opt$i8); }
    }
    return some$(Opt$i8, result);
}
$inline_always Opt$i8 i8_shlChkd(i8 x, u32 shift) {
    if (shift >= 8) { return none$(Opt$i8); }
    if (shift == 0) { return some$(Opt$i8, x); }
    if (x < 0) { return none$(Opt$i8); }
    if (x > (i8_limit_max >> shift)) { return none$(Opt$i8); }
    return some$(Opt$i8, as$(i8, x << shift));
}
$inline_always Opt$i8 i8_shrChkd(i8 x, u32 shift) {
    if (shift >= 8) { return none$(Opt$i8); }
    return some$(Opt$i8, as$(i8, x >> shift));
}

$inline_always Opt$i16 i16_addChkd(i16 lhs, i16 rhs) {
    i16 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$i16); }
    return some$(Opt$i16, result);
}
$inline_always Opt$i16 i16_subChkd(i16 lhs, i16 rhs) {
    i16 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$i16); }
    return some$(Opt$i16, result);
}
$inline_always Opt$i16 i16_mulChkd(i16 lhs, i16 rhs) {
    i16 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$i16); }
    return some$(Opt$i16, result);
}
$inline_always Opt$i16 i16_divChkd(i16 lhs, i16 rhs) {
    if (rhs == 0 || (lhs == i16_limit_min && rhs == -1)) { return none$(Opt$i16); }
    return some$(Opt$i16, as$(i16, lhs / rhs));
}
$inline_always Opt$i16 i16_modChkd(i16 lhs, i16 rhs) {
    if (rhs == 0 || (lhs == i16_limit_min && rhs == -1)) { return none$(Opt$i16); }
    return some$(Opt$i16, as$(i16, lhs % rhs));
}
$inline_always Opt$i16 i16_negChkd(i16 x) {
    if (x == i16_limit_min) { return none$(Opt$i16); }
    return some$(Opt$i16, as$(i16, -x));
}
$inline_always Opt$i16 i16_absChkd(i16 x) {
    if (x == i16_limit_min) { return none$(Opt$i16); }
    return some$(Opt$i16, as$(i16, (x < 0) ? -x : x));
}
$inline_always Opt$i16 i16_powChkd(i16 base, u32 exp) {
    if (exp == 0) { return some$(Opt$i16, 1); }
    i16 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return none$(Opt$i16); }
    }
    return some$(Opt$i16, result);
}
$inline_always Opt$i16 i16_shlChkd(i16 x, u32 shift) {
    if (shift >= 16) { return none$(Opt$i16); }
    if (shift == 0) { return some$(Opt$i16, x); }
    if (x < 0) { return none$(Opt$i16); }
    if (x > (i16_limit_max >> shift)) { return none$(Opt$i16); }
    return some$(Opt$i16, as$(i16, x << shift));
}
$inline_always Opt$i16 i16_shrChkd(i16 x, u32 shift) {
    if (shift >= 16) { return none$(Opt$i16); }
    return some$(Opt$i16, as$(i16, x >> shift));
}

$inline_always Opt$i32 i32_addChkd(i32 lhs, i32 rhs) {
    i32 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$i32); }
    return some$(Opt$i32, result);
}
$inline_always Opt$i32 i32_subChkd(i32 lhs, i32 rhs) {
    i32 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$i32); }
    return some$(Opt$i32, result);
}
$inline_always Opt$i32 i32_mulChkd(i32 lhs, i32 rhs) {
    i32 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$i32); }
    return some$(Opt$i32, result);
}
$inline_always Opt$i32 i32_divChkd(i32 lhs, i32 rhs) {
    if (rhs == 0 || (lhs == i32_limit_min && rhs == -1)) { return none$(Opt$i32); }
    return some$(Opt$i32, lhs / rhs);
}
$inline_always Opt$i32 i32_modChkd(i32 lhs, i32 rhs) {
    if (rhs == 0 || (lhs == i32_limit_min && rhs == -1)) { return none$(Opt$i32); }
    return some$(Opt$i32, lhs % rhs);
}
$inline_always Opt$i32 i32_negChkd(i32 x) {
    if (x == i32_limit_min) { return none$(Opt$i32); }
    return some$(Opt$i32, -x);
}
$inline_always Opt$i32 i32_absChkd(i32 x) {
    if (x == i32_limit_min) { return none$(Opt$i32); }
    return some$(Opt$i32, (x < 0) ? -x : x);
}
$inline_always Opt$i32 i32_powChkd(i32 base, u32 exp) {
    if (exp == 0) { return some$(Opt$i32, 1); }
    i32 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return none$(Opt$i32); }
    }
    return some$(Opt$i32, result);
}
$inline_always Opt$i32 i32_shlChkd(i32 x, u32 shift) {
    if (shift >= 32) { return none$(Opt$i32); }
    if (shift == 0) { return some$(Opt$i32, x); }
    if (x < 0) { return none$(Opt$i32); }
    if (x > (i32_limit_max >> shift)) { return none$(Opt$i32); }
    return some$(Opt$i32, x << shift);
}
$inline_always Opt$i32 i32_shrChkd(i32 x, u32 shift) {
    if (shift >= 32) { return none$(Opt$i32); }
    return some$(Opt$i32, x >> shift);
}

$inline_always Opt$i64 i64_addChkd(i64 lhs, i64 rhs) {
    i64 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$i64); }
    return some$(Opt$i64, result);
}
$inline_always Opt$i64 i64_subChkd(i64 lhs, i64 rhs) {
    i64 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$i64); }
    return some$(Opt$i64, result);
}
$inline_always Opt$i64 i64_mulChkd(i64 lhs, i64 rhs) {
    i64 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$i64); }
    return some$(Opt$i64, result);
}
$inline_always Opt$i64 i64_divChkd(i64 lhs, i64 rhs) {
    if (rhs == 0 || (lhs == i64_limit_min && rhs == -1)) { return none$(Opt$i64); }
    return some$(Opt$i64, lhs / rhs);
}
$inline_always Opt$i64 i64_modChkd(i64 lhs, i64 rhs) {
    if (rhs == 0 || (lhs == i64_limit_min && rhs == -1)) { return none$(Opt$i64); }
    return some$(Opt$i64, lhs % rhs);
}
$inline_always Opt$i64 i64_negChkd(i64 x) {
    if (x == i64_limit_min) { return none$(Opt$i64); }
    return some$(Opt$i64, -x);
}
$inline_always Opt$i64 i64_absChkd(i64 x) {
    if (x == i64_limit_min) { return none$(Opt$i64); }
    return some$(Opt$i64, (x < 0) ? -x : x);
}
$inline_always Opt$i64 i64_powChkd(i64 base, u32 exp) {
    if (exp == 0) { return some$(Opt$i64, 1); }
    i64 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return none$(Opt$i64); }
    }
    return some$(Opt$i64, result);
}
$inline_always Opt$i64 i64_shlChkd(i64 x, u32 shift) {
    if (shift >= 64) { return none$(Opt$i64); }
    if (shift == 0) { return some$(Opt$i64, x); }
    if (x < 0) { return none$(Opt$i64); }
    if (x > (i64_limit_max >> shift)) { return none$(Opt$i64); }
    return some$(Opt$i64, x << shift);
}
$inline_always Opt$i64 i64_shrChkd(i64 x, u32 shift) {
    if (shift >= 64) { return none$(Opt$i64); }
    return some$(Opt$i64, x >> shift);
}

$inline_always Opt$isize isize_addChkd(isize lhs, isize rhs) {
    isize result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$isize); }
    return some$(Opt$isize, result);
}
$inline_always Opt$isize isize_subChkd(isize lhs, isize rhs) {
    isize result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$isize); }
    return some$(Opt$isize, result);
}
$inline_always Opt$isize isize_mulChkd(isize lhs, isize rhs) {
    isize result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$isize); }
    return some$(Opt$isize, result);
}
$inline_always Opt$isize isize_divChkd(isize lhs, isize rhs) {
    if (rhs == 0 || (lhs == isize_limit_min && rhs == -1)) { return none$(Opt$isize); }
    return some$(Opt$isize, lhs / rhs);
}
$inline_always Opt$isize isize_modChkd(isize lhs, isize rhs) {
    if (rhs == 0 || (lhs == isize_limit_min && rhs == -1)) { return none$(Opt$isize); }
    return some$(Opt$isize, lhs % rhs);
}
$inline_always Opt$isize isize_negChkd(isize x) {
    if (x == isize_limit_min) { return none$(Opt$isize); }
    return some$(Opt$isize, -x);
}
$inline_always Opt$isize isize_absChkd(isize x) {
    if (x == isize_limit_min) { return none$(Opt$isize); }
    return some$(Opt$isize, (x < 0) ? -x : x);
}
$inline_always Opt$isize isize_powChkd(isize base, u32 exp) {
    if (exp == 0) { return some$(Opt$isize, 1); }
    isize result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return none$(Opt$isize); }
    }
    return some$(Opt$isize, result);
}
$inline_always Opt$isize isize_shlChkd(isize x, u32 shift) {
    if (shift >= (sizeof(isize) * 8)) { return none$(Opt$isize); }
    if (shift == 0) { return some$(Opt$isize, x); }
    if (x < 0) { return none$(Opt$isize); }
    if (x > (isize_limit_max >> shift)) { return none$(Opt$isize); }
    return some$(Opt$isize, x << shift);
}
$inline_always Opt$isize isize_shrChkd(isize x, u32 shift) {
    if (shift >= (sizeof(isize) * 8)) { return none$(Opt$isize); }
    return some$(Opt$isize, x >> shift);
}

/*========== Wrapping Arithmetic Implementations ============================*/

/// Unsigned integer wrapping implementations

$inline_always u8 u8_addWrap(u8 lhs, u8 rhs) { return as$(u8, lhs + rhs); }
$inline_always u8 u8_subWrap(u8 lhs, u8 rhs) { return as$(u8, lhs - rhs); }
$inline_always u8 u8_mulWrap(u8 lhs, u8 rhs) { return as$(u8, lhs* rhs); }
$inline_always u8 u8_shlWrap(u8 x, u32 shift) { return as$(u8, x << (shift & 7)); }
$inline_always u8 u8_shrWrap(u8 x, u32 shift) { return as$(u8, x >> (shift & 7)); }

$inline_always u16 u16_addWrap(u16 lhs, u16 rhs) { return as$(u16, lhs + rhs); }
$inline_always u16 u16_subWrap(u16 lhs, u16 rhs) { return as$(u16, lhs - rhs); }
$inline_always u16 u16_mulWrap(u16 lhs, u16 rhs) { return as$(u16, lhs* rhs); }
$inline_always u16 u16_shlWrap(u16 x, u32 shift) { return as$(u16, x << (shift & 15)); }
$inline_always u16 u16_shrWrap(u16 x, u32 shift) { return as$(u16, x >> (shift & 15)); }

$inline_always u32 u32_addWrap(u32 lhs, u32 rhs) { return lhs + rhs; }
$inline_always u32 u32_subWrap(u32 lhs, u32 rhs) { return lhs - rhs; }
$inline_always u32 u32_mulWrap(u32 lhs, u32 rhs) { return lhs * rhs; }
$inline_always u32 u32_shlWrap(u32 x, u32 shift) { return x << (shift & 31); }
$inline_always u32 u32_shrWrap(u32 x, u32 shift) { return x >> (shift & 31); }

$inline_always u64 u64_addWrap(u64 lhs, u64 rhs) { return lhs + rhs; }
$inline_always u64 u64_subWrap(u64 lhs, u64 rhs) { return lhs - rhs; }
$inline_always u64 u64_mulWrap(u64 lhs, u64 rhs) { return lhs * rhs; }
$inline_always u64 u64_shlWrap(u64 x, u32 shift) { return x << (shift & 63); }
$inline_always u64 u64_shrWrap(u64 x, u32 shift) { return x >> (shift & 63); }

$inline_always usize usize_addWrap(usize lhs, usize rhs) { return lhs + rhs; }
$inline_always usize usize_subWrap(usize lhs, usize rhs) { return lhs - rhs; }
$inline_always usize usize_mulWrap(usize lhs, usize rhs) { return lhs * rhs; }
$inline_always usize usize_shlWrap(usize x, u32 shift) {
    return x << (shift & ((sizeof(usize) * 8) - 1));
}
$inline_always usize usize_shrWrap(usize x, u32 shift) {
    return x >> (shift & ((sizeof(usize) * 8) - 1));
}

/// Signed integer wrapping implementations

$inline_always i8 i8_addWrap(i8 lhs, i8 rhs) { return as$(i8, lhs + rhs); }
$inline_always i8 i8_subWrap(i8 lhs, i8 rhs) { return as$(i8, lhs - rhs); }
$inline_always i8 i8_mulWrap(i8 lhs, i8 rhs) { return as$(i8, lhs* rhs); }
$inline_always i8 i8_negWrap(i8 x) { return as$(i8, -x); }
$inline_always i8 i8_shlWrap(i8 x, u32 shift) { return as$(i8, x << (shift & 7)); }
$inline_always i8 i8_shrWrap(i8 x, u32 shift) { return as$(i8, x >> (shift & 7)); }

$inline_always i16 i16_addWrap(i16 lhs, i16 rhs) { return as$(i16, lhs + rhs); }
$inline_always i16 i16_subWrap(i16 lhs, i16 rhs) { return as$(i16, lhs - rhs); }
$inline_always i16 i16_mulWrap(i16 lhs, i16 rhs) { return as$(i16, lhs* rhs); }
$inline_always i16 i16_negWrap(i16 x) { return as$(i16, -x); }
$inline_always i16 i16_shlWrap(i16 x, u32 shift) { return as$(i16, x << (shift & 15)); }
$inline_always i16 i16_shrWrap(i16 x, u32 shift) { return as$(i16, x >> (shift & 15)); }

$inline_always i32 i32_addWrap(i32 lhs, i32 rhs) { return lhs + rhs; }
$inline_always i32 i32_subWrap(i32 lhs, i32 rhs) { return lhs - rhs; }
$inline_always i32 i32_mulWrap(i32 lhs, i32 rhs) { return lhs * rhs; }
$inline_always i32 i32_negWrap(i32 x) { return -x; }
$inline_always i32 i32_shlWrap(i32 x, u32 shift) { return x << (shift & 31); }
$inline_always i32 i32_shrWrap(i32 x, u32 shift) { return x >> (shift & 31); }

$inline_always i64 i64_addWrap(i64 lhs, i64 rhs) { return lhs + rhs; }
$inline_always i64 i64_subWrap(i64 lhs, i64 rhs) { return lhs - rhs; }
$inline_always i64 i64_mulWrap(i64 lhs, i64 rhs) { return lhs * rhs; }
$inline_always i64 i64_negWrap(i64 x) { return -x; }
$inline_always i64 i64_shlWrap(i64 x, u32 shift) { return x << (shift & 63); }
$inline_always i64 i64_shrWrap(i64 x, u32 shift) { return x >> (shift & 63); }

$inline_always isize isize_addWrap(isize lhs, isize rhs) { return lhs + rhs; }
$inline_always isize isize_subWrap(isize lhs, isize rhs) { return lhs - rhs; }
$inline_always isize isize_mulWrap(isize lhs, isize rhs) { return lhs * rhs; }
$inline_always isize isize_negWrap(isize x) { return -x; }
$inline_always isize isize_shlWrap(isize x, u32 shift) {
    return x << (shift & ((sizeof(isize) * 8) - 1));
}
$inline_always isize isize_shrWrap(isize x, u32 shift) {
    return x >> (shift & ((sizeof(isize) * 8) - 1));
}

/*========== Saturating Arithmetic Implementations ==========================*/

/// Unsigned integer saturating implementations

$inline_always u8 u8_addSat(u8 lhs, u8 rhs) {
    u8 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return u8_limit_max; }
    return result;
}
$inline_always u8 u8_subSat(u8 lhs, u8 rhs) {
    u8 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return u8_limit_min; }
    return result;
}
$inline_always u8 u8_mulSat(u8 lhs, u8 rhs) {
    u8 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return u8_limit_max; }
    return result;
}
$inline_always u8 u8_powSat(u8 base, u32 exp) {
    if (exp == 0) { return 1; }
    u8 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return u8_limit_max; }
    }
    return result;
}

$inline_always u16 u16_addSat(u16 lhs, u16 rhs) {
    u16 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return u16_limit_max; }
    return result;
}
$inline_always u16 u16_subSat(u16 lhs, u16 rhs) {
    u16 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return u16_limit_min; }
    return result;
}
$inline_always u16 u16_mulSat(u16 lhs, u16 rhs) {
    u16 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return u16_limit_max; }
    return result;
}
$inline_always u16 u16_powSat(u16 base, u32 exp) {
    if (exp == 0) { return 1; }
    u16 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return u16_limit_max; }
    }
    return result;
}

$inline_always u32 u32_addSat(u32 lhs, u32 rhs) {
    u32 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return u32_limit_max; }
    return result;
}
$inline_always u32 u32_subSat(u32 lhs, u32 rhs) {
    u32 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return u32_limit_min; }
    return result;
}
$inline_always u32 u32_mulSat(u32 lhs, u32 rhs) {
    u32 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return u32_limit_max; }
    return result;
}
$inline_always u32 u32_powSat(u32 base, u32 exp) {
    if (exp == 0) { return 1; }
    u32 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return u32_limit_max; }
    }
    return result;
}

$inline_always u64 u64_addSat(u64 lhs, u64 rhs) {
    u64 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return u64_limit_max; }
    return result;
}
$inline_always u64 u64_subSat(u64 lhs, u64 rhs) {
    u64 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return u64_limit_min; }
    return result;
}
$inline_always u64 u64_mulSat(u64 lhs, u64 rhs) {
    u64 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return u64_limit_max; }
    return result;
}
$inline_always u64 u64_powSat(u64 base, u32 exp) {
    if (exp == 0) { return 1; }
    u64 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return u64_limit_max; }
    }
    return result;
}

$inline_always usize usize_addSat(usize lhs, usize rhs) {
    usize result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return usize_limit_max; }
    return result;
}
$inline_always usize usize_subSat(usize lhs, usize rhs) {
    usize result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return usize_limit_min; }
    return result;
}
$inline_always usize usize_mulSat(usize lhs, usize rhs) {
    usize result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return usize_limit_max; }
    return result;
}
$inline_always usize usize_powSat(usize base, u32 exp) {
    if (exp == 0) { return 1; }
    usize result = 1;
    for (u32 i = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) { return usize_limit_max; }
    }
    return result;
}

/// Signed integer saturating implementations

$inline_always i8 i8_addSat(i8 lhs, i8 rhs) {
    i8 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) {
        return (lhs > 0) ? i8_limit_max : i8_limit_min;
    }
    return result;
}
$inline_always i8 i8_subSat(i8 lhs, i8 rhs) {
    i8 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) {
        return (lhs > 0) ? i8_limit_max : i8_limit_min;
    }
    return result;
}
$inline_always i8 i8_mulSat(i8 lhs, i8 rhs) {
    i8 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) {
        bool same_sign = (lhs > 0 && rhs > 0) || (lhs < 0 && rhs < 0);
        return same_sign ? i8_limit_max : i8_limit_min;
    }
    return result;
}
$inline_always i8 i8_divSat(i8 lhs, i8 rhs) {
    if (rhs == 0) { return (lhs >= 0) ? i8_limit_max : i8_limit_min; }
    if (lhs == i8_limit_min && rhs == -1) { return i8_limit_max; }
    return as$(i8, lhs / rhs);
}
$inline_always i8 i8_negSat(i8 x) {
    if (x == i8_limit_min) { return i8_limit_max; }
    return as$(i8, -x);
}
$inline_always i8 i8_absSat(i8 x) {
    if (x == i8_limit_min) { return i8_limit_max; }
    return as$(i8, (x < 0) ? -x : x);
}
$inline_always i8 i8_powSat(i8 base, u32 exp) {
    if (exp == 0) { return 1; }
    i8 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        i8 old_result = result;
        if (__builtin_mul_overflow(result, base, &result)) {
            bool same_sign = (old_result > 0 && base > 0) || (old_result < 0 && base < 0);
            return same_sign ? i8_limit_max : i8_limit_min;
        }
    }
    return result;
}

$inline_always i16 i16_addSat(i16 lhs, i16 rhs) {
    i16 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) {
        return (lhs > 0) ? i16_limit_max : i16_limit_min;
    }
    return result;
}
$inline_always i16 i16_subSat(i16 lhs, i16 rhs) {
    i16 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) {
        return (lhs > 0) ? i16_limit_max : i16_limit_min;
    }
    return result;
}
$inline_always i16 i16_mulSat(i16 lhs, i16 rhs) {
    i16 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) {
        bool same_sign = (lhs > 0 && rhs > 0) || (lhs < 0 && rhs < 0);
        return same_sign ? i16_limit_max : i16_limit_min;
    }
    return result;
}
$inline_always i16 i16_divSat(i16 lhs, i16 rhs) {
    if (rhs == 0) { return (lhs >= 0) ? i16_limit_max : i16_limit_min; }
    if (lhs == i16_limit_min && rhs == -1) { return i16_limit_max; }
    return as$(i16, lhs / rhs);
}
$inline_always i16 i16_negSat(i16 x) {
    if (x == i16_limit_min) { return i16_limit_max; }
    return as$(i16, -x);
}
$inline_always i16 i16_absSat(i16 x) {
    if (x == i16_limit_min) { return i16_limit_max; }
    return as$(i16, (x < 0) ? -x : x);
}
$inline_always i16 i16_powSat(i16 base, u32 exp) {
    if (exp == 0) { return 1; }
    i16 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        i16 old_result = result;
        if (__builtin_mul_overflow(result, base, &result)) {
            bool same_sign = (old_result > 0 && base > 0) || (old_result < 0 && base < 0);
            return same_sign ? i16_limit_max : i16_limit_min;
        }
    }
    return result;
}

$inline_always i32 i32_addSat(i32 lhs, i32 rhs) {
    i32 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) {
        return (lhs > 0) ? i32_limit_max : i32_limit_min;
    }
    return result;
}
$inline_always i32 i32_subSat(i32 lhs, i32 rhs) {
    i32 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) {
        return (lhs > 0) ? i32_limit_max : i32_limit_min;
    }
    return result;
}
$inline_always i32 i32_mulSat(i32 lhs, i32 rhs) {
    i32 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) {
        bool same_sign = (lhs > 0 && rhs > 0) || (lhs < 0 && rhs < 0);
        return same_sign ? i32_limit_max : i32_limit_min;
    }
    return result;
}
$inline_always i32 i32_divSat(i32 lhs, i32 rhs) {
    if (rhs == 0) { return (lhs >= 0) ? i32_limit_max : i32_limit_min; }
    if (lhs == i32_limit_min && rhs == -1) { return i32_limit_max; }
    return lhs / rhs;
}
$inline_always i32 i32_negSat(i32 x) {
    if (x == i32_limit_min) { return i32_limit_max; }
    return -x;
}
$inline_always i32 i32_absSat(i32 x) {
    if (x == i32_limit_min) { return i32_limit_max; }
    return (x < 0) ? -x : x;
}
$inline_always i32 i32_powSat(i32 base, u32 exp) {
    if (exp == 0) { return 1; }
    i32 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        i32 old_result = result;
        if (__builtin_mul_overflow(result, base, &result)) {
            bool same_sign = (old_result > 0 && base > 0) || (old_result < 0 && base < 0);
            return same_sign ? i32_limit_max : i32_limit_min;
        }
    }
    return result;
}

$inline_always i64 i64_addSat(i64 lhs, i64 rhs) {
    i64 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) {
        return (lhs > 0) ? i64_limit_max : i64_limit_min;
    }
    return result;
}
$inline_always i64 i64_subSat(i64 lhs, i64 rhs) {
    i64 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) {
        return (lhs > 0) ? i64_limit_max : i64_limit_min;
    }
    return result;
}
$inline_always i64 i64_mulSat(i64 lhs, i64 rhs) {
    i64 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) {
        bool same_sign = (lhs > 0 && rhs > 0) || (lhs < 0 && rhs < 0);
        return same_sign ? i64_limit_max : i64_limit_min;
    }
    return result;
}
$inline_always i64 i64_divSat(i64 lhs, i64 rhs) {
    if (rhs == 0) { return (lhs >= 0) ? i64_limit_max : i64_limit_min; }
    if (lhs == i64_limit_min && rhs == -1) { return i64_limit_max; }
    return lhs / rhs;
}
$inline_always i64 i64_negSat(i64 x) {
    if (x == i64_limit_min) { return i64_limit_max; }
    return -x;
}
$inline_always i64 i64_absSat(i64 x) {
    if (x == i64_limit_min) { return i64_limit_max; }
    return (x < 0) ? -x : x;
}
$inline_always i64 i64_powSat(i64 base, u32 exp) {
    if (exp == 0) { return 1; }
    i64 result = 1;
    for (u32 i = 0; i < exp; ++i) {
        i64 old_result = result;
        if (__builtin_mul_overflow(result, base, &result)) {
            bool same_sign = (old_result > 0 && base > 0) || (old_result < 0 && base < 0);
            return same_sign ? i64_limit_max : i64_limit_min;
        }
    }
    return result;
}

$inline_always isize isize_addSat(isize lhs, isize rhs) {
    isize result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) {
        return (lhs > 0) ? isize_limit_max : isize_limit_min;
    }
    return result;
}
$inline_always isize isize_subSat(isize lhs, isize rhs) {
    isize result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) {
        return (lhs > 0) ? isize_limit_max : isize_limit_min;
    }
    return result;
}
$inline_always isize isize_mulSat(isize lhs, isize rhs) {
    isize result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) {
        bool same_sign = (lhs > 0 && rhs > 0) || (lhs < 0 && rhs < 0);
        return same_sign ? isize_limit_max : isize_limit_min;
    }
    return result;
}
$inline_always isize isize_divSat(isize lhs, isize rhs) {
    if (rhs == 0) { return (lhs >= 0) ? isize_limit_max : isize_limit_min; }
    if (lhs == isize_limit_min && rhs == -1) { return isize_limit_max; }
    return lhs / rhs;
}
$inline_always isize isize_negSat(isize x) {
    if (x == isize_limit_min) { return isize_limit_max; }
    return -x;
}
$inline_always isize isize_absSat(isize x) {
    if (x == isize_limit_min) { return isize_limit_max; }
    return (x < 0) ? -x : x;
}
$inline_always isize isize_powSat(isize base, u32 exp) {
    if (exp == 0) { return 1; }
    isize result = 1;
    for (u32 i = 0; i < exp; ++i) {
        isize old_result = result;
        if (__builtin_mul_overflow(result, base, &result)) {
            bool same_sign = (old_result > 0 && base > 0) || (old_result < 0 && base < 0);
            return same_sign ? isize_limit_max : isize_limit_min;
        }
    }
    return result;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* int__included */

/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    int.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-02 (date of creation)
 * @updated 2026-06-03 (date of last update)
 * @ingroup dasae-headers(dh)/prl
 * @prefix  (none)
 *
 * @brief   Integer type overflow checking utilities
 * @details Provides utilities for checking integer overflows.
 */
#pragma once
#ifndef prl_int__included
#define prl_int__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

/// Unsigned integer basic arithmetic with debug assertions

$attr($inline_always)
$static fn_((u8_add(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_sub(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_mul(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_div(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_mod(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_pow(u8 base, u32 exp))(u8));
$attr($inline_always)
$static fn_((u8_shl(u8 x, u32 shift))(u8));
$attr($inline_always)
$static fn_((u8_shr(u8 x, u32 shift))(u8));

$attr($inline_always)
$static fn_((u16_add(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_sub(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_mul(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_div(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_mod(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_pow(u16 base, u32 exp))(u16));
$attr($inline_always)
$static fn_((u16_shl(u16 x, u32 shift))(u16));
$attr($inline_always)
$static fn_((u16_shr(u16 x, u32 shift))(u16));

$attr($inline_always)
$static fn_((u32_add(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_sub(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_mul(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_div(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_mod(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_pow(u32 base, u32 exp))(u32));
$attr($inline_always)
$static fn_((u32_shl(u32 x, u32 shift))(u32));
$attr($inline_always)
$static fn_((u32_shr(u32 x, u32 shift))(u32));

$attr($inline_always)
$static fn_((u64_add(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_sub(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_mul(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_div(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_mod(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_pow(u64 base, u32 exp))(u64));
$attr($inline_always)
$static fn_((u64_shl(u64 x, u32 shift))(u64));
$attr($inline_always)
$static fn_((u64_shr(u64 x, u32 shift))(u64));

$attr($inline_always)
$static fn_((usize_add(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_sub(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_mul(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_div(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_mod(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_pow(usize base, u32 exp))(usize));
$attr($inline_always)
$static fn_((usize_shl(usize x, u32 shift))(usize));
$attr($inline_always)
$static fn_((usize_shr(usize x, u32 shift))(usize));

/// Signed integer basic arithmetic with debug assertions

$attr($inline_always)
$static fn_((i8_add(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_sub(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_mul(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_div(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_mod(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_neg(i8 x))(i8));
$attr($inline_always)
$static fn_((i8_abs(i8 x))(i8));
$attr($inline_always)
$static fn_((i8_pow(i8 base, u32 exp))(i8));
$attr($inline_always)
$static fn_((i8_shl(i8 x, u32 shift))(i8));
$attr($inline_always)
$static fn_((i8_shr(i8 x, u32 shift))(i8));

$attr($inline_always)
$static fn_((i16_add(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_sub(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_mul(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_div(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_mod(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_neg(i16 x))(i16));
$attr($inline_always)
$static fn_((i16_abs(i16 x))(i16));
$attr($inline_always)
$static fn_((i16_pow(i16 base, u32 exp))(i16));
$attr($inline_always)
$static fn_((i16_shl(i16 x, u32 shift))(i16));
$attr($inline_always)
$static fn_((i16_shr(i16 x, u32 shift))(i16));

$attr($inline_always)
$static fn_((i32_add(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_sub(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_mul(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_div(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_mod(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_neg(i32 x))(i32));
$attr($inline_always)
$static fn_((i32_abs(i32 x))(i32));
$attr($inline_always)
$static fn_((i32_pow(i32 base, u32 exp))(i32));
$attr($inline_always)
$static fn_((i32_shl(i32 x, u32 shift))(i32));
$attr($inline_always)
$static fn_((i32_shr(i32 x, u32 shift))(i32));

$attr($inline_always)
$static fn_((i64_add(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_sub(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_mul(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_div(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_mod(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_neg(i64 x))(i64));
$attr($inline_always)
$static fn_((i64_abs(i64 x))(i64));
$attr($inline_always)
$static fn_((i64_pow(i64 base, u32 exp))(i64));
$attr($inline_always)
$static fn_((i64_shl(i64 x, u32 shift))(i64));
$attr($inline_always)
$static fn_((i64_shr(i64 x, u32 shift))(i64));

$attr($inline_always)
$static fn_((isize_add(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_sub(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_mul(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_div(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_mod(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_neg(isize x))(isize));
$attr($inline_always)
$static fn_((isize_abs(isize x))(isize));
$attr($inline_always)
$static fn_((isize_pow(isize base, u32 exp))(isize));
$attr($inline_always)
$static fn_((isize_shl(isize x, u32 shift))(isize));
$attr($inline_always)
$static fn_((isize_shr(isize x, u32 shift))(isize));

/// Unsigned integer overflow checking functions

$attr($inline_always)
$static fn_((u8_addChkd(u8 lhs, u8 rhs))(O$u8));
$attr($inline_always)
$static fn_((u8_subChkd(u8 lhs, u8 rhs))(O$u8));
$attr($inline_always)
$static fn_((u8_mulChkd(u8 lhs, u8 rhs))(O$u8));
$attr($inline_always)
$static fn_((u8_divChkd(u8 lhs, u8 rhs))(O$u8));
$attr($inline_always)
$static fn_((u8_modChkd(u8 lhs, u8 rhs))(O$u8));
$attr($inline_always)
$static fn_((u8_powChkd(u8 base, u32 exp))(O$u8));
$attr($inline_always)
$static fn_((u8_shlChkd(u8 x, u32 shift))(O$u8));
$attr($inline_always)
$static fn_((u8_shrChkd(u8 x, u32 shift))(O$u8));

$attr($inline_always)
$static fn_((u16_addChkd(u16 lhs, u16 rhs))(O$u16));
$attr($inline_always)
$static fn_((u16_subChkd(u16 lhs, u16 rhs))(O$u16));
$attr($inline_always)
$static fn_((u16_mulChkd(u16 lhs, u16 rhs))(O$u16));
$attr($inline_always)
$static fn_((u16_divChkd(u16 lhs, u16 rhs))(O$u16));
$attr($inline_always)
$static fn_((u16_modChkd(u16 lhs, u16 rhs))(O$u16));
$attr($inline_always)
$static fn_((u16_powChkd(u16 base, u32 exp))(O$u16));
$attr($inline_always)
$static fn_((u16_shlChkd(u16 x, u32 shift))(O$u16));
$attr($inline_always)
$static fn_((u16_shrChkd(u16 x, u32 shift))(O$u16));

$attr($inline_always)
$static fn_((u32_addChkd(u32 lhs, u32 rhs))(O$u32));
$attr($inline_always)
$static fn_((u32_subChkd(u32 lhs, u32 rhs))(O$u32));
$attr($inline_always)
$static fn_((u32_mulChkd(u32 lhs, u32 rhs))(O$u32));
$attr($inline_always)
$static fn_((u32_divChkd(u32 lhs, u32 rhs))(O$u32));
$attr($inline_always)
$static fn_((u32_modChkd(u32 lhs, u32 rhs))(O$u32));
$attr($inline_always)
$static fn_((u32_powChkd(u32 base, u32 exp))(O$u32));
$attr($inline_always)
$static fn_((u32_shlChkd(u32 x, u32 shift))(O$u32));
$attr($inline_always)
$static fn_((u32_shrChkd(u32 x, u32 shift))(O$u32));

$attr($inline_always)
$static fn_((u64_addChkd(u64 lhs, u64 rhs))(O$u64));
$attr($inline_always)
$static fn_((u64_subChkd(u64 lhs, u64 rhs))(O$u64));
$attr($inline_always)
$static fn_((u64_mulChkd(u64 lhs, u64 rhs))(O$u64));
$attr($inline_always)
$static fn_((u64_divChkd(u64 lhs, u64 rhs))(O$u64));
$attr($inline_always)
$static fn_((u64_modChkd(u64 lhs, u64 rhs))(O$u64));
$attr($inline_always)
$static fn_((u64_powChkd(u64 base, u32 exp))(O$u64));
$attr($inline_always)
$static fn_((u64_shlChkd(u64 x, u32 shift))(O$u64));
$attr($inline_always)
$static fn_((u64_shrChkd(u64 x, u32 shift))(O$u64));

$attr($inline_always)
$static fn_((usize_addChkd(usize lhs, usize rhs))(O$usize));
$attr($inline_always)
$static fn_((usize_subChkd(usize lhs, usize rhs))(O$usize));
$attr($inline_always)
$static fn_((usize_mulChkd(usize lhs, usize rhs))(O$usize));
$attr($inline_always)
$static fn_((usize_divChkd(usize lhs, usize rhs))(O$usize));
$attr($inline_always)
$static fn_((usize_modChkd(usize lhs, usize rhs))(O$usize));
$attr($inline_always)
$static fn_((usize_powChkd(usize base, u32 exp))(O$usize));
$attr($inline_always)
$static fn_((usize_shlChkd(usize x, u32 shift))(O$usize));
$attr($inline_always)
$static fn_((usize_shrChkd(usize x, u32 shift))(O$usize));

/// Signed integer overflow checking functions

$attr($inline_always)
$static fn_((i8_addChkd(i8 lhs, i8 rhs))(O$i8));
$attr($inline_always)
$static fn_((i8_subChkd(i8 lhs, i8 rhs))(O$i8));
$attr($inline_always)
$static fn_((i8_mulChkd(i8 lhs, i8 rhs))(O$i8));
$attr($inline_always)
$static fn_((i8_divChkd(i8 lhs, i8 rhs))(O$i8));
$attr($inline_always)
$static fn_((i8_modChkd(i8 lhs, i8 rhs))(O$i8));
$attr($inline_always)
$static fn_((i8_negChkd(i8 x))(O$i8));
$attr($inline_always)
$static fn_((i8_absChkd(i8 x))(O$i8));
$attr($inline_always)
$static fn_((i8_powChkd(i8 base, u32 exp))(O$i8));
$attr($inline_always)
$static fn_((i8_shlChkd(i8 x, u32 shift))(O$i8));
$attr($inline_always)
$static fn_((i8_shrChkd(i8 x, u32 shift))(O$i8));

$attr($inline_always)
$static fn_((i16_addChkd(i16 lhs, i16 rhs))(O$i16));
$attr($inline_always)
$static fn_((i16_subChkd(i16 lhs, i16 rhs))(O$i16));
$attr($inline_always)
$static fn_((i16_mulChkd(i16 lhs, i16 rhs))(O$i16));
$attr($inline_always)
$static fn_((i16_divChkd(i16 lhs, i16 rhs))(O$i16));
$attr($inline_always)
$static fn_((i16_modChkd(i16 lhs, i16 rhs))(O$i16));
$attr($inline_always)
$static fn_((i16_negChkd(i16 x))(O$i16));
$attr($inline_always)
$static fn_((i16_absChkd(i16 x))(O$i16));
$attr($inline_always)
$static fn_((i16_powChkd(i16 base, u32 exp))(O$i16));
$attr($inline_always)
$static fn_((i16_shlChkd(i16 x, u32 shift))(O$i16));
$attr($inline_always)
$static fn_((i16_shrChkd(i16 x, u32 shift))(O$i16));

$attr($inline_always)
$static fn_((i32_addChkd(i32 lhs, i32 rhs))(O$i32));
$attr($inline_always)
$static fn_((i32_subChkd(i32 lhs, i32 rhs))(O$i32));
$attr($inline_always)
$static fn_((i32_mulChkd(i32 lhs, i32 rhs))(O$i32));
$attr($inline_always)
$static fn_((i32_divChkd(i32 lhs, i32 rhs))(O$i32));
$attr($inline_always)
$static fn_((i32_modChkd(i32 lhs, i32 rhs))(O$i32));
$attr($inline_always)
$static fn_((i32_negChkd(i32 x))(O$i32));
$attr($inline_always)
$static fn_((i32_absChkd(i32 x))(O$i32));
$attr($inline_always)
$static fn_((i32_powChkd(i32 base, u32 exp))(O$i32));
$attr($inline_always)
$static fn_((i32_shlChkd(i32 x, u32 shift))(O$i32));
$attr($inline_always)
$static fn_((i32_shrChkd(i32 x, u32 shift))(O$i32));

$attr($inline_always)
$static fn_((i64_addChkd(i64 lhs, i64 rhs))(O$i64));
$attr($inline_always)
$static fn_((i64_subChkd(i64 lhs, i64 rhs))(O$i64));
$attr($inline_always)
$static fn_((i64_mulChkd(i64 lhs, i64 rhs))(O$i64));
$attr($inline_always)
$static fn_((i64_divChkd(i64 lhs, i64 rhs))(O$i64));
$attr($inline_always)
$static fn_((i64_modChkd(i64 lhs, i64 rhs))(O$i64));
$attr($inline_always)
$static fn_((i64_negChkd(i64 x))(O$i64));
$attr($inline_always)
$static fn_((i64_absChkd(i64 x))(O$i64));
$attr($inline_always)
$static fn_((i64_powChkd(i64 base, u32 exp))(O$i64));
$attr($inline_always)
$static fn_((i64_shlChkd(i64 x, u32 shift))(O$i64));
$attr($inline_always)
$static fn_((i64_shrChkd(i64 x, u32 shift))(O$i64));

$attr($inline_always)
$static fn_((isize_addChkd(isize lhs, isize rhs))(O$isize));
$attr($inline_always)
$static fn_((isize_subChkd(isize lhs, isize rhs))(O$isize));
$attr($inline_always)
$static fn_((isize_mulChkd(isize lhs, isize rhs))(O$isize));
$attr($inline_always)
$static fn_((isize_divChkd(isize lhs, isize rhs))(O$isize));
$attr($inline_always)
$static fn_((isize_modChkd(isize lhs, isize rhs))(O$isize));
$attr($inline_always)
$static fn_((isize_negChkd(isize x))(O$isize));
$attr($inline_always)
$static fn_((isize_absChkd(isize x))(O$isize));
$attr($inline_always)
$static fn_((isize_powChkd(isize base, u32 exp))(O$isize));
$attr($inline_always)
$static fn_((isize_shlChkd(isize x, u32 shift))(O$isize));
$attr($inline_always)
$static fn_((isize_shrChkd(isize x, u32 shift))(O$isize));

/// Unsigned integer wrapping arithmetic functions

$attr($inline_always)
$static fn_((u8_addWrap(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_subWrap(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_mulWrap(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_shlWrap(u8 x, u32 shift))(u8));
$attr($inline_always)
$static fn_((u8_shrWrap(u8 x, u32 shift))(u8));

$attr($inline_always)
$static fn_((u16_addWrap(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_subWrap(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_mulWrap(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_shlWrap(u16 x, u32 shift))(u16));
$attr($inline_always)
$static fn_((u16_shrWrap(u16 x, u32 shift))(u16));

$attr($inline_always)
$static fn_((u32_addWrap(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_subWrap(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_mulWrap(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_shlWrap(u32 x, u32 shift))(u32));
$attr($inline_always)
$static fn_((u32_shrWrap(u32 x, u32 shift))(u32));

$attr($inline_always)
$static fn_((u64_addWrap(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_subWrap(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_mulWrap(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_shlWrap(u64 x, u32 shift))(u64));
$attr($inline_always)
$static fn_((u64_shrWrap(u64 x, u32 shift))(u64));

$attr($inline_always)
$static fn_((usize_addWrap(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_subWrap(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_mulWrap(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_shlWrap(usize x, u32 shift))(usize));
$attr($inline_always)
$static fn_((usize_shrWrap(usize x, u32 shift))(usize));

/// Signed integer wrapping arithmetic functions

$attr($inline_always)
$static fn_((i8_addWrap(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_subWrap(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_mulWrap(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_negWrap(i8 x))(i8));
$attr($inline_always)
$static fn_((i8_shlWrap(i8 x, u32 shift))(i8));
$attr($inline_always)
$static fn_((i8_shrWrap(i8 x, u32 shift))(i8));

$attr($inline_always)
$static fn_((i16_addWrap(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_subWrap(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_mulWrap(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_negWrap(i16 x))(i16));
$attr($inline_always)
$static fn_((i16_shlWrap(i16 x, u32 shift))(i16));
$attr($inline_always)
$static fn_((i16_shrWrap(i16 x, u32 shift))(i16));

$attr($inline_always)
$static fn_((i32_addWrap(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_subWrap(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_mulWrap(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_negWrap(i32 x))(i32));
$attr($inline_always)
$static fn_((i32_shlWrap(i32 x, u32 shift))(i32));
$attr($inline_always)
$static fn_((i32_shrWrap(i32 x, u32 shift))(i32));

$attr($inline_always)
$static fn_((i64_addWrap(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_subWrap(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_mulWrap(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_negWrap(i64 x))(i64));
$attr($inline_always)
$static fn_((i64_shlWrap(i64 x, u32 shift))(i64));
$attr($inline_always)
$static fn_((i64_shrWrap(i64 x, u32 shift))(i64));

$attr($inline_always)
$static fn_((isize_addWrap(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_subWrap(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_mulWrap(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_negWrap(isize x))(isize));
$attr($inline_always)
$static fn_((isize_shlWrap(isize x, u32 shift))(isize));
$attr($inline_always)
$static fn_((isize_shrWrap(isize x, u32 shift))(isize));

/// Unsigned integer saturating arithmetic functions

$attr($inline_always)
$static fn_((u8_addSat(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_subSat(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_mulSat(u8 lhs, u8 rhs))(u8));
$attr($inline_always)
$static fn_((u8_powSat(u8 base, u32 exp))(u8));

$attr($inline_always)
$static fn_((u16_addSat(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_subSat(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_mulSat(u16 lhs, u16 rhs))(u16));
$attr($inline_always)
$static fn_((u16_powSat(u16 base, u32 exp))(u16));

$attr($inline_always)
$static fn_((u32_addSat(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_subSat(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_mulSat(u32 lhs, u32 rhs))(u32));
$attr($inline_always)
$static fn_((u32_powSat(u32 base, u32 exp))(u32));

$attr($inline_always)
$static fn_((u64_addSat(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_subSat(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_mulSat(u64 lhs, u64 rhs))(u64));
$attr($inline_always)
$static fn_((u64_powSat(u64 base, u32 exp))(u64));

$attr($inline_always)
$static fn_((usize_addSat(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_subSat(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_mulSat(usize lhs, usize rhs))(usize));
$attr($inline_always)
$static fn_((usize_powSat(usize base, u32 exp))(usize));

/// Signed integer saturating arithmetic functions

$attr($inline_always)
$static fn_((i8_addSat(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_subSat(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_mulSat(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_divSat(i8 lhs, i8 rhs))(i8));
$attr($inline_always)
$static fn_((i8_negSat(i8 x))(i8));
$attr($inline_always)
$static fn_((i8_absSat(i8 x))(i8));
$attr($inline_always)
$static fn_((i8_powSat(i8 base, u32 exp))(i8));

$attr($inline_always)
$static fn_((i16_addSat(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_subSat(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_mulSat(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_divSat(i16 lhs, i16 rhs))(i16));
$attr($inline_always)
$static fn_((i16_negSat(i16 x))(i16));
$attr($inline_always)
$static fn_((i16_absSat(i16 x))(i16));
$attr($inline_always)
$static fn_((i16_powSat(i16 base, u32 exp))(i16));

$attr($inline_always)
$static fn_((i32_addSat(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_subSat(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_mulSat(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_divSat(i32 lhs, i32 rhs))(i32));
$attr($inline_always)
$static fn_((i32_negSat(i32 x))(i32));
$attr($inline_always)
$static fn_((i32_absSat(i32 x))(i32));
$attr($inline_always)
$static fn_((i32_powSat(i32 base, u32 exp))(i32));

$attr($inline_always)
$static fn_((i64_addSat(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_subSat(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_mulSat(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_divSat(i64 lhs, i64 rhs))(i64));
$attr($inline_always)
$static fn_((i64_negSat(i64 x))(i64));
$attr($inline_always)
$static fn_((i64_absSat(i64 x))(i64));
$attr($inline_always)
$static fn_((i64_powSat(i64 base, u32 exp))(i64));

$attr($inline_always)
$static fn_((isize_addSat(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_subSat(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_mulSat(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_divSat(isize lhs, isize rhs))(isize));
$attr($inline_always)
$static fn_((isize_negSat(isize x))(isize));
$attr($inline_always)
$static fn_((isize_absSat(isize x))(isize));
$attr($inline_always)
$static fn_((isize_powSat(isize base, u32 exp))(isize));

/*========== Macros and Definitions =========================================*/

/// Unsigned integer basic arithmetic implementations
fn_((u8_add(u8 lhs, u8 rhs))(u8)) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &l0$((u8)))), as$(u8)(lhs + rhs);
};
fn_((u8_sub(u8 lhs, u8 rhs))(u8)) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &l0$((u8)))), as$(u8)(lhs - rhs);
};
fn_((u8_mul(u8 lhs, u8 rhs))(u8)) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &l0$((u8)))), as$(u8)(lhs * rhs);
};
fn_((u8_div(u8 lhs, u8 rhs))(u8)) {
    return debug_assert(rhs != 0), lhs / rhs;
};
fn_((u8_mod(u8 lhs, u8 rhs))(u8)) {
    return debug_assert(rhs != 0), lhs % rhs;
};
fn_((u8_pow(u8 base, u32 exp))(u8)) {
    if (exp == 0) return 1;
    var_(result, u8) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &l0$((u8))));
        result = as$(u8)(result * base);
    }
    return result;
};
fn_((u8_shl(u8 x, u32 shift))(u8)) {
    return debug_assert(shift < 8), as$(u8)(x << shift);
};
fn_((u8_shr(u8 x, u32 shift))(u8)) {
    return debug_assert(shift < 8), as$(u8)(x >> shift);
};
fn_((u16_add(u16 lhs, u16 rhs))(u16)) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &l0$((u16)))), as$(u16)(lhs + rhs);
};
fn_((u16_sub(u16 lhs, u16 rhs))(u16)) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &l0$((u16)))), as$(u16)(lhs - rhs);
};
fn_((u16_mul(u16 lhs, u16 rhs))(u16)) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &l0$((u16)))), as$(u16)(lhs * rhs);
};
fn_((u16_div(u16 lhs, u16 rhs))(u16)) {
    return debug_assert(rhs != 0), lhs / rhs;
};
fn_((u16_mod(u16 lhs, u16 rhs))(u16)) {
    return debug_assert(rhs != 0), lhs % rhs;
};
fn_((u16_pow(u16 base, u32 exp))(u16)) {
    if (exp == 0) return 1;
    var_(result, u16) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &l0$((u16))));
        result = as$(u16)(result * base);
    }
    return result;
};
fn_((u16_shl(u16 x, u32 shift))(u16)) {
    return debug_assert(shift < 16), as$(u16)(x << shift);
};
fn_((u16_shr(u16 x, u32 shift))(u16)) {
    return debug_assert(shift < 16), as$(u16)(x >> shift);
};
fn_((u32_add(u32 lhs, u32 rhs))(u32)) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &l0$((u32)))), lhs + rhs;
};
fn_((u32_sub(u32 lhs, u32 rhs))(u32)) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &l0$((u32)))), lhs - rhs;
};
fn_((u32_mul(u32 lhs, u32 rhs))(u32)) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &l0$((u32)))), lhs * rhs;
};
fn_((u32_div(u32 lhs, u32 rhs))(u32)) {
    return debug_assert(rhs != 0), lhs / rhs;
};
fn_((u32_mod(u32 lhs, u32 rhs))(u32)) {
    return debug_assert(rhs != 0), lhs % rhs;
};
fn_((u32_pow(u32 base, u32 exp))(u32)) {
    if (exp == 0) return 1;
    var_(result, u32) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &l0$((u32))));
        result *= base;
    }
    return result;
};
fn_((u32_shl(u32 x, u32 shift))(u32)) {
    return debug_assert(shift < 32), x << shift;
};
fn_((u32_shr(u32 x, u32 shift))(u32)) {
    return debug_assert(shift < 32), x >> shift;
};
fn_((u64_add(u64 lhs, u64 rhs))(u64)) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &l0$((u64)))), lhs + rhs;
};
fn_((u64_sub(u64 lhs, u64 rhs))(u64)) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &l0$((u64)))), lhs - rhs;
};
fn_((u64_mul(u64 lhs, u64 rhs))(u64)) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &l0$((u64)))), lhs * rhs;
};
fn_((u64_div(u64 lhs, u64 rhs))(u64)) {
    return debug_assert(rhs != 0), lhs / rhs;
};
fn_((u64_mod(u64 lhs, u64 rhs))(u64)) {
    return debug_assert(rhs != 0), lhs % rhs;
};
fn_((u64_pow(u64 base, u32 exp))(u64)) {
    if (exp == 0) return 1;
    var_(result, u64) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &l0$((u64))));
        result *= base;
    }
    return result;
};
fn_((u64_shl(u64 x, u32 shift))(u64)) {
    return debug_assert(shift < 64), x << shift;
};
fn_((u64_shr(u64 x, u32 shift))(u64)) {
    return debug_assert(shift < 64), x >> shift;
};
fn_((usize_add(usize lhs, usize rhs))(usize)) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &l0$((usize)))), lhs + rhs;
};
fn_((usize_sub(usize lhs, usize rhs))(usize)) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &l0$((usize)))), lhs - rhs;
};
fn_((usize_mul(usize lhs, usize rhs))(usize)) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &l0$((usize)))), lhs * rhs;
};
fn_((usize_div(usize lhs, usize rhs))(usize)) {
    return debug_assert(rhs != 0), lhs / rhs;
};
fn_((usize_mod(usize lhs, usize rhs))(usize)) {
    return debug_assert(rhs != 0), lhs % rhs;
};
fn_((usize_pow(usize base, u32 exp))(usize)) {
    if (exp == 0) return 1;
    var_(result, usize) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &l0$((usize))));
        result *= base;
    }
    return result;
};
fn_((usize_shl(usize x, u32 shift))(usize)) {
    return debug_assert(shift < (sizeOf$(usize) * 8)), x << shift;
};
fn_((usize_shr(usize x, u32 shift))(usize)) {
    return debug_assert(shift < (sizeOf$(usize) * 8)), x >> shift;
};

/// Signed integer basic arithmetic implementations
fn_((i8_add(i8 lhs, i8 rhs))(i8)) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &l0$((i8)))), as$(i8)(lhs + rhs);
};
fn_((i8_sub(i8 lhs, i8 rhs))(i8)) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &l0$((i8)))), as$(i8)(lhs - rhs);
};
fn_((i8_mul(i8 lhs, i8 rhs))(i8)) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &l0$((i8)))), as$(i8)(lhs * rhs);
};
fn_((i8_div(i8 lhs, i8 rhs))(i8)) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i8_limit_min && rhs == -1)), as$(i8)(lhs / rhs);
};
fn_((i8_mod(i8 lhs, i8 rhs))(i8)) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i8_limit_min && rhs == -1)), as$(i8)(lhs % rhs);
};
fn_((i8_neg(i8 x))(i8)) {
    return debug_assert(x != i8_limit_min), as$(i8)(-x);
};
fn_((i8_abs(i8 x))(i8)) {
    return debug_assert(x != i8_limit_min), as$(i8)((x < 0) ? -x : x);
};
fn_((i8_pow(i8 base, u32 exp))(i8)) {
    if (exp == 0) return 1;
    var_(result, i8) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &l0$((i8))));
        result = as$(i8)(result * base);
    }
    return result;
};
fn_((i8_shl(i8 x, u32 shift))(i8)) {
    return debug_assert(shift < 8), debug_assert(0 <= x), as$(i8)(x << shift);
};
fn_((i8_shr(i8 x, u32 shift))(i8)) {
    return debug_assert(shift < 8), as$(i8)(x >> shift);
};
fn_((i16_add(i16 lhs, i16 rhs))(i16)) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &l0$((i16)))), as$(i16)(lhs + rhs);
};
fn_((i16_sub(i16 lhs, i16 rhs))(i16)) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &l0$((i16)))), as$(i16)(lhs - rhs);
};
fn_((i16_mul(i16 lhs, i16 rhs))(i16)) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &l0$((i16)))), as$(i16)(lhs * rhs);
};
fn_((i16_div(i16 lhs, i16 rhs))(i16)) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i16_limit_min && rhs == -1)), as$(i16)(lhs / rhs);
};
fn_((i16_mod(i16 lhs, i16 rhs))(i16)) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i16_limit_min && rhs == -1)), as$(i16)(lhs % rhs);
};
fn_((i16_neg(i16 x))(i16)) {
    return debug_assert(x != i16_limit_min), as$(i16)(-x);
};
fn_((i16_abs(i16 x))(i16)) {
    return debug_assert(x != i16_limit_min), as$(i16)((x < 0) ? -x : x);
};
fn_((i16_pow(i16 base, u32 exp))(i16)) {
    if (exp == 0) return 1;
    var_(result, i16) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &l0$((i16))));
        result = as$(i16)(result * base);
    }
    return result;
};
fn_((i16_shl(i16 x, u32 shift))(i16)) {
    return debug_assert(shift < 16), debug_assert(0 <= x), as$(i16)(x << shift);
};
fn_((i16_shr(i16 x, u32 shift))(i16)) {
    return debug_assert(shift < 16), as$(i16)(x >> shift);
};
fn_((i32_add(i32 lhs, i32 rhs))(i32)) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &l0$((i32)))), lhs + rhs;
};
fn_((i32_sub(i32 lhs, i32 rhs))(i32)) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &l0$((i32)))), lhs - rhs;
};
fn_((i32_mul(i32 lhs, i32 rhs))(i32)) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &l0$((i32)))), lhs * rhs;
};
fn_((i32_div(i32 lhs, i32 rhs))(i32)) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i32_limit_min && rhs == -1)), lhs / rhs;
};
fn_((i32_mod(i32 lhs, i32 rhs))(i32)) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i32_limit_min && rhs == -1)), lhs % rhs;
};
fn_((i32_neg(i32 x))(i32)) {
    return debug_assert(x != i32_limit_min), -x;
};
fn_((i32_abs(i32 x))(i32)) {
    return debug_assert(x != i32_limit_min), (x < 0) ? -x : x;
};
fn_((i32_pow(i32 base, u32 exp))(i32)) {
    if (exp == 0) return 1;
    var_(result, i32) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &l0$((i32))));
        result *= base;
    }
    return result;
};
fn_((i32_shl(i32 x, u32 shift))(i32)) {
    return debug_assert(shift < 32), debug_assert(0 <= x), x << shift;
};
fn_((i32_shr(i32 x, u32 shift))(i32)) {
    return debug_assert(shift < 32), x >> shift;
};
fn_((i64_add(i64 lhs, i64 rhs))(i64)) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &l0$((i64)))), lhs + rhs;
};
fn_((i64_sub(i64 lhs, i64 rhs))(i64)) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &l0$((i64)))), lhs - rhs;
};
fn_((i64_mul(i64 lhs, i64 rhs))(i64)) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &l0$((i64)))), lhs * rhs;
};
fn_((i64_div(i64 lhs, i64 rhs))(i64)) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i64_limit_min && rhs == -1)), lhs / rhs;
};
fn_((i64_mod(i64 lhs, i64 rhs))(i64)) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == i64_limit_min && rhs == -1)), lhs % rhs;
};
fn_((i64_neg(i64 x))(i64)) {
    return debug_assert(x != i64_limit_min), -x;
};
fn_((i64_abs(i64 x))(i64)) {
    return debug_assert(x != i64_limit_min), (x < 0) ? -x : x;
};
fn_((i64_pow(i64 base, u32 exp))(i64)) {
    if (exp == 0) return 1;
    var_(result, i64) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &l0$((i64))));
        result *= base;
    }
    return result;
};
fn_((i64_shl(i64 x, u32 shift))(i64)) {
    return debug_assert(shift < 64), debug_assert(0 <= x), x << shift;
};
fn_((i64_shr(i64 x, u32 shift))(i64)) {
    return debug_assert(shift < 64), x >> shift;
};
fn_((isize_add(isize lhs, isize rhs))(isize)) {
    return debug_assert(!__builtin_add_overflow(lhs, rhs, &l0$((isize)))), lhs + rhs;
};
fn_((isize_sub(isize lhs, isize rhs))(isize)) {
    return debug_assert(!__builtin_sub_overflow(lhs, rhs, &l0$((isize)))), lhs - rhs;
};
fn_((isize_mul(isize lhs, isize rhs))(isize)) {
    return debug_assert(!__builtin_mul_overflow(lhs, rhs, &l0$((isize)))), lhs * rhs;
};
fn_((isize_div(isize lhs, isize rhs))(isize)) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == isize_limit_min && rhs == -1)), lhs / rhs;
};
fn_((isize_mod(isize lhs, isize rhs))(isize)) {
    return debug_assert(rhs != 0), debug_assert(!(lhs == isize_limit_min && rhs == -1)), lhs % rhs;
};
fn_((isize_neg(isize x))(isize)) {
    return debug_assert(x != isize_limit_min), -x;
};
fn_((isize_abs(isize x))(isize)) {
    return debug_assert(x != isize_limit_min), (x < 0) ? -x : x;
};
fn_((isize_pow(isize base, u32 exp))(isize)) {
    if (exp == 0) return 1;
    var_(result, isize) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        debug_assert(!__builtin_mul_overflow(result, base, &l0$((isize))));
        result *= base;
    }
    return result;
};
fn_((isize_shl(isize x, u32 shift))(isize)) {
    return debug_assert(shift < (sizeOf$(isize) * 8)), debug_assert(0 <= x), x << shift;
};
fn_((isize_shr(isize x, u32 shift))(isize)) {
    return debug_assert(shift < (sizeOf$(isize) * 8)), x >> shift;
};

/*========== Checked Arithmetic Implementations =============================*/

/// Unsigned integer overflow checking implementations
fn_((u8_addChkd(u8 lhs, u8 rhs))(O$u8)) {
    var_(result, u8) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return none$((O$u8));
    return some$((O$u8)(result));
};
fn_((u8_subChkd(u8 lhs, u8 rhs))(O$u8)) {
    var_(result, u8) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return none$((O$u8));
    return some$((O$u8)(result));
};
fn_((u8_mulChkd(u8 lhs, u8 rhs))(O$u8)) {
    var_(result, u8) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return none$((O$u8));
    return some$((O$u8)(result));
};
fn_((u8_divChkd(u8 lhs, u8 rhs))(O$u8)) {
    if (rhs == 0) return none$((O$u8));
    return some$((O$u8)(lhs / rhs));
};
fn_((u8_modChkd(u8 lhs, u8 rhs))(O$u8)) {
    if (rhs == 0) return none$((O$u8));
    return some$((O$u8)(lhs % rhs));
};
fn_((u8_powChkd(u8 base, u32 exp))(O$u8)) {
    if (exp == 0) return some$((O$u8)(1));
    var_(result, u8) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return none$((O$u8));
    }
    return some$((O$u8)(result));
};
fn_((u8_shlChkd(u8 x, u32 shift))(O$u8)) {
    if (shift >= 8) return none$((O$u8));
    if (shift == 0) return some$((O$u8)(x));
    if (x > (u8_limit_max >> shift)) return none$((O$u8));
    return some$((O$u8)(as$(u8)(x << shift)));
};
fn_((u8_shrChkd(u8 x, u32 shift))(O$u8)) {
    if (shift >= 8) return none$((O$u8));
    return some$((O$u8)(as$(u8)(x >> shift)));
};
fn_((u16_addChkd(u16 lhs, u16 rhs))(O$u16)) {
    var_(result, u16) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return none$((O$u16));
    return some$((O$u16)(result));
};
fn_((u16_subChkd(u16 lhs, u16 rhs))(O$u16)) {
    var_(result, u16) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return none$((O$u16));
    return some$((O$u16)(result));
};
fn_((u16_mulChkd(u16 lhs, u16 rhs))(O$u16)) {
    var_(result, u16) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return none$((O$u16));
    return some$((O$u16)(result));
};
fn_((u16_divChkd(u16 lhs, u16 rhs))(O$u16)) {
    if (rhs == 0) return none$((O$u16));
    return some$((O$u16)(lhs / rhs));
};
fn_((u16_modChkd(u16 lhs, u16 rhs))(O$u16)) {
    if (rhs == 0) return none$((O$u16));
    return some$((O$u16)(lhs % rhs));
};
fn_((u16_powChkd(u16 base, u32 exp))(O$u16)) {
    if (exp == 0) return some$((O$u16)(1));
    var_(result, u16) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return none$((O$u16));
    }
    return some$((O$u16)(result));
};
fn_((u16_shlChkd(u16 x, u32 shift))(O$u16)) {
    if (shift >= 16) return none$((O$u16));
    if (shift == 0) return some$((O$u16)(x));
    if (x > (u16_limit_max >> shift)) return none$((O$u16));
    return some$((O$u16)(as$(u16)(x << shift)));
};
fn_((u16_shrChkd(u16 x, u32 shift))(O$u16)) {
    if (shift >= 16) return none$((O$u16));
    return some$((O$u16)(as$(u16)(x >> shift)));
};
fn_((u32_addChkd(u32 lhs, u32 rhs))(O$u32)) {
    var_(result, u32) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return none$((O$u32));
    return some$((O$u32)(result));
};
fn_((u32_subChkd(u32 lhs, u32 rhs))(O$u32)) {
    var_(result, u32) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return none$((O$u32));
    return some$((O$u32)(result));
};
fn_((u32_mulChkd(u32 lhs, u32 rhs))(O$u32)) {
    var_(result, u32) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return none$((O$u32));
    return some$((O$u32)(result));
};
fn_((u32_divChkd(u32 lhs, u32 rhs))(O$u32)) {
    if (rhs == 0) return none$((O$u32));
    return some$((O$u32)(lhs / rhs));
};
fn_((u32_modChkd(u32 lhs, u32 rhs))(O$u32)) {
    if (rhs == 0) return none$((O$u32));
    return some$((O$u32)(lhs % rhs));
};
fn_((u32_powChkd(u32 base, u32 exp))(O$u32)) {
    if (exp == 0) return some$((O$u32)(1));
    var_(result, u32) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return none$((O$u32));
    }
    return some$((O$u32)(result));
};
fn_((u32_shlChkd(u32 x, u32 shift))(O$u32)) {
    if (shift >= 32) return none$((O$u32));
    if (shift == 0) return some$((O$u32)(x));
    if (x > (u32_limit_max >> shift)) return none$((O$u32));
    return some$((O$u32)(x << shift));
};
fn_((u32_shrChkd(u32 x, u32 shift))(O$u32)) {
    if (shift >= 32) return none$((O$u32));
    return some$((O$u32)(x >> shift));
};
fn_((u64_addChkd(u64 lhs, u64 rhs))(O$u64)) {
    var_(result, u64) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return none$((O$u64));
    return some$((O$u64)(result));
};
fn_((u64_subChkd(u64 lhs, u64 rhs))(O$u64)) {
    var_(result, u64) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return none$((O$u64));
    return some$((O$u64)(result));
};
fn_((u64_mulChkd(u64 lhs, u64 rhs))(O$u64)) {
    var_(result, u64) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return none$((O$u64));
    return some$((O$u64)(result));
};
fn_((u64_divChkd(u64 lhs, u64 rhs))(O$u64)) {
    if (rhs == 0) return none$((O$u64));
    return some$((O$u64)(lhs / rhs));
};
fn_((u64_modChkd(u64 lhs, u64 rhs))(O$u64)) {
    if (rhs == 0) return none$((O$u64));
    return some$((O$u64)(lhs % rhs));
};
fn_((u64_powChkd(u64 base, u32 exp))(O$u64)) {
    if (exp == 0) return some$((O$u64)(1));
    var_(result, u64) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return none$((O$u64));
    }
    return some$((O$u64)(result));
};
fn_((u64_shlChkd(u64 x, u32 shift))(O$u64)) {
    if (shift >= 64) return none$((O$u64));
    if (shift == 0) return some$((O$u64)(x));
    if (x > (u64_limit_max >> shift)) return none$((O$u64));
    return some$((O$u64)(x << shift));
};
fn_((u64_shrChkd(u64 x, u32 shift))(O$u64)) {
    if (shift >= 64) return none$((O$u64));
    return some$((O$u64)(x >> shift));
};
fn_((usize_addChkd(usize lhs, usize rhs))(O$usize)) {
    var_(result, usize) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return none$((O$usize));
    return some$((O$usize)(result));
};
fn_((usize_subChkd(usize lhs, usize rhs))(O$usize)) {
    var_(result, usize) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return none$((O$usize));
    return some$((O$usize)(result));
};
fn_((usize_mulChkd(usize lhs, usize rhs))(O$usize)) {
    var_(result, usize) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return none$((O$usize));
    return some$((O$usize)(result));
};
fn_((usize_divChkd(usize lhs, usize rhs))(O$usize)) {
    if (rhs == 0) return none$((O$usize));
    return some$((O$usize)(lhs / rhs));
};
fn_((usize_modChkd(usize lhs, usize rhs))(O$usize)) {
    if (rhs == 0) return none$((O$usize));
    return some$((O$usize)(lhs % rhs));
};
fn_((usize_powChkd(usize base, u32 exp))(O$usize)) {
    if (exp == 0) return some$((O$usize)(1));
    var_(result, usize) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return none$((O$usize));
    }
    return some$((O$usize)(result));
};
fn_((usize_shlChkd(usize x, u32 shift))(O$usize)) {
    if (shift >= (sizeOf$(usize) * 8)) return none$((O$usize));
    if (shift == 0) return some$((O$usize)(x));
    if (x > (usize_limit_max >> shift)) return none$((O$usize));
    return some$((O$usize)(x << shift));
};
fn_((usize_shrChkd(usize x, u32 shift))(O$usize)) {
    if (shift >= (sizeOf$(usize) * 8)) return none$((O$usize));
    return some$((O$usize)(x >> shift));
};

/// Signed integer overflow checking implementations
fn_((i8_addChkd(i8 lhs, i8 rhs))(O$i8)) {
    var_(result, i8) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return none$((O$i8));
    return some$((O$i8)(result));
};
fn_((i8_subChkd(i8 lhs, i8 rhs))(O$i8)) {
    var_(result, i8) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return none$((O$i8));
    return some$((O$i8)(result));
};
fn_((i8_mulChkd(i8 lhs, i8 rhs))(O$i8)) {
    var_(result, i8) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return none$((O$i8));
    return some$((O$i8)(result));
};
fn_((i8_divChkd(i8 lhs, i8 rhs))(O$i8)) {
    if (rhs == 0 || (lhs == i8_limit_min && rhs == -1)) return none$((O$i8));
    return some$((O$i8)(as$(i8)(lhs / rhs)));
};
fn_((i8_modChkd(i8 lhs, i8 rhs))(O$i8)) {
    if (rhs == 0 || (lhs == i8_limit_min && rhs == -1)) return none$((O$i8));
    return some$((O$i8)(as$(i8)(lhs % rhs)));
};
fn_((i8_negChkd(i8 x))(O$i8)) {
    if (x == i8_limit_min) return none$((O$i8));
    return some$((O$i8)(as$(i8)(-x)));
};
fn_((i8_absChkd(i8 x))(O$i8)) {
    if (x == i8_limit_min) return none$((O$i8));
    return some$((O$i8)(as$(i8)((x < 0) ? -x : x)));
};
fn_((i8_powChkd(i8 base, u32 exp))(O$i8)) {
    if (exp == 0) return some$((O$i8)(1));
    var_(result, i8) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return none$((O$i8));
    }
    return some$((O$i8)(result));
};
fn_((i8_shlChkd(i8 x, u32 shift))(O$i8)) {
    if (shift >= 8) return none$((O$i8));
    if (shift == 0) return some$((O$i8)(x));
    if (x < 0) return none$((O$i8));
    if (x > (i8_limit_max >> shift)) return none$((O$i8));
    return some$((O$i8)(as$(i8)(x << shift)));
};
fn_((i8_shrChkd(i8 x, u32 shift))(O$i8)) {
    if (shift >= 8) return none$((O$i8));
    return some$((O$i8)(as$(i8)(x >> shift)));
};
fn_((i16_addChkd(i16 lhs, i16 rhs))(O$i16)) {
    var_(result, i16) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return none$((O$i16));
    return some$((O$i16)(result));
};
fn_((i16_subChkd(i16 lhs, i16 rhs))(O$i16)) {
    var_(result, i16) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return none$((O$i16));
    return some$((O$i16)(result));
};
fn_((i16_mulChkd(i16 lhs, i16 rhs))(O$i16)) {
    var_(result, i16) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return none$((O$i16));
    return some$((O$i16)(result));
};
fn_((i16_divChkd(i16 lhs, i16 rhs))(O$i16)) {
    if (rhs == 0 || (lhs == i16_limit_min && rhs == -1)) return none$((O$i16));
    return some$((O$i16)(as$(i16)(lhs / rhs)));
};
fn_((i16_modChkd(i16 lhs, i16 rhs))(O$i16)) {
    if (rhs == 0 || (lhs == i16_limit_min && rhs == -1)) return none$((O$i16));
    return some$((O$i16)(as$(i16)(lhs % rhs)));
};
fn_((i16_negChkd(i16 x))(O$i16)) {
    if (x == i16_limit_min) return none$((O$i16));
    return some$((O$i16)(as$(i16)(-x)));
};
fn_((i16_absChkd(i16 x))(O$i16)) {
    if (x == i16_limit_min) return none$((O$i16));
    return some$((O$i16)(as$(i16)((x < 0) ? -x : x)));
};
fn_((i16_powChkd(i16 base, u32 exp))(O$i16)) {
    if (exp == 0) return some$((O$i16)(1));
    var_(result, i16) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return none$((O$i16));
    }
    return some$((O$i16)(result));
};
fn_((i16_shlChkd(i16 x, u32 shift))(O$i16)) {
    if (shift >= 16) return none$((O$i16));
    if (shift == 0) return some$((O$i16)(x));
    if (x < 0) return none$((O$i16));
    if (x > (i16_limit_max >> shift)) return none$((O$i16));
    return some$((O$i16)(as$(i16)(x << shift)));
};
fn_((i16_shrChkd(i16 x, u32 shift))(O$i16)) {
    if (shift >= 16) return none$((O$i16));
    return some$((O$i16)(as$(i16)(x >> shift)));
};
fn_((i32_addChkd(i32 lhs, i32 rhs))(O$i32)) {
    var_(result, i32) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return none$((O$i32));
    return some$((O$i32)(result));
};
fn_((i32_subChkd(i32 lhs, i32 rhs))(O$i32)) {
    var_(result, i32) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return none$((O$i32));
    return some$((O$i32)(result));
};
fn_((i32_mulChkd(i32 lhs, i32 rhs))(O$i32)) {
    var_(result, i32) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return none$((O$i32));
    return some$((O$i32)(result));
};
fn_((i32_divChkd(i32 lhs, i32 rhs))(O$i32)) {
    if (rhs == 0 || (lhs == i32_limit_min && rhs == -1)) return none$((O$i32));
    return some$((O$i32)(lhs / rhs));
};
fn_((i32_modChkd(i32 lhs, i32 rhs))(O$i32)) {
    if (rhs == 0 || (lhs == i32_limit_min && rhs == -1)) return none$((O$i32));
    return some$((O$i32)(lhs % rhs));
};
fn_((i32_negChkd(i32 x))(O$i32)) {
    if (x == i32_limit_min) return none$((O$i32));
    return some$((O$i32)(-x));
};
fn_((i32_absChkd(i32 x))(O$i32)) {
    if (x == i32_limit_min) return none$((O$i32));
    return some$((O$i32)((x < 0) ? -x : x));
};
fn_((i32_powChkd(i32 base, u32 exp))(O$i32)) {
    if (exp == 0) return some$((O$i32)(1));
    var_(result, i32) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return none$((O$i32));
    }
    return some$((O$i32)(result));
};
fn_((i32_shlChkd(i32 x, u32 shift))(O$i32)) {
    if (shift >= 32) return none$((O$i32));
    if (shift == 0) return some$((O$i32)(x));
    if (x < 0) return none$((O$i32));
    if (x > (i32_limit_max >> shift)) return none$((O$i32));
    return some$((O$i32)(x << shift));
};
fn_((i32_shrChkd(i32 x, u32 shift))(O$i32)) {
    if (shift >= 32) return none$((O$i32));
    return some$((O$i32)(x >> shift));
};
fn_((i64_addChkd(i64 lhs, i64 rhs))(O$i64)) {
    var_(result, i64) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return none$((O$i64));
    return some$((O$i64)(result));
};
fn_((i64_subChkd(i64 lhs, i64 rhs))(O$i64)) {
    var_(result, i64) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return none$((O$i64));
    return some$((O$i64)(result));
};
fn_((i64_mulChkd(i64 lhs, i64 rhs))(O$i64)) {
    var_(result, i64) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return none$((O$i64));
    return some$((O$i64)(result));
};
fn_((i64_divChkd(i64 lhs, i64 rhs))(O$i64)) {
    if (rhs == 0 || (lhs == i64_limit_min && rhs == -1)) return none$((O$i64));
    return some$((O$i64)(lhs / rhs));
};
fn_((i64_modChkd(i64 lhs, i64 rhs))(O$i64)) {
    if (rhs == 0 || (lhs == i64_limit_min && rhs == -1)) return none$((O$i64));
    return some$((O$i64)(lhs % rhs));
};
fn_((i64_negChkd(i64 x))(O$i64)) {
    if (x == i64_limit_min) return none$((O$i64));
    return some$((O$i64)(-x));
};
fn_((i64_absChkd(i64 x))(O$i64)) {
    if (x == i64_limit_min) return none$((O$i64));
    return some$((O$i64)((x < 0) ? -x : x));
};
fn_((i64_powChkd(i64 base, u32 exp))(O$i64)) {
    if (exp == 0) return some$((O$i64)(1));
    var_(result, i64) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return none$((O$i64));
    }
    return some$((O$i64)(result));
};
fn_((i64_shlChkd(i64 x, u32 shift))(O$i64)) {
    if (shift >= 64) return none$((O$i64));
    if (shift == 0) return some$((O$i64)(x));
    if (x < 0) return none$((O$i64));
    if (x > (i64_limit_max >> shift)) return none$((O$i64));
    return some$((O$i64)(x << shift));
};
fn_((i64_shrChkd(i64 x, u32 shift))(O$i64)) {
    if (shift >= 64) return none$((O$i64));
    return some$((O$i64)(x >> shift));
};
fn_((isize_addChkd(isize lhs, isize rhs))(O$isize)) {
    var_(result, isize) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return none$((O$isize));
    return some$((O$isize)(result));
};
fn_((isize_subChkd(isize lhs, isize rhs))(O$isize)) {
    var_(result, isize) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return none$((O$isize));
    return some$((O$isize)(result));
};
fn_((isize_mulChkd(isize lhs, isize rhs))(O$isize)) {
    var_(result, isize) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return none$((O$isize));
    return some$((O$isize)(result));
};
fn_((isize_divChkd(isize lhs, isize rhs))(O$isize)) {
    if (rhs == 0 || (lhs == isize_limit_min && rhs == -1)) return none$((O$isize));
    return some$((O$isize)(lhs / rhs));
};
fn_((isize_modChkd(isize lhs, isize rhs))(O$isize)) {
    if (rhs == 0 || (lhs == isize_limit_min && rhs == -1)) return none$((O$isize));
    return some$((O$isize)(lhs % rhs));
};
fn_((isize_negChkd(isize x))(O$isize)) {
    if (x == isize_limit_min) return none$((O$isize));
    return some$((O$isize)(-x));
};
fn_((isize_absChkd(isize x))(O$isize)) {
    if (x == isize_limit_min) return none$((O$isize));
    return some$((O$isize)((x < 0) ? -x : x));
};
fn_((isize_powChkd(isize base, u32 exp))(O$isize)) {
    if (exp == 0) return some$((O$isize)(1));
    var_(result, isize) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return none$((O$isize));
    }
    return some$((O$isize)(result));
};
fn_((isize_shlChkd(isize x, u32 shift))(O$isize)) {
    if (shift >= (sizeOf$(isize) * 8)) return none$((O$isize));
    if (shift == 0) return some$((O$isize)(x));
    if (x < 0) return none$((O$isize));
    if (x > (isize_limit_max >> shift)) return none$((O$isize));
    return some$((O$isize)(x << shift));
};
fn_((isize_shrChkd(isize x, u32 shift))(O$isize)) {
    if (shift >= (sizeOf$(isize) * 8)) return none$((O$isize));
    return some$((O$isize)(x >> shift));
};

/*========== Wrapping Arithmetic Implementations ============================*/

/// Unsigned integer wrapping implementations
fn_((u8_addWrap(u8 lhs, u8 rhs))(u8)) { return as$(u8)(lhs + rhs); };
fn_((u8_subWrap(u8 lhs, u8 rhs))(u8)) { return as$(u8)(lhs - rhs); };
fn_((u8_mulWrap(u8 lhs, u8 rhs))(u8)) { return as$(u8)(lhs * rhs); };
fn_((u8_shlWrap(u8 x, u32 shift))(u8)) { return as$(u8)(x << (shift & 7)); };
fn_((u8_shrWrap(u8 x, u32 shift))(u8)) { return as$(u8)(x >> (shift & 7)); };

fn_((u16_addWrap(u16 lhs, u16 rhs))(u16)) { return as$(u16)(lhs + rhs); };
fn_((u16_subWrap(u16 lhs, u16 rhs))(u16)) { return as$(u16)(lhs - rhs); };
fn_((u16_mulWrap(u16 lhs, u16 rhs))(u16)) { return as$(u16)(lhs * rhs); };
fn_((u16_shlWrap(u16 x, u32 shift))(u16)) { return as$(u16)(x << (shift & 15)); };
fn_((u16_shrWrap(u16 x, u32 shift))(u16)) { return as$(u16)(x >> (shift & 15)); };

fn_((u32_addWrap(u32 lhs, u32 rhs))(u32)) { return lhs + rhs; };
fn_((u32_subWrap(u32 lhs, u32 rhs))(u32)) { return lhs - rhs; };
fn_((u32_mulWrap(u32 lhs, u32 rhs))(u32)) { return lhs * rhs; };
fn_((u32_shlWrap(u32 x, u32 shift))(u32)) { return x << (shift & 31); };
fn_((u32_shrWrap(u32 x, u32 shift))(u32)) { return x >> (shift & 31); };

fn_((u64_addWrap(u64 lhs, u64 rhs))(u64)) { return lhs + rhs; };
fn_((u64_subWrap(u64 lhs, u64 rhs))(u64)) { return lhs - rhs; };
fn_((u64_mulWrap(u64 lhs, u64 rhs))(u64)) { return lhs * rhs; };
fn_((u64_shlWrap(u64 x, u32 shift))(u64)) { return x << (shift & 63); };
fn_((u64_shrWrap(u64 x, u32 shift))(u64)) { return x >> (shift & 63); };

fn_((usize_addWrap(usize lhs, usize rhs))(usize)) { return lhs + rhs; };
fn_((usize_subWrap(usize lhs, usize rhs))(usize)) { return lhs - rhs; };
fn_((usize_mulWrap(usize lhs, usize rhs))(usize)) { return lhs * rhs; };
fn_((usize_shlWrap(usize x, u32 shift))(usize)) { return x << (shift & ((sizeOf$(usize) * 8) - 1)); };
fn_((usize_shrWrap(usize x, u32 shift))(usize)) { return x >> (shift & ((sizeOf$(usize) * 8) - 1)); };

/// Signed integer wrapping implementations
fn_((i8_addWrap(i8 lhs, i8 rhs))(i8)) { return as$(i8)(lhs + rhs); };
fn_((i8_subWrap(i8 lhs, i8 rhs))(i8)) { return as$(i8)(lhs - rhs); };
fn_((i8_mulWrap(i8 lhs, i8 rhs))(i8)) { return as$(i8)(lhs * rhs); };
fn_((i8_negWrap(i8 x))(i8)) { return as$(i8)(-x); };
fn_((i8_shlWrap(i8 x, u32 shift))(i8)) { return as$(i8)(x << (shift & 7)); };
fn_((i8_shrWrap(i8 x, u32 shift))(i8)) { return as$(i8)(x >> (shift & 7)); };

fn_((i16_addWrap(i16 lhs, i16 rhs))(i16)) { return as$(i16)(lhs + rhs); };
fn_((i16_subWrap(i16 lhs, i16 rhs))(i16)) { return as$(i16)(lhs - rhs); };
fn_((i16_mulWrap(i16 lhs, i16 rhs))(i16)) { return as$(i16)(lhs * rhs); };
fn_((i16_negWrap(i16 x))(i16)) { return as$(i16)(-x); };
fn_((i16_shlWrap(i16 x, u32 shift))(i16)) { return as$(i16)(x << (shift & 15)); };
fn_((i16_shrWrap(i16 x, u32 shift))(i16)) { return as$(i16)(x >> (shift & 15)); };

fn_((i32_addWrap(i32 lhs, i32 rhs))(i32)) { return lhs + rhs; };
fn_((i32_subWrap(i32 lhs, i32 rhs))(i32)) { return lhs - rhs; };
fn_((i32_mulWrap(i32 lhs, i32 rhs))(i32)) { return lhs * rhs; };
fn_((i32_negWrap(i32 x))(i32)) { return -x; };
fn_((i32_shlWrap(i32 x, u32 shift))(i32)) { return x << (shift & 31); };
fn_((i32_shrWrap(i32 x, u32 shift))(i32)) { return x >> (shift & 31); };

fn_((i64_addWrap(i64 lhs, i64 rhs))(i64)) { return lhs + rhs; };
fn_((i64_subWrap(i64 lhs, i64 rhs))(i64)) { return lhs - rhs; };
fn_((i64_mulWrap(i64 lhs, i64 rhs))(i64)) { return lhs * rhs; };
fn_((i64_negWrap(i64 x))(i64)) { return -x; };
fn_((i64_shlWrap(i64 x, u32 shift))(i64)) { return x << (shift & 63); };
fn_((i64_shrWrap(i64 x, u32 shift))(i64)) { return x >> (shift & 63); };

fn_((isize_addWrap(isize lhs, isize rhs))(isize)) { return lhs + rhs; };
fn_((isize_subWrap(isize lhs, isize rhs))(isize)) { return lhs - rhs; };
fn_((isize_mulWrap(isize lhs, isize rhs))(isize)) { return lhs * rhs; };
fn_((isize_negWrap(isize x))(isize)) { return -x; };
fn_((isize_shlWrap(isize x, u32 shift))(isize)) { return x << (shift & ((sizeOf$(isize) * 8) - 1)); };
fn_((isize_shrWrap(isize x, u32 shift))(isize)) { return x >> (shift & ((sizeOf$(isize) * 8) - 1)); };

/*========== Saturating Arithmetic Implementations ==========================*/

/// Unsigned integer saturating implementations
fn_((u8_addSat(u8 lhs, u8 rhs))(u8)) {
    var_(result, u8) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return u8_limit_max;
    return result;
};
fn_((u8_subSat(u8 lhs, u8 rhs))(u8)) {
    var_(result, u8) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return u8_limit_min;
    return result;
};
fn_((u8_mulSat(u8 lhs, u8 rhs))(u8)) {
    var_(result, u8) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return u8_limit_max;
    return result;
};
fn_((u8_powSat(u8 base, u32 exp))(u8)) {
    if (exp == 0) return 1;
    var_(result, u8) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return u8_limit_max;
    }
    return result;
};
fn_((u16_addSat(u16 lhs, u16 rhs))(u16)) {
    var_(result, u16) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return u16_limit_max;
    return result;
};
fn_((u16_subSat(u16 lhs, u16 rhs))(u16)) {
    var_(result, u16) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return u16_limit_min;
    return result;
};
fn_((u16_mulSat(u16 lhs, u16 rhs))(u16)) {
    var_(result, u16) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return u16_limit_max;
    return result;
};
fn_((u16_powSat(u16 base, u32 exp))(u16)) {
    if (exp == 0) return 1;
    var_(result, u16) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return u16_limit_max;
    }
    return result;
};
fn_((u32_addSat(u32 lhs, u32 rhs))(u32)) {
    var_(result, u32) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return u32_limit_max;
    return result;
};
fn_((u32_subSat(u32 lhs, u32 rhs))(u32)) {
    var_(result, u32) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return u32_limit_min;
    return result;
};
fn_((u32_mulSat(u32 lhs, u32 rhs))(u32)) {
    var_(result, u32) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return u32_limit_max;
    return result;
};
fn_((u32_powSat(u32 base, u32 exp))(u32)) {
    if (exp == 0) return 1;
    var_(result, u32) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return u32_limit_max;
    }
    return result;
};
fn_((u64_addSat(u64 lhs, u64 rhs))(u64)) {
    var_(result, u64) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return u64_limit_max;
    return result;
};
fn_((u64_subSat(u64 lhs, u64 rhs))(u64)) {
    var_(result, u64) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return u64_limit_min;
    return result;
};
fn_((u64_mulSat(u64 lhs, u64 rhs))(u64)) {
    var_(result, u64) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return u64_limit_max;
    return result;
};
fn_((u64_powSat(u64 base, u32 exp))(u64)) {
    if (exp == 0) return 1;
    var_(result, u64) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return u64_limit_max;
    }
    return result;
};
fn_((usize_addSat(usize lhs, usize rhs))(usize)) {
    var_(result, usize) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return usize_limit_max;
    return result;
};
fn_((usize_subSat(usize lhs, usize rhs))(usize)) {
    var_(result, usize) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return usize_limit_min;
    return result;
};
fn_((usize_mulSat(usize lhs, usize rhs))(usize)) {
    var_(result, usize) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) return usize_limit_max;
    return result;
};
fn_((usize_powSat(usize base, u32 exp))(usize)) {
    if (exp == 0) return 1;
    var_(result, usize) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        if (__builtin_mul_overflow(result, base, &result)) return usize_limit_max;
    }
    return result;
};

/// Signed integer saturating implementations
fn_((i8_addSat(i8 lhs, i8 rhs))(i8)) {
    var_(result, i8) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return (0 < lhs) ? i8_limit_max : i8_limit_min;
    return result;
};
fn_((i8_subSat(i8 lhs, i8 rhs))(i8)) {
    var_(result, i8) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return (0 < lhs) ? i8_limit_max : i8_limit_min;
    return result;
};
fn_((i8_mulSat(i8 lhs, i8 rhs))(i8)) {
    var_(result, i8) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) {
        let_(same_sign, bool) = (0 < lhs && 0 < rhs) || (lhs < 0 && rhs < 0);
        return same_sign ? i8_limit_max : i8_limit_min;
    }
    return result;
};
fn_((i8_divSat(i8 lhs, i8 rhs))(i8)) {
    if (rhs == 0) return (0 <= lhs) ? i8_limit_max : i8_limit_min;
    if (lhs == i8_limit_min && rhs == -1) return i8_limit_max;
    return as$(i8)(lhs / rhs);
};
fn_((i8_negSat(i8 x))(i8)) {
    if (x == i8_limit_min) return i8_limit_max;
    return as$(i8)(-x);
};
fn_((i8_absSat(i8 x))(i8)) {
    if (x == i8_limit_min) return i8_limit_max;
    return as$(i8)((x < 0) ? -x : x);
};
fn_((i8_powSat(i8 base, u32 exp))(i8)) {
    if (exp == 0) return 1;
    var_(result, i8) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        let_(old_result, i8) = result;
        if (__builtin_mul_overflow(result, base, &result)) {
            let_(same_sign, bool) = (0 < old_result && 0 < base) || (old_result < 0 && base < 0);
            return same_sign ? i8_limit_max : i8_limit_min;
        }
    }
    return result;
};
fn_((i16_addSat(i16 lhs, i16 rhs))(i16)) {
    var_(result, i16) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return (0 < lhs) ? i16_limit_max : i16_limit_min;
    return result;
};
fn_((i16_subSat(i16 lhs, i16 rhs))(i16)) {
    var_(result, i16) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return (0 < lhs) ? i16_limit_max : i16_limit_min;
    return result;
};
fn_((i16_mulSat(i16 lhs, i16 rhs))(i16)) {
    var_(result, i16) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) {
        let_(same_sign, bool) = (0 < lhs && 0 < rhs) || (lhs < 0 && rhs < 0);
        return same_sign ? i16_limit_max : i16_limit_min;
    }
    return result;
};
fn_((i16_divSat(i16 lhs, i16 rhs))(i16)) {
    if (rhs == 0) return (0 <= lhs) ? i16_limit_max : i16_limit_min;
    if (lhs == i16_limit_min && rhs == -1) return i16_limit_max;
    return as$(i16)(lhs / rhs);
};
fn_((i16_negSat(i16 x))(i16)) {
    if (x == i16_limit_min) return i16_limit_max;
    return as$(i16)(-x);
};
fn_((i16_absSat(i16 x))(i16)) {
    if (x == i16_limit_min) return i16_limit_max;
    return as$(i16)((x < 0) ? -x : x);
};
fn_((i16_powSat(i16 base, u32 exp))(i16)) {
    if (exp == 0) return 1;
    var_(result, i16) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        let_(old_result, i16) = result;
        if (__builtin_mul_overflow(result, base, &result)) {
            let_(same_sign, bool) = (0 < old_result && 0 < base) || (old_result < 0 && base < 0);
            return same_sign ? i16_limit_max : i16_limit_min;
        }
    }
    return result;
};
fn_((i32_addSat(i32 lhs, i32 rhs))(i32)) {
    var_(result, i32) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return (0 < lhs) ? i32_limit_max : i32_limit_min;
    return result;
};
fn_((i32_subSat(i32 lhs, i32 rhs))(i32)) {
    var_(result, i32) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return (0 < lhs) ? i32_limit_max : i32_limit_min;
    return result;
};
fn_((i32_mulSat(i32 lhs, i32 rhs))(i32)) {
    var_(result, i32) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) {
        let_(same_sign, bool) = (0 < lhs && 0 < rhs) || (lhs < 0 && rhs < 0);
        return same_sign ? i32_limit_max : i32_limit_min;
    }
    return result;
};
fn_((i32_divSat(i32 lhs, i32 rhs))(i32)) {
    if (rhs == 0) return (0 <= lhs) ? i32_limit_max : i32_limit_min;
    if (lhs == i32_limit_min && rhs == -1) return i32_limit_max;
    return lhs / rhs;
};
fn_((i32_negSat(i32 x))(i32)) {
    if (x == i32_limit_min) return i32_limit_max;
    return -x;
};
fn_((i32_absSat(i32 x))(i32)) {
    if (x == i32_limit_min) return i32_limit_max;
    return (x < 0) ? -x : x;
};
fn_((i32_powSat(i32 base, u32 exp))(i32)) {
    if (exp == 0) return 1;
    var_(result, i32) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        let_(old_result, i32) = result;
        if (__builtin_mul_overflow(result, base, &result)) {
            let_(same_sign, bool) = (0 < old_result && 0 < base) || (old_result < 0 && base < 0);
            return same_sign ? i32_limit_max : i32_limit_min;
        }
    }
    return result;
};
fn_((i64_addSat(i64 lhs, i64 rhs))(i64)) {
    var_(result, i64) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return (0 < lhs) ? i64_limit_max : i64_limit_min;
    return result;
};
fn_((i64_subSat(i64 lhs, i64 rhs))(i64)) {
    var_(result, i64) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return (0 < lhs) ? i64_limit_max : i64_limit_min;
    return result;
};
fn_((i64_mulSat(i64 lhs, i64 rhs))(i64)) {
    var_(result, i64) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) {
        let_(same_sign, bool) = (0 < lhs && 0 < rhs) || (lhs < 0 && rhs < 0);
        return same_sign ? i64_limit_max : i64_limit_min;
    }
    return result;
};
fn_((i64_divSat(i64 lhs, i64 rhs))(i64)) {
    if (rhs == 0) return (0 <= lhs) ? i64_limit_max : i64_limit_min;
    if (lhs == i64_limit_min && rhs == -1) return i64_limit_max;
    return lhs / rhs;
};
fn_((i64_negSat(i64 x))(i64)) {
    if (x == i64_limit_min) return i64_limit_max;
    return -x;
};
fn_((i64_absSat(i64 x))(i64)) {
    if (x == i64_limit_min) return i64_limit_max;
    return (x < 0) ? -x : x;
};
fn_((i64_powSat(i64 base, u32 exp))(i64)) {
    if (exp == 0) return 1;
    var_(result, i64) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        let_(old_result, i64) = result;
        if (__builtin_mul_overflow(result, base, &result)) {
            let_(same_sign, bool) = (0 < old_result && 0 < base) || (old_result < 0 && base < 0);
            return same_sign ? i64_limit_max : i64_limit_min;
        }
    }
    return result;
};
fn_((isize_addSat(isize lhs, isize rhs))(isize)) {
    var_(result, isize) = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) return (0 < lhs) ? isize_limit_max : isize_limit_min;
    return result;
};
fn_((isize_subSat(isize lhs, isize rhs))(isize)) {
    var_(result, isize) = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) return (0 < lhs) ? isize_limit_max : isize_limit_min;
    return result;
};
fn_((isize_mulSat(isize lhs, isize rhs))(isize)) {
    var_(result, isize) = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) {
        let_(same_sign, bool) = (0 < lhs && 0 < rhs) || (lhs < 0 && rhs < 0);
        return same_sign ? isize_limit_max : isize_limit_min;
    }
    return result;
};
fn_((isize_divSat(isize lhs, isize rhs))(isize)) {
    if (rhs == 0) return (0 <= lhs) ? isize_limit_max : isize_limit_min;
    if (lhs == isize_limit_min && rhs == -1) return isize_limit_max;
    return lhs / rhs;
};
fn_((isize_negSat(isize x))(isize)) {
    if (x == isize_limit_min) return isize_limit_max;
    return -x;
};
fn_((isize_absSat(isize x))(isize)) {
    if (x == isize_limit_min) return isize_limit_max;
    return (x < 0) ? -x : x;
};
fn_((isize_powSat(isize base, u32 exp))(isize)) {
    if (exp == 0) return 1;
    var_(result, isize) = 1;
    for (var_(i, u32) = 0; i < exp; ++i) {
        let_(old_result, isize) = result;
        if (__builtin_mul_overflow(result, base, &result)) {
            let_(same_sign, bool) = (0 < old_result && 0 < base) || (old_result < 0 && base < 0);
            return same_sign ? isize_limit_max : isize_limit_min;
        }
    }
    return result;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_int__included */

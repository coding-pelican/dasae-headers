/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    unicode.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-10 (date of creation)
 * @updated 2026-01-10 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  unicode
 *
 * @brief   Central hub for Unicode conversions.
 * @details orchestrates conversions between UTF-8, UTF-16, WTF-8, and WTF-16.
 * Resolves circular dependencies by keeping sub-modules independent.
 */
#pragma once
#ifndef unicode__included
#define unicode__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "utf8.h"
#include "utf16.h"
#include "wtf8.h"
#include "wtf16.h"
#include "mem/Alctr.h"

/*========== Macros and Declarations ========================================*/

/*---------------------------------------------------------------------------
 * Section: Strict Conversions (UTF-8 <-> UTF-16)
 * Validates data. Returns error on invalid sequences or surrogates.
 *-------------------------------------------------------------------------*/

typedef E$$($set(utf8_E)(usize)) unicode_utf8_E$usize;
/// Calculates the length of the UTF-8 string in UTF-16 code units.
$attr($must_check)
$extern fn_((unicode_utf8ToUTF16Len(S_const$u8 utf8))(unicode_utf8_E$usize));
errset_((unicode_utf8ToUTF16_E)() $union_errset_(utf8_E, io_WriteE));
T_use_E$($set(unicode_utf8ToUTF16_E)(S$u16));
/// Converts UTF-8 to UTF-16.
/// Returns the slice of the written buffer.
$attr($must_check)
$extern fn_((unicode_utf8ToUTF16(S_const$u8 utf8, S$u16 out))(unicode_utf8ToUTF16_E$S$u16));
typedef E$$($set(utf8_E)(S$u16)) unicode_utf8_E$S$u16;
$attr($must_check)
$extern fn_((unicode_utf8ToUTF16Within(S_const$u8 utf8, S$u16 out))(unicode_utf8_E$S$u16));
errset_((unicode_utf8ToUTF16Alloc_E)() $union_errset_(utf8_E, mem_E));
T_use_E$($set(unicode_utf8ToUTF16Alloc_E)(S$u16));
/// Allocates and converts UTF-8 to UTF-16.
$attr($must_check)
$extern fn_((unicode_utf8ToUTF16Alloc(S_const$u8 utf8, mem_Alctr gpa))(unicode_utf8ToUTF16Alloc_E$S$u16));

/// Calculates the length of the UTF-16 string in UTF-8 bytes.
typedef E$$($set(utf16_E)(usize)) unicode_utf16_E$usize;
$attr($must_check)
$extern fn_((unicode_utf16ToUTF8Len(S_const$u16 utf16))(unicode_utf16_E$usize));
errset_((unicode_utf16ToUTF8_E)() $union_errset_(utf16_E, io_WriteE));
T_use_E$($set(unicode_utf16ToUTF8_E)(S$u8));
/// Converts UTF-16 to UTF-8.
/// Returns the slice of the written buffer.
$attr($must_check)
$extern fn_((unicode_utf16ToUTF8(S_const$u16 utf16, S$u8 out))(unicode_utf16ToUTF8_E$S$u8));
typedef E$$($set(utf16_E)(S$u8)) unicode_utf16_E$S$u8;
$attr($must_check)
$extern fn_((unicode_utf16ToUTF8Within(S_const$u16 utf16, S$u8 out))(unicode_utf16_E$S$u8));
errset_((unicode_utf16ToUTF8Alloc_E)() $union_errset_(utf16_E, mem_E));
T_use_E$($set(unicode_utf16ToUTF8Alloc_E)(S$u8));
/// Allocates and converts UTF-16 to UTF-8.
$attr($must_check)
$extern fn_((unicode_utf16ToUTF8Alloc(S_const$u16 utf16, mem_Alctr gpa))(unicode_utf16ToUTF8Alloc_E$S$u8));

/*---------------------------------------------------------------------------
 * Section: Permissive Conversions (WTF-8 <-> WTF-16)
 * Allows unpaired surrogates. Generally assumes valid WTF format.
 *-------------------------------------------------------------------------*/

/// Calculates bytes needed for WTF-8 to WTF-16 conversion.
$extern fn_((unicode_wtf8ToWTF16Len(S_const$u8 wtf8))(usize));
errset_((unicode_wtf8ToWTF16_E)() $union_errset_(wtf8_E, mem_E));
T_use_E$($set(unicode_wtf8ToWTF16_E)(S$u16));
/// Converts WTF-8 to WTF-16 (Permissive).
$attr($must_check)
$extern fn_((unicode_wtf8ToWTF16(S_const$u8 wtf8, S$u16 out))(unicode_wtf8ToWTF16_E$S$u16));
typedef E$$($set(wtf8_E)(S$u16)) unicode_wtf8_E$S$u16;
$attr($must_check)
$extern fn_((unicode_wtf8ToWTF16Within(S_const$u8 wtf8, S$u16 out))(unicode_wtf8_E$S$u16));
errset_((unicode_wtf8ToWTF16Alloc_E)() $union_errset_(wtf8_E, mem_E));
T_use_E$($set(unicode_wtf8ToWTF16Alloc_E)(S$u16));
/// Allocates and converts WTF-8 to WTF-16.
$attr($must_check)
$extern fn_((unicode_wtf8ToWTF16Alloc(S_const$u8 wtf8, mem_Alctr gpa))(unicode_wtf8ToWTF16Alloc_E$S$u16));

/// Calculates bytes needed for WTF-16 to WTF-8 conversion.
$extern fn_((unicode_wtf16ToWTF8Len(S_const$u16 wtf16))(usize));
T_alias$((unicode_io_WriteE$S$u8)(E$$($set(io_WriteE)(S$u8))));
/// Converts WTF-16 to WTF-8 (Permissive).
$attr($must_check)
$extern fn_((unicode_wtf16ToWTF8(S_const$u16 wtf16, S$u8 out))(unicode_io_WriteE$S$u8));
/// Converts WTF-16 to WTF-8 into a guaranteed buffer.
/// Cannot fail: No validation errors (permissive), No OOM (guaranteed buffer).
$extern fn_((unicode_wtf16ToWTF8Within(S_const$u16 wtf16, S$u8 out))(S$u8));
T_alias$((unicode_mem_E$S$u8)(E$$($set(mem_E)(S$u8))));
/// Allocates and converts WTF-16 to WTF-8.
$attr($must_check)
$extern fn_((unicode_wtf16ToWTF8Alloc(S_const$u16 wtf16, mem_Alctr gpa))(unicode_mem_E$S$u8));

/*---------------------------------------------------------------------------
 * Section: Cross-Casting & Validation (View Conversions)
 * Zero-cost casts where safe, Checks where necessary.
 *-------------------------------------------------------------------------*/

/// Casts UTF-8 view to WTF-8 view (Safe, Zero-cost).
/// UTF-8 is a strict subset of WTF-8.
$attr($inline_always)
$static fn_((unicode_utf8AsWTF8(utf8_View utf))(wtf8_View));
/// Tries to cast WTF-8 view to UTF-8 view (Checked).
/// O(N) validation required to ensure no unpaired surrogates exist.
$attr($must_check)
$extern fn_((unicode_wtf8AsUTF8(wtf8_View wtf))(utf8_E$utf8_View));
errset_((unicode_wtf8ToUTF8LossyAlloc_E)() $union_errset_(utf8_E, io_WriteE, mem_E));
T_use_E$($set(unicode_wtf8ToUTF8LossyAlloc_E)(S$u8));
/// Lossy conversion from WTF-8 to UTF-8 (Allocating).
/// Replaces ill-formed sequences or surrogates with Replacement Char.
$attr($must_check)
$extern fn_((unicode_wtf8ToUTF8LossyAlloc(S_const$u8 wtf8, mem_Alctr gpa))(unicode_wtf8ToUTF8LossyAlloc_E$S$u8));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((unicode_utf8AsWTF8(utf8_View utf))(wtf8_View)) {
    return wtf8_viewUnchkd(utf.bytes);
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* unicode__included */

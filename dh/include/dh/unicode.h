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
#ifndef unicode__included
#define unicode__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/utf8.h"
#include "dh/utf16.h"
#include "dh/wtf8.h"
#include "dh/wtf16.h"

/*========== Macros and Declarations ========================================*/

/*---------------------------------------------------------------------------
 * Section: Strict Conversions (UTF-8 <-> UTF-16)
 * Validates data. Returns error on invalid sequences or surrogates.
 *-------------------------------------------------------------------------*/

typedef E$$($set(utf8_Err)(usize)) unicode_utf8_Err$usize;
/// Calculates the length of the UTF-8 string in UTF-16 code units.
$attr($must_check)
$extern fn_((unicode_utf8ToUTF16Len(S_const$u8 utf8))(unicode_utf8_Err$usize));
/// Converts UTF-8 to UTF-16.
/// Returns the slice of the written buffer.
$attr($must_check) /* `utf8_Err` + `unicode_mem_Err` */
$extern fn_((unicode_utf8ToUTF16(S_const$u8 utf8, S$u16 out))(E$S$u16));
typedef E$$($set(utf8_Err)(S$u16)) unicode_utf8_Err$S$u16;
$attr($must_check)
$extern fn_((unicode_utf8ToUTF16Within(S_const$u8 utf8, S$u16 out))(unicode_utf8_Err$S$u16));
typedef E$$($set(mem_Err)(S$u16)) unicode_mem_Err$S$u16;
/// Allocates and converts UTF-8 to UTF-16.
$attr($must_check)
$extern fn_((unicode_utf8ToUTF16Alloc(S_const$u8 utf8, mem_Allocator gpa))(unicode_mem_Err$S$u16));

/// Calculates the length of the UTF-16 string in UTF-8 bytes.
typedef E$$($set(utf16_Err)(usize)) unicode_utf16_Err$usize;
$attr($must_check)
$extern fn_((unicode_utf16ToUTF8Len(S_const$u16 utf16))(unicode_utf16_Err$usize));
/// Converts UTF-16 to UTF-8.
/// Returns the slice of the written buffer.
$attr($must_check) /* utf16_Err` + `unicode_mem_Err` */
$extern fn_((unicode_utf16ToUTF8(S_const$u16 utf16, S$u8 out))(E$S$u8));
typedef E$$($set(utf16_Err)(S$u8)) unicode_utf16_Err$S$u8;
$attr($must_check)
$extern fn_((unicode_utf16ToUTF8Within(S_const$u16 utf16, S$u8 out))(unicode_utf16_Err$S$u8));
typedef E$$($set(mem_Err)(S$u8)) unicode_mem_Err$S$u8;
/// Allocates and converts UTF-16 to UTF-8.
$attr($must_check)
$extern fn_((unicode_utf16ToUTF8Alloc(S_const$u16 utf16, mem_Allocator gpa))(unicode_mem_Err$S$u8));

/*---------------------------------------------------------------------------
 * Section: Permissive Conversions (WTF-8 <-> WTF-16)
 * Allows unpaired surrogates. Generally assumes valid WTF format.
 *-------------------------------------------------------------------------*/

/// Calculates bytes needed for WTF-8 to WTF-16 conversion.
$extern fn_((unicode_wtf8ToWTF16Len(S_const$u8 wtf8))(usize));
/// Converts WTF-8 to WTF-16 (Permissive).
$attr($must_check) /* `wtf8_Err` + `unicode_mem_Err` */
$extern fn_((unicode_wtf8ToWTF16(S_const$u8 wtf8, S$u16 out))(E$S$u16));
typedef E$$($set(wtf8_Err)(S$u16)) unicode_wtf8_Err$S$u16;
$attr($must_check)
$extern fn_((unicode_wtf8ToWTF16Within(S_const$u8 wtf8, S$u16 out))(unicode_wtf8_Err$S$u16));
/// Allocates and converts WTF-8 to WTF-16.
$attr($must_check)
$extern fn_((unicode_wtf8ToWTF16Alloc(S_const$u8 wtf8, mem_Allocator gpa))(unicode_mem_Err$S$u16));

/// Calculates bytes needed for WTF-16 to WTF-8 conversion.
$extern fn_((unicode_wtf16ToWTF8Len(S_const$u16 wtf16))(usize));
/// Converts WTF-16 to WTF-8 (Permissive).
$attr($must_check)
$extern fn_((unicode_wtf16ToWTF8(S_const$u16 wtf16, S$u8 out))(unicode_mem_Err$S$u8));
/// Converts WTF-16 to WTF-8 into a guaranteed buffer.
/// Cannot fail: No validation errors (permissive), No OOM (guaranteed buffer).
$extern fn_((unicode_wtf16ToWTF8Within(S_const$u16 wtf16, S$u8 out))(S$u8));
/// Allocates and converts WTF-16 to WTF-8.
$attr($must_check)
$extern fn_((unicode_wtf16ToWTF8Alloc(S_const$u16 wtf16, mem_Allocator gpa))(unicode_mem_Err$S$u8));

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
$extern fn_((unicode_wtf8AsUTF8(wtf8_View wtf))(utf8_Err$utf8_View));
/// Lossy conversion from WTF-8 to UTF-8 (Allocating).
/// Replaces ill-formed sequences or surrogates with Replacement Char.
$attr($must_check)
$extern fn_((unicode_wtf8ToUTF8LossyAlloc(S_const$u8 wtf8, mem_Allocator gpa))(unicode_mem_Err$S$u8));

/*========== Macros and Definitions =========================================*/

fn_((unicode_utf8AsWTF8(utf8_View utf))(wtf8_View)) {
    return wtf8_viewUnchkd(utf.bytes);
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* unicode__included */

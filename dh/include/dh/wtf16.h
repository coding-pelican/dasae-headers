/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    wtf16.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-10 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  wtf16
 *
 * @brief   Wobbly Transformation Format - 16-bit.
 * @details Essentially standard u16 arrays, treated permissively.
 */
#ifndef wtf16__included
#define wtf16__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "utf16.h" // For utf16_isHighSurrogate helpers

/*========== Macros and Declarations ========================================*/

/// Unlike utf16_Iter, this does NOT return errors for unpaired surrogates.
/// It yields them as standalone codepoints.
typedef struct wtf16_Iter {
    var_(bytes, S_const$u16);
    var_(idx, usize);
} wtf16_Iter;
$extern fn_((wtf16_iter(S_const$u16 units))(wtf16_Iter));
/// Returns next codepoint.
/// - If valid pair: returns combined scalar value.
/// - If unpaired surrogate: returns the surrogate value itself (U+D800..U+DFFF).
$extern fn_((wtf16_Iter_next(wtf16_Iter* self))(O$u32));

/// Calculates bytes needed for WTF-8.
$extern fn_((wtf16_calcWTF8Len(S_const$u16 wtf16))(usize));
typedef E$$($set(mem_Err)(S$u8)) wtf16_mem_Err$S$u8;
/// Converts to WTF-8. Cannot fail (unpaired surrogates are valid in WTF-8).
$attr($must_check)
$extern fn_((wtf16_toWTF8(S_const$u16 wtf16, S$u8 out_wtf8))(wtf16_mem_Err$S$u8));
$extern fn_((wtf16_toWTF8Within(S_const$u16 wtf16, S$u8 out_wtf8))(S$u8));
/// Allocates a new WTF-8 string from the given WTF-16 string.
$attr($must_check)
$extern fn_((wtf16_toWTF8Alloc(S_const$u16 wtf16, mem_Allocator gpa))(wtf16_mem_Err$S$u8));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* wtf16__included */

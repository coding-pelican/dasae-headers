/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    wtf8.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-10 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  wtf8
 *
 * @brief   Wobbly Transformation Format - 8-bit.
 * @details Superset of UTF-8 that allows unpaired surrogates.
 */
#ifndef wtf8__included
#define wtf8__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "utf8.h" // Re-uses utf8_SeqByte, etc.

/*========== Macros and Declarations ========================================*/

/// WTF-8 specific errors (mostly subset of UTF-8 errors minus surrogate checks)
errset_((wtf8_Err)(
    InvalidStartByte,
    ExpectedContinuation,
    OverlongEncoding,
    CodepointTooLarge
));

T_use_E$(utf8_SeqByte);
/// Encodes a codepoint into WTF-8 (Allows surrogates).
$attr($must_check) /* `utf8_Err` + `mem_Err` */
$extern fn_((wtf8_encode(u32 codepoint, S$u8 out))(E$S$u8));
T_use_E$($set(wtf8_Err)(S$u8));
$attr($must_check)
$extern fn_((wtf8_encodeWithin(u32 codepoint, S$u8 out))(wtf8_Err$S$u8));
T_use_E$($set(wtf8_Err)(u32));
/// Decodes WTF-8 bytes to a codepoint (Allows surrogates).
$attr($must_check)
$extern fn_((wtf8_decode(S_const$u8 bytes))(wtf8_Err$u32));
/// Validates if the slice is valid WTF-8.
$extern fn_((wtf8_validate(S_const$u8 bytes))(bool));

typedef struct wtf8_View {
    var_(bytes, S_const$u8);
} wtf8_View;
T_use_E$($set(wtf8_Err)(wtf8_View));
$attr($must_check)
$extern fn_((wtf8_view(S_const$u8 bytes))(wtf8_Err$wtf8_View));
$extern fn_((wtf8_viewUnchkd(S_const$u8 bytes))(wtf8_View));

typedef struct wtf8_Iter {
    var_(bytes, S_const$u8);
    var_(idx, usize);
} wtf8_Iter;
$extern fn_((wtf8_iter(wtf8_View view))(wtf8_Iter));
/// Returns next codepoint. (Surrogates are returned as-is, not errors).
$extern fn_((wtf8_Iter_next(wtf8_Iter* self))(O$u32));
$extern fn_((wtf8_Iter_nextBytes(wtf8_Iter* self))(O$S_const$u8));

T_use_E$($set(wtf8_Err)(usize));
/// Conversion from WTF-8 to WTF-16 is defined here.
/// Note: WTF-16 is just S$u16 (allows unpaired surrogates).
$attr($must_check)
$extern fn_((wtf8_calcWTF16Len(S_const$u8 wtf8))(wtf8_Err$usize));
/// Converts a WTF-8 string to WTF-16 (Host Endian).
$attr($must_check) /* `wtf8_Err` + `mem_Err` */
$extern fn_((wtf8_toWTF16(S_const$u8 wtf8, S$u16 out_wtf16))(E$S$u16));
T_use_E$($set(wtf8_Err)(S$u16));
$attr($must_check)
$extern fn_((wtf8_toWTF16Within(S_const$u8 wtf8, S$u16 out_wtf16))(wtf8_Err$S$u16));
typedef E$$($set(mem_Err)(S$u16)) wtf8_mem_Err$S$u16;
/// Allocates a new WTF-16 string from the given WTF-8 string.
$attr($must_check)
$extern fn_((wtf8_toWTF16Alloc(S_const$u8 wtf8, mem_Allocator gpa))(wtf8_mem_Err$S$u16));
typedef E$$($set(mem_Err)(S$u8)) wtf8_mem_Err$S$u8;
/// Lossy conversion to strict UTF-8 (Replaces ill-formed/surrogates with replacement char).
$attr($must_check)
$extern fn_((wtf8_toUTF8LossyAlloc(S_const$u8 wtf8, mem_Allocator gpa))(wtf8_mem_Err$S$u8));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* wtf8__included */

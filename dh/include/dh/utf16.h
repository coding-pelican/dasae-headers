/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    utf16.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-10 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  utf16

 * @brief   Strict UTF-16 handling (Validates Surrogate Pairs).
 * @details Assumes Host Endian for u16 arrays.
 */
#ifndef utf16__included
#define utf16__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "utf8.h" // For utf8_Err reuse or defining generic unicode errors

/*========== Macros and Declarations ========================================*/

/// Re-using utf8_Err for general encoding errors, or define utf16_Err if specific needed.
/// Commonly shared errors: ExpectedSecondSurrogateHalf, DanglingSurrogateHalf
errset_((utf16_Err)(
    InvalidStartCodeUnit,
    ExpectedSecondSurrogateHalf,
    DanglingSurrogateHalf,
    UnexpectedSecondSurrogateHalf,
    CodepointTooLarge
));

/// Codeunit sequence length (1-2)
typedef enum_($packed utf16_SeqLen){
    utf16_SeqLen_1 = 1,
    utf16_SeqLen_2 = 2,
} utf16_SeqLen;
claim_assert_static(Type_eq$(utf16_SeqLen, u8));
T_use_E$($set(utf16_Err)(utf16_SeqLen));

/// Returns true if the codeunit is a high surrogate (U+D800..U+DBFF).
$attr($inline_always)
$static fn_((utf16_isHighSurrogate(u16 codeunit))(bool));
/// Returns true if the codeunit is a low surrogate (U+DC00..U+DFFF).
$attr($inline_always)
$static fn_((utf16_isLowSurrogate(u16 codeunit))(bool));
/// Returns true if the codepoint is in the surrogate range (U+D800..U+DFFF).
$attr($inline_always)
$static fn_((utf16_isSurrogate(u32 codepoint))(bool));
/// Returns 1 or 2 depending on whether the code unit is a high surrogate.
$attr($must_check)
$extern fn_((utf16_codeunitSeqLen(u16 first_codeunit))(utf16_Err$utf16_SeqLen));
T_use_E$($set(utf16_Err)(utf8_SeqLen));
/// Returns bytes length (1-4) if encoded in UTF-8.
$attr($must_check)
$extern fn_((utf16_codepointSeqLen(u32 codepoint))(utf16_Err$utf8_SeqLen));
T_use_E$($set(utf16_Err)(S$u16));
/// Encodes a single codepoint into UTF-16 code units.
/// Returns the slice of the written buffer.
$attr($must_check) /* `utf16_Err` + `mem_Err` */
$extern fn_((utf16_encode(u32 codepoint, S$u16 out))(E$S$u16));
$attr($must_check)
$extern fn_((utf16_encodeWithin(u32 codepoint, S$u16 out))(utf16_Err$S$u16));
T_use_E$($set(utf16_Err)(u32));
/// Decodes UTF-16 code units to a codepoint.
$attr($must_check)
$extern fn_((utf16_decode(S_const$u16 codeunits))(utf16_Err$u32));
/// Decodes a surrogate pair (High Codeunit, Low Codeunit) into a codepoint.
$attr($must_check)
$extern fn_((utf16_decodeSurrogatePair(u16 high_codeunit, u16 low_codeunit))(utf16_Err$u32));
/// Validates if the slice is valid UTF-16.
$extern fn_((utf16_validate(S_const$u16 codeunits))(bool));
/// Returns the number of code units in the given string.
$extern fn_((utf16_count(S_const$u16 codeunits))(usize));

typedef struct utf16_Iter {
    var_(bytes, S_const$u16);
    var_(idx, usize);
} utf16_Iter;
$extern fn_((utf16_iter(S_const$u16 codeunits))(utf16_Iter));
T_use_E$($set(utf16_Err)(O$u32));
/// Returns next codepoint. Fails if invalid surrogate pair found.
$attr($must_check)
$extern fn_((utf16_Iter_next(utf16_Iter* self))(utf16_Err$O$u32));

/*========== Macros and Definitions =========================================*/

fn_((utf16_isHighSurrogate(u16 codeunit))(bool)) {
    return (codeunit & ~0x03ff) == 0xd800;
};

fn_((utf16_isLowSurrogate(u16 codeunit))(bool)) {
    return (codeunit & ~0x03ff) == 0xdc00;
};

fn_((utf16_isSurrogate(u32 codepoint))(bool)) {
    return 0xD800 <= codepoint && codepoint <= 0xDFFF;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* utf16__included */

/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    utf8.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-31 (date of creation)
 * @updated 2026-01-10 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  utf8
 */
#ifndef utf8__included
#define utf8__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

errset_((utf8_Err)(
    InvalidBytes,
    InvalidStartByte,
    TooLargeCodepoint,
    EncodesSurrogateHalf,
    ExpectedContinuation,
    OverlongEncoding
));

/// Byte sequence length (1-4)
typedef enum_($packed utf8_SeqByte){
    utf8_SeqByte_1 = lit_num(0b, 0000, 0000),
    utf8_SeqByte_2 = lit_num(0b, 1100, 0000),
    utf8_SeqByte_3 = lit_num(0b, 1110, 0000),
    utf8_SeqByte_4 = lit_num(0b, 1111, 0000),
} utf8_SeqByte;
claim_assert_static(Type_eq$(utf8_SeqByte, u8));

/// Codepoint sequence length (1-4)
typedef enum_($packed utf8_SeqLen){
    utf8_SeqLen_1 = 1,
    utf8_SeqLen_2 = 2,
    utf8_SeqLen_3 = 3,
    utf8_SeqLen_4 = 4,
} utf8_SeqLen;
claim_assert_static(Type_eq$(utf8_SeqLen, u8));
T_use_E$(utf8_SeqLen);
T_use_E$($set(utf8_Err)(utf8_SeqLen));

#define utf8_replacement_ch __comp_int__utf8_replacement_ch

$attr($must_check)
$extern fn_((utf8_codepointSeqLen(u32 codepoint))(utf8_Err$utf8_SeqLen));
$attr($must_check)
$extern fn_((utf8_byteSeqLen(u8 first_byte))(utf8_Err$utf8_SeqLen));
$attr($must_check) /* `utf8_Err` + `mem_Err` */
$extern fn_((utf8_encode(u32 codepoint, S$u8 out))(E$S$u8));
T_use_E$($set(utf8_Err)(S$u8));
$extern fn_((utf8_encodeWithin(u32 codepoint, S$u8 out))(utf8_Err$S$u8));

T_use_E$($set(utf8_Err)(u32));
$attr($must_check)
$extern fn_((utf8_decode(S_const$u8 bytes))(utf8_Err$u32));
typedef A$$(2, u8) utf8_Decode2Buf;
$attr($must_check)
$extern fn_((utf8_decode2(utf8_Decode2Buf bytes))(utf8_Err$u32));
typedef A$$(3, u8) utf8_Decode3Buf;
$attr($must_check)
$extern fn_((utf8_decode3(utf8_Decode3Buf bytes))(utf8_Err$u32));
typedef A$$(4, u8) utf8_Decode4Buf;
$attr($must_check)
$extern fn_((utf8_decode4(utf8_Decode4Buf bytes))(utf8_Err$u32));

$extern fn_((utf8_isValid(u32 codepoint))(bool));
$extern fn_((utf8_validate(S_const$u8 bytes))(bool));
$extern fn_((utf8_count(S_const$u8 bytes))(usize));

typedef struct utf8_View {
    var_(bytes, S_const$u8);
} utf8_View;
T_use_E$($set(utf8_Err)(utf8_View));
$attr($must_check)
$extern fn_((utf8_view(S_const$u8 bytes))(utf8_Err$utf8_View));
$extern fn_((utf8_viewUnchkd(S_const$u8 bytes))(utf8_View));

typedef struct utf8_Iter {
    var_(bytes, S_const$u8);
    var_(idx, usize);
} utf8_Iter;
$extern fn_((utf8_iter(utf8_View view))(utf8_Iter));
$extern fn_((utf8_Iter_next(utf8_Iter* self))(O$u32));
$extern fn_((utf8_Iter_nextBytes(utf8_Iter* self))(O$S_const$u8));
$extern fn_((utf8_Iter_peek(utf8_Iter* self))(O$u32));
$extern fn_((utf8_Iter_peekBytes(utf8_Iter* self))(O$S_const$u8));
$extern fn_((utf8_Iter_peekAt(utf8_Iter* self, usize n))(O$u32));
$extern fn_((utf8_Iter_peekAtBytes(utf8_Iter* self, usize n))(O$S_const$u8));

/*========== Macros and Definitions =========================================*/

/// Use this to replace an unknown, unrecognized, or unrepresentable character.
///
/// See also:
/// https://en.wikipedia.org/wiki/Specials_(Unicode_block)#Replacement_character
#define __comp_int__utf8_replacement_ch 0xFFFDu

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* utf8__included */

/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ascii.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-20 (date of creation)
 * @updated 2026-06-12 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  ascii
 */
#pragma once
#ifndef ascii__included
#define ascii__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "mem/Alctr.h"

/*========== Macros and Declarations ========================================*/

/// The C0 control codes of the ASCII encoding.
enum_((ascii_CtrlCode $fits($packed))(
    ascii_CtrlCode_nul = 0x00, ///< Null ('\0').
    ascii_CtrlCode_soh = 0x01, ///< Start of Heading.
    ascii_CtrlCode_stx = 0x02, ///< Start of Text.
    ascii_CtrlCode_etx = 0x03, ///< End of Text.
    ascii_CtrlCode_eot = 0x04, ///< End of Transmission.
    ascii_CtrlCode_enq = 0x05, ///< Enquiry.
    ascii_CtrlCode_ack = 0x06, ///< Acknowledge.
    ascii_CtrlCode_bel = 0x07, ///< Bell, Alert ('\a').
    ascii_CtrlCode_bs = 0x08, ///< Backspace ('\b').
    ascii_CtrlCode_ht = 0x09, ///< Horizontal Tab, Tab ('\t').
    ascii_CtrlCode_lf = 0x0A, ///< Line Feed, Newline ('\n').
    ascii_CtrlCode_vt = 0x0B, ///< Vertical Tab ('\v').
    ascii_CtrlCode_ff = 0x0C, ///< Form Feed ('\f').
    ascii_CtrlCode_cr = 0x0D, ///< Carriage Return ('\r').
    ascii_CtrlCode_so = 0x0E, ///< Shift Out.
    ascii_CtrlCode_si = 0x0F, ///< Shift In.
    ascii_CtrlCode_dle = 0x10, ///< Data Link Escape.
    ascii_CtrlCode_dc1 = 0x11, ///< Device Control One (XON, `ascii_CtrlCode_xon`).
    ascii_CtrlCode_dc2 = 0x12, ///< Device Control Two.
    ascii_CtrlCode_dc3 = 0x13, ///< Device Control Three (XOFF, `ascii_CtrlCode_xoff`).
    ascii_CtrlCode_dc4 = 0x14, ///< Device Control Four.
    ascii_CtrlCode_nak = 0x15, ///< Negative Acknowledge.
    ascii_CtrlCode_syn = 0x16, ///< Synchronous Idle.
    ascii_CtrlCode_etb = 0x17, ///< End of Transmission Block
    ascii_CtrlCode_can = 0x18, ///< Cancel.
    ascii_CtrlCode_em = 0x19, ///< End of Medium.
    ascii_CtrlCode_sub = 0x1A, ///< Substitute.
    ascii_CtrlCode_esc = 0x1B, ///< Escape.
    ascii_CtrlCode_fs = 0x1C, ///< File Separator.
    ascii_CtrlCode_gs = 0x1D, ///< Group Separator.
    ascii_CtrlCode_rs = 0x1E, ///< Record Separator.
    ascii_CtrlCode_us = 0x1F, ///< Unit Separator.

    ascii_CtrlCode_del = 0x7F, /// Delete.

    ascii_CtrlCode_xon = ascii_CtrlCode_dc1, /// An alias to `dc1`.
    ascii_CtrlCode_xoff = ascii_CtrlCode_dc3, /// An alias to `dc3`.
));
claim_assert_static(eqlType$(enum ascii_CtrlCode, u8));

#define ascii_nul __str__ascii_nul
#define ascii_nul_byte __uint__ascii_nul_byte
#define ascii_bel __str__ascii_bel
#define ascii_bel_byte __uint__ascii_bel_byte
#define ascii_bs __str__ascii_bs
#define ascii_bs_byte __uint__ascii_bs_byte
#define ascii_ht __str__ascii_ht
#define ascii_ht_byte __uint__ascii_ht_byte
#define ascii_lf __str__ascii_lf
#define ascii_lf_byte __uint__ascii_lf_byte
#define ascii_vt __str__ascii_vt
#define ascii_vt_byte __uint__ascii_vt_byte
#define ascii_ff __str__ascii_ff
#define ascii_ff_byte __uint__ascii_ff_byte
#define ascii_cr __str__ascii_cr
#define ascii_cr_byte __uint__ascii_cr_byte
#define ascii_sp __str__ascii_sp
#define ascii_sp_byte __uint__ascii_sp_byte

/// Returns whether the character is a 7-bit ASCII character.
$attr($inline_always)
$static fn_((ascii_isASCII(u8 c))(bool));
/// Returns whether the character is an uppercase letter: A-Z.
$attr($inline_always)
$static fn_((ascii_isUpper(u8 c))(bool));
/// Returns whether the character is a lowercase letter: a-z.
$attr($inline_always)
$static fn_((ascii_isLower(u8 c))(bool));
/// Returns whether the character is alphabetic: A-Z || a-z.
$attr($inline_always)
$static fn_((ascii_isAlpha(u8 c))(bool));
/// Returns whether the character is a digit: 0-9.
$attr($inline_always)
$static fn_((ascii_isDigit(u8 c))(bool));
/// Returns whether the character is alphanumeric: A-Z, a-z, || 0-9.
$attr($inline_always)
$static fn_((ascii_isAlNum(u8 c))(bool));
/// Returns whether the character is a hexadecimal digit: A-F, a-f, || 0-9.
$attr($inline_always)
$static fn_((ascii_isHex(u8 c))(bool));
/// Returns whether the character is a control character.
$attr($inline_always)
$static fn_((ascii_isCtrl(u8 c))(bool));
/// Returns whether the character is a glyph.
$attr($inline_always)
$static fn_((ascii_isGlyph(u8 c))(bool));
/// Returns whether the character is a whitespace character.
$attr($inline_always)
$static fn_((ascii_isWhitespace(u8 c))(bool));
#define ascii_whitespaces \
    /** \
     * Whitespace for general use. \
     * This may be used with e.g. `mem_trim` to trim whitespace. \
     */ \
    __str__ascii_whitespaces

#define ascii_whitespace_sp __str__ascii_whitespace_sp
#define ascii_whitespace_sp_byte __uint__ascii_whitespace_sp_byte
#define ascii_whitespace_ht __str__ascii_whitespace_ht
#define ascii_whitespace_ht_byte __uint__ascii_whitespace_ht_byte
#define ascii_whitespace_lf __str__ascii_whitespace_lf
#define ascii_whitespace_lf_byte __uint__ascii_whitespace_lf_byte
#define ascii_whitespace_cr __str__ascii_whitespace_cr
#define ascii_whitespace_cr_byte __uint__ascii_whitespace_cr_byte
#define ascii_whitespace_vt __str__ascii_whitespace_vt
#define ascii_whitespace_vt_byte __uint__ascii_whitespace_vt_byte
#define ascii_whitespace_ff __str__ascii_whitespace_ff
#define ascii_whitespace_ff_byte __uint__ascii_whitespace_ff_byte

/// Uppercases the character && returns it as-is if already uppercase || not a letter.
$attr($inline_always)
$static fn_((ascii_toUpper(u8 c))(u8));
/// Lowercases the character && returns it as-is if already lowercase || not a letter.
$attr($inline_always)
$static fn_((ascii_toLower(u8 c))(u8));
/// Toggles the case of the character && returns it as-is if not a letter.
$attr($inline_always)
$static fn_((ascii_toggleCase(u8 c))(u8));

/// Converts the string to uppercase.
$extern fn_((ascii_toUppers(S$u8 ascii_str))(S$u8));
/// Converts the string to lowercase.
$extern fn_((ascii_toLowers(S$u8 ascii_str))(S$u8));
/// Converts the string to toggle case.
$extern fn_((ascii_toggleCases(S$u8 ascii_str))(S$u8));

/// Allocates an upper case copy of `ascii_str`.
$attr($must_check)
$extern fn_((ascii_allocUppers(mem_Alctr gpa, S_const$u8 ascii_str))(E$S$u8));
/// Allocates a lower case copy of `ascii_str`.
$attr($must_check)
$extern fn_((ascii_allocLowers(mem_Alctr gpa, S_const$u8 ascii_str))(E$S$u8));
/// Allocates a toggled case copy of `ascii_str`.
$attr($must_check)
$extern fn_((ascii_allocToggledCases(mem_Alctr gpa, S_const$u8 ascii_str))(E$S$u8));

/// Writes an upper case copy of `ascii_str` to `buf`.
$extern fn_((ascii_makeUppers(S$u8 out_buf, S_const$u8 ascii_str))(S$u8));
/// Writes a lower case copy of `ascii_str` to `buf`.
$extern fn_((ascii_makeLowers(S$u8 out_buf, S_const$u8 ascii_str))(S$u8));
/// Writes a toggled case copy of `ascii_str` to `buf`.
$extern fn_((ascii_makeToggledCases(S$u8 out_buf, S_const$u8 ascii_str))(S$u8));

$attr($inline_always)
$static fn_((ascii_digitToInt(u8 c))(u8));
$static let ascii_intFromDigit = ascii_digitToInt;
$attr($inline_always)
$static fn_((ascii_intToDigit(u8 val))(u8));
$static let ascii_digitFromInt = ascii_intToDigit;

/// Returns the index of the first occurrence of `ascii_substr` in `ascii_str`, ignoring case.
$extern fn_((ascii_idxOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr))(O$usize));
/// Returns the index of the first occurrence of `ascii_substr` starting from `start_front`, ignoring case.
$extern fn_((ascii_idxFirstOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr, usize start_front))(O$usize));
/// Returns the index of the last occurrence of `ascii_substr` starting from `start_back`, ignoring case.
$extern fn_((ascii_idxLastOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr, usize start_back))(O$usize));

/// Returns whether `ascii_str` starts with `ascii_prefix`, ignoring case.
$extern fn_((ascii_startsWithIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_prefix))(bool));
/// Returns whether `ascii_str` ends with `ascii_suffix`, ignoring case.
$extern fn_((ascii_endsWithIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_suffix))(bool));

/// Equality comparison.
$extern fn_((ascii_eql(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs, bool ignores_case))(bool));
/// Equality comparison, respecting case.
$extern fn_((ascii_eqlSenseCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(bool));
/// Equality comparison, ignoring case.
$extern fn_((ascii_eqlIgnoreCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(bool));

/// Compares lexicographical order of two ASCII strings.
$extern fn_((ascii_ord(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs, bool ignores_case))(cmp_Ord));
/// Compares lexicographical order of two ASCII strings, respecting case.
$extern fn_((ascii_ordSenseCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(cmp_Ord));
/// Compares lexicographical order of two ASCII strings, ignoring case.
$extern fn_((ascii_ordIgnoreCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(cmp_Ord));

/*========== Macros and Definitions =========================================*/

#define __str__ascii_nul "\0"
#define __uint__ascii_nul_byte u8_c('\0')
#define __str__ascii_bel "\a"
#define __uint__ascii_bel_byte u8_c('\a')
#define __str__ascii_bs "\b"
#define __uint__ascii_bs_byte u8_c('\b')
#define __str__ascii_ht "\t"
#define __uint__ascii_ht_byte u8_c('\t')
#define __str__ascii_lf "\n"
#define __uint__ascii_lf_byte u8_c('\n')
#define __str__ascii_vt "\v"
#define __uint__ascii_vt_byte u8_c('\v')
#define __str__ascii_ff "\f"
#define __uint__ascii_ff_byte u8_c('\f')
#define __str__ascii_cr "\r"
#define __uint__ascii_cr_byte u8_c('\r')
#define __str__ascii_sp " "
#define __uint__ascii_sp_byte u8_c(' ')

#if on_analysis_active_only || on_comptime
fn_((ascii_isASCII(u8 c))(bool)) { return c < 0x80; };
fn_((ascii_isUpper(u8 c))(bool)) { return u8_c('A') <= c && c <= u8_c('Z'); };
fn_((ascii_isLower(u8 c))(bool)) { return u8_c('a') <= c && c <= u8_c('z'); };
fn_((ascii_isAlpha(u8 c))(bool)) { return ascii_isUpper(c) || ascii_isLower(c); };
fn_((ascii_isDigit(u8 c))(bool)) { return u8_c('0') <= c && c <= u8_c('9'); };
fn_((ascii_isAlNum(u8 c))(bool)) { return ascii_isDigit(c) || ascii_isAlpha(c); };
fn_((ascii_isHex(u8 c))(bool)) { return ascii_isDigit(c) || (u8_c('A') <= c && c <= u8_c('F')) || (u8_c('a') <= c && c <= u8_c('f')); };
fn_((ascii_isCtrl(u8 c))(bool)) { return c <= ascii_CtrlCode_us || c == ascii_CtrlCode_del; };
fn_((ascii_isGlyph(u8 c))(bool)) { return c < 0x20 || 0x7E <= c; };
fn_((ascii_isWhitespace(u8 c))(bool)) { return c == ascii_sp_byte || (ascii_CtrlCode_ht <= c && c <= ascii_CtrlCode_lf); };
#endif /* on_analysis_active_only || on_comptime */
#define __str__ascii_whitespaces ascii_sp ascii_ht ascii_lf ascii_cr ascii_vt ascii_ff

#define __str__ascii_whitespace_sp ascii_sp
#define __uint__ascii_whitespace_sp_byte ascii_sp_byte
#define __str__ascii_whitespace_ht ascii_ht
#define __uint__ascii_whitespace_ht_byte ascii_ht_byte
#define __str__ascii_whitespace_lf ascii_lf
#define __uint__ascii_whitespace_lf_byte ascii_lf_byte
#define __str__ascii_whitespace_cr ascii_cr
#define __uint__ascii_whitespace_cr_byte ascii_cr_byte
#define __str__ascii_whitespace_vt ascii_vt
#define __uint__ascii_whitespace_vt_byte ascii_vt_byte
#define __str__ascii_whitespace_ff ascii_ff
#define __uint__ascii_whitespace_ff_byte ascii_ff_byte

#if on_analysis_active_only || on_comptime
fn_((ascii_toUpper(u8 c))(u8)) {
    let mask = int_shl(boolToInt(ascii_isLower(c)), 5);
    return c ^ mask;
};
fn_((ascii_toLower(u8 c))(u8)) {
    let mask = int_shl(boolToInt(ascii_isUpper(c)), 5);
    return c | mask;
};
fn_((ascii_toggleCase(u8 c))(u8)) {
    let mask = int_shl(boolToInt(ascii_isAlpha(c)), 5);
    return c ^ mask;
};

fn_((ascii_digitToInt(u8 c))(u8)) {
    let_(val, u8) = (claim_assert(ascii_isDigit(c)), c - u8_c('0'));
    return val;
};
fn_((ascii_intToDigit(u8 val))(u8)) {
    let_(digit, u8) = val + u8_c('0');
    return claim_assert(ascii_isDigit(digit)), digit;
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ascii__included */

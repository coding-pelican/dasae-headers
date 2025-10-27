/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ascii.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-20 (date of creation)
 * @updated 2025-06-20 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  ascii
 */

#ifndef ascii__included
#define ascii__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/core.h"
#include "dh/fn.h"
#include "dh/sli.h"
#include "dh/Arr.h"
#include "dh/mem/Allocator.h"

/// The C0 control codes of the ASCII encoding.
enum ascii_CtrlCode {
    /// Null.
    ascii_CtrlCode_nul = 0x00,
    /// Start of Heading.
    ascii_CtrlCode_soh = 0x01,
    /// Start of Text.
    ascii_CtrlCode_stx = 0x02,
    /// End of Text.
    ascii_CtrlCode_etx = 0x03,
    /// End of Transmission.
    ascii_CtrlCode_eot = 0x04,
    /// Enquiry.
    ascii_CtrlCode_enq = 0x05,
    /// Acknowledge.
    ascii_CtrlCode_ack = 0x06,
    /// Bell, Alert.
    ascii_CtrlCode_bel = 0x07,
    /// Backspace.
    ascii_CtrlCode_bs = 0x08,
    /// Horizontal Tab, Tab ('\t').
    ascii_CtrlCode_ht = 0x09,
    /// Line Feed, Newline ('\n').
    ascii_CtrlCode_lf = 0x0A,
    /// Vertical Tab.
    ascii_CtrlCode_vt = 0x0B,
    /// Form Feed.
    ascii_CtrlCode_ff = 0x0C,
    /// Carriage Return ('\r').
    ascii_CtrlCode_cr = 0x0D,
    /// Shift Out.
    ascii_CtrlCode_so = 0x0E,
    /// Shift In.
    ascii_CtrlCode_si = 0x0F,
    /// Data Link Escape.
    ascii_CtrlCode_dle = 0x10,
    /// Device Control One (XON).
    ascii_CtrlCode_dc1 = 0x11,
    /// Device Control Two.
    ascii_CtrlCode_dc2 = 0x12,
    /// Device Control Three (XOFF).
    ascii_CtrlCode_dc3 = 0x13,
    /// Device Control Four.
    ascii_CtrlCode_dc4 = 0x14,
    /// Negative Acknowledge.
    ascii_CtrlCode_nak = 0x15,
    /// Synchronous Idle.
    ascii_CtrlCode_syn = 0x16,
    /// End of Transmission Block
    ascii_CtrlCode_etb = 0x17,
    /// Cancel.
    ascii_CtrlCode_can = 0x18,
    /// End of Medium.
    ascii_CtrlCode_em = 0x19,
    /// Substitute.
    ascii_CtrlCode_sub = 0x1A,
    /// Escape.
    ascii_CtrlCode_esc = 0x1B,
    /// File Separator.
    ascii_CtrlCode_fs = 0x1C,
    /// Group Separator.
    ascii_CtrlCode_gs = 0x1D,
    /// Record Separator.
    ascii_CtrlCode_rs = 0x1E,
    /// Unit Separator.
    ascii_CtrlCode_us = 0x1F,

    /// Delete.
    ascii_CtrlCode_del = 0x7F,

    /// An alias to `dc1`.
    ascii_CtrlCode_xon = ascii_CtrlCode_dc1,
    /// An alias to `dc3`.
    ascii_CtrlCode_xoff = ascii_CtrlCode_dc3,
};

/// Returns whether the character is a 7-bit ASCII character.
static $inline fn_((ascii_isAscii(u8 c))(bool)) { return c < 0x80; }
/// Returns whether the character is an uppercase letter: A-Z.
static $inline fn_((ascii_isUpper(u8 c))(bool)) { return 'A' <= c && c <= 'Z'; }
/// Returns whether the character is a lowercase letter: a-z.
static $inline fn_((ascii_isLower(u8 c))(bool)) { return 'a' <= c && c <= 'z'; }
/// Returns whether the character is alphabetic: A-Z || a-z.
static $inline fn_((ascii_isAlpha(u8 c))(bool)) { return ascii_isUpper(c) || ascii_isLower(c); }
/// Returns whether the character is a digit: 0-9.
static $inline fn_((ascii_isDigit(u8 c))(bool)) { return '0' <= c && c <= '9'; }
/// Returns whether the character is alphanumeric: A-Z, a-z, || 0-9.
static $inline fn_((ascii_isAlNum(u8 c))(bool)) { return ascii_isDigit(c) || ascii_isAlpha(c); }
/// Returns whether the character is a hexadecimal digit: A-F, a-f, || 0-9.
static $inline fn_((ascii_isHex(u8 c))(bool)) { return ascii_isDigit(c) || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f'); }
/// Returns whether the character is a control character.
static $inline fn_((ascii_isCtrl(u8 c))(bool)) { return c <= ascii_CtrlCode_us || c == ascii_CtrlCode_del; }
/// Returns whether the character is a glyph.
static $inline fn_((ascii_isGlyph(u8 c))(bool)) { return c < 0x20 || 0x7E <= c; }
/// Returns whether the character is a whitespace character.
static $inline fn_((ascii_isWhitespace(u8 c))(bool)) { return c == ' ' || (ascii_CtrlCode_ht <= c && c <= ascii_CtrlCode_lf); }
/// Whitespace for general use.
/// This may be used with e.g. `mem_trim` to trim whitespace.
static let ascii_whitespaces = init$A$$((6, u8){ ' ', '\t', '\n', '\r', ascii_CtrlCode_vt, ascii_CtrlCode_ff });

/// Uppercases the character && returns it as-is if already uppercase || not a letter.
static $inline fn_((ascii_toUpper(u8 c))(u8)) {
    let mask = as$((u8)(ascii_isLower(c))) << 5;
    return c ^ mask;
}
/// Lowercases the character && returns it as-is if already lowercase || not a letter.
static $inline fn_((ascii_toLower(u8 c))(u8)) {
    let mask = as$((u8)(ascii_isUpper(c))) << 5;
    return c | mask;
}
/// Toggles the case of the character && returns it as-is if not a letter.
static $inline fn_((ascii_toggleCase(u8 c))(u8)) {
    let mask = as$((u8)(ascii_isAlpha(c))) << 5;
    return c ^ mask;
}

/// Converts the string to uppercase.
extern fn_((ascii_toUppers(S$u8 ascii_str))(S$u8));
/// Converts the string to lowercase.
extern fn_((ascii_toLowers(S$u8 ascii_str))(S$u8));
/// Converts the string to toggle case.
extern fn_((ascii_toggleCases(S$u8 ascii_str))(S$u8));

/// Writes an upper case copy of `ascii_str` to `buf`.
extern fn_((ascii_makeUppers(S$u8 buf, S_const$u8 ascii_str))(S$u8));
/// Writes a lower case copy of `ascii_str` to `buf`.
extern fn_((ascii_makeLowers(S$u8 buf, S_const$u8 ascii_str))(S$u8));
/// Writes a toggled case copy of `ascii_str` to `buf`.
extern fn_((ascii_makeToggledCases(S$u8 buf, S_const$u8 ascii_str))(S$u8));

/// Allocates an upper case copy of `ascii_str`.
extern fn_((ascii_allocUppers(mem_Allocator allocator, S_const$u8 ascii_str))(E$S$u8)) $must_check;
/// Allocates a lower case copy of `ascii_str`.
extern fn_((ascii_allocLowers(mem_Allocator allocator, S_const$u8 ascii_str))(E$S$u8)) $must_check;
/// Allocates a toggled case copy of `ascii_str`.
extern fn_((ascii_allocToggledCases(mem_Allocator allocator, S_const$u8 ascii_str))(E$S$u8)) $must_check;

/// Returns the index of the first occurrence of `ascii_substr` in `ascii_str`, ignoring case.
extern fn_((ascii_idxOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr))(O$usize));
/// Returns the index of the first occurrence of `ascii_substr` starting from `start_front`, ignoring case.
extern fn_((ascii_idxFirstOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr, usize start_front))(O$usize));
/// Returns the index of the last occurrence of `ascii_substr` starting from `start_back`, ignoring case.
extern fn_((ascii_idxLastOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr, usize start_back))(O$usize));

/// Returns whether `ascii_str` starts with `ascii_prefix`, ignoring case.
extern fn_((ascii_startsWithIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_prefix))(bool));
/// Returns whether `ascii_str` ends with `ascii_suffix`, ignoring case.
extern fn_((ascii_endsWithIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_suffix))(bool));

/// Compares lexicographical order of two ASCII strings.
extern fn_((ascii_cmp(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs, bool ignores_case))(cmp_Ord));
/// Compares lexicographical order of two ASCII strings, respecting case.
extern fn_((ascii_cmpSenseCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(cmp_Ord));
/// Compares lexicographical order of two ASCII strings, ignoring case.
extern fn_((ascii_cmpIgnoreCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(cmp_Ord));

/// Equality comparison.
extern fn_((ascii_eql(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs, bool ignores_case))(bool));
/// Equality comparison, respecting case.
extern fn_((ascii_eqlSenseCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(bool));
/// Equality comparison, ignoring case.
extern fn_((ascii_eqlIgnoreCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(bool));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ascii__included */

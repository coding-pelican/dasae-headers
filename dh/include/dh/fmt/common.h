/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-09 (date of creation)
 * @updated 2025-10-25 (date of last update)
 * @version v0.2-alpha
 * @ingroup dasae-headers(dh)/fmt
 * @prefix  fmt
 *
 * @brief   Comprehensive formatting.
 * @details Supports indexed arguments, format specifiers, primitive types, optional, error result, and error.
 *
 * FORMAT SPECIFICATION:
 *   `{[index]:[fill][alignment][width].[precision][type][size]}`
 *
 *   Components:
 *   - index (optional):     Index of argument (0-indexed, 0-15, positional if omitted)
 *   - fill (optional):      Fill character for padding (default: ' '(space))
 *   - alignment (optional): '<' left, '>' right, '^' center (default: '<' left-aligned)
 *   - width (optional):     Minimum field width (default: 0)
 *   - precision (optional): Decimal places for floats (default: 6)
 *   - type (required):      Format type character (see TYPE CHARACTERS)
 *   - size (required):      Size modifier (see SIZE MODIFIERS)
 *
 * TYPE CHARACTERS:
 *   - Hex:                    x (lower), X (upper)
 *   - Octal:                  o
 *   - Binary:                 b
 *   - Boolean:                B
 *   - Unsigned:               u
 *   - Signed:                 d, i
 *   - Float:                  f, F
 *
 *   - Pointer:                p
 *   - Optional:               ? (see OPTIONAL/RESULT)
 *   - Result:                 ! (see OPTIONAL/RESULT)
 *   - Error:                  e (see OPTIONAL/RESULT)
 *   - Void:                   0 (see OPTIONAL/RESULT)
 *
 *   - ASCII Code:             c (u8)
 *   - UTF-8 Codepoint:        C (u32)
 *   - String Null-Terminated: z (P_const$u8)
 *   - String Slice:           s (S_const$u8)
 *
 * OPTIONAL/RESULT:
 *   - {:?}      → Optional tag: "some" or "none"
 *   - {:?T}     → Optional full: value or "none"
 *   - {:?0}     → O$void: "some" or "none" (same as {:?})
 *
 *   - {:!}      → Result tag: "ok" or "err"
 *   - {:!T}     → Result full: value or "[Domain] Code"
 *   - {:!0}     → E$void: "ok" or "[Domain] Code"
 *
 *   - {:e}      → Error type: "[Domain] Code"
 *
 * SIZE MODIFIERS:
 *   - hh     = 8-bit  (u8, i8)
 *   - h      = 16-bit (u16, i16)
 *   - (none) = 32-bit (u32, i32, f32) - DEFAULT
 *   - l      = 64-bit (u64, i64, f64)
 *   - ll     = 128-bit (u128, i128, f128) - (not supported yet)
 *   - z      = pointer size (usize, isize)
 *
 * EXAMPLES:
 *    - {:d}      → i32
 *    - {:dl}     → i64
 *    - {:uhh}    → u8
 *    - {:uz}     → usize
 *    - {:.2f}    → f64 with 2 decimals
 *    - {:08xl}   → u64 hex, zero-padded, width 8
 *    - {0:d}     → i32, argument 0 (indexed)
 *    - {:?d}     → O$i32
 *    - {:!s}     → E$S_const$u8
 *    - {:e}      → error object direct
 */

#ifndef fmt_common__included
#define fmt_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/io/Writer.h"
#include <stdarg.h>

/*========== Macros and Declarations ========================================*/

errset_((fmt_Err)(
    UnexpectedEndOfFormat,
    InvalidFormatSpecifier,
    MissingClosingBrace,
    InvalidIndexFormat,
    IndexOutOfBounds,
    InvalidWidthFormat,
    InvalidPrecisionFormat,
    InvalidAlignmentFormat,
    InvalidIntegerFormat,
    InvalidFloatFormat,
    InvalidBoolFormat,
    TooManyArguments,
    TooFewArguments
));

/// Maximum number of arguments
#define fmt_max_args          (16ull)
#define fmt_max_bytes_per_arg (24ull)

// ==================== Formatting (Output) ====================

/// Format values to a writer with indexed arguments
$extern fn_((fmt_format(io_Writer writer, S_const$u8 fmt, ...))(E$void)) $must_check;
/// Format values using va_list (for wrapper functions)
$extern fn_((fmt_formatVaArgs(io_Writer writer, S_const$u8 fmt, va_list va_args))(E$void)) $must_check;

// ==================== Parsing (Input) - Slice based ====================

$extern fn_((fmt_parseBool(S_const$u8 str))(E$bool)) $must_check;
$extern fn_((fmt_parse$bool(S_const$u8 str))(E$bool)) $must_check;

$extern fn_((fmt_parseUInt(S_const$u8 str, u8 base))(E$u64)) $must_check;
$extern fn_((fmt_parse$usize(S_const$u8 str, u8 base))(E$usize)) $must_check;
$extern fn_((fmt_parse$u64(S_const$u8 str, u8 base))(E$u64)) $must_check;
$extern fn_((fmt_parse$u32(S_const$u8 str, u8 base))(E$u32)) $must_check;
$extern fn_((fmt_parse$u16(S_const$u8 str, u8 base))(E$u16)) $must_check;
$extern fn_((fmt_parse$u8(S_const$u8 str, u8 base))(E$u8)) $must_check;

$extern fn_((fmt_parseInt(S_const$u8 str, u8 base))(E$i64)) $must_check;
$extern fn_((fmt_parse$isize(S_const$u8 str, u8 base))(E$isize)) $must_check;
$extern fn_((fmt_parse$i64(S_const$u8 str, u8 base))(E$i64)) $must_check;
$extern fn_((fmt_parse$i32(S_const$u8 str, u8 base))(E$i32)) $must_check;
$extern fn_((fmt_parse$i16(S_const$u8 str, u8 base))(E$i16)) $must_check;
$extern fn_((fmt_parse$i8(S_const$u8 str, u8 base))(E$i8)) $must_check;

$extern fn_((fmt_parseFlt(S_const$u8 str))(E$f64)) $must_check;
$extern fn_((fmt_parse$f64(S_const$u8 str))(E$f64)) $must_check;
$extern fn_((fmt_parse$f32(S_const$u8 str))(E$f32)) $must_check;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fmt_common__included */

/*
1. indexed arguments 지원.
[index][type]:[fill][alignment][width].[precision]
2. format specifier 지원.
pointer,
string slice (slice, but it formatted as string),
null-terminated string (C string),
character (single byte),
codepoint (4 bytes),
u8-u64, usize
i8-i64, isize
f32-f64 (precision supported, scientific notation supported)
boolean
binary (true false)
octal
hexadecimal lowercase
hexadecimal uppercase
only optional's tag(some, none)
only result's tag (ok, err)
err object (16bytes fat pointer, when implementing, use Err_domainToStr -> S_const$u8, Err_codeToStr -> S_const$u8)

3. optional, result type
? -> optional
! -> result

?c O$u8 -> (character|"none")
?p O$P$raw -> (pointer|"none")
!C E$u32 -> (codepoint|format(err object))
...
*/

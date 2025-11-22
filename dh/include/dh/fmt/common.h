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
 *   `{[index]:[fill][alignment][sign][#][width].[precision][type][size]}`
 *
 *   Components:
 *   - index (optional):     Index of argument (0-indexed, 0-15, positional if omitted)
 *   - fill (optional):      Fill character for padding (default: ' ', use '0' for zero-pad)
 *   - alignment (optional): '<' left, '>' right, '^' center (default: '<' left-aligned)
 *   - sign (optional):      '+' always show, ' ' space for positive (default: '-' only)
 *   - # (optional):         Alternate form (see ALTERNATE FORM below)
 *   - width (optional):     Minimum field width (default: 0)
 *   - precision (optional): Decimal places for floats (default: 6)
 *   - type (required):      Format type character (see TYPE CHARACTERS)
 *   - size (required):      Size modifier (see SIZE MODIFIERS)
 *
 * ALTERNATE FORM (#):
 *   - Hex (x, X):     Adds '0x' prefix (e.g., 0xff, 0x0)
 *   - Binary (b):     Adds '0b' prefix (e.g., 0b101, 0b0)
 *   - Octal (o):      Adds '0' prefix (e.g., 0100, 00)
 *   - Float (f, F):   Always show decimal point even for whole numbers
 *   - Pointer (p, P): Adds '0x' prefix (e.g., 0x12345678, 0x0)
 *
 * TYPE CHARACTERS:
 *   - Hex:                    x (lower), X (upper)
 *   - Octal:                  o
 *   - Binary:                 b
 *   - Boolean:                B
 *   - Unsigned:               u
 *   - Signed:                 d, i
 *   - Float:                  f (str lower), F (str upper)
 *
 *   - Pointer:                p (hex lower), P (hex upper)
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
 *   - {:?T}     → Optional full: value or "none"
 *   - {:?0}     → O$void: "some" or "none" (same as {:?})
 *
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
 *    - {:d}       → i32 (42)
 *    - {:dl}      → i64 (9223372036854775807)
 *    - {:uhh}     → u8 (255)
 *    - {:uz}      → usize
 *    - {:.2fl}    → f64 with 2 decimals (123.46)
 *    - {:+d}      → i32 with sign (+42)
 *    - {: d}      → i32 with space for positive ( 42)
 *    - {:#x}      → u32 hex with prefix (0xff, 0x0 for zero)
 *    - {:#X}      → u32 hex upper with prefix (0xFF) [digits uppercase, 0x lowercase]
 *    - {:#b}      → u32 binary with prefix (0b101, 0b0 for zero)
 *    - {:08x}     → u32 hex, zero-padded, width 8 (0000002a)
 *    - {:p}       → pointer without prefix (a3b4c5d6, 0 for null)
 *    - {:#p}      → pointer with prefix (0xa3b4c5d6, 0x0 for null)
 *    - {0:d}      → i32, argument 0 (indexed)
 *    - {:?d}      → O$i32 (optional)
 *    - {:!s}      → E$S_const$u8 (result)
 *    - {:e}       → Err (error object)
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

typedef enum fmt_Align : u8 {
    fmt_Align_left = 0,
    fmt_Align_center = 1,
    fmt_Align_right = 2
} fmt_Align;
T_use_O$(fmt_Align);
$static let fmt_Align_default = fmt_Align_left;
$static let_(fmt_Align_values, A$$(3, u8)) = init$A({
    [fmt_Align_left] = u8_c('<'),
    [fmt_Align_center] = u8_c('^'),
    [fmt_Align_right] = u8_c('>'),
});
$extern fn_((fmt_Align_parse(u8 ch))(O$fmt_Align));

typedef enum fmt_TypePrefix : u8 {
    fmt_TypePrefix_optional = 0,
    fmt_TypePrefix_error_result = 1,
} fmt_TypePrefix;
T_use_O$(fmt_TypePrefix);
$static let_(fmt_TypePrefix_values, A$$(2, u8)) = init$A({
    [fmt_TypePrefix_optional] = u8_c('?'),
    [fmt_TypePrefix_error_result] = u8_c('!'),
});
$extern fn_((fmt_TypePrefix_parse(u8 ch))(O$fmt_TypePrefix));

typedef enum fmt_Type : u8 {
    fmt_Type_hex_lower = 0,
    fmt_Type_hex_upper,
    fmt_Type_octal,
    fmt_Type_binary,
    fmt_Type_boolean,
    fmt_Type_unsigned,
    fmt_Type_signed,
    fmt_Type_float_lower,
    fmt_Type_float_upper,
    fmt_Type_pointer_lower,
    fmt_Type_pointer_upper,
    fmt_Type_error,
    fmt_Type_void,
    fmt_Type_ascii_code,
    fmt_Type_utf8_codepoint,
    fmt_Type_string_z0,
    fmt_Type_string_s,
} fmt_Type;
T_use_O$(fmt_Type);
$static let fmt_Type_values = A_from$((S_const$u8){
    [fmt_Type_hex_lower] = u8_l("x"),
    [fmt_Type_hex_upper] = u8_l("X"),
    [fmt_Type_octal] = u8_l("o"),
    [fmt_Type_binary] = u8_l("b"),
    [fmt_Type_boolean] = u8_l("B"),
    [fmt_Type_unsigned] = u8_l("u"),
    [fmt_Type_signed] = u8_l("di"),
    [fmt_Type_float_lower] = u8_l("f"),
    [fmt_Type_float_upper] = u8_l("F"),
    [fmt_Type_pointer_lower] = u8_l("p"),
    [fmt_Type_pointer_upper] = u8_l("P"),
    [fmt_Type_error] = u8_l("e"),
    [fmt_Type_void] = u8_l("0"),
    [fmt_Type_ascii_code] = u8_l("c"),
    [fmt_Type_utf8_codepoint] = u8_l("C"),
    [fmt_Type_string_z0] = u8_l("z"),
    [fmt_Type_string_s] = u8_l("s"),
});
$extern fn_((fmt_Type_parse(S_const$u8 str))(O$fmt_Type));

typedef enum fmt_Size : u8 {
    fmt_Size_8 = 0,
    fmt_Size_16 = 1,
    fmt_Size_32 = 2,
    fmt_Size_64 = 3,
    fmt_Size_128 = 4,
    fmt_Size_ptr = 5,
} fmt_Size;
T_use_O$(fmt_Size);
$static let fmt_Size_default = fmt_Size_32;
$static let fmt_Size_values_default = u8_l("");
$static let fmt_Size_values = A_from$((S_const$u8){
    [fmt_Size_8] = u8_l("hh"),
    [fmt_Size_16] = u8_l("h"),
    [fmt_Size_32] = fmt_Size_values_default,
    [fmt_Size_64] = u8_l("l"),
    [fmt_Size_128] = u8_l("ll"),
    [fmt_Size_ptr] = u8_l("z"),
});
$extern fn_((fmt_Size_parse(S_const$u8 str))(O$fmt_Size));

/// Sign display mode for numeric formatting
typedef enum fmt_Sign : u8 {
    fmt_Sign_auto = 0,   // Only show sign for negative numbers (default)
    fmt_Sign_always, // Always show sign (+ or -)
    fmt_Sign_space,  // Show space for positive, - for negative
} fmt_Sign;

typedef struct fmt_Spec {
    O$u8 index;
    O$u8 fill;
    O$fmt_Align align;
    fmt_Sign sign;
    bool alt_form;
    O$u8 width;
    O$u8 precision;
    O$fmt_TypePrefix type_prefix;
    fmt_Type type;
    fmt_Size size;
} fmt_Spec;

/// Maximum number of arguments
#define fmt_max_args (16ull)

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

$extern fn_((fmt_parseIInt(S_const$u8 str, u8 base))(E$i64)) $must_check;
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

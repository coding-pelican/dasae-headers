/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    test-fmt_parse.c
 * @author  Gyeongtae Kim(dev-dasae) & AI Assistant
 * @date    2025-06-15 (date of creation)
 * @updated 2025-06-15 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/tests
 * @prefix  test
 *
 * @brief   Unit tests for fmt parse functions.
 */

#include "dh/main.h"
#include "dh/fmt/common.h"
#include "dh/math/common.h"

/*========== Unsigned Integer Parsing Tests =================================*/

TEST_fn_("fmt_parse$u64: Basic and different bases" $scope) {
    // Basic decimal
    let res1 = try_(fmt_parse$u64(u8_l("12345"), 10));
    try_(TEST_expect(res1 == 12345ULL));

    // Hexadecimal
    let res2 = try_(fmt_parse$u64(u8_l("ABCD"), 16));
    try_(TEST_expect(res2 == 0xABCDULL));

    // Binary
    let res3 = try_(fmt_parse$u64(u8_l("110110"), 2));
    try_(TEST_expect(res3 == 54ULL));

    // Octal
    let res4 = try_(fmt_parse$u64(u8_l("777"), 8));
    try_(TEST_expect(res4 == 511ULL));

    // Zero
    let res5 = try_(fmt_parse$u64(u8_l("0"), 10));
    try_(TEST_expect(res5 == 0ULL));

    // Positive sign
    let res6 = try_(fmt_parse$u64(u8_l("+42"), 10));
    try_(TEST_expect(res6 == 42ULL));
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse$u64: Edge cases and errors" $scope) {
    // Max value
    let res_max = try_(fmt_parse$u64(u8_l("18446744073709551615"), 10));
    try_(TEST_expect(res_max == u64_limit_max));

    // Overflow
    let res_overflow = fmt_parse$u64(u8_l("18446744073709551616"), 10);
    try_(TEST_expect(isErr(res_overflow)));

    // Empty string
    let res_empty = fmt_parse$u64(u8_l(""), 10);
    try_(TEST_expect(isErr(res_empty)));

    // Invalid characters
    let res_invalid = fmt_parse$u64(u8_l("12a34"), 10);
    try_(TEST_expect(isErr(res_invalid)));

    // Just a sign
    let res_sign = fmt_parse$u64(u8_l("+"), 10);
    try_(TEST_expect(isErr(res_sign)));
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse$u32: Edge cases and errors" $scope) {
    // Max value
    let res_max = try_(fmt_parse$u32(u8_l("4294967295"), 10));
    try_(TEST_expect(res_max == u32_limit_max));

    // Overflow
    let res_overflow = fmt_parse$u32(u8_l("4294967296"), 10);
    try_(TEST_expect(isErr(res_overflow)));
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse$usize: Basic test" $scope) {
    // A reasonably large number that should fit in usize
    let res = try_(fmt_parse$usize(u8_l("12345678"), 10));
    try_(TEST_expect(res == 12345678UL));
} $unscoped_(TEST_fn);

/*========== Signed Integer Parsing Tests ===================================*/

TEST_fn_("fmt_parse$i64: Basic and signs" $scope) {
    // Positive
    let res1 = try_(fmt_parse$i64(u8_l("12345"), 10));
    try_(TEST_expect(res1 == 12345LL));

    // Negative
    let res2 = try_(fmt_parse$i64(u8_l("-54321"), 10));
    try_(TEST_expect(res2 == -54321LL));

    // Positive with sign
    let res3 = try_(fmt_parse$i64(u8_l("+99"), 10));
    try_(TEST_expect(res3 == 99LL));
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse$i64: Edge cases and errors" $scope) {
    // Max value
    let res_max = try_(fmt_parse$i64(u8_l("9223372036854775807"), 10));
    try_(TEST_expect(res_max == i64_limit_max));

    // Min value
    let res_min = try_(fmt_parse$i64(u8_l("-9223372036854775808"), 10));
    try_(TEST_expect(res_min == i64_limit_min));

    // Overflow
    let res_overflow = fmt_parse$i64(u8_l("9223372036854775808"), 10);
    try_(TEST_expect(isErr(res_overflow)));

    // Underflow
    let res_underflow = fmt_parse$i64(u8_l("-9223372036854775809"), 10);
    try_(TEST_expect(isErr(res_underflow)));
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse$i32: Edge cases and errors" $scope) {
    // Max value
    let res_max = try_(fmt_parse$i32(u8_l("2147483647"), 10));
    try_(TEST_expect(res_max == i32_limit_max));

    // Min value
    let res_min = try_(fmt_parse$i32(u8_l("-2147483648"), 10));
    try_(TEST_expect(res_min == i32_limit_min));

    // Overflow
    let res_overflow = fmt_parse$i32(u8_l("2147483648"), 10);
    try_(TEST_expect(isErr(res_overflow)));

    // Underflow
    let res_underflow = fmt_parse$i32(u8_l("-2147483649"), 10);
    try_(TEST_expect(isErr(res_underflow)));
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse$isize: Basic test" $scope) {
    let res_pos = try_(fmt_parse$isize(u8_l("12345"), 10));
    try_(TEST_expect(res_pos == 12345L));
    let res_neg = try_(fmt_parse$isize(u8_l("-54321"), 10));
    try_(TEST_expect(res_neg == -54321L));
} $unscoped_(TEST_fn);

/*========== Floating Point Parsing Tests ===================================*/

TEST_fn_("fmt_parse$f64: Basic and scientific notation" $scope) {
    let epsilon = 1e-9;

    // Integer
    let res1 = try_(fmt_parse$f64(u8_l("123")));
    try_(TEST_expect(math_abs(res1 - 123.0) < epsilon));

    // Decimal
    let res2 = try_(fmt_parse$f64(u8_l("-123.456")));
    try_(TEST_expect(math_abs(res2 - (-123.456)) < epsilon));

    // Positive sign
    let res3 = try_(fmt_parse$f64(u8_l("+98.76")));
    try_(TEST_expect(math_abs(res3 - 98.76) < epsilon));

    // Leading dot
    let res4 = try_(fmt_parse$f64(u8_l(".5")));
    try_(TEST_expect(math_abs(res4 - 0.5) < epsilon));

    // Trailing dot
    let res5 = try_(fmt_parse$f64(u8_l("5.")));
    try_(TEST_expect(math_abs(res5 - 5.0) < epsilon));

    // Scientific notation
    let res6 = try_(fmt_parse$f64(u8_l("1.23e2")));
    try_(TEST_expect(math_abs(res6 - 123.0) < epsilon));

    // Scientific notation with negative exponent
    let res7 = try_(fmt_parse$f64(u8_l("1.23e-2")));
    try_(TEST_expect(math_abs(res7 - 0.0123) < epsilon));
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse$f64: Errors" $scope) {
    // Empty
    try_(TEST_expect(isErr(fmt_parse$f64(u8_l("")))));

    // Invalid format
    try_(TEST_expect(isErr(fmt_parse$f64(u8_l("1.2.3")))));
    try_(TEST_expect(isErr(fmt_parse$f64(u8_l("1.23e")))));
    try_(TEST_expect(isErr(fmt_parse$f64(u8_l("e5")))));
    try_(TEST_expect(isErr(fmt_parse$f64(u8_l("1.23e+")))));
    try_(TEST_expect(isErr(fmt_parse$f64(u8_l(".")))));
    try_(TEST_expect(isErr(fmt_parse$f64(u8_l("1a")))));
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse$f32: Basic test" $scope) {
    let epsilon = 1e-6f;
    let res     = try_(fmt_parse$f32(u8_l("12.34")));
    try_(TEST_expect(math_abs(res - 12.34f) < epsilon));
} $unscoped_(TEST_fn);

/*========== Boolean Parsing Tests ==========================================*/

TEST_fn_("fmt_parse$bool: Valid values" $scope) {
    // True values
    try_(TEST_expect(try_(fmt_parse$bool(u8_l("true")))));
    try_(TEST_expect(try_(fmt_parse$bool(u8_l("TRUE")))));
    try_(TEST_expect(try_(fmt_parse$bool(u8_l("1")))));

    // False values
    try_(TEST_expect(!try_(fmt_parse$bool(u8_l("false")))));
    try_(TEST_expect(!try_(fmt_parse$bool(u8_l("FALSE")))));
    try_(TEST_expect(!try_(fmt_parse$bool(u8_l("0")))));
} $unscoped_(TEST_fn);

TEST_fn_("fmt_parse$bool: Errors" $scope) {
    // Empty
    try_(TEST_expect(isErr(fmt_parse$bool(u8_l("")))));

    // Invalid values
    try_(TEST_expect(isErr(fmt_parse$bool(u8_l("t")))));
    try_(TEST_expect(isErr(fmt_parse$bool(u8_l("yes")))));
    try_(TEST_expect(isErr(fmt_parse$bool(u8_l("2")))));
} $unscoped_(TEST_fn);

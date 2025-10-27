/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    test-ascii.c
 * @author  Gyeongtae Kim(dev-dasae)
 * @date    2025-06-21 (date of creation)
 * @updated 2025-06-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/tests
 * @prefix  test
 *
 * @brief   Unit tests for ASCII functions.
 */

#include "dh/main.h"
#include "dh/ascii.h"
#include "dh/Arr.h"
#include "dh/Str.h"
#include "dh/opt.h"

//==============================================================================
// CASE CONVERSION TESTS
//==============================================================================

TEST_fn_("ascii_toUppers: In-place uppercase conversion" $scope) {
    // Test normal case
    use_A$(20, u8);
    A$20$u8 test_mem = A_zero();
    bti_memcpy(test_mem.val, "hello world", 11);
    let test_str = Sli_from$(S$u8, test_mem.val, 11);
    let result   = ascii_toUppers(test_str);
    try_(TEST_expect(Str_eql(Str_view(result.ptr, result.len), u8_l("HELLO WORLD"))));

    // Test already uppercase
    bti_memcpy(test_mem.val, "HELLO", 5);
    let upper_str    = Sli_from$(S$u8, test_mem.val, 5);
    let upper_result = ascii_toUppers(upper_str);
    try_(TEST_expect(Str_eql(Str_view(upper_result.ptr, upper_result.len), u8_l("HELLO"))));

    // Test mixed case with numbers
    bti_memcpy(test_mem.val, "Test123", 7);
    let mixed_str    = Sli_from$(S$u8, test_mem.val, 7);
    let mixed_result = ascii_toUppers(mixed_str);
    try_(TEST_expect(Str_eql(Str_view(mixed_result.ptr, mixed_result.len), u8_l("TEST123"))));
} $unscoped_(TEST_fn);

TEST_fn_("ascii_toLowers: In-place lowercase conversion" $scope) {
    // Test normal case
    use_A$(20, u8);
    A$20$u8 test_mem = A_zero();
    bti_memcpy(test_mem.val, "HELLO WORLD", 11);
    let test_str = Sli_from$(S$u8, test_mem.val, 11);
    let result   = ascii_toLowers(test_str);
    try_(TEST_expect(Str_eql(Str_view(result.ptr, result.len), u8_l("hello world"))));

    // Test already lowercase
    bti_memcpy(test_mem.val, "hello", 5);
    let lower_str    = Sli_from$(S$u8, test_mem.val, 5);
    let lower_result = ascii_toLowers(lower_str);
    try_(TEST_expect(Str_eql(Str_view(lower_result.ptr, lower_result.len), u8_l("hello"))));

    // Test mixed case with symbols
    bti_memcpy(test_mem.val, "Test@123", 8);
    let mixed_str    = Sli_from$(S$u8, test_mem.val, 8);
    let mixed_result = ascii_toLowers(mixed_str);
    try_(TEST_expect(Str_eql(Str_view(mixed_result.ptr, mixed_result.len), u8_l("test@123"))));
} $unscoped_(TEST_fn);

TEST_fn_("ascii_makeUppers: Copy to buffer with uppercase" $scope) {
    // Test normal case
    let source = u8_l("hello");
    use_A$(10, u8);
    A$10$u8 buffer_mem = A_zero();
    let     buffer     = Sli_from$(S$u8, buffer_mem.val, 10);
    let     result     = ascii_makeUppers(buffer, source);
    try_(TEST_expect(Str_eql(Str_view(result.ptr, result.len), u8_l("HELLO"))));
    try_(TEST_expect(result.len == 5));
} $unscoped_(TEST_fn);

TEST_fn_("ascii_makeLowers: Copy to buffer with lowercase" $scope) {
    // Test normal case
    let source = u8_l("WORLD");
    use_A$(10, u8);
    A$10$u8 buffer_mem = A_zero();
    let     buffer     = Sli_from$(S$u8, buffer_mem.val, 10);
    let     result     = ascii_makeLowers(buffer, source);
    try_(TEST_expect(Str_eql(Str_view(result.ptr, result.len), u8_l("world"))));
    try_(TEST_expect(result.len == 5));
} $unscoped_(TEST_fn);

//==============================================================================
// SEARCH FUNCTION TESTS
//==============================================================================

TEST_fn_("ascii_idxOfIgnoreCase: Case-insensitive substring search" $scope) {
    let haystack = u8_l("Hello World");

    // Test found at beginning
    let result1 = ascii_idxOfIgnoreCase(haystack, u8_l("HELLO"));
    try_(TEST_expect(isSome(result1)));
    try_(TEST_expect(result1.value == 0));

    // Test found in middle
    let result2 = ascii_idxOfIgnoreCase(haystack, u8_l("world"));
    try_(TEST_expect(isSome(result2)));
    try_(TEST_expect(result2.value == 6));

    // Test not found
    let result3 = ascii_idxOfIgnoreCase(haystack, u8_l("xyz"));
    try_(TEST_expect(isNone(result3)));

    // Test empty substring
    let result4 = ascii_idxOfIgnoreCase(haystack, u8_l(""));
    try_(TEST_expect(isSome(result4)));
    try_(TEST_expect(result4.value == 0));

    // Test substring longer than haystack
    let result5 = ascii_idxOfIgnoreCase(u8_l("Hi"), u8_l("Hello"));
    try_(TEST_expect(isNone(result5)));
} $unscoped_(TEST_fn);

TEST_fn_("ascii_idxFirstOfIgnoreCase: Search from specific position" $scope) {
    let haystack = u8_l("Hello Hello World");

    // Test found from start
    let result1 = ascii_idxFirstOfIgnoreCase(haystack, u8_l("hello"), 0);
    try_(TEST_expect(isSome(result1)));
    try_(TEST_expect(result1.value == 0));

    // Test found from middle
    let result2 = ascii_idxFirstOfIgnoreCase(haystack, u8_l("hello"), 6);
    try_(TEST_expect(isSome(result2)));
    try_(TEST_expect(result2.value == 6));

    // Test not found from late start
    let result3 = ascii_idxFirstOfIgnoreCase(haystack, u8_l("hello"), 12);
    try_(TEST_expect(isNone(result3)));
} $unscoped_(TEST_fn);

TEST_fn_("ascii_idxLastOfIgnoreCase: Search backwards from position" $scope) {
    let haystack = u8_l("Hello Hello World");

    // Test found from end
    let result1 = ascii_idxLastOfIgnoreCase(haystack, u8_l("hello"), 16);
    try_(TEST_expect(isSome(result1)));
    try_(TEST_expect(unwrap(result1) == 6)); // Last occurrence

    // Test found from middle
    let result2 = ascii_idxLastOfIgnoreCase(haystack, u8_l("hello"), 10);
    try_(TEST_expect(isSome(result2)));
    try_(TEST_expect(unwrap(result2) == 6));

    // Test first occurrence within range
    let result3 = ascii_idxLastOfIgnoreCase(haystack, u8_l("hello"), 4);
    try_(TEST_expect(isSome(result3)));
    try_(TEST_expect(unwrap(result3) == 0));
} $unscoped_(TEST_fn);

//==============================================================================
// PREFIX/SUFFIX TESTS
//==============================================================================

TEST_fn_("ascii_startsWithIgnoreCase: Case-insensitive prefix check" $scope) {
    let text = u8_l("Hello World");

    // Test matching prefix
    try_(TEST_expect(ascii_startsWithIgnoreCase(text, u8_l("hello"))));
    try_(TEST_expect(ascii_startsWithIgnoreCase(text, u8_l("HELLO"))));
    try_(TEST_expect(ascii_startsWithIgnoreCase(text, u8_l("Hello"))));

    // Test non-matching prefix
    try_(TEST_expect(!ascii_startsWithIgnoreCase(text, u8_l("world"))));
    try_(TEST_expect(!ascii_startsWithIgnoreCase(text, u8_l("hi"))));

    // Test empty prefix
    try_(TEST_expect(ascii_startsWithIgnoreCase(text, u8_l(""))));

    // Test prefix longer than string
    try_(TEST_expect(!ascii_startsWithIgnoreCase(u8_l("Hi"), u8_l("Hello"))));

    // Test exact match
    try_(TEST_expect(ascii_startsWithIgnoreCase(text, text)));
} $unscoped_(TEST_fn);

TEST_fn_("ascii_endsWithIgnoreCase: Case-insensitive suffix check" $scope) {
    let text = u8_l("Hello World");

    // Test matching suffix
    try_(TEST_expect(ascii_endsWithIgnoreCase(text, u8_l("world"))));
    try_(TEST_expect(ascii_endsWithIgnoreCase(text, u8_l("WORLD"))));
    try_(TEST_expect(ascii_endsWithIgnoreCase(text, u8_l("World"))));

    // Test non-matching suffix
    try_(TEST_expect(!ascii_endsWithIgnoreCase(text, u8_l("hello"))));
    try_(TEST_expect(!ascii_endsWithIgnoreCase(text, u8_l("test"))));

    // Test empty suffix
    try_(TEST_expect(ascii_endsWithIgnoreCase(text, u8_l(""))));

    // Test suffix longer than string
    try_(TEST_expect(!ascii_endsWithIgnoreCase(u8_l("Hi"), u8_l("World"))));

    // Test exact match
    try_(TEST_expect(ascii_endsWithIgnoreCase(text, text)));
} $unscoped_(TEST_fn);

//==============================================================================
// COMPARISON TESTS
//==============================================================================

TEST_fn_("ascii_cmpSenseCase: Case-sensitive string comparison" $scope) {
    // Test equal strings
    try_(TEST_expect(ascii_cmpSenseCase(u8_l("hello"), u8_l("hello")) == cmp_Ord_eq));

    // Test lexicographical order
    try_(TEST_expect(ascii_cmpSenseCase(u8_l("apple"), u8_l("banana")) == cmp_Ord_lt));
    try_(TEST_expect(ascii_cmpSenseCase(u8_l("banana"), u8_l("apple")) == cmp_Ord_gt));

    // Test case sensitivity
    try_(TEST_expect(ascii_cmpSenseCase(u8_l("Apple"), u8_l("apple")) == cmp_Ord_lt));
    try_(TEST_expect(ascii_cmpSenseCase(u8_l("apple"), u8_l("Apple")) == cmp_Ord_gt));

    // Test length differences
    try_(TEST_expect(ascii_cmpSenseCase(u8_l("hello"), u8_l("hello world")) == cmp_Ord_lt));
    try_(TEST_expect(ascii_cmpSenseCase(u8_l("hello world"), u8_l("hello")) == cmp_Ord_gt));

    // Test empty strings
    try_(TEST_expect(ascii_cmpSenseCase(u8_l(""), u8_l("")) == cmp_Ord_eq));
    try_(TEST_expect(ascii_cmpSenseCase(u8_l(""), u8_l("hello")) == cmp_Ord_lt));
    try_(TEST_expect(ascii_cmpSenseCase(u8_l("hello"), u8_l("")) == cmp_Ord_gt));
} $unscoped_(TEST_fn);

TEST_fn_("ascii_cmpIgnoreCase: Case-insensitive string comparison" $scope) {
    // Test equal strings with different cases
    try_(TEST_expect(ascii_cmpIgnoreCase(u8_l("Hello"), u8_l("hello")) == cmp_Ord_eq));
    try_(TEST_expect(ascii_cmpIgnoreCase(u8_l("HELLO"), u8_l("hello")) == cmp_Ord_eq));

    // Test lexicographical order ignoring case
    try_(TEST_expect(ascii_cmpIgnoreCase(u8_l("Apple"), u8_l("banana")) == cmp_Ord_lt));
    try_(TEST_expect(ascii_cmpIgnoreCase(u8_l("BANANA"), u8_l("apple")) == cmp_Ord_gt));

    // Test mixed case
    try_(TEST_expect(ascii_cmpIgnoreCase(u8_l("HeLLo"), u8_l("hello")) == cmp_Ord_eq));
} $unscoped_(TEST_fn);

TEST_fn_("ascii_cmp: Generic comparison with case flag" $scope) {
    // Test with ignores_case = false
    try_(TEST_expect(ascii_cmp(u8_l("Hello"), u8_l("hello"), false) == cmp_Ord_lt));
    try_(TEST_expect(ascii_cmp(u8_l("hello"), u8_l("hello"), false) == cmp_Ord_eq));

    // Test with ignores_case = true
    try_(TEST_expect(ascii_cmp(u8_l("Hello"), u8_l("hello"), true) == cmp_Ord_eq));
    try_(TEST_expect(ascii_cmp(u8_l("HELLO"), u8_l("hello"), true) == cmp_Ord_eq));
} $unscoped_(TEST_fn);

//==============================================================================
// EQUALITY TESTS
//==============================================================================

TEST_fn_("ascii_eqlSenseCase: Case-sensitive string equality" $scope) {
    // Test equal strings
    try_(TEST_expect(ascii_eqlSenseCase(u8_l("hello"), u8_l("hello"))));
    try_(TEST_expect(ascii_eqlSenseCase(u8_l("HELLO"), u8_l("HELLO"))));

    // Test case sensitivity
    try_(TEST_expect(!ascii_eqlSenseCase(u8_l("Hello"), u8_l("hello"))));
    try_(TEST_expect(!ascii_eqlSenseCase(u8_l("HELLO"), u8_l("hello"))));

    // Test different strings
    try_(TEST_expect(!ascii_eqlSenseCase(u8_l("hello"), u8_l("world"))));

    // Test different lengths
    try_(TEST_expect(!ascii_eqlSenseCase(u8_l("hello"), u8_l("hello world"))));

    // Test empty strings
    try_(TEST_expect(ascii_eqlSenseCase(u8_l(""), u8_l(""))));
    try_(TEST_expect(!ascii_eqlSenseCase(u8_l(""), u8_l("hello"))));
} $unscoped_(TEST_fn);

TEST_fn_("ascii_eqlIgnoreCase: Case-insensitive string equality" $scope) {
    // Test equal strings with different cases
    try_(TEST_expect(ascii_eqlIgnoreCase(u8_l("Hello"), u8_l("hello"))));
    try_(TEST_expect(ascii_eqlIgnoreCase(u8_l("HELLO"), u8_l("hello"))));
    try_(TEST_expect(ascii_eqlIgnoreCase(u8_l("HeLLo"), u8_l("hello"))));

    // Test same case
    try_(TEST_expect(ascii_eqlIgnoreCase(u8_l("hello"), u8_l("hello"))));

    // Test different strings
    try_(TEST_expect(!ascii_eqlIgnoreCase(u8_l("hello"), u8_l("world"))));

    // Test different lengths
    try_(TEST_expect(!ascii_eqlIgnoreCase(u8_l("hello"), u8_l("hello world"))));

    // Test with numbers and symbols
    try_(TEST_expect(ascii_eqlIgnoreCase(u8_l("Test123!"), u8_l("test123!"))));
} $unscoped_(TEST_fn);

TEST_fn_("ascii_eql: Generic equality with case flag" $scope) {
    // Test with ignores_case = false
    try_(TEST_expect(ascii_eql(u8_l("hello"), u8_l("hello"), false)));
    try_(TEST_expect(!ascii_eql(u8_l("Hello"), u8_l("hello"), false)));

    // Test with ignores_case = true
    try_(TEST_expect(ascii_eql(u8_l("Hello"), u8_l("hello"), true)));
    try_(TEST_expect(ascii_eql(u8_l("HELLO"), u8_l("hello"), true)));
} $unscoped_(TEST_fn);

//==============================================================================
// EDGE CASES AND SPECIAL CHARACTERS
//==============================================================================

TEST_fn_("ascii functions: Edge cases and special characters" $scope) {
    // Test with special ASCII characters
    let special_chars = u8_l("!@#$%^&*()");

    // Case conversion should not affect non-alphabetic characters
    use_A$(20, u8);
    A$20$u8 test_mem = A_zero();
    bti_memcpy(test_mem.val, special_chars.ptr, special_chars.len);
    let special_slice = Sli_from$(S$u8, test_mem.val, special_chars.len);

    let upper_result = ascii_toUppers(special_slice);
    try_(TEST_expect(Str_eql(Str_view(upper_result.ptr, upper_result.len), special_chars)));

    bti_memcpy(test_mem.val, special_chars.ptr, special_chars.len);
    let lower_result = ascii_toLowers(special_slice);
    try_(TEST_expect(Str_eql(Str_view(lower_result.ptr, lower_result.len), special_chars)));

    // Test equality with special characters
    try_(TEST_expect(ascii_eqlSenseCase(special_chars, special_chars)));
    try_(TEST_expect(ascii_eqlIgnoreCase(special_chars, special_chars)));

    // Test comparison with special characters
    try_(TEST_expect(ascii_cmpSenseCase(special_chars, special_chars) == cmp_Ord_eq));
    try_(TEST_expect(ascii_cmpIgnoreCase(special_chars, special_chars) == cmp_Ord_eq));
} $unscoped_(TEST_fn);

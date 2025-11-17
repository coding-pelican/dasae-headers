/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    io_Writer-print.c
 * @author  Gyeongtae Kim(dev-dasae)
 * @date    2025-06-06 (date of creation)
 * @updated 2025-06-06 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/tests
 * @prefix  test
 *
 * @brief   Unit tests for io_Writer print functions with basic format
 * @details Tests for io_Writer_print and related internal formatting functionality
 */

/*========== Includes =======================================================*/

#include "dh/main.h"
#include "dh/io/Writer.h"
#include "dh/fmt/common.h"
#include "dh/Str.h"
#include <stdio.h>

/*========== Test Helper - Buffer Writer ===================================*/

typedef struct test_Buf {
    S$u8 data;
    usize pos;
} test_Buf;
$must_check
$static fn_((test_Buf_VT_write(const P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    let self      = as$(test_Buf*)(ctx);
    let remaining = self->data.len - self->pos;
    let to_write  = prim_min(bytes.len, remaining);
    if (0 < to_write) {
        prim_memcpy(self->data.ptr + self->pos, bytes.ptr, to_write);
        self->pos += to_write;
    }
    return_ok(to_write);
} $unscoped_(fn);
$static fn_((test_Buf_init(S$u8 data))(test_Buf)) {
    return (test_Buf){
        .data = data,
        .pos  = 0
    };
}
$static fn_((test_Buf_writer(test_Buf* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return lit$((io_Writer){ .ctx = self, .write = test_Buf_VT_write });
}
$static fn_((test_Buf_clear(test_Buf* self))(void)) {
    claim_assert_nonnull(self);
    self->pos = 0;
}
$static fn_((test_Buf_view(test_Buf self))(S_const$u8)) {
    if (self.pos == 0) { return zeroS$((const u8)); }
    return sliceS(self.data, $r(0, self.pos)).as_const;
}
$maybe_unused
$static fn_((test_Buf_take(test_Buf* self, S_const$u8 data))(bool)) {
    claim_assert_nonnull(self);
    if (self->data.len < data.len) { return false; }
    prim_memcpy(self->data.ptr, data.ptr, data.len);
    self->pos = data.len;
    return true;
}
$maybe_unused
$static fn_((test_Buf_give(test_Buf* self, S$u8 output))(O$S$u8) $scope) {
    claim_assert_nonnull(self);
    if (output.len < self->pos) { return_none(); }
    prim_memcpy(output.ptr, self->data.ptr, self->pos);
    self->pos = 0;
    return_some(output);
} $unscoped_(fn);
$must_check $maybe_unused
$static fn_((test_Buf_giveAlloc(test_Buf* self, mem_Allocator allocator))(E$S$u8) $scope) {
    claim_assert_nonnull(self);
    let out = try_(u_castE$((E$S$u8)(mem_Allocator_alloc(allocator, typeInfo$(u8), self->pos))));
    prim_memcpy(out.ptr, self->data.ptr, self->pos);
    self->pos = 0;
    return_ok(out);
} $unscoped_(fn);

/*========== Basic Format Type Tests =======================================*/

TEST_fn_("io_Writer-print: Integer formatting - decimal" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf    = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test basic integers
    try_(io_Writer_print(writer, u8_l("{:d}"), 42));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("42"))));
    }

    // Reset mem
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:d}"), -123));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("-123"))));
    }

    // Test with 64-bit
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:dl}"), 9223372036854775807LL));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("9223372036854775807"))));
    }

    // Test with isize
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:dz}"), as$(isize)(12345)));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("12345"))));
    }
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: Unsigned integer formatting" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test basic unsigned
    try_(io_Writer_print(writer, u8_l("{:u}"), 42U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("42"))));
    }

    // Test maximum values
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:u}"), UINT32_MAX));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("4294967295"))));
    }

    // Test 64-bit unsigned
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:ul}"), 18446744073709551615ULL));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("18446744073709551615"))));
    }
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: Hexadecimal formatting" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test lowercase hex
    try_(io_Writer_print(writer, u8_l("{:x}"), 255U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("ff"))));
    }

    // Test uppercase hex
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:X}"), 255U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("FF"))));
    }

    // Test with alternate form
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:#x}"), 255U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("0xff"))));
    }

    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:#X}"), 255U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("0xFF")))); // 0x is always lowercase
    }
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: Binary and Octal formatting" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test binary
    try_(io_Writer_print(writer, u8_l("{:b}"), 5U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("101"))));
    }

    // Test binary with alternate form
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:#b}"), 5U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("0b101"))));
    }

    // Test octal
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:o}"), 64U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("100"))));
    }

    // Test octal with alternate form
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:#o}"), 64U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("0100"))));
    }
} $unscoped_(TEST_fn);

/*========== Character and String Tests =====================================*/

TEST_fn_("io_Writer-print: Character formatting" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test basic character
    try_(io_Writer_print(writer, u8_l("{:c}"), 'A'));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("A"))));
    }

    // Test with numbers (should cast to char)
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:c}"), 65));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("A"))));
    }
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: String slice formatting" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test basic string slice
    let test_str = u8_l("Hello, World!");
    try_(io_Writer_print(writer, u8_l("{:s}"), test_str));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("Hello, World!"))));
    }
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: Null-terminated string formatting" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test basic null-terminated string
    const char* test_str = "Hello, C!";
    try_(io_Writer_print(writer, u8_l("{:z}"), test_str));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("Hello, C!"))));
    }
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: Pointer formatting" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test null pointer (without prefix)
    try_(io_Writer_print(writer, u8_l("{:p}"), null));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("0"))));
    }

    // Test non-null pointer without prefix (value will vary, just check format)
    test_Buf_clear(&buf);
    i32 dummy = 42;
    try_(io_Writer_print(writer, u8_l("{:p}"), &dummy));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(result.len >= 1)); // At least one hex digit
        // Should not have 0x prefix
        if (result.len >= 2) {
            try_(TEST_expect(!(result.ptr[0] == '0' && result.ptr[1] == 'x')));
        }
    }

    // Test pointer with alternate form (adds 0x prefix)
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:#p}"), &dummy));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(result.len >= 3)); // "0x" + at least one digit
        try_(TEST_expect(result.ptr[0] == '0'));
        try_(TEST_expect(result.ptr[1] == 'x'));
    }

    // Test null pointer with alternate form (should add prefix even for zero)
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:#p}"), null));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("0x0"))));
    }
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: Floating point formatting" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Basic float
    try_(io_Writer_print(writer, u8_l("{:fl}"), 123.456));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("123.456000")))); // Default precision is 6
    }

    // Precision
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:.2f}"), 123.456));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("123.46")))); // Should round
    }

    // Negative float
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:f}"), -0.123));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("-0.123000"))));
    }

    // Special values
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:f} {:F}"), f32_nan, f32_nan));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("nan NAN"))));
    }

    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:f} {:F}"), f32_inf, f32_inf));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("inf INF"))));
    }
} $unscoped_(TEST_fn);

/*========== Format Flag Tests ==============================================*/

TEST_fn_("io_Writer-print: Sign flags" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test + flag for positive numbers
    try_(io_Writer_print(writer, u8_l("{:+d}"), 42));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("+42"))));
    }

    // Test + flag for negative numbers
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:+d}"), -42));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("-42"))));
    }

    // Test space flag for positive numbers
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{: d}"), 42));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l(" 42"))));
    }

    // Test space flag for negative numbers
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{: d}"), -42));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("-42"))));
    }
} $unscoped_(TEST_fn);

/*========== Special Cases and Edge Tests ===================================*/

TEST_fn_("io_Writer-print: Brace escaping" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test escaped braces
    try_(io_Writer_print(writer, u8_l("{{")));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("{"))));
    }

    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("}}")));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("}"))));
    }

    // Test mixed escaping and formatting
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{{value: {:d}}}"), 42));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("{value: 42}"))));
    }
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: Multiple arguments" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test multiple different types
    try_(io_Writer_print(writer, u8_l("{:d} {:c} {:x} {:s}"), 42, 'A', 255U, u8_l("test")));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("42 A ff test"))));
    }
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: Malformed format specifiers" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test missing closing brace - should write literally
    try_(io_Writer_print(writer, u8_l("{")));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("{"))));
    }

    // Test unmatched closing brace
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("}")));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("}"))));
    }

    // Test unknown format type`
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:q}"), 42)); // 'q' is not a valid type
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("{:q}")))); // Should write literally
    }
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: Empty format string" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test empty format string
    try_(io_Writer_print(writer, u8_l("")));
    try_(TEST_expect(test_Buf_view(buf).len == 0));
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print: Zero values" $scope) {
    T_use_A$(256, u8);
    A$256$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test zero in different bases
    try_(io_Writer_print(writer, u8_l("{:d}"), 0));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("0"))));
    }

    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:x}"), 0U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("0"))));
    }

    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:b}"), 0U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("0"))));
    }

    // Test that alternate form adds prefix even for zero
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:#x}"), 0U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("0x0"))));
    }
} $unscoped_(TEST_fn);

/*========== Complex Integration Tests ======================================*/

TEST_fn_("io_Writer-print: Complex format combinations" $scope) {
    T_use_A$(512, u8);
    A$512$u8 mem = zero$A();
    test_Buf buf = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test a complex real-world-like format string
    let name  = u8_l("Alice");
    let age   = 25;
    let score = 95.7f;
    let addr  = &age;

    try_(io_Writer_print(
        writer,
        u8_l("User: {:s}, Age: {:d}, ID: {:04x}, Addr: {:#p}, Score: {:.1f}"),
        name, age, 0x2A3B, addr, score
    ));

    let result = test_Buf_view(buf);
    printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
    try_(TEST_expect(Str_contains(result, u8_l("User: Alice"))));
    try_(TEST_expect(Str_contains(result, u8_l("Age: 25"))));
    try_(TEST_expect(Str_contains(result, u8_l("ID: 2a3b"))));
    try_(TEST_expect(Str_contains(result, u8_l("Addr: 0x")))); // Now uses #p to add prefix
    try_(TEST_expect(Str_contains(result, u8_l("Score: 95.7"))));
} $unscoped_(TEST_fn);

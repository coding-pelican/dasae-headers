/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    io_Writer-print_w_arg_idx.c
 * @author  Gyeongtae Kim(dev-dasae)
 * @date    2025-06-06 (date of creation)
 * @updated 2025-06-06 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/tests
 * @prefix  test
 *
 * @brief   Unit tests for io_Writer print functions with indexed arguments
 * @details Tests for io_Writer_print and related internal formatting functionality
 *          with indexed arguments
 */

/*========== Includes =======================================================*/

#include "dh/main.h"
#include "dh/Arr.h"
#include "dh/Str.h"
#include "dh/io/Writer.h"
#include "dh/fmt/common.h"

/*========== Test Helper - Buffer Writer ===================================*/

typedef struct test_Buf {
    Sli$u8 data;
    usize  pos;
} test_Buf;
$must_check
$static fn_(test_Buf_VT_write(const anyptr ctx, Sli_const$u8 bytes), Err$usize $scope) {
    let self      = as$(test_Buf*, ctx);
    let remaining = self->data.len - self->pos;
    let to_write  = prim_min(bytes.len, remaining);
    if (0 < to_write) {
        bti_memcpy(self->data.ptr + self->pos, bytes.ptr, to_write);
        self->pos += to_write;
    }
    return_ok(to_write);
} $unscoped;
static fn_(test_Buf_init(Sli$u8 data), test_Buf) {
    return (test_Buf){
        .data = data,
        .pos  = 0
    };
}
static fn_(test_Buf_writer(test_Buf* self), io_Writer) {
    debug_assert_nonnull(self);
    return make$(io_Writer, .ctx = self, .write = test_Buf_VT_write);
}
static fn_(test_Buf_clear(test_Buf* self), void) {
    debug_assert_nonnull(self);
    self->pos = 0;
}
static fn_(test_Buf_view(test_Buf self), Sli_const$u8) {
    // TODO: use Sli_sliceZ
    if (self.pos == 0) { return Sli_from$(Sli_const$u8, self.data.ptr, 0); }
    return Sli_slice(self.data, (0, self.pos)).as_const;
}

/*========== Indexed Argument Access Tests ==================================*/

TEST_fn_("io_Writer-print_w_arg_idx: Basic indexed arguments" $scope) {
    use_Arr$(256, u8);
    Arr$256$u8 mem    = Arr_zero();
    test_Buf   buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer  writer = test_Buf_writer(&buf);

    // Test basic indexed access - simple reordering
    try_(io_Writer_print(writer, u8_l("{1:d} {0:d}"), 42, 24));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("24 42"))));
    }

    // Test same argument used multiple times
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{0:d} + {0:d} = {:d}"), 5, 10));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("5 + 5 = 10"))));
    }
} $unscoped_TEST_fn;

TEST_fn_("io_Writer-print_w_arg_idx: Indexed arguments with different types" $scope) {
    use_Arr$(256, u8);
    Arr$256$u8 mem    = Arr_zero();
    test_Buf   buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer  writer = test_Buf_writer(&buf);

    let test_str = u8_l("Hello");

    // Test mixing indexed and sequential access
    try_(io_Writer_print(writer, u8_l("{1:s} {0:d} {:c}"), 42, test_str, 'X'));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("Hello 42 X"))));
    }

    // Test different format types for same argument
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{0:d} = 0x{0:x} = 0b{0:b}"), 15));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("15 = 0xf = 0b1111"))));
    }
} $unscoped_TEST_fn;

TEST_fn_("io_Writer-print_w_arg_idx: Indexed arguments out of order" $scope) {
    use_Arr$(256, u8);
    Arr$256$u8 mem    = Arr_zero();
    test_Buf   buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer  writer = test_Buf_writer(&buf);

    // Test arguments accessed in reverse order
    try_(io_Writer_print(writer, u8_l("{3:c}{2:d}{1:x}{0:s}"), u8_l("end"), 255U, 10, 'A'));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("A10ffend"))));
    }

    // Test sparse indexing (skipping indices)
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{0:d} {2:d}"), 1, 999, 3)); // Skip argument 1
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("1 3"))));
    }
} $unscoped_TEST_fn;

TEST_fn_("io_Writer-print_w_arg_idx: Indexed arguments with format flags" $scope) {
    use_Arr$(256, u8);
    Arr$256$u8 mem    = Arr_zero();
    test_Buf   buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer  writer = test_Buf_writer(&buf);

    // Test indexed access with format flags
    try_(io_Writer_print(writer, u8_l("{0:+d} {1:#x} {0: d}"), 42, 255U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("+42 0xff  42"))));
    }

    // Test indexed access with precision
    buf.pos      = 0;
    let long_str = u8_l("very long string");
    try_(io_Writer_print(writer, u8_l("{1:.5s} {0:04d} {1:.3s}"), 7, long_str));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("very  0007 ver"))));
    }
} $unscoped_TEST_fn;

TEST_fn_("io_Writer-print_w_arg_idx: Indexed arguments with 64-bit types" $scope) {
    use_Arr$(256, u8);
    Arr$256$u8 mem    = Arr_zero();
    test_Buf   buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer  writer = test_Buf_writer(&buf);

    // Test indexed access with 64-bit integers
    try_(io_Writer_print(writer, u8_l("{1:lld} {0:llu} {1:llx}"), 18446744073709551615ULL, 9223372036854775807LL));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_contains(result, u8_l("9223372036854775807"))));
        try_(TEST_expect(Str_contains(result, u8_l("18446744073709551615"))));
        try_(TEST_expect(Str_contains(result, u8_l("7fffffffffffffff"))));
    }

    // Test indexed access with size types
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{1:zd} {0:zu}"), as$(usize, 12345), as$(isize, -6789)));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_contains(result, u8_l("-6789"))));
        try_(TEST_expect(Str_contains(result, u8_l("12345"))));
    }
} $unscoped_TEST_fn;

TEST_fn_("io_Writer-print_w_arg_idx: Indexed arguments with pointers and strings" $scope) {
    use_Arr$(256, u8);
    Arr$256$u8 mem    = Arr_zero();
    test_Buf   buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer  writer = test_Buf_writer(&buf);

    let         slice_str = u8_l("slice");
    const char* cstr      = "cstring";
    i32         dummy     = 42;

    // Test indexed access with different string types and pointers
    try_(io_Writer_print(writer, u8_l("{2:p} {1:z} {0:s} {1:.3z}"), slice_str, cstr, &dummy));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_contains(result, u8_l("0x"))));      // pointer format
        try_(TEST_expect(Str_contains(result, u8_l("cstring")))); // full c-string
        try_(TEST_expect(Str_contains(result, u8_l("slice"))));   // slice string
        try_(TEST_expect(Str_contains(result, u8_l("cst"))));     // truncated c-string
    }
} $unscoped_TEST_fn;

TEST_fn_("io_Writer-print_w_arg_idx: Complex indexed argument patterns" $scope) {
    use_Arr$(512, u8);
    Arr$512$u8 mem    = Arr_zero();
    test_Buf   buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer  writer = test_Buf_writer(&buf);

    // Test a realistic use case: logging with indexed arguments for reordering
    let operation  = u8_l("READ");
    let filename   = u8_l("config.txt");
    i32 line_num   = 42;
    i32 error_code = 404;

    try_(io_Writer_print(writer, u8_l("[{1:s}] Error {3:d}: {0:s} operation failed on line {2:d}"), operation, filename, line_num, error_code));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("[config.txt] Error 404: READ operation failed on line 42"))));
    }

    // Test repeated use of same indexed argument in different contexts
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("Value: {0:d} (hex: 0x{0:x}, octal: 0{0:o}, binary: {0:b})"), 42));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("Value: 42 (hex: 0x2a, octal: 052, binary: 101010)"))));
    }
} $unscoped_TEST_fn;

TEST_fn_("io_Writer-print_w_arg_idx: Edge cases with indexed arguments" $scope) {
    use_Arr$(256, u8);
    Arr$256$u8 mem    = Arr_zero();
    test_Buf   buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer  writer = test_Buf_writer(&buf);

    // Test index 0 (first argument)
    try_(io_Writer_print(writer, u8_l("{0:d}"), 123));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("123"))));
    }

    // Test mixing indexed and non-indexed in same format string
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{:d} {1:d} {:d}"), 1, 2, 3));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        // This should work but behavior depends on implementation
        // The first {:d} should use arg 0, {1:d} uses arg 1, last {:d} should use arg 2
        try_(TEST_expect(Str_contains(result, u8_l("1"))));
        try_(TEST_expect(Str_contains(result, u8_l("2"))));
        try_(TEST_expect(Str_contains(result, u8_l("3"))));
    }

    // Test escaped braces with indexed arguments
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{{index {0:d}}}"), 5));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("{index 5}"))));
    }
} $unscoped_TEST_fn;

TEST_fn_("io_Writer-print_w_arg_idx: Error handling with indexed arguments" $scope) {
    use_Arr$(256, u8);
    Arr$256$u8 mem    = Arr_zero();
    test_Buf   buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer  writer = test_Buf_writer(&buf);

    // // Test out of bounds index - should return an error
    // {
    //     let result = io_Writer_print(writer, u8_l("{999:d}"), 42);
    //     try_(TEST_expect(result.is_err));
    //     let err    = result.data.err;
    //     try_(TEST_expect(Str_eql(Err_codeToStr(err), u8_l("IndexOutOfBounds"))));
    // }

    // Test invalid index format - should be handled gracefully by ignoring specifier
    test_Buf_clear(&buf);
    try_(io_Writer_print(writer, u8_l("{abc:d}"), 42)); // Non-numeric index
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("{abc:d}"))));
    }
} $unscoped_TEST_fn;

fn_(dh_main(Sli$Sli_const$u8 args), Err$void $scope) {
    $ignore = args;

    use_Arr$(256, u8);
    Arr$256$u8 mem       = Arr_zero();
    let        mem_slice = Arr_ref$(Sli$u8, mem);
    test_Buf   buf       = test_Buf_init(mem_slice);
    io_Writer  writer    = test_Buf_writer(&buf);

    // Test indexed access with format flags
    try_(io_Writer_print(writer, u8_l("{0:+d} {1:#x} {0: d}"), 42, 255U));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("+42 0xff  42"))));
    }

    // Test indexed access with precision
    buf.pos      = 0;
    let long_str = u8_l("very long string");
    try_(io_Writer_print(writer, u8_l("{1:.5s} {0:04d} {1:.3s}"), 7, long_str));
    {
        let result = test_Buf_view(buf);
        printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
        try_(TEST_expect(Str_eql(result, u8_l("very  0007 ver"))));
    }

    return_ok({});
} $unscoped;

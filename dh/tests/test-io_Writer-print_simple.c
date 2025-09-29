/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    io_Writer-print_simple.c
 * @author  Gyeongtae Kim(dev-dasae)
 * @date    2025-06-06 (date of creation)
 * @updated 2025-06-06 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/tests
 * @prefix  test
 *
 * @brief   Unit tests for io_Writer print functions with simple format
 * @details Tests for io_Writer_print and related internal formatting functionality
 *          with simple format
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

/*========== Simple Format Tests ============================================*/

TEST_fn_("io_Writer-print_simple: Integer test" $scope) {
    use_Arr$(64, u8);
    Arr$64$u8 mem    = Arr_zero();
    test_Buf  buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer writer = test_Buf_writer(&buf);

    // Test very basic integer
    try_(io_Writer_print(writer, u8_l("{:d}"), 42));
    let result = test_Buf_view(buf);

    printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
    try_(TEST_expect(Str_eql(result, u8_l("42"))));
} $unscoped_TEST_fn;

TEST_fn_("io_Writer-print_simple: Character test" $scope) {
    use_Arr$(64, u8);
    Arr$64$u8 mem    = Arr_zero();
    test_Buf  buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer writer = test_Buf_writer(&buf);

    // Test very basic character
    try_(io_Writer_print(writer, u8_l("{:c}"), 'A'));
    let result = test_Buf_view(buf);

    printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
    try_(TEST_expect(Str_eql(result, u8_l("A"))));
} $unscoped_TEST_fn;

TEST_fn_("io_Writer-print_simple: Hex test" $scope) {
    use_Arr$(64, u8);
    Arr$64$u8 mem    = Arr_zero();
    test_Buf  buf    = test_Buf_init(Arr_ref$(Sli$u8, mem));
    io_Writer writer = test_Buf_writer(&buf);

    // Test very basic hex
    try_(io_Writer_print(writer, u8_l("{:x}"), 255U));
    let result = test_Buf_view(buf);

    printf("Result: '%.*s' (len=%zu)\n", as$(i32, result.len), result.ptr, result.len);
    try_(TEST_expect(Str_eql(result, u8_l("ff"))));
} $unscoped_TEST_fn;

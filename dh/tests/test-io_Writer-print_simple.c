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
        prim_memcpyS(prefixS(suffixS(self->data, self->pos), to_write), bytes);
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
$maybe_unused
$static fn_((test_Buf_clear(test_Buf* self))(void)) {
    claim_assert_nonnull(self);
    self->pos = 0;
}
$static fn_((test_Buf_view(test_Buf self))(S_const$u8)) {
    if (self.pos == 0) { return zeroS$((const u8)); }
    return sliceS(self.data, $r(0, self.pos)).as_const;
}

/*========== Simple Format Tests ============================================*/

TEST_fn_("io_Writer-print_simple: Integer test" $scope) {
    T_use_A$(64, u8);
    A$64$u8   mem    = zero$A();
    test_Buf  buf    = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test very basic integer
    try_(io_Writer_print(writer, u8_l("{:d}"), 42));
    let result = test_Buf_view(buf);

    printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
    try_(TEST_expect(Str_eql(result, u8_l("42"))));
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print_simple: Character test" $scope) {
    T_use_A$(64, u8);
    A$64$u8   mem    = zero$A();
    test_Buf  buf    = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test very basic character
    try_(io_Writer_print(writer, u8_l("{:c}"), 'A'));
    let result = test_Buf_view(buf);

    printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
    try_(TEST_expect(Str_eql(result, u8_l("A"))));
} $unscoped_(TEST_fn);

TEST_fn_("io_Writer-print_simple: Hex test" $scope) {
    T_use_A$(64, u8);
    A$64$u8   mem    = zero$A();
    test_Buf  buf    = test_Buf_init(ref$A$((u8)(mem)));
    io_Writer writer = test_Buf_writer(&buf);

    // Test very basic hex
    try_(io_Writer_print(writer, u8_l("{:x}"), 255U));
    let result = test_Buf_view(buf);

    printf("Result: '%.*s' (len=%zu)\n", as$(i32)(result.len), result.ptr, result.len);
    try_(TEST_expect(Str_eql(result, u8_l("ff"))));
} $unscoped_(TEST_fn);

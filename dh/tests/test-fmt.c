#include "dh-main.h"
#include "dh/fmt/common.h"
#include "dh/io/Fixed.h"
#include "dh/mem/common.h"

$static fn_((test_fmt_format_toFixed(S$u8 buf, S_const$u8 fmt, S_const$u8 arg))(E$S_const$u8) $scope) {
    var fixed_writer = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(fmt_format(io_Fixed_writer(&fixed_writer), fmt, arg));
    return_ok(io_Fixed_written(fixed_writer.stream).as_const);
} $unscoped(fn);

TEST_fn_("fmt/common: format writes literal without collecting varargs" $scope) {
    var buf = u8_a("................................");
    var fixed_writer = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(buf))));

    try_(fmt_format(io_Fixed_writer(&fixed_writer), u8_l("literal only")));

    try_(TEST_expect(mem_eqlBytes(io_Fixed_written(fixed_writer.stream).as_const, u8_l("literal only"))));
} $unscoped(TEST_fn);

TEST_fn_("fmt/common: format applies string alignment specs" $scope) {
    var right_buf = u8_a("................................");
    let right = try_(test_fmt_format_toFixed(A_ref$((S$u8)(right_buf)), u8_l("[{:>5s}]"), u8_l("hi")));
    try_(TEST_expect(mem_eqlBytes(right, u8_l("[   hi]"))));

    var left_buf = u8_a("................................");
    let left = try_(test_fmt_format_toFixed(A_ref$((S$u8)(left_buf)), u8_l("[{:<5s}]"), u8_l("hi")));
    try_(TEST_expect(mem_eqlBytes(left, u8_l("[hi   ]"))));

    var center_buf = u8_a("................................");
    let center = try_(test_fmt_format_toFixed(A_ref$((S$u8)(center_buf)), u8_l("[{:^6s}]"), u8_l("hi")));
    try_(TEST_expect(mem_eqlBytes(center, u8_l("[  hi  ]"))));

    var fill_buf = u8_a("................................");
    let fill = try_(test_fmt_format_toFixed(A_ref$((S$u8)(fill_buf)), u8_l("[{:_>5s}]"), u8_l("hi")));
    try_(TEST_expect(mem_eqlBytes(fill, u8_l("[___hi]"))));
} $unscoped(TEST_fn);

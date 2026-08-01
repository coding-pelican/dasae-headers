/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    io_Writer-newline.c
 * @author  Gyeongtae Kim(dev-dasae)
 * @date    2026-05-25 (date of creation)
 * @updated 2026-05-25 (date of last update)
 * @ingroup dasae-headers(dh)/tests
 * @prefix  test
 */

/*========== Includes =======================================================*/

#include "dh-main.h"
#include "dh/io.h"
#include "dh/mem/common.h"

/*========== Tests ==========================================================*/

TEST_fn_("io/Writer: nl writes logical LF byte" $scope) {
    T_use_A$(8, u8);
    var_(mem, A$8$u8) $undefined;
    var writer_impl = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(mem))));

    try_(io_Writer_nl(io_Fixed_writer(&writer_impl)));

    try_(TEST_expect(mem_eqlBytes(io_Fixed_written(writer_impl.stream).as_const, u8_l(io_nl))));
} $unscoped(TEST_fn);

TEST_fn_("io/Writer: println appends logical LF byte" $scope) {
    T_use_A$(8, u8);
    var_(mem, A$8$u8) $undefined;
    var writer_impl = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(mem))));

    try_(io_Writer_println(io_Fixed_writer(&writer_impl), u8_l("abc")));

    try_(TEST_expect(mem_eqlBytes(io_Fixed_written(writer_impl.stream).as_const, u8_l("abc" io_nl))));
} $unscoped(TEST_fn);

TEST_fn_("io/Writer: crlf writes explicit CRLF bytes" $scope) {
    T_use_A$(8, u8);
    var_(mem, A$8$u8) $undefined;
    var writer_impl = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(mem))));

    try_(io_Writer_crlf(io_Fixed_writer(&writer_impl)));

    try_(TEST_expect(mem_eqlBytes(io_Fixed_written(writer_impl.stream).as_const, u8_l(io_crlf))));
} $unscoped(TEST_fn);

TEST_fn_("io/common: native newline follows platform separator" $scope) {
    let expected = pp_if_(plat_is_windows)(
        pp_then_(u8_l(io_crlf)),
        pp_else_(u8_l(io_nl)));

    try_(TEST_expect(mem_eqlBytes(u8_l(io_nl_native), expected)));
} $unscoped(TEST_fn);

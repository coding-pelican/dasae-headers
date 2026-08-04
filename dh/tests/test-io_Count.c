#include "dh-main.h"
#include "dh/io/Count.h"
#include "dh/io/Fixed.h"
#include "dh/mem/common.h"

TEST_fn_("io/Count/Reader: count follows bytes returned by the inner reader" $scope) {
    var inner = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abc")));
    var counted = io_Count_Reader_from(io_Fixed_reader(&inner));
    var_(out, A$$(2, u8)) $undefined;

    try_(TEST_expect(try_(io_Reader_read(
        io_Count_reader(&counted), A_ref$((S$u8)(out))
    )) == 2));
    try_(TEST_expect(io_Count_Reader_count(counted) == 2));
    try_(TEST_expect(try_(io_Reader_read(
        io_Count_reader(&counted), A_ref$((S$u8)(out))
    )) == 1));
    try_(TEST_expect(io_Count_Reader_count(counted) == 3));
    try_(TEST_expect(try_(io_Reader_read(
        io_Count_reader(&counted), A_ref$((S$u8)(out))
    )) == 0));
    try_(TEST_expect(io_Count_Reader_count(counted) == 3));
} $unscoped(TEST_fn);

TEST_fn_("io/Count/Writer: count follows bytes consumed by the inner writer" $scope) {
    var_(out, A$$(4, u8)) $undefined;
    var inner = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(out))));
    var counted = io_Count_Writer_from(io_Fixed_writer(&inner));

    try_(io_Writer_writeBytes(io_Count_writer(&counted), u8_l("abc")));
    try_(TEST_expect(io_Count_Writer_count(counted) == 3));
    try_(TEST_expect(mem_eqlBytes(io_Fixed_written(inner.stream).as_const, u8_l("abc"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Count: failing interfaces do not count failed transfers" $scope) {
    var reader = io_Count_Reader_from(io_Reader_failing);
    var writer = io_Count_Writer_from(io_Writer_failing);
    var_(out, A$$(1, u8)) $undefined;

    try_(TEST_expect(isErr(io_Reader_read(
        io_Count_reader(&reader), A_ref$((S$u8)(out))
    ))));
    try_(TEST_expect(isErr(io_Writer_write(
        io_Count_writer(&writer), u8_l("x")
    ))));
    try_(TEST_expect(io_Count_Reader_count(reader) == 0));
    try_(TEST_expect(io_Count_Writer_count(writer) == 0));
} $unscoped(TEST_fn);

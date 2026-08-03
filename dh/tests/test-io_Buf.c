#include "dh-main.h"
#include "dh/io.h"
#include "dh/mem/common.h"

TEST_fn_("io/Buf/Reader: peek take and skip expose buffered cursor" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abcdef")));
    var_(mem, A$$(4, u8)) $undefined;
    var reader = io_Buf_Reader_from(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(mem)));

    let peeked = try_(io_Buf_Reader_peekBytes(&reader, 3));
    try_(TEST_expect(mem_eqlBytes(peeked, u8_l("abc"))));

    let byte = try_(io_Buf_Reader_takeByte(&reader));
    try_(TEST_expect(byte == u8_c('a')));
    try_(TEST_expect(mem_eqlBytes(io_Buf_Reader_ready(reader), u8_l("bcd"))));

    let taken = try_(io_Buf_Reader_takeBytes(&reader, 2));
    try_(TEST_expect(mem_eqlBytes(taken, u8_l("bc"))));
    try_(TEST_expect(mem_eqlBytes(io_Buf_Reader_ready(reader), u8_l("d"))));

    io_Buf_Reader_drop(&reader, 1);
    try_(TEST_expect(io_Buf_Reader_ready(reader).len == 0));

    try_(io_Buf_Reader_require(&reader, 2));
    try_(TEST_expect(mem_eqlBytes(io_Buf_Reader_ready(reader), u8_l("ef"))));

    io_Buf_Reader_dropAll(&reader);
    try_(TEST_expect(io_Buf_Reader_ready(reader).len == 0));
} $unscoped(TEST_fn);

TEST_fn_("io/Buf/Reader: readUntilAnyByte consumes first matching delimiter" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abc,def;ghi")));
    var_(mem, A$$(4, u8)) $undefined;
    var reader = io_Buf_Reader_from(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(mem)));
    var_(out, A$$(8, u8)) $undefined;

    let first = try_(io_Buf_Reader_readUntilAny(&reader, u8_l(",;"), A_ref$((S$u8)(out))));
    try_(TEST_expect(mem_eqlBytes(first.as_const, u8_l("abc"))));

    let second = try_(io_Buf_Reader_readUntilAny(&reader, u8_l(",;"), A_ref$((S$u8)(out))));
    try_(TEST_expect(mem_eqlBytes(second.as_const, u8_l("def"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Buf/Reader: skipUntilAnyByte consumes matching delimiter" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abc,def;ghi")));
    var_(mem, A$$(4, u8)) $undefined;
    var reader = io_Buf_Reader_from(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(mem)));
    var_(out, A$$(8, u8)) $undefined;

    try_(io_Buf_Reader_skipUntilAny(&reader, u8_l(",;")));
    let next = try_(io_Buf_Reader_readUntilAny(&reader, u8_l(",;"), A_ref$((S$u8)(out))));

    try_(TEST_expect(mem_eqlBytes(next.as_const, u8_l("def"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Buf/Reader: readUntilSeq preserves delimiter across buffer boundary" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("ab\r\ncd")));
    var_(mem, A$$(3, u8)) $undefined;
    var reader = io_Buf_Reader_from(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(mem)));
    var_(out, A$$(8, u8)) $undefined;

    let line = try_(io_Buf_Reader_readUntilSeq(&reader, u8_l("\r\n"), A_ref$((S$u8)(out))));

    try_(TEST_expect(mem_eqlBytes(line.as_const, u8_l("ab"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Buf/Reader: skipUntilSeq preserves delimiter across buffer boundary" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("ab\r\ncd\r\n")));
    var_(mem, A$$(3, u8)) $undefined;
    var reader = io_Buf_Reader_from(io_Fixed_reader(&reader_impl), A_ref$((S$u8)(mem)));
    var_(out, A$$(8, u8)) $undefined;

    try_(io_Buf_Reader_skipUntilSeq(&reader, u8_l("\r\n")));
    let line = try_(io_Buf_Reader_readUntilSeq(&reader, u8_l("\r\n"), A_ref$((S$u8)(out))));

    try_(TEST_expect(mem_eqlBytes(line.as_const, u8_l("cd"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Buf/Writer: pending reports buffered bytes before flush" $scope) {
    var_(out, A$$(8, u8)) $undefined;
    var writer_impl = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(out))));
    var_(mem, A$$(4, u8)) $undefined;
    var writer = io_Buf_Writer_from(io_Fixed_writer(&writer_impl), A_ref$((S$u8)(mem)));

    try_(io_Writer_writeBytes(io_Buf_writer(&writer), u8_l("ab")));
    try_(TEST_expect(mem_eqlBytes(io_Buf_Writer_pending(writer), u8_l("ab"))));
    try_(TEST_expect(io_Fixed_written(writer_impl.stream).len == 0));

    try_(io_Buf_Writer_flush(&writer));
    try_(TEST_expect(io_Buf_Writer_pending(writer).len == 0));
    try_(TEST_expect(mem_eqlBytes(io_Fixed_written(writer_impl.stream).as_const, u8_l("ab"))));
} $unscoped(TEST_fn);

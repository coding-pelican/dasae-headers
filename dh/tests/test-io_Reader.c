#include "dh-main.h"
#include "dh/io.h"
#include "dh/mem/common.h"

TEST_fn_("io/Reader: readAtLeast returns partial count at EOF" $scope) {
    var reader_impl = io_Fixed_Reader_init(io_Fixed_reading(u8_l("abc")));
    let reader = io_Fixed_reader(&reader_impl);
    var_(out, A$$(5, u8)) $undefined;

    let bytes_read = try_(io_Reader_readAtLeast(reader, A_ref$((S$u8)(out)), 5));

    try_(TEST_expect(bytes_read == 3));
    try_(TEST_expect(mem_eqlBytes(A_slice$((S_const$u8)(out)$r(0, bytes_read)), u8_l("abc"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: skipAtLeast returns skipped count at EOF" $scope) {
    var reader_impl = io_Fixed_Reader_init(io_Fixed_reading(u8_l("abc")));
    let skipped_len = try_(io_Reader_skipAtLeast(io_Fixed_reader(&reader_impl), 5));

    try_(TEST_expect(skipped_len == 3));
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: copy moves bytes into any writer" $scope) {
    var reader_impl = io_Fixed_Reader_init(io_Fixed_reading(u8_l("abcdef")));
    var_(out, A$$(8, u8)) $undefined;
    var writer_impl = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(out))));

    let copied = try_(io_Reader_copy(io_Fixed_reader(&reader_impl), io_Fixed_writer(&writer_impl)));
    let written = io_Fixed_written(writer_impl.stream);

    try_(TEST_expect(copied == 6));
    try_(TEST_expect(mem_eqlBytes(written.as_const, u8_l("abcdef"))));
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: copyExact fails when source ends early" $scope) {
    var reader_impl = io_Fixed_Reader_init(io_Fixed_reading(u8_l("abc")));
    var_(out, A$$(8, u8)) $undefined;
    var writer_impl = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(out))));

    let result = io_Reader_copyExact(io_Fixed_reader(&reader_impl), io_Fixed_writer(&writer_impl), 4);

    try_(TEST_expect(isErr(result)));
} $unscoped(TEST_fn);

TEST_fn_("io/Reader: copyAtLeast returns copied count at EOF" $scope) {
    var reader_impl = io_Fixed_Reader_init(io_Fixed_reading(u8_l("abc")));
    var_(out, A$$(8, u8)) $undefined;
    var writer_impl = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(out))));

    let copied_len = try_(io_Reader_copyAtLeast(io_Fixed_reader(&reader_impl), io_Fixed_writer(&writer_impl), 5));
    let written = io_Fixed_written(writer_impl.stream);

    try_(TEST_expect(copied_len == 3));
    try_(TEST_expect(mem_eqlBytes(written.as_const, u8_l("abc"))));
} $unscoped(TEST_fn);

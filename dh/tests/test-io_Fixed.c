#include "dh-main.h"
#include "dh/io/Fixed.h"
#include "dh/mem/common.h"

TEST_fn_("io/Fixed: reader and writer advance independent cursors" $scope) {
    var reader_impl = io_Fixed_Reader_from(io_Fixed_reading(u8_l("abc")));
    var_(read_mem, A$$(2, u8)) $undefined;
    let read_len = try_(io_Reader_read(
        io_Fixed_reader(&reader_impl), A_ref$((S$u8)(read_mem))
    ));
    try_(TEST_expect(read_len == 2));
    try_(TEST_expect(mem_eqlBytes(
        A_prefix$((S_const$u8)(read_mem)(read_len)), u8_l("ab")
    )));

    var_(write_mem, A$$(4, u8)) $undefined;
    var writer_impl = io_Fixed_Writer_from(
        io_Fixed_writing(A_ref$((S$u8)(write_mem)))
    );
    try_(io_Writer_writeBytes(io_Fixed_writer(&writer_impl), u8_l("xy")));
    try_(TEST_expect(mem_eqlBytes(
        io_Fixed_written(writer_impl.stream).as_const, u8_l("xy")
    )));
} $unscoped(TEST_fn);

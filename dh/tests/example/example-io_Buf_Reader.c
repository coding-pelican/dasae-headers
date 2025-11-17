#include "dh/main.h"
#include "dh/io/Buf.h"
#include "dh/io/common.h"
#include "dh/io/stream.h"
#include "dh/fmt/common.h"

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    let stdin_file = io_getStdIn();
    let unbuffered = fs_File_reader(stdin_file);

    A$$(4096, u8) buf = A_zero();
    var buffered = io_Buf_Reader_init(unbuffered, A_ref$((S$u8)(buf)));
    let first_byte = try_(io_Buf_Reader_peekByte(&buffered));
    io_stream_print(u8_l("First byte: '{0:dhh}' -> '{0:c}'\n"), first_byte);

    A$$(256, u8) line_buf = A_zero();
    let line = try_(io_Buf_Reader_readUntilByte(&buffered, u8_c('\n'), A_ref$((S$u8)(line_buf))));
    io_stream_print(u8_l("Line: '{:s}'\n"), line);

    let num = try_(fmt_parse$i64(line.as_const, 10));
    io_stream_print(u8_l("Number: '{:dll}'\n"), num);

    return_ok({});
} $unscoped_(fn);

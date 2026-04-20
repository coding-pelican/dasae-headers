#include "dh-main.h"
#include "dh/io/Buf.h"
#include "dh/io/common.h"
#include "dh/io/stream.h"
#include "dh/fmt/common.h"

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    let in_file = io_getStdIn();
    let unbufd = fs_File_reader(in_file);

    var_(buf, A$$(4096, u8)) = A_zero();
    var bufd = io_Buf_Reader_init(unbufd, A_ref$((S$u8)(buf)));
    let first_byte = try_(io_Buf_Reader_peekByte(&bufd));
    io_stream_print(u8_l("First byte: '{0:dhh}' -> '{0:c}'\n"), first_byte);

    var_(line_buf, A$$(256, u8)) = A_zero();
    let line = try_(io_Buf_Reader_readUntilByte(&bufd, u8_c('\n'), A_ref$((S$u8)(line_buf))));
    io_stream_print(u8_l("Line: '{:s}'\n"), line);

    let num = try_(fmt_parse$i64(line.as_const, 10));
    io_stream_print(u8_l("Number: '{:dll}'\n"), num);

    return_ok({});
} $unscoped(fn);

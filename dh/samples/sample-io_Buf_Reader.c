#include "dh-main.h"
#include "dh/io/Buf.h"
#include "dh/proc/std.h"
#include "dh/fs/File/self.h"
#include "dh/io/stream.h"
#include "dh/fmt/common.h"

fn_((main(proc_Entry entry))(E$void) $scope) {
    let in_file = proc_std_in(entry.std);
    let unbufd = fs_File_reader(in_file);

    var_(buf, A$$(4096, u8)) $undefined;
    var bufd = io_Buf_Reader_init(unbufd, A_ref$((S$u8)(buf)));
    let first_byte = try_(io_Buf_Reader_peekByte(&bufd));
    io_stream_print(u8_l("First byte: '{0:dhh}' -> '{0:c}'\n"), first_byte);

    var_(line_buf, A$$(256, u8)) $undefined;
    let line = try_(io_Buf_Reader_readUntilByte(&bufd, u8_c('\n'), A_ref$((S$u8)(line_buf))));
    io_stream_print(u8_l("Line: '{:s}'\n"), line);

    let num = try_(fmt_parse$i64(line.as_const, 10));
    io_stream_print(u8_l("Number: '{:dll}'\n"), num);

    return_ok({});
} $unscoped(fn);

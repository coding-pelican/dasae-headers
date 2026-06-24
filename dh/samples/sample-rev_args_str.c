#include "dh-main.h"
#include "dh/io/stream.h"
#include "dh/fs/File/self.h"
#include "dh/io/Buf.h"
#include "dh/mem/common.h"
#include "dh/fmt/common.h"
#include "dh/ascii.h"

T_use$((u8)(
    mem_Delim,
    mem_TokzIter,
    mem_tokzAny,
    mem_TokzIter_next
));

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    io_stream_println(u8_l("passed args:"));
    for_($rev($s(args), $rf(0))(arg, index)) {
        io_stream_println(u8_l("  arg {:ul}: {:s}"), index, arg);
    } $end(for);
    io_stream_nl();

    var_(input_mem, A$$(64, u8)) $undefined;
    let input_buf = A_ref$((S$u8)(input_mem));
    var_(read_mem, A$$(256, u8)) $undefined;
    var stream_in = io_Buf_Reader_init(fs_File_reader(io_handleStdIn()), A_ref$((S$u8)(read_mem)));
    let whitespace = A_ref$((S_const$u8)(u8_a(ascii_whitespaces)));

    io_stream_print(u8_l("tokenizing reading input: "));
    using_(let input = try_(io_Buf_Reader_readUntilByte(&stream_in, ascii_lf_byte, input_buf))) {
        var iter = mem_tokzAny$u8(input.as_const, whitespace);
        while_some(mem_TokzIter_next$u8(&iter), token) {
            io_stream_println(u8_l("token: {:s}"), token);
        }
        io_stream_nl();
    }

    io_stream_print(u8_l("input i32 '<a> <b> [<c>]': "));
    using_(let input = try_(io_Buf_Reader_readUntilByte(&stream_in, ascii_lf_byte, input_buf))) {
        var iter = mem_tokzAny$u8(input.as_const, whitespace);
        let a = mem_TokzIter_next$u8(&iter);
        io_stream_println(u8_l("a: {:?s}"), a);
        let b = mem_TokzIter_next$u8(&iter);
        io_stream_println(u8_l("b: {:?s}"), b);
        let c_maybe_none = mem_TokzIter_next$u8(&iter);
        io_stream_println(u8_l("c: {:?s}"), c_maybe_none);
        let parsed_a = try_(fmt_parse$i32(unwrap_(a), 10));
        io_stream_println(u8_l("parsed_a: {:i}"), parsed_a);
        let parsed_b = try_(fmt_parse$i32(unwrap_(b), 10));
        io_stream_println(u8_l("parsed_b: {:i}"), parsed_b);
        let add_a_b = parsed_a + parsed_b;
        io_stream_println(u8_l("add_a_b: {:i}"), add_a_b);
        io_stream_nl();
    }

    return_ok_void(io_stream_println(u8_l("done")));
} $unscoped(fn);

#include "dh/main.h"
#include "dh/fs/File.h"
#include "dh/io/common.h"
#include "dh/io/stream.h"
#include "dh/io/Reader.h"
#include "dh/mem/common.h"
#include "dh/fmt/common.h"

T_use$((u8)(
    mem_TokenIter,
    mem_tokenizeValue,
    mem_TokenIter_next
));

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    io_stream_println(u8_l("passed args:"));
    for_($rev($s(args), $rf(0))(arg, index) {
        io_stream_println(u8_l("  arg {:ul}: {:s}"), index, arg);
    });
    io_stream_nl();

    var_(input_mem, A$$(64, u8)) = zero$A();
    let input_buf = A_ref$((S$u8)(input_mem));
    let stream_in = fs_File_reader(io_getStdIn());

    with_(let input = S_prefix((input_buf)try_(io_Reader_read(stream_in, input_buf)))) {
        var iter = mem_tokenizeValue$u8(input.as_const, u8_c(' '));
        while_some(mem_TokenIter_next$u8(&iter), token) {
            io_stream_println(u8_l("token: {:s}"), token);
        }
        io_stream_nl();
    }

    with_(let input = S_prefix((input_buf)try_(io_Reader_read(stream_in, input_buf)))) {
        var iter = mem_tokenizeValue$u8(input.as_const, u8_c(' '));
        let a = mem_TokenIter_next$u8(&iter);
        io_stream_println(u8_l("a: {:?s}"), a);
        let b = mem_TokenIter_next$u8(&iter);
        io_stream_println(u8_l("b: {:?s}"), b);
        let c_maybe_none = mem_TokenIter_next$u8(&iter);
        io_stream_println(u8_l("c: {:?s}"), c_maybe_none);
        let parsed_a = try_(fmt_parse$i32(unwrap_(a), 10));
        io_stream_println(u8_l("parsed_a: {:i}"), parsed_a);
        let parsed_b = try_(fmt_parse$i32(unwrap_(b), 10));
        io_stream_println(u8_l("parsed_b: {:i}"), parsed_b);
        let add_a_b = parsed_a + parsed_b;
        io_stream_println(u8_l("add_a_b: {:i}"), add_a_b);
        io_stream_nl();
    }

    return_ok({});
} $unscoped_(fn);

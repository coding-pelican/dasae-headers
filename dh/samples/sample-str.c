#include "dh-main.h"
#include "dh/io/stream.h"
#include "dh/utf8.h"

fn_((main(proc_Self self))(E$void) $scope) {
    let args = self.args.items;
    let_ignore = args;

    let readonly = u8_l("readonly byte slice");
    var writable_mem = u8_a("writable byte slice");
    let writable = A_ref$((S_const$u8)(writable_mem));

    io_stream_println(u8_l("readonly: {:s}"), readonly);
    io_stream_println(u8_l("writable: {:s}"), writable);

    let utf8_text = u8_l("Hello, 世界! 안녕! 나는 왈도! 강하고 굳센 아침?");
    try_(utf8_view(utf8_text));
    io_stream_println(u8_l("utf8 bytes: {:uz}"), utf8_text.len);
    io_stream_println(u8_l("utf8 codepoints: {:uz}"), utf8_count(utf8_text));

    using_(var iter = utf8_iter(utf8_viewUnchkd(utf8_text))) {
        io_stream_print(u8_l("codepoints as chs:"));
        while_some((utf8_Iter_next(&iter)), cp) {
            io_stream_print(u8_l(" {:C}"), cp);
        }
        io_stream_nl();
    }
    using_(var iter = utf8_iter(utf8_viewUnchkd(utf8_text))) {
        io_stream_print(u8_l("codepoints as hex:"));
        while_some((utf8_Iter_next(&iter)), cp) {
            io_stream_print(u8_l(" U+{:X}"), cp);
        }
        io_stream_nl();
    }
    io_stream_println(u8_l("할아버지 보고싶어요"));

    return_ok({});
} $unscoped(fn);

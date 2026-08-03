#include <dh-main.h>
#include <dh/heap/Sys.h>
#include <dh/heap/Arena.h>
#include "daterm-context/ANSI.h"
#include <dh/io/stream.h>
#include <dh/ascii.h>

fn_((main(proc_Entry entry))(E$void) $guard) {
    var_(arg_mem, A$$(16, u8)) $undefined;
    let output_mode_arg = local_({
        var args = proc_Args_iter(entry.args);
        let_ignore = try_(proc_Args_Iter_skip(&args));
        let scratch = A_ref$((S$u8)(arg_mem));
        local_return_(orelse_((try_(proc_Args_Iter_next(&args, scratch)))({
            io_stream_eprintln(u8_l("Usage: example-linefeed <on|off>"));
            start_exit(1);
        })));
    });
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    var ansi = try_(daterm_ANSI_init(with_((unwrap_(
        daterm_ANSI_Cfg_direct(heap_Sys_alctr(&heap))
    ))((.output_mode)(local_({
        let is_processed = ascii_eqlSenseCase(output_mode_arg, u8_l("on"));
        let is_raw = ascii_eqlSenseCase(output_mode_arg, u8_l("off"));
        if (!is_processed && !is_raw) {
            io_stream_eprintln(u8_l("Usage: example-linefeed <on|off>"));
            start_exit(1);
        }
        local_return_(as$(daterm_ANSI_OutputMode)(when_(is_processed)(
            provide_(daterm_ANSI_OutputMode_processed),
            instead_(daterm_ANSI_OutputMode_raw)
        )));
    }))))));
    defer_(daterm_ANSI_fini(&ansi));
    try_(daterm_ANSI_enableRawMode(&ansi));
    defer_(daterm_ANSI_disableRawMode(&ansi));
    let term = daterm_ANSI_term(&ansi);
    let in = daterm_Term_reader(term);
    let out = daterm_Term_writer(term);
    let screen_rect = try_(daterm_Term_queryNativeScreenCells(term));
    try_(io_Writer_print(out, u8_l("Screen size: {:u}x{:u}" io_crlf), screen_rect.cols, screen_rect.rows));
    try_(io_Writer_writeByteN(out, u8_c('A'), screen_rect.cols - 2));
    try_(io_Writer_writeBytes(out, u8_l(io_lf)));
    try_(io_Writer_writeByteN(out, u8_c('B'), screen_rect.cols - 1));
    try_(io_Writer_writeBytes(out, u8_l(io_lf)));
    try_(io_Writer_writeByteN(out, u8_c('C'), screen_rect.cols));
    try_(io_Writer_writeBytes(out, u8_l(io_lf)));
    try_(io_Writer_writeByteN(out, u8_c('D'), screen_rect.cols + 1));
    try_(io_Writer_writeBytes(out, u8_l(io_lf)));
    let_ignore = try_(io_Reader_readByte(in));
    return_ok({});
} $unguarded(fn);

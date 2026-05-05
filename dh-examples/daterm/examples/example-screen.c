#include <dh-main.h>
#include <dh/io/common.h>
#include <dh/io/Buf.h>
#include <dh/fs/File.h>
#include "daterm.h"

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    let in_file = io_getStdIn();
    let out_file = io_getStdOut();
    var term = try_(daterm_Ctx_init(in_file.handle, out_file.handle));
    defer_(catch_((daterm_Ctx_fini(&term))($ignore, $do_nothing)));

    var_(out_buf, A$$(1024, u8)) = A_zero();
    var out_bufd = io_Buf_Writer_init(fs_File_writer(out_file), A_ref$((S$u8)(out_buf)));
    defer_(catch_((io_Buf_Writer_flush(&out_bufd))($ignore, $do_nothing)));
    let out_writer = io_Buf_writer(&out_bufd);

    try_(daterm_screen_enterAlternateWrite(out_writer));
    defer_(catch_((daterm_screen_exitAlternateWrite(out_writer))($ignore, $do_nothing)));

    try_(daterm_cursor_hideWrite(out_writer));
    defer_(catch_((daterm_cursor_showWrite(out_writer))($ignore, $do_nothing)));

    try_(daterm_cursor_moveToWrite(1, 1, out_writer));
    try_(daterm_style_italicWrite(true, out_writer));
    try_(io_Writer_print(out_writer, u8_l("This is being shown in the alternate screen...")));
    try_(io_Buf_Writer_flush(&out_bufd));

    time_sleep(time_Duration_fromSecs(2));
    return_ok({});
} $unguarded(fn);

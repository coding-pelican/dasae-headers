#include <dh-main.h>
#include <dh/io/common.h>
#include <dh/fs/File.h>
#include "daterm.h"

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    let in_file = io_getStdIn();
    let out_file = io_getStdOut();
    var term = try_(daterm_Ctx_init(in_file.handle, out_file.handle));
    defer_(catch_((daterm_Ctx_fini(&term))($ignore, $do_nothing)));

    let in_reader = fs_File_reader(in_file);
    let out_writer = fs_File_writer(out_file);

    try_(io_Writer_println(
        out_writer, u8_l("{:s}Warning text"),
        u8_l(daterm_color_fg4bit_static(daterm_color_fg4bit_staticParse(daterm_Palette4bit_red)))
    ));

    try_(io_Writer_println(
        out_writer, u8_l("{:s}Blue text"),
        daterm_color_fg8bit(daterm_Palette8bit_blue, &l0$((daterm_Color_Palette8bitBuf)))
    ));

    try_(daterm_color_fg24bitWrite(97, 37, 160, out_writer));
    try_(io_Writer_println(out_writer, u8_l("Purple text")));

    let_ignore = try_(io_Reader_readByte(in_reader));
    return_ok({});
} $unguarded(fn);

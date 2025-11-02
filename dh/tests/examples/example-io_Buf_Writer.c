#include "dh/main.h"
#include "dh/io/Buf.h"
#include "dh/io/common.h"

fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    let stdout_file = io_getStdOut();
    let unbuffered  = fs_File_writer(stdout_file);

    use_A$(4096, u8);
    A$4096$u8 buf = A_zero();

    var buffered = io_Buf_Writer_init(unbuffered, A_ref$(S$u8, buf));
    defer_(catch_((io_Buf_Writer_flush(&buffered))($ignore, claim_unreachable)));

    let writer = io_Buf_Writer_writer(&buffered);
    for_(($r(0, 1000))(i) {
        try_(io_Writer_print(writer, u8_l("Line {:d}\n"), i));
    });

    return_ok({});
} $unguarded_(fn);

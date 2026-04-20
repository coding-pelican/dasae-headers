#include "dh-main.h"
#include "dh/io/Buf.h"
#include "dh/io/common.h"

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    let out_file = io_getStdOut();
    let unbufd = fs_File_writer(out_file);

    var_(buf, A$$(4096, u8)) = A_zero();
    var bufd = io_Buf_Writer_init(unbufd, A_ref$((S$u8)(buf)));
    defer_(catch_((io_Buf_Writer_flush(&bufd))($ignore, claim_unreachable)));

    let out_writer = io_Buf_writer(&bufd);
    for_(($r(0, 1000))(i)) {
        try_(io_Writer_print(out_writer, u8_l("Line {:uz}\n"), i));
    } $end(for);

    return_ok({});
} $unguarded(fn);

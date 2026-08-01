#include "dh-main.h"
#include "dh/io/Buf.h"
#include "dh/io/std.h"

fn_((main(proc_Entry entry))(E$void) $guard) {
    let_ignore = entry;
    let std = catch_((io_std_direct())($ignore, io_std_noop));
    let unbufd = io_std_out(std);

    var_(buf, A$$(4096, u8)) $undefined;
    var bufd = io_Buf_Writer_from(unbufd, A_ref$((S$u8)(buf)));
    defer_(catch_((io_Buf_Writer_flush(&bufd))($ignore, claim_unreachable)));

    let out_writer = io_Buf_writer(&bufd);
    for_(($r(0, 1000))(i)) {
        try_(io_Writer_print(out_writer, u8_l("Line {:uz}\n"), i));
    } $end(for);

    return_ok({});
} $unguarded(fn);

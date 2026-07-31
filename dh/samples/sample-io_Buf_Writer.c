#include "dh-main.h"
#include "dh/io/Buf.h"
#include "dh/proc/std.h"
#include "dh/fs/File/self.h"

fn_((main(proc_Entry entry))(E$void) $guard) {
    let out_file = proc_std_out(entry.std);
    let unbufd = fs_File_writer(out_file);

    var_(buf, A$$(4096, u8)) $undefined;
    var bufd = io_Buf_Writer_init(unbufd, A_ref$((S$u8)(buf)));
    defer_(catch_((io_Buf_Writer_flush(&bufd))($ignore, claim_unreachable)));

    let out_writer = io_Buf_writer(&bufd);
    for_(($r(0, 1000))(i)) {
        try_(io_Writer_print(out_writer, u8_l("Line {:uz}\n"), i));
    } $end(for);

    return_ok({});
} $unguarded(fn);

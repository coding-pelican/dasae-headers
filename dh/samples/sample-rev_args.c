#include "dh-main.h"
#include "dh/io/stream.h"

fn_((main(proc_Entry entry))(E$void) $scope) {
    var count_iter = proc_Args_iter(entry.args);
    var_(count, usize) = 0;
    while (try_(proc_Args_Iter_skip(&count_iter))) ++count;

    io_stream_println(u8_l("passed args:"));
    for_($rev($rt(count))(index)) {
        var iter = proc_Args_iter(entry.args);
        for_(($rt(index))($ignore)) {
            claim_assert(try_(proc_Args_Iter_skip(&iter)));
        } $end(for);
        var_(scratch, A$$(4096, u8)) $undefined;
        let arg = unwrap_(try_(proc_Args_Iter_next(
            &iter,
            A_ref$((S$u8)(scratch))
        )));
        io_stream_println(u8_l("  arg {:ul}: {:s}"), index, arg);
    } $end(for);

    return_ok({});
} $unscoped(fn);

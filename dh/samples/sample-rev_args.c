#include "dh-main.h"
#include "dh/io/stream.h"

fn_((main(proc_Self self))(E$void) $scope) {
    let args = self.args.items;
    io_stream_println(u8_l("passed args:"));
    for_($rev($s(args), $rf(0))(arg, index)) {
        io_stream_println(u8_l("  arg {:ul}: {:s}"), index, arg);
    } $end(for);

    return_ok({});
} $unscoped(fn);

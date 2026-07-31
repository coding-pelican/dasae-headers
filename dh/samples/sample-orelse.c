#include "dh-main.h"
#include "dh/io/stream.h"

fn_((main(proc_Entry entry))(E$void) $scope) {
    let_(opt, O$(i32)) = none();

    let temp = 123;
    let a = orelse_((opt)(123));
    let b = orelse_((opt)(temp));
    let c = orelse_((opt)(claim_unreachable));
    let d = orelse_((opt)({
        io_stream_eprintln(u8_l("opt is none!"));
        claim_unreachable;
    }));
    let e = orelse_((opt)((
        io_stream_eprintln(u8_l("opt is none!")), 123
    )));
    let f = unwrap_(opt);

    return_ok({});
    $unused(entry, a, b, c, d, e, f);
} $unscoped(fn);

#include "dh/core.h"
#include "dh/main.h"
#include "dh/log.h"

fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    const Opt$(i32) opt = none();

    let temp = 123;
    let a    = orelse(opt, 123);
    let b    = orelse(opt, temp);
    let c    = orelse(opt, claim_unreachable);
    let d    = orelse(opt, eval({
        log_error("opt is none!");
        claim_unreachable;
    }));
    let e    = orelse(opt, eval({
        log_error("opt is none!");
        eval_return 123;
    }));
    let f    = unwrap(opt);

    return_ok({});
    $unused(args, a, b, c, d, e, f);
} ext_unscoped;

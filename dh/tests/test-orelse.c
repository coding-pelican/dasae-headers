#include "dh/main.h"
#include "dh/log.h"

fn_(dh_main(Sli$Str_const args), Err$void, $scope) {
    let_(opt, Opt$(i32)) = none();

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
} $unscoped;

#include "dh-main.h"
#include "dh/time/self/Boot.h"

TEST_fn_("time/self/Boot: direct clock advances across sleep" $scope) {
    let clock = catch_((time_Boot_direct())(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        return_ok_void();
    }));
    let begin = time_Boot_now(clock);
    try_(time_Boot_sleepMillis(clock, 1));
    let end = time_Boot_now(clock);
    let elapsed = unwrap_(time_Boot_Inst_durSinceChkd(end, begin));

    try_(TEST_expect(cmp_ge$(time_Boot_Inst)(end, begin)));
    try_(TEST_expect(!time_Dur_isZero(elapsed)));
} $unscoped(TEST_fn);

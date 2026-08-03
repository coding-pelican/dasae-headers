#include "dh-main.h"
#include "dh/time/Clock.h"

TEST_fn_("time/Clock: variant dispatch preserves clock and instant tags" $scope) {
    let awake = catch_((time_Awake_direct())(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        return_ok_void();
    }));
    let clock = union_of$((time_Clock)(time_Clock_awake)(awake));
    let begin = time_Clock_now(clock);
    try_(time_Clock_sleepMillis(clock, 1));
    let end = time_Clock_now(clock);
    let elapsed = unwrap_(time_Clock_Inst_durSinceChkd(end, begin));

    try_(TEST_expect(matches(begin, time_Clock_awake)));
    try_(TEST_expect(matches(end, time_Clock_awake)));
    try_(TEST_expect(!time_Dur_isZero(elapsed)));
} $unscoped(TEST_fn);

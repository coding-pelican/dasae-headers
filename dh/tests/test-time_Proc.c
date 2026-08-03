#include "dh-main.h"
#include "dh/time/self/Proc.h"

TEST_fn_("time/self/Proc: direct process clock is monotonic" $scope) {
    let clock = catch_((time_Proc_direct())(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$time_direct_Unsupported));
        return_ok_void();
    }));
    let begin = time_Proc_now(clock);
    let end = time_Proc_now(clock);

    try_(TEST_expect(cmp_ge$(time_Proc_Inst)(end, begin)));
    try_(TEST_expect(isSome(time_Proc_Inst_durSinceChkd(end, begin))));
} $unscoped(TEST_fn);

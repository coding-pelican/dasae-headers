#include "test-thrd/main.h"

TEST_fn_("thrd/ftx: deadline reports timeout for unchanged word" $guard) {
    var_(word, atom_V$u32) = atom_V_init(0);
    var deadline = thrd_ftx_Deadline_init(
        some$((O$time_Dur)(time_Dur_zero))
    );

    let result = thrd_ftx_Deadline_wait(
        &deadline, &word, 0, none$((O$thrd_Wakeable))
    );
    try_(TEST_expect(isErr(result)));
} $unguarded(TEST_fn);

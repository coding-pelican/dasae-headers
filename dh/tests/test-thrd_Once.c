#include "test-thrd/main.h"

TEST_fn_("thrd/Once: successful begin publishes completion exactly once" $guard) {
    var once = thrd_Once_init();
    defer_(thrd_Once_fini(&once));

    try_(TEST_expect(!thrd_Once_isDone(&once)));
    try_(TEST_expect(thrd_Once_tryBegin(&once)));
    thrd_Once_finish(&once);
    thrd_Once_wait(&once);
    try_(TEST_expect(thrd_Once_isDone(&once)));
    try_(TEST_expect(!thrd_Once_tryBegin(&once)));
} $unguarded(TEST_fn);

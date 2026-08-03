#include "test-thrd/main.h"

TEST_fn_("thrd/Mtx: exclusive and recursive locks release ownership" $guard) {
    var mtx = thrd_Mtx_init();
    defer_(thrd_Mtx_fini(&mtx));
    try_(TEST_expect(thrd_Mtx_tryLock(&mtx)));
    thrd_Mtx_unlock(&mtx);
    try_(TEST_expect(thrd_Mtx_tryLock(&mtx)));
    thrd_Mtx_unlock(&mtx);

    var recur = thrd_Mtx_Recur_init();
    defer_(thrd_Mtx_Recur_fini(&recur));
    try_(TEST_expect(thrd_Mtx_Recur_tryLock(&recur)));
    try_(TEST_expect(thrd_Mtx_Recur_tryLock(&recur)));
    thrd_Mtx_Recur_unlock(&recur);
    thrd_Mtx_Recur_unlock(&recur);
} $unguarded(TEST_fn);

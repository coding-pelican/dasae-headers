#include "test-thrd/main.h"

TEST_fn_("thrd/Sem: each post releases one successful wait" $guard) {
    var sem = thrd_Sem_init();
    defer_(thrd_Sem_fini(&sem));

    try_(TEST_expect(!thrd_Sem_tryWait(&sem)));
    thrd_Sem_post(&sem);
    try_(TEST_expect(thrd_Sem_tryWait(&sem)));
    try_(TEST_expect(!thrd_Sem_tryWait(&sem)));
} $unguarded(TEST_fn);

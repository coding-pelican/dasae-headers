#include "dh-main.h"
#include "dh/proc/Child.h"
#include "dh/proc/Self.h"

$static fn_((test_proc_Child__fakeWait(
    P$raw ctx, proc_Child* self
))(proc_Child_Wait_E$proc_Child_Trm) $scope) {
    let calls = ptrCast$((usize*)(ensureNonnull(ctx)));
    claim_assert_nonnull(self);
    *P_at((calls)[0]) += 1;
    asg_l((&self->handle)(none()));
    self->id = 0;
    return_ok(union_of$((proc_Child_Trm)(proc_Child_Trm_exited)(u8_(42))));
} $unscoped(fn);
$static fn_((test_proc_Child__fakeKill(P$raw ctx, proc_Child* self))(void)) {
    let calls = ptrCast$((usize*)(ensureNonnull(ctx)));
    claim_assert_nonnull(self);
    *P_at((calls)[0]) += 1;
    asg_l((&self->handle)(none()));
    self->id = 0;
};

TEST_fn_("proc/Child: operations dispatch through process capability" $guard) {
    let direct = try_(proc_direct());
    let vtbl = with_((*P_at((direct.vtbl)[0]))(
        (.child)({
            .waitFn = test_proc_Child__fakeWait,
            .killFn = test_proc_Child__fakeKill,
        })
    ));
    var_(calls, usize) = 0;
    let proc = proc_ensureValid((proc_Self){
        .ctx = &calls,
        .vtbl = &vtbl,
    });
    var_(child, proc_Child) = {
        .handle = some(9),
        .id = 9,
        .io = {
            .in = none(),
            .out = none(),
            .err = none(),
        },
    };

    let trm = try_(proc_Child_wait(&child, proc));
    try_(TEST_expect(matches(trm, proc_Child_Trm_exited)));
    try_(TEST_expect(union_to((trm)(proc_Child_Trm_exited)) == 42));

    asg_l((&child.handle)(some(10)));
    child.id = 10;
    proc_Child_kill(&child, proc);
    try_(TEST_expect(calls == 2));
    try_(TEST_expect(child.id == 0));
    proc_Child_kill(&child, proc);
    try_(TEST_expect(calls == 2));
} $unguarded(TEST_fn);

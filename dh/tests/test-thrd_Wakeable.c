#include "test-thrd/main.h"

T_alias$((test_thrd_Wakeable_Ctx)(struct test_thrd_Wakeable_Ctx {
    var_(ready, bool);
    var_(linked, thrd_wait_Link*);
    var_(unlinks, usize);
}));
$static var_(test_thrd_Wakeable__unlinked, thrd_wait_Link) $undefined_static;
$static fn_((test_thrd_Wakeable__wake(P$raw ctx))(void)) {
    let_ignore = ctx;
};
$static fn_((test_thrd_Wakeable__ready(P$raw ctx))(bool)) {
    return ptrCast$((test_thrd_Wakeable_Ctx*)(ensureNonnull(ctx)))->ready;
};
$static fn_((test_thrd_Wakeable__link(P$raw ctx, thrd_wait_Link* link))(bool)) {
    let self = ptrCast$((test_thrd_Wakeable_Ctx*)(ensureNonnull(ctx)));
    self->linked = link;
    return self->ready;
};
$static fn_((test_thrd_Wakeable__unlink(P$raw ctx, thrd_wait_Link* link))(void)) {
    let self = ptrCast$((test_thrd_Wakeable_Ctx*)(ensureNonnull(ctx)));
    claim_assert(self->linked == link);
    self->linked = &test_thrd_Wakeable__unlinked;
    ++self->unlinks;
};

TEST_fn_("thrd/Wakeable: readiness and link ownership dispatch together" $scope) {
    var ctx = (test_thrd_Wakeable_Ctx){
        .ready = true,
        .linked = &test_thrd_Wakeable__unlinked,
        .unlinks = 0,
    };
    let vtbl = (thrd_Wakeable_VTbl){
        .readyFn = test_thrd_Wakeable__ready,
        .linkFn = test_thrd_Wakeable__link,
        .unlinkFn = test_thrd_Wakeable__unlink,
    };
    let wakeable = thrd_Wakeable_ensureValid((thrd_Wakeable){
        .ctx = &ctx,
        .vtbl = &vtbl,
    });
    var link = thrd_wait_Link_from(
        thrd_Waker_ensureValid((thrd_Waker){
            .ctx = &ctx,
            .wakeFn = test_thrd_Wakeable__wake,
        }),
        0
    );

    try_(TEST_expect(thrd_Wakeable_ready(wakeable)));
    try_(TEST_expect(thrd_Wakeable_link(wakeable, &link)));
    try_(TEST_expect(ctx.linked == &link));
    thrd_Wakeable_unlink(wakeable, &link);
    try_(TEST_expect(ctx.linked == &test_thrd_Wakeable__unlinked));
    try_(TEST_expect(ctx.unlinks == 1));
} $unscoped(TEST_fn);

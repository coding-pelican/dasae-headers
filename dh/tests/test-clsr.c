#include "dh-main.h"
#include "dh/clsr.h"

T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((i32)(Co_Ctx, Co_Rtn, Co_Frame));
T_use_clsr_invokeToComplete$(i32);
T_use_clsr_invokeToStep$(i32);

$static co_fn_(test__sumNoSuspend, (i32 lhs; i32 rhs), i32);
/* NOLINTNEXTLINE(hicpp-multiway-paths-covered) */
co_fn_frame_scope(
    test__sumNoSuspend,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({})
);
co_fn_scope(test__sumNoSuspend) {
    co_return_($co_arg(lhs) + $co_arg(rhs));
} $unscoped(co_fn);
co_use_Clsr_((test__sumNoSuspend)(i32, i32)(i32));

$static co_fn_(test__sumAfterSuspend, (i32 lhs; i32 rhs), i32);
co_fn_frame_scope(
    test__sumAfterSuspend,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({
        var_(idle, Void);
    })
);
co_fn_scope(test__sumAfterSuspend) {
    suspend_((idle)(Void_()));
    co_return_($co_arg(lhs) + $co_arg(rhs));
} $unscoped(co_fn);
co_use_Clsr_((test__sumAfterSuspend)(i32, i32)(i32));

TEST_fn_("clsr: no suspend direct" $scope) {
    var clsr = clsr_((test__sumNoSuspend)(19, 23));
    try_(TEST_expect(*clsr_invokeToComplete$i32(clsr.as_base) == 42));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("clsr: suspend direct invokeToComplete" $scope) {
    var clsr = clsr_((test__sumAfterSuspend)(19, 23));
    try_(TEST_expect(*clsr_invokeToComplete$i32(clsr.as_base) == 42));
    try_(TEST_expect(clsr.ctx.data.frame.ctx.ctrl.state == Co_State_ready));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("clsr: suspend step progression" $scope) {
    var clsr = clsr_((test__sumAfterSuspend)(19, 23));

    let step1 = clsr_invokeToStep$i32(clsr.as_base);
    try_(TEST_expect(!step1.is_some));
    try_(TEST_expect(clsr.ctx.data.frame.ctx.ctrl.state == Co_State_suspended));

    let step2 = clsr_invokeToStep$i32(clsr.as_base);
    try_(TEST_expect(step2.is_some));
    try_(TEST_expect(*unwrap_(step2) == 42));
    try_(TEST_expect(clsr.ctx.data.frame.ctx.ctrl.state == Co_State_ready));
    return_ok({});
} $unscoped(TEST_fn);

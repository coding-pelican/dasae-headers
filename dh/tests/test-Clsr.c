#include "dh-main.h"
#include "dh/Clsr.h"

T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use$((i32)(Co_Ctx, Co_Rtn, Co_Frame));
T_use_Clsr_invokeToComplete$(i32);
T_use_Clsr_invokeToStep$(i32);

co_fn_(test__sumNoSuspend, (i32 lhs; i32 rhs), i32);
/* NOLINTNEXTLINE(hicpp-multiway-paths-covered) */
co_fn_scope(
    test__sumNoSuspend,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({})
) {
    co_return_($co_arg(lhs) + $co_arg(rhs));
} $unscoped(co_fn);
co_use_Clsr_((test__sumNoSuspend)(i32, i32)(i32));

co_fn_(test__sumAfterSuspend, (i32 lhs; i32 rhs), i32);
co_fn_scope(
    test__sumAfterSuspend,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({
        var_(idle, Void);
    })
) {
    suspend_((idle)(Void_()));
    co_return_($co_arg(lhs) + $co_arg(rhs));
} $unscoped(co_fn);
co_use_Clsr_((test__sumAfterSuspend)(i32, i32)(i32));

TEST_fn_("Clsr: no suspend direct" $scope) {
    var clsr = clsr_(test__sumNoSuspend)(19, 23);
    try_(TEST_expect(*Clsr_invokeToComplete$i32(clsr.as_base) == 42));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("Clsr: suspend direct invokeToComplete" $scope) {
    var clsr = clsr_(test__sumAfterSuspend)(19, 23);
    try_(TEST_expect(*Clsr_invokeToComplete$i32(clsr.as_base) == 42));
    try_(TEST_expect(clsr.ctx.data.frame.ctx.ctrl.state == Co_State_ready));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("Clsr: suspend step progression" $scope) {
    var clsr = clsr_(test__sumAfterSuspend)(19, 23);

    let step1 = Clsr_invokeToStep$i32(clsr.as_base);
    try_(TEST_expect(!step1.is_some));
    try_(TEST_expect(clsr.ctx.data.frame.ctx.ctrl.state == Co_State_suspended));

    let step2 = Clsr_invokeToStep$i32(clsr.as_base);
    try_(TEST_expect(step2.is_some));
    try_(TEST_expect(*unwrap_(step2) == 42));
    try_(TEST_expect(clsr.ctx.data.frame.ctx.ctrl.state == Co_State_ready));
    return_ok({});
} $unscoped(TEST_fn);

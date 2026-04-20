#include "dh/prl.h"

#include "dh/time.h"
T_alias$((da_time_Inst_VT)(struct da_time_Inst_VT {
    fn_(((*sleep)(P_const$raw ctx, time_Duration duration))(E$void));
    fn_(((*now)(P_const$raw ctx))(time_SysTime));
}));
T_alias$((da_time_Inst)(struct da_time_Inst {
    var_(ctx, P_const$raw);
    var_(vtbl, P_const$$(da_time_Inst_VT));
}));
$extern fn_((da_time_blking(void))(da_time_Inst));
$extern fn_((da_time_thrded(void))(da_time_Inst));
$extern fn_((da_time_evented(void))(da_time_Inst));
$extern fn_((da_time_stkless(void))(da_time_Inst));


$static fn_((da_time_blking__sleep(P_const$raw ctx, time_Duration duration))(E$void) $scope) {
    let_ignore = ctx;
    return_ok_void(time_sleep(duration));
} $unscoped(fn);
$static fn_((da_time_blking__now(P_const$raw ctx))(time_SysTime)) {
    let_ignore = ctx;
    return time_SysTime_now();
};
fn_((da_time_blking(void))(da_time_Inst)) {
    $static let_(ctx, Void) = {};
    $static let_(vtbl, da_time_Inst_VT) = {
        .sleep = da_time_blking__sleep,
        .now = da_time_blking__now,
    };
    return (da_time_Inst){ .ctx = &ctx, .vtbl = &vtbl };
};
$static fn_((da_time_thrded(void))(da_time_Inst)) {
    return da_time_blking();
};
$static fn_((da_time_evented(void))(da_time_Inst)) {
    return da_time_blking();
};
$static fn_((da_time_stkless(void))(da_time_Inst)) {
    return da_time_blking();
};










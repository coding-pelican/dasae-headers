#include "dh-main.h"
#include "dh/thrd.h"
#include "dh/clsr.h"
#include "dh/time/self.h"
#include "dh/heap/Sys.h"
#include "dh/heap/ThrdSafe.h"

T_use$((i32)(
    Co_Ctx, Co_Rtn, Co_Frame,
    Clsr_Ctx, Clsr_Rtn, Clsr,
    clsr_kind, thrd_spawn, thrd_join
));
T_use$((u32)(
    Clsr_Ctx, Clsr_Rtn, Clsr,
    thrd_OnceLock,
    thrd_OnceLock_fini,
    thrd_OnceLock_isSet,
    thrd_OnceLock_trySet,
    thrd_OnceLock_wait,
    thrd_OnceLock_get,
    thrd_LazeLock,
    thrd_LazeLock_fini,
    thrd_LazeLock_isSet,
    thrd_LazeLock_getOrInit
));
$attr($maybe_unused)
$static fn_((test__double(i32 value))(i32)) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    catch_((time_Awake_sleepMillis(clock, 1))($ignore, $do_nothing));
    return value * 2;
};
fn_use_Clsr_((test__double)(i32)(i32));

$attr($maybe_unused)
$static fn_((test__markDone(atom_V$usize* counter))(Void) $scope) {
    let_ignore = atom_V_pri_fetchAdd(counter, 1, atom_MemOrd_acq_rel);
    return_void();
} $unscoped(fn);
fn_use_Clsr_((test__markDone)(atom_V$usize*)(Void));

$attr($maybe_unused)
$static fn_((test__nextLazyValue(u32* counter))(u32)) {
    *counter += 1;
    return 100 + *counter;
};
fn_use_Clsr_((test__nextLazyValue)(u32*)(u32));

$attr($maybe_unused)
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

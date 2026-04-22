#include "Sched.h"

fn_((Sched_async(Sched self, Closure$raw* closure, TypeInfo ret_ty, V$Future$raw ret_mem))(V$Future$raw)) {
    claim_assert_nonnull(closure);
    claim_assert_nonnull(ret_mem);
    debug_assert_eqBy(ret_mem->type, ret_ty, TypeInfo_eql);
    let result = Future_resultMut(ret_mem, ret_ty);
    ret_mem->any_future = self.vtbl->asyncFn(self.ctx, result, closure);
    return ret_mem;
};

fn_((Sched_spawn(Sched self, Closure$raw* closure, TypeInfo ret_ty, V$Future$raw ret_mem))(Sched_ConcE$V$Future$raw) $scope) {
    claim_assert_nonnull(closure);
    claim_assert_nonnull(ret_mem);
    debug_assert_eqBy(ret_mem->type, ret_ty, TypeInfo_eql);
    let result = Future_resultMut(ret_mem, ret_ty);
    asg_l((&ret_mem->any_future)(some(try_(self.vtbl->spawnFn(self.ctx, result, closure)))));
    return_ok(ret_mem);
} $unscoped(fn);

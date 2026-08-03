#include "dh/Future/self.h"
#include "dh/Sched/self.h"

/*========== External Definitions ===========================================*/

$static fn_((Future__await(Future$raw* self, Sched sched, TypeInfo type))(u_P_const$raw));
fn_((Future_await(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw)) {
    return u_deref(u_memmove(ret_mem.ref, Future__await(self, sched, ret_mem.type)));
};
fn_((Future__await(Future$raw* self, Sched sched, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let result = Future_resultMut(self, type);
    let any_future = orelse_((self->any_future)(return result.as_const));
    sched.vtbl->future.awaitFn(sched.ctx, any_future, result);
    asg_l((&self->any_future)(none()));
    return result.as_const;
};

$static fn_((Future__cancel(Future$raw* self, Sched sched, TypeInfo type))(u_P_const$raw));
fn_((Future_cancel(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw)) {
    return u_deref(u_memmove(ret_mem.ref, Future__cancel(self, sched, ret_mem.type)));
};
fn_((Future__cancel(Future$raw* self, Sched sched, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let result = Future_resultMut(self, type);
    let any_future = orelse_((self->any_future)(return result.as_const));
    sched.vtbl->future.cancelFn(sched.ctx, any_future, result);
    asg_l((&self->any_future)(none()));
    return result.as_const;
};

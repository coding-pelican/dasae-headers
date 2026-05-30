#include "self.h"
#include "../Sched/self.h"

fn_((Future_await(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw) $guard) {
    let type = ret_mem.type;
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let result = u_deref(Future_resultMut(self, type));
    defer_(u_memcpy(ret_mem.ref, result.ref.as_const));
    let any_future = orelse_((self->any_future)(return_(result)));
    sched.vtbl->awaitFn(sched.ctx, any_future, result.ref);
    asg_l((&self->any_future)(none()));
    return_(result);
} $unguarded(fn);

fn_((Future_cancel(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw) $guard) {
    let type = ret_mem.type;
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let result = u_deref(Future_resultMut(self, type));
    defer_(u_memcpy(ret_mem.ref, result.ref.as_const));
    let any_future = orelse_((self->any_future)(return_(result)));
    sched.vtbl->cancelFn(sched.ctx, any_future, result.ref);
    asg_l((&self->any_future)(none()));
    return_(result);
} $unguarded(fn);

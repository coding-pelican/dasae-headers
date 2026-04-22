#include "Future.h"
#include "Sched.h"

fn_((Future_await(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw)) {
    let type = ret_mem.type;
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let any_future = orelse_((self->any_future)(return ret_mem));
    sched.vtbl->awaitFn(sched.ctx, any_future, ret_mem.ref);
    asg_l((&self->any_future)(none()));
    return ret_mem;
};

fn_((Future_cancel(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw)) {
    let type = ret_mem.type;
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let any_future = orelse_((self->any_future)(return ret_mem));
    sched.vtbl->cancelFn(sched.ctx, any_future, ret_mem.ref);
    asg_l((&self->any_future)(none()));
    return ret_mem;
};

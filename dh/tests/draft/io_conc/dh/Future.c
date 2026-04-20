#include "Future.h"
#include "Sched.h"

fn_((Future_await(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw) $guard) {
    defer_(u_memcpy(ret_mem.ref, reservedReturn().ref.as_const));
    {
        let type = ret_mem.type;
        claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
        let result = Future_resultMut(self, type);
        let any_future = orelse_((self->any_future)(return_(u_deref(result))));
        sched.vtbl->awaitFn(sched.ctx, any_future, result);
        asg_l((&self->any_future)(none()));
        return_(u_deref(result));
    }
} $unguarded(fn);

fn_((Future_cancel(Future$raw* self, Sched sched, u_V$raw ret_mem))(u_V$raw) $guard) {
    defer_(u_memcpy(ret_mem.ref, reservedReturn().ref.as_const));
    {
        let type = ret_mem.type;
        claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
        let result = Future_resultMut(self, type);
        let any_future = orelse_((self->any_future)(return_(u_deref(result))));
        sched.vtbl->cancelFn(sched.ctx, any_future, result);
        asg_l((&self->any_future)(none()));
        return_(u_deref(result));
    }
} $unguarded(fn);

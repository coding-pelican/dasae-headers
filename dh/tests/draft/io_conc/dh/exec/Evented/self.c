#include "self.h"

/*========== Internal Definitions ===========================================*/

$static fn_((exec_Evented__noopFini(P$raw ctx))(void)) {
    let_ignore = ctx;
}

$static fn_((exec_Evented__noopAssociate(P$raw ctx, exec_Evented_Handle handle, usize key))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = handle;
    let_ignore = key;
    return_err(exec_Evented_E_Unsupported());
} $unscoped(fn);

$static fn_((exec_Evented__noopPoll(P$raw ctx, time_Dur timeout))(E$O$exec_Evented_Completion) $scope) {
    let_ignore = ctx;
    let_ignore = timeout;
    return_ok(none());
} $unscoped(fn);

$static fn_((exec_Evented__noopPost(P$raw ctx, exec_Evented_Completion completion))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = completion;
    return_err(exec_Evented_E_Unsupported());
} $unscoped(fn);

$static fn_((exec_Evented__noopCancel(P$raw ctx, exec_Evented_Handle handle, P$raw op))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = handle;
    let_ignore = op;
    return_err(exec_Evented_E_Unsupported());
} $unscoped(fn);

$static var_(exec_Evented__noopCtx, Void) = cleared();
$static let_(exec_Evented__noopVTbl, exec_Evented_VTbl) = {
    .finiFn = exec_Evented__noopFini,
    .associateFn = exec_Evented__noopAssociate,
    .pollFn = exec_Evented__noopPoll,
    .postFn = exec_Evented__noopPost,
    .cancelFn = exec_Evented__noopCancel,
};

let_(exec_Evented_noop, exec_Evented) = {
    .ctx = &exec_Evented__noopCtx,
    .vtbl = &exec_Evented__noopVTbl,
};

/*========== External Definitions ===========================================*/

fn_((exec_Evented_fini(exec_Evented* self))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->vtbl);
    claim_assert_nonnull(self->vtbl->finiFn);
    self->vtbl->finiFn(self->ctx);
    *self = exec_Evented_noop;
}

fn_((exec_Evented_associate(exec_Evented self, exec_Evented_Handle handle, usize key))(E$void)) {
    claim_assert_nonnull(self.vtbl);
    claim_assert_nonnull(self.vtbl->associateFn);
    return self.vtbl->associateFn(self.ctx, handle, key);
}

fn_((exec_Evented_poll(exec_Evented self, time_Dur timeout))(E$O$exec_Evented_Completion)) {
    claim_assert_nonnull(self.vtbl);
    claim_assert_nonnull(self.vtbl->pollFn);
    return self.vtbl->pollFn(self.ctx, timeout);
}

fn_((exec_Evented_post(exec_Evented self, exec_Evented_Completion completion))(E$void)) {
    claim_assert_nonnull(self.vtbl);
    claim_assert_nonnull(self.vtbl->postFn);
    return self.vtbl->postFn(self.ctx, completion);
}

fn_((exec_Evented_cancel(exec_Evented self, exec_Evented_Handle handle, P$raw op))(E$void)) {
    claim_assert_nonnull(self.vtbl);
    claim_assert_nonnull(self.vtbl->cancelFn);
    return self.vtbl->cancelFn(self.ctx, handle, op);
}

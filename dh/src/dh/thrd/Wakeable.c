#include "dh/thrd/Wakeable.h"

fn_((thrd_Wakeable_ready(thrd_Wakeable self))(bool)) {
    self = thrd_Wakeable_ensureValid(self);
    return self.vtbl->readyFn(self.ctx);
};
fn_((thrd_Wakeable_link(thrd_Wakeable self, thrd_wait_Link* link))(bool)) {
    self = thrd_Wakeable_ensureValid(self);
    claim_assert_nonnull(link);
    return self.vtbl->linkFn(self.ctx, ensureNonnull(link));
};
fn_((thrd_Wakeable_unlink(thrd_Wakeable self, thrd_wait_Link* link))(void)) {
    self = thrd_Wakeable_ensureValid(self);
    claim_assert_nonnull(link);
    self.vtbl->unlinkFn(self.ctx, ensureNonnull(link));
};

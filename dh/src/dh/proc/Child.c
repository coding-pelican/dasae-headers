#include "dh/proc/Self.h"

fn_((proc_Child_wait(
    proc_Self provider,
    proc_Child* self
))(proc_Child_Wait_E$proc_Child_Ter)) {
    proc_ensureValid(provider);
    claim_assert_nonnull(self);
    return provider.vtbl->waitFn(provider.ctx, self);
};
fn_((proc_Child_kill(proc_Self provider, proc_Child* self))(void)) {
    proc_ensureValid(provider);
    claim_assert_nonnull(self);
    provider.vtbl->killFn(provider.ctx, self);
};

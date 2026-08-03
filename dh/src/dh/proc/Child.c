#include "dh/proc/Self.h"

/*========== External Definitions ===========================================*/

fn_((proc_Child_wait(proc_Child* self, proc_Self proc))(proc_Child_Wait_E$proc_Child_Trm) $scope) {
    claim_assert_nonnull(self);
    claim_assert(isSome(self->handle));
    claim_assert(self->id != 0);
    proc = proc_ensureValid(proc);
    let terminated = try_(proc.vtbl->child.waitFn(proc.ctx, self));
    claim_assert(isNone(self->handle));
    claim_assert(self->id == 0);
    claim_assert(isNone(self->io.in));
    claim_assert(isNone(self->io.out));
    claim_assert(isNone(self->io.err));
    return_ok(terminated);
} $unscoped(fn);
fn_((proc_Child_kill(proc_Child* self, proc_Self proc))(void)) {
    claim_assert_nonnull(self);
    if_none(self->handle) {
        claim_assert(self->id == 0);
        claim_assert(isNone(self->io.in));
        claim_assert(isNone(self->io.out));
        claim_assert(isNone(self->io.err));
        return;
    }
    claim_assert(self->id != 0);
    proc = proc_ensureValid(proc);
    proc.vtbl->child.killFn(proc.ctx, self);
    claim_assert(isNone(self->handle));
    claim_assert(self->id == 0);
    claim_assert(isNone(self->io.in));
    claim_assert(isNone(self->io.out));
    claim_assert(isNone(self->io.err));
};

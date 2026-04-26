#include "Child.h"
#include "Self.h"

fn_((proc_Child_wait(proc_Child* self, proc_Self proc))(E$proc_Term)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(proc.vtbl);
    return proc.vtbl->waitFn(proc.ctx, self);
}

fn_((proc_Child_kill(proc_Child* self, proc_Self proc))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(proc.vtbl);
    proc.vtbl->killFn(proc.ctx, self);
}

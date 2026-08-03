#include "dh/proc/Env.h"
#include "dh/mem/common.h"

/*========== External Definitions ===========================================*/

$static var_(proc_Env__ctx_empty, Void) $undefined_static;
$static let_(proc_Env_VTbl_empty, proc_Env_VTbl) = {
    .nextFn = proc_Env_VTbl_emptyNext,
};
let_(proc_Env_empty, proc_Env) = {
    .ctx = &proc_Env__ctx_empty,
    .vtbl = &proc_Env_VTbl_empty,
};
fn_((proc_Env_by(proc_Env self, S_const$u8 name, S$u8 scratch))(proc_Env_E$O$S_const$u8) $scope) {
    var it = proc_Env_iter(self);
    while_some((try_(proc_Env_Iter_next(&it, scratch))), entry) {
        let separator = orelse_((mem_findFirstUnitBytes(entry, u8_c('=')))(continue));
        if (!mem_eqlBytes(S_prefix((entry)(separator)), name)) continue;
        return_ok(some(S_suffix((entry)(separator + 1))));
    }
    return_ok(none());
} $unscoped(fn);

fn_((proc_Env_iter(proc_Env self))(proc_Env_Iter)) {
    return (proc_Env_Iter){
        .src = proc_Env_ensureValid(self),
        .idx = 0,
        .offset = 0,
    };
};
fn_((proc_Env_Iter_next(proc_Env_Iter* self, S$u8 scratch))(proc_Env_E$O$S_const$u8)) {
    claim_assert_nonnull(self);
    let src = proc_Env_ensureValid(self->src);
    return src.vtbl->nextFn(
        src.ctx, &self->idx, &self->offset, scratch
    );
};

fn_((proc_Env_VTbl_emptyNext(P$raw ctx, usize* idx, usize* offset, S$u8 scratch))(proc_Env_E$O$S_const$u8) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnull(idx);
    let_ignore = ensureNonnull(offset);
    let_ignore = ensureNonnullS(scratch);
    return_ok(none());
} $unscoped(fn);

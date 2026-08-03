#include "dh/proc/Args.h"

/*========== External Definitions ===========================================*/

$static var_(proc_Args__ctx_empty, Void) $undefined_static;
$static let_(proc_Args_VTbl_empty, proc_Args_VTbl) = {
    .nextFn = proc_Args_VTbl_emptyNext,
    .skipFn = proc_Args_VTbl_emptySkip,
};
let_(proc_Args_empty, proc_Args) = {
    .ctx = &proc_Args__ctx_empty,
    .vtbl = &proc_Args_VTbl_empty,
};

fn_((proc_Args_iter(proc_Args self))(proc_Args_Iter)) {
    return (proc_Args_Iter){
        .src = proc_Args_ensureValid(self),
        .idx = 0,
        .offset = 0,
    };
};
fn_((proc_Args_Iter_next(proc_Args_Iter* self, S$u8 scratch))(proc_Args_E$O$S_const$u8)) {
    claim_assert_nonnull(self);
    let src = proc_Args_ensureValid(self->src);
    return src.vtbl->nextFn(src.ctx, &self->idx, &self->offset, scratch);
};
fn_((proc_Args_Iter_skip(proc_Args_Iter* self))(proc_Args_E$bool)) {
    claim_assert_nonnull(self);
    let src = proc_Args_ensureValid(self->src);
    return src.vtbl->skipFn(src.ctx, &self->idx, &self->offset);
};

fn_((proc_Args_VTbl_emptyNext(P$raw ctx, usize* idx, usize* offset, S$u8 scratch))(proc_Args_E$O$S_const$u8) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnull(idx);
    let_ignore = ensureNonnull(offset);
    let_ignore = ensureNonnullS(scratch);
    return_ok(none());
} $unscoped(fn);
fn_((proc_Args_VTbl_emptySkip(P$raw ctx, usize* idx, usize* offset))(proc_Args_E$bool) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnull(idx);
    let_ignore = ensureNonnull(offset);
    return_ok(false);
} $unscoped(fn);

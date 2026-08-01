#include "dh/proc/Env.h"
#include "dh/start/Invoc/Env.h"
#include "dh/mem/common.h"
#include "dh/unicode.h"

/*========== Internal Declarations ==========================================*/

T_use$((u16)(mem_findFirstUnit));

$static fn_((proc_Env__emptyNext(
    P$raw ctx,
    usize* idx,
    usize* offset,
    S$u8 scratch
))(proc_Env_E$O$S_const$u8));
$static fn_((start_Invoc_Env__next(
    P$raw ctx,
    usize* idx,
    usize* offset,
    S$u8 scratch
))(proc_Env_E$O$S_const$u8));

$static let_(proc_Env__empty_vtbl, proc_Env_VTbl) = {
    .nextFn = proc_Env__emptyNext,
};

$static let_(start_Invoc_Env__vtbl, proc_Env_VTbl) = {
    .nextFn = start_Invoc_Env__next,
};

/*========== External Definitions ===========================================*/

$static var_(proc_Env__empty_ctx, Void) $undefined_static;

let_(proc_Env_empty, proc_Env) = {
    .ctx = &proc_Env__empty_ctx,
    .vtbl = &proc_Env__empty_vtbl,
};

fn_((proc_Env_iter(proc_Env self))(proc_Env_Iter)) {
    return (proc_Env_Iter){
        .src = proc_Env_ensureValid(self),
        .idx = 0,
        .offset = 0,
    };
};

fn_((proc_Env_Iter_next(
    proc_Env_Iter* self,
    S$u8 scratch
))(proc_Env_E$O$S_const$u8)) {
    claim_assert_nonnull(self);
    let src = proc_Env_ensureValid(self->src);
    return src.vtbl->nextFn(
        src.ctx, &self->idx, &self->offset, scratch
    );
};

fn_((proc_Env_by(
    proc_Env self,
    S_const$u8 name,
    S$u8 scratch
))(proc_Env_E$O$S_const$u8) $scope) {
    var it = proc_Env_iter(self);
    while_some((try_(proc_Env_Iter_next(&it, scratch))), entry) {
        let separator = orelse_((mem_findFirstUnitBytes(entry, u8_c('=')))(continue));
        if (!mem_eqlBytes(S_prefix((entry)(separator)), name)) continue;
        return_ok(some(S_suffix((entry)(separator + 1))));
    }
    return_ok(none());
} $unscoped(fn);

fn_((start_Invoc_Env_self(start_Invoc_Env* self))(proc_Env)) {
    claim_assert_nonnull(self);
    if (matches(*self, start_Invoc_Env_empty)) return proc_Env_empty;
    return proc_Env_ensureValid((proc_Env){
        .ctx = self,
        .vtbl = &start_Invoc_Env__vtbl,
    });
};

/*========== Internal Definitions ===========================================*/

/*--- Common ---*/

$static fn_((proc_Env__emptyNext(
    P$raw ctx,
    usize* idx,
    usize* offset,
    S$u8 scratch
))(proc_Env_E$O$S_const$u8) $scope) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(idx);
    claim_assert_nonnull(offset);
    let_ignore = idx;
    let_ignore = offset;
    let_ignore = scratch;
    return_ok(none());
} $unscoped(fn);

/*--- Start Invocation ---*/

$static fn_((start_Invoc_Env__next(
    P$raw ctx,
    usize* idx,
    usize* offset,
    S$u8 scratch
))(proc_Env_E$O$S_const$u8) $scope) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(idx);
    claim_assert_nonnull(offset);
    let self = ptrCast$((start_Invoc_Env*)(ctx));
    if (matches(*self, start_Invoc_Env_empty)) return_ok(none());
    if (matches(*self, start_Invoc_Env_vec_z)) {
        let vec = union_as((self)(start_Invoc_Env_vec_z));
        if (*idx == vec->count) return_ok(none());
        let item_z = *P_at((vec->items)[(*idx)++]);
        return_ok(some(mem_spanZ0Bytes(item_z)));
    }
    let block = union_as((self)(start_Invoc_Env_wtf16))->block;
    if (*offset >= block.len || *S_at((block)[*offset]) == 0) {
        return_ok(none());
    }
    let tail = S_slice((block)$r(*offset, block.len));
    let item_len = unwrap_(mem_findFirstUnit$u16(
        tail,
        u16_(0)
    ));
    let item = S_prefix((tail)(item_len));
    let required = unicode_wtf16ToWTF8Len(item);
    if (required > scratch.len) {
        return_err(E_cause$proc_ResourceLimitReached());
    }
    let converted = unicode_wtf16ToWTF8Within(item, scratch);
    *offset += item.len + 1;
    ++*idx;
    return_ok(some(converted.as_const));
} $unscoped(fn);

#include "dh/start/Invoc/Env.h"
#include "dh/mem/common.h"
#include "dh/unicode.h"

/*========== Internal Declarations ==========================================*/

$static fn_((start_Invoc_Env__next(P$raw ctx, usize* idx, usize* offset, S$u8 scratch))(proc_Env_E$O$S_const$u8));

/*========== External Definitions ===========================================*/

fn_((start_Invoc_Env_self(start_Invoc_Env* self))(proc_Env)) {
    claim_assert_nonnull(self);
    if (matches(*self, start_Invoc_Env_empty)) return proc_Env_empty;
    $static let_(vtbl, proc_Env_VTbl) = {
        .nextFn = start_Invoc_Env__next,
    };
    return proc_Env_ensureValid((proc_Env){
        .ctx = self,
        .vtbl = &vtbl,
    });
};

/*========== Internal Definitions ===========================================*/

T_use$((u16)(mem_findFirstUnit));
$static fn_((start_Invoc_Env__next(P$raw ctx, usize* idx, usize* offset, S$u8 scratch))(proc_Env_E$O$S_const$u8) $scope) {
    claim_assert_nonnull(ctx), claim_assert_nonnull(idx), claim_assert_nonnull(offset), claim_assert_nonnullS(scratch);
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
    let item_len = unwrap_(mem_findFirstUnit$u16(tail, u16_(0)));
    let item = S_prefix((tail)(item_len));
    let required = unicode_wtf16ToWTF8Len(item);
    if (required > scratch.len) return_err(E_cause$proc_ResrcLimitReached());
    let converted = unicode_wtf16ToWTF8Within(item, scratch);
    *offset += item.len + 1;
    ++*idx;
    return_ok(some(converted.as_const));
} $unscoped(fn);

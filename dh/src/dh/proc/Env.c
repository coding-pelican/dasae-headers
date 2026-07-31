#include "dh/proc/Env.h"
#include "dh/mem/common.h"
#include "dh/unicode.h"

fn_((proc_Env_iter(proc_Env self))(proc_Env_Iter)) {
    return (proc_Env_Iter){
        .src = self,
        .idx = 0,
        .offset = 0,
    };
};

fn_((proc_Env_Iter_next(
    proc_Env_Iter* self,
    S$u8 scratch
))(proc_Env_E$O$S_const$u8) $scope) {
    claim_assert_nonnull(self);
    if (matches(self->src, proc_Env_empty)) return_ok(none());
    if (matches(self->src, proc_Env_posix)) {
        let posix = union_as((&self->src)(proc_Env_posix));
        if (self->idx == posix->count) return_ok(none());
        let item_z = *P_at((posix->items)[self->idx++]);
        return_ok(some(mem_spanZ0$u8(item_z)));
    }
    let win32 = union_as((&self->src)(proc_Env_win32));
    let block = win32->block;
    if (self->offset >= block.len || *S_at((block)[self->offset]) == 0) {
        return_ok(none());
    }
    let tail = S_slice((block)$r(self->offset, block.len));
    var_(item_len, usize) = 0;
    while (*S_at((tail)[item_len]) != 0) ++item_len;
    let item = S_prefix((tail)(item_len));
    let required = unicode_wtf16ToWTF8Len(item);
    if (required > scratch.len) {
        return_err(E_cause$proc_ResourceLimitReached());
    }
    let converted = unicode_wtf16ToWTF8Within(item, scratch);
    self->offset += item.len + 1;
    ++self->idx;
    return_ok(some(converted.as_const));
} $unscoped(fn);

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

#include "private/share.h"
#include "dh/proc/NativeStrList.h"
#include "dh/mem/common.h"
#include "dh/unicode.h"

/*========== External Definitions ===========================================*/

fn_((proc_NativeStrList_init(void))(proc_NativeStrList)) {
    return ArrList_empty(typeInfo$(P$raw));
};
fn_((proc_NativeStrList_from(mem_Alctr gpa, S$S_const$u8 items))(mem_E$proc_NativeStrList) $guard) {
    var self = proc_NativeStrList_init();
    errdefer_($ignore, proc_NativeStrList_fini(&self, gpa));
    for_(($s(items))(item)) try_(proc_NativeStrList_append(&self, gpa, *item)) $end(for);
    try_(proc_NativeStrList_finish(&self, gpa));
    return_ok(self);
} $unguarded(fn);
fn_((proc_NativeStrList_fromEnv(
    mem_Alctr gpa, proc_Env inherited, O$proc_cmd_Env override
))(mem_E$proc_NativeStrList) $guard) {
    if_some((override)(items)) return_ok(try_(proc_NativeStrList_from(gpa, items)));

    var self = proc_NativeStrList_init();
    errdefer_($ignore, proc_NativeStrList_fini(&self, gpa));
    var scratch = try_(mem_Alctr_allocBytes($trace gpa, usize_(256)));
    defer_(mem_Alctr_freeBytes($trace gpa, scratch));
    var it = proc_Env_iter(inherited);
    while (true) {
        let item = catch_((proc_Env_Iter_next(&it, scratch))($ignore, {
            try_(proc__growScratch(&scratch, gpa));
            continue;
        }));
        let entry = orelse_((item)(break));
        try_(proc_NativeStrList_append(&self, gpa, entry));
    }
    try_(proc_NativeStrList_finish(&self, gpa));
    return_ok(self);
} $unguarded(fn);
fn_((proc_NativeStrList_fini(proc_NativeStrList* self, mem_Alctr gpa))(void)) {
    claim_assert_nonnull(self);
    let ptrs = u_castS$((S$P$raw)(ArrList_itemsMut(*self, typeInfo$(P$raw))));
    for_(($s(ptrs))(ptr)) {
        if (isNull(*ptr)) break;
        let item = mem_spanZ0Bytes(as$(const u8*)(*ptr));
        mem_Alctr_freeBytes($trace gpa, P_prefix$((S$u8)(as$(P$u8)(*ptr))(item.len + usize_(1))));
    } $end(for);
    ArrList_clearAndFree(self, typeInfo$(P$raw), gpa);
    asg_l((self)(proc_NativeStrList_init()));
};

fn_((proc_NativeStrList_raw(proc_NativeStrList self))(char**)) {
    claim_assert(ArrList_len(self) != 0);
    let ptrs = u_castS$((S$P$raw)(ArrList_itemsMut(self, typeInfo$(P$raw))));
    claim_assert(isNull(*S_at((ptrs)[ptrs.len - usize_(1)])));
    return as$(char**)(ptrs.ptr);
};
fn_((proc_NativeStrList_len(proc_NativeStrList self))(usize)) {
    claim_assert(ArrList_len(self) != 0);
    return ArrList_len(self) - usize_(1);
};
fn_((proc_NativeStrList_at(proc_NativeStrList self, usize idx))(S_const$u8)) {
    let ptrs = u_castS$((S_const$P$raw)(ArrList_items(self, typeInfo$(P$raw))));
    claim_assert(idx < ptrs.len - usize_(1));
    return mem_spanZ0Bytes(as$(const u8*)(*S_at((ptrs)[idx])));
};

fn_((proc_NativeStrList_append(proc_NativeStrList* self, mem_Alctr gpa, S_const$u8 item))(mem_E$void) $guard) {
    claim_assert_nonnull(self);
    try_(ArrList_ensureUnusedCap(self, typeInfo$(P$raw), gpa, usize_(1)));

    let_(parts, A$$(2, S_const$u8)) = A_init({ item, u8_lZ0("") });
    let copy = try_(mem_joinAllocBytes(u8_l(""), A_ref$((S_const$S_const$u8)(parts)), gpa));

    ArrList_appendWithin(self, u_anyV(as$(P$raw)(copy.ptr)));
    return_ok({});
} $unguarded(fn);
fn_((proc_NativeStrList_finish(proc_NativeStrList* self, mem_Alctr gpa))(mem_E$void) $scope) {
    claim_assert_nonnull(self);
    try_(ArrList_append(self, gpa, u_anyV(as$(P$raw)(null))));
    return_ok({});
} $unscoped(fn);

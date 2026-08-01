#include "share.h"
#include "dh/mem/common.h"
#include "dh/unicode.h"

/*========== Internal Declarations ==========================================*/

$static fn_((proc__NativeStrs_init(void))(proc__NativeStrs));
$static fn_((proc__NativeStrs_append(
    proc__NativeStrs* self,
    mem_Alctr gpa,
    S_const$u8 item
))(mem_E$void));
$static fn_((proc__NativeStrs_finish(
    proc__NativeStrs* self,
    mem_Alctr gpa
))(mem_E$void));
$static fn_((proc__NativeStrs_len(proc__NativeStrs self))(usize));

/*========== External Definitions ===========================================*/

fn_((proc__growScratch(
    S$u8* self,
    mem_Alctr gpa
))(mem_E$void) $scope) {
    claim_assert_nonnull(self);
    let new_len = orelse_((
        usize_mulChkd(self->len, usize_(2))
    )(return_err(E_cause$OutOfMemory())));
    asg_l((self)(try_(
        mem_Alctr_reallocBytes($trace gpa, *self, new_len)
    )));
    return_ok({});
} $unscoped(fn);

fn_((proc__NativeStrs_fini(
    proc__NativeStrs* self,
    mem_Alctr gpa
))(void)) {
    claim_assert_nonnull(self);
    let ptrs = u_castS$((
        S$P$raw
    )(ArrList_itemsMut(*self, typeInfo$(P$raw))));
    for_(($s(ptrs))(ptr)) {
        if (isNull(*ptr)) break;
        let item = mem_spanZ0Bytes(as$(const u8*)(*ptr));
        mem_Alctr_freeBytes(
            $trace gpa,
            P_prefix$((S$u8)(as$(P$u8)(*ptr))(item.len + usize_(1)))
        );
    } $end(for);
    ArrList_clearAndFree(self, typeInfo$(P$raw), gpa);
    asg_l((self)(proc__NativeStrs_init()));
};

fn_((proc__NativeStrs_from(
    mem_Alctr gpa,
    S$S_const$u8 items
))(mem_E$proc__NativeStrs) $guard) {
    var self = proc__NativeStrs_init();
    errdefer_($ignore, proc__NativeStrs_fini(&self, gpa));
    for_(($s(items))(item)) {
        try_(proc__NativeStrs_append(&self, gpa, *item));
    } $end(for);
    try_(proc__NativeStrs_finish(&self, gpa));
    return_ok(self);
} $unguarded(fn);

fn_((proc__NativeStrs_fromEnv(
    mem_Alctr gpa,
    proc_Env inherited,
    O$proc_Cmd_Env override
))(mem_E$proc__NativeStrs) $guard) {
    if_some((override)(items)) {
        return_ok(try_(proc__NativeStrs_from(gpa, items)));
    }

    var self = proc__NativeStrs_init();
    errdefer_($ignore, proc__NativeStrs_fini(&self, gpa));
    var scratch = try_(mem_Alctr_allocBytes($trace gpa, usize_(256)));
    defer_(mem_Alctr_freeBytes($trace gpa, scratch));
    var it = proc_Env_iter(inherited);
    while (true) {
        let item = catch_((proc_Env_Iter_next(&it, scratch))(err, {
            claim_assert(E_eql(
                err.as_any,
                E_cause$proc_ResourceLimitReached().as_any
            ));
            try_(proc__growScratch(&scratch, gpa));
            continue;
        }));
        let entry = orelse_((item)(break));
        try_(proc__NativeStrs_append(&self, gpa, entry));
    }
    try_(proc__NativeStrs_finish(&self, gpa));
    return_ok(self);
} $unguarded(fn);

fn_((proc__NativeStrs_raw(proc__NativeStrs self))(char**)) {
    claim_assert(ArrList_len(self) != 0);
    let ptrs = u_castS$((
        S$P$raw
    )(ArrList_itemsMut(self, typeInfo$(P$raw))));
    claim_assert(isNull(*S_at((ptrs)[ptrs.len - usize_(1)])));
    return as$(char**)(ptrs.ptr);
};

fn_((proc__NativeStrs_at(
    proc__NativeStrs self,
    usize idx
))(S_const$u8)) {
    let ptrs = u_castS$((
        S_const$P$raw
    )(ArrList_items(self, typeInfo$(P$raw))));
    claim_assert(idx < ptrs.len - usize_(1));
    return mem_spanZ0Bytes(as$(const u8*)(*S_at((ptrs)[idx])));
};

fn_((proc__envWTF16(
    mem_Alctr gpa,
    proc_Env inherited,
    O$proc_Cmd_Env override
))(proc__envWTF16_E$S$u16) $guard) {
    var items = try_(proc__NativeStrs_fromEnv(gpa, inherited, override));
    defer_(proc__NativeStrs_fini(&items, gpa));

    let item_count = proc__NativeStrs_len(items);
    var_(len, usize) = item_count == 0 ? usize_(2) : usize_(1);
    for_(($rt(item_count))(i)) {
        let item_len = unicode_wtf8ToWTF16Len(
            proc__NativeStrs_at(items, i)
        );
        len = orelse_((
            usize_addChkd(
                len,
                orelse_((
                    usize_addChkd(item_len, usize_(1))
                )(return_err(E_cause$OutOfMemory())))
            )
        )(return_err(E_cause$OutOfMemory())));
    } $end(for);

    let bytes = u_castS$((S$u16)(try_(
        mem_Alctr_alloc($trace gpa, typeInfo$(u16), len)
    )));
    errdefer_($ignore, mem_Alctr_free($trace gpa, u_anyS(bytes)));

    var_(pos, usize) = 0;
    for_(($rt(item_count))(i)) {
        let item = proc__NativeStrs_at(items, i);
        let item_len = unicode_wtf8ToWTF16Len(item);
        let end = orelse_((
            usize_addChkd(pos, item_len)
        )(return_err(E_cause$OutOfMemory())));
        let converted = catch_((unicode_wtf8ToWTF16Within(
            item,
            S_slice((bytes)$r(pos, end))
        ))($ignore, return_err(E_cause$proc_InvalidName())));
        claim_assert(converted.len == item_len);
        pos = end;
        *S_at((bytes)[pos++]) = 0;
    } $end(for);

    if (item_count == 0) *S_at((bytes)[pos++]) = 0;
    *S_at((bytes)[pos++]) = 0;
    claim_assert(pos == len);
    return_ok(bytes);
} $unguarded(fn);

fn_((proc__child(
    proc_Child_Handle handle,
    proc_Child_Id id,
    proc_Child_IO io
))(proc_Child)) {
    return (proc_Child){
        .handle = some(handle),
        .id = id,
        .io = io,
    };
};

/*========== Internal Definitions ===========================================*/

fn_((proc__NativeStrs_init(void))(proc__NativeStrs)) {
    return ArrList_empty(typeInfo$(P$raw));
};

fn_((proc__NativeStrs_append(
    proc__NativeStrs* self,
    mem_Alctr gpa,
    S_const$u8 item
))(mem_E$void) $guard) {
    claim_assert_nonnull(self);
    try_(ArrList_ensureUnusedCap(
        self, typeInfo$(P$raw), gpa, usize_(1)
    ));

    let len = orelse_((
        usize_addChkd(item.len, usize_(1))
    )(return_err(E_cause$OutOfMemory())));
    let copy = try_(mem_Alctr_allocBytes($trace gpa, len));
    mem_copyBytes(S_prefix((copy)(item.len)), item);
    *S_at((copy)[item.len]) = 0;

    ArrList_appendWithin(self, u_anyV(as$(P$raw)(copy.ptr)));
    return_ok({});
} $unguarded(fn);

fn_((proc__NativeStrs_finish(
    proc__NativeStrs* self,
    mem_Alctr gpa
))(mem_E$void) $scope) {
    claim_assert_nonnull(self);
    try_(ArrList_append(
        self, gpa, u_anyV(as$(P$raw)(null))
    ));
    return_ok({});
} $unscoped(fn);

fn_((proc__NativeStrs_len(proc__NativeStrs self))(usize)) {
    claim_assert(ArrList_len(self) != 0);
    return ArrList_len(self) - usize_(1);
};

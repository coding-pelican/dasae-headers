#include "dh/thrd/OnceLock.h"

fn_((thrd_OnceLock_init(TypeInfo val_type, u_V$thrd_OnceLock$raw ret_mem))(u_V$thrd_OnceLock$raw)) {
    claim_assert_nonnull(ret_mem.inner);
    let ty_fields = A_ref$((S_const$TypeInfo)(with_((u_Fields_type$thrd_OnceLock)(
        (.val[u_Fields_Idx_val_$thrd_OnceLock])(val_type)
    ))));
    claim_assert(TypeInfo_eql(u_typeInfoRecord(ty_fields), ret_mem.inner_type));
    let field_once = &ret_mem.inner->once;
    *field_once = thrd_Once_init();
    let field_val_type = &ret_mem.inner->val_type;
    *field_val_type = $typing(val_type);
    let field_val_ = u_fieldPtrMut(ret_mem.ref, ty_fields, u_Fields_Idx_val_$thrd_OnceLock);
    mem_set0P(field_val_);
    return ret_mem;
};
fn_((thrd_OnceLock_fini(thrd_OnceLock$raw* self))(void)) {
    claim_assert_nonnull(self);
    thrd_Once_fini(&self->once);
};

fn_((thrd_OnceLock_isSet(const thrd_OnceLock$raw* self))(bool)) {
    claim_assert_nonnull(self);
    return thrd_Once_isDone(&self->once);
};
fn_((thrd_OnceLock_trySet(thrd_OnceLock$raw* self, u_V$raw val))(bool)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->val_type), val.type, TypeInfo_eql);
    if (thrd_Once_tryBegin(&self->once)) {
        mem_setP(thrd_OnceLock_valMut(self, val.type), val);
        thrd_Once_finish(&self->once);
        return true;
    }
    return false;
};
fn_((thrd_OnceLock_wait(thrd_OnceLock$raw* self))(void)) {
    claim_assert_nonnull(self);
    thrd_Once_wait(&self->once);
};
fn_((thrd_OnceLock_get(thrd_OnceLock$raw* self, u_V$raw ret_mem))(u_V$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->val_type), ret_mem.type, TypeInfo_eql);
    thrd_Once_wait(&self->once);
    return u_deref(mem_copyP(ret_mem.ref, thrd_OnceLock_val(self, ret_mem.type)));
};

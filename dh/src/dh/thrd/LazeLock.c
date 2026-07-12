#include "dh/thrd/LazeLock.h"

fn_((thrd_LazeLock_init(TypeInfo val_type, u_V$thrd_LazeLock$raw ret_mem))(u_V$thrd_LazeLock$raw)) {
    claim_assert_nonnull(ret_mem.inner);
    let ty_fields = A_ref$((S_const$TypeInfo)(with_((u_Fields_type$thrd_LazeLock)(
        (.val[u_Fields_Idx_val_$thrd_LazeLock])(val_type)
    ))));
    claim_assert(TypeInfo_eql(u_typeInfoRecord(ty_fields), ret_mem.inner_type));
    let field_once = &ret_mem.inner->once;
    *field_once = thrd_Once_init();
    let field_val_type = &ret_mem.inner->val_type;
    *field_val_type = $typing(val_type);
    let field_val_ = u_fieldPtrMut(ret_mem.ref, ty_fields, u_Fields_Idx_val_$thrd_LazeLock);
    mem_set0P(field_val_);
    return ret_mem;
};
fn_((thrd_LazeLock_fini(thrd_LazeLock$raw* self))(void)) {
    claim_assert_nonnull(self);
    thrd_Once_fini(&self->once);
};

fn_((thrd_LazeLock_isSet(const thrd_LazeLock$raw* self))(bool)) {
    claim_assert_nonnull(self);
    return thrd_Once_isDone(&self->once);
};
fn_((thrd_LazeLock_wait(thrd_LazeLock$raw* self))(void)) {
    claim_assert_nonnull(self);
    thrd_Once_wait(&self->once);
};
fn_((thrd_LazeLock_get(thrd_LazeLock$raw* self, TypeInfo val_type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->val_type), val_type, TypeInfo_eql);
    thrd_Once_wait(&self->once);
    return thrd_LazeLock_valMut(self, val_type);
};
fn_((thrd_LazeLock_getOrInit(thrd_LazeLock$raw* self, TypeInfo val_type, Clsr$raw* init))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->val_type), val_type, TypeInfo_eql);
    if (thrd_Once_tryBegin(&self->once)) {
        let val = clsr_invokeToComplete(init, val_type);
        mem_copyP(thrd_LazeLock_valMut(self, val_type), val);
        thrd_Once_finish(&self->once);
    } else if (!thrd_Once_isDone(&self->once)) {
        thrd_Once_wait(&self->once);
    }
    return thrd_LazeLock_valMut(self, val_type);
};

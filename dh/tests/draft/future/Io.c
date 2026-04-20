#include "Io.h"
#include "dh/meta.h"

fn_((Io_Future_result(const Io_Future* self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->result_type, type, TypeInfo_eql);
    let ty_fields = typeInfosFrom(u_anyP(self).type, type);
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, 1);
};

fn_((Io_Future_resultMut(Io_Future* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->result_type, type, TypeInfo_eql);
    let ty_fields = typeInfosFrom(u_anyP(self).type, type);
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, 1);
};

fn_((Io_Future_await(Io_Future* self, Io io, u_V$raw ret_mem))(u_V$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->result_type, ret_mem.type, TypeInfo_eql);
    let af = orelse_((self->any_future)(return u_deref(u_memcpy(ret_mem.ref, Io_Future_result(self, ret_mem.type)))));
    io.vtbl->awaitFn(io.userdata, af, Io_Future_resultMut(self, ret_mem.type));
    asg_l((&self->any_future)(none()));
    return u_deref(u_memcpy(ret_mem.ref, Io_Future_result(self, ret_mem.type)));
};

fn_((Io_Future_cancel(Io_Future* self, Io io, u_V$raw ret_mem))(u_V$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->result_type, ret_mem.type, TypeInfo_eql);
    let af = orelse_((self->any_future)(return u_deref(u_memcpy(ret_mem.ref, Io_Future_result(self, ret_mem.type)))));
    io.vtbl->cancelFn(io.userdata, af, Io_Future_resultMut(self, ret_mem.type));
    asg_l((&self->any_future)(none()));
    return u_deref(u_memcpy(ret_mem.ref, Io_Future_result(self, ret_mem.type)));
};

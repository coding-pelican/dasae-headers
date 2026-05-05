#include "bar.h"

fn_((bar_Obj_init(void))(bar_Obj)) {
    return (bar_Obj){ .name = A_init(nameOf(bar_Obj)), .name_len = u8_l(nameOf(bar_Obj)).len };
};

fn_((bar_Obj_name(bar_Obj self, bar_Obj_NameBuf* buf))(S$u8)) {
    let name = A_ref$((S$u8)(*buf));
    return pri_memcpyS(name, bar_Obj_nameRef(&self));
};

fn_((bar_Obj_nameRef(const bar_Obj* self))(S_const$u8)) {
    return S_prefix((A_ref$((S_const$u8)(self->name)))(as$(usize)(self->name_len)));
};

fn_((bar_Obj_nameMut(bar_Obj* self))(S$u8)) {
    return S_prefix((A_ref$((S$u8)(self->name)))(as$(usize)(self->name_len)));
};

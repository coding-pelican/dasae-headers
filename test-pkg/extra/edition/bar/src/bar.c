#include "bar.h"

fn_((bar_Obj_init(void))(bar_Obj)) {
    return (bar_Obj){ .name = "bar_Obj", .name_len = 3 };
}

fn_((bar_Obj_name(bar_Obj self, bar_Obj_NameBuf* buf))(S$u8)) {
    let name = A_ref$((S$u8)(*buf));
    return prim_memcpyS(name, bar_Obj_nameRef(&self));
}

fn_((bar_Obj_nameRef(const bar_Obj* self))(S_const$u8)) {
    return slice$P$((const u8)(&*self->name, $r(0, self->name_len)));
}

fn_((bar_Obj_nameMut(bar_Obj* self))(S$u8)) {
    return slice$P$((u8)(&*self->name, $r(0, self->name_len)));
}

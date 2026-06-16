#pragma once
#ifndef bar__included
#define bar__included 1

#include <dh/prl.h>

T_alias$((bar_Obj)(struct bar_Obj {
    var_(name, A$$(63, u8));
    var_(name_len, u8);
}));
$extern fn_((bar_Obj_init(void))(bar_Obj));
T_alias$((bar_Obj_NameBuf)(FieldType$(bar_Obj, name)));
$extern fn_((bar_Obj_name(bar_Obj self, bar_Obj_NameBuf* buf))(S$u8));
$extern fn_((bar_Obj_nameRef(const bar_Obj* self))(S_const$u8));
$extern fn_((bar_Obj_nameMut(bar_Obj* self))(S$u8));

#endif /* bar__included */

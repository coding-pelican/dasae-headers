#ifndef bar__included
#define bar__included 1

#include "dh/prl.h"

typedef struct bar_Obj {
    u8 name[63];
    u8 name_len;
} bar_Obj;
typedef A$$(63, u8) bar_Obj_NameBuf;
$extern fn_((bar_Obj_init(void))(bar_Obj));
$extern fn_((bar_Obj_name(bar_Obj self, bar_Obj_NameBuf* buf))(S$u8));
$extern fn_((bar_Obj_nameRef(const bar_Obj* self))(S_const$u8));
$extern fn_((bar_Obj_nameMut(bar_Obj* self))(S$u8));

#endif /* bar__included */

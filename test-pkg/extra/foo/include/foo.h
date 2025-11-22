#ifndef foo__included
#define foo__included 1

#include "dh/prl.h"

typedef struct foo_Obj {
    usize id;
    f64 value;
} foo_Obj;
$extern fn_((foo_Obj_init(void))(foo_Obj));
$extern fn_((foo_Obj_id(foo_Obj self))(usize));
$extern fn_((foo_Obj_value(foo_Obj self))(f64));

#endif /* foo__included */

#include "daterm/attr.h"

/*========== External Definitions ===========================================*/

fn_((daterm_attr_reset(void))(S_const$u8)) {
    return u8_l(daterm_attr_reset_static());
};

fn_((daterm_attr_resetWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_attr_reset());
};

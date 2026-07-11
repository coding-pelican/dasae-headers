#include "dh/conc/Mtx.h"

fn_((conc_Mtx_init(void))(conc_Mtx)) {
    return conc_Mtx_init_static();
};

fn_((conc_Mtx_fini(conc_Mtx* self))(void)) {
    *self = conc_Mtx_init_static();
};

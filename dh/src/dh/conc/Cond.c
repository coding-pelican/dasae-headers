#include "dh/conc/Cond.h"

fn_((conc_Cond_init(void))(conc_Cond)) {
    return conc_Cond_init_static();
};

fn_((conc_Cond_fini(conc_Cond* self))(void)) {
    *self = conc_Cond_init_static();
};

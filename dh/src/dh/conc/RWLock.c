#include "dh/conc/RWLock.h"

fn_((conc_RWLock_init(void))(conc_RWLock)) {
    return conc_RWLock_init_static();
};

fn_((conc_RWLock_fini(conc_RWLock* self))(void)) {
    *self = conc_RWLock_init_static();
};

#include "dh/conc/Sem.h"

fn_((conc_Sem_init(void))(conc_Sem)) {
    return conc_Sem_init_static();
};

fn_((conc_Sem_fini(conc_Sem* self))(void)) {
    *self = conc_Sem_init_static();
};

#include "target-root-contract.h"

int trc_core(void) {
    return 20;
}

int trc_total(void) {
    return trc_core() + trc_feature() + trc_internal();
}

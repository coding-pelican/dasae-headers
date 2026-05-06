#include "target-root-contract.h"

int main(void) {
    return trc_total() == 42 ? 0 : 1;
}

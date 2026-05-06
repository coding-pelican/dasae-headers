#include "A.h"
#include "B.h"

int dep_b_value(void) {
    return dep_a_value() * 2;
}

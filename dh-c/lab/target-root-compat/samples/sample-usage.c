#include "target-root-compat.h"

int main(void) {
    return trcompat_value() == 7 ? 0 : 1;
}

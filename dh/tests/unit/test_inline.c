#include "dh/builtin.h"
#include "dh/core/prim.h"
#include <stdio.h>

force_inline i32 getValueInc(i32 x);
force_inline i32 getValueInc(i32 x) {
    return x + 1;
}

force_inline i32 getValueDec(i32 x);

int main(void) {
    i32 x = 1;
    printf("x: %d\n", x);
    printf("x + 1: %d\n", getValueInc(x));
    printf("x - 1: %d\n", getValueDec(x));
    return 0;
}

force_inline i32 getValueDec(i32 x) {
    return x - 1;
}

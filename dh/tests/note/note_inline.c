#include "dh/builtin.h"
#include "dh/core/prim.h"
#include <stdio.h>

$inline_always i32 getValueInc(i32 x);
$inline_always i32 getValueInc(i32 x) {
    return x + 1;
}

$inline_always i32 getValueDec(i32 x);

int main(void) {
    i32 x = 1;
    printf("x: %d\n", x);
    printf("x + 1: %d\n", getValueInc(x));
    printf("x - 1: %d\n", getValueDec(x));
    return 0;
}

$inline_always i32 getValueDec(i32 x) {
    return x - 1;
}

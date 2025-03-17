#include "dh/core.h"

int main() {
    int idx = 123;
    int a   = sizeOf(idx++);
    a       = idx;
    printf("%d, %d\n", idx, a);
    TypeOf(idx++) b = idx;
    printf("%d, %d\n", idx, b);
    let c = make$(TypeOf(idx++), idx);
    printf("%d, %d\n", idx, c);
    var d = alignOf(TypeOf(idx++));
    d     = idx;
    printf("%d, %llu\n", idx, d);
    return 0;
}

// #include "dh/debug/assert.h"
#include <assert.h>

typedef union Ref_int {
    int value;
    int addr[1];
}* Ref_int;

int main(void) {
    int     n     = 123;
    int*    ptr_n = &n;
    Ref_int ref_n = (void*)&n;

    *ref_n->addr = 321;

    assert(n == *ptr_n && n == ref_n->value);
    assert(&n == ptr_n && &n == ref_n->addr);
    assert(n == 321);

    return 0;
}

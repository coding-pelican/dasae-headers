#include "dh/fn.h"

fn_scope_ext(deferTest(i32 a, i32 b), i32) {
    defer_(printf("deferTest1: %d + %d\n", a, b));
    defer_({
        printf("deferTest2: %d + %d\n", a, b);
        debug_assert_true(a == 1);
        debug_assert_true(b == 2);
    });
    return_(a + b);
} unscoped_ext;

#include "dh/main.h"
#include "dh/TEST.h"
#include <stdio.h>

fn_(addInt(i32 a, i32 b), i32) {
    return a + b;
}

fn_TEST_scope("Basic Addition Operation") {
    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    try_(TEST_expect(c == 3));

    let d = 3;
    let e = 4;
    let f = addInt(d, e);
    try_(TEST_expect(f != 5));
} TEST_unscoped;

fn_TEST_scope("Always Fails") {
    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    try_(TEST_expect(c != 3)); // This will always fail
} TEST_unscoped;

fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    $ignore args;
    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    printf("%d\n", c);
    return_ok({});
} ext_unscoped;

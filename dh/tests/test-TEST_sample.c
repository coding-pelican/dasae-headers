#include "dh/main.h"
#include "dh/TEST.h"

#include <stdio.h>

static fn_(addInt(i32 a, i32 b), i32) {
    return a + b;
}

TEST_fn_("Basic Addition Operation", $scope) {
    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    try_(TEST_expect(c == 3));

    let d = 3;
    let e = 4;
    let f = addInt(d, e);
    try_(TEST_expect(f != 5));
} $unscoped_TEST;

TEST_fn_("Always Fails", $scope) {
    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    try_(TEST_expect(c != 3)); // This will always fail
} $unscoped_TEST;

fn_(dh_main(Sli$Str_const args), Err$void, $scope) {
    $ignore = args;

    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    printf("%d\n", c);

    return_ok({});
} $unscoped;

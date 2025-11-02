#include "dh/TEST.h"

$static fn_((addInt(i32 a, i32 b))(i32)) {
    return a + b;
}

TEST_fn_("Basic Addition Operation" $scope) {
    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    try_(TEST_expect(c == 3));

    let d = 3;
    let e = 4;
    let f = addInt(d, e);
    try_(TEST_expect(f != 5));
} $unscoped_(TEST_fn);

TEST_fn_("Always Fails" $scope) {
    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    try_(TEST_expect(c != 3)); // This will always fail
} $unscoped_(TEST_fn);

#include "dh/main.h"
#include "dh/io/stream.h"

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    io_stream_print(u8_l("{:d}\n"), c);

    return_ok({});
} $unscoped_(fn);

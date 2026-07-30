#include "dh/TEST.h"

$static fn_((addInt(i32 lhs, i32 rhs))(i32)) {
    return lhs + rhs;
};

TEST_fn_("TEST: Basic Addition Operation" $scope) {
    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    try_(TEST_expect(c == 3));

    let d = 3;
    let e = 4;
    let f = addInt(d, e);
    try_(TEST_expect(f != 5));
} $unscoped(TEST_fn);

TEST_fn_("TEST: Always Fails" $scope) {
    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    let result = expr_(TEST_E$void $guard)({
        ETrace_disable();
        defer_(ETrace_enable());
        $break_(TEST_expect(c != 3)); // This will always fail
    }) $unguarded(expr);
    try_(TEST_expect(isErr(result)));
} $unscoped(TEST_fn);

#include "dh-main.h"
#include "dh/io/stream.h"

fn_((main(proc_Self self))(E$void) $scope) {
    let args = self.args.items;
    let_ignore = args;

    let a = 1;
    let b = 2;
    let c = addInt(a, b);
    io_stream_print(u8_l("{:d}\n"), c);

    return_ok({});
} $unscoped(fn);

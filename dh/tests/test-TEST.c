#include "dh/TEST.h"

/// Basic tests demonstrating the framework's features
TEST_fn_("Basic Math Operations" $scope) {
    // Simple boolean condition
    try_(TEST_expect(2 + 2 == 4));

    // Testing equality
    let_(a, i32) = 10;
    let_(b, i32) = 10;
    try_(TEST_expect(a == b));

    // Testing with expressions
    try_(TEST_expect(3 < 5));
    try_(TEST_expect(10 % 3 == 1));
} $unscoped(TEST_fn);

/// Test side effects of increment operators
TEST_fn_("Increment Operator Side Effects in Type System Operations" $scope) {
    var_(cnt, i32) = 123;

    let cnt_post_inc = cnt++;
    try_(TEST_expect(cnt_post_inc == 123));
    try_(TEST_expect(cnt == 124));

    let cnt_pre_inc = ++cnt;
    try_(TEST_expect(cnt_pre_inc == 125));
    try_(TEST_expect(cnt == 125));

    let cnt_post_inc_with_size_of = sizeOf$(TypeOf(cnt++)); // cnt++ side effect is not applied her
    try_(TEST_expect(cnt_post_inc_with_size_of == sizeOf$(TypeOf(cnt))));
    try_(TEST_expect(cnt == 125));

    let cnt_pre_inc_with_size_of = sizeOf$(TypeOf(++cnt)); // ++cnt side effect is not applied here
    try_(TEST_expect(cnt_pre_inc_with_size_of == sizeOf$(TypeOf(cnt))));
    try_(TEST_expect(cnt == 125));

    let_(cnt_post_inc_with_type_of, TypeOf(cnt++)) = cnt; // cnt++ side effect is not applied here
    try_(TEST_expect(cnt_post_inc_with_type_of == 125));
    try_(TEST_expect(cnt == 125));

    let_(cnt_pre_inc_with_type_of, TypeOf(++cnt)) = cnt; // ++cnt side effect is not applied here
    try_(TEST_expect(cnt_pre_inc_with_type_of == 125));
    try_(TEST_expect(cnt == 125));

    let cnt_post_inc_with_align_of = alignOf$(TypeOf(cnt++)); // cnt++ side effect is not applied here
    try_(TEST_expect(cnt_post_inc_with_align_of == alignOf$(TypeOf(cnt))));
    try_(TEST_expect(cnt == 125));

    let cnt_pre_inc_with_align_of = alignOf$(TypeOf(++cnt)); // ++cnt side effect is not applied here
    try_(TEST_expect(cnt_pre_inc_with_align_of == alignOf$(TypeOf(cnt))));
    try_(TEST_expect(cnt == 125));
} $unscoped(TEST_fn);

/// Test type comparison
TEST_fn_("Simply Type Comparison with Failing Test" $scope) {
    let_(integer, i32) = 10;
    let_(floating, f32) = 10.0f;
    try_(TEST_expect(!eqlType$(TypeOf(integer), TypeOf(floating))));
    try_(TEST_expect(eqlType$(TypeOf(integer), TypeOf(floating)))); // This will always fail
} $unscoped(TEST_fn);

#include "dh-main.h"
#include "dh/io/stream.h"

/// Sample main function
fn_((main(S$S_const$u8 args))(E$void $scope)) {
    let_ignore = args;
    io_stream_println(u8_l("Hello, world!"));
    return_ok({});
} $unscoped(fn);

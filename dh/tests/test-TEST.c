#include "dh/main.h"
#include "dh/TEST.h"

/// Basic tests demonstrating the framework's features
fn_TEST_scope("Basic Math Operations") {
    // Simple boolean condition
    try_(TEST_expect(2 + 2 == 4));

    // Testing equality
    const i32 a = 10;
    const i32 b = 10;
    try_(TEST_expect(a == b));

    // Testing with expressions
    try_(TEST_expect(3 < 5));
    try_(TEST_expect(10 % 3 == 1));
} TEST_unscoped;

/// Test side effects of increment operators
fn_TEST_scope("Side Effect") {
    i32 cnt = 123;

    let cnt_post_inc = cnt++;
    try_(TEST_expect(cnt_post_inc == 123));
    try_(TEST_expect(cnt == 124));

    let cnt_pre_inc = ++cnt;
    try_(TEST_expect(cnt_pre_inc == 125));
    try_(TEST_expect(cnt == 125));

    let cnt_post_inc_with_size_of = sizeOf(cnt++); // cnt++ side effect is not applied her
    try_(TEST_expect(cnt_post_inc_with_size_of == sizeOf(cnt)));
    try_(TEST_expect(cnt == 125));

    let cnt_pre_inc_with_size_of = sizeOf(++cnt); // ++cnt side effect is not applied here
    try_(TEST_expect(cnt_pre_inc_with_size_of == sizeOf(cnt)));
    try_(TEST_expect(cnt == 125));

    TypeOf(cnt++) cnt_post_inc_with_type_of = cnt; // cnt++ side effect is not applied here
    try_(TEST_expect(cnt_post_inc_with_type_of == 125));
    try_(TEST_expect(cnt == 125));

    TypeOf(++cnt) cnt_pre_inc_with_type_of = cnt; // ++cnt side effect is not applied here
    try_(TEST_expect(cnt_pre_inc_with_type_of == 125));
    try_(TEST_expect(cnt == 125));

    let cnt_post_inc_with_align_of = alignOf(TypeOf(cnt++)); // cnt++ side effect is not applied here
    try_(TEST_expect(cnt_post_inc_with_align_of == alignOf(TypeOf(cnt))));
    try_(TEST_expect(cnt == 125));

    let cnt_pre_inc_with_align_of = alignOf(TypeOf(++cnt)); // ++cnt side effect is not applied here
    try_(TEST_expect(cnt_pre_inc_with_align_of == alignOf(TypeOf(cnt))));
    try_(TEST_expect(cnt == 125));
} TEST_unscoped;

/// Test type comparison
fn_TEST_scope("Compare Types") {
    const i32 a = 10;
    const f32 b = 10.0f;
    try_(TEST_expect(!isSameType(TypeOf(a), TypeOf(b))));
    try_(TEST_expect(isSameType(TypeOf(a), TypeOf(b))));
} TEST_unscoped;

/// Sample main function
fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    ignore args;
    printf("Hello, World!\n");
    return_void();
} ext_unscoped;

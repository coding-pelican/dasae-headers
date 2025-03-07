#define main_no_args (1)
#include "dh/main.h"
#include "dh/core.h"
#include "dh/err_res.h"

static fn_ext_scope(alwaysFailure(void), Err$void) {
    return_err(Err_Unexpected());
} ext_unscoped;

static fn_ext_scope(func3(void), Err$void) {
    return_ok(try_(alwaysFailure()));
} ext_unscoped;

static fn_ext_scope(func2(void), Err$void) {
    return_ok(try_(func3()));
} ext_unscoped;

static fn_ext_scope(func1(void), Err$void) {
    return_ok(try_(func2()));
} ext_unscoped;

fn_ext_scope(dh_main(void), Err$void) {
    return_ok(try_(func1()));
} ext_unscoped;

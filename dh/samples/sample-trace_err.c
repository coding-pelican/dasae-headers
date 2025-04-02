#define main_no_args (1)
#include "dh/main.h"

static fn_scope(alwaysFailure(void), Err$void) {
    return_err(Err_Unexpected());
} unscoped;

static fn_scope(func3(void), Err$void) {
    return_ok(try_(alwaysFailure()));
} unscoped;

static fn_scope(func2(void), Err$void) {
    return_ok(try_(func3()));
} unscoped;

static fn_scope(func1(void), Err$void) {
    return_ok(try_(func2()));
} unscoped;

fn_scope(dh_main(void), Err$void) {
    return_ok(try_(func1()));
} unscoped;

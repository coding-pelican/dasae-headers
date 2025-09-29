#define main_no_args (1)
#include "dh/main.h"

static fn_(alwaysFailure(void), Err$void $scope) {
    return_err(Err_Unexpected());
} $unscoped;

static fn_(func3(void), Err$void $scope) {
    return_ok(try_(alwaysFailure()));
} $unscoped;

static fn_(func2(void), Err$void $scope) {
    return_ok(try_(func3()));
} $unscoped;

static fn_(func1(void), Err$void $scope) {
    return_ok(try_(func2()));
} $unscoped;

fn_(dh_main(void), Err$void $scope) {
    return_ok(try_(func1()));
} $unscoped;

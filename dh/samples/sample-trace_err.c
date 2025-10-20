#include "dh/main.h"

$static $must_check
fn_((alwaysFailure(void))(Err$void) $scope) {
    return_err(Err_Unexpected());
} $unscoped_(fn);

$static $must_check
fn_((func3(void))(Err$void) $scope) {
    return_ok(try_(alwaysFailure()));
} $unscoped_(fn);

$static $must_check
fn_((func2(void))(Err$void) $scope) {
    return_ok(try_(func3()));
} $unscoped_(fn);

$static $must_check
fn_((func1(void))(Err$void) $scope) {
    return_ok(try_(func2()));
} $unscoped_(fn);

fn_((dh_main(Sli$Sli_const$u8 args))(Err$void) $scope) {
    let_ignore = args;
    return_ok(try_(func1()));
} $unscoped_(fn);

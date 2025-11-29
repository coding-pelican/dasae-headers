#include "dh/main.h"

$attr($must_check)
$static fn_((alwaysFailure(void))(E$void) $scope) {
    return_err(Err_Unexpected());
} $unscoped_(fn);

$attr($must_check)
$static fn_((func3(void))(E$void) $scope) {
    return_ok(try_(alwaysFailure()));
} $unscoped_(fn);

$attr($must_check)
$static fn_((func2(void))(E$void) $scope) {
    return_ok(try_(func3()));
} $unscoped_(fn);

$attr($must_check)
$static fn_((func1(void))(E$void) $scope) {
    return_ok(try_(func2()));
} $unscoped_(fn);

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    return_ok(try_(func1()));
} $unscoped_(fn);

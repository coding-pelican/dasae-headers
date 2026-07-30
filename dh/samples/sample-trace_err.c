#include "dh-main.h"

$attr($must_check)
$static fn_((alwaysFailure(void))(E$void) $scope) {
    return_err(E_cause$Unexpected());
} $unscoped(fn);

$attr($must_check)
$static fn_((func3(void))(E$void) $scope) {
    return_ok(try_(alwaysFailure()));
} $unscoped(fn);

$attr($must_check)
$static fn_((func2(void))(E$void) $scope) {
    return_ok(try_(func3()));
} $unscoped(fn);

$attr($must_check)
$static fn_((func1(void))(E$void) $scope) {
    return_ok(try_(func2()));
} $unscoped(fn);

fn_((main(proc_Self self))(E$void) $scope) {
    let args = self.args.items;
    let_ignore = args;
    return_ok(try_(func1()));
} $unscoped(fn);

#include "dh-main.h"

$static fn_((test_getValueIfPositive(i32 val))(O$i32) $scope) {
    if (val <= 0) {
        return_none();
    }
    return_some(val);
} $unscoped(fn);

$static fn_((test_getCharIfNumeric(u8 val))(O$u8) $scope) {
    if (val < u8_c('0') || u8_c('9') < val) {
        return_none();
    }
    return_some(val);
} $unscoped(fn);

TEST_fn_("prl/tpl/Opt: some and none states" $scope) {
    let value = test_getValueIfPositive(10);
    let missing = test_getValueIfPositive(-10);

    try_(TEST_expect(isSome(value)));
    try_(TEST_expect(unwrap_(value) == 10));
    try_(TEST_expect(isNone(missing)));
} $unscoped(TEST_fn);

TEST_fn_("prl/tpl/Opt: if_some and else_none branches" $scope) {
    var matched_some = false;
    var matched_none = false;

    if_some((test_getValueIfPositive(10))(value)) {
        matched_some = value == 10;
    } else_none {
        matched_some = false;
    };

    if_some((test_getCharIfNumeric(u8_c('a')))(value)) {
        let_ignore = value;
        matched_none = false;
    } else_none {
        matched_none = true;
    };

    try_(TEST_expect(matched_some));
    try_(TEST_expect(matched_none));
} $unscoped(TEST_fn);

TEST_fn_("prl/tpl/Opt: if_none and else_some branches" $scope) {
    var matched_none = false;
    var matched_some = false;

    if_none((test_getValueIfPositive(-10))) {
        matched_none = true;
    } else_some(value) {
        let_ignore = value;
        matched_none = false;
    };

    if_none((test_getCharIfNumeric(u8_c('9')))) {
        matched_some = false;
    } else_some(value) {
        matched_some = value == u8_c('9');
    };

    try_(TEST_expect(matched_none));
    try_(TEST_expect(matched_some));
} $unscoped(TEST_fn);

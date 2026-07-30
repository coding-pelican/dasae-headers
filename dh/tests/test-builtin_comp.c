#include "dh-main.h"

$attr($keep_symbol $section(".dh.test.data"))
$static var_(test_builtin_comp__section_data, u32) = u32_(0x12345678);

$attr($keep_symbol $section(".dh.test.text") $inline_never)
$static fn_((test_builtin_comp__sectionFn(void))(u32)) {
    return test_builtin_comp__section_data;
};

TEST_fn_("builtin/comp: section DSL preserves data and code placement" $scope) {
    try_(TEST_expect(test_builtin_comp__sectionFn() == u32_(0x12345678)));
} $unscoped(TEST_fn);

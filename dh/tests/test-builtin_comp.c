#include "dh-main.h"

#if !comp_has_builtin(__builtin_memcpy)
#error "`comp_has_builtin` failed to detect `__builtin_memcpy`"
#endif
#if !comp_has_attribute(noreturn)
#error "`comp_has_attribute` failed to detect `noreturn`"
#endif
#if !comp_is_identifier(test_builtin_comp__identifier)
#error "`comp_is_identifier` failed to recognize an ordinary identifier"
#endif
#if comp_has_keyword(test_builtin_comp__identifier)
#error "`comp_has_keyword` classified an ordinary identifier as a keyword"
#endif

T_alias$((test_builtin_comp__Union)(union test_builtin_comp__Union {
    var_(value, u32);
}));

$attr($maybe_unused)
$static let_(test_builtin_comp__const_union, test_builtin_comp__Union) = {
    .value = 1,
};
$attr($maybe_unused)
$static let_(test_builtin_comp__const_array, A$$(2, u32)) = A_init({
    [0] = 1,
    [1] = 2,
});

$attr($keep_symbol $section(".dh.test.data"))
$static var_(test_builtin_comp__section_data, u32) = u32_(0x12345678);

$attr($keep_symbol $section(".dh.test.text") $inline_never)
$static fn_((test_builtin_comp__sectionFn(void))(u32)) {
    return test_builtin_comp__section_data;
};

TEST_fn_("builtin/comp: section DSL preserves data and code placement" $scope) {
    try_(TEST_expect(test_builtin_comp__sectionFn() == u32_(0x12345678)));
} $unscoped(TEST_fn);

TEST_fn_("builtin/comp: TypeOfUnqual removes qualifiers from unions and arrays" $scope) {
    var_(union_value, TypeOfUnqual(test_builtin_comp__const_union)) = {
        .value = 3,
    };
    union_value.value = 4;
    try_(TEST_expect(union_value.value == 4));

    var_(array_value, TypeOfUnqual(test_builtin_comp__const_array)) = A_init({
        [0] = 5,
        [1] = 6,
    });
    *A_at((array_value)[0]) = 7;
    try_(TEST_expect(*A_at((array_value)[0]) == 7));
    try_(TEST_expect(*A_at((array_value)[1]) == 6));
} $unscoped(TEST_fn);

TEST_fn_("builtin/comp: copy preserves scalar, union, and array-wrapper values" $scope) {
    $static let_(const_scalar, u32) = 7;

    var scalar = copy(const_scalar);
    scalar = 8;
    try_(TEST_expect(scalar == 8));

    var union_value = copy(test_builtin_comp__const_union);
    union_value.value = 9;
    try_(TEST_expect(union_value.value == 9));

    var anonymous_union = copy(((test_builtin_comp__Union){ .value = 11 }));
    anonymous_union.value = 12;
    try_(TEST_expect(anonymous_union.value == 12));

    var array_value = copy(test_builtin_comp__const_array);
    *A_at((array_value)[0]) = 10;
    try_(TEST_expect(*A_at((array_value)[0]) == 10));
    try_(TEST_expect(*A_at((array_value)[1]) == 2));
} $unscoped(TEST_fn);

#include "dh-main.h"
#include "dh/Ver.h"

TEST_fn_("Ver: numeric conversion preserves every component" $scope) {
    let expected_num = ver_calc(1, 2, 3, ver_label_prefix_as_num_rc, 4);
    let from_parts = Ver_from(1, 2, 3, Ver_Label_rc, 4);
    let from_num = Ver_fromNum(expected_num);

    try_(TEST_expect(Ver_intoNum(from_parts) == expected_num));
    try_(TEST_expect(Ver_intoNum(from_num) == expected_num));
    try_(TEST_expect(from_num.major == 1));
    try_(TEST_expect(from_num.minor == 2));
    try_(TEST_expect(from_num.patch == 3));
    try_(TEST_expect(from_num.label == Ver_Label_rc));
    try_(TEST_expect(from_num.label_suffix == 4));
} $unscoped(TEST_fn);

TEST_fn_("Ver: ordering follows packed version precedence" $scope) {
    let alpha = Ver_from(1, 2, 3, Ver_Label_alpha, 1);
    let beta = Ver_from(1, 2, 3, Ver_Label_beta, 1);
    let release = Ver_from(1, 2, 3, Ver_Label_release, 0);
    let next_patch = Ver_from(1, 2, 4, Ver_Label_alpha, 0);

    try_(TEST_expect(cmp_lt$(Ver)(alpha, beta)));
    try_(TEST_expect(cmp_lt$(Ver)(beta, release)));
    try_(TEST_expect(cmp_lt$(Ver)(release, next_patch)));
    try_(TEST_expect(cmp_eq$(Ver)(alpha, Ver_fromNum(Ver_intoNum(alpha)))));
    try_(TEST_expect(cmp_eql$(Ver)(alpha, Ver_fromNum(Ver_intoNum(alpha)))));
} $unscoped(TEST_fn);

TEST_fn_("Ver: environment queries expose only available compiler versions" $scope) {
    try_(TEST_expect(Ver_intoNum(Ver_self()) == ver_self_num));
    try_(TEST_expect(Ver_intoNum(Ver_comp()) == comp_ver));

#if comp_is_clang
    try_(TEST_expect(Ver_intoNum(unwrap_(Ver_clang())) == comp_clang_ver));
    try_(TEST_expect(isNone(Ver_gcc())));
#elif comp_is_gcc
    try_(TEST_expect(isNone(Ver_clang())));
    try_(TEST_expect(Ver_intoNum(unwrap_(Ver_gcc())) == comp_gcc_ver));
#else
    try_(TEST_expect(isNone(Ver_clang())));
    try_(TEST_expect(isNone(Ver_gcc())));
#endif /* comp_is_clang, comp_is_gcc */

#if lang_has_extn_gnu
    try_(TEST_expect(Ver_intoNum(unwrap_(Ver_gnu())) == comp_gnu_ver));
#else
    try_(TEST_expect(isNone(Ver_gnu())));
#endif /* lang_has_extn_gnu */
} $unscoped(TEST_fn);

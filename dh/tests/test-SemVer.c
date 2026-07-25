#include "dh-main.h"
#include "dh/SemVer.h"

$static fn_((test_SemVer_prerelease(S_const$u8 label))(SemVer_Prerelease)) {
    return SemVer_Prerelease_from(label);
};

TEST_fn_("SemVer: prerelease precedence follows canonical identifier ordering" $scope) {
    let labels = A_from$((S_const$u8){
        u8_l("alpha"),
        u8_l("alpha.1"),
        u8_l("alpha.beta"),
        u8_l("beta"),
        u8_l("beta.2"),
        u8_l("beta.11"),
        u8_l("rc.1"),
        u8_l(""),
    });

    for_(($r(1, A_len(labels)))(i)) {
        let lhs = test_SemVer_prerelease(*A_at((labels)[i - 1]));
        let rhs = test_SemVer_prerelease(*A_at((labels)[i]));
        try_(TEST_expect(cmp_lt$(SemVer_Prerelease)(lhs, rhs)));
        try_(TEST_expect(cmp_gt$(SemVer_Prerelease)(rhs, lhs)));
    } $end(for);
} $unscoped(TEST_fn);

TEST_fn_("SemVer: numeric prerelease identifiers compare without integer limits" $scope) {
    let smaller = test_SemVer_prerelease(u8_l("99999999999999999999999999999999999999"));
    let larger = test_SemVer_prerelease(u8_l("100000000000000000000000000000000000000"));
    let numeric = test_SemVer_prerelease(u8_l("123"));
    let nonnumeric = test_SemVer_prerelease(u8_l("abc"));

    try_(TEST_expect(cmp_lt$(SemVer_Prerelease)(smaller, larger)));
    try_(TEST_expect(cmp_lt$(SemVer_Prerelease)(numeric, nonnumeric)));
} $unscoped(TEST_fn);

TEST_fn_("SemVer: equal identifiers defer precedence to sequence length" $scope) {
    let shorter = test_SemVer_prerelease(u8_l("alpha.1"));
    let longer = test_SemVer_prerelease(u8_l("alpha.1.1"));
    let same = test_SemVer_prerelease(u8_l("alpha.1"));

    try_(TEST_expect(cmp_lt$(SemVer_Prerelease)(shorter, longer)));
    try_(TEST_expect(cmp_eq$(SemVer_Prerelease)(shorter, same)));
} $unscoped(TEST_fn);

#include "dh-main.h"
#include "dh/Rand.h"
#include "dh/mem/common.h"

TEST_fn_("Rand: same seed produces same stream" $scope) {
    var lhs = Rand_initSeed(123);
    var rhs = Rand_initSeed(123);

    for_(($r(0, 16))($ignore)) {
        try_(TEST_expect(Rand_next$u64(&lhs) == Rand_next$u64(&rhs)));
    } $end(for);
} $unscoped(TEST_fn)

TEST_fn_("Rand: seed initializes both xoroshiro lanes" $scope) {
    let rng = Rand_initSeed(0);
    try_(TEST_expect(rng.state != 0));
    try_(TEST_expect(rng.stream != 0));
    try_(TEST_expect(rng.state != rng.stream));
} $unscoped(TEST_fn)

TEST_fn_("Rand: fillBytes is deterministic for same seed" $scope) {
    var lhs = Rand_initSeed(456);
    var rhs = Rand_initSeed(456);

    var_(lhs_buf, A$$(16, u8)) = A_zero();
    var_(rhs_buf, A$$(16, u8)) = A_zero();
    var_(zero_buf, A$$(16, u8)) = A_zero();
    let lhs_bytes = Rand_fillBytes(&lhs, A_ref$((S$u8)(lhs_buf)));
    let rhs_bytes = Rand_fillBytes(&rhs, A_ref$((S$u8)(rhs_buf)));

    try_(TEST_expect(mem_eqlBytes(lhs_bytes.as_const, rhs_bytes.as_const)));
    try_(TEST_expect(!mem_eqlBytes(lhs_bytes.as_const, A_ref$((S_const$u8)(zero_buf)))));
} $unscoped(TEST_fn)

TEST_fn_("Rand: boolean consumes stream" $scope) {
    var lhs = Rand_initSeed(789);
    var rhs = Rand_initSeed(789);

    let b = Rand_boolean(&lhs);
    let expected = (Rand_next$u8(&rhs) & 1u) != 0;
    try_(TEST_expect(b == expected));
} $unscoped(TEST_fn)

TEST_fn_("Rand: upper bound helpers preserve exact singleton ranges" $scope) {
    var rng = Rand_initSeed(101112);

    try_(TEST_expect(Rand_lessThan$u32(&rng, 1) == 0));
    try_(TEST_expect(Rand_atMost$u16(&rng, 0) == 0));
    try_(TEST_expect(Rand_range$u8(&rng, Range_inclExcl$(u8, 7, 8)) == 7));
} $unscoped(TEST_fn)

TEST_fn_("Rand: range uses explicit inclusive and exclusive limits" $scope) {
    var rng = Rand_initSeed(131415);

    try_(TEST_expect(Rand_range$u8(&rng, Range_inclExcl$(u8, 10, 11)) == 10));
    try_(TEST_expect(Rand_range$u8(&rng, Range_exclIncl$(u8, 9, 10)) == 10));
    try_(TEST_expect(Rand_range$i8(&rng, Range_exclExcl$(i8, -2, 0)) == -1));
} $unscoped(TEST_fn)

TEST_fn_("Rand: range accepts full primitive ranges" $scope) {
    var rng = Rand_initSeed(161718);

    let_ignore = Rand_range$u8(&rng, Range_inclIncl$(u8, 0, uint_limit$(u8)));
    let full_i8 = Range_inclIncl$(i8, int_limit_min$(i8), int_limit_max$(i8));
    try_(TEST_expect(full_i8.begin.point == int_limit_min$(i8)));
    try_(TEST_expect(full_i8.end.point == int_limit_max$(i8)));
    let_ignore = Rand_range$i8(&rng, full_i8);
} $unscoped(TEST_fn)

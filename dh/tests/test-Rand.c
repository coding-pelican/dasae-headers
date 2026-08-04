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

    var_(lhs_buf, A$$(16, u8)) $undefined;
    var_(rhs_buf, A$$(16, u8)) $undefined;
    var zero_buf = A_zero$((A$$(16, u8)));
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

TEST_fn_("Rand: resetting a seed reproduces the stream" $scope) {
    var rng = Rand_initSeed(0);
    Rand_setSeed(&rng, 0x12345678);
    var expected = Rand_initSeed(0x12345678);

    for_(($r(0, 16))($ignore)) {
        try_(TEST_expect(Rand_next$u64(&rng) == Rand_next$u64(&expected)));
    } $end(for);
} $unscoped(TEST_fn)

TEST_fn_("Rand: empty byte fill preserves the stream" $scope) {
    var rng = Rand_initSeed(0x22334455);
    var expected = rng;
    var_(empty_mem, A$$(0, u8)) $undefined;

    let filled = Rand_fillBytes(&rng, A_ref$((S$u8)(empty_mem)));
    try_(TEST_expect(filled.len == 0));
    try_(TEST_expect(Rand_next$u64(&rng) == Rand_next$u64(&expected)));
} $unscoped(TEST_fn)

TEST_fn_("Rand: upper bound helpers preserve exact singleton ranges" $scope) {
    var rng = Rand_initSeed(101112);

    try_(TEST_expect(Rand_lessThan$u32(&rng, 1) == 0));
    try_(TEST_expect(Rand_atMost$u16(&rng, 0) == 0));
    try_(TEST_expect(Rand_range$u8(&rng, range$((R$u8)(incl_(7), excl_(8)))) == 7));
} $unscoped(TEST_fn)

TEST_fn_("Rand: range uses explicit inclusive and exclusive limits" $scope) {
    var rng = Rand_initSeed(131415);

    try_(TEST_expect(Rand_range$u8(&rng, range$((R$u8)(incl_(10), excl_(11)))) == 10));
    try_(TEST_expect(Rand_range$u8(&rng, range$((R$u8)(excl_(9), incl_(10)))) == 10));
    try_(TEST_expect(Rand_range$i8(&rng, range$((R$i8)(excl_(-2), excl_(0)))) == -1));
} $unscoped(TEST_fn)

TEST_fn_("Rand: range accepts full primitive ranges" $scope) {
    var rng = Rand_initSeed(161718);

    let_ignore = Rand_range$u8(&rng, range$((R$u8)(incl_(0), incl_(uint_limit$(u8)))));
    let full_i8 = range$((R$i8)(incl_(int_limit_min$(i8)), incl_(int_limit_max$(i8))));
    try_(TEST_expect(full_i8.begin.point == int_limit_min$(i8)));
    try_(TEST_expect(full_i8.end.point == int_limit_max$(i8)));
    let_ignore = Rand_range$i8(&rng, full_i8);
} $unscoped(TEST_fn)

TEST_fn_("Rand: bounded integer results stay inside requested ranges" $scope) {
    var rng = Rand_initSeed(192021);

    for_(($r(0, 256))($ignore)) {
        let u = Rand_range$u64(&rng, range$((R$u64)(incl_(17), excl_(29))));
        let i = Rand_range$i32(&rng, range$((R$i32)(excl_(-19), incl_(23))));
        try_(TEST_expect(u >= 17 && u < 29));
        try_(TEST_expect(i > -19 && i <= 23));
    } $end(for);
} $unscoped(TEST_fn)

TEST_fn_("Rand: floating results honor unit and explicit bounds" $scope) {
    var rng = Rand_initSeed(222324);

    for_(($r(0, 256))($ignore)) {
        let unit64 = Rand_next$f64(&rng);
        let unit32 = Rand_next$f32(&rng);
        let ranged64 = Rand_range$f64(&rng, range$((R$f64)(excl_(-2.0), excl_(3.0))));
        let ranged32 = Rand_range$f32(&rng, range$((R$f32)(incl_(-4.0f), incl_(5.0f))));
        try_(TEST_expect(flt_isFinite(unit64) && unit64 >= 0.0 && unit64 <= 1.0));
        try_(TEST_expect(flt_isFinite(unit32) && unit32 >= 0.0f && unit32 <= 1.0f));
        try_(TEST_expect(ranged64 > -2.0 && ranged64 < 3.0));
        try_(TEST_expect(ranged32 >= -4.0f && ranged32 <= 5.0f));
    } $end(for);
} $unscoped(TEST_fn)

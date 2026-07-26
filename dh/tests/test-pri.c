#include "dh-main.h"
#include "dh/core/cmp.h"
#include "dh/core/pri.h"

typedef enum_((test_pri_Enum $fits($packed))(
    test_pri_Enum_zero = 0,
    test_pri_Enum_answer = 42,
)) test_pri_Enum;

TEST_fn_("pri: enum construction and integer conversion preserve values" $scope) {
    let value = enum_of$((test_pri_Enum)(42));
    try_(TEST_expect(value == test_pri_Enum_answer));
    try_(TEST_expect(enumToInt$((u8)(value)) == u8_(42)));
    try_(TEST_expect(intToEnum$((test_pri_Enum)(u8_(0))) == test_pri_Enum_zero));
} $unscoped(TEST_fn);

TEST_fn_("pri: bool reductions cover any all and none" $scope) {
    try_(TEST_expect(bool_any(false, true, false)));
    try_(TEST_expect(!bool_any(false, false)));
    try_(TEST_expect(bool_all(true, true, true)));
    try_(TEST_expect(!bool_all(true, false, true)));
    try_(TEST_expect(bool_none(false, false, false)));
    try_(TEST_expect(!bool_none(false, true, false)));
} $unscoped(TEST_fn);

TEST_fn_("pri: uint static helpers cover builtin dispatch values" $scope) {
    $static let exp10 = uint_exp10_static$((u64)(19));
    try_(TEST_expect(exp10 == u64_(10, 000, 000, 000, 000, 000, 000ull)));
    $static let pow2round = uint_pow2Round_static$((u32)(23));
    try_(TEST_expect(pow2round == u32_(16)));
    $static let log10round = uint_log10Round_static(u64_(3, 163));
    try_(TEST_expect(log10round == u32_(4)));
    $static let log10floor = uint_log10Floor_static(u64_(9, 999));
    try_(TEST_expect(log10floor == u32_(3)));
    $static let log10ceil = uint_log10Ceil_static(u64_(10, 001));
    try_(TEST_expect(log10ceil == u32_(5)));
} $unscoped(TEST_fn);

TEST_fn_("pri: integer query wrappers cover single bit and ordering" $scope) {
    try_(TEST_expect(int_isZero((u32)0)));
    try_(TEST_expect(int_isNonzero((i32)-1)));
    try_(TEST_expect(int_isPow2_static((u32)16)));
    try_(TEST_expect(!int_isPow2_static((i32)-16)));
    try_(TEST_expect(int_isPow2((u32)16)));
    try_(TEST_expect(!int_isPow2((i32)-16)));
    try_(TEST_expect(int_hasSglBit_static(i8_limit_min)));
    try_(TEST_expect(int_hasSglBit((u32)0x80000000u)));
    try_(TEST_expect(int_hasSglBit(i8_limit_min)));
    try_(TEST_expect(!int_hasSglBit((u32)0)));
    try_(TEST_expect(!int_hasSglBit((u32)3)));
    try_(TEST_expect(int_eql((u16)7, (u16)7)));
    try_(TEST_expect(int_neq((i16)-7, (i16)7)));
    try_(TEST_expect(int_ord((u32)1, (u32)2) == cmp_Ord_lt));
    try_(TEST_expect(iint_sgn((i32)-2) == cmp_Sgn_ngtv));
    try_(TEST_expect(iint_sgnBit_static((i32)-2)));
    try_(TEST_expect(iint_sgnBit((i64)-1)));
} $unscoped(TEST_fn);

TEST_fn_("pri: integer overflow wrap and saturating helpers cover arithmetic edges" $scope) {
    var out_u8 = u8_(0);
    try_(TEST_expect(int_addOverflow(u8_limit_max, u8_(1), &out_u8)));
    try_(TEST_expect(out_u8 == u8_(0)));
    try_(TEST_expect(int_addWrap(u8_limit_max, u8_(2)) == u8_(1)));
    try_(TEST_expect(int_addSat(u8_limit_max, u8_(1)) == u8_limit_max));
    try_(TEST_expect(int_subOverflow(u8_(0), u8_(1), &out_u8)));
    try_(TEST_expect(out_u8 == u8_limit_max));
    try_(TEST_expect(int_subWrap(u8_(0), u8_(2)) == as$(u8)(254)));
    try_(TEST_expect(int_subSat(u8_(0), u8_(1)) == u8_limit_min));
    try_(TEST_expect(int_mulOverflow(u8_(128), u8_(2), &out_u8)));
    try_(TEST_expect(out_u8 == u8_(0)));
    try_(TEST_expect(int_mulWrap(u8_(128), u8_(2)) == u8_(0)));
    try_(TEST_expect(int_mulSat(u8_(128), u8_(2)) == u8_limit_max));

    var out_i8 = i8_(0);
    try_(TEST_expect(int_addOverflow(i8_limit_max, i8_(1), &out_i8)));
    try_(TEST_expect(out_i8 == i8_limit_min));
    try_(TEST_expect(int_addWrap(i8_limit_max, i8_(1)) == i8_limit_min));
    try_(TEST_expect(int_addSat(i8_limit_max, i8_(1)) == i8_limit_max));
    try_(TEST_expect(int_addSat(i8_limit_min, as$(i8)(-1)) == i8_limit_min));
    try_(TEST_expect(int_subOverflow(i8_limit_min, i8_(1), &out_i8)));
    try_(TEST_expect(out_i8 == i8_limit_max));
    try_(TEST_expect(int_subWrap(i8_limit_min, i8_(1)) == i8_limit_max));
    try_(TEST_expect(int_subSat(i8_limit_min, i8_(1)) == i8_limit_min));
    try_(TEST_expect(int_subSat(i8_limit_max, as$(i8)(-1)) == i8_limit_max));
    try_(TEST_expect(int_mulOverflow(i8_(64), i8_(2), &out_i8)));
    try_(TEST_expect(out_i8 == i8_limit_min));
    try_(TEST_expect(int_mulWrap(i8_(64), i8_(2)) == i8_limit_min));
    try_(TEST_expect(int_mulSat(i8_(64), i8_(2)) == i8_limit_max));
    try_(TEST_expect(int_mulSat(as$(i8)(-64), i8_(2)) == i8_limit_min));
    try_(TEST_expect(iint_negOverflow(i8_limit_min, &out_i8)));
    try_(TEST_expect(out_i8 == i8_limit_min));
    try_(TEST_expect(!iint_negOverflow(i8_(7), &out_i8)));
    try_(TEST_expect(out_i8 == as$(i8)(-7)));
    try_(TEST_expect(iint_negWrap(i8_limit_min) == i8_limit_min));
    try_(TEST_expect(iint_negSat(i8_limit_min) == i8_limit_max));
    try_(TEST_expect(iint_absOverflow(i8_limit_min, &out_i8)));
    try_(TEST_expect(out_i8 == i8_limit_min));
    try_(TEST_expect(!iint_absOverflow(as$(i8)(-7), &out_i8)));
    try_(TEST_expect(out_i8 == i8_(7)));
    try_(TEST_expect(iint_absSat(i8_limit_min) == i8_limit_max));
    try_(TEST_expect(iint_absSat(as$(i8)(-7)) == i8_(7)));
} $unscoped(TEST_fn);

TEST_fn_("pri: integer bit helpers cover builtin wrappers and bit ranges" $scope) {
    var carry = u32_(0);
    var add_res = uint_addCarry(u32_limit_max, u32_(1), u32_(0), &carry);
    try_(TEST_expect(add_res == u32_(0)));
    try_(TEST_expect(carry == u32_(1)));

    var borrow = u64_(0);
    var sub_res = uint_subBorrow(u64_(0), u64_(1), u64_(0), &borrow);
    try_(TEST_expect(sub_res == u64_limit_max));
    try_(TEST_expect(borrow == u64_(1)));

    try_(TEST_expect(iint_leadingRedundantSgnBits((i8)-1) == u32_(7)));
    try_(TEST_expect(iint_leadingRedundantSgnBits((i8)1) == u32_(6)));
    try_(TEST_expect(int_firstSetBit(u32_(0x10)) == u32_(5)));
    try_(TEST_expect(int_firstSetBit(u32_(0)) == u32_(0)));
    try_(TEST_expect(int_lastSetBit(u32_(0x10)) == u32_(5)));
    try_(TEST_expect(int_bitWidth(u32_(0x10)) == u32_(5)));
    try_(TEST_expect(int_bitWidth(u32_(0)) == u32_(0)));
    try_(TEST_expect(int_parity(u32_(0b1011))));
    try_(TEST_expect(!int_parity(u32_(0b1001))));
    try_(TEST_expect(int_bitFloor(u32_(23)) == u32_(16)));
    try_(TEST_expect(int_bitCeil(u32_(24)) == u32_(32)));
    try_(TEST_expect(int_hasBit(u32_(0x10), u32_(4))));
    try_(TEST_expect(!int_hasBit(u32_(0x10), u32_(3))));
    try_(TEST_expect(int_extractBits(u32_(0b110110), u32_(1), u32_(3)) == u32_(0b011)));
    try_(TEST_expect(int_replaceBits(u32_(0b110110), u32_(1), u32_(3), u32_(0b101)) == u32_(0b111010)));
} $unscoped(TEST_fn);

TEST_fn_("pri: float classification and comparison wrappers are implemented" $scope) {
    let sub_f32 = __builtin_bit_cast(f32, u32_(1));
    let sub_f64 = __builtin_bit_cast(f64, u64_(1));
    try_(TEST_expect(flt_isSubnormal(sub_f32)));
    try_(TEST_expect(flt_isSubnormal(sub_f64)));
    try_(TEST_expect(!flt_isSubnormal((f32)0.0f)));
    try_(TEST_expect(flt_isZero((f32)-0.0f)));
    try_(TEST_expect(!flt_isNonzero((f64)-0.0)));
    try_(TEST_expect(flt_isNonzero((f64)1.0)));
    try_(TEST_expect(flt_eql((f32)1.0f, (f32)1.0f)));
    try_(TEST_expect(flt_neq((f64)1.0, (f64)2.0)));
    try_(TEST_expect(flt_ord((f64)1.0, (f64)2.0) == cmp_Ord_lt));
    try_(TEST_expect(flt_sgn((f64)-2.0) == cmp_Sgn_ngtv));
    $static let sgnbit = flt_sgnBit_static((f32)-0.0f);
    try_(TEST_expect(sgnbit));
    try_(TEST_expect(flt_sgnBit((f64)-0.0)));
    $static let inv = flt_inv_static((f64)4.0);
    try_(TEST_expect(inv == (f64)0.25));
    try_(TEST_expect(flt_inv((f32)4.0f) == (f32)0.25f));
} $unscoped(TEST_fn);

TEST_fn_("pri: float builtin wrappers cover logs rounding and decomposition" $scope) {
    try_(TEST_expect(flt_log2((f64)8.0) == (f64)3.0));
    try_(TEST_expect(flt_exp10((f64)3.0) == (f64)1000.0));
    try_(TEST_expect(flt_log1p((f64)0.0) == (f64)0.0));
    try_(TEST_expect(flt_expm1((f64)0.0) == (f64)0.0));
    try_(TEST_expect(flt_fdim((f32)5.0f, (f32)3.0f) == (f32)2.0f));

    var exp = i32_(0);
    try_(TEST_expect(flt_frexp((f64)8.0, &exp) == (f64)0.5));
    try_(TEST_expect(exp == i32_(4)));

    var int_part = f64_(0.0);
    try_(TEST_expect(flt_modf((f64)3.25, &int_part) == (f64)0.25));
    try_(TEST_expect(int_part == (f64)3.0));

    var quo = i32_(0);
    try_(TEST_expect(flt_remquo((f64)5.0, (f64)2.0, &quo) == (f64)1.0));
    try_(TEST_expect(quo != i32_(0)));

    try_(TEST_expect(flt_nearbyint((f64)2.0) == (f64)2.0));
    try_(TEST_expect(flt_rint((f64)2.0) == (f64)2.0));
    try_(TEST_expect(flt_lround((f64)2.5) == ilong_(3)));
    try_(TEST_expect(flt_llround((f64)2.5) == i64_(3)));
    try_(TEST_expect(flt_lrint((f64)2.0) == ilong_(2)));
    try_(TEST_expect(flt_llrint((f64)2.0) == i64_(2)));
    try_(TEST_expect(flt_scalbn((f64)1.5, i32_(2)) == (f64)6.0));
    try_(TEST_expect(flt_scalbln((f64)1.5, ilong_(2)) == (f64)6.0));
    try_(TEST_expect(flt_nextUp((f64)0.0) > (f64)0.0));
    try_(TEST_expect(flt_nextDown((f64)0.0) < (f64)0.0));
    try_(TEST_expect(flt_fract((f64)3.25) == (f64)0.25));
    try_(TEST_expect(flt_isIntegral((f64)3.0)));
    try_(TEST_expect(!flt_isIntegral((f64)3.25)));
} $unscoped(TEST_fn);

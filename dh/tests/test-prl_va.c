#include "dh-main.h"
#include "dh/clsr.h"
#include "dh/prl/va.h"
#include "dh/u-meta.h"

T_use$((bool)(Clsr_Ctx, Clsr_Rtn, Clsr));
T_use_clsr_invokeToComplete$(bool);

$attr($inline_never)
$static fn_((test_prl_va_runtimeI16(void))(i16)) {
    return i16_(7);
};

$static fn_((test_prl_va_matches(u16 expected_mask, $va_args))(bool));
$static fn_((test_prl_va_count(u16 expected_mask, usize expected_count, $va_args))(bool));
fn_use_Clsr_((test_prl_va_matches)(u16, $va_args_decl)(bool));
fn_use_Clsr_((test_prl_va_count)(u16, usize, $va_args_decl)(bool));

fn_((test_prl_va_matches(u16 expected_mask, $va_args))(bool)) {
    if ($va_comptime_mask != expected_mask || $va_tup.fields.len != 3) return false;
    if (!TypeInfo_eql(u_Tup_type($va_tup), u_typeInfoRecord($va_tup.fields))) return false;

    let first = u_Tup_fieldPtr($va_tup, 0);
    let second = u_Tup_fieldPtr($va_tup, 1);
    let third = u_Tup_fieldPtr($va_tup, 2);
    return TypeInfo_eql(first.type, typeInfo$(u8))
        && TypeInfo_eql(second.type, typeInfo$(i16))
        && TypeInfo_eql(third.type, typeInfo$(u32))
        && *u_castP$((const u8*)(first)) == u8_(3)
        && *u_castP$((const i16*)(second)) == i16_(7)
        && *u_castP$((const u32*)(third)) == u32_(42);
};
fn_((test_prl_va_count(u16 expected_mask, usize expected_count, $va_args))(bool)) {
    return $va_comptime_mask == expected_mask && $va_tup.fields.len == expected_count;
};

TEST_fn_("prl/va: tuple metadata and comptime mask preserve argument order" $scope) {
    try_(TEST_expect(va_((test_prl_va_matches)(u16_(0b101))(
        u8_(3),
        test_prl_va_runtimeI16(),
        u32_(42)
    ))));
} $unscoped(TEST_fn);

TEST_fn_("prl/va: empty and maximum argument sets preserve mask bounds" $scope) {
    try_(TEST_expect(va_((test_prl_va_count)(u16_(0), usize_(0))())));
    try_(TEST_expect(va_((test_prl_va_count)(u16_(0xffff), usize_(16))(
        u8_(0), u8_(1), u8_(2), u8_(3),
        u8_(4), u8_(5), u8_(6), u8_(7),
        u8_(8), u8_(9), u8_(10), u8_(11),
        u8_(12), u8_(13), u8_(14), u8_(15)
    ))));
} $unscoped(TEST_fn);

TEST_fn_("prl/va: closure keeps typed variadic arguments for its caller block" $scope) {
    var empty_clsr = va_clsr_((test_prl_va_count)(u16_(0), usize_(0))());
    var clsr = va_clsr_((test_prl_va_matches)(u16_(0b101))(
        u8_(3),
        test_prl_va_runtimeI16(),
        u32_(42)
    ));
    try_(TEST_expect(*clsr_invokeToComplete$bool(empty_clsr.as_base)));
    try_(TEST_expect(*clsr_invokeToComplete$bool(clsr.as_base)));
} $unscoped(TEST_fn);

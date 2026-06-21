#include "dh-main.h"
#include "dh/prl/int.h"

TEST_fn_("prl/int: ulong implements every arithmetic family" $scope) {
    try_(TEST_expect(ulong_add(2, 3) == 5));
    try_(TEST_expect(ulong_sub(5, 3) == 2));
    try_(TEST_expect(ulong_mul(3, 4) == 12));
    try_(TEST_expect(ulong_div(12, 3) == 4));
    try_(TEST_expect(ulong_mod(13, 4) == 1));
    try_(TEST_expect(ulong_pow(3, 3) == 27));
    try_(TEST_expect(ulong_shl(1, 3) == 8));
    try_(TEST_expect(ulong_shr(8, 3) == 1));

    try_(TEST_expect(unwrap_(ulong_addChkd(2, 3)) == 5));
    try_(TEST_expect(unwrap_(ulong_subChkd(5, 3)) == 2));
    try_(TEST_expect(unwrap_(ulong_mulChkd(3, 4)) == 12));
    try_(TEST_expect(unwrap_(ulong_divChkd(12, 3)) == 4));
    try_(TEST_expect(unwrap_(ulong_modChkd(13, 4)) == 1));
    try_(TEST_expect(unwrap_(ulong_powChkd(3, 3)) == 27));
    try_(TEST_expect(unwrap_(ulong_shlChkd(1, 3)) == 8));
    try_(TEST_expect(unwrap_(ulong_shrChkd(8, 3)) == 1));
    try_(TEST_expect(isNone(ulong_addChkd(ulong_limit_max, 1))));

    try_(TEST_expect(ulong_addWrap(ulong_limit_max, 1) == 0));
    try_(TEST_expect(ulong_subWrap(0, 1) == ulong_limit_max));
    try_(TEST_expect(ulong_mulWrap(ulong_limit_max, 2) == ulong_limit_max - 1));
    try_(TEST_expect(ulong_shlWrap(1, as$(u32)(sizeOf$(ulong) * 8)) == 1));
    try_(TEST_expect(ulong_shrWrap(1, as$(u32)(sizeOf$(ulong) * 8)) == 1));

    try_(TEST_expect(ulong_addSat(ulong_limit_max, 1) == ulong_limit_max));
    try_(TEST_expect(ulong_subSat(0, 1) == 0));
    try_(TEST_expect(ulong_mulSat(ulong_limit_max, 2) == ulong_limit_max));
    try_(TEST_expect(ulong_powSat(ulong_limit_max, 2) == ulong_limit_max));
} $unscoped(TEST_fn)

TEST_fn_("prl/int: ilong implements every arithmetic family" $scope) {
    try_(TEST_expect(ilong_add(2, 3) == 5));
    try_(TEST_expect(ilong_sub(5, 3) == 2));
    try_(TEST_expect(ilong_mul(-3, 4) == -12));
    try_(TEST_expect(ilong_div(-12, 3) == -4));
    try_(TEST_expect(ilong_mod(-13, 4) == -1));
    try_(TEST_expect(ilong_neg(-3) == 3));
    try_(TEST_expect(ilong_abs(-3) == 3));
    try_(TEST_expect(ilong_pow(-3, 3) == -27));
    try_(TEST_expect(ilong_shl(1, 3) == 8));
    try_(TEST_expect(ilong_shr(8, 3) == 1));

    try_(TEST_expect(unwrap_(ilong_addChkd(2, 3)) == 5));
    try_(TEST_expect(unwrap_(ilong_subChkd(5, 3)) == 2));
    try_(TEST_expect(unwrap_(ilong_mulChkd(-3, 4)) == -12));
    try_(TEST_expect(unwrap_(ilong_divChkd(-12, 3)) == -4));
    try_(TEST_expect(unwrap_(ilong_modChkd(-13, 4)) == -1));
    try_(TEST_expect(unwrap_(ilong_negChkd(-3)) == 3));
    try_(TEST_expect(unwrap_(ilong_absChkd(-3)) == 3));
    try_(TEST_expect(unwrap_(ilong_powChkd(-3, 3)) == -27));
    try_(TEST_expect(unwrap_(ilong_shlChkd(1, 3)) == 8));
    try_(TEST_expect(unwrap_(ilong_shrChkd(8, 3)) == 1));
    try_(TEST_expect(isNone(ilong_addChkd(ilong_limit_max, 1))));

    try_(TEST_expect(ilong_addWrap(2, 3) == 5));
    try_(TEST_expect(ilong_subWrap(5, 3) == 2));
    try_(TEST_expect(ilong_mulWrap(-3, 4) == -12));
    try_(TEST_expect(ilong_negWrap(-3) == 3));
    try_(TEST_expect(ilong_shlWrap(1, as$(u32)(sizeOf$(ilong) * 8)) == 1));
    try_(TEST_expect(ilong_shrWrap(1, as$(u32)(sizeOf$(ilong) * 8)) == 1));

    try_(TEST_expect(ilong_addSat(ilong_limit_max, 1) == ilong_limit_max));
    try_(TEST_expect(ilong_subSat(ilong_limit_min, 1) == ilong_limit_min));
    try_(TEST_expect(ilong_mulSat(ilong_limit_max, 2) == ilong_limit_max));
    try_(TEST_expect(ilong_divSat(ilong_limit_min, -1) == ilong_limit_max));
    try_(TEST_expect(ilong_negSat(ilong_limit_min) == ilong_limit_max));
    try_(TEST_expect(ilong_absSat(ilong_limit_min) == ilong_limit_max));
    try_(TEST_expect(ilong_powSat(ilong_limit_max, 2) == ilong_limit_max));
} $unscoped(TEST_fn)

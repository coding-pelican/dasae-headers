#include "dh-main.h"
#include "dh/simd.h"

TEST_fn_("simd: runtime lane sequence operations" $scope) {
    T_use_simd_V$(4, u32);

    let base = l$((simd_V$(4, u32)){ .val = { 10, 20, 30, 40 } });
    let shifted_l = simd_shiftElemsLeft(base, 2, 99);
    let shifted_r = simd_shiftElemsRight(base, 2, 99);
    let rotated_l = simd_rotateElemsLeft(base, 1);
    let rotated_r = simd_rotateElemsRight(base, 1);
    let reversed = simd_reverseOrder(base);

    try_(TEST_expect(simd_suggestLen$(u32) > 0));
    try_(TEST_expect(*simd_V_at((shifted_l)[0]) == 30));
    try_(TEST_expect(*simd_V_at((shifted_l)[3]) == 99));
    try_(TEST_expect(*simd_V_at((shifted_r)[0]) == 99));
    try_(TEST_expect(*simd_V_at((shifted_r)[3]) == 20));
    try_(TEST_expect(*simd_V_at((rotated_l)[0]) == 20));
    try_(TEST_expect(*simd_V_at((rotated_r)[0]) == 40));
    try_(TEST_expect(*simd_V_at((reversed)[0]) == 40));
} $unscoped(TEST_fn);

TEST_fn_("simd: runtime lane search operations" $scope) {
    T_use_simd_V$(4, u32);

    let values = l$((simd_V$(4, u32)){ .val = { 1, 2, 1, 1 } });
    let matches = simd_V_int_eq(values, simd_V_splat(values, 1));
    let first = simd_bool_findFirst(matches);
    let last = simd_findLast(values, 1);

    try_(TEST_expect(unwrap_(first) == 0));
    try_(TEST_expect(unwrap_(last) == 3));
    try_(TEST_expect(simd_bool_count(matches) == 3));
    try_(TEST_expect(simd_count(values, 1) == 3));
} $unscoped(TEST_fn);

TEST_fn_("simd: gather scatter and masked memory operations" $scope) {
    T_use_simd_V$(4, u32);

    var src = A_from$((u32){ 10, 20, 30, 40, 50, 60 });
    let indices = l$((simd_V$(4, u32)){ .val = { 5, 3, 1, 0 } });
    let gathered = simd_gather$((simd_V$(4, u32))(A_ptr(src), indices));
    let mask = l$((simd_V$(4, u32)){ .val = { 1, 0, 1, 0 } });
    let defaults = simd_V_splat(gathered, 99);
    let loaded = simd_loadMask(A_ptr(src), mask, defaults);
    var dst = A_from$((u32){ 0, 0, 0, 0, 0, 0 });
    simd_scatter(A_ptr(dst), indices, gathered);
    simd_storeMask(A_ptr(dst), mask, gathered);

    try_(TEST_expect(*simd_V_at((gathered)[0]) == 60));
    try_(TEST_expect(*simd_V_at((gathered)[3]) == 10));
    try_(TEST_expect(*simd_V_at((loaded)[0]) == 10));
    try_(TEST_expect(*simd_V_at((loaded)[1]) == 99));
    try_(TEST_expect(*A_at((dst)[5]) == 60));
    try_(TEST_expect(*A_at((dst)[2]) == 20));
} $unscoped(TEST_fn);

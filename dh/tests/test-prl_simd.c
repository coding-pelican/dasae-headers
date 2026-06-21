#include "dh-main.h"
#include "dh/prl/simd.h"

TEST_fn_("prl/simd: construct and inspect lanes" $scope) {
    T_use_simd_V$(4, i32);
    T_use_simd_V$(4, u32);
    T_use_simd_V$(4, f32);
    T_use_simd_V$(4, f64);

    let v = l$((simd_V$(4, i32)){ .val = { 1, 2, 3, 4 } });
    let init = simd_V_init$((simd_V$(4, i32)){ 5, 6, 7, 8 });
    let inferred = simd_V_from$((i32){ 9, 10, 11, 12 });
    let two = simd_V_splat(v, 2);
    let arr = simd_V_toA(v);
    var src = A_from$((i32){ 13, 14, 15, 16 });
    var dst = A_zero$((A$(4, i32)));
    let loaded = simd_V_load$((simd_V$(4, i32))(A_ptr(src)));
    let as_f32 = simd_V_as$((simd_V$(4, f32))(loaded));
    let int_casted = simd_V_intCast$((simd_V$(4, u32))(loaded));
    let int_to_flt = simd_V_intToFlt$((simd_V$(4, f32))(loaded));
    let flt_to_int = simd_V_fltToInt$((simd_V$(4, i32))(int_to_flt));
    let flt_casted = simd_V_fltCast$((simd_V$(4, f64))(int_to_flt));
    simd_V_store(A_ptr(dst), loaded);

    try_(TEST_expect(simd_V_len(v) == 4));
    try_(TEST_expect(*simd_V_at((v)[0]) == 1));
    try_(TEST_expect(*simd_V_at((v)[3]) == 4));
    try_(TEST_expect(*simd_V_at((init)[0]) == 5));
    try_(TEST_expect(*simd_V_at((init)[3]) == 8));
    try_(TEST_expect(*simd_V_at((inferred)[0]) == 9));
    try_(TEST_expect(*simd_V_at((inferred)[3]) == 12));
    try_(TEST_expect(*simd_V_at((two)[0]) == 2));
    try_(TEST_expect(*simd_V_at((two)[3]) == 2));
    try_(TEST_expect(A_len(arr) == 4));
    try_(TEST_expect(*A_at((arr)[2]) == 3));
    try_(TEST_expect(*simd_V_at((loaded)[0]) == 13));
    try_(TEST_expect(*simd_V_at((loaded)[3]) == 16));
    try_(TEST_expect(*A_at((dst)[2]) == 15));
    try_(TEST_expect(*simd_V_at((as_f32)[1]) == 14.0f));
    try_(TEST_expect(*simd_V_at((int_casted)[2]) == 15u));
    try_(TEST_expect(*simd_V_at((int_to_flt)[3]) == 16.0f));
    try_(TEST_expect(*simd_V_at((flt_to_int)[0]) == 13));
    try_(TEST_expect(*simd_V_at((flt_casted)[1]) == 14.0));
} $unscoped(TEST_fn);

TEST_fn_("prl/simd: compress boolean lanes to bit mask" $scope) {
    T_use_simd_V$(16, u8);

    let lanes = l$((simd_V$(16, u8)){ .val = {
                                          0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff } });

    try_(TEST_expect(simd_V_bool_bitMask(lanes) == as$(usize)(0x800a)));
} $unscoped(TEST_fn);

TEST_fn_("prl/simd: integer arithmetic and ordering" $scope) {
    T_use_simd_V$(4, i32);

    let lhs = l$((simd_V$(4, i32)){ .val = { 1, -2, 3, -4 } });
    let rhs = l$((simd_V$(4, i32)){ .val = { 10, 20, 30, 40 } });
    let sum = simd_V_int_add(lhs, rhs);
    let diff = simd_V_int_sub(rhs, lhs);
    let abs = simd_V_iint_abs(lhs);
    let sgn = simd_V_sgn(lhs);
    let lane_min = simd_V_map2(lhs, rhs, pri_min);
    let lane_sum = simd_V_reduce(lhs, 0, pri_add);
    let selected = simd_V_select(l$((simd_V$(4, i32)){ .val = { 0, 1, 0, 1 } }), rhs, lhs);
    let shuffled = simd_V_shuffle(lhs, rhs, 0, 5, 2, 7);
    let dot = simd_V_dot(lhs, rhs);
    let eq_mask = simd_V_int_eq(lhs, lhs);
    let lt_mask = simd_V_int_lt(lhs, rhs);
    let ne_mask = simd_V_int_ne(lhs, rhs);
    let not_mask = simd_V_not(l$((simd_V$(4, i32)){ .val = { 0, 1, 2, 0 } }));
    let and_mask = simd_V_and(l$((simd_V$(4, i32)){ .val = { 0, 1, 1, 0 } }), l$((simd_V$(4, i32)){ .val = { 1, 1, 0, 0 } }));
    let or_mask = simd_V_or(l$((simd_V$(4, i32)){ .val = { 0, 1, 0, 0 } }), l$((simd_V$(4, i32)){ .val = { 0, 0, 1, 0 } }));

    try_(TEST_expect(*simd_V_at((sum)[0]) == 11));
    try_(TEST_expect(*simd_V_at((sum)[3]) == 36));
    try_(TEST_expect(*simd_V_at((diff)[1]) == 22));
    try_(TEST_expect(*simd_V_at((diff)[2]) == 27));
    try_(TEST_expect(*simd_V_at((abs)[0]) == 1));
    try_(TEST_expect(*simd_V_at((abs)[1]) == 2));
    try_(TEST_expect(*simd_V_at((abs)[3]) == 4));
    try_(TEST_expect(*simd_V_at((sgn)[0]) == 1));
    try_(TEST_expect(*simd_V_at((sgn)[1]) == -1));
    try_(TEST_expect(*simd_V_at((lane_min)[0]) == 1));
    try_(TEST_expect(*simd_V_at((lane_min)[1]) == -2));
    try_(TEST_expect(lane_sum == -2));
    try_(TEST_expect(*simd_V_at((selected)[0]) == 1));
    try_(TEST_expect(*simd_V_at((selected)[1]) == 20));
    try_(TEST_expect(*simd_V_at((shuffled)[0]) == 1));
    try_(TEST_expect(*simd_V_at((shuffled)[1]) == 20));
    try_(TEST_expect(*simd_V_at((shuffled)[3]) == 40));
    try_(TEST_expect(dot == -100));
    try_(TEST_expect(*simd_V_at((eq_mask)[0]) != 0));
    try_(TEST_expect(*simd_V_at((eq_mask)[3]) != 0));
    try_(TEST_expect(simd_V_bool_all(eq_mask)));
    try_(TEST_expect(simd_V_bool_all(lt_mask)));
    try_(TEST_expect(simd_V_bool_all(ne_mask)));
    try_(TEST_expect(*simd_V_at((not_mask)[0]) != 0));
    try_(TEST_expect(*simd_V_at((not_mask)[1]) == 0));
    try_(TEST_expect(*simd_V_at((not_mask)[3]) != 0));
    try_(TEST_expect(*simd_V_at((and_mask)[0]) == 0));
    try_(TEST_expect(*simd_V_at((and_mask)[1]) != 0));
    try_(TEST_expect(*simd_V_at((and_mask)[2]) == 0));
    try_(TEST_expect(*simd_V_at((or_mask)[0]) == 0));
    try_(TEST_expect(*simd_V_at((or_mask)[1]) != 0));
    try_(TEST_expect(*simd_V_at((or_mask)[2]) != 0));
    try_(TEST_expect(simd_V_eql(lhs, lhs)));
    try_(TEST_expect(simd_V_ord(rhs, lhs) == cmp_Ord_gt));
    try_(TEST_expect(simd_V_isNonzero(lhs)));
    try_(TEST_expect(!simd_V_isZero(rhs)));
    try_(TEST_expect(simd_V_isZero(simd_V_zero$((simd_V$(4, i32))))));
} $unscoped(TEST_fn);

TEST_fn_("prl/simd: fixed-shape construction primitives" $scope) {
    T_use_simd_V$(2, u32);
    T_use_simd_V$(4, u32);
    T_use_simd_V$(8, u32);

    let base = l$((simd_V$(4, u32)){ .val = { 10, 20, 30, 40 } });
    let other = l$((simd_V$(4, u32)){ .val = { 50, 60, 70, 80 } });
    let repeated = simd_V_repeat$((simd_V$(8, u32))(base));
    let catted = simd_V_cat(base, other);
    let extracted = simd_V_extract$((simd_V$(2, u32))(base, 1));
    let arr_base = A_from$((u32){ 10, 20, 30, 40 });
    let arr_repeated = A_repeat$((A$(8, u32))(arr_base));
    let arr_extracted = A_extract$((A$(2, u32))(arr_base, 1));

    try_(TEST_expect(*simd_V_at((repeated)[4]) == 10));
    try_(TEST_expect(*simd_V_at((catted)[6]) == 70));
    try_(TEST_expect(*simd_V_at((extracted)[0]) == 20));
    try_(TEST_expect(*A_at((arr_repeated)[4]) == 10));
    try_(TEST_expect(*A_at((arr_extracted)[0]) == 20));
} $unscoped(TEST_fn);

TEST_fn_("prl/simd: vector arithmetic primitives" $scope) {
    T_use_simd_V$(4, i32);

    let lhs = l$((simd_V$(4, i32)){ .val = { 1, 2, 3, 0 } });
    let rhs = l$((simd_V$(4, i32)){ .val = { 4, 5, 6, 0 } });
    let cross = simd_V_cross3(lhs, rhs);

    try_(TEST_expect(simd_V_dot(lhs, rhs) == 32));
    try_(TEST_expect(*simd_V_at((cross)[0]) == -3));
    try_(TEST_expect(*simd_V_at((cross)[1]) == 6));
    try_(TEST_expect(*simd_V_at((cross)[2]) == -3));
} $unscoped(TEST_fn);

#if UNUSED_CODE
TEST_fn_("prl/simd: pending legacy operations" $scope) {
    let_ignore = simd_V_hAdd;
    let_ignore = simd_V_hSub;
    let_ignore = simd_V_rcpEstimate;
    let_ignore = simd_V_rsqrtEstimate;
    let_ignore = simd_V_packSat;
    let_ignore = simd_V_unpackLo;
    let_ignore = simd_V_unpackHi;
    let_ignore = simd_V_prefixScan;
} $unscoped(TEST_fn);
#endif /* UNUSED_CODE */

TEST_fn_("prl/simd: unsigned bit operations" $scope) {
    T_use_simd_V$(4, u32);

    let bits = l$((simd_V$(4, u32)){ .val = { 0x01u, 0x02u, 0x04u, 0x08u } });
    let shifted = simd_V_int_shl(bits, 1);
    let set = simd_V_int_setBit(bits, 4);
    let reset = simd_V_int_resetBit(set, 4);
    let toggled = simd_V_int_toggleBit(bits, 0);
    let ones = simd_V_int_countOnes(bits);
    let rotated = simd_V_int_rotateLeft(bits, 1);
    let reversed = simd_V_int_reverseBits(l$((simd_V$(4, u32)){ .val = { 0x80000000u, 0x40000000u, 0x00000001u, 0x00000002u } }));
    let bool_int = simd_V_boolToInt$((simd_V$(4, u32))(l$((simd_V$(4, u32)){ .val = { 0u, 2u, 0u, 8u } })));

    try_(TEST_expect(*simd_V_at((shifted)[0]) == 0x02u));
    try_(TEST_expect(*simd_V_at((shifted)[3]) == 0x10u));
    try_(TEST_expect(*simd_V_at((set)[0]) == 0x11u));
    try_(TEST_expect(*simd_V_at((set)[3]) == 0x18u));
    try_(TEST_expect(*simd_V_at((reset)[0]) == *simd_V_at((bits)[0])));
    try_(TEST_expect(*simd_V_at((reset)[3]) == *simd_V_at((bits)[3])));
    try_(TEST_expect(*simd_V_at((toggled)[0]) == 0x00u));
    try_(TEST_expect(*simd_V_at((toggled)[1]) == 0x03u));
    try_(TEST_expect(*simd_V_at((ones)[0]) == 1u));
    try_(TEST_expect(*simd_V_at((ones)[3]) == 1u));
    try_(TEST_expect(*simd_V_at((rotated)[0]) == 0x02u));
    try_(TEST_expect(*simd_V_at((reversed)[0]) == 0x00000001u));
    try_(TEST_expect(*simd_V_at((reversed)[2]) == 0x80000000u));
    try_(TEST_expect(*simd_V_at((bool_int)[0]) == 0u));
    try_(TEST_expect(*simd_V_at((bool_int)[1]) == 1u));
    try_(TEST_expect(*simd_V_at((bool_int)[3]) == 1u));
} $unscoped(TEST_fn);

TEST_fn_("prl/simd: updated integer hierarchy and checked operations" $scope) {
    T_use_simd_V$(4, u32);
    T_use_simd_V$(4, i32);

    let maxes = l$((simd_V$(4, u32)){ .val = { u32_limit_max, u32_limit_max, u32_limit_max, u32_limit_max } });
    let ones = l$((simd_V$(4, u32)){ .val = { 1u, 1u, 1u, 1u } });
    let zeros = simd_V_zero$((simd_V$(4, u32)));
    var add_out = simd_V_zero$((simd_V$(4, u32)));
    var carry_out = simd_V_zero$((simd_V$(4, u32)));
    var borrow_out = simd_V_zero$((simd_V$(4, u32)));
    let overflow = simd_V_int_addOverflow(maxes, ones, &add_out);
    let wrapped = simd_V_int_addWrap(maxes, ones);
    let saturated = simd_V_int_addSat(maxes, ones);
    let carried = simd_V_uint_addCarry(maxes, ones, zeros, &carry_out);
    let borrowed = simd_V_uint_subBorrow(zeros, ones, zeros, &borrow_out);
    let powers = simd_V_uint_exp2$((simd_V$(4, u32))(l$((simd_V$(4, u32)){ .val = { 0u, 1u, 2u, 3u } })));
    let logs = simd_V_uint_log2(l$((simd_V$(4, u32)){ .val = { 1u, 2u, 4u, 8u } }));
    let bit_values = l$((simd_V$(4, u32)){ .val = { 1u, 2u, 8u, 16u } });
    let first_bits = simd_V_int_firstSetBit(bit_values);
    let widths = simd_V_int_bitWidth(bit_values);
    let has_bit = simd_V_int_hasBit(bit_values, 3);
    let extracted = simd_V_int_extractBits(l$((simd_V$(4, u32)){ .val = { 0xf0u, 0xa0u, 0x50u, 0x10u } }), 4, 4);
    let masks_lo = simd_V_int_maskLo$((simd_V$(4, u32))(l$((simd_V$(4, u32)){ .val = { 1u, 2u, 3u, 4u } })));
    let masks = simd_V_int_mask$((simd_V$(4, u32))(
        l$((simd_V$(4, u32)){ .val = { 0u, 1u, 2u, 3u } }),
        l$((simd_V$(4, u32)){ .val = { 1u, 1u, 2u, 2u } })
    ));
    let signed_values = l$((simd_V$(4, i32)){ .val = { -1, -2, 0, 1 } });
    let sgn_bits = simd_V_iint_sgnBit(signed_values);
    let redundant = simd_V_iint_leadingRedundantSgnBits(signed_values);

    try_(TEST_expect(simd_V_bool_all(overflow)));
    try_(TEST_expect(*simd_V_at((add_out)[0]) == 0u));
    try_(TEST_expect(*simd_V_at((wrapped)[1]) == 0u));
    try_(TEST_expect(*simd_V_at((saturated)[2]) == u32_limit_max));
    try_(TEST_expect(*simd_V_at((carried)[0]) == 0u));
    try_(TEST_expect(*simd_V_at((carry_out)[0]) == 1u));
    try_(TEST_expect(*simd_V_at((borrowed)[0]) == u32_limit_max));
    try_(TEST_expect(*simd_V_at((borrow_out)[0]) == 1u));
    try_(TEST_expect(*simd_V_at((powers)[3]) == 8u));
    try_(TEST_expect(*simd_V_at((logs)[2]) == 2u));
    try_(TEST_expect(*simd_V_at((first_bits)[2]) == 4u));
    try_(TEST_expect(*simd_V_at((widths)[3]) == 5u));
    try_(TEST_expect(*simd_V_at((has_bit)[0]) == 0u));
    try_(TEST_expect(*simd_V_at((has_bit)[2]) == 1u));
    try_(TEST_expect(*simd_V_at((extracted)[0]) == 0xfu));
    try_(TEST_expect(*simd_V_at((extracted)[3]) == 0x1u));
    try_(TEST_expect(*simd_V_at((masks_lo)[3]) == 0xfu));
    try_(TEST_expect(*simd_V_at((masks)[2]) == 0xcu));
    try_(TEST_expect(*simd_V_at((sgn_bits)[0]) == 1));
    try_(TEST_expect(*simd_V_at((sgn_bits)[3]) == 0));
    try_(TEST_expect(*simd_V_at((redundant)[0]) == 31u));
} $unscoped(TEST_fn);

TEST_fn_("prl/simd: floating point lane maps" $scope) {
    T_use_simd_V$(4, f32);
    T_use_simd_V$(4, i32);

    let vals = l$((simd_V$(4, f32)){ .val = { 4.0f, 9.0f, 16.0f, 25.0f } });
    let roots = simd_V_flt_sqrt(vals);
    let halves = simd_V_flt_mul(vals, simd_V_splat(vals, 0.5f));
    let logs = simd_V_flt_ln(l$((simd_V$(4, f32)){ .val = { 1.0f, 1.0f, 1.0f, 1.0f } }));
    let sins = simd_V_flt_sin(l$((simd_V$(4, f32)){ .val = { 0.0f, 0.0f, 0.0f, 0.0f } }));
    let finite = simd_V_flt_isFinite(vals);
    let integral = simd_V_flt_isIntegral(l$((simd_V$(4, f32)){ .val = { 1.0f, 1.5f, -2.0f, -2.5f } }));
    let clamped = simd_V_flt_clamp01(l$((simd_V$(4, f32)){ .val = { -1.0f, 0.25f, 1.0f, 2.0f } }));
    let wrapped = simd_V_flt_wrap01(l$((simd_V$(4, f32)){ .val = { -0.25f, 0.25f, 1.25f, 2.5f } }));
    let rounded = simd_V_flt_lround(l$((simd_V$(4, f32)){ .val = { 1.2f, 1.8f, -1.2f, -1.8f } }));
    let exp10s = simd_V_flt_exp10(l$((simd_V$(4, f32)){ .val = { 0.0f, 1.0f, 2.0f, 3.0f } }));
    let next = simd_V_flt_nextUp(l$((simd_V$(4, f32)){ .val = { 1.0f, 2.0f, 3.0f, 4.0f } }));
    let apx_eq = flt_ordApxAbs(1.0f, 1.05f, 0.1f);
    let apx_lt = flt_ordApxAbs(1.0f, 1.2f, 0.1f);
    let apx_abs_lt = flt_ordApx(1000.0f, 1001.0f, 0.001f, cmp_ApxMode_abs);
    let apx_rel_eq = flt_ordApxRel(1000.0f, 1001.0f, 0.001f);
    let apx_rel_eq_swapped = flt_ordApxRel(1001.0f, 1000.0f, 0.001f);
    let apx_boundary_eq = flt_ordApxAbs(1.0f, 1.125f, 0.125f);
    let apx_near_zero_abs_eq = flt_ordApxAbs(0.0f, 0.001f, 0.01f);
    let apx_near_zero_rel_lt = flt_ordApxRel(0.0f, 0.001f, 0.01f);
    let apx_inf_gt = flt_ordApxRel(f32_inf, f32_inf_ngtv, 1.0f);
    let apx_lhs = l$((simd_V$(4, f32)){ .val = { 1000.0f, 2.0f, 3.0f, 4.0f } });
    let apx_rhs = l$((simd_V$(4, f32)){ .val = { 1001.0f, 2.0f, 3.0f, 4.0f } });
    let apx_threshold = l$((simd_V$(4, f32)){ .val = { 0.001f, 0.0f, 0.0f, 0.0f } });
    let simd_apx_abs_lt = simd_V_flt_ordApxAbs(apx_lhs, apx_rhs, apx_threshold);
    let simd_apx_rel_eq = simd_V_flt_ordApxRel(apx_lhs, apx_rhs, apx_threshold);
    var exponents = simd_V_zero$((simd_V$(4, i32)));
    var int_parts = simd_V_zero$((simd_V$(4, f32)));
    var quotients = simd_V_zero$((simd_V$(4, i32)));
    let fractions = simd_V_flt_frexp(l$((simd_V$(4, f32)){ .val = { 1.0f, 2.0f, 4.0f, 8.0f } }), &exponents);
    let modf_fracts = simd_V_flt_modf(l$((simd_V$(4, f32)){ .val = { 1.25f, 2.5f, 3.75f, 4.0f } }), &int_parts);
    let remainders = simd_V_flt_remquo(
        l$((simd_V$(4, f32)){ .val = { 5.0f, 6.0f, 7.0f, 8.0f } }),
        l$((simd_V$(4, f32)){ .val = { 2.0f, 2.0f, 2.0f, 2.0f } }),
        &quotients
    );

    try_(TEST_expect(*simd_V_at((roots)[0]) == 2.0f));
    try_(TEST_expect(*simd_V_at((roots)[1]) == 3.0f));
    try_(TEST_expect(*simd_V_at((roots)[2]) == 4.0f));
    try_(TEST_expect(*simd_V_at((roots)[3]) == 5.0f));
    try_(TEST_expect(*simd_V_at((halves)[0]) == 2.0f));
    try_(TEST_expect(*simd_V_at((halves)[3]) == 12.5f));
    try_(TEST_expect(*simd_V_at((logs)[0]) == 0.0f));
    try_(TEST_expect(*simd_V_at((sins)[2]) == 0.0f));
    try_(TEST_expect(*simd_V_at((finite)[0]) == 1.0f));
    try_(TEST_expect(*simd_V_at((finite)[3]) == 1.0f));
    try_(TEST_expect(*simd_V_at((integral)[0]) == 1.0f));
    try_(TEST_expect(*simd_V_at((integral)[1]) == 0.0f));
    try_(TEST_expect(*simd_V_at((clamped)[0]) == 0.0f));
    try_(TEST_expect(*simd_V_at((clamped)[3]) == 1.0f));
    try_(TEST_expect(*simd_V_at((wrapped)[0]) == 0.75f));
    try_(TEST_expect(*simd_V_at((wrapped)[2]) == 0.25f));
    try_(TEST_expect(*simd_V_at((rounded)[1]) == 2));
    try_(TEST_expect(*simd_V_at((rounded)[3]) == -2));
    try_(TEST_expect(*simd_V_at((exp10s)[2]) == 100.0f));
    try_(TEST_expect(*simd_V_at((next)[0]) > 1.0f));
    try_(TEST_expect(apx_eq == cmp_Ord_eq));
    try_(TEST_expect(apx_lt == cmp_Ord_lt));
    try_(TEST_expect(apx_abs_lt == cmp_Ord_lt));
    try_(TEST_expect(apx_rel_eq == cmp_Ord_eq));
    try_(TEST_expect(apx_rel_eq_swapped == cmp_Ord_eq));
    try_(TEST_expect(apx_boundary_eq == cmp_Ord_eq));
    try_(TEST_expect(apx_near_zero_abs_eq == cmp_Ord_eq));
    try_(TEST_expect(apx_near_zero_rel_lt == cmp_Ord_lt));
    try_(TEST_expect(apx_inf_gt == cmp_Ord_gt));
    try_(TEST_expect(simd_apx_abs_lt == cmp_Ord_lt));
    try_(TEST_expect(simd_apx_rel_eq == cmp_Ord_eq));
    try_(TEST_expect(*simd_V_at((fractions)[0]) == 0.5f));
    try_(TEST_expect(*simd_V_at((exponents)[3]) == 4));
    try_(TEST_expect(*simd_V_at((modf_fracts)[1]) == 0.5f));
    try_(TEST_expect(*simd_V_at((int_parts)[2]) == 3.0f));
    try_(TEST_expect(*simd_V_at((remainders)[0]) == 1.0f));
    try_(TEST_expect(*simd_V_at((quotients)[0]) != 0));
} $unscoped(TEST_fn);

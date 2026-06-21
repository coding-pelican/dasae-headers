#include "dh-main.h"
#include "dh/cmp.h"

$static cmp_fn_ord$((i32)(lhs, rhs)) { return pri_ord(lhs, rhs); }
$static cmp_fn_eq_default$((i32)(lhs, rhs));
$static cmp_fn_ne_default$((i32)(lhs, rhs));
$static cmp_fn_lt_default$((i32)(lhs, rhs));
$static cmp_fn_gt_default$((i32)(lhs, rhs));
$static cmp_fn_le_default$((i32)(lhs, rhs));
$static cmp_fn_ge_default$((i32)(lhs, rhs));

$static cmp_fn_u_ord_default$((i32)(lhs, rhs));
$static cmp_fn_u_eq_default$((i32)(lhs, rhs));
$static cmp_fn_u_ne_default$((i32)(lhs, rhs));
$static cmp_fn_u_lt_default$((i32)(lhs, rhs));
$static cmp_fn_u_gt_default$((i32)(lhs, rhs));
$static cmp_fn_u_le_default$((i32)(lhs, rhs));
$static cmp_fn_u_ge_default$((i32)(lhs, rhs));

typedef struct cmp_TestApx {
    f32 value;
} cmp_TestApx;

$static cmp_fn_ordApxAbs$((cmp_TestApx)(lhs, rhs, threshold)) {
    return flt_ordApxAbs(lhs.value, rhs.value, threshold.value);
}
$static cmp_fn_eqApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_neApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_ltApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_gtApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_leApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_geApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));

$static cmp_fn_ordApxRel$((cmp_TestApx)(lhs, rhs, threshold)) {
    return flt_ordApxRel(lhs.value, rhs.value, threshold.value);
}
$static cmp_fn_eqApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_neApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_ltApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_gtApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_leApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_geApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));

$static cmp_fn_ordApx_default$((cmp_TestApx)(lhs, rhs, threshold, mode));
$static cmp_fn_eqApx_default$((cmp_TestApx)(lhs, rhs, threshold, mode));
$static cmp_fn_neApx_default$((cmp_TestApx)(lhs, rhs, threshold, mode));
$static cmp_fn_ltApx_default$((cmp_TestApx)(lhs, rhs, threshold, mode));
$static cmp_fn_gtApx_default$((cmp_TestApx)(lhs, rhs, threshold, mode));
$static cmp_fn_leApx_default$((cmp_TestApx)(lhs, rhs, threshold, mode));
$static cmp_fn_geApx_default$((cmp_TestApx)(lhs, rhs, threshold, mode));

$static cmp_fn_u_ordApx_default$((cmp_TestApx)(lhs, rhs, threshold, mode));
$static cmp_fn_u_eqApx_default$((cmp_TestApx)(lhs, rhs, threshold, mode));
$static cmp_fn_u_ordApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_eqApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_neApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_ltApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_gtApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_leApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_geApxAbs_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_ordApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_eqApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_neApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_ltApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_gtApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_leApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));
$static cmp_fn_u_geApxRel_default$((cmp_TestApx)(lhs, rhs, threshold));

TEST_fn_("cmp: typed ordering defaults from ord" $scope) {
    try_(TEST_expect(cmp_ord$(i32)(1, 2) == cmp_Ord_lt));
    try_(TEST_expect(cmp_ord$(i32)(2, 1) == cmp_Ord_gt));
    try_(TEST_expect(cmp_ord$(i32)(2, 2) == cmp_Ord_eq));

    try_(TEST_expect(cmp_eq$(i32)(2, 2)));
    try_(TEST_expect(cmp_ne$(i32)(1, 2)));
    try_(TEST_expect(cmp_lt$(i32)(1, 2)));
    try_(TEST_expect(cmp_gt$(i32)(2, 1)));
    try_(TEST_expect(cmp_le$(i32)(2, 2)));
    try_(TEST_expect(cmp_ge$(i32)(2, 2)));
} $unscoped(TEST_fn);

TEST_fn_("cmp: untyped wrappers use typed defaults" $scope) {
    let lhs = u_anyV(as$(i32)(1));
    let rhs = u_anyV(as$(i32)(2));
    let same = u_anyV(as$(i32)(1));

    try_(TEST_expect(cmp_u_ord$(i32)(lhs, rhs) == cmp_Ord_lt));
    try_(TEST_expect(cmp_u_eq$(i32)(lhs, same)));
    try_(TEST_expect(cmp_u_ne$(i32)(lhs, rhs)));
    try_(TEST_expect(cmp_u_lt$(i32)(lhs, rhs)));
    try_(TEST_expect(cmp_u_gt$(i32)(rhs, lhs)));
    try_(TEST_expect(cmp_u_le$(i32)(lhs, same)));
    try_(TEST_expect(cmp_u_ge$(i32)(same, lhs)));
} $unscoped(TEST_fn);

TEST_fn_("cmp: runtime scalar comparator tables" $scope) {
    let lhs = u_anyV(as$(i32)(1));
    let rhs = u_anyV(as$(i32)(2));
    let same = u_anyV(as$(i32)(1));

    let ord_asc = cmp_OrdFn_defaultAsc(cmp_m_T_i32);
    let ord_desc = cmp_OrdFn_defaultDesc(cmp_m_T_i32);
    let eql = cmp_EqlFn_default(cmp_m_T_i32);

    try_(TEST_expect(cmp_ord(lhs, rhs, ord_asc) == cmp_Ord_lt));
    try_(TEST_expect(cmp_ord(lhs, rhs, ord_desc) == cmp_Ord_gt));
    try_(TEST_expect(cmp_eq(lhs, same, ord_asc)));
    try_(TEST_expect(cmp_ne(lhs, rhs, ord_asc)));
    try_(TEST_expect(cmp_eql(lhs, same, eql)));
    try_(TEST_expect(cmp_neq(lhs, rhs, eql)));
} $unscoped(TEST_fn);

TEST_fn_("cmp: approximate defaults propagate absolute and relative modes" $scope) {
    let lhs = l$((cmp_TestApx){ .value = 1000.0f });
    let rhs = l$((cmp_TestApx){ .value = 1001.0f });
    let threshold = l$((cmp_TestApx){ .value = 0.001f });

    try_(TEST_expect(cmp_ordApx$(cmp_TestApx)(lhs, rhs, threshold, cmp_ApxMode_abs) == cmp_Ord_lt));
    try_(TEST_expect(cmp_eqApx$(cmp_TestApx)(lhs, rhs, threshold, cmp_ApxMode_rel)));
    try_(TEST_expect(cmp_neApx$(cmp_TestApx)(lhs, rhs, threshold, cmp_ApxMode_abs)));
    try_(TEST_expect(cmp_ltApx$(cmp_TestApx)(lhs, rhs, threshold, cmp_ApxMode_abs)));
    try_(TEST_expect(cmp_gtApx$(cmp_TestApx)(rhs, lhs, threshold, cmp_ApxMode_abs)));
    try_(TEST_expect(cmp_leApx$(cmp_TestApx)(lhs, rhs, threshold, cmp_ApxMode_rel)));
    try_(TEST_expect(cmp_geApx$(cmp_TestApx)(lhs, rhs, threshold, cmp_ApxMode_rel)));

    try_(TEST_expect(cmp_ordApxAbs$(cmp_TestApx)(lhs, rhs, threshold) == cmp_Ord_lt));
    try_(TEST_expect(cmp_eqApxAbs$(cmp_TestApx)(lhs, lhs, threshold)));
    try_(TEST_expect(cmp_neApxAbs$(cmp_TestApx)(lhs, rhs, threshold)));
    try_(TEST_expect(cmp_ltApxAbs$(cmp_TestApx)(lhs, rhs, threshold)));
    try_(TEST_expect(cmp_gtApxAbs$(cmp_TestApx)(rhs, lhs, threshold)));
    try_(TEST_expect(cmp_leApxAbs$(cmp_TestApx)(lhs, rhs, threshold)));
    try_(TEST_expect(cmp_geApxAbs$(cmp_TestApx)(rhs, lhs, threshold)));

    try_(TEST_expect(cmp_ordApxRel$(cmp_TestApx)(lhs, rhs, threshold) == cmp_Ord_eq));
    try_(TEST_expect(cmp_eqApxRel$(cmp_TestApx)(lhs, rhs, threshold)));
    try_(TEST_expect(!cmp_neApxRel$(cmp_TestApx)(lhs, rhs, threshold)));
    try_(TEST_expect(!cmp_ltApxRel$(cmp_TestApx)(lhs, rhs, threshold)));
    try_(TEST_expect(!cmp_gtApxRel$(cmp_TestApx)(lhs, rhs, threshold)));
    try_(TEST_expect(cmp_leApxRel$(cmp_TestApx)(lhs, rhs, threshold)));
    try_(TEST_expect(cmp_geApxRel$(cmp_TestApx)(lhs, rhs, threshold)));

    try_(TEST_expect(cmp_u_ordApx$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold), cmp_ApxMode_abs) == cmp_Ord_lt));
    try_(TEST_expect(cmp_u_eqApx$(cmp_TestApx)(
        u_anyV(lhs), u_anyV(rhs), u_anyV(threshold), cmp_ApxMode_rel
    )));
    try_(TEST_expect(cmp_u_ordApxAbs$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold)) == cmp_Ord_lt));
    try_(TEST_expect(cmp_u_eqApxAbs$(cmp_TestApx)(u_anyV(lhs), u_anyV(lhs), u_anyV(threshold))));
    try_(TEST_expect(cmp_u_neApxAbs$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold))));
    try_(TEST_expect(cmp_u_ltApxAbs$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold))));
    try_(TEST_expect(cmp_u_gtApxAbs$(cmp_TestApx)(u_anyV(rhs), u_anyV(lhs), u_anyV(threshold))));
    try_(TEST_expect(cmp_u_leApxAbs$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold))));
    try_(TEST_expect(cmp_u_geApxAbs$(cmp_TestApx)(u_anyV(rhs), u_anyV(lhs), u_anyV(threshold))));
    try_(TEST_expect(cmp_u_ordApxRel$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold)) == cmp_Ord_eq));
    try_(TEST_expect(cmp_u_eqApxRel$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold))));
    try_(TEST_expect(!cmp_u_neApxRel$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold))));
    try_(TEST_expect(!cmp_u_ltApxRel$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold))));
    try_(TEST_expect(!cmp_u_gtApxRel$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold))));
    try_(TEST_expect(cmp_u_leApxRel$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold))));
    try_(TEST_expect(cmp_u_geApxRel$(cmp_TestApx)(u_anyV(lhs), u_anyV(rhs), u_anyV(threshold))));
    return_ok({});
} $unscoped(TEST_fn);

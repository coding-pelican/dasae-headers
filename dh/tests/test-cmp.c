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

#include "dh/Ver.h"

/*========== External Definitions ===========================================*/

fn_((Ver_self(void))(Ver)) { return Ver_fromNum(ver_self_num); };
fn_((Ver_comp(void))(Ver)) { return Ver_fromNum(comp_ver); };
fn_((Ver_clang(void))(O$Ver) $scope) {
    return_(pp_if_(comp_is_clang)(
        pp_then_(some(Ver_fromNum(comp_clang_ver))),
        pp_else_(none())));
} $unscoped(fn);
fn_((Ver_gcc(void))(O$Ver) $scope) {
    return_(pp_if_(comp_is_gcc)(
        pp_then_(some(Ver_fromNum(comp_gcc_ver))),
        pp_else_(none())));
} $unscoped(fn);
fn_((Ver_gnu(void))(O$Ver) $scope) {
    return_(pp_if_(lang_has_extn_gnu)(
        pp_then_(some(Ver_fromNum(comp_gnu_ver))),
        pp_else_(none())));
} $unscoped(fn);

cmp_fn_eql$((Ver)(lhs, rhs)) { return pri_eql(Ver_intoNum(lhs), Ver_intoNum(rhs)); };
cmp_fn_neq_default$((Ver)(lhs, rhs));

cmp_fn_ord$((Ver)(lhs, rhs)) { return pri_ord(Ver_intoNum(lhs), Ver_intoNum(rhs)); };
cmp_fn_eq_default$((Ver)(lhs, rhs));
cmp_fn_ne_default$((Ver)(lhs, rhs));
cmp_fn_lt_default$((Ver)(lhs, rhs));
cmp_fn_gt_default$((Ver)(lhs, rhs));
cmp_fn_le_default$((Ver)(lhs, rhs));
cmp_fn_ge_default$((Ver)(lhs, rhs));

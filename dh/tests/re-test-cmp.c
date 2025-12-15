#include "dh/core/cmp.h"
#include "dh/main.h"
#include "dh/io/stream.h"

cmp_fn_ord$((i32)(lhs, rhs)) { return prim_ord(lhs, rhs); }
cmp_fn_eq_default$((i32)(lhs, rhs));
cmp_fn_ne_default$((i32)(lhs, rhs));
cmp_fn_lt_default$((i32)(lhs, rhs));
cmp_fn_gt_default$((i32)(lhs, rhs));
cmp_fn_le_default$((i32)(lhs, rhs));
cmp_fn_ge_default$((i32)(lhs, rhs));
cmp_fn_u_ord_default$((i32)(lhs, rhs));
cmp_fn_u_eq_default$((i32)(lhs, rhs));
cmp_fn_u_ne_default$((i32)(lhs, rhs));
cmp_fn_u_lt_default$((i32)(lhs, rhs));
cmp_fn_u_gt_default$((i32)(lhs, rhs));
cmp_fn_u_le_default$((i32)(lhs, rhs));
cmp_fn_u_ge_default$((i32)(lhs, rhs));

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    let lhs = 1;
    let rhs = 2;
    switch (cmp_ord$(i32)(lhs, rhs)) {
    case_((cmp_Ord_lt)) io_stream_println(u8_l("lhs < rhs")); break $end(case);
    case_((cmp_Ord_eq)) io_stream_println(u8_l("lhs == rhs")); break $end(case);
    case_((cmp_Ord_gt)) io_stream_println(u8_l("lhs > rhs")); break $end(case);
    }

    return_ok({});
} $unscoped_(fn);

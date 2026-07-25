#include "dh/SemVer.h"
#include "dh/ascii.h"
#include "dh/mem/common.h"

/*========== Internal Declarations ==========================================*/

$static fn_((SemVer_Prerelease__isNumericId(S_const$u8 id))(bool));
$static fn_((SemVer_Prerelease__ordNumericId(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord));
$static fn_((SemVer_Prerelease__ordId(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord));

/*========== External Definitions ===========================================*/

cmp_fn_eql$((SemVer_Core)(lhs, rhs)) { return cmp_eq$(SemVer_Core)(lhs, rhs); };
cmp_fn_neq_default$((SemVer_Core)(lhs, rhs));

cmp_fn_ord$((SemVer_Core)(lhs, rhs)) {
    let l_vals = A_from$((u64){ lhs.major, lhs.minor, lhs.patch });
    let r_vals = A_from$((u64){ rhs.major, rhs.minor, rhs.patch });
    loop_inline_(for_)(($a(l_vals), $a(r_vals))(l, r)) {
        if_(let ord = pri_ord(l, r), cmp_Ord_isNe(ord)) return ord;
    } $end(for);
    return cmp_Ord_eq;
};
cmp_fn_eq_default$((SemVer_Core)(lhs, rhs));
cmp_fn_ne_default$((SemVer_Core)(lhs, rhs));
cmp_fn_lt_default$((SemVer_Core)(lhs, rhs));
cmp_fn_gt_default$((SemVer_Core)(lhs, rhs));
cmp_fn_le_default$((SemVer_Core)(lhs, rhs));
cmp_fn_ge_default$((SemVer_Core)(lhs, rhs));

cmp_fn_eql$((SemVer_Prerelease)(lhs, rhs)) { return cmp_eq$(SemVer_Prerelease)(lhs, rhs); };
cmp_fn_neq_default$((SemVer_Prerelease)(lhs, rhs));

cmp_fn_ord$((SemVer_Prerelease)(lhs, rhs)) {
    if (lhs.label.len == 0 || rhs.label.len == 0) {
        if (lhs.label.len == rhs.label.len) return cmp_Ord_eq;
        if (lhs.label.len == 0) return cmp_Ord_gt;
        return cmp_Ord_lt;
    }
    var lhs_ids = mem_splitUnitBytes(lhs.label, u8_c('.'));
    var lhs_id = mem_SplitIter_firstBytes(&lhs_ids);
    var rhs_ids = mem_splitUnitBytes(rhs.label, u8_c('.'));
    var rhs_id = mem_SplitIter_firstBytes(&rhs_ids);
    while (true) {
        if_(let ord = SemVer_Prerelease__ordId(lhs_id, rhs_id), cmp_Ord_isNe(ord)) return ord;
        let lhs_next = mem_SplitIter_nextBytes(&lhs_ids);
        let rhs_next = mem_SplitIter_nextBytes(&rhs_ids);
        if_none(lhs_next) return isNone(rhs_next) ? cmp_Ord_eq : cmp_Ord_lt;
        if_none(rhs_next) return cmp_Ord_gt;
        lhs_id = unwrap_(lhs_next);
        rhs_id = unwrap_(rhs_next);
    }
};
cmp_fn_eq_default$((SemVer_Prerelease)(lhs, rhs));
cmp_fn_ne_default$((SemVer_Prerelease)(lhs, rhs));
cmp_fn_lt_default$((SemVer_Prerelease)(lhs, rhs));
cmp_fn_gt_default$((SemVer_Prerelease)(lhs, rhs));
cmp_fn_le_default$((SemVer_Prerelease)(lhs, rhs));
cmp_fn_ge_default$((SemVer_Prerelease)(lhs, rhs));

cmp_fn_eql$((SemVer)(lhs, rhs)) {
    return cmp_eq$(SemVer)(lhs, rhs)
        && mem_eqlBytes(lhs.build.label, rhs.build.label);
};
cmp_fn_neq_default$((SemVer)(lhs, rhs));

cmp_fn_ord$((SemVer)(lhs, rhs)) {
    if_(let core_ord = cmp_ord$(SemVer_Core)(lhs.core, rhs.core), cmp_Ord_isNe(core_ord)) return core_ord;
    return cmp_ord$(SemVer_Prerelease)(lhs.prerelease, rhs.prerelease);
};
cmp_fn_eq_default$((SemVer)(lhs, rhs));
cmp_fn_ne_default$((SemVer)(lhs, rhs));
cmp_fn_lt_default$((SemVer)(lhs, rhs));
cmp_fn_gt_default$((SemVer)(lhs, rhs));
cmp_fn_le_default$((SemVer)(lhs, rhs));
cmp_fn_ge_default$((SemVer)(lhs, rhs));

/*========== Internal Definitions ===========================================*/

$static fn_((SemVer_Prerelease__isNumericId(S_const$u8 id))(bool)) {
    if (id.len == 0) return false;
    for_(($s(id))(ch)) {
        if (!ascii_isDigit(*ch)) return false;
    } $end(for);
    return true;
};
$static fn_((SemVer_Prerelease__ordNumericId(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord)) {
    while (lhs.len > 1 && *S_at((lhs)[0]) == u8_c('0')) lhs = S_suffix((lhs)(1));
    while (rhs.len > 1 && *S_at((rhs)[0]) == u8_c('0')) rhs = S_suffix((rhs)(1));
    if_(let len_ord = pri_ord(lhs.len, rhs.len), cmp_Ord_isNe(len_ord)) return len_ord;
    return mem_ordBytes(lhs, rhs);
};
$static fn_((SemVer_Prerelease__ordId(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord)) {
    let lhs_is_numeric = SemVer_Prerelease__isNumericId(lhs);
    let rhs_is_numeric = SemVer_Prerelease__isNumericId(rhs);
    if (lhs_is_numeric != rhs_is_numeric) return lhs_is_numeric ? cmp_Ord_lt : cmp_Ord_gt;
    return lhs_is_numeric ? SemVer_Prerelease__ordNumericId(lhs, rhs) : mem_ordBytes(lhs, rhs);
};

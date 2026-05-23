#include "dh-main.h"
#include "dh/sort.h"

$static cmp_fn_ord$((i32)(lhs, rhs)) { return pri_ord(lhs, rhs); }
$static cmp_fn_u_ord_default$((i32)(lhs, rhs));

TEST_fn_("sort: pdq - regression input from 5823 coordinate compression" $scope) {
    var data = A_from$((i32){
        0, 2000000001,
        511025150, 1272686665, 1168723365, 280080820, 794472658,
        1966976506, 1296908414, 1018022223, 1343724115, 1247370366,
        140722156, 1300515102, 28278034, 1951672655, 1798451156,
        1007668780, 556958498, 1182801014, 503221913, 411767315,
        1540063683, 1009883194, 1161732570, 1796287290, 1180323947,
        1022960728, 852840001, 1372388373, 1849030896, 323446306,
        498048707, 1363352895, 325606562, 1864113090, 1988216538,
        1123523096, 837382728, 1591826252, 32527369, 1441845164,
        1669098783, 137506472, 342308754, 1628287049, 1269376693,
        91888745, 646967375, 1675201516, 66597223, 1768604193,
        1858705584, 578600102, 1015220939, 1277214859, 1543662608,
        1976021287, 1887236124, 832383095, 1533581380, 1692450872,
        1978044335, 916835693, 848177449, 1563638617, 1719938488,
        1238904066, 954816441, 288083023, 1887351790, 784947489,
        209281550, 77065967, 291990909, 1062754042, 465975918,
        554010463, 1443197553, 936650469, 1672830881, 1345716944,
        904409541, 1789878482, 1232704445, 1146977644, 875262509,
        499046146, 1464545481, 1839106216, 1300986884, 1500266159,
        1163433497, 1222149063, 1532910375, 1359305406, 573560469,
        267221778, 1035090161, 190091164, 1719265369, 881460063,
        43218872, 917279942, 1874252912, 94897439, 1319698551,
        811329557, 710718622, 1891729738, 1085381991, 77336990,
        15532186, 232184442, 1150145366, 423880744, 626232860,
        565589653, 1481190503, 1863541293, 673945910, 297052915,
        809067079, 1868077821, 829343032, 1462373739, 220273459,
        1742708828, 582613664 $listed
    });
    sort_pdq(u_anyS(A_ref(data)), cmp_u_ord$(i32));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(data)).as_const, cmp_u_ord$(i32))));
} $unscoped(TEST_fn);

TEST_fn_("sort: pdq - partial insertion shift regression with duplicates" $scope) {
    var_(data, A$$(162, i32)) $undefined;

    using_(var_(written, usize) = 0) {
        *A_at((data)[written++]) = 0;
        *A_at((data)[written++]) = 2000000001;

        let counts = A_from$((usize){
            4, 11, 6, 9, 6, 11, 6,
            10, 7, 7, 9, 3, 7, 5,
            3, 9, 4, 9, 5, 9, 10 $listed
        });
        for_(($rt(A_len(counts)))(val)) {
            for_(($rt(*A_at((counts)[val])))($ignore)) {
                *A_at((data)[written++]) = intCast$((i32)(val));
            } $end(for);
        } $end(for);

        let updates = A_from$((i32){
            11, 14, 18, 8, 7, 9, 13, 17, 3, 16 $listed
        });
        for_(($a(updates))(update)) {
            *A_at((data)[written++]) = *update;
        } $end(for);

        try_(TEST_expect(written == A_len(data)));
    }

    sort_pdq(u_anyS(A_ref(data)), cmp_u_ord$(i32));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(data)).as_const, cmp_u_ord$(i32))));
} $unscoped(TEST_fn);

TEST_fn_("sort: pdq - deterministic duplicates and patterned values" $scope) {
    var_(data, A$$(257, i32)) $undefined;

    using_(var state = u32_(0x9E3779B9u)) {
        for_(($s(A_ref(data)), $rf(0))(val, i)) {
            state = state * 1664525u + 1013904223u;
            *val = as$(i32)((state >> 8) % 97) - 48;
            if (i % 17 == 0) *val = 7;
            if (i % 29 == 0) *val = -13;
        } $end(for);
    }

    sort_pdq(u_anyS(A_ref(data)), cmp_u_ord$(i32));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(data)).as_const, cmp_u_ord$(i32))));
} $unscoped(TEST_fn);

T_alias$((test_sort_pdq_IdxCtx)(struct test_sort_pdq_IdxCtx {
    var_(data, i32*);
    var_(range, R);
}));
$static fn_((test_sort_pdq_IdxCtx_ord(usize lhs, usize rhs, u_V$raw raw_ctx))(cmp_Ord)) {
    let ctx = u_castV$((test_sort_pdq_IdxCtx)(raw_ctx));
    claim_assert(ctx.range.begin <= lhs), claim_assert(lhs < ctx.range.end);
    claim_assert(ctx.range.begin <= rhs), claim_assert(rhs < ctx.range.end);
    return pri_ord(*P_at((ctx.data)[lhs]), *P_at((ctx.data)[rhs]));
};
$static fn_((test_sort_pdq_IdxCtx_swap(usize lhs, usize rhs, u_V$raw raw_ctx))(void)) {
    let ctx = u_castV$((test_sort_pdq_IdxCtx)(raw_ctx));
    claim_assert(ctx.range.begin <= lhs), claim_assert(lhs < ctx.range.end);
    claim_assert(ctx.range.begin <= rhs), claim_assert(rhs < ctx.range.end);
    return pri_swap(P_at((ctx.data)[lhs]), P_at((ctx.data)[rhs]));
};
TEST_fn_("sort: pdqIdx - non-zero range stays in bounds and sorts range" $scope) {
    enum {
        data_len = usize_(2000),
        sort_begin = usize_(1118),
        sort_end = usize_(1764),
    };
    $static var_(data, A$$(data_len, i32)) $undefined_static;

    for_(($s(A_ref(data)), $rf(0))(val, i)) {
        *val = intCast$((i32)((i * 1103515245u + 12345u) % 1009u)) - 504;
    } $end(for);
    let before_begin = *A_at((data)[sort_begin - 1]);
    let after_end = *A_at((data)[sort_end]);

    let ctx = (test_sort_pdq_IdxCtx){
        .data = A_ptr(data),
        .range = $r(sort_begin, sort_end),
    };
    let idx_ctx = (sort_IdxCtx){
        .ordFn = test_sort_pdq_IdxCtx_ord,
        .swapFn = test_sort_pdq_IdxCtx_swap,
        .inner = u_deref(u_anyP(&ctx)),
    };
    sort_pdqIdx($r(sort_begin, sort_end), idx_ctx);

    try_(TEST_expect(*A_at((data)[sort_begin - 1]) == before_begin));
    try_(TEST_expect(*A_at((data)[sort_end]) == after_end));
    try_(TEST_expect(sort_inOrdd(
        u_anyS(A_slice((data)$r(sort_begin, sort_end))).as_const,
        cmp_u_ord$(i32)
    )));
} $unscoped(TEST_fn);

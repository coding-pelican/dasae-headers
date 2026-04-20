#include <dh-main.h>
#include <dh/TEST.h>
#include <dh/sort.h>

$static cmp_fn_ord$((i32)(lhs, rhs)) { return pri_ord(lhs, rhs); }
$static cmp_fn_u_ord_default$((i32)(lhs, rhs));

T_alias$((PdqIdxTestCtx)(struct PdqIdxTestCtx {
    var_(data, i32*);
    var_(range, R);
}));

$static fn_((expect_sorted_i32(const i32* data, usize len))(E$void) $scope) {
    for (usize i = 1; i < len; ++i) {
        try_(TEST_expect(data[i - 1] <= data[i]));
    }
    return_ok({});
} $unscoped(fn);

$static fn_((pdq_idx_test_ord(usize lhs, usize rhs, u_V$raw raw_ctx))(cmp_Ord)) {
    let ctx = u_castV$((PdqIdxTestCtx)(raw_ctx));
    claim_assert(ctx.range.begin <= lhs && lhs < ctx.range.end);
    claim_assert(ctx.range.begin <= rhs && rhs < ctx.range.end);
    return pri_ord(ctx.data[lhs], ctx.data[rhs]);
}

$static fn_((pdq_idx_test_swap(usize lhs, usize rhs, u_V$raw raw_ctx))(void)) {
    let ctx = u_castV$((PdqIdxTestCtx)(raw_ctx));
    claim_assert(ctx.range.begin <= lhs && lhs < ctx.range.end);
    claim_assert(ctx.range.begin <= rhs && rhs < ctx.range.end);
    let tmp = ctx.data[lhs];
    ctx.data[lhs] = ctx.data[rhs];
    ctx.data[rhs] = tmp;
}

TEST_fn_("sort_pdq: regression input from 5823 coordinate compression" $scope) {
    T_use_A$(129, i32);
    /* clang-format off */
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
        1742708828, 582613664,
    });
    /* clang-format on */

    sort_pdq(u_anyS(A_ref(data)), cmp_u_ord$(i32));
    try_(expect_sorted_i32(A_ptr(data), A_len(data)));
} $unscoped(TEST_fn);

TEST_fn_("sort_pdq: partial insertion shift regression with duplicates" $scope) {
    var_(data, A$$(162, i32)) = A_zero();
    usize write = 0;
    data.val[write++] = 0;
    data.val[write++] = 2000000001;

    const usize counts[21] = {
        4, 11, 6, 9, 6, 11, 6, 10, 7, 7, 9, 3, 7, 5, 3, 9, 4, 9, 5, 9, 10
    };
    for (usize val = 0; val < countOf$(counts); ++val) {
        for (usize n = 0; n < counts[val]; ++n) {
            data.val[write++] = as$(i32)(val);
        }
    }

    const i32 updates[10] = { 11, 14, 18, 8, 7, 9, 13, 17, 3, 16 };
    for (usize i = 0; i < countOf$(updates); ++i) {
        data.val[write++] = updates[i];
    }
    try_(TEST_expect(write == A_len(data)));

    sort_pdq(u_anyS(A_ref(data)), cmp_ord$(i32));
    try_(expect_sorted_i32(A_ptr(data), A_len(data)));
} $unscoped(TEST_fn);

TEST_fn_("sort_pdq: deterministic duplicates and patterned values" $scope) {
    var_(data, A$$(257, i32)) = A_zero();

    var state = as$(u32)(0x9E3779B9u);
    for (usize i = 0; i < A_len(data); ++i) {
        state = state * 1664525u + 1013904223u;
        data.val[i] = as$(i32)((state >> 8) % 97) - 48;
        if (i % 17 == 0) data.val[i] = 7;
        if (i % 29 == 0) data.val[i] = -13;
    }

    sort_pdq(u_anyS(A_ref(data)), cmp_ord$(i32));
    try_(expect_sorted_i32(A_ptr(data), A_len(data)));
} $unscoped(TEST_fn);

TEST_fn_("sort_pdqIdx: non-zero range stays in bounds and sorts range" $scope) {
    enum {
        data_len = 2000,
        sort_begin = 1118,
        sort_end = 1764,
    };
    i32 data[data_len] = {};

    for (usize i = 0; i < data_len; ++i) {
        data[i] = as$(i32)((i * 1103515245u + 12345u) % 1009u) - 504;
    }
    let before_begin = data[sort_begin - 1];
    let after_end = data[sort_end];

    let ctx = (PdqIdxTestCtx){
        .data = data,
        .begin = sort_begin,
        .end = sort_end,
    };
    let idx_ctx = (sort_IdxCtx){
        .ordFn = pdq_idx_test_ord,
        .swapFn = pdq_idx_test_swap,
        .inner = u_anyV(ctx),
    };

    sort_pdqIdx($r(sort_begin, sort_end), idx_ctx);

    try_(TEST_expect(data[sort_begin - 1] == before_begin));
    try_(TEST_expect(data[sort_end] == after_end));
    try_(expect_sorted_i32(data + sort_begin, sort_end - sort_begin));
} $unscoped(TEST_fn);

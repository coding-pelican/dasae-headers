#include "dh-main.h"
#include "dh/sort.h"

$static cmp_fn_ord$((i32)(lhs, rhs)) { return pri_ord(lhs, rhs); }
$static cmp_fn_u_ord_default$((i32)(lhs, rhs));
$static fn_((test_sort_i32_descOrd(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) {
    return pri_ord(u_castV$((i32)(rhs)), u_castV$((i32)(lhs)));
};

T_alias$((test_sort_CallStats)(struct test_sort_CallStats {
    var_(ord_count, usize);
    var_(swap_count, usize);
}));
$static fn_((test_sort_countOrd(u_V$raw lhs, u_V$raw rhs, u_V$raw raw_ctx))(cmp_Ord)) {
    let stats = u_castV$((test_sort_CallStats*)(raw_ctx));
    stats->ord_count += 1;
    return pri_ord(u_castV$((i32)(lhs)), u_castV$((i32)(rhs)));
};
$static fn_((test_sort_countIdxOrd(usize lhs, usize rhs, u_V$raw raw_ctx))(cmp_Ord)) {
    let stats = u_castV$((test_sort_CallStats*)(raw_ctx));
    let_ignore = lhs;
    let_ignore = rhs;
    stats->ord_count += 1;
    return cmp_Ord_eq;
};
$static fn_((test_sort_countIdxSwap(usize lhs, usize rhs, u_V$raw raw_ctx))(void)) {
    let stats = u_castV$((test_sort_CallStats*)(raw_ctx));
    let_ignore = lhs;
    let_ignore = rhs;
    stats->swap_count += 1;
};

T_alias$((test_sort_Pair)(struct test_sort_Pair {
    var_(key, i32);
    var_(order, i32);
}));
$static cmp_fn_ord$((test_sort_Pair)(lhs, rhs)) {
    return pri_ord(lhs.key, rhs.key);
}
$static cmp_fn_u_ord_default$((test_sort_Pair)(lhs, rhs));

T_use_A$(64, test_sort_Pair);
T_use_A$(128, test_sort_Pair);

$static fn_((test_sort_fillPattern(i32* data, usize len, usize pattern))(void)) {
    for_(($rt(len))(i)) {
        var value = as$(i32)(0);
        if (pattern == 0) {
            value = intCast$((i32)i);
        } else if (pattern == 1) {
            value = intCast$((i32)(len - i));
        } else if (pattern == 2) {
            value = 7;
        } else if (pattern == 3) {
            value = intCast$((i32)((i * 7u + 3u) % 11u)) - 5;
        } else if (pattern == 4) {
            let folded = (i < (len / 2)) ? i : (len - i);
            value = intCast$((i32)folded);
        } else {
            value = intCast$((i32)((i * 1103515245u + 12345u) % 257u)) - 128;
        }
        *P_at((data)[i]) = value;
    } $end(for);
};

$static fn_((test_sort_algorithmsPatternSorted(usize pattern))(bool)) {
    var_(insert_data, A$$(96, i32)) $undefined;
    var_(heap_data, A$$(96, i32)) $undefined;
    var_(pdq_data, A$$(96, i32)) $undefined;
    var_(block_data, A$$(96, i32)) $undefined;

    test_sort_fillPattern(A_ptr(insert_data), A_len(insert_data), pattern);
    test_sort_fillPattern(A_ptr(heap_data), A_len(heap_data), pattern);
    test_sort_fillPattern(A_ptr(pdq_data), A_len(pdq_data), pattern);
    test_sort_fillPattern(A_ptr(block_data), A_len(block_data), pattern);

    sort_insert(u_anyS(A_ref(insert_data)), cmp_u_ord$(i32));
    sort_heap(u_anyS(A_ref(heap_data)), cmp_u_ord$(i32));
    sort_pdq(u_anyS(A_ref(pdq_data)), cmp_u_ord$(i32));
    sort_block(u_anyS(A_ref(block_data)), cmp_u_ord$(i32));

    return sort_inOrdd(u_anyS(A_ref(insert_data)).as_const, cmp_u_ord$(i32))
        && sort_inOrdd(u_anyS(A_ref(heap_data)).as_const, cmp_u_ord$(i32))
        && sort_inOrdd(u_anyS(A_ref(pdq_data)).as_const, cmp_u_ord$(i32))
        && sort_inOrdd(u_anyS(A_ref(block_data)).as_const, cmp_u_ord$(i32));
};

$static fn_((test_sort_blockStablePattern(usize pattern))(bool)) {
    var_(items, A$$(128, test_sort_Pair)) $undefined;

    for_(($rt(A_len(items)))(i)) {
        var key = as$(i32)(0);
        if (pattern == 0) {
            key = intCast$((i32)(i % 7u));
        } else if (pattern == 1) {
            key = intCast$((i32)((A_len(items) - i) % 9u));
        } else {
            key = intCast$((i32)((i * 13u + 5u) % 11u));
        }
        *A_at((items)[i]) = (test_sort_Pair){
            .key = key,
            .order = intCast$((i32)i),
        };
    } $end(for);

    sort_block(u_anyS(A_ref(items)), cmp_u_ord$(test_sort_Pair));

    for_(($r(1, A_len(items)))(i)) {
        let prev = A_at((items)[i - 1]);
        let curr = A_at((items)[i]);
        if (prev->key > curr->key) return false;
        if (prev->key == curr->key && prev->order > curr->order) return false;
    } $end(for);
    return true;
};

$static fn_((test_sort_blockCacheLenSorted(usize cache_len))(bool)) {
    var_(data, A$$(96, i32)) $undefined;
    var_(cache, A$$(96, i32)) $undefined;

    test_sort_fillPattern(A_ptr(data), A_len(data), 5);
    let sorted = sort_blockCache(
        u_anyS(A_ref(data)),
        cmp_u_ord$(i32),
        u_prefixS(u_anyS(A_ref(cache)), cache_len)
    );

    return sorted.raw.ptr == A_ref(data).ptr
        && sorted.len == A_len(data)
        && sort_inOrdd(u_anyS(A_ref(data)).as_const, cmp_u_ord$(i32));
};

TEST_fn_("sort: insertion heap pdq and block all produce ordered output" $scope) {
    var insert_data = A_from$((i32){ 9, 1, 5, 1, -3, 8, 8, 0 $listed });
    var heap_data = insert_data;
    var pdq_data = insert_data;
    var block_data = insert_data;

    sort_insert(u_anyS(A_ref(insert_data)), cmp_u_ord$(i32));
    sort_heap(u_anyS(A_ref(heap_data)), cmp_u_ord$(i32));
    sort_pdq(u_anyS(A_ref(pdq_data)), cmp_u_ord$(i32));
    sort_block(u_anyS(A_ref(block_data)), cmp_u_ord$(i32));

    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(insert_data)).as_const, cmp_u_ord$(i32))));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(heap_data)).as_const, cmp_u_ord$(i32))));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(pdq_data)).as_const, cmp_u_ord$(i32))));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(block_data)).as_const, cmp_u_ord$(i32))));
} $unscoped(TEST_fn);

TEST_fn_("sort: empty and singleton inputs do not compare swap or mutate" $scope) {
    var empty_storage = A_from$((i32){ 42 $listed });
    var single_storage = A_from$((i32){ 7 $listed });
    var cache_storage = A_from$((i32){ 99 $listed });
    var_(stats, test_sort_CallStats) = {};
    let empty_seq = u_prefixS(u_anyS(A_ref(empty_storage)), 0);
    let single_seq = u_anyS(A_ref(single_storage));
    let cache_seq = u_anyS(A_ref(cache_storage));
    let idx_ctx = (sort_IdxCtx){
        .ordFn = test_sort_countIdxOrd,
        .swapFn = test_sort_countIdxSwap,
        .inner = u_anyV(&stats),
    };

    stats = (test_sort_CallStats){};
    sort_insertCtx(empty_seq, test_sort_countOrd, u_anyV(&stats));
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(*A_at((empty_storage)[0]) == 42));

    stats = (test_sort_CallStats){};
    sort_heapCtx(empty_seq, test_sort_countOrd, u_anyV(&stats));
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(*A_at((empty_storage)[0]) == 42));

    stats = (test_sort_CallStats){};
    sort_pdqCtx(empty_seq, test_sort_countOrd, u_anyV(&stats));
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(*A_at((empty_storage)[0]) == 42));

    stats = (test_sort_CallStats){};
    let empty_sorted = sort_blockCtxCache(empty_seq, test_sort_countOrd, u_anyV(&stats), cache_seq);
    try_(TEST_expect(empty_sorted.raw.ptr == empty_seq.raw.ptr));
    try_(TEST_expect(empty_sorted.len == 0));
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(*A_at((empty_storage)[0]) == 42));
    try_(TEST_expect(*A_at((cache_storage)[0]) == 99));

    stats = (test_sort_CallStats){};
    sort_insertCtx(single_seq, test_sort_countOrd, u_anyV(&stats));
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(*A_at((single_storage)[0]) == 7));

    stats = (test_sort_CallStats){};
    sort_heapCtx(single_seq, test_sort_countOrd, u_anyV(&stats));
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(*A_at((single_storage)[0]) == 7));

    stats = (test_sort_CallStats){};
    sort_pdqCtx(single_seq, test_sort_countOrd, u_anyV(&stats));
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(*A_at((single_storage)[0]) == 7));

    stats = (test_sort_CallStats){};
    let single_sorted = sort_blockCtxCache(single_seq, test_sort_countOrd, u_anyV(&stats), cache_seq);
    try_(TEST_expect(single_sorted.raw.ptr == single_seq.raw.ptr));
    try_(TEST_expect(single_sorted.len == 1));
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(*A_at((single_storage)[0]) == 7));
    try_(TEST_expect(*A_at((cache_storage)[0]) == 99));

    stats = (test_sort_CallStats){};
    sort_insertIdx($r(5, 5), idx_ctx);
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(stats.swap_count == 0));

    stats = (test_sort_CallStats){};
    sort_heapIdx($r(5, 5), idx_ctx);
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(stats.swap_count == 0));

    stats = (test_sort_CallStats){};
    sort_pdqIdx($r(5, 5), idx_ctx);
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(stats.swap_count == 0));

    stats = (test_sort_CallStats){};
    sort_insertIdx($r(5, 6), idx_ctx);
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(stats.swap_count == 0));

    stats = (test_sort_CallStats){};
    sort_heapIdx($r(5, 6), idx_ctx);
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(stats.swap_count == 0));

    stats = (test_sort_CallStats){};
    sort_pdqIdx($r(5, 6), idx_ctx);
    try_(TEST_expect(stats.ord_count == 0));
    try_(TEST_expect(stats.swap_count == 0));
} $unscoped(TEST_fn);

TEST_fn_("sort: algorithms define empty singleton pair and triple behavior" $scope) {
    var empty_storage = A_from$((i32){ 42 $listed });
    var insert_single = A_from$((i32){ 9 $listed });
    var heap_single = A_from$((i32){ 9 $listed });
    var pdq_single = A_from$((i32){ 9 $listed });
    var block_single = A_from$((i32){ 9 $listed });
    var insert_pair = A_from$((i32){ 2, 1 $listed });
    var heap_pair = A_from$((i32){ 2, 1 $listed });
    var pdq_pair = A_from$((i32){ 2, 1 $listed });
    var block_pair = A_from$((i32){ 2, 1 $listed });
    var insert_triple = A_from$((i32){ 1, -1, 0 $listed });
    var heap_triple = A_from$((i32){ 1, -1, 0 $listed });
    var pdq_triple = A_from$((i32){ 1, -1, 0 $listed });
    var block_triple = A_from$((i32){ 1, -1, 0 $listed });

    sort_insert(u_prefixS(u_anyS(A_ref(empty_storage)), 0), cmp_u_ord$(i32));
    sort_heap(u_prefixS(u_anyS(A_ref(empty_storage)), 0), cmp_u_ord$(i32));
    sort_pdq(u_prefixS(u_anyS(A_ref(empty_storage)), 0), cmp_u_ord$(i32));
    sort_block(u_prefixS(u_anyS(A_ref(empty_storage)), 0), cmp_u_ord$(i32));
    try_(TEST_expect(*A_at((empty_storage)[0]) == 42));

    sort_insert(u_anyS(A_ref(insert_single)), cmp_u_ord$(i32));
    sort_heap(u_anyS(A_ref(heap_single)), cmp_u_ord$(i32));
    sort_pdq(u_anyS(A_ref(pdq_single)), cmp_u_ord$(i32));
    sort_block(u_anyS(A_ref(block_single)), cmp_u_ord$(i32));
    try_(TEST_expect(*A_at((insert_single)[0]) == 9));
    try_(TEST_expect(*A_at((heap_single)[0]) == 9));
    try_(TEST_expect(*A_at((pdq_single)[0]) == 9));
    try_(TEST_expect(*A_at((block_single)[0]) == 9));

    sort_insert(u_anyS(A_ref(insert_pair)), cmp_u_ord$(i32));
    sort_heap(u_anyS(A_ref(heap_pair)), cmp_u_ord$(i32));
    sort_pdq(u_anyS(A_ref(pdq_pair)), cmp_u_ord$(i32));
    sort_block(u_anyS(A_ref(block_pair)), cmp_u_ord$(i32));
    try_(TEST_expect(*A_at((insert_pair)[0]) == 1 && *A_at((insert_pair)[1]) == 2));
    try_(TEST_expect(*A_at((heap_pair)[0]) == 1 && *A_at((heap_pair)[1]) == 2));
    try_(TEST_expect(*A_at((pdq_pair)[0]) == 1 && *A_at((pdq_pair)[1]) == 2));
    try_(TEST_expect(*A_at((block_pair)[0]) == 1 && *A_at((block_pair)[1]) == 2));

    sort_insert(u_anyS(A_ref(insert_triple)), cmp_u_ord$(i32));
    sort_heap(u_anyS(A_ref(heap_triple)), cmp_u_ord$(i32));
    sort_pdq(u_anyS(A_ref(pdq_triple)), cmp_u_ord$(i32));
    sort_block(u_anyS(A_ref(block_triple)), cmp_u_ord$(i32));
    try_(TEST_expect(*A_at((insert_triple)[0]) == -1 && *A_at((insert_triple)[1]) == 0 && *A_at((insert_triple)[2]) == 1));
    try_(TEST_expect(*A_at((heap_triple)[0]) == -1 && *A_at((heap_triple)[1]) == 0 && *A_at((heap_triple)[2]) == 1));
    try_(TEST_expect(*A_at((pdq_triple)[0]) == -1 && *A_at((pdq_triple)[1]) == 0 && *A_at((pdq_triple)[2]) == 1));
    try_(TEST_expect(*A_at((block_triple)[0]) == -1 && *A_at((block_triple)[1]) == 0 && *A_at((block_triple)[2]) == 1));
} $unscoped(TEST_fn);

TEST_fn_("sort: algorithms cover representative ordered reverse equal duplicate and patterned distributions" $scope) {
    for_(($rt(6))(pattern)) {
        try_(TEST_expect(test_sort_algorithmsPatternSorted(pattern)));
    } $end(for);
} $unscoped(TEST_fn);

TEST_fn_("sort: algorithms honor descending comparator ordering" $scope) {
    var insert_data = A_from$((i32){ 0, -9, 7, 7, 4, -2, 12, 1, 12 $listed });
    var heap_data = insert_data;
    var pdq_data = insert_data;
    var block_data = insert_data;

    sort_insert(u_anyS(A_ref(insert_data)), test_sort_i32_descOrd);
    sort_heap(u_anyS(A_ref(heap_data)), test_sort_i32_descOrd);
    sort_pdq(u_anyS(A_ref(pdq_data)), test_sort_i32_descOrd);
    sort_block(u_anyS(A_ref(block_data)), test_sort_i32_descOrd);

    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(insert_data)).as_const, test_sort_i32_descOrd)));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(heap_data)).as_const, test_sort_i32_descOrd)));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(pdq_data)).as_const, test_sort_i32_descOrd)));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(block_data)).as_const, test_sort_i32_descOrd)));

    try_(TEST_expect(*A_at((insert_data)[0]) == 12));
    try_(TEST_expect(*A_at((heap_data)[0]) == 12));
    try_(TEST_expect(*A_at((pdq_data)[0]) == 12));
    try_(TEST_expect(*A_at((block_data)[0]) == 12));
} $unscoped(TEST_fn);

TEST_fn_("sort: block sort keeps equal-key items stable" $scope) {
    var items = A_from$((test_sort_Pair){
        { .key = 2, .order = 0 },
        { .key = 1, .order = 1 },
        { .key = 2, .order = 2 },
        { .key = 1, .order = 3 },
        { .key = 3, .order = 4 },
        { .key = 2, .order = 5 },
        { .key = 1, .order = 6 },
        { .key = 3, .order = 7 } $listed
    });

    sort_block(u_anyS(A_ref(items)), cmp_u_ord$(test_sort_Pair));

    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(items)).as_const, cmp_u_ord$(test_sort_Pair))));
    try_(TEST_expect(A_at((items)[0])->key == 1 && A_at((items)[0])->order == 1));
    try_(TEST_expect(A_at((items)[1])->key == 1 && A_at((items)[1])->order == 3));
    try_(TEST_expect(A_at((items)[2])->key == 1 && A_at((items)[2])->order == 6));
    try_(TEST_expect(A_at((items)[3])->key == 2 && A_at((items)[3])->order == 0));
    try_(TEST_expect(A_at((items)[4])->key == 2 && A_at((items)[4])->order == 2));
    try_(TEST_expect(A_at((items)[5])->key == 2 && A_at((items)[5])->order == 5));
    try_(TEST_expect(A_at((items)[6])->key == 3 && A_at((items)[6])->order == 4));
    try_(TEST_expect(A_at((items)[7])->key == 3 && A_at((items)[7])->order == 7));
} $unscoped(TEST_fn);

TEST_fn_("sort: block sort defines stable tie order across representative distributions" $scope) {
    for_(($rt(3))(pattern)) {
        try_(TEST_expect(test_sort_blockStablePattern(pattern)));
    } $end(for);
} $unscoped(TEST_fn);

TEST_fn_("sort: block sort keeps equal-key stability beyond insertion fallback" $scope) {
    var_(items, A$$(64, test_sort_Pair)) $undefined;

    for_(($rt(A_len(items)))(i)) {
        *A_at((items)[i]) = (test_sort_Pair){
            .key = intCast$((i32)((i * 7u + 3u) % 5u)),
            .order = intCast$((i32)i),
        };
    } $end(for);

    sort_block(u_anyS(A_ref(items)), cmp_u_ord$(test_sort_Pair));

    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(items)).as_const, cmp_u_ord$(test_sort_Pair))));
    for_(($r(1, A_len(items)))(i)) {
        let prev = A_at((items)[i - 1]);
        let curr = A_at((items)[i]);
        if (prev->key == curr->key) {
            try_(TEST_expect(prev->order < curr->order));
        }
    } $end(for);
} $unscoped(TEST_fn);

TEST_fn_("sort: block cache returns sorted sequence while preserving caller cache ownership" $scope) {
    var data = A_from$((i32){ 5, 4, 3, 2, 1, 0, -1, -2 $listed });
    var_(cache, A$$(4, i32)) $undefined;
    let sorted = sort_blockCache(u_anyS(A_ref(data)), cmp_u_ord$(i32), u_anyS(A_ref(cache)));

    try_(TEST_expect(sorted.raw.ptr == A_ref(data).ptr));
    try_(TEST_expect(sorted.len == A_len(data)));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(data)).as_const, cmp_u_ord$(i32))));
} $unscoped(TEST_fn);

TEST_fn_("sort: block cache sorts inputs beyond insertion fallback" $scope) {
    var_(data, A$$(64, i32)) $undefined;
    var_(cache, A$$(8, i32)) $undefined;

    for_(($rt(A_len(data)))(i)) {
        *A_at((data)[i]) = intCast$((i32)(((A_len(data) - i) * 17u + i * 3u) % 41u)) - 20;
    } $end(for);

    let sorted = sort_blockCache(u_anyS(A_ref(data)), cmp_u_ord$(i32), u_anyS(A_ref(cache)));

    try_(TEST_expect(sorted.raw.ptr == A_ref(data).ptr));
    try_(TEST_expect(sorted.len == A_len(data)));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(data)).as_const, cmp_u_ord$(i32))));
} $unscoped(TEST_fn);

TEST_fn_("sort: block cache uses external merge path beyond insertion fallback" $scope) {
    var_(data, A$$(64, i32)) $undefined;
    var_(cache, A$$(64, i32)) $undefined;

    for_(($rt(A_len(data)))(i)) {
        *A_at((data)[i]) = intCast$((i32)((i * 23u + 19u) % 53u)) - 26;
    } $end(for);

    let sorted = sort_blockCache(u_anyS(A_ref(data)), cmp_u_ord$(i32), u_anyS(A_ref(cache)));

    try_(TEST_expect(sorted.raw.ptr == A_ref(data).ptr));
    try_(TEST_expect(sorted.len == A_len(data)));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(data)).as_const, cmp_u_ord$(i32))));
} $unscoped(TEST_fn);

TEST_fn_("sort: block sort handles lengths just above insertion fallback threshold" $scope) {
    var_(data_25, A$$(25, i32)) $undefined;
    var_(data_31, A$$(31, i32)) $undefined;

    for_(($rt(A_len(data_25)))(i)) {
        *A_at((data_25)[i]) = intCast$((i32)(((A_len(data_25) - i) * 13u + i) % 29u)) - 14;
    } $end(for);
    for_(($rt(A_len(data_31)))(i)) {
        *A_at((data_31)[i]) = intCast$((i32)(((A_len(data_31) - i) * 17u + i * 5u) % 37u)) - 18;
    } $end(for);

    sort_block(u_anyS(A_ref(data_25)), cmp_u_ord$(i32));
    sort_block(u_anyS(A_ref(data_31)), cmp_u_ord$(i32));

    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(data_25)).as_const, cmp_u_ord$(i32))));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(data_31)).as_const, cmp_u_ord$(i32))));
} $unscoped(TEST_fn);

TEST_fn_("sort: block cache length matrix preserves the same sorted contract" $scope) {
    var cache_lens = A_from$((usize){ 0, 1, 8, 24, 48, 96 $listed });

    for_(($a(cache_lens))(cache_len)) {
        try_(TEST_expect(test_sort_blockCacheLenSorted(*cache_len)));
    } $end(for);
} $unscoped(TEST_fn);

TEST_fn_("sort: block cache handles uneven merge levels with right-side cache" $scope) {
    var_(items, A$$(128, test_sort_Pair)) $undefined;
    var_(cache, A$$(26, test_sort_Pair)) $undefined;

    for_(($rt(A_len(items)))(i)) {
        *A_at((items)[i]) = (test_sort_Pair){
            .key = intCast$((i32)((i * 37u + 11u) % 17u)),
            .order = intCast$((i32)i),
        };
    } $end(for);

    let sorted = sort_blockCache(
        u_anyS(A_ref(items)),
        cmp_u_ord$(test_sort_Pair),
        u_anyS(A_ref(cache))
    );

    try_(TEST_expect(sorted.raw.ptr == A_ref(items).ptr));
    try_(TEST_expect(sorted.len == A_len(items)));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(items)).as_const, cmp_u_ord$(test_sort_Pair))));
    for_(($r(1, A_len(items)))(i)) {
        let prev = A_at((items)[i - 1]);
        let curr = A_at((items)[i]);
        if (prev->key == curr->key) {
            try_(TEST_expect(prev->order < curr->order));
        }
    } $end(for);
} $unscoped(TEST_fn);

TEST_fn_("sort: block cache preserves stability without external cache" $scope) {
    var_(items, A$$(192, test_sort_Pair)) $undefined;
    var_(cache, A$$(1, test_sort_Pair)) $undefined;

    for_(($rt(A_len(items)))(i)) {
        *A_at((items)[i]) = (test_sort_Pair){
            .key = intCast$((i32)((i * 41u + 7u) % 23u)),
            .order = intCast$((i32)i),
        };
    } $end(for);

    let sorted = sort_blockCache(
        u_anyS(A_ref(items)),
        cmp_u_ord$(test_sort_Pair),
        u_prefixS(u_anyS(A_ref(cache)), 0)
    );

    try_(TEST_expect(sorted.raw.ptr == A_ref(items).ptr));
    try_(TEST_expect(sorted.len == A_len(items)));
    try_(TEST_expect(sort_inOrdd(u_anyS(A_ref(items)).as_const, cmp_u_ord$(test_sort_Pair))));
    for_(($r(1, A_len(items)))(i)) {
        let prev = A_at((items)[i - 1]);
        let curr = A_at((items)[i]);
        if (prev->key == curr->key) {
            try_(TEST_expect(prev->order < curr->order));
        }
    } $end(for);
} $unscoped(TEST_fn);

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

T_alias$((test_sort_IdxCtx)(struct test_sort_IdxCtx {
    var_(data, i32*);
    var_(range, R);
}));
$static fn_((test_sort_IdxCtx_ord(usize lhs, usize rhs, u_V$raw raw_ctx))(cmp_Ord)) {
    let ctx = u_castV$((test_sort_IdxCtx)(raw_ctx));
    claim_assert(ctx.range.begin <= lhs), claim_assert(lhs < ctx.range.end);
    claim_assert(ctx.range.begin <= rhs), claim_assert(rhs < ctx.range.end);
    return pri_ord(*P_at((ctx.data)[lhs]), *P_at((ctx.data)[rhs]));
};
$static fn_((test_sort_IdxCtx_swap(usize lhs, usize rhs, u_V$raw raw_ctx))(void)) {
    let ctx = u_castV$((test_sort_IdxCtx)(raw_ctx));
    claim_assert(ctx.range.begin <= lhs), claim_assert(lhs < ctx.range.end);
    claim_assert(ctx.range.begin <= rhs), claim_assert(rhs < ctx.range.end);
    return pri_swap(P_at((ctx.data)[lhs]), P_at((ctx.data)[rhs]));
};
$static fn_((test_sort_fillIdxData(i32* data, usize len))(void)) {
    for_(($rt(len))(i)) {
        *P_at((data)[i]) = intCast$((i32)((i * 1103515245u + 12345u) % 1009u)) - 504;
    } $end(for);
};

TEST_fn_("sort: idx algorithms respect non-zero range boundaries" $scope) {
    enum {
        data_len = usize_(256),
        sort_begin = usize_(37),
        sort_end = usize_(219),
    };
    var_(insert_data, A$$(data_len, i32)) $undefined;
    var_(heap_data, A$$(data_len, i32)) $undefined;
    var_(pdq_data, A$$(data_len, i32)) $undefined;

    test_sort_fillIdxData(A_ptr(insert_data), A_len(insert_data));
    test_sort_fillIdxData(A_ptr(heap_data), A_len(heap_data));
    test_sort_fillIdxData(A_ptr(pdq_data), A_len(pdq_data));

    let insert_before_begin = *A_at((insert_data)[sort_begin - 1]);
    let insert_after_end = *A_at((insert_data)[sort_end]);
    let heap_before_begin = *A_at((heap_data)[sort_begin - 1]);
    let heap_after_end = *A_at((heap_data)[sort_end]);
    let pdq_before_begin = *A_at((pdq_data)[sort_begin - 1]);
    let pdq_after_end = *A_at((pdq_data)[sort_end]);

    let insert_ctx = (test_sort_IdxCtx){
        .data = A_ptr(insert_data),
        .range = $r(sort_begin, sort_end),
    };
    let heap_ctx = (test_sort_IdxCtx){
        .data = A_ptr(heap_data),
        .range = $r(sort_begin, sort_end),
    };
    let pdq_ctx = (test_sort_IdxCtx){
        .data = A_ptr(pdq_data),
        .range = $r(sort_begin, sort_end),
    };

    sort_insertIdx($r(sort_begin, sort_end), (sort_IdxCtx){
        .ordFn = test_sort_IdxCtx_ord,
        .swapFn = test_sort_IdxCtx_swap,
        .inner = u_deref(u_anyP(&insert_ctx)),
    });
    sort_heapIdx($r(sort_begin, sort_end), (sort_IdxCtx){
        .ordFn = test_sort_IdxCtx_ord,
        .swapFn = test_sort_IdxCtx_swap,
        .inner = u_deref(u_anyP(&heap_ctx)),
    });
    sort_pdqIdx($r(sort_begin, sort_end), (sort_IdxCtx){
        .ordFn = test_sort_IdxCtx_ord,
        .swapFn = test_sort_IdxCtx_swap,
        .inner = u_deref(u_anyP(&pdq_ctx)),
    });

    try_(TEST_expect(*A_at((insert_data)[sort_begin - 1]) == insert_before_begin));
    try_(TEST_expect(*A_at((insert_data)[sort_end]) == insert_after_end));
    try_(TEST_expect(sort_inOrdd(
        u_anyS(A_slice((insert_data)$r(sort_begin, sort_end))).as_const,
        cmp_u_ord$(i32)
    )));
    try_(TEST_expect(*A_at((heap_data)[sort_begin - 1]) == heap_before_begin));
    try_(TEST_expect(*A_at((heap_data)[sort_end]) == heap_after_end));
    try_(TEST_expect(sort_inOrdd(
        u_anyS(A_slice((heap_data)$r(sort_begin, sort_end))).as_const,
        cmp_u_ord$(i32)
    )));
    try_(TEST_expect(*A_at((pdq_data)[sort_begin - 1]) == pdq_before_begin));
    try_(TEST_expect(*A_at((pdq_data)[sort_end]) == pdq_after_end));
    try_(TEST_expect(sort_inOrdd(
        u_anyS(A_slice((pdq_data)$r(sort_begin, sort_end))).as_const,
        cmp_u_ord$(i32)
    )));
} $unscoped(TEST_fn);

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

    let ctx = (test_sort_IdxCtx){
        .data = A_ptr(data),
        .range = $r(sort_begin, sort_end),
    };
    let idx_ctx = (sort_IdxCtx){
        .ordFn = test_sort_IdxCtx_ord,
        .swapFn = test_sort_IdxCtx_swap,
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

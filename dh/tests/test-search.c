#include "dh-main.h"
#include "dh/search.h"

$static cmp_fn_ord$((i32)(lhs, rhs)) { return pri_ord(lhs, rhs); }
$static cmp_fn_eql$((i32)(lhs, rhs)) { return lhs == rhs; }
$static cmp_fn_u_ord_default$((i32)(lhs, rhs));
$static cmp_fn_u_eql_default$((i32)(lhs, rhs));
$static cmp_fn_ord$((u32)(lhs, rhs)) { return pri_ord(lhs, rhs); }
$static cmp_fn_u_ord_default$((u32)(lhs, rhs));
$static cmp_fn_ord$((f32)(lhs, rhs)) { return pri_ord(lhs, rhs); }
$static cmp_fn_u_ord_default$((f32)(lhs, rhs));

T_alias$((test_search_Range)(struct test_search_Range {
    var_(begin, i32);
    var_(end, i32);
}));
$static fn_((test_search_Range_ord(u_V$raw val, u_V$raw ctx))(cmp_Ord)) {
    let range = u_castV$((test_search_Range)(val));
    let needle = u_castV$((i32)(ctx));
    if (needle < range.begin) return cmp_Ord_gt;
    if (range.end < needle) return cmp_Ord_lt;
    return cmp_Ord_eq;
};

T_alias$((test_search_KeyLen)(struct test_search_KeyLen {
    var_(len, usize);
}));
$static fn_((test_search_KeyLen_ord(u_V$raw val, u_V$raw ctx))(cmp_Ord)) {
    return pri_ord(u_castV$((test_search_KeyLen)(val)).len, u_castV$((usize)(ctx)));
};

$static fn_((test_search_lessThan(u_V$raw val, u_V$raw ctx))(bool)) {
    return u_castV$((i32)(val)) < u_castV$((i32)(ctx));
};
$static fn_((test_search_lessEq(u_V$raw val, u_V$raw ctx))(bool)) {
    return u_castV$((i32)(val)) <= u_castV$((i32)(ctx));
};
$static fn_((test_search_u32LessThan(u_V$raw val, u_V$raw ctx))(bool)) {
    return u_castV$((u32)(val)) < u_castV$((u32)(ctx));
};
$static fn_((test_search_u32LessEq(u_V$raw val, u_V$raw ctx))(bool)) {
    return u_castV$((u32)(val)) <= u_castV$((u32)(ctx));
};
$static fn_((test_search_even(u_V$raw val, u_V$raw ctx))(bool)) {
    let_ignore = ctx;
    return (u_castV$((u8)(val)) % 2) == 0;
};

TEST_fn_("search: linear search defines first last and missing match semantics" $scope) {
    var data = A_from$((i32){ 4, 2, 7, 2, 9, 2 $listed });
    let seq = u_anyS(A_ref(data)).as_const;

    try_(TEST_expect(unwrap_(search_linearFirst(seq, u_anyV(as$(i32)(2)), cmp_u_ord$(i32))) == 1));
    try_(TEST_expect(unwrap_(search_linearLast(seq, u_anyV(as$(i32)(2)), cmp_u_ord$(i32))) == 5));
    try_(TEST_expect(isNone(search_linearFirst(seq, u_anyV(as$(i32)(8)), cmp_u_ord$(i32)))));
    try_(TEST_expect(isNone(search_linearLast(seq, u_anyV(as$(i32)(8)), cmp_u_ord$(i32)))));
} $unscoped(TEST_fn);

TEST_fn_("search: binary search covers empty singleton and custom comparator shapes" $scope) {
    var ints = A_from$((i32){ -100, -25, 2, 98, 99, 100 $listed });
    var single = A_from$((u32){ 1 $listed });
    var ranges = A_from$((test_search_Range){
        { .begin = -100, .end = -50 },
        { .begin = -40, .end = -20 },
        { .begin = -10, .end = 20 },
        { .begin = 30, .end = 40 } $listed
    });
    var by_len = A_from$((test_search_KeyLen){
        { .len = 0 },
        { .len = 3 },
        { .len = 4 },
        { .len = 5 } $listed
    });

    let int_seq = u_anyS(A_ref(ints)).as_const;
    let empty_int_seq = u_prefixS(int_seq, 0);
    let single_seq = u_anyS(A_ref(single)).as_const;
    let range_seq = u_anyS(A_ref(ranges)).as_const;
    let len_seq = u_anyS(A_ref(by_len)).as_const;

    try_(TEST_expect(isNone(search_binary(empty_int_seq, u_anyV(as$(i32)(1)), cmp_u_ord$(i32)))));
    try_(TEST_expect(unwrap_(search_binary(single_seq, u_anyV(as$(u32)(1)), cmp_u_ord$(u32))) == 0));
    try_(TEST_expect(isNone(search_binary(single_seq, u_anyV(as$(u32)(0)), cmp_u_ord$(u32)))));
    try_(TEST_expect(isNone(search_binary(single_seq, u_anyV(as$(u32)(2)), cmp_u_ord$(u32)))));
    try_(TEST_expect(unwrap_(search_binary(int_seq, u_anyV(as$(i32)(98)), cmp_u_ord$(i32))) == 3));
    try_(TEST_expect(isNone(search_binary(range_seq, u_anyV(as$(i32)(-45)), test_search_Range_ord))));
    try_(TEST_expect(unwrap_(search_binary(range_seq, u_anyV(as$(i32)(10)), test_search_Range_ord)) == 2));
    try_(TEST_expect(unwrap_(search_binary(range_seq, u_anyV(as$(i32)(-20)), test_search_Range_ord)) == 1));
    try_(TEST_expect(unwrap_(search_binary(len_seq, u_anyV(as$(usize)(4)), test_search_KeyLen_ord)) == 2));
} $unscoped(TEST_fn);

TEST_fn_("search: binary lower upper and equal range define duplicate boundaries" $scope) {
    var data = A_from$((i32){ 1, 2, 2, 2, 4, 5, 7 $listed });
    let seq = u_anyS(A_ref(data)).as_const;

    let found = unwrap_(search_binary(seq, u_anyV(as$(i32)(2)), cmp_u_ord$(i32)));
    try_(TEST_expect(1 <= found && found <= 3));
    try_(TEST_expect(isNone(search_binary(seq, u_anyV(as$(i32)(3)), cmp_u_ord$(i32)))));

    try_(TEST_expect(search_lowerBound(seq, u_anyV(as$(i32)(0)), cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_lowerBound(seq, u_anyV(as$(i32)(2)), cmp_u_ord$(i32)) == 1));
    try_(TEST_expect(search_upperBound(seq, u_anyV(as$(i32)(2)), cmp_u_ord$(i32)) == 4));
    try_(TEST_expect(search_lowerBound(seq, u_anyV(as$(i32)(8)), cmp_u_ord$(i32)) == A_len(data)));
    try_(TEST_expect(search_upperBound(seq, u_anyV(as$(i32)(8)), cmp_u_ord$(i32)) == A_len(data)));

    let range = search_eqRange(seq, u_anyV(as$(i32)(2)), cmp_u_ord$(i32));
    try_(TEST_expect(range.begin == 1));
    try_(TEST_expect(range.end == 4));

    let missing = search_eqRange(seq, u_anyV(as$(i32)(3)), cmp_u_ord$(i32));
    try_(TEST_expect(missing.begin == 4));
    try_(TEST_expect(missing.end == 4));
} $unscoped(TEST_fn);

TEST_fn_("search: lower upper and equal range match boundary matrix semantics" $scope) {
    var ordered = A_from$((i32){ 2, 4, 8, 16, 32, 64 $listed });
    var dup_less = A_from$((u32){ 2, 4, 7, 7, 7, 7, 16, 32, 64 $listed });
    var dup_equal = A_from$((u32){ 2, 4, 8, 8, 8, 8, 16, 32, 64 $listed });
    var floats = A_from$((f32){ -54.2f, -26.7f, 0.0f, 56.55f, 100.1f, 322.0f $listed });
    var by_len = A_from$((test_search_KeyLen){
        { .len = 4 },
        { .len = 5 },
        { .len = 5 },
        { .len = 6 },
        { .len = 6 },
        { .len = 6 },
        { .len = 7 },
        { .len = 7 } $listed
    });

    let ordered_seq = u_anyS(A_ref(ordered)).as_const;
    let empty_seq = u_prefixS(ordered_seq, 0);
    let dup_less_seq = u_anyS(A_ref(dup_less)).as_const;
    let dup_equal_seq = u_anyS(A_ref(dup_equal)).as_const;
    let float_seq = u_anyS(A_ref(floats)).as_const;
    let len_seq = u_anyS(A_ref(by_len)).as_const;

    try_(TEST_expect(search_lowerBound(empty_seq, u_anyV(as$(i32)(0)), cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_lowerBound(ordered_seq, u_anyV(as$(i32)(0)), cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_lowerBound(ordered_seq, u_anyV(as$(i32)(2)), cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_lowerBound(ordered_seq, u_anyV(as$(i32)(5)), cmp_u_ord$(i32)) == 2));
    try_(TEST_expect(search_lowerBound(ordered_seq, u_anyV(as$(i32)(8)), cmp_u_ord$(i32)) == 2));
    try_(TEST_expect(search_lowerBound(dup_less_seq, u_anyV(as$(u32)(8)), cmp_u_ord$(u32)) == 6));
    try_(TEST_expect(search_lowerBound(dup_equal_seq, u_anyV(as$(u32)(8)), cmp_u_ord$(u32)) == 2));
    try_(TEST_expect(search_lowerBound(ordered_seq, u_anyV(as$(i32)(64)), cmp_u_ord$(i32)) == 5));
    try_(TEST_expect(search_lowerBound(ordered_seq, u_anyV(as$(i32)(100)), cmp_u_ord$(i32)) == 6));
    try_(TEST_expect(search_lowerBound(float_seq, u_anyV(as$(f32)(-33.4f)), cmp_u_ord$(f32)) == 1));

    try_(TEST_expect(search_upperBound(empty_seq, u_anyV(as$(i32)(0)), cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_upperBound(ordered_seq, u_anyV(as$(i32)(0)), cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_upperBound(ordered_seq, u_anyV(as$(i32)(2)), cmp_u_ord$(i32)) == 1));
    try_(TEST_expect(search_upperBound(ordered_seq, u_anyV(as$(i32)(5)), cmp_u_ord$(i32)) == 2));
    try_(TEST_expect(search_upperBound(dup_less_seq, u_anyV(as$(u32)(8)), cmp_u_ord$(u32)) == 6));
    try_(TEST_expect(search_upperBound(dup_equal_seq, u_anyV(as$(u32)(8)), cmp_u_ord$(u32)) == 6));
    try_(TEST_expect(search_upperBound(ordered_seq, u_anyV(as$(i32)(8)), cmp_u_ord$(i32)) == 3));
    try_(TEST_expect(search_upperBound(ordered_seq, u_anyV(as$(i32)(64)), cmp_u_ord$(i32)) == 6));
    try_(TEST_expect(search_upperBound(ordered_seq, u_anyV(as$(i32)(100)), cmp_u_ord$(i32)) == 6));
    try_(TEST_expect(search_upperBound(float_seq, u_anyV(as$(f32)(-33.4f)), cmp_u_ord$(f32)) == 1));

    try_(TEST_expect(R_eq(search_eqRange(empty_seq, u_anyV(as$(i32)(0)), cmp_u_ord$(i32)), $r(0, 0))));
    try_(TEST_expect(R_eq(search_eqRange(ordered_seq, u_anyV(as$(i32)(0)), cmp_u_ord$(i32)), $r(0, 0))));
    try_(TEST_expect(R_eq(search_eqRange(ordered_seq, u_anyV(as$(i32)(2)), cmp_u_ord$(i32)), $r(0, 1))));
    try_(TEST_expect(R_eq(search_eqRange(ordered_seq, u_anyV(as$(i32)(5)), cmp_u_ord$(i32)), $r(2, 2))));
    try_(TEST_expect(R_eq(search_eqRange(ordered_seq, u_anyV(as$(i32)(8)), cmp_u_ord$(i32)), $r(2, 3))));
    try_(TEST_expect(R_eq(search_eqRange(ordered_seq, u_anyV(as$(i32)(64)), cmp_u_ord$(i32)), $r(5, 6))));
    try_(TEST_expect(R_eq(search_eqRange(ordered_seq, u_anyV(as$(i32)(100)), cmp_u_ord$(i32)), $r(6, 6))));
    try_(TEST_expect(R_eq(search_eqRange(dup_equal_seq, u_anyV(as$(u32)(8)), cmp_u_ord$(u32)), $r(2, 6))));
    try_(TEST_expect(R_eq(search_eqRange(len_seq, u_anyV(as$(usize)(6)), test_search_KeyLen_ord), $r(3, 6))));
} $unscoped(TEST_fn);

TEST_fn_("search: partition point returns the first false predicate boundary" $scope) {
    var data = A_from$((i32){ 1, 2, 3, 4, 5, 6 $listed });
    let seq = u_anyS(A_ref(data)).as_const;

    try_(TEST_expect(search_partPoint(seq, u_anyV(as$(i32)(4)), test_search_lessThan) == 3));
    try_(TEST_expect(search_partPoint(seq, u_anyV(as$(i32)(0)), test_search_lessThan) == 0));
    try_(TEST_expect(search_partPoint(seq, u_anyV(as$(i32)(9)), test_search_lessThan) == A_len(data)));
} $unscoped(TEST_fn);

TEST_fn_("search: partition point covers strict inclusive and arbitrary predicates" $scope) {
    var ints = A_from$((i32){ 2, 4, 8, 16, 32, 64 $listed });
    var dup_less = A_from$((u32){ 2, 4, 7, 7, 7, 7, 16, 32, 64 $listed });
    var dup_equal = A_from$((u32){ 2, 4, 8, 8, 8, 8, 16, 32, 64 $listed });
    var evens_then_odds = A_from$((u8){ 0, 50, 14, 2, 5, 71 $listed });

    let int_seq = u_anyS(A_ref(ints)).as_const;
    let empty_int_seq = u_prefixS(int_seq, 0);
    let dup_less_seq = u_anyS(A_ref(dup_less)).as_const;
    let dup_equal_seq = u_anyS(A_ref(dup_equal)).as_const;
    let even_seq = u_anyS(A_ref(evens_then_odds)).as_const;

    try_(TEST_expect(search_partPoint(empty_int_seq, u_anyV(as$(i32)(0)), test_search_lessThan) == 0));
    try_(TEST_expect(search_partPoint(int_seq, u_anyV(as$(i32)(0)), test_search_lessThan) == 0));
    try_(TEST_expect(search_partPoint(int_seq, u_anyV(as$(i32)(2)), test_search_lessThan) == 0));
    try_(TEST_expect(search_partPoint(int_seq, u_anyV(as$(i32)(5)), test_search_lessThan) == 2));
    try_(TEST_expect(search_partPoint(int_seq, u_anyV(as$(i32)(8)), test_search_lessThan) == 2));
    try_(TEST_expect(search_partPoint(int_seq, u_anyV(as$(i32)(64)), test_search_lessThan) == 5));
    try_(TEST_expect(search_partPoint(int_seq, u_anyV(as$(i32)(100)), test_search_lessThan) == 6));

    try_(TEST_expect(search_partPoint(int_seq, u_anyV(as$(i32)(0)), test_search_lessEq) == 0));
    try_(TEST_expect(search_partPoint(int_seq, u_anyV(as$(i32)(2)), test_search_lessEq) == 1));
    try_(TEST_expect(search_partPoint(int_seq, u_anyV(as$(i32)(5)), test_search_lessEq) == 2));
    try_(TEST_expect(search_partPoint(dup_less_seq, u_anyV(as$(u32)(8)), test_search_u32LessThan) == 6));
    try_(TEST_expect(search_partPoint(dup_equal_seq, u_anyV(as$(u32)(8)), test_search_u32LessThan) == 2));
    try_(TEST_expect(search_partPoint(dup_equal_seq, u_anyV(as$(u32)(8)), test_search_u32LessEq) == 6));
    try_(TEST_expect(search_partPoint(even_seq, u_anyV(as$(u8)(0)), test_search_even) == 4));
} $unscoped(TEST_fn);

TEST_fn_("search: min max helpers define first and last tie handling" $scope) {
    var data = A_from$((i32){ 3, 1, 4, 1, 4, 2 $listed });
    let seq = u_anyS(A_ref(data)).as_const;

    let first_pair = search_firstMinMax(seq, cmp_u_ord$(i32));
    try_(TEST_expect(first_pair.min == 1));
    try_(TEST_expect(first_pair.max == 2));
    try_(TEST_expect(search_firstMin(seq, cmp_u_ord$(i32)) == 1));
    try_(TEST_expect(search_firstMax(seq, cmp_u_ord$(i32)) == 2));

    let last_pair = search_lastMinMax(seq, cmp_u_ord$(i32));
    try_(TEST_expect(last_pair.min == 3));
    try_(TEST_expect(last_pair.max == 4));
    try_(TEST_expect(search_lastMin(seq, cmp_u_ord$(i32)) == 3));
    try_(TEST_expect(search_lastMax(seq, cmp_u_ord$(i32)) == 4));
} $unscoped(TEST_fn);

TEST_fn_("search: min max helpers define singleton and all-equal tie contracts" $scope) {
    var single = A_from$((i32){ 7 $listed });
    var all_equal = A_from$((i32){ 5, 5, 5, 5 $listed });
    let single_seq = u_anyS(A_ref(single)).as_const;
    let equal_seq = u_anyS(A_ref(all_equal)).as_const;

    let single_first = search_firstMinMax(single_seq, cmp_u_ord$(i32));
    let single_last = search_lastMinMax(single_seq, cmp_u_ord$(i32));
    try_(TEST_expect(single_first.min == 0 && single_first.max == 0));
    try_(TEST_expect(single_last.min == 0 && single_last.max == 0));
    try_(TEST_expect(search_firstMin(single_seq, cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_lastMin(single_seq, cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_firstMax(single_seq, cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_lastMax(single_seq, cmp_u_ord$(i32)) == 0));

    let equal_first = search_firstMinMax(equal_seq, cmp_u_ord$(i32));
    let equal_last = search_lastMinMax(equal_seq, cmp_u_ord$(i32));
    try_(TEST_expect(equal_first.min == 0 && equal_first.max == 0));
    try_(TEST_expect(equal_last.min == 3 && equal_last.max == 3));
    try_(TEST_expect(search_firstMin(equal_seq, cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_lastMin(equal_seq, cmp_u_ord$(i32)) == 3));
    try_(TEST_expect(search_firstMax(equal_seq, cmp_u_ord$(i32)) == 0));
    try_(TEST_expect(search_lastMax(equal_seq, cmp_u_ord$(i32)) == 3));
} $unscoped(TEST_fn);

TEST_fn_("search: unit sequence any none and diff helpers define empty and boundary behavior" $scope) {
    var data = A_from$((i32){ 1, 2, 3, 2, 4, 5 $listed });
    var needle = A_from$((i32){ 2, 4 $listed });
    var any = A_from$((i32){ 4, 9 $listed });
    var none = A_from$((i32){ 1, 2, 3 $listed });
    var same_prefix = A_from$((i32){ 1, 2, 3, 9 $listed });
    var short_prefix = A_from$((i32){ 1, 2, 3, 2 $listed });
    let seq = u_anyS(A_ref(data)).as_const;

    try_(TEST_expect(unwrap_(search_firstUnit(seq, u_anyV(as$(i32)(2)), cmp_u_eql$(i32))) == 1));
    try_(TEST_expect(unwrap_(search_lastUnit(seq, u_anyV(as$(i32)(2)), cmp_u_eql$(i32))) == 3));
    try_(TEST_expect(isNone(search_firstUnit(seq, u_anyV(as$(i32)(9)), cmp_u_eql$(i32)))));
    try_(TEST_expect(unwrap_(search_firstSeq(seq, u_anyS(A_ref(needle)).as_const, cmp_u_eql$(i32))) == 3));
    try_(TEST_expect(unwrap_(search_lastSeq(seq, u_anyS(A_ref(needle)).as_const, cmp_u_eql$(i32))) == 3));
    try_(TEST_expect(unwrap_(search_firstAny(seq, u_anyS(A_ref(any)).as_const, cmp_u_eql$(i32))) == 4));
    try_(TEST_expect(unwrap_(search_lastAny(seq, u_anyS(A_ref(any)).as_const, cmp_u_eql$(i32))) == 4));
    try_(TEST_expect(unwrap_(search_firstNone(seq, u_anyS(A_ref(none)).as_const, cmp_u_eql$(i32))) == 4));
    try_(TEST_expect(unwrap_(search_lastNone(seq, u_anyS(A_ref(none)).as_const, cmp_u_eql$(i32))) == 5));
    try_(TEST_expect(unwrap_(search_firstDiff(seq, u_anyS(A_ref(same_prefix)).as_const, cmp_u_eql$(i32))) == 3));
    try_(TEST_expect(unwrap_(search_lastDiff(seq, u_anyS(A_ref(same_prefix)).as_const, cmp_u_eql$(i32))) == 3));
    try_(TEST_expect(unwrap_(search_firstDiff(seq, u_anyS(A_ref(short_prefix)).as_const, cmp_u_eql$(i32))) == 4));
    try_(TEST_expect(unwrap_(search_lastDiff(seq, u_anyS(A_ref(short_prefix)).as_const, cmp_u_eql$(i32))) == 4));
    try_(TEST_expect(isNone(search_firstDiff(seq, seq, cmp_u_eql$(i32)))));
} $unscoped(TEST_fn);

TEST_fn_("search: unit sequence helpers define empty sequence needle and set contracts" $scope) {
    var data = A_from$((i32){ 1, 2, 3 $listed });
    var missing_needle = A_from$((i32){ 3, 4 $listed });
    var longer_needle = A_from$((i32){ 1, 2, 3, 4 $listed });
    let seq = u_anyS(A_ref(data)).as_const;
    let empty_seq = u_prefixS(seq, 0);
    let empty_needle = u_prefixS(seq, 0);
    let empty_set = u_prefixS(seq, 0);

    try_(TEST_expect(isNone(search_firstUnit(empty_seq, u_anyV(as$(i32)(1)), cmp_u_eql$(i32)))));
    try_(TEST_expect(isNone(search_lastUnit(empty_seq, u_anyV(as$(i32)(1)), cmp_u_eql$(i32)))));

    try_(TEST_expect(unwrap_(search_firstSeq(seq, empty_needle, cmp_u_eql$(i32))) == 0));
    try_(TEST_expect(unwrap_(search_lastSeq(seq, empty_needle, cmp_u_eql$(i32))) == A_len(data)));
    try_(TEST_expect(unwrap_(search_firstSeq(empty_seq, empty_needle, cmp_u_eql$(i32))) == 0));
    try_(TEST_expect(unwrap_(search_lastSeq(empty_seq, empty_needle, cmp_u_eql$(i32))) == 0));
    try_(TEST_expect(isNone(search_firstSeq(seq, u_anyS(A_ref(missing_needle)).as_const, cmp_u_eql$(i32)))));
    try_(TEST_expect(isNone(search_lastSeq(seq, u_anyS(A_ref(missing_needle)).as_const, cmp_u_eql$(i32)))));
    try_(TEST_expect(isNone(search_firstSeq(seq, u_anyS(A_ref(longer_needle)).as_const, cmp_u_eql$(i32)))));
    try_(TEST_expect(isNone(search_lastSeq(seq, u_anyS(A_ref(longer_needle)).as_const, cmp_u_eql$(i32)))));

    try_(TEST_expect(isNone(search_firstAny(seq, empty_set, cmp_u_eql$(i32)))));
    try_(TEST_expect(isNone(search_lastAny(seq, empty_set, cmp_u_eql$(i32)))));
    try_(TEST_expect(unwrap_(search_firstNone(seq, empty_set, cmp_u_eql$(i32))) == 0));
    try_(TEST_expect(unwrap_(search_lastNone(seq, empty_set, cmp_u_eql$(i32))) == A_len(data) - 1));
    try_(TEST_expect(isNone(search_firstNone(empty_seq, empty_set, cmp_u_eql$(i32)))));
    try_(TEST_expect(isNone(search_lastNone(empty_seq, empty_set, cmp_u_eql$(i32)))));

    try_(TEST_expect(unwrap_(search_firstDiff(seq, empty_seq, cmp_u_eql$(i32))) == 0));
    try_(TEST_expect(unwrap_(search_lastDiff(seq, empty_seq, cmp_u_eql$(i32))) == 0));
    try_(TEST_expect(isNone(search_firstDiff(empty_seq, empty_seq, cmp_u_eql$(i32)))));
    try_(TEST_expect(isNone(search_lastDiff(empty_seq, empty_seq, cmp_u_eql$(i32)))));
} $unscoped(TEST_fn);

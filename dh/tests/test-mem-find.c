#include "dh-main.h"
#include "dh/mem/common.h"

T_use$((u8)(
    mem_findFirstUnit,
    mem_findLastUnit,
    mem_findFirstSeq,
    mem_findLastSeq,
    mem_findFirstAny,
    mem_findLastAny,
    mem_findFirstNone,
    mem_findLastNone,
    mem_findFirstDiff,
    mem_findLastDiff,
    mem_countUnit,
    mem_containsUnit,
    mem_contains,
    mem_count,
    mem_containsAtLeastUnit,
    mem_containsAtLeast
));

TEST_fn_("mem: find - typed primitive searches" $scope) {
    let text = u8_l("abacadaba");

    let first_a = mem_findFirstUnit$u8(text, u8_c('a'));
    try_(TEST_expect(isSome(first_a)));
    try_(TEST_expect(unwrap_(first_a) == 0));

    let last_a = mem_findLastUnit$u8(text, u8_c('a'));
    try_(TEST_expect(isSome(last_a)));
    try_(TEST_expect(unwrap_(last_a) == 8));

    let first_aba = mem_findFirstSeq$u8(text, u8_l("aba"));
    try_(TEST_expect(isSome(first_aba)));
    try_(TEST_expect(unwrap_(first_aba) == 0));

    let last_aba = mem_findLastSeq$u8(text, u8_l("aba"));
    try_(TEST_expect(isSome(last_aba)));
    try_(TEST_expect(unwrap_(last_aba) == 6));

    let first_any = mem_findFirstAny$u8(text, u8_l("cd"));
    try_(TEST_expect(isSome(first_any)));
    try_(TEST_expect(unwrap_(first_any) == 3));

    let last_any = mem_findLastAny$u8(text, u8_l("cd"));
    try_(TEST_expect(isSome(last_any)));
    try_(TEST_expect(unwrap_(last_any) == 5));

    let first_none = mem_findFirstNone$u8(text, u8_l("ab"));
    try_(TEST_expect(isSome(first_none)));
    try_(TEST_expect(unwrap_(first_none) == 3));

    let last_none = mem_findLastNone$u8(text, u8_l("ab"));
    try_(TEST_expect(isSome(last_none)));
    try_(TEST_expect(unwrap_(last_none) == 5));

    let first_diff = mem_findFirstDiff$u8(u8_l("abc"), u8_l("abd"));
    try_(TEST_expect(isSome(first_diff)));
    try_(TEST_expect(unwrap_(first_diff) == 2));

    let last_diff = mem_findLastDiff$u8(u8_l("abc"), u8_l("zbc"));
    try_(TEST_expect(isSome(last_diff)));
    try_(TEST_expect(unwrap_(last_diff) == 0));

    try_(TEST_expect(isNone(mem_findFirstUnit$u8(text, u8_c('z')))));
    try_(TEST_expect(isNone(mem_findFirstSeq$u8(text, u8_l("zz")))));
} $unscoped(TEST_fn)

TEST_fn_("mem: find - direct bytes searches" $scope) {
    let text = u8_l("ab--cd,ef");

    let first_seq = mem_findFirstSeqBytes(text, u8_l("--"));
    try_(TEST_expect(isSome(first_seq)));
    try_(TEST_expect(unwrap_(first_seq) == 2));

    let last_any = mem_findLastAnyBytes(text, u8_l("-,"));
    try_(TEST_expect(isSome(last_any)));
    try_(TEST_expect(unwrap_(last_any) == 6));

    let first_none = mem_findFirstNoneBytes(text, u8_l("ab-"));
    try_(TEST_expect(isSome(first_none)));
    try_(TEST_expect(unwrap_(first_none) == 4));

    let first_diff = mem_findFirstDiffBytes(u8_l("same"), u8_l("save"));
    try_(TEST_expect(isSome(first_diff)));
    try_(TEST_expect(unwrap_(first_diff) == 2));

    try_(TEST_expect(isNone(mem_findFirstSeqBytes(text, u8_l("zz")))));
    try_(TEST_expect(isNone(mem_findLastDiffBytes(u8_l("same"), u8_l("same")))));
} $unscoped(TEST_fn)

TEST_fn_("mem: find - count and contains typed searches" $scope) {
    try_(TEST_expect(mem_count$u8(u8_l(""), u8_l("h")) == 0));
    try_(TEST_expect(mem_count$u8(u8_l("h"), u8_l("h")) == 1));
    try_(TEST_expect(mem_count$u8(u8_l("hh"), u8_l("h")) == 2));
    try_(TEST_expect(mem_count$u8(u8_l("world!"), u8_l("hello")) == 0));
    try_(TEST_expect(mem_count$u8(u8_l("hello world!"), u8_l("hello")) == 1));
    try_(TEST_expect(mem_count$u8(u8_l("   abcabc   abc"), u8_l("abc")) == 3));
    try_(TEST_expect(mem_count$u8(u8_l("radaradar"), u8_l("radar")) == 1));
    try_(TEST_expect(mem_count$u8(u8_l("aaa"), u8_l("aa")) == 1));

    try_(TEST_expect(mem_countUnit$u8(u8_l(""), u8_c('h')) == 0));
    try_(TEST_expect(mem_countUnit$u8(u8_l("h"), u8_c('h')) == 1));
    try_(TEST_expect(mem_countUnit$u8(u8_l("hh"), u8_c('h')) == 2));
    try_(TEST_expect(mem_countUnit$u8(u8_l("ahhb"), u8_c('h')) == 2));
    try_(TEST_expect(mem_countUnit$u8(u8_l("   abcabc   abc"), u8_c('b')) == 3));

    try_(TEST_expect(mem_containsUnit$u8(u8_l("abc"), u8_c('b'))));
    try_(TEST_expect(!mem_containsUnit$u8(u8_l("abc"), u8_c('z'))));
    try_(TEST_expect(!mem_containsUnit$u8(u8_l(""), u8_c('z'))));
    try_(TEST_expect(mem_contains$u8(u8_l("abc"), u8_l("bc"))));
    try_(TEST_expect(!mem_contains$u8(u8_l("abc"), u8_l("bd"))));
    try_(TEST_expect(mem_contains$u8(u8_l("abc"), u8_l(""))));

    try_(TEST_expect(mem_containsAtLeast$u8(u8_l("aa"), u8_l("a"), 0)));
    try_(TEST_expect(mem_containsAtLeast$u8(u8_l(""), u8_l("a"), 0)));
    try_(TEST_expect(mem_containsAtLeast$u8(u8_l(""), u8_l("aa"), 0)));
    try_(TEST_expect(mem_containsAtLeast$u8(u8_l("aa"), u8_l("a"), 1)));
    try_(TEST_expect(mem_containsAtLeast$u8(u8_l("aa"), u8_l("a"), 2)));
    try_(TEST_expect(!mem_containsAtLeast$u8(u8_l("aa"), u8_l("a"), 3)));
    try_(TEST_expect(mem_containsAtLeast$u8(u8_l("radaradar"), u8_l("radar"), 1)));
    try_(TEST_expect(!mem_containsAtLeast$u8(u8_l("radaradar"), u8_l("radar"), 2)));
    try_(TEST_expect(mem_containsAtLeast$u8(u8_l("radarradaradarradar"), u8_l("radar"), 3)));
    try_(TEST_expect(!mem_containsAtLeast$u8(u8_l("radarradaradarradar"), u8_l("radar"), 4)));

    try_(TEST_expect(mem_containsAtLeastUnit$u8(u8_l(""), u8_c('d'), 0)));
    try_(TEST_expect(mem_containsAtLeastUnit$u8(u8_l("adadda"), u8_c('d'), 3)));
    try_(TEST_expect(!mem_containsAtLeastUnit$u8(u8_l("adadda"), u8_c('d'), 4)));
} $unscoped(TEST_fn)

TEST_fn_("mem: find - count and contains direct bytes searches" $scope) {
    try_(TEST_expect(mem_countBytes(u8_l("foo bar"), u8_l("o bar")) == 1));
    try_(TEST_expect(mem_countBytes(u8_l("aaa"), u8_l("aa")) == 1));
    try_(TEST_expect(mem_countUnitBytes(u8_l("mississippi"), u8_c('s')) == 4));

    try_(TEST_expect(mem_containsUnitBytes(u8_l("abc"), u8_c('b'))));
    try_(TEST_expect(!mem_containsUnitBytes(u8_l("abc"), u8_c('z'))));
    try_(TEST_expect(!mem_containsUnitBytes(u8_l(""), u8_c('z'))));
    try_(TEST_expect(mem_containsBytes(u8_l("abc"), u8_l("bc"))));
    try_(TEST_expect(!mem_containsBytes(u8_l("abc"), u8_l("bd"))));
    try_(TEST_expect(mem_containsBytes(u8_l("abc"), u8_l(""))));

    try_(TEST_expect(mem_containsAtLeastBytes(u8_l(""), u8_l("a"), 0)));
    try_(TEST_expect(mem_containsAtLeastBytes(u8_l(""), u8_l("aa"), 0)));
    try_(TEST_expect(mem_containsAtLeastBytes(u8_l("   radar      radar   "), u8_l("radar"), 2)));
    try_(TEST_expect(!mem_containsAtLeastBytes(u8_l("   radar      radar   "), u8_l("radar"), 3)));
    try_(TEST_expect(mem_containsAtLeastUnitBytes(u8_l(""), u8_c('a'), 0)));
    try_(TEST_expect(mem_containsAtLeastUnitBytes(u8_l("banana"), u8_c('a'), 3)));
    try_(TEST_expect(!mem_containsAtLeastUnitBytes(u8_l("banana"), u8_c('a'), 4)));
} $unscoped(TEST_fn)

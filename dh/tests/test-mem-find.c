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
    mem_findLastDiff
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

#include "dh-main.h"
#include "dh/mem/common.h"

T_use_E$($set(mem_E)(S$u8));
T_use$((u8)(
    mem_bytesAsS,
    mem_bytesAsMutS,
    mem_WindowIter,
    mem_window,
    mem_WindowIter_reset,
    mem_WindowIter_next
));
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
    mem_trimStart,
    mem_trimEnd,
    mem_trim,
    mem_cat,
    mem_catWithin,
    mem_join,
    mem_joinWithin
));
T_use$((u8)(
    mem_Cutted,
    mem_Cutted_before,
    mem_Cutted_after,
    mem_cutFirstUnit,
    mem_cutLastUnit,
    mem_cutFirstSeq,
    mem_cutLastSeq,
    mem_cutFirstAny,
    mem_cutLastAny
));
T_use$((u8)(
    mem_Delim,
    mem_SplitIter,
    mem_splitUnit,
    mem_splitSeq,
    mem_splitAny,
    mem_SplitIter_reset,
    mem_SplitIter_first,
    mem_SplitIter_next,
    mem_SplitIter_peek,
    mem_SplitIter_rest
));

TEST_fn_("mem find: primitive unit seq any none diff" $scope) {
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
} $unscoped(TEST_fn)

TEST_fn_("mem trim cut and within concat" $scope) {
    let padded = u8_l(" \tvalue \n");
    try_(TEST_expect(mem_eqlBytes(mem_trimStart$u8(padded, u8_l(" \t\n")), u8_l("value \n"))));
    try_(TEST_expect(mem_eqlBytes(mem_trimEnd$u8(padded, u8_l(" \t\n")), u8_l(" \tvalue"))));
    try_(TEST_expect(mem_eqlBytes(mem_trim$u8(padded, u8_l(" \t\n")), u8_l("value"))));

    var cat_buf = u8_a("................");
    let cat = mem_catWithin$u8(u8_l("ab"), u8_l("cd"), A_ref$((S$u8)(cat_buf)));
    try_(TEST_expect(mem_eqlBytes(cat.as_const, u8_l("abcd"))));

    var checked_cat_buf = u8_a("................");
    let checked_cat = try_(mem_cat$u8(u8_l("xy"), u8_l("z"), A_ref$((S$u8)(checked_cat_buf))));
    try_(TEST_expect(mem_eqlBytes(checked_cat.as_const, u8_l("xyz"))));

    var join_buf = u8_a("................");
    let joined = mem_joinWithin$u8(u8_l(","), u8_l("ab"), u8_l("cd"), A_ref$((S$u8)(join_buf)));
    try_(TEST_expect(mem_eqlBytes(joined.as_const, u8_l("ab,cd"))));

    var checked_buf = u8_a("................");
    let checked = try_(mem_join$u8(u8_l(":"), u8_l("ab"), u8_l("cd"), A_ref$((S$u8)(checked_buf))));
    try_(TEST_expect(mem_eqlBytes(checked.as_const, u8_l("ab:cd"))));
} $unscoped(TEST_fn)

TEST_fn_("mem templates: bytesAsS window and cut instantiate" $scope) {
    let typed_const = mem_bytesAsS$u8(u8_l("xy"));
    try_(TEST_expect(mem_eqlBytes(typed_const, u8_l("xy"))));

    var bytes = u8_a("pq");
    let typed_mut = mem_bytesAsMutS$u8(A_ref$((S$u8)(bytes)));
    try_(TEST_expect(mem_eqlBytes(typed_mut.as_const, u8_l("pq"))));

    var windows = mem_window$u8(u8_l("abcd"), 2, 1);
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_WindowIter_next$u8(&windows)), u8_l("ab"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_WindowIter_next$u8(&windows)), u8_l("bc"))));
    mem_WindowIter_reset$u8(&windows);
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_WindowIter_next$u8(&windows)), u8_l("ab"))));

    let cut_seq = unwrap_(mem_cutFirstSeq$u8(u8_l("ab--cd"), u8_l("--")));
    try_(TEST_expect(mem_eqlBytes(mem_Cutted_before$u8(cut_seq), u8_l("ab"))));
    try_(TEST_expect(mem_eqlBytes(mem_Cutted_after$u8(cut_seq), u8_l("cd"))));

    try_(TEST_expect(isSome(mem_cutFirstUnit$u8(u8_l("a,b"), u8_c(',')))));
    try_(TEST_expect(isSome(mem_cutLastUnit$u8(u8_l("a,b,c"), u8_c(',')))));
    try_(TEST_expect(isSome(mem_cutLastSeq$u8(u8_l("ab--cd--ef"), u8_l("--")))));
    try_(TEST_expect(isSome(mem_cutFirstAny$u8(u8_l("a,b;c"), u8_l(",;")))));
    try_(TEST_expect(isSome(mem_cutLastAny$u8(u8_l("a,b;c"), u8_l(",;")))));
} $unscoped(TEST_fn)

TEST_fn_("mem split: unit seq any preserves empty fields" $scope) {
    var unit = mem_splitUnit$u8(u8_l("x,y"), u8_c(','));
    try_(TEST_expect(mem_eqlBytes(mem_SplitIter_first$u8(&unit), u8_l("x"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitIter_next$u8(&unit)), u8_l("y"))));
    try_(TEST_expect(isNone(mem_SplitIter_next$u8(&unit))));

    var seq = mem_splitSeq$u8(u8_l("ab--cd--"), u8_l("--"));
    try_(TEST_expect(mem_eqlBytes(mem_SplitIter_first$u8(&seq), u8_l("ab"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitIter_next$u8(&seq)), u8_l("cd"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitIter_next$u8(&seq)), u8_l(""))));
    try_(TEST_expect(isNone(mem_SplitIter_next$u8(&seq))));

    mem_SplitIter_reset$u8(&seq);
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitIter_peek$u8(&seq)), u8_l("ab"))));
    try_(TEST_expect(mem_eqlBytes(mem_SplitIter_rest$u8(&seq), u8_l("ab--cd--"))));

    var any = mem_splitAny$u8(u8_l("a,b;c"), u8_l(",;"));
    try_(TEST_expect(mem_eqlBytes(mem_SplitIter_first$u8(&any), u8_l("a"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitIter_next$u8(&any)), u8_l("b"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitIter_next$u8(&any)), u8_l("c"))));
    try_(TEST_expect(isNone(mem_SplitIter_next$u8(&any))));
} $unscoped(TEST_fn)

#include "dh-main.h"
#include "dh/mem/common.h"

T_use$((u8)(
    mem_bytesAsS,
    mem_bytesAsMutS,
    mem_WindowIter,
    mem_window,
    mem_WindowIter_reset,
    mem_WindowIter_next
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
T_use$((u8)(
    mem_TokzBwdIter,
    mem_tokzBwdUnit,
    mem_tokzBwdSeq,
    mem_tokzBwdAny,
    mem_TokzBwdIter_reset,
    mem_TokzBwdIter_next,
    mem_TokzBwdIter_peek,
    mem_TokzBwdIter_rest
));
T_use$((u8)(
    mem_SplitBwdIter,
    mem_splitBwdUnit,
    mem_splitBwdSeq,
    mem_splitBwdAny,
    mem_SplitBwdIter_reset,
    mem_SplitBwdIter_first,
    mem_SplitBwdIter_next,
    mem_SplitBwdIter_peek,
    mem_SplitBwdIter_rest
));

TEST_fn_("mem: iter - typed wrappers instantiate over bytes" $scope) {
    let typed_const = mem_bytesAsS$u8(u8_l("xy"));
    try_(TEST_expect(mem_eqlBytes(typed_const, u8_l("xy"))));

    var bytes = u8_a("pq");
    let typed_mut = mem_bytesAsMutS$u8(A_ref$((S$u8)(bytes)));
    try_(TEST_expect(mem_eqlBytes(typed_mut.as_const, u8_l("pq"))));
} $unscoped(TEST_fn)

TEST_fn_("mem: iter - windows advance and reset" $scope) {
    var windows = mem_window$u8(u8_l("abcd"), 2, 1);

    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_WindowIter_next$u8(&windows)), u8_l("ab"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_WindowIter_next$u8(&windows)), u8_l("bc"))));
    mem_WindowIter_reset$u8(&windows);
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_WindowIter_next$u8(&windows)), u8_l("ab"))));

    var byte_windows = mem_windowBytes(u8_l("abcd"), 2, 2);
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_WindowIter_nextBytes(&byte_windows)), u8_l("ab"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_WindowIter_nextBytes(&byte_windows)), u8_l("cd"))));
    try_(TEST_expect(isNone(mem_WindowIter_nextBytes(&byte_windows))));
} $unscoped(TEST_fn)

TEST_fn_("mem: iter - cuts expose before and after views" $scope) {
    let cut_seq = unwrap_(mem_cutFirstSeq$u8(u8_l("ab--cd"), u8_l("--")));
    try_(TEST_expect(mem_eqlBytes(mem_Cutted_before$u8(cut_seq), u8_l("ab"))));
    try_(TEST_expect(mem_eqlBytes(mem_Cutted_after$u8(cut_seq), u8_l("cd"))));

    try_(TEST_expect(isSome(mem_cutFirstUnit$u8(u8_l("a,b"), u8_c(',')))));
    try_(TEST_expect(isSome(mem_cutLastUnit$u8(u8_l("a,b,c"), u8_c(',')))));
    try_(TEST_expect(isSome(mem_cutLastSeq$u8(u8_l("ab--cd--ef"), u8_l("--")))));
    try_(TEST_expect(isSome(mem_cutFirstAny$u8(u8_l("a,b;c"), u8_l(",;")))));
    try_(TEST_expect(isSome(mem_cutLastAny$u8(u8_l("a,b;c"), u8_l(",;")))));

    let byte_cut = unwrap_(mem_cutFirstSeqBytes(u8_l("ab--cd,ef"), u8_l("--")));
    try_(TEST_expect(mem_eqlBytes(mem_Cutted_beforeBytes(byte_cut), u8_l("ab"))));
    try_(TEST_expect(mem_eqlBytes(mem_Cutted_afterBytes(byte_cut), u8_l("cd,ef"))));
} $unscoped(TEST_fn)

TEST_fn_("mem: iter - tokenizer skips delimiters" $scope) {
    var tokz = mem_tokzAnyBytes(u8_l(",a;;b,"), u8_l(",;"));

    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzIter_nextBytes(&tokz)), u8_l("a"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzIter_nextBytes(&tokz)), u8_l("b"))));
    try_(TEST_expect(isNone(mem_TokzIter_nextBytes(&tokz))));
} $unscoped(TEST_fn)

TEST_fn_("mem: iter - split preserves empty fields" $scope) {
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

    var byte_split = mem_splitSeqBytes(u8_l("a--b--"), u8_l("--"));
    try_(TEST_expect(mem_eqlBytes(mem_SplitIter_firstBytes(&byte_split), u8_l("a"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitIter_nextBytes(&byte_split)), u8_l("b"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitIter_nextBytes(&byte_split)), u8_l(""))));
    try_(TEST_expect(isNone(mem_SplitIter_nextBytes(&byte_split))));
} $unscoped(TEST_fn)

TEST_fn_("mem: iter - backward tokenizer skips delimiters" $scope) {
    var tokz = mem_tokzBwdAny$u8(u8_l(",a;;b,"), u8_l(",;"));

    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzBwdIter_next$u8(&tokz)), u8_l("b"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzBwdIter_peek$u8(&tokz)), u8_l("a"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzBwdIter_next$u8(&tokz)), u8_l("a"))));
    try_(TEST_expect(isNone(mem_TokzBwdIter_next$u8(&tokz))));

    mem_TokzBwdIter_reset$u8(&tokz);
    try_(TEST_expect(mem_eqlBytes(mem_TokzBwdIter_rest$u8(&tokz), u8_l(",a;;b"))));

    var unit_tokz = mem_tokzBwdUnit$u8(u8_l("ab cd "), u8_c(' '));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzBwdIter_next$u8(&unit_tokz)), u8_l("cd"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzBwdIter_next$u8(&unit_tokz)), u8_l("ab"))));

    var seq_tokz = mem_tokzBwdSeq$u8(u8_l("<>ab<>cd<><>"), u8_l("<>"));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzBwdIter_next$u8(&seq_tokz)), u8_l("cd"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzBwdIter_next$u8(&seq_tokz)), u8_l("ab"))));

    var byte_tokz = mem_tokzBwdAnyBytes(u8_l(",a;;b,"), u8_l(",;"));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzBwdIter_nextBytes(&byte_tokz)), u8_l("b"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_TokzBwdIter_nextBytes(&byte_tokz)), u8_l("a"))));
    try_(TEST_expect(isNone(mem_TokzBwdIter_nextBytes(&byte_tokz))));
} $unscoped(TEST_fn)

TEST_fn_("mem: iter - backward split preserves empty fields" $scope) {
    var unit = mem_splitBwdUnit$u8(u8_l("abc|def||ghi"), u8_c('|'));
    try_(TEST_expect(mem_eqlBytes(mem_SplitBwdIter_first$u8(&unit), u8_l("ghi"))));
    try_(TEST_expect(mem_eqlBytes(mem_SplitBwdIter_rest$u8(&unit), u8_l("abc|def|"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_next$u8(&unit)), u8_l(""))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_next$u8(&unit)), u8_l("def"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_next$u8(&unit)), u8_l("abc"))));
    try_(TEST_expect(isNone(mem_SplitBwdIter_next$u8(&unit))));

    var seq = mem_splitBwdSeq$u8(u8_l("a, b ,, c, d, e"), u8_l(", "));
    try_(TEST_expect(mem_eqlBytes(mem_SplitBwdIter_first$u8(&seq), u8_l("e"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_peek$u8(&seq)), u8_l("d"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_next$u8(&seq)), u8_l("d"))));
    mem_SplitBwdIter_reset$u8(&seq);
    try_(TEST_expect(mem_eqlBytes(mem_SplitBwdIter_first$u8(&seq), u8_l("e"))));

    var any = mem_splitBwdAny$u8(u8_l("a,b, c d e"), u8_l(", "));
    try_(TEST_expect(mem_eqlBytes(mem_SplitBwdIter_first$u8(&any), u8_l("e"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_next$u8(&any)), u8_l("d"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_next$u8(&any)), u8_l("c"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_next$u8(&any)), u8_l(""))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_next$u8(&any)), u8_l("b"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_next$u8(&any)), u8_l("a"))));
    try_(TEST_expect(isNone(mem_SplitBwdIter_next$u8(&any))));

    var byte_any = mem_splitBwdAnyBytes(u8_l("a,b"), u8_l(","));
    try_(TEST_expect(mem_eqlBytes(mem_SplitBwdIter_firstBytes(&byte_any), u8_l("b"))));
    try_(TEST_expect(mem_eqlBytes(unwrap_(mem_SplitBwdIter_nextBytes(&byte_any)), u8_l("a"))));
    try_(TEST_expect(isNone(mem_SplitBwdIter_nextBytes(&byte_any))));
} $unscoped(TEST_fn)

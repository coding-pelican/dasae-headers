#include "dh-main.h"
#include "dh/mem/common.h"
#include "dh/mem/seq.h"

T_use$((u8)(
    mem_seq_set0Range,
    mem_seq_copyRange,
    mem_seq_moveRange,
    mem_seq_swapAt
));

TEST_fn_("mem/seq: set0Range changes only the requested interior range" $scope) {
    var bytes = u8_a("abcdef");
    let seq = A_ref$((S$u8)(bytes));

    mem_seq_set0Range$u8(seq, 2, 2);

    try_(TEST_expect(*S_at((seq)[0]) == u8_c('a')));
    try_(TEST_expect(*S_at((seq)[1]) == u8_c('b')));
    try_(TEST_expect(*S_at((seq)[2]) == 0));
    try_(TEST_expect(*S_at((seq)[3]) == 0));
    try_(TEST_expect(*S_at((seq)[4]) == u8_c('e')));
    try_(TEST_expect(*S_at((seq)[5]) == u8_c('f')));
} $unscoped(TEST_fn);

TEST_fn_("mem/seq: copyRange selects matching source and destination ranges" $scope) {
    var dst = u8_a("abcdef");
    let dst_seq = A_ref$((S$u8)(dst));

    mem_seq_copyRange$u8(dst_seq, 2, u8_l("WXYZ"), 1, 2);

    try_(TEST_expect(mem_eqlBytes(dst_seq.as_const, u8_l("abXYef"))));
} $unscoped(TEST_fn);

TEST_fn_("mem/seq: moveRange supports either overlapping direction and swapAt exchanges values" $scope) {
    var bytes = u8_a("abcdef");
    let seq = A_ref$((S$u8)(bytes));

    mem_seq_moveRange$u8(seq, 2, 0, 4);
    try_(TEST_expect(mem_eqlBytes(seq.as_const, u8_l("ababcd"))));

    mem_seq_moveRange$u8(seq, 0, 2, 4);
    try_(TEST_expect(mem_eqlBytes(seq.as_const, u8_l("abcdcd"))));

    mem_seq_swapAt$u8(seq, 0, 5);
    try_(TEST_expect(mem_eqlBytes(seq.as_const, u8_l("dbcdca"))));
} $unscoped(TEST_fn);

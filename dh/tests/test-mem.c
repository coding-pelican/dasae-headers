#include "dh-main.h"
#include "dh/mem/common.h"

T_use$((u8)(
    mem_copy,
    mem_move,
    mem_set,
    mem_set0,
    mem_eql
));

TEST_fn_("mem/common: basic - copies only source length and preserves tail" $scope) {
    var dst = u8_a("............");
    let copied = mem_copyBytes(A_ref$((S$u8)(dst)), u8_l("hello"));

    try_(TEST_expect(copied.len == A_len(dst)));
    try_(TEST_expect(mem_eqlBytes(S_prefix((copied.as_const)(5)), u8_l("hello"))));
    try_(TEST_expect(mem_eqlBytes(S_suffix((copied.as_const)(5)), u8_l("......."))));
} $unscoped(TEST_fn)

TEST_fn_("mem/common: basic - typed copy returns destination view" $scope) {
    var dst = u8_a("........");
    let copied = mem_copy$u8(A_ref$((S$u8)(dst)), u8_l("abc"));

    try_(TEST_expect(copied.ptr == A_ptr(dst)));
    try_(TEST_expect(copied.len == A_len(dst)));
    try_(TEST_expect(mem_eqlBytes(S_prefix((copied.as_const)(3)), u8_l("abc"))));
} $unscoped(TEST_fn)

TEST_fn_("mem/common: basic - move handles overlapping ranges" $scope) {
    var data = u8_a("abcdef");
    let buf = A_ref$((S$u8)(data));

    mem_moveBytes(S_slice((buf)$r(2, 6)), S_slice((buf)$r(0, 4)).as_const);
    try_(TEST_expect(mem_eqlBytes(buf.as_const, u8_l("ababcd"))));

    mem_move$u8(S_slice((buf)$r(0, 4)), S_slice((buf)$r(2, 6)).as_const);
    try_(TEST_expect(mem_eqlBytes(buf.as_const, u8_l("abcdcd"))));
} $unscoped(TEST_fn)

TEST_fn_("mem/common: basic - set and zero fill exact views" $scope) {
    var data = u8_a("abcdef");
    let buf = A_ref$((S$u8)(data));

    mem_setBytes(S_slice((buf)$r(1, 4)), u8_c('x'));
    try_(TEST_expect(mem_eqlBytes(buf.as_const, u8_l("axxxef"))));

    mem_set$u8(S_slice((buf)$r(4, 6)), u8_c('y'));
    try_(TEST_expect(mem_eqlBytes(buf.as_const, u8_l("axxxyy"))));

    mem_set0$u8(S_slice((buf)$r(1, 3)));
    try_(TEST_expect(*S_at((buf)[0]) == u8_c('a')));
    try_(TEST_expect(*S_at((buf)[1]) == 0));
    try_(TEST_expect(*S_at((buf)[2]) == 0));
    try_(TEST_expect(*S_at((buf)[3]) == u8_c('x')));
} $unscoped(TEST_fn)

TEST_fn_("mem/common: basic - equality compares complete views" $scope) {
    try_(TEST_expect(mem_eqlBytes(u8_l("abc"), u8_l("abc"))));
    try_(TEST_expect(!mem_eqlBytes(u8_l("abc"), u8_l("ab"))));
    try_(TEST_expect(!mem_eqlBytes(u8_l("abc"), u8_l("abd"))));

    try_(TEST_expect(mem_eql$u8(u8_l("same"), u8_l("same"))));
    try_(TEST_expect(!mem_eql$u8(u8_l("same"), u8_l("save"))));
} $unscoped(TEST_fn)

TEST_fn_("mem/common: basic - log2 alignment helpers match runtime alignment math" $scope) {
    for_(($r(0, $incl(6)))(step)) {
        let log2_align = intCast$((u8)(step));
        let align = log2ToAlign(log2_align);

        try_(TEST_expect(align == mem_log2ToAlign(log2_align)));
        try_(TEST_expect(alignToLog2(align) == log2_align));

        for_(($r(0, 64))(addr)) {
            try_(TEST_expect(isAlignedLog2(addr, log2_align) == mem_isAlignedLog2(addr, log2_align)));
            try_(TEST_expect(alignFwdLog2(addr, log2_align) == mem_alignFwdLog2(addr, log2_align)));
            try_(TEST_expect(alignBwdLog2(addr, log2_align) == mem_alignBwdLog2(addr, log2_align)));
        } $end(for);
    } $end(for);
} $unscoped(TEST_fn)

TEST_fn_("mem/common: basic - endian read and write use fixed byte arrays" $scope) {
    let le16 = S_deref$((const mem_ReadLE16Buf)(u8_l("\064\022")));
    let le32 = S_deref$((const mem_ReadLE32Buf)(u8_l("\170\126\064\022")));
    let le64 = S_deref$((const mem_ReadLE64Buf)(u8_l("\360\336\274\232\170\126\064\022")));
    try_(TEST_expect(mem_readLE16(le16) == u16_(0x1234u)));
    try_(TEST_expect(mem_readLE32(le32) == u32_(0x12345678u)));
    try_(TEST_expect(mem_readLE64(le64) == u64_(0x123456789abcdef0ull)));

    let be16 = S_deref$((const mem_ReadBE16Buf)(u8_l("\022\064")));
    let be32 = S_deref$((const mem_ReadBE32Buf)(u8_l("\022\064\126\170")));
    let be64 = S_deref$((const mem_ReadBE64Buf)(u8_l("\022\064\126\170\232\274\336\360")));
    try_(TEST_expect(mem_readBE16(be16) == u16_(0x1234u)));
    try_(TEST_expect(mem_readBE32(be32) == u32_(0x12345678u)));
    try_(TEST_expect(mem_readBE64(be64) == u64_(0x123456789abcdef0ull)));

    let written_le32 = mem_writeLE32(u32_(0x12345678u));
    let written_le64 = mem_writeLE64(u64_(0x123456789abcdef0ull));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(written_le32)), u8_l("\170\126\064\022"))));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(written_le64)), u8_l("\360\336\274\232\170\126\064\022"))));

    let written_be32 = mem_writeBE32(u32_(0x12345678u));
    let written_be64 = mem_writeBE64(u64_(0x123456789abcdef0ull));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(written_be32)), u8_l("\022\064\126\170"))));
    try_(TEST_expect(mem_eqlBytes(A_ref$((S_const$u8)(written_be64)), u8_l("\022\064\126\170\232\274\336\360"))));
} $unscoped(TEST_fn)

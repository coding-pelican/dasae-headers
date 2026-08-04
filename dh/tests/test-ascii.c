#include "dh-main.h"
#include "dh/ascii.h"
#include "dh/mem/common.h"

TEST_fn_("ascii: scalar classification and case conversion" $scope) {
    try_(TEST_expect(ascii_isASCII(u8_c('A'))));
    try_(TEST_expect(!ascii_isASCII(u8_(0x80))));
    try_(TEST_expect(ascii_isUpper(u8_c('Z'))));
    try_(TEST_expect(ascii_isLower(u8_c('z'))));
    try_(TEST_expect(ascii_isAlpha(u8_c('q'))));
    try_(TEST_expect(ascii_isDigit(u8_c('7'))));
    try_(TEST_expect(ascii_isAlNum(u8_c('9'))));
    try_(TEST_expect(ascii_isHex(u8_c('f'))));
    try_(TEST_expect(ascii_isCtrl(as$(u8)(ascii_ctrl_Code_lf))));
    try_(TEST_expect(ascii_isWhitespace(u8_c('\t'))));

    try_(TEST_expect(ascii_toUpper(u8_c('a')) == u8_c('A')));
    try_(TEST_expect(ascii_toUpper(u8_c('A')) == u8_c('A')));
    try_(TEST_expect(ascii_toLower(u8_c('A')) == u8_c('a')));
    try_(TEST_expect(ascii_toLower(u8_c('a')) == u8_c('a')));
    try_(TEST_expect(ascii_toggleCase(u8_c('A')) == u8_c('a')));
    try_(TEST_expect(ascii_toggleCase(u8_c('a')) == u8_c('A')));
    try_(TEST_expect(ascii_toggleCase(u8_c('!')) == u8_c('!')));
} $unscoped(TEST_fn);

TEST_fn_("ascii: named constants and digit conversions define byte-level ASCII behavior" $scope) {
    try_(TEST_expect(ascii_nul_byte == u8_(0x00)));
    try_(TEST_expect(ascii_bel_byte == u8_(0x07)));
    try_(TEST_expect(ascii_bs_byte == u8_(0x08)));
    try_(TEST_expect(ascii_ht_byte == u8_(0x09)));
    try_(TEST_expect(ascii_lf_byte == u8_(0x0A)));
    try_(TEST_expect(ascii_vt_byte == u8_(0x0B)));
    try_(TEST_expect(ascii_ff_byte == u8_(0x0C)));
    try_(TEST_expect(ascii_cr_byte == u8_(0x0D)));
    try_(TEST_expect(ascii_sp_byte == u8_(0x20)));

    try_(TEST_expect(ascii_digitToInt(u8_c('0')) == 0));
    try_(TEST_expect(ascii_digitToInt(u8_c('9')) == 9));
    try_(TEST_expect(ascii_intToDigit(0) == u8_c('0')));
    try_(TEST_expect(ascii_intToDigit(9) == u8_c('9')));
    try_(TEST_expect(ascii_intFromDigit(u8_c('7')) == 7));
    try_(TEST_expect(ascii_digitFromInt(4) == u8_c('4')));
} $unscoped(TEST_fn);

TEST_fn_("ascii: in-place and within-buffer case conversion" $scope) {
    T_use_A$(32, u8);
    var_(mem, A$32$u8) = A_init({ 'h', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd' });
    let text = S_prefix((A_ref$((S$u8)(mem)))(11));
    let upper = ascii_toUppers(text);
    try_(TEST_expect(mem_eqlBytes(upper.as_const, u8_l("HELLO WORLD"))));

    let lower = ascii_toLowers(text);
    try_(TEST_expect(mem_eqlBytes(lower.as_const, u8_l("hello world"))));

    let toggled = ascii_toggleCases(text);
    try_(TEST_expect(mem_eqlBytes(toggled.as_const, u8_l("HELLO WORLD"))));

    var_(out_mem, A$32$u8) $undefined;
    let out = A_ref$((S$u8)(out_mem));
    try_(TEST_expect(mem_eqlBytes(ascii_makeUppers(out, u8_l("Test123!")).as_const, u8_l("TEST123!"))));
    try_(TEST_expect(mem_eqlBytes(ascii_makeLowers(out, u8_l("Test123!")).as_const, u8_l("test123!"))));
    try_(TEST_expect(mem_eqlBytes(ascii_makeToggledCases(out, u8_l("Test123!")).as_const, u8_l("tEST123!"))));
} $unscoped(TEST_fn);

TEST_fn_("ascii: case-insensitive search" $scope) {
    let haystack = u8_l("Hello Hello World");

    try_(TEST_expect(unwrap_(ascii_idxOfIgnoreCase(haystack, u8_l("HELLO"))) == 0));
    try_(TEST_expect(unwrap_(ascii_idxFirstOfIgnoreCase(haystack, u8_l("hello"), 6)) == 6));
    try_(TEST_expect(unwrap_(ascii_idxLastOfIgnoreCase(haystack, u8_l("hello"), haystack.len - 1)) == 6));
    try_(TEST_expect(isNone(ascii_idxOfIgnoreCase(haystack, u8_l("missing")))));
    try_(TEST_expect(isNone(ascii_idxFirstOfIgnoreCase(haystack, u8_l("hello"), 12))));
    try_(TEST_expect(unwrap_(ascii_idxOfIgnoreCase(haystack, u8_l(""))) == 0));
    try_(TEST_expect(unwrap_(ascii_idxFirstOfIgnoreCase(haystack, u8_l(""), 5)) == 5));
    try_(TEST_expect(unwrap_(ascii_idxLastOfIgnoreCase(haystack, u8_l(""), 7)) == 7));
    try_(TEST_expect(unwrap_(ascii_idxLastOfIgnoreCase(haystack, u8_l("HELLO"), 10)) == 6));
    try_(TEST_expect(isNone(ascii_idxLastOfIgnoreCase(haystack, u8_l("HELLO"), 3))));
} $unscoped(TEST_fn);

TEST_fn_("ascii: prefix suffix equality and ordering" $scope) {
    let text = u8_l("Hello World");

    try_(TEST_expect(ascii_startsWithIgnoreCase(text, u8_l("hello"))));
    try_(TEST_expect(!ascii_startsWithIgnoreCase(text, u8_l("world"))));
    try_(TEST_expect(ascii_endsWithIgnoreCase(text, u8_l("WORLD"))));
    try_(TEST_expect(!ascii_endsWithIgnoreCase(text, u8_l("hello"))));

    try_(TEST_expect(ascii_eqlSenseCase(u8_l("hello"), u8_l("hello"))));
    try_(TEST_expect(!ascii_eqlSenseCase(u8_l("Hello"), u8_l("hello"))));
    try_(TEST_expect(ascii_eqlIgnoreCase(u8_l("Hello"), u8_l("hello"))));
    try_(TEST_expect(ascii_eql(u8_l("HELLO"), u8_l("hello"), true)));
    try_(TEST_expect(!ascii_eql(u8_l("HELLO"), u8_l("hello"), false)));

    try_(TEST_expect(ascii_ordSenseCase(u8_l("Apple"), u8_l("apple")) == cmp_Ord_lt));
    try_(TEST_expect(ascii_ordIgnoreCase(u8_l("Apple"), u8_l("apple")) == cmp_Ord_eq));
    try_(TEST_expect(ascii_ord(u8_l("banana"), u8_l("apple"), true) == cmp_Ord_gt));
} $unscoped(TEST_fn);

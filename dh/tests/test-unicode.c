#include "dh-main.h"
#include "dh/unicode.h"
#include "dh/mem/common.h"

$static fn_((test_unicode_u16_eql(S_const$u16 lhs, S_const$u16 rhs))(bool)) {
    if (lhs.len != rhs.len) return false;
    for_(($s(lhs), $s(rhs))(l, r)) {
        if (*l != *r) return false;
    } $end(for);
    return true;
};

TEST_fn_("unicode: strict UTF-8 to UTF-16 conversion computes length and rejects invalid UTF-8" $scope) {
    let bytes = u8_l("A\xC2\xA2\xE2\x82\xAC\xF0\x9F\x98\x80");
    var expected = A_from$((u16){ 0x0041, 0x00A2, 0x20AC, 0xD83D, 0xDE00 $listed });
    var_(out_mem, A$$(5, u16)) $undefined;
    let out = A_ref$((S$u16)(out_mem));

    try_(TEST_expect(try_(unicode_utf8ToUTF16Len(bytes)) == 5));
    let converted = try_(unicode_utf8ToUTF16(bytes, out));
    try_(TEST_expect(test_unicode_u16_eql(converted.as_const, A_ref$((S_const$u16)(expected)))));

    var_(small_mem, A$$(4, u16)) $undefined;
    try_(TEST_expect(isErr(unicode_utf8ToUTF16(bytes, A_ref$((S$u16)(small_mem))))));
    try_(TEST_expect(isErr(unicode_utf8ToUTF16Len(u8_l("\xED\xA0\x80")))));
} $unscoped(TEST_fn);

TEST_fn_("unicode: strict UTF-16 to UTF-8 conversion computes length and rejects unpaired surrogates" $scope) {
    var units = A_from$((u16){ 0x0041, 0x00A2, 0x20AC, 0xD83D, 0xDE00 $listed });
    var invalid = A_from$((u16){ 0x0041, 0xD800 $listed });
    var_(out_mem, A$$(10, u8)) $undefined;
    let out = A_ref$((S$u8)(out_mem));

    let units_s = A_ref$((S_const$u16)(units));
    try_(TEST_expect(try_(unicode_utf16ToUTF8Len(units_s)) == 10));
    let converted = try_(unicode_utf16ToUTF8(units_s, out));
    try_(TEST_expect(mem_eqlBytes(converted.as_const, u8_l("A\xC2\xA2\xE2\x82\xAC\xF0\x9F\x98\x80"))));

    var_(small_mem, A$$(9, u8)) $undefined;
    try_(TEST_expect(isErr(unicode_utf16ToUTF8(units_s, A_ref$((S$u8)(small_mem))))));
    try_(TEST_expect(isErr(unicode_utf16ToUTF8Len(A_ref$((S_const$u16)(invalid))))));
} $unscoped(TEST_fn);

TEST_fn_("unicode: WTF conversions preserve unpaired surrogate values" $scope) {
    let wtf8 = u8_l("A\xED\xA0\x80\xF0\x9F\x98\x80");
    var expected_wtf16 = A_from$((u16){ 0x0041, 0xD800, 0xD83D, 0xDE00 $listed });
    var_(out16_mem, A$$(4, u16)) $undefined;
    var_(out8_mem, A$$(8, u8)) $undefined;

    try_(TEST_expect(unicode_wtf8ToWTF16Len(wtf8) == 4));
    let wtf16 = try_(unicode_wtf8ToWTF16(wtf8, A_ref$((S$u16)(out16_mem))));
    try_(TEST_expect(test_unicode_u16_eql(wtf16.as_const, A_ref$((S_const$u16)(expected_wtf16)))));

    try_(TEST_expect(unicode_wtf16ToWTF8Len(wtf16.as_const) == wtf8.len));
    let encoded = try_(unicode_wtf16ToWTF8(wtf16.as_const, A_ref$((S$u8)(out8_mem))));
    try_(TEST_expect(mem_eqlBytes(encoded.as_const, wtf8)));
} $unscoped(TEST_fn);

TEST_fn_("unicode: UTF-8 and WTF-8 view casts encode strict subset boundary" $scope) {
    let strict = try_(utf8_view(u8_l("A\xF0\x9F\x98\x80")));
    let wtf = unicode_utf8AsWTF8(strict);
    try_(TEST_expect(wtf.bytes.ptr == strict.bytes.ptr));
    try_(TEST_expect(wtf.bytes.len == strict.bytes.len));
    try_(TEST_expect((try_(unicode_wtf8AsUTF8(wtf))).bytes.ptr == strict.bytes.ptr));

    let permissive = wtf8_viewUnchkd(u8_l("\xED\xA0\x80"));
    try_(TEST_expect(isErr(unicode_wtf8AsUTF8(permissive))));
} $unscoped(TEST_fn);

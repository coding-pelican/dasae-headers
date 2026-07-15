#include "dh-main.h"
#include "dh/wtf8.h"
#include "dh/wtf16.h"
#include "dh/mem/common.h"

TEST_fn_("wtf8: surrogate halves are valid codepoints and round-trip as bytes" $scope) {
    var_(out_mem, A$$(4, u8)) $undefined;
    let out = A_ref$((S$u8)(out_mem));

    let encoded = try_(wtf8_encode(0xD800, out));
    try_(TEST_expect(mem_eqlBytes(encoded.as_const, u8_l("\xED\xA0\x80"))));
    try_(TEST_expect(try_(wtf8_decode(encoded.as_const)) == 0xD800));
    try_(TEST_expect(wtf8_validate(encoded.as_const)));
    try_(TEST_expect(wtf8_count(encoded.as_const) == 1));
} $unscoped(TEST_fn);

TEST_fn_("wtf8: invalid byte structure is still rejected" $scope) {
    let missing_continuation = eval_(bool $scope)(catch_((wtf8_decode(u8_l("\xE2\x28\xA1")))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$wtf8_ExpectedContinuation().as_any)));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(missing_continuation));

    try_(TEST_expect(!wtf8_validate(u8_l("\x80"))));
    try_(TEST_expect(!wtf8_validate(u8_l("\xF4\x90\x80\x80"))));
    try_(TEST_expect(wtf8_count(u8_l("A\x80\xED\xA0\x80")) == 2));
} $unscoped(TEST_fn);

TEST_fn_("wtf8: view and iterator preserve surrogate bytes as a logical item" $scope) {
    let bytes = u8_l("A\xED\xA0\x80\xF0\x9F\x98\x80");
    let view = try_(wtf8_view(bytes));
    var it = wtf8_iter(view);

    try_(TEST_expect(unwrap_(wtf8_Iter_next(&it)) == 0x41));
    try_(TEST_expect(mem_eqlBytes(unwrap_(wtf8_Iter_nextBytes(&it)), u8_l("\xED\xA0\x80"))));
    try_(TEST_expect(unwrap_(wtf8_Iter_next(&it)) == 0x1F600));
    try_(TEST_expect(isNone(wtf8_Iter_next(&it))));
} $unscoped(TEST_fn);

TEST_fn_("wtf16: iterator preserves unpaired surrogates and combines valid pairs" $scope) {
    var units = A_from$((u16){ 0x0041, 0xD800, 0xD83D, 0xDE00, 0xDC00 $listed });
    var it = wtf16_iter(A_ref$((S_const$u16)(units)));

    try_(TEST_expect(unwrap_(wtf16_Iter_next(&it)) == 0x41));
    try_(TEST_expect(unwrap_(wtf16_Iter_next(&it)) == 0xD800));
    try_(TEST_expect(unwrap_(wtf16_Iter_next(&it)) == 0x1F600));
    try_(TEST_expect(unwrap_(wtf16_Iter_next(&it)) == 0xDC00));
    try_(TEST_expect(isNone(wtf16_Iter_next(&it))));
} $unscoped(TEST_fn);

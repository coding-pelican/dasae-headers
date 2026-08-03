#include "dh-main.h"
#include "dh/wtf8.h"
#include "dh/mem/common.h"

TEST_fn_("wtf8: surrogate halves are valid codepoints and round-trip" $scope) {
    var_(out_mem, A$$(4, u8)) $undefined;
    let encoded = try_(wtf8_encode(0xD800, A_ref$((S$u8)(out_mem))));
    try_(TEST_expect(mem_eqlBytes(
        encoded.as_const, u8_l("\xED\xA0\x80")
    )));
    try_(TEST_expect(try_(wtf8_decode(encoded.as_const)) == 0xD800));
    try_(TEST_expect(wtf8_validate(encoded.as_const)));
    try_(TEST_expect(wtf8_count(encoded.as_const) == 1));
} $unscoped(TEST_fn);

TEST_fn_("wtf8: invalid byte structure is rejected" $scope) {
    let rejected = eval_(bool $scope)(catch_((wtf8_decode(
        u8_l("\xE2\x28\xA1")
    ))(err, {
        try_(TEST_expect(E_eql(
            err.as_any, E_cause$wtf8_ExpectedContinuation().as_any
        )));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(rejected));
    try_(TEST_expect(!wtf8_validate(u8_l("\x80"))));
    try_(TEST_expect(!wtf8_validate(u8_l("\xF4\x90\x80\x80"))));
    try_(TEST_expect(wtf8_count(u8_l("A\x80\xED\xA0\x80")) == 2));
} $unscoped(TEST_fn);

TEST_fn_("wtf8: iterator preserves surrogate bytes as one item" $scope) {
    let view = try_(wtf8_view(u8_l("A\xED\xA0\x80\xF0\x9F\x98\x80")));
    var it = wtf8_iter(view);

    try_(TEST_expect(unwrap_(wtf8_Iter_next(&it)) == 0x41));
    try_(TEST_expect(mem_eqlBytes(
        unwrap_(wtf8_Iter_nextBytes(&it)), u8_l("\xED\xA0\x80")
    )));
    try_(TEST_expect(unwrap_(wtf8_Iter_next(&it)) == 0x1F600));
    try_(TEST_expect(isNone(wtf8_Iter_next(&it))));
} $unscoped(TEST_fn);

#include "dh-main.h"
#include "dh/utf8.h"
#include "dh/mem/common.h"

TEST_fn_("utf8: sequence length boundaries define scalar encoding size" $scope) {
    try_(TEST_expect(try_(utf8_codepointSeqLen(0x0000)) == utf8_SeqLen_1));
    try_(TEST_expect(try_(utf8_codepointSeqLen(0x007F)) == utf8_SeqLen_1));
    try_(TEST_expect(try_(utf8_codepointSeqLen(0x0080)) == utf8_SeqLen_2));
    try_(TEST_expect(try_(utf8_codepointSeqLen(0x07FF)) == utf8_SeqLen_2));
    try_(TEST_expect(try_(utf8_codepointSeqLen(0x0800)) == utf8_SeqLen_3));
    try_(TEST_expect(try_(utf8_codepointSeqLen(0xFFFF)) == utf8_SeqLen_3));
    try_(TEST_expect(try_(utf8_codepointSeqLen(0x10000)) == utf8_SeqLen_4));
    try_(TEST_expect(try_(utf8_codepointSeqLen(0x10FFFF)) == utf8_SeqLen_4));

    let too_large = eval_(bool $scope)(catch_((utf8_codepointSeqLen(0x110000))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$utf8_TooLargeCodepoint().as_any)));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(too_large));

    try_(TEST_expect(try_(utf8_byteSeqLen(u8_(0x00))) == utf8_SeqLen_1));
    try_(TEST_expect(try_(utf8_byteSeqLen(u8_(0xC2))) == utf8_SeqLen_2));
    try_(TEST_expect(try_(utf8_byteSeqLen(u8_(0xE0))) == utf8_SeqLen_3));
    try_(TEST_expect(try_(utf8_byteSeqLen(u8_(0xF0))) == utf8_SeqLen_4));
    try_(TEST_expect(isErr(utf8_byteSeqLen(u8_(0x80)))));
    try_(TEST_expect(isErr(utf8_byteSeqLen(u8_(0xFF)))));
} $unscoped(TEST_fn);

TEST_fn_("utf8: encode and decode accept scalar boundaries and reject surrogate halves" $scope) {
    var_(out_mem, A$$(4, u8)) $undefined;
    let out = A_ref$((S$u8)(out_mem));

    let dollar = try_(utf8_encode(0x24, out));
    try_(TEST_expect(mem_eqlBytes(dollar.as_const, u8_l("$"))));

    let cent = try_(utf8_encode(0xA2, out));
    try_(TEST_expect(mem_eqlBytes(cent.as_const, u8_l("\xC2\xA2"))));

    let euro = try_(utf8_encode(0x20AC, out));
    try_(TEST_expect(mem_eqlBytes(euro.as_const, u8_l("\xE2\x82\xAC"))));

    let face = try_(utf8_encode(0x1F600, out));
    try_(TEST_expect(mem_eqlBytes(face.as_const, u8_l("\xF0\x9F\x98\x80"))));

    try_(TEST_expect(try_(utf8_decode(u8_l("$"))) == 0x24));
    try_(TEST_expect(try_(utf8_decode(u8_l("\xC2\xA2"))) == 0xA2));
    try_(TEST_expect(try_(utf8_decode(u8_l("\xE2\x82\xAC"))) == 0x20AC));
    try_(TEST_expect(try_(utf8_decode(u8_l("\xF0\x9F\x98\x80"))) == 0x1F600));

    let surrogate_encode = eval_(bool $scope)(catch_((utf8_encode(0xD800, out))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$utf8_EncodesSurrogateHalf().as_any)));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(surrogate_encode));

    let surrogate_decode = eval_(bool $scope)(catch_((utf8_decode(u8_l("\xED\xA0\x80")))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$utf8_EncodesSurrogateHalf().as_any)));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(surrogate_decode));
} $unscoped(TEST_fn);

TEST_fn_("utf8: invalid byte sequences are classified and validation rejects them" $scope) {
    let overlong = eval_(bool $scope)(catch_((utf8_decode(u8_l("\xC0\x80")))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$utf8_OverlongEncoding().as_any)));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(overlong));

    let missing_continuation = eval_(bool $scope)(catch_((utf8_decode(u8_l("\xE2\x28\xA1")))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$utf8_ExpectedContinuation().as_any)));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(missing_continuation));

    let too_large = eval_(bool $scope)(catch_((utf8_decode(u8_l("\xF4\x90\x80\x80")))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$utf8_TooLargeCodepoint().as_any)));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(too_large));

    try_(TEST_expect(utf8_validate(u8_l("A\xC2\xA2\xE2\x82\xAC\xF0\x9F\x98\x80"))));
    try_(TEST_expect(!utf8_validate(u8_l("\x80"))));
    try_(TEST_expect(!utf8_validate(u8_l("\xE2\x82"))));
    try_(TEST_expect(!utf8_validate(u8_l("\xED\xA0\x80"))));
    try_(TEST_expect(utf8_count(u8_l("A\x80\xC2\xA2")) == 2));
} $unscoped(TEST_fn);

TEST_fn_("utf8: view and iterator expose codepoints and byte slices without consuming peeks" $scope) {
    let bytes = u8_l("A\xC2\xA2\xE2\x82\xAC\xF0\x9F\x98\x80");
    let view = try_(utf8_view(bytes));
    var it = utf8_iter(view);

    try_(TEST_expect(unwrap_(utf8_Iter_peek(&it)) == 0x41));
    try_(TEST_expect(unwrap_(utf8_Iter_peekAt(&it, 1)) == 0xA2));
    try_(TEST_expect(mem_eqlBytes(unwrap_(utf8_Iter_peekAtBytes(&it, 2)), u8_l("\xE2\x82\xAC"))));

    try_(TEST_expect(unwrap_(utf8_Iter_next(&it)) == 0x41));
    try_(TEST_expect(mem_eqlBytes(unwrap_(utf8_Iter_nextBytes(&it)), u8_l("\xC2\xA2"))));
    try_(TEST_expect(unwrap_(utf8_Iter_next(&it)) == 0x20AC));
    try_(TEST_expect(unwrap_(utf8_Iter_next(&it)) == 0x1F600));
    try_(TEST_expect(isNone(utf8_Iter_next(&it))));

    try_(TEST_expect(isErr(utf8_view(u8_l("\xED\xA0\x80")))));
} $unscoped(TEST_fn);

#include "dh-main.h"
#include "dh/utf16.h"

$static fn_((test_utf16_eql(S_const$u16 lhs, S_const$u16 rhs))(bool)) {
    if (lhs.len != rhs.len) return false;
    for_(($s(lhs), $s(rhs))(l, r)) {
        if (*l != *r) return false;
    } $end(for);
    return true;
};

TEST_fn_("utf16: surrogate classification and sequence length define strict starts" $scope) {
    try_(TEST_expect(!utf16_isSurrogate(0xD7FF)));
    try_(TEST_expect(utf16_isSurrogate(0xD800)));
    try_(TEST_expect(utf16_isSurrogate(0xDFFF)));
    try_(TEST_expect(!utf16_isSurrogate(0xE000)));
    try_(TEST_expect(utf16_isHighSurrogate(0xD800)));
    try_(TEST_expect(utf16_isHighSurrogate(0xDBFF)));
    try_(TEST_expect(utf16_isLowSurrogate(0xDC00)));
    try_(TEST_expect(utf16_isLowSurrogate(0xDFFF)));

    try_(TEST_expect(try_(utf16_codeunitSeqLen(0x0041)) == utf16_SeqLen_1));
    try_(TEST_expect(try_(utf16_codeunitSeqLen(0xD800)) == utf16_SeqLen_2));
    try_(TEST_expect(isErr(utf16_codeunitSeqLen(0xDC00))));
} $unscoped(TEST_fn);

TEST_fn_("utf16: encode and decode accept BMP and supplementary scalar values" $scope) {
    var_(out_mem, A$$(2, u16)) $undefined;
    let out = A_ref$((S$u16)(out_mem));
    var expected_bmp = A_from$((u16){ 0x20AC $listed });
    var expected_face = A_from$((u16){ 0xD83D, 0xDE00 $listed });

    let bmp = try_(utf16_encode(0x20AC, out));
    try_(TEST_expect(test_utf16_eql(bmp.as_const, A_ref$((S_const$u16)(expected_bmp)))));
    try_(TEST_expect(try_(utf16_decode(bmp.as_const)) == 0x20AC));

    let face = try_(utf16_encode(0x1F600, out));
    try_(TEST_expect(test_utf16_eql(face.as_const, A_ref$((S_const$u16)(expected_face)))));
    try_(TEST_expect(try_(utf16_decode(face.as_const)) == 0x1F600));
    try_(TEST_expect(try_(utf16_decodeSurrogatePair(0xD83D, 0xDE00)) == 0x1F600));
} $unscoped(TEST_fn);

TEST_fn_("utf16: strict decode rejects dangling and misplaced surrogates" $scope) {
    var dangling = A_from$((u16){ 0xD83D $listed });
    var low_first = A_from$((u16){ 0xDE00 $listed });
    var wrong_second = A_from$((u16){ 0xD83D, 0x0041 $listed });

    let dangling_seen = eval_(bool $scope)(catch_((utf16_decode(A_ref$((S_const$u16)(dangling))))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$utf16_DanglingSurrogateHalf().as_any)));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(dangling_seen));

    let low_seen = eval_(bool $scope)(catch_((utf16_decode(A_ref$((S_const$u16)(low_first))))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$utf16_UnexpectedSecondSurrogateHalf().as_any)));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(low_seen));

    let wrong_seen = eval_(bool $scope)(catch_((utf16_decode(A_ref$((S_const$u16)(wrong_second))))(err, {
        try_(TEST_expect(E_eql(err.as_any, E_cause$utf16_ExpectedSecondSurrogateHalf().as_any)));
        $break_(true);
    }))) eval_(else)({ $break_(false); }) $unscoped(eval);
    try_(TEST_expect(wrong_seen));

    try_(TEST_expect(!utf16_validate(A_ref$((S_const$u16)(dangling)))));
    try_(TEST_expect(!utf16_validate(A_ref$((S_const$u16)(low_first)))));
    try_(TEST_expect(!utf16_validate(A_ref$((S_const$u16)(wrong_second)))));
} $unscoped(TEST_fn);

TEST_fn_("utf16: iterator returns strict scalar values and errors at invalid boundaries" $scope) {
    var units = A_from$((u16){ 0x0041, 0xD83D, 0xDE00, 0x20AC $listed });
    var it = utf16_iter(A_ref$((S_const$u16)(units)));

    try_(TEST_expect(unwrap_(try_(utf16_Iter_next(&it))) == 0x41));
    try_(TEST_expect(unwrap_(try_(utf16_Iter_next(&it))) == 0x1F600));
    try_(TEST_expect(unwrap_(try_(utf16_Iter_next(&it))) == 0x20AC));
    try_(TEST_expect(isNone(try_(utf16_Iter_next(&it)))));

    var invalid = A_from$((u16){ 0x0041, 0xDC00 $listed });
    var invalid_it = utf16_iter(A_ref$((S_const$u16)(invalid)));
    try_(TEST_expect(unwrap_(try_(utf16_Iter_next(&invalid_it))) == 0x41));
    try_(TEST_expect(isErr(utf16_Iter_next(&invalid_it))));
} $unscoped(TEST_fn);

#include "dh/utf16.h"
#include "dh/utf8.h"

$attr($inline_always)
$static fn_((utf16__makeContinuationByte(u8 payload))(u8)) {
    return prim_maskHi_static$((u8)(1)) | (payload & prim_maskLo_static$((u8)(6)));
};
$attr($must_check)
$static fn_((utf16__encodeCodepoint(u32 codepoint, utf8_SeqLen seq_len, S$u8 out))(utf16_Err$S$u8));
$attr($must_check)
$static fn_((utf16__toUTF8(S_const$u16 utf16, usize required_len, S$u8 out_utf8))(utf16_Err$S$u8));

fn_((utf16_codeunitSeqLen(u16 first_codeunit))(utf16_Err$utf16_SeqLen) $scope) {
    if (utf16_isHighSurrogate(first_codeunit)) { return_ok(utf16_SeqLen_2); }
    if (utf16_isLowSurrogate(first_codeunit)) { return_err(utf16_Err_InvalidStartCodeUnit()); }
    return_ok(utf16_SeqLen_1);
} $unscoped_(fn);

fn_((utf16_codepointSeqLen(u32 codepoint))(utf16_Err$utf8_SeqLen) $scope) {
    return_ok(try_(utf8_codepointSeqLen(codepoint)));
} $unscoped_(fn);

fn_((utf16__encodeCodepoint(u32 codepoint, utf8_SeqLen seq_len, S$u8 out))(utf16_Err$S$u8) $scope) {
    claim_assert(seq_len <= out.len);
    switch (seq_len) {
    case utf8_SeqLen_1:
        *S_at((out)[0]) = intCast$((u8)codepoint);
        break;
    case utf8_SeqLen_2:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_2 | (codepoint >> 6)));
        *S_at((out)[1]) = utf16__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    case utf8_SeqLen_3:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_3 | (codepoint >> 12)));
        *S_at((out)[1]) = utf16__makeContinuationByte(intCast$((u8)(codepoint >> 6)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[2]) = utf16__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    case utf8_SeqLen_4:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_4 | (codepoint >> 18)));
        *S_at((out)[1]) = utf16__makeContinuationByte(intCast$((u8)(codepoint >> 12)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[2]) = utf16__makeContinuationByte(intCast$((u8)(codepoint >> 6)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[3]) = utf16__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    }
    return_ok(S_slice((out)$r(0, seq_len)));
} $unscoped_(fn);

fn_((utf16_decodeSurrogatePair(u16 high_codeunit, u16 low_codeunit))(utf16_Err$u32) $scope) {
    if (!utf16_isHighSurrogate(high_codeunit)) { return_err(utf16_Err_InvalidStartCodeUnit()); }
    if (!utf16_isLowSurrogate(low_codeunit)) { return_err(utf16_Err_ExpectedSecondSurrogateHalf()); }
    let high_half = (u32)(high_codeunit & prim_maskLo_static$((u16)(10)));
    let low_half = (u32)(low_codeunit & prim_maskLo_static$((u16)(10)));
    return_ok(0x10000 + (high_half << 10) | low_half);
} $unscoped_(fn);

fn_((utf16_count(S_const$u16 codeunits))(usize) $scope) {
    var_(count, usize) = 0;
    var_(idx, usize) = 0;
    while (idx < codeunits.len) {
        if (utf16_isHighSurrogate(*S_at((codeunits)[idx]))) {
            idx += 2;
        } else {
            idx += 1;
        }
        count += 1;
    }
    return_(count);
} $unscoped_(fn);

fn_((utf16_iter(S_const$u16 codeunits))(utf16_Iter) $scope) {
    return_({ .bytes = codeunits, .idx = 0 });
} $unscoped_(fn);

fn_((utf16_Iter_next(utf16_Iter* self))(utf16_Err$O$u32) $scope) {
    if (self->idx >= self->bytes.len) { return_ok(none()); }
    let first = *S_at((self->bytes)[self->idx]);
    if (utf16_isHighSurrogate(first)) {
        if (self->idx + 1 >= self->bytes.len) { return_err(utf16_Err_DanglingSurrogateHalf()); }
        let second = *S_at((self->bytes)[self->idx + 1]);
        let codepoint = try_(utf16_decodeSurrogatePair(first, second));
        self->idx += 2;
        return_ok(some(codepoint));
    } else if (utf16_isLowSurrogate(first)) {
        return_err(utf16_Err_UnexpectedSecondSurrogateHalf());
    } else {
        self->idx += 1;
        return_ok(some((u32)first));
    }
} $unscoped_(fn);

fn_((utf16_calcUTF8Len(S_const$u16 utf16))(utf16_Err$usize) $scope) {
    var_(len8, usize) = 0;
    var it = utf16_iter(utf16);
    while_some((try_(utf16_Iter_next(&it))), codepoint) {
        let seq_len = try_(utf8_codepointSeqLen(codepoint));
        len8 += seq_len;
    }
    return_ok(len8);
} $unscoped_(fn);

fn_((utf16__toUTF8(S_const$u16 utf16, usize required_len, S$u8 out_utf8))(utf16_Err$S$u8) $scope) {
    claim_assert(required_len <= out_utf8.len);
    var_(out_idx, usize) = 0;
    var it = utf16_iter(utf16);
    while_some((try_(utf16_Iter_next(&it))), codepoint) {
        let seq_len = try_(utf8_codepointSeqLen(codepoint));
        let encoded = try_(utf16__encodeCodepoint(codepoint, seq_len, S_slice((out_utf8)$r(out_idx, out_idx + seq_len))));
        out_idx += encoded.len;
    }
    return_ok(S_slice((out_utf8)$r(0, out_idx)));
} $unscoped_(fn);

fn_((utf16_toUTF8(S_const$u16 utf16, S$u8 out_utf8))(E$S$u8) $scope) {
    let required_len = try_(utf16_calcUTF8Len(utf16));
    if (required_len > out_utf8.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok(try_(utf16__toUTF8(utf16, required_len, out_utf8)));
} $unscoped_(fn);

fn_((utf16_toUTF8Within(S_const$u16 utf16, S$u8 out_utf8))(utf16_Err$S$u8) $scope) {
    return_(utf16__toUTF8(utf16, try_(utf16_calcUTF8Len(utf16)), out_utf8));
} $unscoped_(fn);

fn_((utf16_toUTF8Alloc(S_const$u16 utf16, mem_Allocator gpa))(utf16_mem_Err$S$u8) $scope) {
    let required_len = try_(utf16_calcUTF8Len(utf16));
    let required_buf = u_castS$((S$u8)try_(mem_Allocator_alloc(gpa, typeInfo$(u8), required_len)));
    let utf8 = try_(utf16__toUTF8(utf16, required_len, required_buf));
    claim_assert(utf8.len == required_buf.len);
    return_ok(utf8);
} $unscoped_(fn);

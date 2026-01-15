#include "dh/utf16.h"
#include "dh/utf8.h"

fn_((utf16_codeunitSeqLen(u16 first_codeunit))(utf16_Err$utf16_SeqLen) $scope) {
    if (utf16_isHighSurrogate(first_codeunit)) { return_ok(utf16_SeqLen_2); }
    if (utf16_isLowSurrogate(first_codeunit)) { return_err(utf16_Err_InvalidStartCodeUnit()); }
    return_ok(utf16_SeqLen_1);
} $unscoped_(fn);

fn_((utf16_codepointSeqLen(u32 codepoint))(utf16_Err$utf8_SeqLen) $scope) {
    return_ok(try_(utf8_codepointSeqLen(codepoint)));
} $unscoped_(fn);

fn_((utf16_encode(u32 codepoint, S$u16 out))(E$S$u16) $scope) {
    if (utf16_isSurrogate(codepoint)) { return_err(utf16_Err_CodepointTooLarge()); }
    let required_len = (codepoint < 0x10000) ? as$(usize)(1) : as$(usize)(2);
    if (codepoint > 0x10FFFF) { return_err(utf16_Err_CodepointTooLarge()); }
    if (required_len > out.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok(try_(utf16_encodeWithin(codepoint, out)));
} $unscoped_(fn);

fn_((utf16_encodeWithin(u32 codepoint, S$u16 out))(utf16_Err$S$u16) $scope) {
    if (utf16_isSurrogate(codepoint)) { return_err(utf16_Err_CodepointTooLarge()); }
    if (codepoint < 0x10000) {
        claim_assert(1 <= out.len);
        *S_at((out)[0]) = intCast$((u16)codepoint);
        return_ok(S_slice((out)$r(0, 1)));
    } else {
        if (codepoint > 0x10FFFF) { return_err(utf16_Err_CodepointTooLarge()); }
        claim_assert(2 <= out.len);
        let high = intCast$((u16)((codepoint - 0x10000) >> 10) + 0xD800);
        let low = intCast$((u16)(codepoint & 0x3FF) + 0xDC00);
        *S_at((out)[0]) = high;
        *S_at((out)[1]) = low;
        return_ok(S_slice((out)$r(0, 2)));
    }
} $unscoped_(fn);

fn_((utf16_decode(S_const$u16 codeunits))(utf16_Err$u32) $scope) {
    if (codeunits.len == 0) { return_err(utf16_Err_InvalidStartCodeUnit()); }
    let first = *S_at((codeunits)[0]);
    if (utf16_isHighSurrogate(first)) {
        if (codeunits.len < 2) { return_err(utf16_Err_DanglingSurrogateHalf()); }
        let second = *S_at((codeunits)[1]);
        return_(utf16_decodeSurrogatePair(first, second));
    } else if (utf16_isLowSurrogate(first)) {
        return_err(utf16_Err_UnexpectedSecondSurrogateHalf());
    } else {
        return_ok(as$(u32)(first));
    }
} $unscoped_(fn);

fn_((utf16_decodeSurrogatePair(u16 high_codeunit, u16 low_codeunit))(utf16_Err$u32) $scope) {
    if (!utf16_isHighSurrogate(high_codeunit)) { return_err(utf16_Err_InvalidStartCodeUnit()); }
    if (!utf16_isLowSurrogate(low_codeunit)) { return_err(utf16_Err_ExpectedSecondSurrogateHalf()); }
    let high_half = as$(u32)(high_codeunit & prim_maskLo_static$((u16)(10)));
    let low_half = as$(u32)(low_codeunit & prim_maskLo_static$((u16)(10)));
    return_ok(0x10000 + (high_half << 10) | low_half);
} $unscoped_(fn);

fn_((utf16_validate(S_const$u16 codeunits))(bool) $scope) {
    var_(idx, usize) = 0;
    while (idx < codeunits.len) {
        let rest = S_slice((codeunits)$r(idx, codeunits.len));
        let_ignore = catch_((utf16_decode(rest))($ignore, return_(false)));
        let seq_len = catch_((utf16_codeunitSeqLen(*S_at((rest)[0])))($ignore, claim_unreachable));
        idx += seq_len;
    }
    return_(true);
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

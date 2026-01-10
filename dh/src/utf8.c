#include "dh/utf8.h"
#include "dh/utf16.h"

$attr($must_check)
$static fn_((utf8__encode(u32 codepoint, utf8_SeqLen requested_len, S$u8 out))(utf8_Err$S$u8));
$attr($must_check)
$static fn_((utf8__decodeSeqLen(S_const$u8 bytes, utf8_SeqLen seq_len))(utf8_Err$u32));
$static fn_((utf8__decodeSeqLenValid(S_const$u8 bytes, utf8_SeqLen seq_len))(u32));
$static fn_((utf8__decode2Valid(utf8_Decode2Buf bytes))(u32));
$static fn_((utf8__decode3Valid(utf8_Decode3Buf bytes))(u32));
$static fn_((utf8__decode4Valid(utf8_Decode4Buf bytes))(u32));

$attr($inline_always)
$static fn_((utf8__isContinuation(u8 byte))(bool)) {
    return (byte & prim_maskHi_static$((u8)(2))) == prim_maskHi_static$((u8)(1));
};

$attr($inline_always)
$static fn_((utf8__continuationPayload(u8 byte))(u8)) {
    return byte & prim_maskLo_static$((u8)(6));
};

$attr($inline_always)
$static fn_((utf8__makeContinuationByte(u8 payload))(u8)) {
    return prim_maskHi_static$((u8)(1)) | (payload & prim_maskLo_static$((u8)(6)));
};

fn_((utf8_codepointSeqLen(u32 codepoint))(utf8_Err$utf8_SeqLen) $scope) {
    if (codepoint < 0x80) { return_ok(utf8_SeqLen_1); }
    if (codepoint < 0x800) { return_ok(utf8_SeqLen_2); }
    if (codepoint < 0x10000) { return_ok(utf8_SeqLen_3); }
    if (codepoint < 0x110000) { return_ok(utf8_SeqLen_4); }
    return_err(utf8_Err_TooLargeCodepoint());
} $unscoped_(fn);

fn_((utf8_byteSeqLen(u8 first_byte))(utf8_Err$utf8_SeqLen) $scope) {
    if ((first_byte & prim_maskHi_static$((u8)(1))) == 0) { return_ok(utf8_SeqLen_1); }
    if ((first_byte & prim_maskHi_static$((u8)(3))) == utf8_SeqByte_2) { return_ok(utf8_SeqLen_2); }
    if ((first_byte & prim_maskHi_static$((u8)(4))) == utf8_SeqByte_3) { return_ok(utf8_SeqLen_3); }
    if ((first_byte & prim_maskHi_static$((u8)(5))) == utf8_SeqByte_4) { return_ok(utf8_SeqLen_4); }
    return_err(utf8_Err_InvalidStartByte());
} $unscoped_(fn);

fn_((utf8_encode(u32 codepoint, S$u8 out))(E$S$u8) $scope) {
    let requested_len = try_(utf8_codepointSeqLen(codepoint));
    if (requested_len >= out.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok(try_(utf8__encode(codepoint, requested_len, out)));
} $unscoped_(fn);

fn_((utf8_encodeWithin(u32 codepoint, S$u8 out))(utf8_Err$S$u8) $scope) {
    return_(utf8__encode(codepoint, try_(utf8_codepointSeqLen(codepoint)), out));
} $unscoped_(fn);

fn_((utf8__encode(u32 codepoint, utf8_SeqLen requested_len, S$u8 out))(utf8_Err$S$u8) $scope) {
    claim_assert(requested_len <= out.len);
    switch (requested_len) {
    case utf8_SeqLen_1:
        *S_at((out)[0]) = intCast$((u8)codepoint);
        break;
    case utf8_SeqLen_2:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_2 | (codepoint >> 6)));
        *S_at((out)[1]) = utf8__makeContinuationByte(intCast$((u8)codepoint)) & prim_maskLo_static$((u8)(6));
        break;
    case utf8_SeqLen_3:
        if (utf16_isSurrogate(codepoint)) { return_err(utf8_Err_EncodesSurrogateHalf()); }
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_3 | (codepoint >> 12)));
        *S_at((out)[1]) = utf8__makeContinuationByte(intCast$((u8)(codepoint >> 6)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[2]) = utf8__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    case utf8_SeqLen_4:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_4 | (codepoint >> 18)));
        *S_at((out)[1]) = utf8__makeContinuationByte(intCast$((u8)(codepoint >> 12)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[2]) = utf8__makeContinuationByte(intCast$((u8)(codepoint >> 6)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[3]) = utf8__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    }
    return_ok(S_slice((out)$r(0, requested_len)));
} $unscoped_(fn);

fn_((utf8_decode(S_const$u8 bytes))(utf8_Err$u32) $scope) {
    if (bytes.len == 0) { return_err(utf8_Err_InvalidBytes()); }
    let seq_len = try_(utf8_byteSeqLen(*S_at((bytes)[0])));
    if (seq_len > bytes.len) { return_err(utf8_Err_InvalidBytes()); }
    return_(utf8__decodeSeqLen(bytes, seq_len));
} $unscoped_(fn);

fn_((utf8__decodeSeqLen(S_const$u8 bytes, utf8_SeqLen seq_len))(utf8_Err$u32) $scope) {
    claim_assert(bytes.len >= seq_len);
    switch (seq_len) {
    case utf8_SeqLen_1: return_ok(*S_at((bytes)[0]));
    case utf8_SeqLen_2: return utf8_decode2(S_deref$((const utf8_Decode2Buf)S_prefix((bytes)2)));
    case utf8_SeqLen_3: return utf8_decode3(S_deref$((const utf8_Decode3Buf)S_prefix((bytes)3)));
    case utf8_SeqLen_4: return utf8_decode4(S_deref$((const utf8_Decode4Buf)S_prefix((bytes)4)));
    }
} $unscoped_(fn);

fn_((utf8_decode2(utf8_Decode2Buf bytes))(utf8_Err$u32) $scope) {
    claim_assert((*A_at((bytes)[0]) & prim_maskHi_static$((u8)(3))) == utf8_SeqByte_2);
    var_(val, u32) = *A_at((bytes)[0]) & prim_maskLo_static$((u8)(5));
    if (!utf8__isContinuation(*A_at((bytes)[1]))) { return_err(utf8_Err_ExpectedContinuation()); }
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[1]));
    if (val < 0x80) { return_err(utf8_Err_OverlongEncoding()); }
    return_ok(val);
} $unscoped_(fn);

fn_((utf8_decode3(utf8_Decode3Buf bytes))(utf8_Err$u32) $scope) {
    claim_assert((*A_at((bytes)[0]) & prim_maskHi_static$((u8)(4))) == utf8_SeqByte_3);
    var_(val, u32) = *A_at((bytes)[0]) & prim_maskLo_static$((u8)(4));
    if (!utf8__isContinuation(*A_at((bytes)[1]))) { return_err(utf8_Err_ExpectedContinuation()); }
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[1]));
    if (!utf8__isContinuation(*A_at((bytes)[2]))) { return_err(utf8_Err_ExpectedContinuation()); }
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[2]));
    if (val < 0x800) { return_err(utf8_Err_OverlongEncoding()); }
    if (utf16_isSurrogate(val)) { return_err(utf8_Err_EncodesSurrogateHalf()); }
    return_ok(val);
} $unscoped_(fn);

fn_((utf8_decode4(utf8_Decode4Buf bytes))(utf8_Err$u32) $scope) {
    claim_assert((*A_at((bytes)[0]) & prim_maskHi_static$((u8)(5))) == utf8_SeqByte_4);
    var_(val, u32) = *A_at((bytes)[0]) & prim_maskLo_static$((u8)(3));
    if (!utf8__isContinuation(*A_at((bytes)[1]))) { return_err(utf8_Err_ExpectedContinuation()); }
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[1]));
    if (!utf8__isContinuation(*A_at((bytes)[2]))) { return_err(utf8_Err_ExpectedContinuation()); }
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[2]));
    if (!utf8__isContinuation(*A_at((bytes)[3]))) { return_err(utf8_Err_ExpectedContinuation()); }
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[3]));
    if (val < 0x10000) { return_err(utf8_Err_OverlongEncoding()); }
    if (val > 0x10FFFF) { return_err(utf8_Err_TooLargeCodepoint()); }
    return_ok(val);
} $unscoped_(fn);

fn_((utf8__decodeSeqLenValid(S_const$u8 bytes, utf8_SeqLen seq_len))(u32) $scope) {
    claim_assert(bytes.len >= seq_len);
    switch (seq_len) {
    case utf8_SeqLen_1: return *S_at((bytes)[0]);
    case utf8_SeqLen_2: return utf8__decode2Valid(S_deref$((const utf8_Decode2Buf)S_prefix((bytes)2)));
    case utf8_SeqLen_3: return utf8__decode3Valid(S_deref$((const utf8_Decode3Buf)S_prefix((bytes)3)));
    case utf8_SeqLen_4: return utf8__decode4Valid(S_deref$((const utf8_Decode4Buf)S_prefix((bytes)4)));
    }
} $unscoped_(fn);

fn_((utf8__decode2Valid(utf8_Decode2Buf bytes))(u32)) {
    claim_assert((*A_at((bytes)[0]) & prim_maskHi_static$((u8)(3))) == utf8_SeqByte_2);
    var_(val, u32) = *A_at((bytes)[0]) & prim_maskLo_static$((u8)(5));
    claim_assert(utf8__isContinuation(*A_at((bytes)[1])));
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[1]));
    claim_assert(val >= 0x80);
    return val;
};

fn_((utf8__decode3Valid(utf8_Decode3Buf bytes))(u32)) {
    claim_assert((*A_at((bytes)[0]) & prim_maskHi_static$((u8)(4))) == utf8_SeqByte_3);
    var_(val, u32) = *A_at((bytes)[0]) & prim_maskLo_static$((u8)(4));
    claim_assert(utf8__isContinuation(*A_at((bytes)[1])));
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[1]));
    claim_assert(utf8__isContinuation(*A_at((bytes)[2])));
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[2]));
    claim_assert(val >= 0x800);
    claim_assert(!utf16_isSurrogate(val));
    return val;
};

fn_((utf8__decode4Valid(utf8_Decode4Buf bytes))(u32)) {
    claim_assert((*A_at((bytes)[0]) & prim_maskHi_static$((u8)(5))) == utf8_SeqByte_4);
    var_(val, u32) = *A_at((bytes)[0]) & prim_maskLo_static$((u8)(3));
    claim_assert(utf8__isContinuation(*A_at((bytes)[1])));
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[1]));
    claim_assert(utf8__isContinuation(*A_at((bytes)[2])));
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[2]));
    claim_assert(utf8__isContinuation(*A_at((bytes)[3])));
    val = (val << 6) | utf8__continuationPayload(*A_at((bytes)[3]));
    claim_assert(val >= 0x10000);
    claim_assert(val <= 0x10FFFF);
    return val;
};

fn_((utf8_isValid(u32 codepoint))(bool)) {
    return codepoint <= 0x10FFFF && !utf16_isSurrogate(codepoint);
};

fn_((utf8_validate(S_const$u8 bytes))(bool) $scope) {
    var_(idx, usize) = 0;
    while (idx < bytes.len) {
        let rest = S_slice((bytes)$r(idx, bytes.len));
        let codepoint = catch_((utf8_decode(rest))($ignore, return_(false)));
        let seq_len = catch_((utf8_codepointSeqLen(codepoint))($ignore, claim_unreachable));
        idx += seq_len;
    }
    return_(true);
} $unscoped_(fn);

fn_((utf8_count(S_const$u8 bytes))(usize) $scope) {
    var_(count, usize) = 0;
    var_(idx, usize) = 0;
    while (idx < bytes.len) {
        let rest = S_slice((bytes)$r(idx, bytes.len));
        let codepoint = catch_((utf8_decode(rest))($ignore, {
            idx += 1;
            continue;
        }));
        let seq_len = catch_((utf8_codepointSeqLen(codepoint))($ignore, claim_unreachable));
        idx += seq_len;
        count += 1;
    }
    return_(count);
} $unscoped_(fn);

fn_((utf8_view(S_const$u8 bytes))(utf8_Err$utf8_View) $scope) {
    if (!utf8_validate(bytes)) { return_err(utf8_Err_InvalidBytes()); }
    return_ok({ .bytes = bytes });
} $unscoped_(fn);

fn_((utf8_viewUnchkd(S_const$u8 bytes))(utf8_View) $scope) {
    return_({ .bytes = bytes });
} $unscoped_(fn);

fn_((utf8_iter(utf8_View view))(utf8_Iter) $scope) {
    return_({ .bytes = view.bytes, .idx = 0 });
} $unscoped_(fn);

fn_((utf8_Iter_next(utf8_Iter* self))(O$u32) $scope) {
    let bytes = orelse_((utf8_Iter_nextBytes(self))(return_none()));
    return_some(utf8__decodeSeqLenValid(bytes, intCast$((utf8_SeqLen)bytes.len)));
} $unscoped_(fn);

fn_((utf8_Iter_nextBytes(utf8_Iter* self))(O$S_const$u8) $scope) {
    if (self->idx >= self->bytes.len) { return_none(); }
    let first = *S_at((self->bytes)[self->idx]);
    let seq_len = catch_((utf8_byteSeqLen(first))($ignore, claim_unreachable));
    self->idx += seq_len;
    return_some(S_slice((self->bytes)$r(self->idx - seq_len, self->idx)));
} $unscoped_(fn);

fn_((utf8_Iter_peek(utf8_Iter* self))(O$u32) $scope) {
    return_(utf8_Iter_next(&copy(*self)));
} $unscoped_(fn);

fn_((utf8_Iter_peekBytes(utf8_Iter* self))(O$S_const$u8) $scope) {
    return_(utf8_Iter_nextBytes(&copy(*self)));
} $unscoped_(fn);

fn_((utf8_Iter_peekAt(utf8_Iter* self, usize n))(O$u32) $scope) {
    var temp = *self;
    for_(($r(0, n))($ignore) {
        let_ignore = orelse_((utf8_Iter_next(&temp))(return_none()));
    });
    return_(utf8_Iter_next(&temp));
} $unscoped_(fn);

fn_((utf8_Iter_peekAtBytes(utf8_Iter* self, usize n))(O$S_const$u8) $scope) {
    var temp = *self;
    for_(($r(0, n))($ignore) {
        let_ignore = orelse_((utf8_Iter_nextBytes(&temp))(return_none()));
    });
    return_(utf8_Iter_nextBytes(&temp));
} $unscoped_(fn);

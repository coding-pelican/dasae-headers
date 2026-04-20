#include "dh/wtf8.h"
#include "dh/utf16.h"

$attr($inline_always)
$static fn_((wtf8__isContinuation(u8 byte))(bool)) {
    return (byte & int_maskHi_static$((u8)(2))) == int_maskHi_static$((u8)(1));
};

$attr($inline_always)
$static fn_((wtf8__continuationPayload(u8 byte))(u8)) {
    return byte & int_maskLo_static$((u8)(6));
};

$attr($inline_always)
$static fn_((wtf8__makeContinuationByte(u8 payload))(u8)) {
    return int_maskHi_static$((u8)(1)) | (payload & int_maskLo_static$((u8)(6)));
};

$attr($must_check)
$static fn_((wtf8__encode(u32 codepoint, utf8_SeqLen requested_len, S$u8 out))(wtf8_E$S$u8) $scope) {
    claim_assert(requested_len <= out.len);
    switch (requested_len) {
    case utf8_SeqLen_1:
        *S_at((out)[0]) = intCast$((u8)codepoint);
        break;
    case utf8_SeqLen_2:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_2 | (codepoint >> 6)));
        *S_at((out)[1]) = wtf8__makeContinuationByte(intCast$((u8)(codepoint & int_maskLo_static$((u8)(6)))));
        break;
    case utf8_SeqLen_3:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_3 | (codepoint >> 12)));
        *S_at((out)[1]) = wtf8__makeContinuationByte(intCast$((u8)((codepoint >> 6) & int_maskLo_static$((u8)(6)))));
        *S_at((out)[2]) = wtf8__makeContinuationByte(intCast$((u8)(codepoint & int_maskLo_static$((u8)(6)))));
        break;
    case utf8_SeqLen_4:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_4 | (codepoint >> 18)));
        *S_at((out)[1]) = wtf8__makeContinuationByte(intCast$((u8)((codepoint >> 12) & int_maskLo_static$((u8)(6)))));
        *S_at((out)[2]) = wtf8__makeContinuationByte(intCast$((u8)((codepoint >> 6) & int_maskLo_static$((u8)(6)))));
        *S_at((out)[3]) = wtf8__makeContinuationByte(intCast$((u8)(codepoint & int_maskLo_static$((u8)(6)))));
        break;
    }
    return_ok(S_slice((out)$r(0, requested_len)));
} $unscoped(fn);

fn_((wtf8_encode(u32 codepoint, S$u8 out))(E$S$u8) $scope) {
    let requested_len = try_(utf8_codepointSeqLen(codepoint));
    if (requested_len >= out.len) return_err(mem_E_OutOfMemory());
    return_ok(try_(wtf8__encode(codepoint, requested_len, out)));
} $unscoped(fn);

fn_((wtf8_encodeWithin(u32 codepoint, S$u8 out))(wtf8_E$S$u8) $scope) {
    return_(wtf8__encode(codepoint, try_(utf8_codepointSeqLen(codepoint)), out));
} $unscoped(fn);

$static fn_((wtf8__decode3(utf8_Decode3Buf bytes))(wtf8_E$u32) $scope) {
    claim_assert((*A_at((bytes)[0]) & int_maskHi_static$((u8)(4))) == utf8_SeqByte_3);
    var_(val, u32) = *A_at((bytes)[0]) & int_maskLo_static$((u8)(4));
    if (!wtf8__isContinuation(*A_at((bytes)[1]))) return_err(wtf8_E_ExpectedContinuation());
    val = (val << 6) | as$(u32)(wtf8__continuationPayload(*A_at((bytes)[1])));
    if (!wtf8__isContinuation(*A_at((bytes)[2]))) return_err(wtf8_E_ExpectedContinuation());
    val = (val << 6) | as$(u32)(wtf8__continuationPayload(*A_at((bytes)[2])));
    if (val < 0x800) return_err(wtf8_E_OverlongEncoding());
    return_ok(val);
} $unscoped(fn);

fn_((wtf8_decode(S_const$u8 bytes))(wtf8_E$u32) $scope) {
    if (bytes.len == 0) return_err(wtf8_E_InvalidStartByte());
    let seq_len = try_(utf8_byteSeqLen(*S_at((bytes)[0])));
    if (seq_len > bytes.len) return_err(wtf8_E_InvalidStartByte());
    switch (seq_len) {
    case utf8_SeqLen_1: return_ok(*S_at((bytes)[0]));
    case utf8_SeqLen_2: return_ok(try_(utf8_decode2(S_deref$((const utf8_Decode2Buf)S_prefix((bytes)2)))));
    case utf8_SeqLen_3: return_ok(try_(wtf8__decode3(S_deref$((const utf8_Decode3Buf)S_prefix((bytes)3)))));
    case utf8_SeqLen_4: return_ok(try_(utf8_decode4(S_deref$((const utf8_Decode4Buf)S_prefix((bytes)4)))));
    }
} $unscoped(fn);

fn_((wtf8_validate(S_const$u8 bytes))(bool) $scope) {
    var_(idx, usize) = 0;
    while (idx < bytes.len) {
        let rest = S_slice((bytes)$r(idx, bytes.len));
        let codepoint = catch_((wtf8_decode(rest))($ignore, return_(false)));
        let seq_len = catch_((utf8_codepointSeqLen(codepoint))($ignore, claim_unreachable));
        idx += seq_len;
    }
    return_(true);
} $unscoped(fn);

fn_((wtf8_count(S_const$u8 bytes))(usize) $scope) {
    var_(count, usize) = 0;
    var_(idx, usize) = 0;
    while (idx < bytes.len) {
        let rest = S_slice((bytes)$r(idx, bytes.len));
        let codepoint = catch_((wtf8_decode(rest))($ignore, {
            idx += 1;
            continue;
        }));
        let seq_len = catch_((utf8_codepointSeqLen(codepoint))($ignore, claim_unreachable));
        idx += seq_len;
        count += 1;
    }
    return_(count);
} $unscoped(fn);

fn_((wtf8_view(S_const$u8 bytes))(wtf8_E$wtf8_View) $scope) {
    if (!wtf8_validate(bytes)) return_err(wtf8_E_InvalidStartByte());
    return_ok({ .bytes = bytes });
} $unscoped(fn);

fn_((wtf8_viewUnchkd(S_const$u8 bytes))(wtf8_View) $scope) {
    return_({ .bytes = bytes });
} $unscoped(fn);

fn_((wtf8_iter(wtf8_View view))(wtf8_Iter) $scope) {
    return_({ .bytes = view.bytes, .idx = 0 });
} $unscoped(fn);

fn_((wtf8_Iter_next(wtf8_Iter* self))(O$u32) $scope) {
    let bytes = orelse_((wtf8_Iter_nextBytes(self))(return_none()));
    return_some(catch_((wtf8_decode(bytes))($ignore, claim_unreachable)));
} $unscoped(fn);

fn_((wtf8_Iter_nextBytes(wtf8_Iter* self))(O$S_const$u8) $scope) {
    if (self->idx >= self->bytes.len) return_none();
    let first = *S_at((self->bytes)[self->idx]);
    let seq_len = catch_((utf8_byteSeqLen(first))($ignore, claim_unreachable));
    self->idx += seq_len;
    return_some(S_slice((self->bytes)$r(self->idx - seq_len, self->idx)));
} $unscoped(fn);

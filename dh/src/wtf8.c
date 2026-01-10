#include "dh/wtf8.h"
#include "dh/utf16.h"

$attr($inline_always)
$static fn_((wtf8__isContinuation(u8 byte))(bool)) {
    return (byte & prim_maskHi_static$((u8)(2))) == prim_maskHi_static$((u8)(1));
};

$attr($inline_always)
$static fn_((wtf8__continuationPayload(u8 byte))(u8)) {
    return byte & prim_maskLo_static$((u8)(6));
};

$attr($inline_always)
$static fn_((wtf8__makeContinuationByte(u8 payload))(u8)) {
    return prim_maskHi_static$((u8)(1)) | (payload & prim_maskLo_static$((u8)(6)));
};

$attr($must_check)
$static fn_((wtf8__encode(u32 codepoint, utf8_SeqLen requested_len, S$u8 out))(wtf8_Err$S$u8) $scope) {
    claim_assert(requested_len <= out.len);
    switch (requested_len) {
    case utf8_SeqLen_1:
        *S_at((out)[0]) = intCast$((u8)codepoint);
        break;
    case utf8_SeqLen_2:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_2 | (codepoint >> 6)));
        *S_at((out)[1]) = wtf8__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    case utf8_SeqLen_3:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_3 | (codepoint >> 12)));
        *S_at((out)[1]) = wtf8__makeContinuationByte(intCast$((u8)(codepoint >> 6)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[2]) = wtf8__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    case utf8_SeqLen_4:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_4 | (codepoint >> 18)));
        *S_at((out)[1]) = wtf8__makeContinuationByte(intCast$((u8)(codepoint >> 12)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[2]) = wtf8__makeContinuationByte(intCast$((u8)(codepoint >> 6)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[3]) = wtf8__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    }
    return_ok(S_slice((out)$r(0, requested_len)));
} $unscoped_(fn);

fn_((wtf8_encode(u32 codepoint, S$u8 out))(E$S$u8) $scope) {
    let requested_len = try_(utf8_codepointSeqLen(codepoint));
    if (requested_len >= out.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok(try_(wtf8__encode(codepoint, requested_len, out)));
} $unscoped_(fn);

fn_((wtf8_encodeWithin(u32 codepoint, S$u8 out))(wtf8_Err$S$u8) $scope) {
    return_(wtf8__encode(codepoint, try_(utf8_codepointSeqLen(codepoint)), out));
} $unscoped_(fn);

$static fn_((wtf8__decode3(utf8_Decode3Buf bytes))(wtf8_Err$u32) $scope) {
    claim_assert((*A_at((bytes)[0]) & prim_maskHi_static$((u8)(4))) == utf8_SeqByte_3);
    var_(val, u32) = *A_at((bytes)[0]) & prim_maskLo_static$((u8)(4));
    if (!wtf8__isContinuation(*A_at((bytes)[1]))) { return_err(wtf8_Err_ExpectedContinuation()); }
    val = (val << 6) | (u32)(wtf8__continuationPayload(*A_at((bytes)[1])));
    if (!wtf8__isContinuation(*A_at((bytes)[2]))) { return_err(wtf8_Err_ExpectedContinuation()); }
    val = (val << 6) | (u32)(wtf8__continuationPayload(*A_at((bytes)[2])));
    if (val < 0x800) { return_err(wtf8_Err_OverlongEncoding()); }
    return_ok(val);
} $unscoped_(fn);

fn_((wtf8_decode(S_const$u8 bytes))(wtf8_Err$u32) $scope) {
    if (bytes.len == 0) { return_err(wtf8_Err_InvalidStartByte()); }
    let seq_len = try_(utf8_byteSeqLen(*S_at((bytes)[0])));
    if (seq_len > bytes.len) { return_err(wtf8_Err_InvalidStartByte()); }
    switch (seq_len) {
    case utf8_SeqLen_1: return_ok(*S_at((bytes)[0]));
    case utf8_SeqLen_2: return_ok(try_(utf8_decode2(S_deref$((const utf8_Decode2Buf)S_prefix((bytes)2)))));
    case utf8_SeqLen_3: return_ok(try_(wtf8__decode3(S_deref$((const utf8_Decode3Buf)S_prefix((bytes)3)))));
    case utf8_SeqLen_4: return_ok(try_(utf8_decode4(S_deref$((const utf8_Decode4Buf)S_prefix((bytes)4)))));
    }
} $unscoped_(fn);

fn_((wtf8_validate(S_const$u8 bytes))(bool) $scope) {
    var_(idx, usize) = 0;
    while (idx < bytes.len) {
        let rest = S_slice((bytes)$r(idx, bytes.len));
        let codepoint = catch_((wtf8_decode(rest))($ignore, return_(false)));
        let seq_len = catch_((utf8_codepointSeqLen(codepoint))($ignore, claim_unreachable));
        idx += seq_len;
    }
    return_(true);
} $unscoped_(fn);

fn_((wtf8_view(S_const$u8 bytes))(wtf8_Err$wtf8_View) $scope) {
    if (!wtf8_validate(bytes)) { return_err(wtf8_Err_InvalidStartByte()); }
    return_ok({ .bytes = bytes });
} $unscoped_(fn);

fn_((wtf8_viewUnchkd(S_const$u8 bytes))(wtf8_View) $scope) {
    return_({ .bytes = bytes });
} $unscoped_(fn);

fn_((wtf8_iter(wtf8_View view))(wtf8_Iter) $scope) {
    return_({ .bytes = view.bytes, .idx = 0 });
} $unscoped_(fn);

fn_((wtf8_Iter_next(wtf8_Iter* self))(O$u32) $scope) {
    let bytes = orelse_((wtf8_Iter_nextBytes(self))(return_none()));
    return_some(catch_((wtf8_decode(bytes))($ignore, claim_unreachable)));
} $unscoped_(fn);

fn_((wtf8_Iter_nextBytes(wtf8_Iter* self))(O$S_const$u8) $scope) {
    if (self->idx >= self->bytes.len) { return_none(); }
    let first = *S_at((self->bytes)[self->idx]);
    let seq_len = catch_((utf8_byteSeqLen(first))($ignore, claim_unreachable));
    self->idx += seq_len;
    return_some(S_slice((self->bytes)$r(self->idx - seq_len, self->idx)));
} $unscoped_(fn);

fn_((wtf8_calcWTF16Len(S_const$u8 wtf8))(wtf8_Err$usize) $scope) {
    var_(len16, usize) = 0;
    var it = wtf8_iter(wtf8_viewUnchkd(wtf8));
    while_some((wtf8_Iter_next(&it)), codepoint) {
        len16 += (codepoint < 0x10000) ? 1 : 2;
    }
    return_ok(len16);
} $unscoped_(fn);

$attr($must_check)
$static fn_((wtf8__toWTF16(S_const$u8 wtf8, usize required_len, S$u16 out_wtf16))(S$u16)) {
    claim_assert(required_len <= out_wtf16.len);
    var_(out_idx, usize) = 0;
    var it = wtf8_iter(wtf8_viewUnchkd(wtf8));
    while_some((wtf8_Iter_next(&it)), codepoint) {
        if (codepoint < 0x10000) {
            *S_at((out_wtf16)[out_idx]) = intCast$((u16)codepoint);
            out_idx += 1;
        } else {
            let high = intCast$((u16)((codepoint - 0x10000) >> 10) + 0xD800);
            let low = intCast$((u16)(codepoint & 0x3FF) + 0xDC00);
            *S_at((out_wtf16)[out_idx]) = high;
            *S_at((out_wtf16)[out_idx + 1]) = low;
            out_idx += 2;
        }
    }
    return S_slice((out_wtf16)$r(0, out_idx));
};

fn_((wtf8_toWTF16(S_const$u8 wtf8, S$u16 out_wtf16))(E$S$u16) $scope) {
    let required_len = try_(wtf8_calcWTF16Len(wtf8));
    if (required_len > out_wtf16.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok(wtf8__toWTF16(wtf8, required_len, out_wtf16));
} $unscoped_(fn);

fn_((wtf8_toWTF16Within(S_const$u8 wtf8, S$u16 out_wtf16))(wtf8_Err$S$u16) $scope) {
    return_ok(wtf8__toWTF16(wtf8, try_(wtf8_calcWTF16Len(wtf8)), out_wtf16));
} $unscoped_(fn);

fn_((wtf8_toWTF16Alloc(S_const$u8 wtf8, mem_Allocator gpa))(wtf8_mem_Err$S$u16) $scope) {
    let required_len = try_(wtf8_calcWTF16Len(wtf8));
    let required_buf = u_castS$((S$u16)try_(mem_Allocator_alloc(gpa, typeInfo$(u16), required_len)));
    let wtf16 = wtf8__toWTF16(wtf8, required_len, required_buf);
    claim_assert(wtf16.len == required_buf.len);
    return_ok(wtf16);
} $unscoped_(fn);

fn_((wtf8_toUTF8LossyAlloc(S_const$u8 wtf8, mem_Allocator gpa))(wtf8_mem_Err$S$u8) $scope) {
    if (!wtf8_validate(wtf8)) { return_err(wtf8_Err_InvalidStartByte()); }
    let len = wtf8.len;
    let buf = u_castS$((S$u8)try_(mem_Allocator_alloc(gpa, typeInfo$(u8), len)));
    var_(dst_idx, usize) = 0;
    var it = wtf8_iter(wtf8_viewUnchkd(wtf8));
    while_some((wtf8_Iter_next(&it)), codepoint) {
        if (utf16_isSurrogate(codepoint)) {
            let replacement = utf8_replacement_ch;
            let repl_len = catch_((utf8_codepointSeqLen(replacement))($ignore, claim_unreachable));
            if (dst_idx + repl_len > buf.len) { return_err(mem_Err_OutOfMemory()); }
            let encoded = catch_((utf8_encodeWithin(replacement, S_slice((buf)$r(dst_idx, dst_idx + repl_len))))($ignore, claim_unreachable));
            dst_idx += encoded.len;
        } else {
            let seq_len = catch_((utf8_codepointSeqLen(codepoint))($ignore, claim_unreachable));
            if (dst_idx + seq_len > buf.len) { return_err(mem_Err_OutOfMemory()); }
            let encoded = catch_((utf8_encodeWithin(codepoint, S_slice((buf)$r(dst_idx, dst_idx + seq_len))))($ignore, claim_unreachable));
            dst_idx += encoded.len;
        }
    }
    return_ok(S_slice((buf)$r(0, dst_idx)));
} $unscoped_(fn);

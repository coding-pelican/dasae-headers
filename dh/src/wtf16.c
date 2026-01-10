#include "dh/wtf16.h"
#include "dh/wtf8.h"

fn_((wtf16_iter(S_const$u16 units))(wtf16_Iter) $scope) {
    return_({ .bytes = units, .idx = 0 });
} $unscoped_(fn);

fn_((wtf16_Iter_next(wtf16_Iter* self))(O$u32) $scope) {
    if (self->idx >= self->bytes.len) { return_none(); }
    let first = *S_at((self->bytes)[self->idx]);
    if (utf16_isHighSurrogate(first)) {
        if (self->idx + 1 < self->bytes.len) {
            let second = *S_at((self->bytes)[self->idx + 1]);
            if (utf16_isLowSurrogate(second)) {
                let codepoint = catch_((utf16_decodeSurrogatePair(first, second))($ignore, claim_unreachable));
                self->idx += 2;
                return_some(codepoint);
            }
        }
        self->idx += 1;
        return_some((u32)first);
    } else {
        self->idx += 1;
        return_some((u32)first);
    }
} $unscoped_(fn);

fn_((wtf16_calcWTF8Len(S_const$u16 wtf16))(usize) $scope) {
    var_(len8, usize) = 0;
    var it = wtf16_iter(wtf16);
    while_some((wtf16_Iter_next(&it)), codepoint) {
        let seq_len = catch_((utf8_codepointSeqLen(codepoint))($ignore, claim_unreachable));
        len8 += seq_len;
    }
    return_(len8);
} $unscoped_(fn);

$attr($inline_always)
$static fn_((wtf16__makeContinuationByte(u8 payload))(u8)) {
    return prim_maskHi_static$((u8)(1)) | (payload & prim_maskLo_static$((u8)(6)));
};

$attr($must_check)
$static fn_((wtf16__encodeCodepoint(u32 codepoint, utf8_SeqLen seq_len, S$u8 out))(S$u8) $scope) {
    claim_assert(seq_len <= out.len);
    switch (seq_len) {
    case utf8_SeqLen_1:
        *S_at((out)[0]) = intCast$((u8)codepoint);
        break;
    case utf8_SeqLen_2:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_2 | (codepoint >> 6)));
        *S_at((out)[1]) = wtf16__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    case utf8_SeqLen_3:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_3 | (codepoint >> 12)));
        *S_at((out)[1]) = wtf16__makeContinuationByte(intCast$((u8)(codepoint >> 6)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[2]) = wtf16__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    case utf8_SeqLen_4:
        *S_at((out)[0]) = intCast$((u8)(utf8_SeqByte_4 | (codepoint >> 18)));
        *S_at((out)[1]) = wtf16__makeContinuationByte(intCast$((u8)(codepoint >> 12)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[2]) = wtf16__makeContinuationByte(intCast$((u8)(codepoint >> 6)) & prim_maskLo_static$((u8)(6)));
        *S_at((out)[3]) = wtf16__makeContinuationByte(intCast$((u8)codepoint) & prim_maskLo_static$((u8)(6)));
        break;
    }
    return_(S_slice((out)$r(0, seq_len)));
} $unscoped_(fn);

$attr($must_check)
$static fn_((wtf16__toWTF8(S_const$u16 wtf16, usize required_len, S$u8 out_wtf8))(S$u8) $scope) {
    claim_assert(required_len <= out_wtf8.len);
    var_(out_idx, usize) = 0;
    var it = wtf16_iter(wtf16);
    while_some((wtf16_Iter_next(&it)), codepoint) {
        let seq_len = catch_((utf8_codepointSeqLen(codepoint))($ignore, claim_unreachable));
        let encoded = wtf16__encodeCodepoint(codepoint, seq_len, S_slice((out_wtf8)$r(out_idx, out_idx + seq_len)));
        out_idx += encoded.len;
    }
    return_(S_slice((out_wtf8)$r(0, out_idx)));
} $unscoped_(fn);

fn_((wtf16_toWTF8(S_const$u16 wtf16, S$u8 out_wtf8))(wtf16_mem_Err$S$u8) $scope) {
    let required_len = wtf16_calcWTF8Len(wtf16);
    if (required_len > out_wtf8.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok(wtf16__toWTF8(wtf16, required_len, out_wtf8));
} $unscoped_(fn);

fn_((wtf16_toWTF8Within(S_const$u16 wtf16, S$u8 out_wtf8))(S$u8) $scope) {
    return_(wtf16__toWTF8(wtf16, wtf16_calcWTF8Len(wtf16), out_wtf8));
} $unscoped_(fn);

fn_((wtf16_toWTF8Alloc(S_const$u16 wtf16, mem_Allocator gpa))(wtf16_mem_Err$S$u8) $scope) {
    let required_len = wtf16_calcWTF8Len(wtf16);
    let required_buf = u_castS$((S$u8)try_(mem_Allocator_alloc(gpa, typeInfo$(u8), required_len)));
    let wtf8 = wtf16__toWTF8(wtf16, required_len, required_buf);
    claim_assert(wtf8.len == required_buf.len);
    return_ok(wtf8);
} $unscoped_(fn);

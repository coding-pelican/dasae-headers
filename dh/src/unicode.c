#include "dh/unicode.h"

/*---------------------------------------------------------------------------
 * Section: Strict Conversions (UTF-8 <-> UTF-16)
 *-------------------------------------------------------------------------*/

fn_((unicode_utf8ToUTF16Len(S_const$u8 utf8))(unicode_utf8_Err$usize) $scope) {
    var_(len16, usize) = 0;
    var_(idx, usize) = 0;
    while (idx < utf8.len) {
        let seq_len = try_(utf8_byteSeqLen(*S_at((utf8)[idx])));
        let next_idx = idx + seq_len;
        if (next_idx > utf8.len) { return_err(utf8_Err_InvalidBytes()); }
        let codepoint = try_(utf8_decode(S_slice((utf8)$r(idx, next_idx))));
        len16 += (codepoint < 0x10000) ? 1 : 2;
        idx += seq_len;
    }
    return_ok(len16);
} $unscoped_(fn);

$static fn_((unicode__utf8ToUTF16(S_const$u8 utf8, usize required_len, S$u16 out_utf16))(S$u16)) {
    claim_assert(required_len <= out_utf16.len);
    var_(out_idx, usize) = 0;
    var it = utf8_iter(utf8_viewUnchkd(utf8));
    while_some(utf8_Iter_next(&it), codepoint) {
        let rest_slice = S_slice((out_utf16)$r(out_idx, out_utf16.len));
        let encoded = catch_((utf16_encodeWithin(codepoint, rest_slice))($ignore, claim_unreachable));
        out_idx += encoded.len;
    }
    return S_slice((out_utf16)$r(0, out_idx));
};

fn_((unicode_utf8ToUTF16(S_const$u8 utf8, S$u16 out))(E$S$u16) $scope) {
    let required_len = try_(unicode_utf8ToUTF16Len(utf8));
    if (required_len > out.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok(unicode__utf8ToUTF16(utf8, required_len, out));
} $unscoped_(fn);

fn_((unicode_utf8ToUTF16Within(S_const$u8 utf8, S$u16 out))(unicode_utf8_Err$S$u16) $scope) {
    return_ok(unicode__utf8ToUTF16(utf8, try_(unicode_utf8ToUTF16Len(utf8)), out));
} $unscoped_(fn);

fn_((unicode_utf8ToUTF16Alloc(S_const$u8 utf8, mem_Allocator gpa))(unicode_mem_Err$S$u16) $scope) {
    let required_len = try_(unicode_utf8ToUTF16Len(utf8));
    let required_buf = u_castS$((S$u16)try_(mem_Allocator_alloc(gpa, typeInfo$(u16), required_len)));
    let utf16 = unicode__utf8ToUTF16(utf8, required_len, required_buf);
    claim_assert(utf16.len == required_buf.len);
    return_ok(utf16);
} $unscoped_(fn);

fn_((unicode_utf16ToUTF8Len(S_const$u16 utf16))(unicode_utf16_Err$usize) $scope) {
    var_(len8, usize) = 0;
    var it = utf16_iter(utf16);
    while_some((try_(utf16_Iter_next(&it))), codepoint) {
        let seq_len = try_(utf8_codepointSeqLen(codepoint));
        len8 += seq_len;
    }
    return_ok(len8);
} $unscoped_(fn);

$attr($must_check)
$static fn_((unicode__utf16ToUTF8(S_const$u16 utf16, usize required_len, S$u8 out_utf8))(unicode_utf16_Err$S$u8) $scope) {
    claim_assert(required_len <= out_utf8.len);
    var_(out_idx, usize) = 0;
    var it = utf16_iter(utf16);
    while_some((try_(utf16_Iter_next(&it))), codepoint) {
        let rest_slice = S_slice((out_utf8)$r(out_idx, out_utf8.len));
        let encoded = try_(utf8_encodeWithin(codepoint, rest_slice));
        out_idx += encoded.len;
    }
    return_ok(S_slice((out_utf8)$r(0, out_idx)));
} $unscoped_(fn);

fn_((unicode_utf16ToUTF8(S_const$u16 utf16, S$u8 out))(E$S$u8) $scope) {
    let required_len = try_(unicode_utf16ToUTF8Len(utf16));
    if (required_len > out.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok(try_(unicode__utf16ToUTF8(utf16, required_len, out)));
} $unscoped_(fn);

fn_((unicode_utf16ToUTF8Within(S_const$u16 utf16, S$u8 out))(unicode_utf16_Err$S$u8) $scope) {
    return_(unicode__utf16ToUTF8(utf16, try_(unicode_utf16ToUTF8Len(utf16)), out));
} $unscoped_(fn);

fn_((unicode_utf16ToUTF8Alloc(S_const$u16 utf16, mem_Allocator gpa))(unicode_mem_Err$S$u8) $scope) {
    let required_len = try_(unicode_utf16ToUTF8Len(utf16));
    let required_buf = u_castS$((S$u8)try_(mem_Allocator_alloc(gpa, typeInfo$(u8), required_len)));
    let utf8 = try_(unicode__utf16ToUTF8(utf16, required_len, required_buf));
    claim_assert(utf8.len == required_buf.len);
    return_ok(utf8);
} $unscoped_(fn);

/*---------------------------------------------------------------------------
 * Section: Permissive Conversions (WTF-8 <-> WTF-16)
 *-------------------------------------------------------------------------*/

fn_((unicode_wtf8ToWTF16Len(S_const$u8 wtf8))(usize)) {
    var_(len16, usize) = 0;
    var it = wtf8_iter(wtf8_viewUnchkd(wtf8));
    while_some((wtf8_Iter_next(&it)), codepoint) {
        len16 += (codepoint < 0x10000) ? 1 : 2;
    }
    return len16;
};

$static fn_((unicode__wtf8ToWTF16(S_const$u8 wtf8, usize required_len, S$u16 out_wtf16))(S$u16)) {
    claim_assert(required_len <= out_wtf16.len);
    var_(out_idx, usize) = 0;
    var it = wtf8_iter(wtf8_viewUnchkd(wtf8));
    while_some((wtf8_Iter_next(&it)), codepoint) {
        let rest_slice = S_slice((out_wtf16)$r(out_idx, out_wtf16.len));
        if (utf16_isSurrogate(codepoint)) {
            claim_assert(1 <= rest_slice.len);
            *S_at((rest_slice)[0]) = intCast$((u16)codepoint);
            out_idx += 1;
        } else {
            let encoded = catch_((utf16_encodeWithin(codepoint, rest_slice))($ignore, claim_unreachable));
            out_idx += encoded.len;
        }
    }
    return S_slice((out_wtf16)$r(0, out_idx));
};

fn_((unicode_wtf8ToWTF16(S_const$u8 wtf8, S$u16 out))(E$S$u16) $scope) {
    let required_len = unicode_wtf8ToWTF16Len(wtf8);
    if (required_len > out.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok(unicode__wtf8ToWTF16(wtf8, required_len, out));
} $unscoped_(fn);

fn_((unicode_wtf8ToWTF16Within(S_const$u8 wtf8, S$u16 out))(unicode_wtf8_Err$S$u16) $scope) {
    return_ok(unicode__wtf8ToWTF16(wtf8, unicode_wtf8ToWTF16Len(wtf8), out));
} $unscoped_(fn);

fn_((unicode_wtf8ToWTF16Alloc(S_const$u8 wtf8, mem_Allocator gpa))(unicode_mem_Err$S$u16) $scope) {
    let required_len = unicode_wtf8ToWTF16Len(wtf8);
    let required_buf = u_castS$((S$u16)try_(mem_Allocator_alloc(gpa, typeInfo$(u16), required_len)));
    let wtf16 = unicode__wtf8ToWTF16(wtf8, required_len, required_buf);
    claim_assert(wtf16.len == required_buf.len);
    return_ok(wtf16);
} $unscoped_(fn);

fn_((unicode_wtf16ToWTF8Len(S_const$u16 wtf16))(usize)) {
    var_(len8, usize) = 0;
    var it = wtf16_iter(wtf16);
    while_some((wtf16_Iter_next(&it)), codepoint) {
        let seq_len = catch_((utf8_codepointSeqLen(codepoint))($ignore, claim_unreachable));
        len8 += seq_len;
    }
    return len8;
};

$attr($must_check)
$static fn_((unicode__wtf16ToWTF8(S_const$u16 wtf16, usize required_len, S$u8 out_wtf8))(S$u8)) {
    claim_assert(required_len <= out_wtf8.len);
    var_(out_idx, usize) = 0;
    var it = wtf16_iter(wtf16);
    while_some((wtf16_Iter_next(&it)), codepoint) {
        let rest_slice = S_slice((out_wtf8)$r(out_idx, out_wtf8.len));
        let encoded = catch_((wtf8_encodeWithin(codepoint, rest_slice))($ignore, claim_unreachable));
        out_idx += encoded.len;
    }
    return S_slice((out_wtf8)$r(0, out_idx));
};

fn_((unicode_wtf16ToWTF8(S_const$u16 wtf16, S$u8 out))(unicode_mem_Err$S$u8) $scope) {
    let required_len = unicode_wtf16ToWTF8Len(wtf16);
    if (required_len > out.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok(unicode__wtf16ToWTF8(wtf16, required_len, out));
} $unscoped_(fn);

fn_((unicode_wtf16ToWTF8Within(S_const$u16 wtf16, S$u8 out))(S$u8)) {
    return unicode__wtf16ToWTF8(wtf16, unicode_wtf16ToWTF8Len(wtf16), out);
};

fn_((unicode_wtf16ToWTF8Alloc(S_const$u16 wtf16, mem_Allocator gpa))(unicode_mem_Err$S$u8) $scope) {
    let required_len = unicode_wtf16ToWTF8Len(wtf16);
    let required_buf = u_castS$((S$u8)try_(mem_Allocator_alloc(gpa, typeInfo$(u8), required_len)));
    let wtf8 = unicode__wtf16ToWTF8(wtf16, required_len, required_buf);
    claim_assert(wtf8.len == required_buf.len);
    return_ok(wtf8);
} $unscoped_(fn);

/*---------------------------------------------------------------------------
 * Section: Cross-Casting & Validation (View Conversions)
 *-------------------------------------------------------------------------*/

fn_((unicode_wtf8AsUTF8(wtf8_View wtf))(utf8_Err$utf8_View) $scope) {
    if (!wtf8_validate(wtf.bytes)) { return_err(utf8_Err_InvalidBytes()); }
    return_ok(utf8_viewUnchkd(wtf.bytes));
} $unscoped_(fn);

fn_((unicode_wtf8ToUTF8LossyAlloc(S_const$u8 wtf8, mem_Allocator gpa))(unicode_mem_Err$S$u8) $scope) {
    if (!wtf8_validate(wtf8)) { return_err(utf8_Err_InvalidBytes()); }
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
            dst_idx += seq_len;
        }
    }
    return_ok(S_slice((buf)$r(0, dst_idx)));
} $unscoped_(fn);

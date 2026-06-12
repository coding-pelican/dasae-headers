#define main_no_args pp_true
#include "dh-main.h"
#include "dh/prl/va.h"
#include "dh/meta.h"
#include "dh/mem/common.h"
#include "dh/ascii.h"
#include <stdio.h>

#if !defined(DRAFT_FMT_CASE)
#define DRAFT_FMT_CASE -1
#endif

typedef enum_((draft_fmt__Size $fits($packed))(
    draft_fmt__Size_8 = 0, // hh
    draft_fmt__Size_16, // h
    draft_fmt__Size_32, // (none)
    draft_fmt__Size_long, // l
    draft_fmt__Size_ptr, // z
    draft_fmt__Size_64, // ll
    draft_fmt__Size_128, // q
)) draft_fmt__Size;

$static let draft_fmt__Size_default = draft_fmt__Size_32;

#if DRAFT_FMT_CASE < 10
$attr($inline)
$static fn_((draft_fmt__formatRuntime(S$u8 mem, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$S$u8));

$attr($inline)
$static fn_((draft_fmt__findSpecOpen(S_const$u8 fmt, usize pos))(O$usize));
$attr($inline)
$static fn_((draft_fmt__findSpecClose(S_const$u8 fmt, usize body_begin))(usize));
$attr($inline)
$static fn_((draft_fmt__findIndexEnd(S_const$u8 fmt, usize body_begin, usize body_end))(O$usize));
$attr($inline)
$static fn_((draft_fmt__parseIndex(S_const$u8 fmt, usize body_begin, usize index_end))(usize));
$attr($inline)
$static fn_((draft_fmt__writeLiteralChunk(S$u8 out, S_const$u8 text, usize len))(usize));
#endif

$attr($inline)
$static fn_((draft_fmt__findSpecColon(S_const$u8 fmt, usize body_begin, usize body_end))(usize));
$attr($inline)
$static fn_((draft_fmt__parseSize(S_const$u8 fmt, usize size_begin, usize colon_pos))(draft_fmt__Size));
$attr($inline)
$static fn_((draft_fmt__parseSign(S_const$u8 opts))(O$u8));
$attr($inline)
$static fn_((draft_fmt__parseWidth(S_const$u8 opts))(usize));
$attr($inline)
$static fn_((draft_fmt__writeSpecBody(S$u8 out, S_const$u8 body, u_P_const$raw field))(usize));
$attr($inline)
$static fn_((draft_fmt__writeField(S$u8 out, u8 type_ch, draft_fmt__Size size, S_const$u8 opts, u_P_const$raw field))(usize));
$attr($inline)
$static fn_((draft_fmt__writePadded(S$u8 out, S_const$u8 content, S_const$u8 opts))(usize));
$attr($inline)
$static fn_((draft_fmt__writeI32(S$u8 out, i32 value, O$u8 sign))(usize));
$attr($inline)
$static fn_((draft_fmt__writeU32(S$u8 out, u32 value, u32 base, S_const$u8 digits))(usize));
$attr($inline)
$static fn_((draft_fmt__writeF64(S$u8 out, f64 value))(usize));
$attr($inline)
$static fn_((draft_fmt__writeSliU8(S$u8 out, S_const$u8 value))(usize));

fn_((main(void))(E$void) $scope) {
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 0
    var_(mem0, A$$(96, u8)) = A_zero();
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 1
    var_(mem1, A$$(96, u8)) = A_zero();
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 2
    var_(mem2, A$$(96, u8)) = A_zero();
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 3
    var_(mem3, A$$(96, u8)) = A_zero();
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 4
    var_(mem4, A$$(96, u8)) = A_zero();
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 5
    var_(mem5, A$$(96, u8)) = A_zero();
#endif
#if DRAFT_FMT_CASE == 10
    var_(mem10, A$$(96, u8)) = A_zero();
    var_(arg10, i32) = 123;
#endif
#if DRAFT_FMT_CASE == 11
    var_(mem11, A$$(96, u8)) = A_zero();
    var_(arg11, u32) = 42;
#endif
#if DRAFT_FMT_CASE == 12
    var_(mem12, A$$(96, u8)) = A_zero();
    var_(arg12, S_const$u8) = u8_l("world");
#endif
#if DRAFT_FMT_CASE == 13
    var_(mem13, A$$(96, u8)) = A_zero();
    var_(arg13, f64) = 3.5;
#endif
#if DRAFT_FMT_CASE == 14
    var_(mem14, A$$(96, u8)) = A_zero();
    var_(arg14, Void) = Void_();
#endif
#if DRAFT_FMT_CASE == 20
    var_(mem20, A$$(96, u8)) = A_zero();
    var_(arg20, i32) = 123;
#endif
#if DRAFT_FMT_CASE == 21
    var_(mem21, A$$(96, u8)) = A_zero();
    var_(arg21, u32) = 42;
#endif
#if DRAFT_FMT_CASE == 22
    var_(mem22, A$$(96, u8)) = A_zero();
    var_(arg22, S_const$u8) = u8_l("world");
#endif
#if DRAFT_FMT_CASE == 30
    var_(mem30, A$$(96, u8)) = A_zero();
    var_(arg30, i32) = 123;
#endif
#if DRAFT_FMT_CASE == 31
    var_(mem31, A$$(96, u8)) = A_zero();
    var_(arg31, i32) = 123;
#endif

#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 0
    let s0 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem0), u8_l("case0: literal {{ok}} abc"))()));
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 1
    let s1 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem1), u8_l("case1: {i:} def"))(123)));
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 2
    let s2 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem2), u8_l("case2: {s:} {i:}"))(u8_l("world"), -5)));
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 3
    let s3 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem3), u8_l("case3: {u:} {x:} {X:} {o:} {b:}"))(u32_(42), u32_(42), u32_(42), u32_(42), u32_(42))));
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 4
    let s4 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem4), u8_l("case4: {[1]i:}{[0]s:}{[0]s:}{[1]i:}"))(u8_l("456"), 123)));
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 5
    let s5 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem5), u8_l("case5: {fl:} {0:}"))(3.5, Void_())));
#endif
#if DRAFT_FMT_CASE == 10
    let w10 = draft_fmt__writeSpecBody(A_ref$((S$u8)mem10), u8_l("i:"), u_anyP(&arg10).as_const);
    let s10 = S_prefix((A_ref$((S$u8)mem10))w10);
#endif
#if DRAFT_FMT_CASE == 11
    let w11 = draft_fmt__writeSpecBody(A_ref$((S$u8)mem11), u8_l("x:"), u_anyP(&arg11).as_const);
    let s11 = S_prefix((A_ref$((S$u8)mem11))w11);
#endif
#if DRAFT_FMT_CASE == 12
    let w12 = draft_fmt__writeSpecBody(A_ref$((S$u8)mem12), u8_l("s:"), u_anyP(&arg12).as_const);
    let s12 = S_prefix((A_ref$((S$u8)mem12))w12);
#endif
#if DRAFT_FMT_CASE == 13
    let w13 = draft_fmt__writeSpecBody(A_ref$((S$u8)mem13), u8_l("fl:"), u_anyP(&arg13).as_const);
    let s13 = S_prefix((A_ref$((S$u8)mem13))w13);
#endif
#if DRAFT_FMT_CASE == 14
    let w14 = draft_fmt__writeSpecBody(A_ref$((S$u8)mem14), u8_l("0:"), u_anyP(&arg14).as_const);
    let s14 = S_prefix((A_ref$((S$u8)mem14))w14);
#endif
#if DRAFT_FMT_CASE == 20
    let w20 = draft_fmt__writeSpecBody(A_ref$((S$u8)mem20), u8_l("i:6"), u_anyP(&arg20).as_const);
    let s20 = S_prefix((A_ref$((S$u8)mem20))w20);
#endif
#if DRAFT_FMT_CASE == 21
    let w21 = draft_fmt__writeSpecBody(A_ref$((S$u8)mem21), u8_l("x:6"), u_anyP(&arg21).as_const);
    let s21 = S_prefix((A_ref$((S$u8)mem21))w21);
#endif
#if DRAFT_FMT_CASE == 22
    let w22 = draft_fmt__writeSpecBody(A_ref$((S$u8)mem22), u8_l("s:8"), u_anyP(&arg22).as_const);
    let s22 = S_prefix((A_ref$((S$u8)mem22))w22);
#endif
#if DRAFT_FMT_CASE == 30
    let w30 = draft_fmt__writeSpecBody(A_ref$((S$u8)mem30), u8_l("i:+"), u_anyP(&arg30).as_const);
    let s30 = S_prefix((A_ref$((S$u8)mem30))w30);
#endif
#if DRAFT_FMT_CASE == 31
    let w31 = draft_fmt__writeSpecBody(A_ref$((S$u8)mem31), u8_l("i:+6"), u_anyP(&arg31).as_const);
    let s31 = S_prefix((A_ref$((S$u8)mem31))w31);
#endif

#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 0
    puts(as$(const char*)(s0.ptr));
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 1
    puts(as$(const char*)(s1.ptr));
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 2
    puts(as$(const char*)(s2.ptr));
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 3
    puts(as$(const char*)(s3.ptr));
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 4
    puts(as$(const char*)(s4.ptr));
#endif
#if DRAFT_FMT_CASE < 0 || DRAFT_FMT_CASE == 5
    puts(as$(const char*)(s5.ptr));
#endif
#if DRAFT_FMT_CASE == 10
    puts(as$(const char*)(s10.ptr));
#endif
#if DRAFT_FMT_CASE == 11
    puts(as$(const char*)(s11.ptr));
#endif
#if DRAFT_FMT_CASE == 12
    puts(as$(const char*)(s12.ptr));
#endif
#if DRAFT_FMT_CASE == 13
    puts(as$(const char*)(s13.ptr));
#endif
#if DRAFT_FMT_CASE == 14
    puts(as$(const char*)(s14.ptr));
#endif
#if DRAFT_FMT_CASE == 20
    puts(as$(const char*)(s20.ptr));
#endif
#if DRAFT_FMT_CASE == 21
    puts(as$(const char*)(s21.ptr));
#endif
#if DRAFT_FMT_CASE == 22
    puts(as$(const char*)(s22.ptr));
#endif
#if DRAFT_FMT_CASE == 30
    puts(as$(const char*)(s30.ptr));
#endif
#if DRAFT_FMT_CASE == 31
    puts(as$(const char*)(s31.ptr));
#endif

    return_ok({});
} $unscoped(fn);

#if DRAFT_FMT_CASE < 10
$static fn_((draft_fmt__formatRuntime(S$u8 mem, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$S$u8) $scope) {
    claim_assert(fields.len <= usize_(8));
    claim_assert(TypeInfo_eql(tuple.type, u_typeInfoRecord(fields)));

    var fmt_pos = usize_(0);
    var arg_pos = usize_(0);
    var out = mem;

    for_(($r(0, 8))($ignore)) {
        let spec_open = orelse_((draft_fmt__findSpecOpen(fmt, fmt_pos))(break));
        let body_begin = spec_open + 1;
        let body_end = draft_fmt__findSpecClose(fmt, body_begin);
        let colon_pos = draft_fmt__findSpecColon(fmt, body_begin, body_end);
        let index_end = draft_fmt__findIndexEnd(fmt, body_begin, colon_pos);
        let type_pos = expr_(usize $scope)(if_some((index_end)(end)) {
            $break_(end + 1);
        }) expr_(else)($break_(body_begin)) $unscoped(expr);
        let type_ch = *S_at((fmt)[type_pos]);
        let size = draft_fmt__parseSize(fmt, type_pos + 1, colon_pos);
        let arg_idx = expr_(usize $scope)(if_some((index_end)(end)) {
            let idx = draft_fmt__parseIndex(fmt, body_begin + 1, end);
            arg_pos = pri_max(arg_pos, idx + 1);
            $break_(idx);
        }) expr_(else)($break_(arg_pos++)) $unscoped(expr);
        claim_assert(arg_idx < fields.len);

        let rest = S_suffix((fmt)fmt_pos);
        var written = draft_fmt__writeLiteralChunk(out, rest, spec_open - fmt_pos);
        written += draft_fmt__writeField(
            S_suffix((out)written),
            type_ch,
            size,
            u8_l(""),
            u_fieldPtr(tuple, fields, arg_idx)
        );
        out = S_suffix((out)written);
        fmt_pos = body_end + 1;
    } $end(for);

    let_ignore = arg_pos;
    let rest = S_suffix((fmt)fmt_pos);
    out = S_suffix((out)draft_fmt__writeLiteralChunk(out, rest, rest.len));
    return_ok(S_prefix((mem)(mem.len - out.len)));
} $unscoped(fn);

$static fn_((draft_fmt__findSpecOpen(S_const$u8 fmt, usize pos))(O$usize) $scope) {
    let rest = S_suffix((fmt)pos);
    var scan_pos = usize_(0);
    while (scan_pos < rest.len) {
        let rel = orelse_((mem_findFirstUnitBytes(S_suffix((rest)scan_pos), u8_c('{')))(return_none()));
        let idx = scan_pos + rel;
        if (idx + 1 < rest.len && *S_at((rest)[idx + 1]) == u8_c('{')) {
            scan_pos = idx + 2;
            continue;
        }
        return_some(pos + idx);
    }
    return_none();
} $unscoped(fn);

$static fn_((draft_fmt__findSpecClose(S_const$u8 fmt, usize body_begin))(usize)) {
    let close_rel = unwrap_(mem_findFirstUnitBytes(S_suffix((fmt)body_begin), u8_c('}')));
    let body_end = body_begin + close_rel;
    claim_assert(body_begin < body_end);
    return body_end;
};

$static fn_((draft_fmt__findIndexEnd(S_const$u8 fmt, usize body_begin, usize body_end))(O$usize) $scope) {
    if (*S_at((fmt)[body_begin]) != u8_c('[')) return_none();
    let rel = unwrap_(mem_findFirstUnitBytes(S_slice((fmt)$r(body_begin + 1, body_end)), u8_c(']')));
    return_some(body_begin + 1 + rel);
} $unscoped(fn);

$static fn_((draft_fmt__parseIndex(S_const$u8 fmt, usize body_begin, usize index_end))(usize)) {
    var idx = usize_(0);
    for_(($s(S_slice((fmt)$r(body_begin, index_end))))(digit)) {
        claim_assert(ascii_isDigit(*digit));
        idx = idx * usize_(10) + as$(usize)(*digit - u8_c('0'));
    } $end(for);
    return idx;
};
#endif

$static fn_((draft_fmt__findSpecColon(S_const$u8 fmt, usize body_begin, usize body_end))(usize)) {
    let colon_rel = unwrap_(mem_findFirstUnitBytes(S_slice((fmt)$r(body_begin, body_end)), u8_c(':')));
    let colon_pos = body_begin + colon_rel;
    return colon_pos;
};

$static fn_((draft_fmt__parseSize(S_const$u8 fmt, usize size_begin, usize colon_pos))(draft_fmt__Size)) {
    let size = S_slice((fmt)$r(size_begin, colon_pos));
    if (size.len == 0) return draft_fmt__Size_default;
    if (mem_eqlBytes(size, u8_l("hh"))) return draft_fmt__Size_8;
    if (mem_eqlBytes(size, u8_l("h"))) return draft_fmt__Size_16;
    if (mem_eqlBytes(size, u8_l("l"))) return draft_fmt__Size_64;
    if (mem_eqlBytes(size, u8_l("ll"))) return draft_fmt__Size_128;
    if (mem_eqlBytes(size, u8_l("z"))) return draft_fmt__Size_ptr;
    claim_unreachable;
};

$static fn_((draft_fmt__parseSign(S_const$u8 opts))(O$u8)) {
    if (opts.len == 0) return none$((O$u8));
    let ch = *S_at((opts)[0]);
    if (ch == u8_c('+') || ch == u8_c(' ')) return some$((O$u8)(ch));
    return none$((O$u8));
};

$static fn_((draft_fmt__parseWidth(S_const$u8 opts))(usize)) {
    var width = usize_(0);
    for_(($s(opts))(digit)) {
        claim_assert(ascii_isDigit(*digit));
        width = width * usize_(10) + as$(usize)(*digit - u8_c('0'));
    } $end(for);
    return width;
};

#if DRAFT_FMT_CASE < 10
$static fn_((draft_fmt__writeLiteralChunk(S$u8 out, S_const$u8 text, usize len))(usize)) {
    var read = usize_(0);
    var written = usize_(0);
    let chunk = S_prefix((text)len);
    while (read < chunk.len) {
        let ch = *S_at((chunk)[read]);
        if (ch == u8_c('{') && read + 1 < chunk.len && *S_at((chunk)[read + 1]) == u8_c('{')) {
            *S_at((out)[written++]) = u8_c('{');
            read += 2;
        } else if (ch == u8_c('}') && read + 1 < chunk.len && *S_at((chunk)[read + 1]) == u8_c('}')) {
            *S_at((out)[written++]) = u8_c('}');
            read += 2;
        } else {
            *S_at((out)[written++]) = ch;
            read += 1;
        }
    }
    return written;
};
#endif

$static fn_((draft_fmt__writeField(S$u8 out, u8 type_ch, draft_fmt__Size size, S_const$u8 opts, u_P_const$raw field))(usize)) {
    var_(tmp, A$$(96, u8)) = A_zero();
    let target = opts.len == 0 ? out : A_ref$((S$u8)tmp);
    var pad_opts = opts;
    var written = usize_(0);
    switch (type_ch) {
    case u8_c('0'):
        claim_assert(size == draft_fmt__Size_default);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(Void)));
        let_ignore = *u_castP$((const Void*)(field));
        written = 0;
        break;
    case u8_c('d'):
    case u8_c('i'):
        claim_assert(size == draft_fmt__Size_default);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(i32)));
        let sign = draft_fmt__parseSign(opts);
        if_some((sign)(ch)) {
            let_ignore = ch;
            pad_opts = S_suffix((opts)1);
        }
        written = draft_fmt__writeI32(target, *u_castP$((const i32*)(field)), sign);
        break;
    case u8_c('u'):
        claim_assert(size == draft_fmt__Size_default);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
        written = draft_fmt__writeU32(target, *u_castP$((const u32*)(field)), u32_(10), u8_l("0123456789"));
        break;
    case u8_c('x'):
        claim_assert(size == draft_fmt__Size_default);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
        written = draft_fmt__writeU32(target, *u_castP$((const u32*)(field)), u32_(16), u8_l("0123456789abcdef"));
        break;
    case u8_c('X'):
        claim_assert(size == draft_fmt__Size_default);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
        written = draft_fmt__writeU32(target, *u_castP$((const u32*)(field)), u32_(16), u8_l("0123456789ABCDEF"));
        break;
    case u8_c('o'):
        claim_assert(size == draft_fmt__Size_default);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
        written = draft_fmt__writeU32(target, *u_castP$((const u32*)(field)), u32_(8), u8_l("01234567"));
        break;
    case u8_c('b'):
        claim_assert(size == draft_fmt__Size_default);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
        written = draft_fmt__writeU32(target, *u_castP$((const u32*)(field)), u32_(2), u8_l("01"));
        break;
    case u8_c('f'):
    case u8_c('F'):
        claim_assert(size == draft_fmt__Size_64);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(f64)));
        written = draft_fmt__writeF64(target, *u_castP$((const f64*)(field)));
        break;
    case u8_c('s'):
        claim_assert(size == draft_fmt__Size_default);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(S_const$u8)));
        written = draft_fmt__writeSliU8(target, *u_castP$((const S_const$u8*)(field)));
        break;
    default:
        claim_unreachable;
    }
    if (pad_opts.len == 0) return written;
    return draft_fmt__writePadded(out, S_prefix((A_ref$((S_const$u8)tmp))written), pad_opts);
};

$static fn_((draft_fmt__writePadded(S$u8 out, S_const$u8 content, S_const$u8 opts))(usize)) {
    let width = draft_fmt__parseWidth(opts);
    let_ignore = mem_copyBytes(out, content);
    if (content.len >= width) return content.len;
    let padding = width - content.len;
    for_(($r(0, padding))(i)) {
        *S_at((out)[content.len + i]) = u8_c(' ');
    } $end(for);
    return width;
};

$static fn_((draft_fmt__writeI32(S$u8 out, i32 value, O$u8 sign))(usize)) {
    var_(tmp, A$$(16, u8)) = A_zero();
    var pos = A_len(tmp);
    let negative = pri_lt(value, 0);
    var abs_val = negative ? as$(u32)(-value) : as$(u32)(value);
    if (isZero(abs_val)) *A_at((tmp)[--pos]) = u8_c('0');
    else {
        while (0 < abs_val) {
            *A_at((tmp)[--pos]) = u8_c('0') + (abs_val % 10);
            abs_val /= 10;
        }
    }
    if (negative) {
        *A_at((tmp)[--pos]) = u8_c('-');
    } else {
        if_some((sign)(ch)) {
            *A_at((tmp)[--pos]) = ch;
        };
    }
    let digits = A_suffix$((S_const$u8)(tmp)(pos));
    let_ignore = mem_copyBytes(out, digits);
    return digits.len;
};

$static fn_((draft_fmt__writeU32(S$u8 out, u32 value, u32 base, S_const$u8 digits))(usize)) {
    var_(tmp, A$$(40, u8)) = A_zero();
    var pos = A_len(tmp);
    var val = value;
    if (isZero(val)) *A_at((tmp)[--pos]) = u8_c('0');
    else {
        while (0 < val) {
            *A_at((tmp)[--pos]) = *S_at((digits)[val % base]);
            val /= base;
        }
    }
    let text = A_suffix$((S_const$u8)(tmp)(pos));
    let_ignore = mem_copyBytes(out, text);
    return text.len;
};

$static fn_((draft_fmt__writeF64(S$u8 out, f64 value))(usize)) {
    let negative = value < 0.0;
    let abs_val = negative ? -value : value;
    let int_part = as$(u32)(abs_val);
    let frac_part = as$(u32)((abs_val - as$(f64)(int_part)) * 1000000.0 + 0.5);
    var written = usize_(0);
    if (negative) *S_at((out)[written++]) = u8_c('-');
    written += draft_fmt__writeU32(S_suffix((out)written), int_part, u32_(10), u8_l("0123456789"));
    *S_at((out)[written++]) = u8_c('.');
    var_(frac, A$$(6, u8)) = A_zero();
    var pos = A_len(frac);
    var val = frac_part;
    for_(($r(0, A_len(frac)))(i)) {
        let_ignore = i;
        *A_at((frac)[--pos]) = u8_c('0') + (val % 10);
        val /= 10;
    } $end(for);
    let_ignore = mem_copyBytes(S_suffix((out)written), A_ref$((S_const$u8)frac));
    return written + A_len(frac);
};

$static fn_((draft_fmt__writeSliU8(S$u8 out, S_const$u8 value))(usize)) {
    let_ignore = mem_copyBytes(out, value);
    return value.len;
};

#define main_no_args pp_true
#define MAIN_NO_PRINT_ERR
#include "dh-main.h"
#include "dh/prl/va.h"
#include "dh/meta.h"
#include "dh/mem/common.h"
#include "dh/utf8.h"
#include "dh/ascii.h"
#include "dh/io/common.h"
#include <stdio.h>

// `%{'['[index]']'[[wrapped]type[size]][[alt]'('mode')'][':'[[fill]align][sign][width]['.'precision]]}`
// wrapped = '?' optional | '!' error result
// type_void = '0'
// type_bool = 'b' | 'B' upper
// type_uint = 'u' | 'U' upper
// type_iint = 'i' | 'I' upper
// type_int = type_uint | type_iint
// type_flt = 'f' | 'F' upper
// type_ptr = 'p' | 'P' upper
// type_ascii_ch = 'c'
// type_utf8_cp = 'C'
// type_str_z0 = 'z'
// type_str_s = 's'
// type_err = 'e'
// type_sizable = type_int | type_flt
// type_mode_usable = type_sizable | type_ptr
// type = type_void | type_bool | type_mode_usable | type_ascii_ch | type_utf8_cp | type_str_z0 | type_str_s | type_err
// size_common = ' ' | '32' | 'll' | '64'
// size_int = 'hh' | '8' | 'h' | '16' | size_common | 'l' | 'long' | 'z' | 'size'
// size_flt = size_common
// size = size_int | size_flt
// alt = '#'
// mode = ' ' auto | 'd' 10 | 'x' 16 | 'o' 8 | 'b' 2 | 'e' scientific
// sign = ' ' auto | '+' always | '-' only ngtv

#if UNUSED_CODE
u8_l("case1: %{[0]?Ulong(#x):_^+8} def");
u8_l("case1: %{[0]?Fl(#x):_^+8.6} def");
#endif /* UNUSED_CODE */

errset_((fmt_StructE)(
    fmt_MissingClosingBrace,
    fmt_UnexpectedEndFormat,
));
errset_((fmt_IdxArgE)(
    fmt_InvalidIdx,
    fmt_IdxOutOfBounds,
    fmt_TooFewArgs,
    fmt_TooManyArgs,
));
errset_((fmt_SpecE)(
    fmt_InvalidTypeSpec,
    fmt_InvalidSizeSpec,
    fmt_InvalidAlignSpec,
    fmt_InvalidWidthSpec,
));
errset_((fmt_FltE)(
    fmt_InvalidPrecisionSpec,
    fmt_InvalidFlt,
    fmt_FltDisabled,
));

typedef enum_((fmt_Size $fits($packed))(
    fmt_Size_8 = 0,
    fmt_Size_16 = 1,
    fmt_Size_32 = 2,
    fmt_Size_long = 3,
    fmt_Size_ptr = 4,
    fmt_Size_64 = 5
)) fmt_Size;
T_use_E$(fmt_Size);
$attr($must_check $inline_always)
$static fn_((fmt_Size_parse(S_const$u8 text))(E$fmt_Size));
$attr($maybe_unused $inline_always)
$static fn_((fmt_Size_strfy(fmt_Size size, bool short_form))(S_const$u8));

$attr($inline_always)
$static fn_((fmt_digitToChar(usize value, bool upper))(u8));
$attr($inline_always)
$static fn_((fmt_isMode(u8 ch))(bool));
$attr($inline_always)
$static fn_((fmt_isAlign(u8 ch))(bool));

typedef enum_((fmt_Align $fits($packed))(
    fmt_Align_left = 0,
    fmt_Align_center = 1,
    fmt_Align_right = 2
)) fmt_Align;
T_use_E$(fmt_Align);
$attr($must_check $inline_always)
$static fn_((fmt_Align_fromByte(u8 ch))(E$fmt_Align));
$attr($maybe_unused $inline_always)
$static fn_((fmt_Align_intoByte(fmt_Align align))(u8));


enum {
    fmt_max_arg_count = 16,
    fmt_max_foldable_steps = 16,
    fmt_max_foldable_event_scan_bytes = 32,
    fmt_max_foldable_body_scan_bytes = 32,
};
typedef variant_((fmt_EventPos $fits($packed))(
    (fmt_EventPos_placeholder, usize),
    (fmt_EventPos_escaped_percent_brace, usize),
)) fmt_EventPos;
T_use_O$(fmt_EventPos);

$attr($must_check $inline_always)
$static fn_((fmt_print(S$u8 out, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$S$u8));
$attr($must_check $inline_never $branch_cold)
$static fn_((fmt_printRuntime(S$u8 out, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$S$u8));

$static fn_((fmt_printPadded(S$u8 out, S_const$u8 text, u8 fill, fmt_Align align, usize width))(usize));
$static fn_((fmt_printVoid(S$u8 out, Void value))(usize));
$static fn_((fmt_printBool(S$u8 out, bool value, bool upper))(usize));
$static fn_((fmt_printU64(S$u8 out, u64 value, u8 mode, bool upper, bool alt))(usize));
$static fn_((fmt_printU64Base(S$u8 out, u64 value, usize base, bool upper))(usize));
$static fn_((fmt_printU64Dec(S$u8 out, u64 value))(usize));
$static fn_((fmt_printU64Hex(S$u8 out, u64 value, bool upper, bool alt))(usize));
$static fn_((fmt_printU64Oct(S$u8 out, u64 value, bool alt))(usize));
$static fn_((fmt_printU64Bin(S$u8 out, u64 value, bool upper, bool alt))(usize));
$static fn_((fmt_printI64(S$u8 out, i64 value, u8 mode, bool upper, bool alt, u8 sign))(usize));
$static fn_((fmt_printI64Dec(S$u8 out, i64 value, u8 sign))(usize));
$attr($must_check)
$static fn_((fmt_printF64(S$u8 out, f64 value, u8 mode, bool upper, bool alt, u8 sign, usize precision, bool has_precision))(E$usize));
$static fn_((fmt_printPtr(S$u8 out, P_const$raw ptr, u8 mode, bool upper, bool alt))(usize));
$attr($must_check)
$static fn_((fmt_printASCII(S$u8 out, u8 code))(E$usize));
$attr($must_check)
$static fn_((fmt_printUTF8(S$u8 out, u32 code))(E$usize));
$static fn_((fmt_printStrZ0(S$u8 out, P_const$u8 str_z0))(usize));
$static fn_((fmt_printStr(S$u8 out, S_const$u8 str))(usize));
$static fn_((fmt_printErr(S$u8 out, EAny err))(usize));

typedef struct fmt_Iter {
    var_(rest, S_const$u8);
    var_(occ_idx, usize);
    var_(done, bool);
} fmt_Iter;
$attr($inline_always)
$static fn_((fmt_Iter_init(S_const$u8 fmt))(fmt_Iter));
typedef variant_((fmt_Iter_Step $fits($packed))(
    (fmt_Iter_Step_written, usize),
    (fmt_Iter_Step_fallback, Void)
)) fmt_Iter_Step;
T_use_E$(fmt_Iter_Step);
$attr($must_check $inline_always)
$static fn_((fmt_Iter_nextOrFinish(fmt_Iter* iter, S$u8 out, S_const$TypeInfo fields, u_P_const$raw tuple))(E$fmt_Iter_Step));



fn_((main(void))(E$void) $scope) {
    var_(mem0, A$$(96, u8)) = A_zero();
    var_(mem1, A$$(160, u8)) = A_zero();
    var_(mem2, A$$(160, u8)) = A_zero();
    var_(mem3, A$$(160, u8)) = A_zero();
    var_(opt_i, O$i32) = some(123);
    var_(none_s, O$S_const$u8) = none$((O$S_const$u8));

    let s0 = try_(va_((fmt_print)(A_ref$((S$u8)mem0), u8_l("case0 {ok} %%{ %{s} %{i:+6}"))(u8_l("world"), 123)));
    let s1 = try_(va_((fmt_print)(A_ref$((S$u8)mem1), u8_l("case1 %{u(#x)} %{U(#x)} %{u(o)} %{u(b)} %{B}"))(u32_(42), u32_(42), u32_(42), u32_(42), false)));
    let s2 = try_(va_((fmt_print)(A_ref$((S$u8)mem2), u8_l("case2 %{[1]i:+6} %{[0]?i} %{?s} %{c}"))(opt_i, 77, none_s, u8_c('A'))));
    let s3 = try_(va_((fmt_print)(A_ref$((S$u8)mem3), u8_l("case3 %{[0]u} %{[0]i} %{[0]i(#x)} %{[0]u(#X)}"))(u32_(0xDEADBEEF))));

    puts(as$(const char*)(s0.ptr));
    puts(as$(const char*)(s1.ptr));
    puts(as$(const char*)(s2.ptr));
    puts(as$(const char*)(s3.ptr));
    return_ok({});
} $unscoped(fn);



$attr($inline_always)
$static fn_((fmt__findEvent(S_const$u8 fmt))(O$fmt_EventPos));
$attr($must_check)
$static fn_((fmt__findEventRuntime(S_const$u8 fmt))(O$fmt_EventPos));
$attr($must_check $inline_always)
$static fn_((fmt__findClose(S_const$u8 fmt))(E$usize));
$attr($must_check)
$static fn_((fmt__findCloseRuntime(S_const$u8 fmt))(E$usize));
$attr($inline_always)
$static fn_((fmt__copy(S$u8 out, S_const$u8 text))(usize));
$attr($must_check $inline_always)
$static fn_((fmt__argIdx(S_const$u8 body, usize occ_idx))(E$usize));

$attr($must_check $inline_always)
$static fn_((fmt__writeBody(S$u8 out, S_const$u8 body, u_P_const$raw field))(E$usize));
$attr($must_check $inline_always)
$static fn_((fmt__writeValue(S$u8 out, u8 wrapped, u8 type, fmt_Size size, u8 mode, bool alt, u8 sign, usize precision, bool has_precision, u_P_const$raw field))(E$usize));
$attr($must_check $inline_always)
$static fn_((fmt__writePlainValue(S$u8 out, u8 type, fmt_Size size, u8 mode, bool alt, u8 sign, usize precision, bool has_precision, u_P_const$raw field))(E$usize));



fn_((fmt_Size_parse(S_const$u8 text))(E$fmt_Size) $scope) {
    if (mem_eqlBytes(text, u8_l("hh")) || mem_eqlBytes(text, u8_l("8"))) return_ok(fmt_Size_8);
    if (mem_eqlBytes(text, u8_l("h")) || mem_eqlBytes(text, u8_l("16"))) return_ok(fmt_Size_16);
    if (mem_eqlBytes(text, u8_l("")) || mem_eqlBytes(text, u8_l("32"))) return_ok(fmt_Size_32);
    if (mem_eqlBytes(text, u8_l("l")) || mem_eqlBytes(text, u8_l("long"))) return_ok(fmt_Size_64);
    if (mem_eqlBytes(text, u8_l("z")) || mem_eqlBytes(text, u8_l("size"))) return_ok(fmt_Size_ptr);
    if (mem_eqlBytes(text, u8_l("ll")) || mem_eqlBytes(text, u8_l("64"))) return_ok(fmt_Size_64);
    return_err(E_cause$fmt_InvalidSizeSpec());
} $unscoped(fn);
fn_((fmt_Size_strfy(fmt_Size size, bool short_form))(S_const$u8)) {
    if (short_form) {
        switch (size) {
        case fmt_Size_8: return u8_l("hh");
        case fmt_Size_16: return u8_l("h");
        case fmt_Size_32: return u8_l("l");
        case fmt_Size_long: return u8_l("ll");
        case fmt_Size_64: return u8_l("z");
        case fmt_Size_ptr: return u8_l("size");
        default: claim_unreachable;
        }
    } else {
        switch (size) {
        case fmt_Size_8: return u8_l("8");
        case fmt_Size_16: return u8_l("16");
        case fmt_Size_32: return u8_l("32");
        case fmt_Size_long: return u8_l("long");
        case fmt_Size_64: return u8_l("64");
        case fmt_Size_ptr: return u8_l("size");
        default: claim_unreachable;
        }
    }
};

fn_((fmt_digitToChar(usize value, bool upper))(u8)) {
    return value < 10 ? u8_c('0') + as$(u8)(value) : (upper ? u8_c('A') : u8_c('a')) + as$(u8)(value - 10);
};
fn_((fmt_isMode(u8 ch))(bool)) {
    return ch == u8_c(' ') || ch == u8_c('d') || ch == u8_c('x') || ch == u8_c('X') || ch == u8_c('o') || ch == u8_c('b') || ch == u8_c('e');
};
fn_((fmt_isAlign(u8 ch))(bool)) {
    return ch == u8_c('<') || ch == u8_c('^') || ch == u8_c('>');
};

fn_((fmt_Align_fromByte(u8 ch))(E$fmt_Align) $scope) {
    if (ch == u8_c('<')) return_ok(fmt_Align_left);
    if (ch == u8_c('^')) return_ok(fmt_Align_center);
    if (ch == u8_c('>')) return_ok(fmt_Align_right);
    return_err(E_cause$fmt_InvalidAlignSpec());
} $unscoped(fn);
fn_((fmt_Align_intoByte(fmt_Align align))(u8)) {
    switch (align) {
    case fmt_Align_left: return u8_c('<');
    case fmt_Align_center: return u8_c('^');
    case fmt_Align_right: return u8_c('>');
    default: claim_unreachable;
    }
};


$attr($inline_always)
$static fn_((fmt_Iter_Step_mapWritten(fmt_Iter_Step step))(O$usize) $scope) {
    match_(step) {
    pattern_((fmt_Iter_Step_written)(written)) return_some(written) $end(pattern);
    pattern_((fmt_Iter_Step_fallback)($ignore)) return_none() $end(pattern);
    } $end(match);
} $unscoped(fn);

fn_((fmt_print(S$u8 out, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$S$u8) $scope) {
    if (!(isComptimeExpr(fmt.len) && (fmt.len == 0 || isComptimeExpr(*S_ptr(fmt))))) {
        return fmt_printRuntime(out, fmt, fields, tuple);
    }

    if (fields.len > fmt_max_arg_count) return_err(E_cause$fmt_TooManyArgs());
    claim_assert(TypeInfo_eql(tuple.type, u_typeInfoRecord(fields)));

    var buf = out;
    using_(var iter = fmt_Iter_init(fmt)) {
        let nextOrFinish = fmt_Iter_nextOrFinish;
        let mapWritten = fmt_Iter_Step_mapWritten;
        loop_inline_(for_)(($rt(usize_(fmt_max_foldable_steps)))($ignore)) {
            buf = S_suffix((buf)orelse_((mapWritten(try_(nextOrFinish(&iter, buf, fields, tuple))))(
                return fmt_printRuntime(out, fmt, fields, tuple)
            )));
        } $end(for);
        if_some((fmt__findEvent(iter.rest))(_event)) {
            let_ignore = _event;
            return fmt_printRuntime(out, fmt, fields, tuple);
        }
        if (iter.occ_idx < fields.len) return_err(E_cause$fmt_TooManyArgs());
        buf = S_suffix((buf)fmt__copy(buf, iter.rest));
    };
    return_ok(S_prefix((out)(out.len - buf.len)));
} $unscoped(fn);

fn_((fmt_printRuntime(S$u8 out, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$S$u8) $scope) {
    if (fields.len > fmt_max_arg_count) return_err(E_cause$fmt_TooManyArgs());
    claim_assert(TypeInfo_eql(tuple.type, u_typeInfoRecord(fields)));

    var buf = out;
    using_(var iter = fmt_Iter_init(fmt)) {
        while (true) {
            let event = orelse_((fmt__findEventRuntime(iter.rest))(break));
            let written_before = expr_(usize $scope)(match_(event) {
                patterns_((fmt_EventPos_placeholder, fmt_EventPos_escaped_percent_brace)(pos) {
                    $break_(fmt__copy(buf, S_prefix((iter.rest)pos)));
                }) $end(patterns);
            } $end(match)) $unscoped(expr);
            buf = S_suffix((buf)written_before);

            match_(event) {
            pattern_((fmt_EventPos_placeholder)(pos)) {
                let body_start = pos + 2;
                let close = try_(fmt__findCloseRuntime(S_suffix((iter.rest)body_start)));
                let body = S_prefix((S_suffix((iter.rest)body_start))close);
                let arg_idx = try_(fmt__argIdx(body, iter.occ_idx));
                if (arg_idx >= fields.len) return_err(E_cause$fmt_TooFewArgs());
                buf = S_suffix((buf)try_(fmt__writeBody(buf, body, u_fieldPtr(tuple, fields, arg_idx))));
                iter.rest = S_suffix((iter.rest)(body_start + close + 1));
                iter.occ_idx++;
            } $end(pattern);
            pattern_((fmt_EventPos_escaped_percent_brace)(pos)) {
                *S_at((buf)[0]) = u8_c('%');
                *S_at((buf)[1]) = u8_c('{');
                buf = S_suffix((buf)2);
                iter.rest = S_suffix((iter.rest)(pos + 3));
            } $end(pattern);
            } $end(match);
        }
        if (iter.occ_idx < fields.len) return_err(E_cause$fmt_TooManyArgs());
        buf = S_suffix((buf)fmt__copy(buf, iter.rest));
    };
    return_ok(S_prefix((out)(out.len - buf.len)));
} $unscoped(fn);

fn_((fmt_printPadded(S$u8 out, S_const$u8 text, u8 fill, fmt_Align align, usize width))(usize)) {
    if (text.len >= width) return fmt__copy(out, text);
    let pad = width - text.len;
    let left = align == fmt_Align_right ? pad : align == fmt_Align_center ? pad / 2
                                                                          : usize_(0);
    let right = pad - left;
    for (usize i = 0; i < left; ++i) *S_at((out)[i]) = fill;
    let_ignore = mem_copyBytes(S_suffix((out)left), text);
    for (usize i = 0; i < right; ++i) *S_at((out)[left + text.len + i]) = fill;
    return width;
};

fn_((fmt_printVoid(S$u8 out, Void value))(usize)) { return $ignore_void out, $ignore_void value, 0; };

fn_((fmt_printBool(S$u8 out, bool value, bool upper))(usize)) {
    return fmt__copy(out, value ? (upper ? u8_l("TRUE") : u8_l("true")) : (upper ? u8_l("FALSE") : u8_l("false")));
};

fn_((fmt_printU64(S$u8 out, u64 value, u8 mode, bool upper, bool alt))(usize)) {
    if (mode == u8_c('x') || mode == u8_c('X')) return fmt_printU64Hex(out, value, upper, alt);
    if (mode == u8_c('o')) return fmt_printU64Oct(out, value, alt);
    if (mode == u8_c('b')) return fmt_printU64Bin(out, value, upper, alt);
    if (mode == u8_c(' ') || mode == u8_c('d')) return fmt_printU64Dec(out, value);
    claim_unreachable;
};

fn_((fmt_printU64Base(S$u8 out, u64 value, usize base, bool upper))(usize)) {
    var_(tmp, A$$(72, u8)) = A_zero();
    var pos = A_len(tmp);
    if (value == 0) *A_at((tmp)[--pos]) = u8_c('0');
    while (value != 0) {
        *A_at((tmp)[--pos]) = fmt_digitToChar(value % base, upper);
        value /= base;
    }
    return fmt__copy(out, A_suffix$((S_const$u8)(tmp)(pos)));
};

fn_((fmt_printU64Dec(S$u8 out, u64 value))(usize)) {
    return fmt_printU64Base(out, value, 10, false);
};

fn_((fmt_printU64Hex(S$u8 out, u64 value, bool upper, bool alt))(usize)) {
    var written = usize_(0);
    if (alt) {
        *S_at((out)[written++]) = u8_c('0');
        *S_at((out)[written++]) = u8_c('x');
    }
    return written + fmt_printU64Base(S_suffix((out)written), value, 16, upper);
};

fn_((fmt_printU64Oct(S$u8 out, u64 value, bool alt))(usize)) {
    var written = usize_(0);
    if (alt) *S_at((out)[written++]) = u8_c('0');
    return written + fmt_printU64Base(S_suffix((out)written), value, 8, false);
};

fn_((fmt_printU64Bin(S$u8 out, u64 value, bool upper, bool alt))(usize)) {
    var written = usize_(0);
    if (alt) {
        *S_at((out)[written++]) = u8_c('0');
        *S_at((out)[written++]) = upper ? u8_c('B') : u8_c('b');
    }
    return written + fmt_printU64Base(S_suffix((out)written), value, 2, false);
};

fn_((fmt_printI64(S$u8 out, i64 value, u8 mode, bool upper, bool alt, u8 sign))(usize)) {
    if (mode == u8_c(' ') || mode == u8_c('d')) return fmt_printI64Dec(out, value, sign);

    var written = usize_(0);
    let neg = value < 0;
    if (neg) *S_at((out)[written++]) = u8_c('-');
    else if (sign == u8_c('+') || sign == u8_c(' ')) *S_at((out)[written++]) = sign;
    let magnitude = neg ? as$(u64)(-(value + 1)) + u64_(1) : as$(u64)(value);
    return written + fmt_printU64(S_suffix((out)written), magnitude, mode, upper, alt);
};

fn_((fmt_printI64Dec(S$u8 out, i64 value, u8 sign))(usize)) {
    var written = usize_(0);
    let neg = value < 0;
    if (neg) *S_at((out)[written++]) = u8_c('-');
    else if (sign == u8_c('+') || sign == u8_c(' ')) *S_at((out)[written++]) = sign;
    return written + fmt_printU64(S_suffix((out)written), neg ? as$(u64)(-value) : as$(u64)(value), u8_c('d'), false, false);
};

fn_((fmt_printF64(S$u8 out, f64 value, u8 mode, bool upper, bool alt, u8 sign, usize precision, bool has_precision))(E$usize) $scope) {
    let_ignore = upper;
    let_ignore = alt;
    if (!has_precision) precision = 6;
    if (mode == u8_c('e')) {
        var exp = i32_(0);
        var norm = value < 0.0 ? -value : value;
        while (norm >= 10.0) {
            norm /= 10.0;
            exp += 1;
        }
        while (norm != 0.0 && norm < 1.0) {
            norm *= 10.0;
            exp -= 1;
        }
        var written = try_(fmt_printF64(out, value < 0.0 ? -norm : norm, u8_c('d'), false, false, sign, precision, true));
        *S_at((out)[written++]) = u8_c('e');
        written += fmt_printI64Dec(S_suffix((out)written), exp, u8_c('+'));
        return_ok(written);
    }
    if (mode != u8_c(' ') && mode != u8_c('d')) return_err(E_cause$fmt_InvalidTypeSpec());
    let neg = value < 0.0;
    let abs_v = neg ? -value : value;
    var scale = u64_(1);
    for (usize i = 0; i < precision; ++i) scale *= 10;
    var whole = as$(u64)(abs_v);
    var frac = as$(u64)((abs_v - as$(f64)(whole)) * as$(f64)(scale) + 0.5);
    if (frac >= scale) {
        whole += 1;
        frac = 0;
    }
    var written = usize_(0);
    if (neg) *S_at((out)[written++]) = u8_c('-');
    else if (sign == u8_c('+') || sign == u8_c(' ')) *S_at((out)[written++]) = sign;
    written += fmt_printU64(S_suffix((out)written), whole, u8_c('d'), false, false);
    if (precision == 0) return_ok(written);
    *S_at((out)[written++]) = u8_c('.');
    var_(tmp, A$$(18, u8)) = A_zero();
    var pos = precision;
    for (usize i = 0; i < precision; ++i) {
        *A_at((tmp)[--pos]) = u8_c('0') + (frac % 10);
        frac /= 10;
    }
    return_ok(written + fmt__copy(S_suffix((out)written), A_prefix$((S_const$u8)(tmp)(precision))));
} $unscoped(fn);

fn_((fmt_printPtr(S$u8 out, P_const$raw ptr, u8 mode, bool upper, bool alt))(usize)) {
    return fmt_printU64(out, as$(u64)(ptrToInt(ptr)), mode == u8_c(' ') ? u8_c('x') : mode, upper, alt);
};

fn_((fmt_printASCII(S$u8 out, u8 code))(E$usize) $scope) {
    if (out.len < 1) return_err(E_cause$TooSmallBuffer());
    *S_at((out)[0]) = code;
    return_ok(1);
} $unscoped(fn);

fn_((fmt_printUTF8(S$u8 out, u32 code))(E$usize) $scope) {
    return_ok(try_(utf8_encode(code, out)).len);
} $unscoped(fn);

fn_((fmt_printStrZ0(S$u8 out, P_const$u8 str))(usize)) {
    return fmt__copy(out, mem_spanZ0$u8(str));
};

fn_((fmt_printStr(S$u8 out, S_const$u8 str))(usize)) {
    return fmt__copy(out, str);
};

fn_((fmt_printErr(S$u8 out, EAny err))(usize)) {
    var written = fmt__copy(out, E_strfy(&err));
    *S_at((out)[written++]) = u8_c(' ');
    *S_at((out)[written++]) = u8_c('(');
    written += fmt_printI64Dec(S_suffix((out)written), E_tag(&err), u8_c('-'));
    *S_at((out)[written++]) = u8_c(')');
    return written;
};



fn_((fmt_Iter_init(S_const$u8 fmt))(fmt_Iter)) {
    return (fmt_Iter){ .rest = fmt, .occ_idx = 0, .done = false };
};

fn_((fmt_Iter_nextOrFinish(fmt_Iter* iter, S$u8 out, S_const$TypeInfo fields, u_P_const$raw tuple))(E$fmt_Iter_Step) $scope) {
    if (iter->done) return_ok(union_of((fmt_Iter_Step_written)(0)));
    let event = orelse_((fmt__findEvent(iter->rest))({
        if (iter->rest.len > fmt_max_foldable_event_scan_bytes) return_ok(union_of((fmt_Iter_Step_fallback){}));
        let written = fmt__copy(out, iter->rest);
        iter->rest = S_suffix((iter->rest)iter->rest.len);
        iter->done = true;
        return_ok(union_of((fmt_Iter_Step_written)(written)));
    }));
    let written_before = expr_(usize $scope)(match_(event) {
        patterns_((fmt_EventPos_placeholder, fmt_EventPos_escaped_percent_brace)(pos) {
            $break_(fmt__copy(out, S_prefix((iter->rest)pos)));
        }) $end(patterns);
    } $end(match)) $unscoped(expr);
    let written_after = expr_(usize $scope)(match_(event) {
        pattern_((fmt_EventPos_placeholder)(pos)) {
            var written = written_before;
            let body_start = pos + 2;
            let close = try_(fmt__findClose(S_suffix((iter->rest)body_start)));
            let body = S_prefix((S_suffix((iter->rest)body_start))close);
            let arg_idx = try_(fmt__argIdx(body, iter->occ_idx));
            if (arg_idx >= fields.len) return_err(E_cause$fmt_TooFewArgs());
            written += try_(fmt__writeBody(S_suffix((out)written), body, u_fieldPtr(tuple, fields, arg_idx)));
            iter->rest = S_suffix((iter->rest)(body_start + close + 1));
            iter->occ_idx++;
            if (iter->rest.len == 0) iter->done = true;
            $break_(written);
        } $end(pattern);
        pattern_((fmt_EventPos_escaped_percent_brace)(pos)) {
            var written = written_before;
            *S_at((out)[written++]) = u8_c('%');
            *S_at((out)[written++]) = u8_c('{');
            iter->rest = S_suffix((iter->rest)(pos + 3));
            if (iter->rest.len == 0) iter->done = true;
            $break_(written);
        } $end(pattern);
    }$end(match)) $unscoped(expr);
    return_ok(union_of((fmt_Iter_Step_written)(written_after)));
} $unscoped(fn);

fn_((fmt__findEvent(S_const$u8 fmt))(O$fmt_EventPos) $scope) {
    loop_inline_(for_)(($rt(usize_(fmt_max_foldable_event_scan_bytes)))(pos)) {
        if (fmt.len > usize_(pos) + 2
            && *S_at((fmt)[usize_(pos)]) == u8_c('%')
            && *S_at((fmt)[usize_(pos) + 1]) == u8_c('%')
            && *S_at((fmt)[usize_(pos) + 2]) == u8_c('{')) {
            return_some(union_of((fmt_EventPos_escaped_percent_brace)(usize_(pos))));
        }
        if (fmt.len > usize_(pos) + 1
            && *S_at((fmt)[usize_(pos)]) == u8_c('%')
            && *S_at((fmt)[usize_(pos) + 1]) == u8_c('{')) {
            return_some(union_of((fmt_EventPos_placeholder)(usize_(pos))));
        }
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((fmt__findEventRuntime(S_const$u8 fmt))(O$fmt_EventPos) $scope) {
    for (usize pos = 0; pos + 1 < fmt.len; ++pos) {
        if (*S_at((fmt)[pos]) != u8_c('%')) continue;
        if (pos + 2 < fmt.len
            && *S_at((fmt)[pos + 1]) == u8_c('%')
            && *S_at((fmt)[pos + 2]) == u8_c('{')) {
            return_some(union_of((fmt_EventPos_escaped_percent_brace)(pos)));
        }
        if (*S_at((fmt)[pos + 1]) == u8_c('{')) {
            return_some(union_of((fmt_EventPos_placeholder)(pos)));
        }
    }
    return_none();
} $unscoped(fn);

fn_((fmt__findClose(S_const$u8 fmt))(E$usize) $scope) {
    loop_inline_(for_)(($rt(usize_(fmt_max_foldable_body_scan_bytes)))(pos)) {
        if (fmt.len > usize_(pos) && *S_at((fmt)[usize_(pos)]) == u8_c('}')) return_ok(usize_(pos));
    } $end(for);
    return_err(E_cause$fmt_MissingClosingBrace());
} $unscoped(fn);

fn_((fmt__findCloseRuntime(S_const$u8 fmt))(E$usize) $scope) {
    for (usize pos = 0; pos < fmt.len; ++pos) {
        if (*S_at((fmt)[pos]) == u8_c('}')) return_ok(pos);
    }
    return_err(E_cause$fmt_MissingClosingBrace());
} $unscoped(fn);

fn_((fmt__copy(S$u8 out, S_const$u8 text))(usize)) {
    let_ignore = mem_copyBytes(out, text);
    return text.len;
};

fn_((fmt__argIdx(S_const$u8 body, usize occ_idx))(E$usize) $scope) {
    if (body.len == 0) return_err(E_cause$fmt_UnexpectedEndFormat());
    if (*S_at((body)[0]) != u8_c('[')) return_ok(occ_idx);
    var pos = usize_(1);
    var idx = usize_(0);
    if (pos >= body.len || !ascii_isDigit(*S_at((body)[pos]))) return_err(E_cause$fmt_InvalidIdx());
    while (pos < body.len && ascii_isDigit(*S_at((body)[pos]))) {
        idx = idx * 10 + as$(usize)(*S_at((body)[pos]) - u8_c('0'));
        pos += 1;
    }
    if (pos >= body.len || *S_at((body)[pos]) != u8_c(']')) return_err(E_cause$fmt_InvalidIdx());
    if (idx >= fmt_max_arg_count) return_err(E_cause$fmt_IdxOutOfBounds());
    return_ok(idx);
} $unscoped(fn);

fn_((fmt__writeBody(S$u8 out, S_const$u8 body, u_P_const$raw field))(E$usize) $scope) {
    var pos = usize_(0);
    if (body.len == 0) return_err(E_cause$fmt_UnexpectedEndFormat());
    if (*S_at((body)[0]) == u8_c('[')) {
        pos = 1;
        while (pos < body.len && *S_at((body)[pos]) != u8_c(']')) pos += 1;
        if (pos >= body.len) return_err(E_cause$fmt_InvalidIdx());
        pos += 1;
    }
    var wrapped = u8_c('\0');
    if (pos < body.len && (*S_at((body)[pos]) == u8_c('?') || *S_at((body)[pos]) == u8_c('!'))) wrapped = *S_at((body)[pos++]);
    if (pos >= body.len) return_err(E_cause$fmt_UnexpectedEndFormat());
    let type = *S_at((body)[pos++]);
    var size_end = pos;
    while (size_end < body.len && *S_at((body)[size_end]) != u8_c('(') && *S_at((body)[size_end]) != u8_c(':')) size_end += 1;
    let size = try_(fmt_Size_parse(S_slice((body)$r(pos, size_end))));
    pos = size_end;
    var mode = u8_c(' ');
    var alt = false;
    if (pos < body.len && *S_at((body)[pos]) == u8_c('(')) {
        pos += 1;
        if (pos < body.len && *S_at((body)[pos]) == u8_c('#')) {
            alt = true;
            pos += 1;
        }
        if (pos >= body.len || !fmt_isMode(*S_at((body)[pos]))) return_err(E_cause$fmt_InvalidTypeSpec());
        mode = *S_at((body)[pos++]);
        if (pos >= body.len || *S_at((body)[pos++]) != u8_c(')')) return_err(E_cause$fmt_InvalidTypeSpec());
    }
    var fill = u8_c(' ');
    var_(align, fmt_Align) = fmt_Align_left;
    var sign = u8_c('-');
    var width = usize_(0);
    var precision = usize_(6);
    var has_precision = false;
    if (pos < body.len) {
        if (*S_at((body)[pos++]) != u8_c(':')) return_err(E_cause$fmt_InvalidTypeSpec());
        if (pos >= body.len) return_err(E_cause$fmt_InvalidWidthSpec());
        if (pos + 1 < body.len && fmt_isAlign(*S_at((body)[pos + 1]))) {
            fill = *S_at((body)[pos++]);
        } else if (!fmt_isAlign(*S_at((body)[pos])) && *S_at((body)[pos]) != u8_c('+') && *S_at((body)[pos]) != u8_c('-') && *S_at((body)[pos]) != u8_c(' ') && *S_at((body)[pos]) != u8_c('.') && !ascii_isDigit(*S_at((body)[pos]))) {
            return_err(E_cause$fmt_InvalidAlignSpec());
        }
        if (pos < body.len && fmt_isAlign(*S_at((body)[pos]))) {
            let align_ch = *S_at((body)[pos++]);
            align = try_(fmt_Align_fromByte(align_ch));
        }
        if (pos < body.len && (*S_at((body)[pos]) == u8_c('+') || *S_at((body)[pos]) == u8_c('-') || *S_at((body)[pos]) == u8_c(' '))) sign = *S_at((body)[pos++]);
        while (pos < body.len && ascii_isDigit(*S_at((body)[pos]))) {
            width = width * 10 + as$(usize)(*S_at((body)[pos++]) - u8_c('0'));
        }
        if (pos < body.len && *S_at((body)[pos]) == u8_c('.')) {
            pos += 1;
            if (pos >= body.len || !ascii_isDigit(*S_at((body)[pos]))) return_err(E_cause$fmt_InvalidPrecisionSpec());
            precision = 0;
            has_precision = true;
            while (pos < body.len && ascii_isDigit(*S_at((body)[pos]))) {
                precision = precision * 10 + as$(usize)(*S_at((body)[pos++]) - u8_c('0'));
            }
        }
        if (pos != body.len) return_err(E_cause$fmt_InvalidWidthSpec());
    }
    var_(tmp, A$$(128, u8)) = A_zero();
    let target = width == 0 ? out : A_ref$((S$u8)tmp);
    let written = try_(fmt__writeValue(target, wrapped, type, size, mode, alt, sign, precision, has_precision, field));
    if (width == 0) return_ok(written);
    return_ok(fmt_printPadded(out, S_prefix((A_ref$((S_const$u8)tmp))written), fill, align, width));
} $unscoped(fn);

fn_((fmt__writeValue(S$u8 out, u8 wrapped, u8 type, fmt_Size size, u8 mode, bool alt, u8 sign, usize precision, bool has_precision, u_P_const$raw field))(E$usize) $scope) {
    if (wrapped == u8_c('\0')) return fmt__writePlainValue(out, type, size, mode, alt, sign, precision, has_precision, field);
    if (wrapped == u8_c('?') && (type == u8_c('i') || type == u8_c('I'))) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(O$i32)));
        let value = *u_castP$((const O$i32*)(field));
        if_none((value)) return_ok(fmt__copy(out, u8_l("none")));
        else_some((payload)) {
            var inner = payload;
            return fmt__writePlainValue(out, type, size, mode, alt, sign, precision, has_precision, u_anyP(&inner).as_const);
        }
    }
    if (wrapped == u8_c('?') && type == u8_c('s')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(O$S_const$u8)));
        let value = *u_castP$((const O$S_const$u8*)(field));
        if_none((value)) return_ok(fmt__copy(out, u8_l("none")));
        else_some((payload)) {
            var inner = payload;
            return fmt__writePlainValue(out, type, size, mode, alt, sign, precision, has_precision, u_anyP(&inner).as_const);
        }
    }
    if (wrapped == u8_c('!') && (type == u8_c('i') || type == u8_c('I'))) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(E$i32)));
        let value = *u_castP$((const E$i32*)(field));
        if_err((value)(err)) return_ok(fmt_printErr(out, err));
        else_ok((payload)) {
            var inner = payload;
            return fmt__writePlainValue(out, type, size, mode, alt, sign, precision, has_precision, u_anyP(&inner).as_const);
        }
    }
    return_err(E_cause$fmt_InvalidTypeSpec());
} $unscoped(fn);

fn_((fmt__writePlainValue(S$u8 out, u8 type, fmt_Size size, u8 mode, bool alt, u8 sign, usize precision, bool has_precision, u_P_const$raw field))(E$usize) $scope) {
    let upper = type == u8_c('B') || type == u8_c('U') || type == u8_c('I') || type == u8_c('F') || type == u8_c('P') || mode == u8_c('X');
    if (type == u8_c('0')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(Void)));
        return_ok(fmt_printVoid(out, *u_castP$((const Void*)(field))));
    }
    if (type == u8_c('b') || type == u8_c('B')) {
        claim_assert(size == fmt_Size_32 && mode == u8_c(' ') && !alt);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(bool)));
        return_ok(fmt_printBool(out, *u_castP$((const bool*)(field)), upper));
    }
    if (type == u8_c('u') || type == u8_c('U')) {
        var value = u64_(0);
        if (size == fmt_Size_8) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(u8)));
            value = *u_castP$((const u8*)(field));
        } else if (size == fmt_Size_16) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(u16)));
            value = *u_castP$((const u16*)(field));
        } else if (size == fmt_Size_32) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
            value = *u_castP$((const u32*)(field));
        } else if (size == fmt_Size_ptr) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(usize)));
            value = *u_castP$((const usize*)(field));
        } else if (size == fmt_Size_64) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(u64)));
            value = *u_castP$((const u64*)(field));
        } else return_err(E_cause$fmt_InvalidSizeSpec());
        return_ok(fmt_printU64(out, value, mode, upper, alt));
    }
    if (type == u8_c('i') || type == u8_c('I')) {
        var value = i64_(0);
        if (size == fmt_Size_32) {
            if (TypeInfo_eql(field.type, typeInfo$(i32))) {
                value = *u_castP$((const i32*)(field));
            } else {
                claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
                value = as$(i32)(*u_castP$((const u32*)(field)));
            }
        } else if (size == fmt_Size_64) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(i64)));
            value = *u_castP$((const i64*)(field));
        } else return_err(E_cause$fmt_InvalidSizeSpec());
        return_ok(fmt_printI64(out, value, mode, upper, alt, sign));
    }
    if (type == u8_c('f') || type == u8_c('F')) {
        if (size == fmt_Size_32) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(f32)));
            return fmt_printF64(out, *u_castP$((const f32*)(field)), mode, upper, alt, sign, precision, has_precision);
        }
        claim_assert(size == fmt_Size_64);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(f64)));
        return fmt_printF64(out, *u_castP$((const f64*)(field)), mode, upper, alt, sign, precision, has_precision);
    }
    if (type == u8_c('p') || type == u8_c('P')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(P_const$raw)));
        return_ok(fmt_printPtr(out, *u_castP$((const P_const$raw*)(field)), mode, upper, alt));
    }
    if (type == u8_c('c')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u8)));
        return_ok(try_(fmt_printASCII(out, *u_castP$((const u8*)(field)))));
    }
    if (type == u8_c('C')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
        return_ok(try_(fmt_printUTF8(out, *u_castP$((const u32*)(field)))));
    }
    if (type == u8_c('z')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(P_const$u8)));
        return_ok(fmt_printStrZ0(out, *u_castP$((const P_const$u8*)(field))));
    }
    if (type == u8_c('s')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(S_const$u8)));
        return_ok(fmt_printStr(out, *u_castP$((const S_const$u8*)(field))));
    }
    if (type == u8_c('e')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(EAny)));
        return_ok(fmt_printErr(out, *u_castP$((const EAny*)(field))));
    }
    return_err(E_cause$fmt_InvalidTypeSpec());
} $unscoped(fn);

#define main_no_args pp_true
#define MAIN_NO_PRINT_ERR
#include "dh-main.h"
#include "dh/prl/va.h"
#include "dh/u-meta.h"
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
    fmt_Size_64 = 4,
    fmt_Size_ptr = 5
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

typedef struct fmt_LayoutSpec {
    var_(fill, u8);
    var_(align, fmt_Align);
    var_(width, usize);
} fmt_LayoutSpec;

typedef enum_((fmt_Case $fits($packed))(
    fmt_Case_lower = 0,
    fmt_Case_upper = 1
)) fmt_Case;

typedef enum_((fmt_IntMode $fits($packed))(
    fmt_IntMode_decimal = 0,
    fmt_IntMode_binary = 1,
    fmt_IntMode_octal = 2,
    fmt_IntMode_hex = 3
)) fmt_IntMode;

typedef enum_((fmt_FltMode $fits($packed))(
    fmt_FltMode_decimal = 0,
    fmt_FltMode_scientific = 1
)) fmt_FltMode;

typedef enum_((fmt_Sign $fits($packed))(
    fmt_Sign_auto = 0,
    fmt_Sign_always = 1,
    fmt_Sign_space = 2
)) fmt_Sign;

typedef struct fmt_BoolSpec {
    var_(layout, fmt_LayoutSpec);
    var_(case_, fmt_Case);
} fmt_BoolSpec;
typedef struct fmt_UIntSpec {
    var_(layout, fmt_LayoutSpec);
    var_(mode, fmt_IntMode);
    var_(case_, fmt_Case);
    var_(alt_form, bool);
} fmt_UIntSpec;
typedef struct fmt_IIntSpec {
    var_(layout, fmt_LayoutSpec);
    var_(mode, fmt_IntMode);
    var_(case_, fmt_Case);
    var_(sign, fmt_Sign);
    var_(alt_form, bool);
} fmt_IIntSpec;
typedef struct fmt_FltSpec {
    var_(layout, fmt_LayoutSpec);
    var_(mode, fmt_FltMode);
    var_(case_, fmt_Case);
    var_(sign, fmt_Sign);
    var_(alt_form, bool);
    var_(precision, usize);
    var_(has_precision, bool);
} fmt_FltSpec;
typedef struct fmt_PtrSpec {
    var_(layout, fmt_LayoutSpec);
    var_(case_, fmt_Case);
    var_(alt_form, bool);
} fmt_PtrSpec;
typedef struct fmt_CharSpec {
    var_(layout, fmt_LayoutSpec);
} fmt_CharSpec;
typedef struct fmt_StrSpec {
    var_(layout, fmt_LayoutSpec);
} fmt_StrSpec;
typedef struct fmt_ErrSpec {
    var_(layout, fmt_LayoutSpec);
} fmt_ErrSpec;

typedef variant_((fmt_Spec $fits($packed))(
    (fmt_Spec_void, Void),
    (fmt_Spec_bool, fmt_BoolSpec),
    (fmt_Spec_uint, fmt_UIntSpec),
    (fmt_Spec_iint, fmt_IIntSpec),
    (fmt_Spec_flt, fmt_FltSpec),
    (fmt_Spec_ptr, fmt_PtrSpec),
    (fmt_Spec_ascii, fmt_CharSpec),
    (fmt_Spec_utf8, fmt_CharSpec),
    (fmt_Spec_str_z0, fmt_StrSpec),
    (fmt_Spec_str, fmt_StrSpec),
    (fmt_Spec_err, fmt_ErrSpec)
)) fmt_Spec;
T_use_E$(fmt_Spec);

typedef struct fmt_ParsedBody {
    var_(wrapped, u8);
    var_(size, fmt_Size);
    var_(spec, fmt_Spec);
} fmt_ParsedBody;
T_use_E$(fmt_ParsedBody);

$attr($maybe_unused $inline_always)
$static fn_((fmt_LayoutSpec_default(void))(fmt_LayoutSpec));
$attr($maybe_unused $inline_always)
$static fn_((fmt_BoolSpec_default(void))(fmt_BoolSpec));
$attr($maybe_unused $inline_always)
$static fn_((fmt_UIntSpec_default(void))(fmt_UIntSpec));
$attr($maybe_unused $inline_always)
$static fn_((fmt_IIntSpec_default(void))(fmt_IIntSpec));
$attr($maybe_unused $inline_always)
$static fn_((fmt_FltSpec_default(void))(fmt_FltSpec));
$attr($maybe_unused $inline_always)
$static fn_((fmt_PtrSpec_default(void))(fmt_PtrSpec));
$attr($maybe_unused $inline_always)
$static fn_((fmt_CharSpec_default(void))(fmt_CharSpec));
$attr($maybe_unused $inline_always)
$static fn_((fmt_StrSpec_default(void))(fmt_StrSpec));
$attr($maybe_unused $inline_always)
$static fn_((fmt_ErrSpec_default(void))(fmt_ErrSpec));

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

struct io_Writer {
    var_(ctx, P$raw);
    $attr($must_check)
    fn_(((*writeFn)(P$raw ctx, S_const$u8 bytes))(E$usize));
};

$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_print(S$u8 out, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$S$u8));
$attr($must_check $inline_never $branch_cold)
$static fn_((fmt_printRuntime(S$u8 out, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$S$u8));
$attr($must_check $inline_always)
$static fn_((fmt_write(io_Writer writer, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$void));
$attr($must_check $inline_never $branch_cold)
$static fn_((fmt_writeRuntime(io_Writer writer, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$void));

$attr($maybe_unused $must_check $inline_always)
$static fn_((io_Writer_print(io_Writer writer, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$void));
$attr($maybe_unused $must_check $inline_always)
$static fn_((io_Writer_println(io_Writer writer, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$void));
$attr($maybe_unused $inline_always)
$static fn_((io_stream_print(S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(void));
$attr($maybe_unused $inline_always)
$static fn_((io_stream_println(S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(void));

$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatVoid(S$u8 out, Void value))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_format$Void(S$u8 out, Void value))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatBool(S$u8 out, bool value, fmt_BoolSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_format$bool(S$u8 out, bool value, fmt_BoolSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatUInt(S$u8 out, u64 value, fmt_UIntSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatIInt(S$u8 out, i64 value, fmt_IIntSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatFlt(S$u8 out, f64 value, fmt_FltSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatPtr(S$u8 out, P_const$raw value, fmt_PtrSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatASCII(S$u8 out, u8 value, fmt_CharSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatUTF8(S$u8 out, u32 value, fmt_CharSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatStrZ0(S$u8 out, P_const$u8 value, fmt_StrSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatStr(S$u8 out, S_const$u8 value, fmt_StrSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_formatErr(S$u8 out, EAny value, fmt_ErrSpec spec))(E$S$u8));

#define fmt__declareDirect(_$Type, _$Spec) \
    $attr($maybe_unused $must_check $inline_always) \
    $static fn_((pp_cat(fmt_format$, _$Type)(S$u8 out, _$Type value, _$Spec spec))(E$S$u8))
fmt__declareDirect(usize, fmt_UIntSpec);
fmt__declareDirect(u64, fmt_UIntSpec);
fmt__declareDirect(ulong, fmt_UIntSpec);
fmt__declareDirect(u32, fmt_UIntSpec);
fmt__declareDirect(u16, fmt_UIntSpec);
fmt__declareDirect(u8, fmt_UIntSpec);
fmt__declareDirect(isize, fmt_IIntSpec);
fmt__declareDirect(i64, fmt_IIntSpec);
fmt__declareDirect(ilong, fmt_IIntSpec);
fmt__declareDirect(i32, fmt_IIntSpec);
fmt__declareDirect(i16, fmt_IIntSpec);
fmt__declareDirect(i8, fmt_IIntSpec);
fmt__declareDirect(f64, fmt_FltSpec);
fmt__declareDirect(f32, fmt_FltSpec);
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_format$P$raw(S$u8 out, P_const$raw value, fmt_PtrSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_format$P$u8(S$u8 out, P_const$u8 value, fmt_StrSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_format$S$u8(S$u8 out, S_const$u8 value, fmt_StrSpec spec))(E$S$u8));
$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_format$EAny(S$u8 out, EAny value, fmt_ErrSpec spec))(E$S$u8));
#undef fmt__declareDirect

$attr($inline_never)
$static fn_((fmt_printPadded(S$u8 out, S_const$u8 text, u8 fill, fmt_Align align, usize width))(usize));
$static fn_((fmt_printVoid(S$u8 out, Void value))(usize));
$attr($inline_never)
$static fn_((fmt_printBool(S$u8 out, bool value, bool upper))(usize));
$attr($inline_never)
$static fn_((fmt_printU64(S$u8 out, u64 value, u8 mode, bool upper, bool alt))(usize));
$static fn_((fmt_printU64Base(S$u8 out, u64 value, usize base, bool upper))(usize));
$static fn_((fmt_printU64Dec(S$u8 out, u64 value))(usize));
$static fn_((fmt_printU64Hex(S$u8 out, u64 value, bool upper, bool alt))(usize));
$static fn_((fmt_printU64Oct(S$u8 out, u64 value, bool alt))(usize));
$static fn_((fmt_printU64Bin(S$u8 out, u64 value, bool upper, bool alt))(usize));
$attr($inline_never)
$static fn_((fmt_printI64(S$u8 out, i64 value, u8 mode, bool upper, bool alt, u8 sign))(usize));
$static fn_((fmt_printI64Dec(S$u8 out, i64 value, u8 sign))(usize));
$attr($must_check $inline_never)
$static fn_((fmt_printF64(S$u8 out, f64 value, u8 mode, bool upper, bool alt, u8 sign, usize precision, bool has_precision))(E$usize));
$attr($inline_never)
$static fn_((fmt_printPtr(S$u8 out, P_const$raw ptr, u8 mode, bool upper, bool alt))(usize));
$attr($must_check $inline_never)
$static fn_((fmt_printASCII(S$u8 out, u8 code))(E$usize));
$attr($must_check $inline_never)
$static fn_((fmt_printUTF8(S$u8 out, u32 code))(E$usize));
$attr($inline_never)
$static fn_((fmt_printStrZ0(S$u8 out, P_const$u8 str_z0))(usize));
$attr($inline_never)
$static fn_((fmt_printStr(S$u8 out, S_const$u8 str))(usize));
$attr($inline_never)
$static fn_((fmt_printErr(S$u8 out, EAny err))(usize));

typedef struct fmt_Iter {
    var_(rest, S_const$u8);
    var_(occ_idx, usize);
    var_(done, bool);
} fmt_Iter;

typedef enum_((fmt_FoldKind $fits($packed))(
    fmt_FoldKind_none = 0,
    fmt_FoldKind_tail = 1,
    fmt_FoldKind_placeholder = 2,
    fmt_FoldKind_escaped_percent_brace = 3
)) fmt_FoldKind;
typedef struct fmt_FoldStep {
    var_(kind, fmt_FoldKind);
    var_(literal_start, usize);
    var_(literal_len, usize);
    var_(body_start, usize);
    var_(body_len, usize);
    var_(occ_idx, usize);
} fmt_FoldStep;
$attr($inline_always)
$static fn_((fmt__buildFoldPlan(S_const$u8 fmt, fmt_FoldStep* plan))(bool));

$attr($inline_always)
$static fn_((fmt_Iter_init(S_const$u8 fmt))(fmt_Iter));
$attr($must_check $inline_never)
$static fn_((fmt__writerWriteAll(io_Writer writer, S_const$u8 bytes))(E$void));
$attr($must_check $inline_never)
$static fn_((fmt__writerWritePadded(io_Writer writer, S_const$u8 text, fmt_LayoutSpec layout))(E$void));
$attr($must_check $inline_always)
$static fn_((fmt__writeBodyToWriter(io_Writer writer, S_const$u8 body, u_P_const$raw field))(E$void));
$attr($must_check $inline_never $branch_cold)
$static fn_((fmt__writeRuntimeFrom(io_Writer writer, fmt_Iter iter, S_const$TypeInfo fields, u_P_const$raw tuple))(E$void));
$attr($must_check $inline_never)
$static fn_((io_stream__write(P$raw ctx, S_const$u8 bytes))(E$usize));

typedef struct fmt_TestWriter {
    var_(buf, S$u8);
    var_(written, usize);
} fmt_TestWriter;
$static volatile bool fmt_test_runtime_format = false;
$attr($must_check $inline_never)
$static fn_((fmt_TestWriter_write(P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    let self = ptrAlignCast$((fmt_TestWriter*)(ctx));
    if (self->buf.len - self->written < bytes.len) return_err(E_cause$TooSmallBuffer());
    let_ignore = mem_copyBytes(S_slice((self->buf)$r(self->written, self->written + bytes.len)), bytes);
    self->written += bytes.len;
    return_ok(bytes.len);
} $unscoped(fn);

$attr($must_check $inline_never)
$static fn_((fmt_test_constantWriter(io_Writer writer, S_const$u8 text, i32 value))(E$void)) {
    return va_((io_Writer_println)(writer, u8_l("writer %{s} %{i:+6}"))(text, value));
}

$attr($must_check $inline_never)
$static fn_((fmt_test_runtimeWriter(io_Writer writer, S_const$u8 fmt, u32 value))(E$void)) {
    return va_((io_Writer_print)(writer, fmt)(value));
}

$attr($must_check $inline_never)
$static fn_((fmt_test_constantBuffer(S$u8 out, u32 value))(E$S$u8)) {
    return va_((fmt_print)(out, u8_l("buffer %{u(#x)}"))(value));
}

$attr($must_check $inline_never)
$static fn_((fmt_test_runtimeBuffer(S$u8 out, S_const$u8 fmt, u32 value))(E$S$u8)) {
    return va_((fmt_print)(out, fmt)(value));
}

$attr($inline_never)
$static fn_((fmt_test_constantStream(u32 value, bool flag))(void)) {
    va_((io_stream_println)(u8_l("stream %{U(#x)} %{B}"))(value, flag));
}


fn_((main(void))(E$void) $scope) {
    var_(mem0, A$$(96, u8)) = A_zero();
    var_(mem1, A$$(160, u8)) = A_zero();
    var_(mem2, A$$(160, u8)) = A_zero();
    var_(mem3, A$$(160, u8)) = A_zero();
    var_(mem4, A$$(32, u8)) = A_zero();
    var_(mem5, A$$(128, u8)) = A_zero();
    var_(mem6, A$$(64, u8)) = A_zero();
    var_(mem7, A$$(64, u8)) = A_zero();
    var_(mem8, A$$(64, u8)) = A_zero();
    var_(mem9, A$$(64, u8)) = A_zero();
    var_(long_mem, A$$(512, u8)) = A_zero();
    var_(long_text, A$$(300, u8)) = A_zero();
    var_(invalid_mem, A$$(32, u8)) = A_zero();
    var_(opt_i, O$i32) = some(123);
    var_(none_s, O$S_const$u8) = none$((O$S_const$u8));

    let s0 = try_(va_((fmt_print)(A_ref$((S$u8)mem0), u8_l("case0 {ok} %%{ %{s} %{i:+6}"))(u8_l("world"), 123)));
    let s1 = try_(va_((fmt_print)(A_ref$((S$u8)mem1), u8_l("case1 %{u(#x)} %{U(#x)} %{u(o)} %{u(b)} %{B}"))(u32_(42), u32_(42), u32_(42), u32_(42), false)));
    let s2 = try_(va_((fmt_print)(A_ref$((S$u8)mem2), u8_l("case2 %{[1]i:+6} %{[0]?i} %{?s} %{c}"))(opt_i, 77, none_s, u8_c('A'))));
    let s3 = try_(va_((fmt_print)(A_ref$((S$u8)mem3), u8_l("case3 %{[0]u} %{[0]i} %{[0]i(#x)} %{[0]u(#X)}"))(u32_(0xDEADBEEF))));
    let s7 = try_(va_((fmt_print)(A_ref$((S$u8)mem7), u8_l("float %{f64:.2}"))(f64_(3.14159))));
    let s8 = try_(fmt_test_constantBuffer(A_ref$((S$u8)mem8), u32_(42)));
    let invalid_int_precision = va_((fmt_print)(
        A_ref$((S$u8)invalid_mem),
        u8_l("%{i:.2}")
    )(i32_(7)));
    if_err((invalid_int_precision)(err)) {
        let expected = E_cause$fmt_InvalidTypeSpec();
        claim_assert(E_eql(&err, expected.as_any));
    } else_ok((unexpected)) {
        let_ignore = unexpected;
        claim_unreachable;
    }
    var spec = fmt_IIntSpec_default();
    spec.layout.fill = u8_c('_');
    spec.layout.align = fmt_Align_right;
    spec.sign = fmt_Sign_always;
    spec.layout.width = 8;
    let s4 = try_(fmt_format$i16(A_ref$((S$u8)mem4), i16_(-42), spec));
    var test_writer = (fmt_TestWriter){ .buf = A_ref$((S$u8)mem5), .written = 0 };
    let writer = (io_Writer){ .ctx = ptrCast$((P$raw)(&test_writer)), .writeFn = fmt_TestWriter_write };
    try_(fmt_test_constantWriter(writer, u8_l("world"), i32_(123)));
    claim_assert(mem_eqlBytes(
        A_prefix$((S_const$u8)(mem5)(test_writer.written)),
        u8_l("writer world +123  " io_nl)
    ));
    var runtime_writer = (fmt_TestWriter){ .buf = A_ref$((S$u8)mem6), .written = 0 };
    let runtime_io = (io_Writer){ .ctx = ptrCast$((P$raw)(&runtime_writer)), .writeFn = fmt_TestWriter_write };
    let runtime_fmt = fmt_test_runtime_format ? u8_l("alternate %{u}") : u8_l("runtime %{u}");
    try_(fmt_test_runtimeWriter(runtime_io, runtime_fmt, u32_(7)));
    let s9 = try_(fmt_test_runtimeBuffer(A_ref$((S$u8)mem9), runtime_fmt, u32_(7)));
    claim_assert(mem_eqlBytes(s8.as_const, u8_l("buffer 0x2a")));
    claim_assert(mem_eqlBytes(s9.as_const, u8_l("runtime 7")));
    claim_assert(mem_eqlBytes(
        A_prefix$((S_const$u8)(mem6)(runtime_writer.written)),
        u8_l("runtime 7")
    ));
    for (usize i = 0; i < 280; ++i) *A_at((long_text)[i]) = u8_c('x');
    var long_writer = (fmt_TestWriter){ .buf = A_ref$((S$u8)long_mem), .written = 0 };
    let long_io = (io_Writer){ .ctx = ptrCast$((P$raw)(&long_writer)), .writeFn = fmt_TestWriter_write };
    try_(va_((io_Writer_print)(long_io, u8_l("long %{s:>300}"))(
        A_prefix$((S_const$u8)(long_text)(280))
    )));
    claim_assert(long_writer.written == 305);
    claim_assert(*A_at((long_mem)[5]) == u8_c(' '));
    claim_assert(*A_at((long_mem)[24]) == u8_c(' '));
    claim_assert(*A_at((long_mem)[25]) == u8_c('x'));
    claim_assert(*A_at((long_mem)[304]) == u8_c('x'));

    puts(as$(const char*)(s0.ptr));
    puts(as$(const char*)(s1.ptr));
    puts(as$(const char*)(s2.ptr));
    puts(as$(const char*)(s3.ptr));
    puts(as$(const char*)(s4.ptr));
    puts(as$(const char*)(s7.ptr));
    puts(as$(const char*)(s8.ptr));
    puts(as$(const char*)(s9.ptr));
    puts(as$(const char*)(A_ref$((S_const$u8)mem5).ptr));
    puts(as$(const char*)(A_ref$((S_const$u8)mem6).ptr));
    fmt_test_constantStream(u32_(42), false);
    return_ok({});
} $unscoped(fn);



$attr($must_check)
$static fn_((fmt__findEventRuntime(S_const$u8 fmt))(O$fmt_EventPos));
$attr($must_check)
$static fn_((fmt__findCloseRuntime(S_const$u8 fmt))(E$usize));
$attr($inline_always)
$static fn_((fmt__copy(S$u8 out, S_const$u8 text))(usize));
$attr($must_check $inline_always)
$static fn_((fmt__argIdx(S_const$u8 body, usize occ_idx))(E$usize));

$attr($must_check $inline_always)
$static fn_((fmt__writeBody(S$u8 out, S_const$u8 body, u_P_const$raw field))(E$usize));
$attr($must_check $inline_always)
$static fn_((fmt__parseBody(S_const$u8 body))(E$fmt_ParsedBody));
$attr($must_check $inline_always)
$static fn_((fmt__parseSpec(u8 type, u8 mode, bool alt, u8 sign, bool has_sign, fmt_LayoutSpec layout, usize precision, bool has_precision))(E$fmt_Spec));
$attr($inline_always)
$static fn_((fmt__specLayout(fmt_Spec spec))(fmt_LayoutSpec));
$attr($must_check $inline_always)
$static fn_((fmt__writeValue(S$u8 out, u8 wrapped, fmt_Size size, fmt_Spec spec, u_P_const$raw field))(E$usize));
$attr($must_check $inline_always)
$static fn_((fmt__writePlainValue(S$u8 out, fmt_Size size, fmt_Spec spec, u_P_const$raw field))(E$usize));
$attr($must_check $inline_always)
$static fn_((fmt__formatTyped(S$u8 out, fmt_Size size, u_P_const$raw value, fmt_Spec spec))(E$S$u8));



fn_((fmt_LayoutSpec_default(void))(fmt_LayoutSpec)) {
    return (fmt_LayoutSpec){
        .fill = u8_c(' '),
        .align = fmt_Align_left,
        .width = 0,
    };
}
fn_((fmt_BoolSpec_default(void))(fmt_BoolSpec)) {
    return (fmt_BoolSpec){ .layout = fmt_LayoutSpec_default(), .case_ = fmt_Case_lower };
}
fn_((fmt_UIntSpec_default(void))(fmt_UIntSpec)) {
    return (fmt_UIntSpec){
        .layout = fmt_LayoutSpec_default(),
        .mode = fmt_IntMode_decimal,
        .case_ = fmt_Case_lower,
        .alt_form = false,
    };
}
fn_((fmt_IIntSpec_default(void))(fmt_IIntSpec)) {
    return (fmt_IIntSpec){
        .layout = fmt_LayoutSpec_default(),
        .mode = fmt_IntMode_decimal,
        .case_ = fmt_Case_lower,
        .sign = fmt_Sign_auto,
        .alt_form = false,
    };
}
fn_((fmt_FltSpec_default(void))(fmt_FltSpec)) {
    return (fmt_FltSpec){
        .layout = fmt_LayoutSpec_default(),
        .mode = fmt_FltMode_decimal,
        .case_ = fmt_Case_lower,
        .sign = fmt_Sign_auto,
        .alt_form = false,
        .precision = 0,
        .has_precision = false,
    };
}
fn_((fmt_PtrSpec_default(void))(fmt_PtrSpec)) {
    return (fmt_PtrSpec){
        .layout = fmt_LayoutSpec_default(),
        .case_ = fmt_Case_lower,
        .alt_form = true,
    };
}
fn_((fmt_CharSpec_default(void))(fmt_CharSpec)) {
    return (fmt_CharSpec){ .layout = fmt_LayoutSpec_default() };
}
fn_((fmt_StrSpec_default(void))(fmt_StrSpec)) {
    return (fmt_StrSpec){ .layout = fmt_LayoutSpec_default() };
}
fn_((fmt_ErrSpec_default(void))(fmt_ErrSpec)) {
    return (fmt_ErrSpec){ .layout = fmt_LayoutSpec_default() };
}

fn_((fmt_formatVoid(S$u8 out, Void value))(E$S$u8)) {
    var_(spec, fmt_Spec) = union_of((fmt_Spec_void){});
    return fmt__formatTyped(out, fmt_Size_32, u_anyP(&value).as_const, spec);
}
fn_((fmt_format$Void(S$u8 out, Void value))(E$S$u8)) {
    return fmt_formatVoid(out, value);
}
fn_((fmt_formatBool(S$u8 out, bool value, fmt_BoolSpec spec))(E$S$u8)) {
    var_(typed, fmt_Spec) = union_of((fmt_Spec_bool)(spec));
    return fmt__formatTyped(out, fmt_Size_32, u_anyP(&value).as_const, typed);
}
fn_((fmt_format$bool(S$u8 out, bool value, fmt_BoolSpec spec))(E$S$u8)) {
    return fmt_formatBool(out, value, spec);
}
fn_((fmt_formatUInt(S$u8 out, u64 value, fmt_UIntSpec spec))(E$S$u8)) {
    var_(typed, fmt_Spec) = union_of((fmt_Spec_uint)(spec));
    return fmt__formatTyped(out, fmt_Size_64, u_anyP(&value).as_const, typed);
}
fn_((fmt_formatIInt(S$u8 out, i64 value, fmt_IIntSpec spec))(E$S$u8)) {
    var_(typed, fmt_Spec) = union_of((fmt_Spec_iint)(spec));
    return fmt__formatTyped(out, fmt_Size_64, u_anyP(&value).as_const, typed);
}
fn_((fmt_formatFlt(S$u8 out, f64 value, fmt_FltSpec spec))(E$S$u8)) {
    var_(typed, fmt_Spec) = union_of((fmt_Spec_flt)(spec));
    return fmt__formatTyped(out, fmt_Size_64, u_anyP(&value).as_const, typed);
}
fn_((fmt_formatPtr(S$u8 out, P_const$raw value, fmt_PtrSpec spec))(E$S$u8)) {
    var_(typed, fmt_Spec) = union_of((fmt_Spec_ptr)(spec));
    return fmt__formatTyped(out, fmt_Size_ptr, u_anyP(&value).as_const, typed);
}
fn_((fmt_formatASCII(S$u8 out, u8 value, fmt_CharSpec spec))(E$S$u8)) {
    var_(typed, fmt_Spec) = union_of((fmt_Spec_ascii)(spec));
    return fmt__formatTyped(out, fmt_Size_8, u_anyP(&value).as_const, typed);
}
fn_((fmt_formatUTF8(S$u8 out, u32 value, fmt_CharSpec spec))(E$S$u8)) {
    var_(typed, fmt_Spec) = union_of((fmt_Spec_utf8)(spec));
    return fmt__formatTyped(out, fmt_Size_32, u_anyP(&value).as_const, typed);
}
fn_((fmt_formatStrZ0(S$u8 out, P_const$u8 value, fmt_StrSpec spec))(E$S$u8)) {
    var_(typed, fmt_Spec) = union_of((fmt_Spec_str_z0)(spec));
    return fmt__formatTyped(out, fmt_Size_ptr, u_anyP(&value).as_const, typed);
}
fn_((fmt_formatStr(S$u8 out, S_const$u8 value, fmt_StrSpec spec))(E$S$u8)) {
    var_(typed, fmt_Spec) = union_of((fmt_Spec_str)(spec));
    return fmt__formatTyped(out, fmt_Size_ptr, u_anyP(&value).as_const, typed);
}
fn_((fmt_formatErr(S$u8 out, EAny value, fmt_ErrSpec spec))(E$S$u8)) {
    var_(typed, fmt_Spec) = union_of((fmt_Spec_err)(spec));
    return fmt__formatTyped(out, fmt_Size_ptr, u_anyP(&value).as_const, typed);
}

#define fmt__defineDirect(_$Type, _$Size, _$SpecType, _$SpecTag) \
    fn_((pp_cat(fmt_format$, _$Type)(S$u8 out, _$Type value, _$SpecType spec))(E$S$u8)) { \
        var_(typed, fmt_Spec) = union_of((_$SpecTag)(spec)); \
        return fmt__formatTyped(out, _$Size, u_anyP(&value).as_const, typed); \
    }
fmt__defineDirect(usize, fmt_Size_ptr, fmt_UIntSpec, fmt_Spec_uint)
fmt__defineDirect(u64, fmt_Size_64, fmt_UIntSpec, fmt_Spec_uint)
fmt__defineDirect(ulong, fmt_Size_long, fmt_UIntSpec, fmt_Spec_uint)
fmt__defineDirect(u32, fmt_Size_32, fmt_UIntSpec, fmt_Spec_uint)
fmt__defineDirect(u16, fmt_Size_16, fmt_UIntSpec, fmt_Spec_uint)
fmt__defineDirect(u8, fmt_Size_8, fmt_UIntSpec, fmt_Spec_uint)
fmt__defineDirect(isize, fmt_Size_ptr, fmt_IIntSpec, fmt_Spec_iint)
fmt__defineDirect(i64, fmt_Size_64, fmt_IIntSpec, fmt_Spec_iint)
fmt__defineDirect(ilong, fmt_Size_long, fmt_IIntSpec, fmt_Spec_iint)
fmt__defineDirect(i32, fmt_Size_32, fmt_IIntSpec, fmt_Spec_iint)
fmt__defineDirect(i16, fmt_Size_16, fmt_IIntSpec, fmt_Spec_iint)
fmt__defineDirect(i8, fmt_Size_8, fmt_IIntSpec, fmt_Spec_iint)
fmt__defineDirect(f64, fmt_Size_64, fmt_FltSpec, fmt_Spec_flt)
fmt__defineDirect(f32, fmt_Size_32, fmt_FltSpec, fmt_Spec_flt)
#undef fmt__defineDirect

fn_((fmt_format$P$raw(S$u8 out, P_const$raw value, fmt_PtrSpec spec))(E$S$u8)) {
    return fmt_formatPtr(out, value, spec);
}
fn_((fmt_format$P$u8(S$u8 out, P_const$u8 value, fmt_StrSpec spec))(E$S$u8)) {
    return fmt_formatStrZ0(out, value, spec);
}
fn_((fmt_format$S$u8(S$u8 out, S_const$u8 value, fmt_StrSpec spec))(E$S$u8)) {
    return fmt_formatStr(out, value, spec);
}
fn_((fmt_format$EAny(S$u8 out, EAny value, fmt_ErrSpec spec))(E$S$u8)) {
    return fmt_formatErr(out, value, spec);
}

fn_((fmt_Size_parse(S_const$u8 text))(E$fmt_Size) $scope) {
    if (mem_eqlBytes(text, u8_l("hh")) || mem_eqlBytes(text, u8_l("8"))) return_ok(fmt_Size_8);
    if (mem_eqlBytes(text, u8_l("h")) || mem_eqlBytes(text, u8_l("16"))) return_ok(fmt_Size_16);
    if (mem_eqlBytes(text, u8_l("")) || mem_eqlBytes(text, u8_l("32"))) return_ok(fmt_Size_32);
    if (mem_eqlBytes(text, u8_l("l")) || mem_eqlBytes(text, u8_l("long"))) return_ok(fmt_Size_long);
    if (mem_eqlBytes(text, u8_l("ll")) || mem_eqlBytes(text, u8_l("64"))) return_ok(fmt_Size_64);
    if (mem_eqlBytes(text, u8_l("z")) || mem_eqlBytes(text, u8_l("size"))) return_ok(fmt_Size_ptr);
    return_err(E_cause$fmt_InvalidSizeSpec());
} $unscoped(fn);
fn_((fmt_Size_strfy(fmt_Size size, bool short_form))(S_const$u8)) {
    if (short_form) {
        switch (size) {
        case fmt_Size_8: return u8_l("hh");
        case fmt_Size_16: return u8_l("h");
        case fmt_Size_32: return u8_l("");
        case fmt_Size_long: return u8_l("l");
        case fmt_Size_64: return u8_l("ll");
        case fmt_Size_ptr: return u8_l("z");
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

fn_((fmt__buildFoldPlan(S_const$u8 fmt, fmt_FoldStep* plan))(bool)) {
    var cursor = usize_(0);
    var occ_idx = usize_(0);
    loop_inline_(for_)(($rt(usize_(fmt_max_foldable_steps)))(step_idx)) {
        var event_rel = usize_(0);
        var_(event_kind, fmt_FoldKind) = fmt_FoldKind_none;
        var found = false;
        loop_inline_(for_)(($rt(usize_(fmt_max_foldable_event_scan_bytes)))(scan_idx)) {
            let pos = cursor + scan_idx;
            if (!found && pos + 2 < fmt.len
                && *S_at((fmt)[pos]) == u8_c('%')
                && *S_at((fmt)[pos + 1]) == u8_c('%')
                && *S_at((fmt)[pos + 2]) == u8_c('{')) {
                event_rel = scan_idx;
                event_kind = fmt_FoldKind_escaped_percent_brace;
                found = true;
            } else if (!found && pos + 1 < fmt.len
                && *S_at((fmt)[pos]) == u8_c('%')
                && *S_at((fmt)[pos + 1]) == u8_c('{')) {
                event_rel = scan_idx;
                event_kind = fmt_FoldKind_placeholder;
                found = true;
            }
        } $end(for);

        if (!found) {
            if (fmt.len - cursor > fmt_max_foldable_event_scan_bytes) return false;
            plan[step_idx] = (fmt_FoldStep){
                .kind = fmt_FoldKind_tail,
                .literal_start = cursor,
                .literal_len = fmt.len - cursor,
                .body_start = 0,
                .body_len = 0,
                .occ_idx = occ_idx,
            };
            return true;
        }

        let event_pos = cursor + event_rel;
        if (event_kind == fmt_FoldKind_escaped_percent_brace) {
            plan[step_idx] = (fmt_FoldStep){
                .kind = event_kind,
                .literal_start = cursor,
                .literal_len = event_pos - cursor,
                .body_start = 0,
                .body_len = 0,
                .occ_idx = occ_idx,
            };
            cursor = event_pos + 3;
            continue;
        }

        let body_start = event_pos + 2;
        var close_rel = usize_(0);
        var close_found = false;
        loop_inline_(for_)(($rt(usize_(fmt_max_foldable_body_scan_bytes)))(scan_idx)) {
            if (!close_found && body_start + scan_idx < fmt.len
                && *S_at((fmt)[body_start + scan_idx]) == u8_c('}')) {
                close_rel = scan_idx;
                close_found = true;
            }
        } $end(for);
        if (!close_found) return false;
        plan[step_idx] = (fmt_FoldStep){
            .kind = fmt_FoldKind_placeholder,
            .literal_start = cursor,
            .literal_len = event_pos - cursor,
            .body_start = body_start,
            .body_len = close_rel,
            .occ_idx = occ_idx,
        };
        cursor = body_start + close_rel + 1;
        occ_idx += 1;
    } $end(for);
    return false;
};

fn_((fmt_print(S$u8 out, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$S$u8) $scope) {
    if (!(isComptimeExpr(fmt.len)
        && (fmt.len == 0 || isComptimeExpr(*S_ptr(fmt))))) {
        return fmt_printRuntime(out, fmt, fields, tuple);
    }

    if (fields.len > fmt_max_arg_count) return_err(E_cause$fmt_TooManyArgs());
    claim_assert(TypeInfo_eql(tuple.type, u_typeInfoRecord(fields)));

    var_(plan, A$$(fmt_max_foldable_steps, fmt_FoldStep)) = A_zero();
    if (!fmt__buildFoldPlan(fmt, A_ptr(plan))) return fmt_printRuntime(out, fmt, fields, tuple);

    var buf = out;
    loop_inline_(for_)(($rt(usize_(fmt_max_foldable_steps)))(step_idx)) {
        let step = *A_at((plan)[step_idx]);
        if (step.kind != fmt_FoldKind_none) {
            buf = S_suffix((buf)fmt__copy(
                buf,
                S_slice((fmt)$r(step.literal_start, step.literal_start + step.literal_len))
            ));
        }
        if (step.kind == fmt_FoldKind_tail) {
            if (step.occ_idx < fields.len) return_err(E_cause$fmt_TooManyArgs());
        } else if (step.kind == fmt_FoldKind_escaped_percent_brace) {
            buf = S_suffix((buf)fmt__copy(buf, u8_l("%{")));
        } else if (step.kind == fmt_FoldKind_placeholder) {
            let body = S_slice((fmt)$r(step.body_start, step.body_start + step.body_len));
            let arg_idx = try_(fmt__argIdx(body, step.occ_idx));
            if (arg_idx >= fields.len) return_err(E_cause$fmt_TooFewArgs());
            buf = S_suffix((buf)try_(fmt__writeBody(buf, body, u_fieldPtr(tuple, fields, arg_idx))));
        }
    } $end(for);
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
                patts_((fmt_EventPos_placeholder, fmt_EventPos_escaped_percent_brace)(pos) {
                    $break_(fmt__copy(buf, S_prefix((iter.rest)pos)));
                }) $end(patts);
            } $end(match)) $unscoped(expr);
            buf = S_suffix((buf)written_before);

            match_(event) {
            patt_((fmt_EventPos_placeholder)(pos)) {
                let body_start = pos + 2;
                let close = try_(fmt__findCloseRuntime(S_suffix((iter.rest)body_start)));
                let body = S_prefix((S_suffix((iter.rest)body_start))close);
                let arg_idx = try_(fmt__argIdx(body, iter.occ_idx));
                if (arg_idx >= fields.len) return_err(E_cause$fmt_TooFewArgs());
                buf = S_suffix((buf)try_(fmt__writeBody(buf, body, u_fieldPtr(tuple, fields, arg_idx))));
                iter.rest = S_suffix((iter.rest)(body_start + close + 1));
                iter.occ_idx++;
            } $end(patt);
            patt_((fmt_EventPos_escaped_percent_brace)(pos)) {
                *S_at((buf)[0]) = u8_c('%');
                *S_at((buf)[1]) = u8_c('{');
                buf = S_suffix((buf)2);
                iter.rest = S_suffix((iter.rest)(pos + 3));
            } $end(patt);
            } $end(match);
        }
        if (iter.occ_idx < fields.len) return_err(E_cause$fmt_TooManyArgs());
        buf = S_suffix((buf)fmt__copy(buf, iter.rest));
    };
    return_ok(S_prefix((out)(out.len - buf.len)));
} $unscoped(fn);

fn_((fmt__writerWriteAll(io_Writer writer, S_const$u8 bytes))(E$void) $scope) {
    var written = usize_(0);
    while (written < bytes.len) {
        written += try_(writer.writeFn(writer.ctx, S_suffix((bytes)written)));
    }
    return_ok({});
} $unscoped(fn);

fn_((fmt__writerWritePadded(io_Writer writer, S_const$u8 text, fmt_LayoutSpec layout))(E$void) $scope) {
    if (text.len >= layout.width) return fmt__writerWriteAll(writer, text);
    let pad = layout.width - text.len;
    let left = layout.align == fmt_Align_right ? pad
        : layout.align == fmt_Align_center ? pad / 2
        : usize_(0);
    let right = pad - left;
    var_(fill_mem, A$$(64, u8)) = A_zero();
    for (usize i = 0; i < A_len(fill_mem); ++i) *A_at((fill_mem)[i]) = layout.fill;
    var remaining = left;
    while (remaining != 0) {
        let chunk_len = remaining < A_len(fill_mem) ? remaining : A_len(fill_mem);
        try_(fmt__writerWriteAll(writer, A_prefix$((S_const$u8)(fill_mem)(chunk_len))));
        remaining -= chunk_len;
    }
    try_(fmt__writerWriteAll(writer, text));
    remaining = right;
    while (remaining != 0) {
        let chunk_len = remaining < A_len(fill_mem) ? remaining : A_len(fill_mem);
        try_(fmt__writerWriteAll(writer, A_prefix$((S_const$u8)(fill_mem)(chunk_len))));
        remaining -= chunk_len;
    }
    return_ok({});
} $unscoped(fn);

fn_((fmt_write(io_Writer writer, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$void) $scope) {
    if (!(isComptimeExpr(fmt.len)
        && (fmt.len == 0 || isComptimeExpr(*S_ptr(fmt))))) {
        return fmt_writeRuntime(writer, fmt, fields, tuple);
    }

    if (fields.len > fmt_max_arg_count) return_err(E_cause$fmt_TooManyArgs());
    claim_assert(TypeInfo_eql(tuple.type, u_typeInfoRecord(fields)));

    var_(plan, A$$(fmt_max_foldable_steps, fmt_FoldStep)) = A_zero();
    if (!fmt__buildFoldPlan(fmt, A_ptr(plan))) return fmt_writeRuntime(writer, fmt, fields, tuple);

    loop_inline_(for_)(($rt(usize_(fmt_max_foldable_steps)))(step_idx)) {
        let step = *A_at((plan)[step_idx]);
        if (step.kind != fmt_FoldKind_none && step.literal_len != 0) {
            try_(fmt__writerWriteAll(
                writer,
                S_slice((fmt)$r(step.literal_start, step.literal_start + step.literal_len))
            ));
        }
        if (step.kind == fmt_FoldKind_tail) {
            if (step.occ_idx < fields.len) return_err(E_cause$fmt_TooManyArgs());
        } else if (step.kind == fmt_FoldKind_escaped_percent_brace) {
            try_(fmt__writerWriteAll(writer, u8_l("%{")));
        } else if (step.kind == fmt_FoldKind_placeholder) {
            let body = S_slice((fmt)$r(step.body_start, step.body_start + step.body_len));
            let arg_idx = try_(fmt__argIdx(body, step.occ_idx));
            if (arg_idx >= fields.len) return_err(E_cause$fmt_TooFewArgs());
            try_(fmt__writeBodyToWriter(writer, body, u_fieldPtr(tuple, fields, arg_idx)));
        }
    } $end(for);
    return_ok({});
} $unscoped(fn);

fn_((fmt_writeRuntime(io_Writer writer, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$void)) {
    return fmt__writeRuntimeFrom(writer, fmt_Iter_init(fmt), fields, tuple);
}

fn_((fmt__writeRuntimeFrom(io_Writer writer, fmt_Iter iter, S_const$TypeInfo fields, u_P_const$raw tuple))(E$void) $scope) {
    if (fields.len > fmt_max_arg_count) return_err(E_cause$fmt_TooManyArgs());
    claim_assert(TypeInfo_eql(tuple.type, u_typeInfoRecord(fields)));

    while (true) {
        let event = orelse_((fmt__findEventRuntime(iter.rest))(break));
        match_(event) {
        patt_((fmt_EventPos_placeholder)(pos)) {
            try_(fmt__writerWriteAll(writer, S_prefix((iter.rest)pos)));
            let body_start = pos + 2;
            let close = try_(fmt__findCloseRuntime(S_suffix((iter.rest)body_start)));
            let body = S_prefix((S_suffix((iter.rest)body_start))close);
            let arg_idx = try_(fmt__argIdx(body, iter.occ_idx));
            if (arg_idx >= fields.len) return_err(E_cause$fmt_TooFewArgs());
            try_(fmt__writeBodyToWriter(writer, body, u_fieldPtr(tuple, fields, arg_idx)));
            iter.rest = S_suffix((iter.rest)(body_start + close + 1));
            iter.occ_idx++;
        } $end(patt);
        patt_((fmt_EventPos_escaped_percent_brace)(pos)) {
            try_(fmt__writerWriteAll(writer, S_prefix((iter.rest)pos)));
            try_(fmt__writerWriteAll(writer, u8_l("%{")));
            iter.rest = S_suffix((iter.rest)(pos + 3));
        } $end(patt);
        } $end(match);
    }
    if (iter.occ_idx < fields.len) return_err(E_cause$fmt_TooManyArgs());
    try_(fmt__writerWriteAll(writer, iter.rest));
    return_ok({});
} $unscoped(fn);

fn_((io_Writer_print(io_Writer writer, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$void)) {
    return fmt_write(writer, fmt, fields, tuple);
}

fn_((io_Writer_println(io_Writer writer, S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(E$void) $scope) {
    try_(fmt_write(writer, fmt, fields, tuple));
    try_(fmt__writerWriteAll(writer, u8_l(io_nl)));
    return_ok({});
} $unscoped(fn);

fn_((io_stream__write(P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    let_ignore = ctx;
    return_ok(as$(usize)(fwrite(bytes.ptr, 1, bytes.len, stdout)));
} $unscoped(fn);

fn_((io_stream_print(S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(void)) {
    let writer = (io_Writer){ .ctx = ptrCast$((P$raw)(stdout)), .writeFn = io_stream__write };
    let_ignore = catch_((fmt_write(writer, fmt, fields, tuple))($ignore, $do_nothing));
}

fn_((io_stream_println(S_const$u8 fmt, S_const$TypeInfo fields, u_P_const$raw tuple))(void)) {
    let writer = (io_Writer){ .ctx = ptrCast$((P$raw)(stdout)), .writeFn = io_stream__write };
    let_ignore = catch_((io_Writer_println(writer, fmt, fields, tuple))($ignore, $do_nothing));
}

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
    if (!has_precision) precision = 6;
    if (precision > 18) return_err(E_cause$fmt_InvalidPrecisionSpec());
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
        var written = try_(fmt_printF64(out, value < 0.0 ? -norm : norm, u8_c('d'), upper, alt, sign, precision, true));
        *S_at((out)[written++]) = upper ? u8_c('E') : u8_c('e');
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
    if (precision == 0) {
        if (alt) *S_at((out)[written++]) = u8_c('.');
        return_ok(written);
    }
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

fn_((fmt__findEventRuntime(S_const$u8 fmt))(O$fmt_EventPos) $scope) {
    for_(($rt(fmt.len))(pos)) {
        if (*S_at((fmt)[pos]) != u8_c('%')) continue;
        if (pos + 2 < fmt.len
            && *S_at((fmt)[pos + 1]) == u8_c('%')
            && *S_at((fmt)[pos + 2]) == u8_c('{')) {
            return_some(union_of((fmt_EventPos_escaped_percent_brace)(pos)));
        }
        if (pos + 1 < fmt.len && *S_at((fmt)[pos + 1]) == u8_c('{')) {
            return_some(union_of((fmt_EventPos_placeholder)(pos)));
        }
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((fmt__findCloseRuntime(S_const$u8 fmt))(E$usize) $scope) {
    for_(($rt(fmt.len))(pos)) {
        if (*S_at((fmt)[pos]) == u8_c('}')) return_ok(pos);
    } $end(for);
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

fn_((fmt__parseSpec(u8 type, u8 mode, bool alt, u8 sign, bool has_sign, fmt_LayoutSpec layout, usize precision, bool has_precision))(E$fmt_Spec) $scope) {
    var_(case_, fmt_Case) = (type == u8_c('B') || type == u8_c('U') || type == u8_c('I') || type == u8_c('F') || type == u8_c('P') || mode == u8_c('X'))
        ? fmt_Case_upper
        : fmt_Case_lower;

    if (type == u8_c('0')) {
        if (mode != u8_c(' ') || alt || has_sign || layout.width != 0 || has_precision) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
        return_ok(union_of((fmt_Spec_void){}));
    }
    if (type == u8_c('b') || type == u8_c('B')) {
        if (mode != u8_c(' ') || alt || has_sign || has_precision) return_err(E_cause$fmt_InvalidTypeSpec());
        return_ok(union_of((fmt_Spec_bool)((fmt_BoolSpec){ .layout = layout, .case_ = case_ })));
    }
    if (type == u8_c('u') || type == u8_c('U')) {
        if (has_sign || has_precision || mode == u8_c('e')) return_err(E_cause$fmt_InvalidTypeSpec());
        var_(int_mode, fmt_IntMode) = mode == u8_c('x') || mode == u8_c('X') ? fmt_IntMode_hex
            : mode == u8_c('o') ? fmt_IntMode_octal
            : mode == u8_c('b') ? fmt_IntMode_binary
            : fmt_IntMode_decimal;
        return_ok(union_of((fmt_Spec_uint)((fmt_UIntSpec){
            .layout = layout,
            .mode = int_mode,
            .case_ = case_,
            .alt_form = alt,
        })));
    }
    if (type == u8_c('i') || type == u8_c('I')) {
        if (has_precision || mode == u8_c('e')) return_err(E_cause$fmt_InvalidTypeSpec());
        var_(int_mode, fmt_IntMode) = mode == u8_c('x') || mode == u8_c('X') ? fmt_IntMode_hex
            : mode == u8_c('o') ? fmt_IntMode_octal
            : mode == u8_c('b') ? fmt_IntMode_binary
            : fmt_IntMode_decimal;
        var_(sign_mode, fmt_Sign) = sign == u8_c('+') ? fmt_Sign_always
            : sign == u8_c(' ') ? fmt_Sign_space
            : fmt_Sign_auto;
        return_ok(union_of((fmt_Spec_iint)((fmt_IIntSpec){
            .layout = layout,
            .mode = int_mode,
            .case_ = case_,
            .sign = sign_mode,
            .alt_form = alt,
        })));
    }
    if (type == u8_c('f') || type == u8_c('F')) {
        if (mode == u8_c('x') || mode == u8_c('X') || mode == u8_c('o') || mode == u8_c('b')) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
        var_(sign_mode, fmt_Sign) = sign == u8_c('+') ? fmt_Sign_always
            : sign == u8_c(' ') ? fmt_Sign_space
            : fmt_Sign_auto;
        return_ok(union_of((fmt_Spec_flt)((fmt_FltSpec){
            .layout = layout,
            .mode = mode == u8_c('e') ? fmt_FltMode_scientific : fmt_FltMode_decimal,
            .case_ = case_,
            .sign = sign_mode,
            .alt_form = alt,
            .precision = precision,
            .has_precision = has_precision,
        })));
    }
    if (type == u8_c('p') || type == u8_c('P')) {
        if (has_sign || has_precision || (mode != u8_c(' ') && mode != u8_c('x') && mode != u8_c('X'))) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
        return_ok(union_of((fmt_Spec_ptr)((fmt_PtrSpec){
            .layout = layout,
            .case_ = case_,
            .alt_form = alt,
        })));
    }
    if (type == u8_c('c') || type == u8_c('C')) {
        if (mode != u8_c(' ') || alt || has_sign || has_precision) return_err(E_cause$fmt_InvalidTypeSpec());
        let spec = (fmt_CharSpec){ .layout = layout };
        if (type == u8_c('c')) return_ok(union_of((fmt_Spec_ascii)(spec)));
        return_ok(union_of((fmt_Spec_utf8)(spec)));
    }
    if (type == u8_c('z') || type == u8_c('s')) {
        if (mode != u8_c(' ') || alt || has_sign || has_precision) return_err(E_cause$fmt_InvalidTypeSpec());
        let spec = (fmt_StrSpec){ .layout = layout };
        if (type == u8_c('z')) return_ok(union_of((fmt_Spec_str_z0)(spec)));
        return_ok(union_of((fmt_Spec_str)(spec)));
    }
    if (type == u8_c('e')) {
        if (mode != u8_c(' ') || alt || has_sign || has_precision) return_err(E_cause$fmt_InvalidTypeSpec());
        return_ok(union_of((fmt_Spec_err)((fmt_ErrSpec){ .layout = layout })));
    }
    return_err(E_cause$fmt_InvalidTypeSpec());
} $unscoped(fn);

fn_((fmt__specLayout(fmt_Spec spec))(fmt_LayoutSpec) $scope) {
    match_(spec) {
    patt_((fmt_Spec_void)($ignore)) return fmt_LayoutSpec_default() $end(patt);
    patt_((fmt_Spec_bool)(typed)) return typed.layout $end(patt);
    patt_((fmt_Spec_uint)(typed)) return typed.layout $end(patt);
    patt_((fmt_Spec_iint)(typed)) return typed.layout $end(patt);
    patt_((fmt_Spec_flt)(typed)) return typed.layout $end(patt);
    patt_((fmt_Spec_ptr)(typed)) return typed.layout $end(patt);
    patt_((fmt_Spec_ascii)(typed)) return typed.layout $end(patt);
    patt_((fmt_Spec_utf8)(typed)) return typed.layout $end(patt);
    patt_((fmt_Spec_str_z0)(typed)) return typed.layout $end(patt);
    patt_((fmt_Spec_str)(typed)) return typed.layout $end(patt);
    patt_((fmt_Spec_err)(typed)) return typed.layout $end(patt);
    } $end(match);
} $unscoped(fn);

fn_((fmt__parseBody(S_const$u8 body))(E$fmt_ParsedBody) $scope) {
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
    var has_sign = false;
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
        if (pos < body.len && (*S_at((body)[pos]) == u8_c('+') || *S_at((body)[pos]) == u8_c('-') || *S_at((body)[pos]) == u8_c(' '))) {
            sign = *S_at((body)[pos++]);
            has_sign = true;
        }
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
    let layout = (fmt_LayoutSpec){ .fill = fill, .align = align, .width = width };
    let spec = try_(fmt__parseSpec(type, mode, alt, sign, has_sign, layout, precision, has_precision));
    return_ok(((fmt_ParsedBody){ .wrapped = wrapped, .size = size, .spec = spec }));
} $unscoped(fn);

fn_((fmt__writeBody(S$u8 out, S_const$u8 body, u_P_const$raw field))(E$usize) $scope) {
    let parsed = try_(fmt__parseBody(body));
    let layout = fmt__specLayout(parsed.spec);
    var_(tmp, A$$(128, u8)) = A_zero();
    let target = layout.width == 0 ? out : A_ref$((S$u8)tmp);
    let written = try_(fmt__writeValue(target, parsed.wrapped, parsed.size, parsed.spec, field));
    if (layout.width == 0) return_ok(written);
    return_ok(fmt_printPadded(
        out,
        S_prefix((A_ref$((S_const$u8)tmp))written),
        layout.fill,
        layout.align,
        layout.width
    ));
} $unscoped(fn);

fn_((fmt__writeBodyToWriter(io_Writer writer, S_const$u8 body, u_P_const$raw field))(E$void) $scope) {
    let parsed = try_(fmt__parseBody(body));
    let layout = fmt__specLayout(parsed.spec);
    if (parsed.wrapped == u8_c('\0') && parsed.spec.tag == fmt_Spec_str) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(S_const$u8)));
        return fmt__writerWritePadded(writer, *u_castP$((const S_const$u8*)(field)), layout);
    }
    if (parsed.wrapped == u8_c('\0') && parsed.spec.tag == fmt_Spec_str_z0) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(P_const$u8)));
        let text = mem_spanZ0$u8(*u_castP$((const P_const$u8*)(field)));
        return fmt__writerWritePadded(writer, text, layout);
    }
    if (parsed.wrapped == u8_c('?') && parsed.spec.tag == fmt_Spec_str) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(O$S_const$u8)));
        let value = *u_castP$((const O$S_const$u8*)(field));
        if_none((value)) return fmt__writerWritePadded(writer, u8_l("none"), layout);
        else_some((payload)) return fmt__writerWritePadded(writer, payload, layout);
    }

    var_(value_mem, A$$(256, u8)) = A_zero();
    let written = try_(fmt__writeValue(
        A_ref$((S$u8)value_mem),
        parsed.wrapped,
        parsed.size,
        parsed.spec,
        field
    ));
    return fmt__writerWritePadded(
        writer,
        A_prefix$((S_const$u8)(value_mem)(written)),
        layout
    );
} $unscoped(fn);

fn_((fmt__writeValue(S$u8 out, u8 wrapped, fmt_Size size, fmt_Spec spec, u_P_const$raw field))(E$usize) $scope) {
    if (wrapped == u8_c('\0')) return fmt__writePlainValue(out, size, spec, field);
    if (wrapped == u8_c('?') && spec.tag == fmt_Spec_iint) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(O$i32)));
        let value = *u_castP$((const O$i32*)(field));
        if_none((value)) return_ok(fmt__copy(out, u8_l("none")));
        else_some((payload)) {
            var inner = payload;
            return fmt__writePlainValue(out, size, spec, u_anyP(&inner).as_const);
        }
    }
    if (wrapped == u8_c('?') && spec.tag == fmt_Spec_str) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(O$S_const$u8)));
        let value = *u_castP$((const O$S_const$u8*)(field));
        if_none((value)) return_ok(fmt__copy(out, u8_l("none")));
        else_some((payload)) {
            var inner = payload;
            return fmt__writePlainValue(out, size, spec, u_anyP(&inner).as_const);
        }
    }
    if (wrapped == u8_c('!') && spec.tag == fmt_Spec_iint) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(E$i32)));
        let value = *u_castP$((const E$i32*)(field));
        if_err((value)(err)) return_ok(fmt_printErr(out, err));
        else_ok((payload)) {
            var inner = payload;
            return fmt__writePlainValue(out, size, spec, u_anyP(&inner).as_const);
        }
    }
    return_err(E_cause$fmt_InvalidTypeSpec());
} $unscoped(fn);

fn_((fmt__formatTyped(S$u8 out, fmt_Size size, u_P_const$raw value, fmt_Spec spec))(E$S$u8) $scope) {
    let layout = fmt__specLayout(spec);
    if (layout.width == 0) {
        let written = try_(fmt__writePlainValue(out, size, spec, value));
        return_ok(S_prefix((out)written));
    }

    var_(un_padded_mem, A$$(128, u8)) = A_zero();
    let un_padded = A_ref$((S$u8)un_padded_mem);
    let un_padded_len = try_(fmt__writePlainValue(un_padded, size, spec, value));
    let written = fmt_printPadded(
        out,
        S_prefix((un_padded.as_const)un_padded_len),
        layout.fill,
        layout.align,
        layout.width
    );
    return_ok(S_prefix((out)written));
} $unscoped(fn);

fn_((fmt__writePlainValue(S$u8 out, fmt_Size size, fmt_Spec spec, u_P_const$raw field))(E$usize) $scope) {
    match_(spec) {
    patt_((fmt_Spec_void)($ignore)) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(Void)));
        return_ok(fmt_printVoid(out, *u_castP$((const Void*)(field))));
    } $end(patt);
    patt_((fmt_Spec_bool)(typed)) {
        claim_assert(size == fmt_Size_32);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(bool)));
        return_ok(fmt_printBool(out, *u_castP$((const bool*)(field)), typed.case_ == fmt_Case_upper));
    } $end(patt);
    patt_((fmt_Spec_uint)(typed)) {
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
        } else if (size == fmt_Size_long) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(ulong)));
            value = *u_castP$((const ulong*)(field));
        } else if (size == fmt_Size_ptr) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(usize)));
            value = *u_castP$((const usize*)(field));
        } else if (size == fmt_Size_64) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(u64)));
            value = *u_castP$((const u64*)(field));
        } else return_err(E_cause$fmt_InvalidSizeSpec());
        var_(mode, u8) = typed.mode == fmt_IntMode_hex ? u8_c('x')
            : typed.mode == fmt_IntMode_octal ? u8_c('o')
            : typed.mode == fmt_IntMode_binary ? u8_c('b')
            : u8_c('d');
        return_ok(fmt_printU64(out, value, mode, typed.case_ == fmt_Case_upper, typed.alt_form));
    } $end(patt);
    patt_((fmt_Spec_iint)(typed)) {
        var value = i64_(0);
        if (size == fmt_Size_8) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(i8)));
            value = *u_castP$((const i8*)(field));
        } else if (size == fmt_Size_16) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(i16)));
            value = *u_castP$((const i16*)(field));
        } else if (size == fmt_Size_32) {
            if (TypeInfo_eql(field.type, typeInfo$(i32))) {
                value = *u_castP$((const i32*)(field));
            } else {
                claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
                value = as$(i32)(*u_castP$((const u32*)(field)));
            }
        } else if (size == fmt_Size_long) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(ilong)));
            value = *u_castP$((const ilong*)(field));
        } else if (size == fmt_Size_ptr) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(isize)));
            value = *u_castP$((const isize*)(field));
        } else if (size == fmt_Size_64) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(i64)));
            value = *u_castP$((const i64*)(field));
        } else return_err(E_cause$fmt_InvalidSizeSpec());
        var_(mode, u8) = typed.mode == fmt_IntMode_hex ? u8_c('x')
            : typed.mode == fmt_IntMode_octal ? u8_c('o')
            : typed.mode == fmt_IntMode_binary ? u8_c('b')
            : u8_c('d');
        var_(sign, u8) = typed.sign == fmt_Sign_always ? u8_c('+')
            : typed.sign == fmt_Sign_space ? u8_c(' ')
            : u8_c('-');
        return_ok(fmt_printI64(out, value, mode, typed.case_ == fmt_Case_upper, typed.alt_form, sign));
    } $end(patt);
    patt_((fmt_Spec_flt)(typed)) {
        var_(mode, u8) = typed.mode == fmt_FltMode_scientific ? u8_c('e') : u8_c('d');
        var_(sign, u8) = typed.sign == fmt_Sign_always ? u8_c('+')
            : typed.sign == fmt_Sign_space ? u8_c(' ')
            : u8_c('-');
        if (size == fmt_Size_32) {
            claim_assert(TypeInfo_eql(field.type, typeInfo$(f32)));
            return fmt_printF64(
                out,
                *u_castP$((const f32*)(field)),
                mode,
                typed.case_ == fmt_Case_upper,
                typed.alt_form,
                sign,
                typed.precision,
                typed.has_precision
            );
        }
        claim_assert(size == fmt_Size_64);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(f64)));
        return fmt_printF64(
            out,
            *u_castP$((const f64*)(field)),
            mode,
            typed.case_ == fmt_Case_upper,
            typed.alt_form,
            sign,
            typed.precision,
            typed.has_precision
        );
    } $end(patt);
    patt_((fmt_Spec_ptr)(typed)) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(P_const$raw)));
        return_ok(fmt_printPtr(
            out,
            *u_castP$((const P_const$raw*)(field)),
            u8_c('x'),
            typed.case_ == fmt_Case_upper,
            typed.alt_form
        ));
    } $end(patt);
    patt_((fmt_Spec_ascii)($ignore)) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u8)));
        return_ok(try_(fmt_printASCII(out, *u_castP$((const u8*)(field)))));
    } $end(patt);
    patt_((fmt_Spec_utf8)($ignore)) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
        return_ok(try_(fmt_printUTF8(out, *u_castP$((const u32*)(field)))));
    } $end(patt);
    patt_((fmt_Spec_str_z0)($ignore)) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(P_const$u8)));
        return_ok(fmt_printStrZ0(out, *u_castP$((const P_const$u8*)(field))));
    } $end(patt);
    patt_((fmt_Spec_str)($ignore)) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(S_const$u8)));
        return_ok(fmt_printStr(out, *u_castP$((const S_const$u8*)(field))));
    } $end(patt);
    patt_((fmt_Spec_err)($ignore)) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(EAny)));
        return_ok(fmt_printErr(out, *u_castP$((const EAny*)(field))));
    } $end(patt);
    default_() return_err(E_cause$fmt_InvalidTypeSpec()) $end(default);
    } $end(match);
} $unscoped(fn);

// dh-c build optimize dh/lab/drafts/draft-fmt-foldable-full.c --lto=off --link-stdlib=off --link=msvcrt --static --emit-disasm=dh/lab/drafts/draft-fmt-foldable-full.disasm
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

typedef enum_((fmt_Sign $fits($packed))(
    fmt_Sign_auto = 0,
    fmt_Sign_always = 1,
    fmt_Sign_space = 2
)) fmt_Sign;

typedef enum_((fmt_BinPrefix $fits($packed))(
    fmt_BinPrefix_none = 0,
    fmt_BinPrefix_lower = 1,
    fmt_BinPrefix_upper = 2
)) fmt_BinPrefix;
typedef struct fmt_HexStyle {
    var_(case_, fmt_Case);
    var_(alt_form, bool);
} fmt_HexStyle;
typedef variant_((fmt_IntStyle $fits($packed))(
    (fmt_IntStyle_decimal, Void),
    (fmt_IntStyle_binary, fmt_BinPrefix),
    (fmt_IntStyle_octal, bool),
    (fmt_IntStyle_hex, fmt_HexStyle)
)) fmt_IntStyle;

typedef struct fmt_FltDecimalStyle {
    var_(alt_form, bool);
} fmt_FltDecimalStyle;
typedef struct fmt_FltScientificStyle {
    var_(case_, fmt_Case);
    var_(alt_form, bool);
} fmt_FltScientificStyle;
typedef variant_((fmt_FltStyle $fits($packed))(
    (fmt_FltStyle_decimal, fmt_FltDecimalStyle),
    (fmt_FltStyle_scientific, fmt_FltScientificStyle)
)) fmt_FltStyle;

typedef struct fmt_BoolSpec {
    var_(layout, fmt_LayoutSpec);
    var_(case_, fmt_Case);
} fmt_BoolSpec;
typedef struct fmt_UIntSpec {
    var_(layout, fmt_LayoutSpec);
    var_(style, fmt_IntStyle);
} fmt_UIntSpec;
typedef struct fmt_IIntSpec {
    var_(layout, fmt_LayoutSpec);
    var_(style, fmt_IntStyle);
    var_(sign, fmt_Sign);
} fmt_IIntSpec;
typedef struct fmt_FltSpec {
    var_(layout, fmt_LayoutSpec);
    var_(style, fmt_FltStyle);
    var_(sign, fmt_Sign);
    var_(precision, O$usize);
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
typedef struct fmt_RuntimeBody {
    var_(wrapped, u8);
    var_(type, u8);
    var_(size, fmt_Size);
    var_(mode, u8);
    var_(alt, bool);
    var_(layout, fmt_LayoutSpec);
    var_(sign, u8);
    var_(has_sign, bool);
    var_(precision, usize);
    var_(has_precision, bool);
} fmt_RuntimeBody;
T_use_E$(fmt_RuntimeBody);

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
    fmt_fold_inline_step_budget = 4,
    fmt_fold_event_scan_bytes = 32,
    fmt_fold_body_scan_bytes = 32,
    fmt_folded_writer_buffer_size = 256,
};

_Static_assert(
    fmt_fold_inline_step_budget == 4,
    "fmt_print scalar expansion must match fmt_fold_inline_step_budget"
);
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
typedef struct fmt_BufferWriter {
    var_(out, S$u8);
    var_(written, usize);
} fmt_BufferWriter;

$attr($maybe_unused $must_check $inline_always)
$static fn_((fmt_print(S$u8 out, S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(E$S$u8));
$attr($must_check $inline_never $branch_cold)
$static fn_((fmt_printRuntime(S$u8 out, S_const$u8 fmt, u_Tup tup))(E$S$u8));
$attr($must_check $inline_always)
$static fn_((fmt_write(io_Writer writer, S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(E$void));
$attr($must_check $inline_never $branch_cold)
$static fn_((fmt_writeRuntime(io_Writer writer, S_const$u8 fmt, u_Tup tup))(E$void));

$attr($maybe_unused $must_check $inline_always)
$static fn_((io_Writer_print(io_Writer writer, S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(E$void));
$attr($maybe_unused $must_check $inline_always)
$static fn_((io_Writer_println(io_Writer writer, S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(E$void));
$attr($maybe_unused $inline_always)
$static fn_((io_stream_print(S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(void));
$attr($maybe_unused $inline_always)
$static fn_((io_stream_println(S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(void));
$attr($maybe_unused $inline_always)
$static fn_((io_stream_eprint(S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(void));
$attr($maybe_unused $inline_always)
$static fn_((io_stream_eprintln(S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(void));

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
$attr($inline_always)
$static fn_((fmt__printPaddedFolded(S$u8 out, S_const$u8 text, u8 fill, fmt_Align align, usize width))(usize));
$static fn_((fmt_printVoid(S$u8 out, Void value))(usize));
$attr($inline_never)
$static fn_((fmt_printBool(S$u8 out, bool value, bool upper))(usize));
$attr($inline_always)
$static fn_((fmt__printBoolFolded(S$u8 out, bool value, bool upper))(usize));
$attr($inline_never)
$static fn_((fmt_printU64(S$u8 out, u64 value, u8 mode, bool upper, bool alt))(usize));
$attr($inline_always)
$static fn_((fmt__printU64Folded(S$u8 out, u64 value, u8 mode, bool upper, bool alt))(usize));
$static fn_((fmt_printU64Base(S$u8 out, u64 value, usize base, bool upper))(usize));
$static fn_((fmt_printU64Dec(S$u8 out, u64 value))(usize));
$static fn_((fmt_printU64Hex(S$u8 out, u64 value, bool upper, bool alt))(usize));
$static fn_((fmt_printU64Oct(S$u8 out, u64 value, bool alt))(usize));
$static fn_((fmt_printU64Bin(S$u8 out, u64 value, bool upper, bool alt))(usize));
$attr($inline_never)
$static fn_((fmt_printI64(S$u8 out, i64 value, u8 mode, bool upper, bool alt, u8 sign))(usize));
$attr($inline_always)
$static fn_((fmt__printI64Folded(S$u8 out, i64 value, u8 mode, bool upper, bool alt, u8 sign))(usize));
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
    fmt_FoldKind_literal_chunk = 1,
    fmt_FoldKind_placeholder = 2,
    fmt_FoldKind_escaped_percent_brace = 3
)) fmt_FoldKind;
typedef struct fmt_FoldFlatWrite {
    var_(written, usize);
    var_(too_small, bool);
} fmt_FoldFlatWrite;
typedef struct fmt_FoldCursorStep {
    var_(kind, fmt_FoldKind);
    var_(literal_len, usize);
    var_(body_start, usize);
    var_(body_len, usize);
} fmt_FoldCursorStep;
$attr($must_check $inline_always)
$static fn_((fmt__parseBody(S_const$u8 body))(E$fmt_ParsedBody));
$attr($must_check $inline_never)
$static fn_((fmt__parseBodyRuntime(S_const$u8 body))(E$fmt_RuntimeBody));

$attr($inline_always)
$static fn_((fmt_Iter_init(S_const$u8 fmt))(fmt_Iter));
$attr($must_check $inline_always)
$static fn_((fmt__writerWriteAll(io_Writer writer, S_const$u8 bytes))(E$void));
$attr($must_check $inline_never)
$static fn_((fmt__writerWritePadded(io_Writer writer, S_const$u8 text, fmt_LayoutSpec layout))(E$void));
$attr($must_check $inline_never)
$static fn_((fmt__writeBodyToWriter(io_Writer writer, S_const$u8 body, u_P_const$raw field))(E$void));
$attr($must_check $inline_always)
$static fn_((fmt__writeParsedToWriter(io_Writer writer, fmt_ParsedBody parsed, u_P_const$raw field, bool fold_values))(E$void));
$attr($must_check $inline_never $branch_cold)
$static fn_((fmt__writeRuntimeFrom(io_Writer writer, fmt_Iter iter, u_Tup tup))(E$void));
$attr($must_check $inline_never)
$static fn_((fmt__writeValueRuntime(S$u8 out, fmt_RuntimeBody body, u_P_const$raw field))(E$usize));
$attr($must_check $inline_always)
$static fn_((fmt__writeFoldedField(S$u8 out, fmt_RuntimeBody body, u_P_const$raw field, bool fold_values))(E$usize));
$attr($must_check $inline_always)
$static fn_((fmt__writeValueCore(S$u8 out, fmt_RuntimeBody body, u_P_const$raw field, bool fold_values))(E$usize));
$attr($inline_always)
$static fn_((fmt__runtimeBodyFromSpec(u8 wrapped, fmt_Size size, fmt_Spec spec))(fmt_RuntimeBody));
$attr($inline_always)
$static fn_((fmt__runtimeValueTypeInfo(fmt_RuntimeBody body))(TypeInfo));
$attr($must_check $inline_never)
$static fn_((fmt__bufferWriterWrite(P$raw ctx, S_const$u8 bytes))(E$usize));
$attr($must_check $inline_always)
$static fn_((io_stream__write(P$raw ctx, S_const$u8 bytes))(E$usize));

$attr($must_check)
$static fn_((fmt__findEventRuntime(S_const$u8 fmt))(O$fmt_EventPos));
$attr($must_check)
$static fn_((fmt__findCloseRuntime(S_const$u8 fmt))(E$usize));
$attr($inline_always)
$static fn_((fmt__copy(S$u8 out, S_const$u8 text))(usize));
$attr($must_check $inline_always)
$static fn_((fmt__argIdx(S_const$u8 body, usize occ_idx))(E$usize));

$attr($must_check $inline_always)
$static fn_((fmt__writeParsed(S$u8 out, fmt_ParsedBody parsed, u_P_const$raw field, bool fold_values))(E$usize));
$attr($must_check $inline_always)
$static fn_((fmt__parseBody(S_const$u8 body))(E$fmt_ParsedBody));
$attr($must_check $inline_always)
$static fn_((fmt__parseSpec(u8 type, u8 mode, bool alt, u8 sign, bool has_sign, bool has_layout, fmt_LayoutSpec layout, usize precision, bool has_precision))(E$fmt_Spec));
$attr($inline_always)
$static fn_((fmt__specLayout(fmt_Spec spec))(fmt_LayoutSpec));
$attr($must_check $inline_always)
$static fn_((fmt__writeValue(S$u8 out, u8 wrapped, fmt_Size size, fmt_Spec spec, u_P_const$raw field, bool fold_values))(E$usize));
$attr($inline_always)
$static fn_((fmt__nextFoldStep(S_const$u8 fmt, usize cursor))(fmt_FoldCursorStep));
$attr($inline_always)
$static fn_((fmt__argIdxFolded(S_const$u8 body, usize occ_idx))(usize));
$attr($inline_always)
$static fn_((fmt__parseBodyFolded(S_const$u8 body))(fmt_RuntimeBody));
$attr($must_check $inline_always)
$static fn_((fmt__writeFoldedText(S$u8 out, S_const$u8 text, fmt_LayoutSpec layout))(E$usize));
$attr($must_check $inline_never)
$static fn_((fmt__writeRuntimeUIntExact(S$u8 out, u64 value, fmt_RuntimeBody body))(E$usize));
$attr($must_check $inline_never)
$static fn_((fmt__writeRuntimeIIntExact(S$u8 out, i64 value, fmt_RuntimeBody body))(E$usize));
$attr($must_check $inline_never)
$static fn_((fmt__writeRuntimePtrExact(S$u8 out, P_const$raw value, fmt_RuntimeBody body))(E$usize));
$attr($must_check $inline_never)
$static fn_((fmt__writeRuntimeUTF8Exact(S$u8 out, u32 value, fmt_RuntimeBody body))(E$usize));
$attr($inline_always)
$static fn_((fmt__writeFoldedBool(S$u8 out, bool value, fmt_RuntimeBody body))(fmt_FoldFlatWrite));
$attr($inline_always)
$static fn_((fmt__writeFoldedUInt(S$u8 out, u64 value, fmt_RuntimeBody body))(fmt_FoldFlatWrite));
$attr($inline_always)
$static fn_((fmt__writeFoldedIInt(S$u8 out, i64 value, fmt_RuntimeBody body))(fmt_FoldFlatWrite));
$attr($must_check $inline_always)
$static fn_((fmt__writeTyped$bool(S$u8 out, fmt_ParsedBody parsed, bool value, bool value_is_comptime))(E$S$u8));
#define fmt__declareTypedValue(_$Type) \
    $attr($must_check $inline_always) \
    $static fn_((pp_cat(fmt__writeTyped$, _$Type)(S$u8 out, fmt_ParsedBody parsed, _$Type value, bool value_is_comptime))(E$S$u8))
fmt__declareTypedValue(u8);
fmt__declareTypedValue(u16);
fmt__declareTypedValue(u32);
fmt__declareTypedValue(ulong);
fmt__declareTypedValue(u64);
fmt__declareTypedValue(i8);
fmt__declareTypedValue(i16);
fmt__declareTypedValue(i32);
fmt__declareTypedValue(ilong);
fmt__declareTypedValue(i64);
fmt__declareTypedValue(f32);
fmt__declareTypedValue(f64);
fmt__declareTypedValue(P_const$raw);
fmt__declareTypedValue(P_const$u8);
fmt__declareTypedValue(S_const$u8);
fmt__declareTypedValue(EAny);
fmt__declareTypedValue(O$u64);
fmt__declareTypedValue(E$S_const$u8);
#undef fmt__declareTypedValue

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
    var_(style, fmt_IntStyle) = union_of((fmt_IntStyle_decimal){});
    return (fmt_UIntSpec){
        .layout = fmt_LayoutSpec_default(),
        .style = style,
    };
}
fn_((fmt_IIntSpec_default(void))(fmt_IIntSpec)) {
    var_(style, fmt_IntStyle) = union_of((fmt_IntStyle_decimal){});
    return (fmt_IIntSpec){
        .layout = fmt_LayoutSpec_default(),
        .style = style,
        .sign = fmt_Sign_auto,
    };
}
fn_((fmt_FltSpec_default(void))(fmt_FltSpec)) {
    var_(style, fmt_FltStyle) = union_of((fmt_FltStyle_decimal)(((fmt_FltDecimalStyle){ .alt_form = false })));
    return (fmt_FltSpec){
        .layout = fmt_LayoutSpec_default(),
        .style = style,
        .sign = fmt_Sign_auto,
        .precision = none$((O$usize)),
    };
}
fn_((fmt_PtrSpec_default(void))(fmt_PtrSpec)) {
    return (fmt_PtrSpec){
        .layout = fmt_LayoutSpec_default(),
        .case_ = fmt_Case_lower,
        .alt_form = false,
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

fn_((fmt_formatVoid(S$u8 out, Void value))(E$S$u8) $scope) {
    $ignore_void value;
    return_ok(S_prefix((out)0));
} $unscoped(fn);
fn_((fmt_format$Void(S$u8 out, Void value))(E$S$u8)) {
    return fmt_formatVoid(out, value);
}
fn_((fmt_formatBool(S$u8 out, bool value, fmt_BoolSpec spec))(E$S$u8) $scope) {
    let text = value
                 ? (spec.case_ == fmt_Case_upper ? u8_l("TRUE") : u8_l("true"))
                 : (spec.case_ == fmt_Case_upper ? u8_l("FALSE") : u8_l("false"));
    let written = isComptimeExpr(value)
                    ? fmt__printPaddedFolded(out, text, spec.layout.fill, spec.layout.align, spec.layout.width)
                    : fmt_printPadded(out, text, spec.layout.fill, spec.layout.align, spec.layout.width);
    return_ok(S_prefix((out)written));
} $unscoped(fn);
fn_((fmt_format$bool(S$u8 out, bool value, fmt_BoolSpec spec))(E$S$u8)) {
    return fmt_formatBool(out, value, spec);
}
fn_((fmt_formatUInt(S$u8 out, u64 value, fmt_UIntSpec spec))(E$S$u8) $scope) {
    var_(tmp, A$$(128, u8));
    let target = spec.layout.width == 0 ? out : A_ref$((S$u8)tmp);
    let written = expr_(usize $scope)(match_(spec.style) {
        patt_((fmt_IntStyle_decimal)($ignore)) {
            $break_(isComptimeExpr(value) ? fmt__printU64Folded(target, value, u8_c('d'), false, false) : fmt_printU64(target, value, u8_c('d'), false, false));
        } $end(patt);
        patt_((fmt_IntStyle_binary)(prefix)) {
            $break_(isComptimeExpr(value) ? fmt__printU64Folded(target, value, u8_c('b'), prefix == fmt_BinPrefix_upper, prefix != fmt_BinPrefix_none) : fmt_printU64(target, value, u8_c('b'), prefix == fmt_BinPrefix_upper, prefix != fmt_BinPrefix_none));
        } $end(patt);
        patt_((fmt_IntStyle_octal)(alt_form)) {
            $break_(isComptimeExpr(value) ? fmt__printU64Folded(target, value, u8_c('o'), false, alt_form) : fmt_printU64(target, value, u8_c('o'), false, alt_form));
        } $end(patt);
        patt_((fmt_IntStyle_hex)(style)) {
            $break_(isComptimeExpr(value) ? fmt__printU64Folded(target, value, u8_c('x'), style.case_ == fmt_Case_upper, style.alt_form) : fmt_printU64(target, value, u8_c('x'), style.case_ == fmt_Case_upper, style.alt_form));
        } $end(patt);
    } $end(match)) $unscoped(expr);
    if (spec.layout.width == 0) return_ok(S_prefix((out)written));
    let padded = isComptimeExpr(written)
                   ? fmt__printPaddedFolded(
                         out,
                         A_prefix$((S_const$u8)(tmp)(written)),
                         spec.layout.fill,
                         spec.layout.align,
                         spec.layout.width
                     )
                   : fmt_printPadded(
                         out,
                         A_prefix$((S_const$u8)(tmp)(written)),
                         spec.layout.fill,
                         spec.layout.align,
                         spec.layout.width
                     );
    return_ok(S_prefix((out)padded));
} $unscoped(fn);
fn_((fmt_formatIInt(S$u8 out, i64 value, fmt_IIntSpec spec))(E$S$u8) $scope) {
    var_(tmp, A$$(128, u8));
    let target = spec.layout.width == 0 ? out : A_ref$((S$u8)tmp);
    var_(sign, u8) = spec.sign == fmt_Sign_always ? u8_c('+')
                   : spec.sign == fmt_Sign_space  ? u8_c(' ')
                                                  : u8_c('-');
    let written = expr_(usize $scope)(match_(spec.style) {
        patt_((fmt_IntStyle_decimal)($ignore)) {
            $break_(isComptimeExpr(value) ? fmt__printI64Folded(target, value, u8_c('d'), false, false, sign) : fmt_printI64(target, value, u8_c('d'), false, false, sign));
        } $end(patt);
        patt_((fmt_IntStyle_binary)(prefix)) {
            $break_(isComptimeExpr(value) ? fmt__printI64Folded(target, value, u8_c('b'), prefix == fmt_BinPrefix_upper, prefix != fmt_BinPrefix_none, sign) : fmt_printI64(target, value, u8_c('b'), prefix == fmt_BinPrefix_upper, prefix != fmt_BinPrefix_none, sign));
        } $end(patt);
        patt_((fmt_IntStyle_octal)(alt_form)) {
            $break_(isComptimeExpr(value) ? fmt__printI64Folded(target, value, u8_c('o'), false, alt_form, sign) : fmt_printI64(target, value, u8_c('o'), false, alt_form, sign));
        } $end(patt);
        patt_((fmt_IntStyle_hex)(style)) {
            $break_(isComptimeExpr(value) ? fmt__printI64Folded(target, value, u8_c('x'), style.case_ == fmt_Case_upper, style.alt_form, sign) : fmt_printI64(target, value, u8_c('x'), style.case_ == fmt_Case_upper, style.alt_form, sign));
        } $end(patt);
    } $end(match)) $unscoped(expr);
    if (spec.layout.width == 0) return_ok(S_prefix((out)written));
    let padded = isComptimeExpr(written)
                   ? fmt__printPaddedFolded(
                         out,
                         A_prefix$((S_const$u8)(tmp)(written)),
                         spec.layout.fill,
                         spec.layout.align,
                         spec.layout.width
                     )
                   : fmt_printPadded(
                         out,
                         A_prefix$((S_const$u8)(tmp)(written)),
                         spec.layout.fill,
                         spec.layout.align,
                         spec.layout.width
                     );
    return_ok(S_prefix((out)padded));
} $unscoped(fn);
fn_((fmt_formatFlt(S$u8 out, f64 value, fmt_FltSpec spec))(E$S$u8) $scope) {
    var_(tmp, A$$(512, u8));
    let target = spec.layout.width == 0 ? out : A_ref$((S$u8)tmp);
    var_(sign, u8) = spec.sign == fmt_Sign_always ? u8_c('+')
                   : spec.sign == fmt_Sign_space  ? u8_c(' ')
                                                  : u8_c('-');
    var precision = usize_(6);
    var has_precision = false;
    if_some((spec.precision)(requested)) {
        precision = requested;
        has_precision = true;
    }
    let written = expr_(E$usize $scope)(match_(spec.style) {
        patt_((fmt_FltStyle_decimal)(style)) {
            $break_(fmt_printF64(target, value, u8_c('d'), false, style.alt_form, sign, precision, has_precision));
        } $end(patt);
        patt_((fmt_FltStyle_scientific)(style)) {
            $break_(fmt_printF64(
                target,
                value,
                u8_c('e'),
                style.case_ == fmt_Case_upper,
                style.alt_form,
                sign,
                precision,
                has_precision
            ));
        } $end(patt);
    } $end(match)) $unscoped(expr);
    let len = try_(written);
    if (spec.layout.width == 0) return_ok(S_prefix((out)len));
    let padded = fmt_printPadded(
        out,
        A_prefix$((S_const$u8)(tmp)(len)),
        spec.layout.fill,
        spec.layout.align,
        spec.layout.width
    );
    return_ok(S_prefix((out)padded));
} $unscoped(fn);
fn_((fmt_formatPtr(S$u8 out, P_const$raw value, fmt_PtrSpec spec))(E$S$u8) $scope) {
    var_(tmp, A$$(128, u8));
    let target = spec.layout.width == 0 ? out : A_ref$((S$u8)tmp);
    let written = fmt_printPtr(target, value, u8_c('x'), spec.case_ == fmt_Case_upper, spec.alt_form);
    if (spec.layout.width == 0) return_ok(S_prefix((out)written));
    let padded = fmt_printPadded(
        out,
        A_prefix$((S_const$u8)(tmp)(written)),
        spec.layout.fill,
        spec.layout.align,
        spec.layout.width
    );
    return_ok(S_prefix((out)padded));
} $unscoped(fn);
fn_((fmt_formatASCII(S$u8 out, u8 value, fmt_CharSpec spec))(E$S$u8) $scope) {
    var_(tmp, A$$(4, u8));
    let target = spec.layout.width == 0 ? out : A_ref$((S$u8)tmp);
    let written = try_(fmt_printASCII(target, value));
    if (spec.layout.width == 0) return_ok(S_prefix((out)written));
    let padded = fmt_printPadded(
        out,
        A_prefix$((S_const$u8)(tmp)(written)),
        spec.layout.fill,
        spec.layout.align,
        spec.layout.width
    );
    return_ok(S_prefix((out)padded));
} $unscoped(fn);
fn_((fmt_formatUTF8(S$u8 out, u32 value, fmt_CharSpec spec))(E$S$u8) $scope) {
    var_(tmp, A$$(4, u8));
    let target = spec.layout.width == 0 ? out : A_ref$((S$u8)tmp);
    let written = try_(fmt_printUTF8(target, value));
    if (spec.layout.width == 0) return_ok(S_prefix((out)written));
    let padded = fmt_printPadded(
        out,
        A_prefix$((S_const$u8)(tmp)(written)),
        spec.layout.fill,
        spec.layout.align,
        spec.layout.width
    );
    return_ok(S_prefix((out)padded));
} $unscoped(fn);
fn_((fmt_formatStrZ0(S$u8 out, P_const$u8 value, fmt_StrSpec spec))(E$S$u8) $scope) {
    let written = fmt_printPadded(
        out,
        mem_spanZ0$u8(value),
        spec.layout.fill,
        spec.layout.align,
        spec.layout.width
    );
    return_ok(S_prefix((out)written));
} $unscoped(fn);
fn_((fmt_formatStr(S$u8 out, S_const$u8 value, fmt_StrSpec spec))(E$S$u8) $scope) {
    let written = fmt_printPadded(out, value, spec.layout.fill, spec.layout.align, spec.layout.width);
    return_ok(S_prefix((out)written));
} $unscoped(fn);
fn_((fmt_formatErr(S$u8 out, EAny value, fmt_ErrSpec spec))(E$S$u8) $scope) {
    var_(tmp, A$$(128, u8));
    let target = spec.layout.width == 0 ? out : A_ref$((S$u8)tmp);
    let written = fmt_printErr(target, value);
    if (spec.layout.width == 0) return_ok(S_prefix((out)written));
    let padded = fmt_printPadded(
        out,
        A_prefix$((S_const$u8)(tmp)(written)),
        spec.layout.fill,
        spec.layout.align,
        spec.layout.width
    );
    return_ok(S_prefix((out)padded));
} $unscoped(fn);

#define fmt__defineDirect(_$Type, _$Canonical, _$Formatter, _$SpecType) \
    fn_((pp_cat(fmt_format$, _$Type)(S$u8 out, _$Type value, _$SpecType spec))(E$S$u8)) { \
        return _$Formatter(out, as$(_$Canonical)(value), spec); \
    }
fmt__defineDirect(usize, u64, fmt_formatUInt, fmt_UIntSpec) fmt__defineDirect(u64, u64, fmt_formatUInt, fmt_UIntSpec) fmt__defineDirect(ulong, u64, fmt_formatUInt, fmt_UIntSpec) fmt__defineDirect(u32, u64, fmt_formatUInt, fmt_UIntSpec) fmt__defineDirect(u16, u64, fmt_formatUInt, fmt_UIntSpec) fmt__defineDirect(u8, u64, fmt_formatUInt, fmt_UIntSpec) fmt__defineDirect(isize, i64, fmt_formatIInt, fmt_IIntSpec) fmt__defineDirect(i64, i64, fmt_formatIInt, fmt_IIntSpec) fmt__defineDirect(ilong, i64, fmt_formatIInt, fmt_IIntSpec) fmt__defineDirect(i32, i64, fmt_formatIInt, fmt_IIntSpec) fmt__defineDirect(i16, i64, fmt_formatIInt, fmt_IIntSpec) fmt__defineDirect(i8, i64, fmt_formatIInt, fmt_IIntSpec) fmt__defineDirect(f64, f64, fmt_formatFlt, fmt_FltSpec) fmt__defineDirect(f32, f64, fmt_formatFlt, fmt_FltSpec)
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
    if (text.len == 0) return_ok(fmt_Size_32);
    if (text.len == 1) {
        let ch = *S_at((text)[0]);
        if (ch == u8_c('8')) return_ok(fmt_Size_8);
        if (ch == u8_c('h')) return_ok(fmt_Size_16);
        if (ch == u8_c('l')) return_ok(fmt_Size_long);
        if (ch == u8_c('z')) return_ok(fmt_Size_ptr);
        return_err(E_cause$fmt_InvalidSizeSpec());
    }
    if (text.len == 2) {
        let first = *S_at((text)[0]);
        let second = *S_at((text)[1]);
        if (first == u8_c('h') && second == u8_c('h')) return_ok(fmt_Size_8);
        if (first == u8_c('1') && second == u8_c('6')) return_ok(fmt_Size_16);
        if (first == u8_c('3') && second == u8_c('2')) return_ok(fmt_Size_32);
        if (first == u8_c('l') && second == u8_c('l')) return_ok(fmt_Size_64);
        if (first == u8_c('6') && second == u8_c('4')) return_ok(fmt_Size_64);
        return_err(E_cause$fmt_InvalidSizeSpec());
    }
    if (text.len == 4) {
        if (*S_at((text)[0]) == u8_c('l')
            && *S_at((text)[1]) == u8_c('o')
            && *S_at((text)[2]) == u8_c('n')
            && *S_at((text)[3]) == u8_c('g')) return_ok(fmt_Size_long);
        if (*S_at((text)[0]) == u8_c('s')
            && *S_at((text)[1]) == u8_c('i')
            && *S_at((text)[2]) == u8_c('z')
            && *S_at((text)[3]) == u8_c('e')) return_ok(fmt_Size_ptr);
    }
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

#define fmt__foldTupleStep(_$out, _$fmt, _$mask, _$tup, _$buf, _$cursor, _$occ_idx) do { \
    if ((_$cursor) < (_$fmt).len) { \
        let __fmt_step = fmt__nextFoldStep(_$fmt, _$cursor); \
        if (__fmt_step.kind == fmt_FoldKind_none) { \
            return fmt_printRuntime(_$out, _$fmt, _$tup); \
        } else if (__fmt_step.kind == fmt_FoldKind_literal_chunk) { \
            if (__fmt_step.literal_len > (_$buf).len) { \
                return_err(E_cause$TooSmallBuffer()); \
            } \
            (_$buf) = S_suffix(((_$buf))fmt__copy( \
                _$buf, \
                S_slice((_$fmt)$r(_$cursor, _$cursor + __fmt_step.literal_len)) \
            )); \
            (_$cursor) += __fmt_step.literal_len; \
        } else if (__fmt_step.kind == fmt_FoldKind_escaped_percent_brace) { \
            if (__fmt_step.literal_len + 2 > (_$buf).len) { \
                return_err(E_cause$TooSmallBuffer()); \
            } \
            (_$buf) = S_suffix(((_$buf))fmt__copy( \
                _$buf, \
                S_slice((_$fmt)$r(_$cursor, _$cursor + __fmt_step.literal_len)) \
            )); \
            (_$buf) = S_suffix(((_$buf))fmt__copy(_$buf, u8_l("%{"))); \
            (_$cursor) += __fmt_step.literal_len + 3; \
        } else if (__fmt_step.kind == fmt_FoldKind_placeholder) { \
            if (__fmt_step.body_start + __fmt_step.body_len >= (_$fmt).len) { \
                return fmt_printRuntime(_$out, _$fmt, _$tup); \
            } \
            if (__fmt_step.literal_len > (_$buf).len) { \
                return_err(E_cause$TooSmallBuffer()); \
            } \
            (_$buf) = S_suffix(((_$buf))fmt__copy( \
                _$buf, \
                S_slice((_$fmt)$r(_$cursor, _$cursor + __fmt_step.literal_len)) \
            )); \
            let __fmt_body_text = S_slice((_$fmt)$r( \
                __fmt_step.body_start, \
                __fmt_step.body_start + __fmt_step.body_len \
            )); \
            let __fmt_arg_idx = fmt__argIdxFolded(__fmt_body_text, _$occ_idx); \
            if (__fmt_arg_idx == fmt_max_arg_count) { \
                return fmt_printRuntime(_$out, _$fmt, _$tup); \
            } \
            if (__fmt_arg_idx >= (_$tup).fields.len) { \
                return_err(E_cause$fmt_TooFewArgs()); \
            } \
            let __fmt_body = fmt__parseBodyFolded(__fmt_body_text); \
            if (__fmt_body.type == u8_c('\0')) { \
                return fmt_printRuntime(_$out, _$fmt, _$tup); \
            } \
            let __fmt_field = u_Tup_fieldPtr(_$tup, __fmt_arg_idx); \
            let __fmt_written = try_(fmt__writeFoldedField( \
                _$buf, __fmt_body, __fmt_field, \
                bool_((_$mask) & (u16_(1) << __fmt_arg_idx)) \
            )); \
            (_$buf) = S_suffix(((_$buf))__fmt_written); \
            (_$cursor) = __fmt_step.body_start + __fmt_step.body_len + 1; \
            (_$occ_idx) += 1; \
        } \
    } \
} while (false)

fn_((fmt_print(S$u8 out, S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(E$S$u8) $scope) {
    if (!(isComptimeExpr(fmt.len)
          && (fmt.len == 0 || isComptimeExpr(*S_ptr(fmt))))) {
        return fmt_printRuntime(out, fmt, tup);
    }
    if (tup.fields.len > fmt_max_arg_count) return_err(E_cause$fmt_TooManyArgs());
    let valid_mask = tup.fields.len == 0 ? u16_(0)
        : tup.fields.len == 16 ? u16_(0xffff)
        : u16_((u16_(1) << tup.fields.len) - 1);
    claim_assert((comptime_mask & ~valid_mask) == 0);

    var buf = out;
    var cursor = usize_(0);
    var occ_idx = usize_(0);
    /* Physically expanded to fmt_fold_inline_step_budget == 4. */
    fmt__foldTupleStep(out, fmt, comptime_mask, tup, buf, cursor, occ_idx);
    fmt__foldTupleStep(out, fmt, comptime_mask, tup, buf, cursor, occ_idx);
    fmt__foldTupleStep(out, fmt, comptime_mask, tup, buf, cursor, occ_idx);
    fmt__foldTupleStep(out, fmt, comptime_mask, tup, buf, cursor, occ_idx);
    if (cursor < fmt.len) return fmt_printRuntime(out, fmt, tup);
    if (occ_idx < tup.fields.len) return_err(E_cause$fmt_TooManyArgs());
    return_ok(S_prefix((out)(out.len - buf.len)));
} $unscoped(fn);

#undef fmt__foldTupleStep

fn_((fmt_printRuntime(S$u8 out, S_const$u8 fmt, u_Tup tup))(E$S$u8) $scope) {
    var sink = (fmt_BufferWriter){ .out = out, .written = 0 };
    let writer = (io_Writer){ .ctx = &sink, .writeFn = fmt__bufferWriterWrite };
    try_(fmt_writeRuntime(writer, fmt, tup));
    return_ok(S_prefix((out)sink.written));
} $unscoped(fn);

fn_((fmt__bufferWriterWrite(P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    let sink = ptrAlignCast$((fmt_BufferWriter*)(ctx));
    if (bytes.len > sink->out.len - sink->written) return_err(E_cause$TooSmallBuffer());
    let_ignore = mem_copyBytes(
        S_slice((sink->out)$r(sink->written, sink->written + bytes.len)),
        bytes
    );
    sink->written += bytes.len;
    return_ok(bytes.len);
} $unscoped(fn);

fn_((fmt__writerWriteAll(io_Writer writer, S_const$u8 bytes))(E$void) $scope) {
    var written = usize_(0);
    while (written < bytes.len) {
        let chunk = try_(writer.writeFn(writer.ctx, S_suffix((bytes)written)));
        claim_assert(chunk != 0 && chunk <= bytes.len - written);
        written += chunk;
    }
    return_ok({});
} $unscoped(fn);

fn_((fmt__writerWritePadded(io_Writer writer, S_const$u8 text, fmt_LayoutSpec layout))(E$void) $scope) {
    if (text.len >= layout.width) return fmt__writerWriteAll(writer, text);
    let pad = layout.width - text.len;
    let left = layout.align == fmt_Align_right  ? pad
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

fn_((fmt_write(io_Writer writer, S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(E$void) $scope) {
    if (!(isComptimeExpr(fmt.len)
          && (fmt.len == 0 || isComptimeExpr(*S_ptr(fmt))))) {
        return fmt_writeRuntime(writer, fmt, tup);
    }
    var_(folded_mem, A$$(fmt_folded_writer_buffer_size, u8));
    let rendered_result = fmt_print(A_ref$((S$u8)folded_mem), fmt, comptime_mask, tup);
    if_err((rendered_result)(err)) {
        let too_small = E_cause$TooSmallBuffer();
        if (E_eql(&err, too_small.as_any)) return fmt_writeRuntime(writer, fmt, tup);
        return_err(err);
    } else_ok((rendered)) {
        return fmt__writerWriteAll(writer, rendered.as_const);
    }
    claim_unreachable;
} $unscoped(fn);

fn_((fmt_writeRuntime(io_Writer writer, S_const$u8 fmt, u_Tup tup))(E$void)) {
    return fmt__writeRuntimeFrom(writer, fmt_Iter_init(fmt), tup);
}

fn_((fmt__writeRuntimeFrom(io_Writer writer, fmt_Iter iter, u_Tup tup))(E$void) $scope) {
    if (tup.fields.len > fmt_max_arg_count) return_err(E_cause$fmt_TooManyArgs());
    while (true) {
        let event = orelse_((fmt__findEventRuntime(iter.rest))(break));
        match_(event) {
        patt_((fmt_EventPos_placeholder)(pos)) {
            try_(fmt__writerWriteAll(writer, S_prefix((iter.rest)pos)));
            let body_start = pos + 2;
            let close = try_(fmt__findCloseRuntime(S_suffix((iter.rest)body_start)));
            let body = S_prefix((S_suffix((iter.rest)body_start))close);
            let arg_idx = try_(fmt__argIdx(body, iter.occ_idx));
            if (arg_idx >= tup.fields.len) return_err(E_cause$fmt_TooFewArgs());
            try_(fmt__writeBodyToWriter(writer, body, u_Tup_fieldPtr(tup, arg_idx)));
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
    if (iter.occ_idx < tup.fields.len) return_err(E_cause$fmt_TooManyArgs());
    try_(fmt__writerWriteAll(writer, iter.rest));
    return_ok({});
} $unscoped(fn);

fn_((io_Writer_print(io_Writer writer, S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(E$void)) {
    return fmt_write(writer, fmt, comptime_mask, tup);
}

fn_((io_Writer_println(io_Writer writer, S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(E$void) $scope) {
    try_(fmt_write(writer, fmt, comptime_mask, tup));
    try_(fmt__writerWriteAll(writer, u8_l(io_nl)));
    return_ok({});
} $unscoped(fn);

fn_((io_stream__write(P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    return_ok(as$(usize)(fwrite(bytes.ptr, 1, bytes.len, ptrCast$((FILE*)(ctx)))));
} $unscoped(fn);

fn_((io_stream_print(S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(void)) {
    let writer = (io_Writer){ .ctx = ptrCast$((P$raw)(stdout)), .writeFn = io_stream__write };
    let_ignore = catch_((fmt_write(writer, fmt, comptime_mask, tup))($ignore, $do_nothing));
}

fn_((io_stream_println(S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(void)) {
    let writer = (io_Writer){ .ctx = ptrCast$((P$raw)(stdout)), .writeFn = io_stream__write };
    let_ignore = catch_((io_Writer_println(writer, fmt, comptime_mask, tup))($ignore, $do_nothing));
}

fn_((io_stream_eprint(S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(void)) {
    let writer = (io_Writer){ .ctx = ptrCast$((P$raw)(stderr)), .writeFn = io_stream__write };
    let_ignore = catch_((fmt_write(writer, fmt, comptime_mask, tup))($ignore, $do_nothing));
}

fn_((io_stream_eprintln(S_const$u8 fmt, u16 comptime_mask, u_Tup tup))(void)) {
    let writer = (io_Writer){ .ctx = ptrCast$((P$raw)(stderr)), .writeFn = io_stream__write };
    let_ignore = catch_((io_Writer_println(writer, fmt, comptime_mask, tup))($ignore, $do_nothing));
}

fn_((fmt_printPadded(S$u8 out, S_const$u8 text, u8 fill, fmt_Align align, usize width))(usize)) {
    if (text.len >= width) return fmt__copy(out, text);
    let pad = width - text.len;
    let left = align == fmt_Align_right ? pad : align == fmt_Align_center ? pad / 2
                                                                          : usize_(0);
    let right = pad - left;
    loop_inline_(for_)(($rt(left))(i)) *S_at((out)[i]) = fill $end(for);
    let_ignore = mem_copyBytes(S_suffix((out)left), text);
    loop_inline_(for_)(($rt(right))(i)) *S_at((out)[left + text.len + i]) = fill $end(for);
    return width;
};

fn_((fmt__printPaddedFolded(S$u8 out, S_const$u8 text, u8 fill, fmt_Align align, usize width))(usize)) {
    if (text.len >= width) return fmt__copy(out, text);
    let pad = width - text.len;
    let left = align == fmt_Align_right  ? pad
             : align == fmt_Align_center ? pad / 2
                                         : usize_(0);
    let right = pad - left;
    for (usize i = 0; i < left; ++i) *S_at((out)[i]) = fill;
    let_ignore = mem_copyBytes(S_suffix((out)left), text);
    for (usize i = 0; i < right; ++i) *S_at((out)[left + text.len + i]) = fill;
    return width;
}

$attr($maybe_unused)
fn_((fmt_printVoid(S$u8 out, Void value))(usize)) { return $ignore_void out, $ignore_void value, 0; };

fn_((fmt_printBool(S$u8 out, bool value, bool upper))(usize)) {
    return fmt__copy(out, value ? (upper ? u8_l("TRUE") : u8_l("true")) : (upper ? u8_l("FALSE") : u8_l("false")));
};

fn_((fmt__printBoolFolded(S$u8 out, bool value, bool upper))(usize)) {
    return fmt__copy(out, value ? (upper ? u8_l("TRUE") : u8_l("true")) : (upper ? u8_l("FALSE") : u8_l("false")));
}

fn_((fmt_printU64(S$u8 out, u64 value, u8 mode, bool upper, bool alt))(usize)) {
    if (mode == u8_c('x') || mode == u8_c('X')) return fmt_printU64Hex(out, value, upper, alt);
    if (mode == u8_c('o')) return fmt_printU64Oct(out, value, alt);
    if (mode == u8_c('b')) return fmt_printU64Bin(out, value, upper, alt);
    if (mode == u8_c(' ') || mode == u8_c('d')) return fmt_printU64Dec(out, value);
    claim_unreachable;
};

fn_((fmt__printU64Folded(S$u8 out, u64 value, u8 mode, bool upper, bool alt))(usize)) {
    var written = usize_(0);
    if (mode == u8_c('x') || mode == u8_c('X')) {
        if (alt) {
            *S_at((out)[written++]) = u8_c('0');
            *S_at((out)[written++]) = u8_c('x');
        }
        let digits = A_from$((u8){
            fmt_digitToChar((value >> 60) & 0xf, upper),
            fmt_digitToChar((value >> 56) & 0xf, upper),
            fmt_digitToChar((value >> 52) & 0xf, upper),
            fmt_digitToChar((value >> 48) & 0xf, upper),
            fmt_digitToChar((value >> 44) & 0xf, upper),
            fmt_digitToChar((value >> 40) & 0xf, upper),
            fmt_digitToChar((value >> 36) & 0xf, upper),
            fmt_digitToChar((value >> 32) & 0xf, upper),
            fmt_digitToChar((value >> 28) & 0xf, upper),
            fmt_digitToChar((value >> 24) & 0xf, upper),
            fmt_digitToChar((value >> 20) & 0xf, upper),
            fmt_digitToChar((value >> 16) & 0xf, upper),
            fmt_digitToChar((value >> 12) & 0xf, upper),
            fmt_digitToChar((value >> 8) & 0xf, upper),
            fmt_digitToChar((value >> 4) & 0xf, upper),
            fmt_digitToChar(value & 0xf, upper),
        });
        let digit_count = value == 0
            ? usize_(1)
            : (usize_(64) - as$(usize)(__builtin_clzll(value)) + 3) / 4;
        return written + fmt__copy(
            S_suffix((out)written),
            A_suffix$((S_const$u8)(digits)(A_len(digits) - digit_count))
        );
    }
    if (mode == u8_c(' ') || mode == u8_c('d')) {
        let digits = A_from$((u8){
            fmt_digitToChar((value / u64_(10000000000000000000ull)) % 10, false),
            fmt_digitToChar((value / u64_(1000000000000000000)) % 10, false),
            fmt_digitToChar((value / u64_(100000000000000000)) % 10, false),
            fmt_digitToChar((value / u64_(10000000000000000)) % 10, false),
            fmt_digitToChar((value / u64_(1000000000000000)) % 10, false),
            fmt_digitToChar((value / u64_(100000000000000)) % 10, false),
            fmt_digitToChar((value / u64_(10000000000000)) % 10, false),
            fmt_digitToChar((value / u64_(1000000000000)) % 10, false),
            fmt_digitToChar((value / u64_(100000000000)) % 10, false),
            fmt_digitToChar((value / u64_(10000000000)) % 10, false),
            fmt_digitToChar((value / u64_(1000000000)) % 10, false),
            fmt_digitToChar((value / u64_(100000000)) % 10, false),
            fmt_digitToChar((value / u64_(10000000)) % 10, false),
            fmt_digitToChar((value / u64_(1000000)) % 10, false),
            fmt_digitToChar((value / u64_(100000)) % 10, false),
            fmt_digitToChar((value / u64_(10000)) % 10, false),
            fmt_digitToChar((value / u64_(1000)) % 10, false),
            fmt_digitToChar((value / u64_(100)) % 10, false),
            fmt_digitToChar((value / u64_(10)) % 10, false),
            fmt_digitToChar(value % 10, false),
        });
        let digit_count =
            value >= u64_(10000000000000000000ull) ? usize_(20) :
            value >= u64_(1000000000000000000) ? usize_(19) :
            value >= u64_(100000000000000000) ? usize_(18) :
            value >= u64_(10000000000000000) ? usize_(17) :
            value >= u64_(1000000000000000) ? usize_(16) :
            value >= u64_(100000000000000) ? usize_(15) :
            value >= u64_(10000000000000) ? usize_(14) :
            value >= u64_(1000000000000) ? usize_(13) :
            value >= u64_(100000000000) ? usize_(12) :
            value >= u64_(10000000000) ? usize_(11) :
            value >= u64_(1000000000) ? usize_(10) :
            value >= u64_(100000000) ? usize_(9) :
            value >= u64_(10000000) ? usize_(8) :
            value >= u64_(1000000) ? usize_(7) :
            value >= u64_(100000) ? usize_(6) :
            value >= u64_(10000) ? usize_(5) :
            value >= u64_(1000) ? usize_(4) :
            value >= u64_(100) ? usize_(3) :
            value >= u64_(10) ? usize_(2) : usize_(1);
        return fmt__copy(
            out,
            A_suffix$((S_const$u8)(digits)(A_len(digits) - digit_count))
        );
    }

    var base = mode == u8_c('o') ? usize_(8) : usize_(2);
    if (mode == u8_c('o')) {
        if (alt) *S_at((out)[written++]) = u8_c('0');
    } else if (mode == u8_c('b') && alt) {
        *S_at((out)[written++]) = u8_c('0');
        *S_at((out)[written++]) = upper ? u8_c('B') : u8_c('b');
    }
    var_(tmp, A$$(72, u8)) = A_zero();
    var pos = A_len(tmp);
    if (value == 0) *A_at((tmp)[--pos]) = u8_c('0');
    loop_inline_(while)(value != 0) {
        *A_at((tmp)[--pos]) = fmt_digitToChar(value % base, upper);
        value /= base;
    }
    return written + fmt__copy(S_suffix((out)written), A_suffix$((S_const$u8)(tmp)(pos)));
}

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

fn_((fmt__printI64Folded(S$u8 out, i64 value, u8 mode, bool upper, bool alt, u8 sign))(usize)) {
    var written = usize_(0);
    let neg = value < 0;
    if (neg) *S_at((out)[written++]) = u8_c('-');
    else if (sign == u8_c('+') || sign == u8_c(' ')) *S_at((out)[written++]) = sign;
    let magnitude = neg ? as$(u64)(-(value + 1)) + u64_(1) : as$(u64)(value);
    return written + fmt__printU64Folded(S_suffix((out)written), magnitude, mode, upper, alt);
}

fn_((fmt_printI64Dec(S$u8 out, i64 value, u8 sign))(usize)) {
    var written = usize_(0);
    let neg = value < 0;
    if (neg) *S_at((out)[written++]) = u8_c('-');
    else if (sign == u8_c('+') || sign == u8_c(' ')) *S_at((out)[written++]) = sign;
    let magnitude = neg
                      ? as$(u64)(-(value + 1)) + 1
                      : as$(u64)(value);
    return written + fmt_printU64(S_suffix((out)written), magnitude, u8_c('d'), false, false);
};

fn_((fmt_printF64(S$u8 out, f64 value, u8 mode, bool upper, bool alt, u8 sign, usize precision, bool has_precision))(E$usize) $scope) {
    if (!has_precision) precision = 6;
    if (precision > 18) return_err(E_cause$fmt_InvalidPrecisionSpec());
    if (mode != u8_c(' ') && mode != u8_c('d') && mode != u8_c('e')) {
        return_err(E_cause$fmt_InvalidTypeSpec());
    }

    var_(text, A$$(512, u8)) = A_zero();
    var_(written, int);
#define fmt__snprintfF64(_$Format) \
    snprintf(as$(char*)(A_ptr(text)), A_len(text), _$Format, as$(int)(precision), value)
    if (mode == u8_c('e') && upper) {
        written = alt
                    ? (sign == u8_c('+')   ? fmt__snprintfF64("%#+.*E")
                       : sign == u8_c(' ') ? fmt__snprintfF64("%# .*E")
                                           : fmt__snprintfF64("%#.*E"))
                    : (sign == u8_c('+')   ? fmt__snprintfF64("%+.*E")
                       : sign == u8_c(' ') ? fmt__snprintfF64("% .*E")
                                           : fmt__snprintfF64("%.*E"));
    } else if (mode == u8_c('e')) {
        written = alt
                    ? (sign == u8_c('+')   ? fmt__snprintfF64("%#+.*e")
                       : sign == u8_c(' ') ? fmt__snprintfF64("%# .*e")
                                           : fmt__snprintfF64("%#.*e"))
                    : (sign == u8_c('+')   ? fmt__snprintfF64("%+.*e")
                       : sign == u8_c(' ') ? fmt__snprintfF64("% .*e")
                                           : fmt__snprintfF64("%.*e"));
    } else {
        written = alt
                    ? (sign == u8_c('+')   ? fmt__snprintfF64("%#+.*f")
                       : sign == u8_c(' ') ? fmt__snprintfF64("%# .*f")
                                           : fmt__snprintfF64("%#.*f"))
                    : (sign == u8_c('+')   ? fmt__snprintfF64("%+.*f")
                       : sign == u8_c(' ') ? fmt__snprintfF64("% .*f")
                                           : fmt__snprintfF64("%.*f"));
    }
#undef fmt__snprintfF64
    if (written < 0) return_err(E_cause$fmt_InvalidFlt());
    let len = as$(usize)(written);
    if (len >= A_len(text) || len > out.len) return_err(E_cause$TooSmallBuffer());
    let_ignore = mem_copyBytes(out, A_prefix$((S_const$u8)(text)(len)));
    return_ok(len);
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

$attr($maybe_unused)
fn_((fmt__parseSpec(u8 type, u8 mode, bool alt, u8 sign, bool has_sign, bool has_layout, fmt_LayoutSpec layout, usize precision, bool has_precision))(E$fmt_Spec) $scope) {
    if (type == u8_c('0')) {
        if (mode != u8_c(' ') || alt || has_sign || has_layout || has_precision) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
        return_ok(union_of((fmt_Spec_void){}));
    }
    if (type == u8_c('b') || type == u8_c('B')) {
        if (mode != u8_c(' ') || alt || has_sign || has_precision) return_err(E_cause$fmt_InvalidTypeSpec());
        var_(case_, fmt_Case) = type == u8_c('B') ? fmt_Case_upper : fmt_Case_lower;
        return_ok(union_of((fmt_Spec_bool)((fmt_BoolSpec){ .layout = layout, .case_ = case_ })));
    }
    if (type == u8_c('u') || type == u8_c('U')) {
        if (has_sign || has_precision || mode == u8_c('e')) return_err(E_cause$fmt_InvalidTypeSpec());
        var_(style, fmt_IntStyle);
        if (mode == u8_c(' ') || mode == u8_c('d')) {
            if (type == u8_c('U') || alt) return_err(E_cause$fmt_InvalidTypeSpec());
            style = (fmt_IntStyle)union_of((fmt_IntStyle_decimal){});
        } else if (mode == u8_c('x') || mode == u8_c('X')) {
            var_(case_, fmt_Case) = type == u8_c('U') || mode == u8_c('X') ? fmt_Case_upper : fmt_Case_lower;
            style = (fmt_IntStyle)union_of((fmt_IntStyle_hex)(((fmt_HexStyle){ .case_ = case_, .alt_form = alt })));
        } else if (mode == u8_c('o')) {
            if (type == u8_c('U')) return_err(E_cause$fmt_InvalidTypeSpec());
            style = (fmt_IntStyle)union_of((fmt_IntStyle_octal)(alt));
        } else {
            claim_assert(mode == u8_c('b'));
            if (type == u8_c('U') && !alt) return_err(E_cause$fmt_InvalidTypeSpec());
            var_(prefix, fmt_BinPrefix) = !alt              ? fmt_BinPrefix_none
                                        : type == u8_c('U') ? fmt_BinPrefix_upper
                                                            : fmt_BinPrefix_lower;
            style = (fmt_IntStyle)union_of((fmt_IntStyle_binary)(prefix));
        }
        return_ok(union_of((fmt_Spec_uint)((fmt_UIntSpec){
            .layout = layout,
            .style = style,
        })));
    }
    if (type == u8_c('i') || type == u8_c('I')) {
        if (has_precision || mode == u8_c('e')) return_err(E_cause$fmt_InvalidTypeSpec());
        var_(style, fmt_IntStyle);
        if (mode == u8_c(' ') || mode == u8_c('d')) {
            if (type == u8_c('I') || alt) return_err(E_cause$fmt_InvalidTypeSpec());
            style = (fmt_IntStyle)union_of((fmt_IntStyle_decimal){});
        } else if (mode == u8_c('x') || mode == u8_c('X')) {
            var_(case_, fmt_Case) = type == u8_c('I') || mode == u8_c('X') ? fmt_Case_upper : fmt_Case_lower;
            style = (fmt_IntStyle)union_of((fmt_IntStyle_hex)(((fmt_HexStyle){ .case_ = case_, .alt_form = alt })));
        } else if (mode == u8_c('o')) {
            if (type == u8_c('I')) return_err(E_cause$fmt_InvalidTypeSpec());
            style = (fmt_IntStyle)union_of((fmt_IntStyle_octal)(alt));
        } else {
            claim_assert(mode == u8_c('b'));
            if (type == u8_c('I') && !alt) return_err(E_cause$fmt_InvalidTypeSpec());
            var_(prefix, fmt_BinPrefix) = !alt              ? fmt_BinPrefix_none
                                        : type == u8_c('I') ? fmt_BinPrefix_upper
                                                            : fmt_BinPrefix_lower;
            style = (fmt_IntStyle)union_of((fmt_IntStyle_binary)(prefix));
        }
        var_(sign_mode, fmt_Sign) = sign == u8_c('+') ? fmt_Sign_always
                                  : sign == u8_c(' ') ? fmt_Sign_space
                                                      : fmt_Sign_auto;
        return_ok(union_of((fmt_Spec_iint)((fmt_IIntSpec){
            .layout = layout,
            .style = style,
            .sign = sign_mode,
        })));
    }
    if (type == u8_c('f') || type == u8_c('F')) {
        if (mode == u8_c('x') || mode == u8_c('X') || mode == u8_c('o') || mode == u8_c('b')) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
        var_(sign_mode, fmt_Sign) = sign == u8_c('+') ? fmt_Sign_always
                                  : sign == u8_c(' ') ? fmt_Sign_space
                                                      : fmt_Sign_auto;
        var_(style, fmt_FltStyle);
        if (mode == u8_c('e')) {
            var_(case_, fmt_Case) = type == u8_c('F') ? fmt_Case_upper : fmt_Case_lower;
            style = (fmt_FltStyle)union_of((fmt_FltStyle_scientific)(((fmt_FltScientificStyle){
                .case_ = case_,
                .alt_form = alt,
            })));
        } else {
            claim_assert(mode == u8_c(' ') || mode == u8_c('d'));
            if (type == u8_c('F')) return_err(E_cause$fmt_InvalidTypeSpec());
            style = (fmt_FltStyle)union_of((fmt_FltStyle_decimal)(((fmt_FltDecimalStyle){ .alt_form = alt })));
        }
        var_(precision_opt, O$usize) = none$((O$usize));
        if (has_precision) precision_opt = (O$usize)some(precision);
        return_ok(union_of((fmt_Spec_flt)((fmt_FltSpec){
            .layout = layout,
            .style = style,
            .sign = sign_mode,
            .precision = precision_opt,
        })));
    }
    if (type == u8_c('p') || type == u8_c('P')) {
        if (has_sign || has_precision || (mode != u8_c(' ') && mode != u8_c('x') && mode != u8_c('X'))) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
        var_(case_, fmt_Case) = type == u8_c('P') || mode == u8_c('X') ? fmt_Case_upper : fmt_Case_lower;
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

$attr($maybe_unused)
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
    var has_layout = false;
    var width = usize_(0);
    var precision = usize_(6);
    var has_precision = false;
    if (pos < body.len) {
        has_layout = true;
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
    if ((type == u8_c('f') || type == u8_c('F'))
        && size != fmt_Size_32
        && size != fmt_Size_64) {
        return_err(E_cause$fmt_InvalidSizeSpec());
    }
    if (type != u8_c('u')
        && type != u8_c('U')
        && type != u8_c('i')
        && type != u8_c('I')
        && type != u8_c('f')
        && type != u8_c('F')
        && size != fmt_Size_32) {
        return_err(E_cause$fmt_InvalidSizeSpec());
    }
    let spec = try_(fmt__parseSpec(type, mode, alt, sign, has_sign, has_layout, layout, precision, has_precision));
    return_ok(((fmt_ParsedBody){ .wrapped = wrapped, .size = size, .spec = spec }));
} $unscoped(fn);

$attr($must_check $inline_never)
fn_((fmt__parseBodyRuntime(S_const$u8 body))(E$fmt_RuntimeBody) $scope) {
    var pos = usize_(0);
    if (body.len == 0) return_err(E_cause$fmt_UnexpectedEndFormat());
    if (*S_at((body)[0]) == u8_c('[')) {
        pos = 1;
        while (pos < body.len && *S_at((body)[pos]) != u8_c(']')) pos += 1;
        if (pos >= body.len) return_err(E_cause$fmt_InvalidIdx());
        pos += 1;
    }
    var wrapped = u8_c('\0');
    if (pos < body.len && (*S_at((body)[pos]) == u8_c('?') || *S_at((body)[pos]) == u8_c('!'))) {
        wrapped = *S_at((body)[pos++]);
    }
    if (pos >= body.len) return_err(E_cause$fmt_UnexpectedEndFormat());
    let type = *S_at((body)[pos++]);
    var size_end = pos;
    while (size_end < body.len && *S_at((body)[size_end]) != u8_c('(') && *S_at((body)[size_end]) != u8_c(':')) {
        size_end += 1;
    }
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
    var has_layout = false;
    var width = usize_(0);
    var precision = usize_(6);
    var has_precision = false;
    if (pos < body.len) {
        has_layout = true;
        if (*S_at((body)[pos++]) != u8_c(':')) return_err(E_cause$fmt_InvalidTypeSpec());
        if (pos >= body.len) return_err(E_cause$fmt_InvalidWidthSpec());
        if (pos + 1 < body.len && fmt_isAlign(*S_at((body)[pos + 1]))) {
            fill = *S_at((body)[pos++]);
        } else if (!fmt_isAlign(*S_at((body)[pos])) && *S_at((body)[pos]) != u8_c('+') && *S_at((body)[pos]) != u8_c('-') && *S_at((body)[pos]) != u8_c(' ') && *S_at((body)[pos]) != u8_c('.') && !ascii_isDigit(*S_at((body)[pos]))) {
            return_err(E_cause$fmt_InvalidAlignSpec());
        }
        if (pos < body.len && fmt_isAlign(*S_at((body)[pos]))) {
            align = try_(fmt_Align_fromByte(*S_at((body)[pos++])));
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
            if (pos >= body.len || !ascii_isDigit(*S_at((body)[pos]))) {
                return_err(E_cause$fmt_InvalidPrecisionSpec());
            }
            precision = 0;
            has_precision = true;
            while (pos < body.len && ascii_isDigit(*S_at((body)[pos]))) {
                precision = precision * 10 + as$(usize)(*S_at((body)[pos++]) - u8_c('0'));
            }
        }
        if (pos != body.len) return_err(E_cause$fmt_InvalidWidthSpec());
    }

    let is_uint = type == u8_c('u') || type == u8_c('U');
    let is_iint = type == u8_c('i') || type == u8_c('I');
    let is_flt = type == u8_c('f') || type == u8_c('F');
    let is_sizable = is_uint || is_iint || is_flt;
    if (is_flt && size != fmt_Size_32 && size != fmt_Size_64) return_err(E_cause$fmt_InvalidSizeSpec());
    if (!is_sizable && size != fmt_Size_32) return_err(E_cause$fmt_InvalidSizeSpec());

    if (type == u8_c('0')) {
        if (mode != u8_c(' ') || alt || has_sign || has_layout || has_precision) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
    } else if (type == u8_c('b') || type == u8_c('B')) {
        if (mode != u8_c(' ') || alt || has_sign || has_precision) return_err(E_cause$fmt_InvalidTypeSpec());
    } else if (is_uint || is_iint) {
        if ((is_uint && has_sign) || has_precision || mode == u8_c('e')) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
        let upper_type = type == u8_c('U') || type == u8_c('I');
        if ((mode == u8_c(' ') || mode == u8_c('d')) && (upper_type || alt)) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
        if (mode == u8_c('o') && upper_type) return_err(E_cause$fmt_InvalidTypeSpec());
        if (mode == u8_c('b') && upper_type && !alt) return_err(E_cause$fmt_InvalidTypeSpec());
    } else if (is_flt) {
        if (mode == u8_c('x') || mode == u8_c('X') || mode == u8_c('o') || mode == u8_c('b')) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
        if (mode != u8_c('e') && type == u8_c('F')) return_err(E_cause$fmt_InvalidTypeSpec());
    } else if (type == u8_c('p') || type == u8_c('P')) {
        if (has_sign || has_precision || (mode != u8_c(' ') && mode != u8_c('x') && mode != u8_c('X'))) {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
    } else if (type == u8_c('c') || type == u8_c('C') || type == u8_c('z') || type == u8_c('s') || type == u8_c('e')) {
        if (mode != u8_c(' ') || alt || has_sign || has_precision) return_err(E_cause$fmt_InvalidTypeSpec());
    } else {
        return_err(E_cause$fmt_InvalidTypeSpec());
    }

    return_ok(((fmt_RuntimeBody){
        .wrapped = wrapped,
        .type = type,
        .size = size,
        .mode = mode,
        .alt = alt,
        .layout = (fmt_LayoutSpec){ .fill = fill, .align = align, .width = width },
        .sign = sign,
        .has_sign = has_sign,
        .precision = precision,
        .has_precision = has_precision,
    }));
} $unscoped(fn);

$attr($maybe_unused)
fn_((fmt__writeParsed(S$u8 out, fmt_ParsedBody parsed, u_P_const$raw field, bool fold_values))(E$usize) $scope) {
    let layout = fmt__specLayout(parsed.spec);
    var_(tmp, A$$(512, u8)) = A_zero();
    let target = layout.width == 0 ? out : A_ref$((S$u8)tmp);
    let written = try_(fmt__writeValue(target, parsed.wrapped, parsed.size, parsed.spec, field, fold_values));
    if (layout.width == 0) return_ok(written);
    let text = S_prefix((A_ref$((S_const$u8)tmp))written);
    return_ok(fold_values ? fmt__printPaddedFolded(out, text, layout.fill, layout.align, layout.width) : fmt_printPadded(out, text, layout.fill, layout.align, layout.width));
} $unscoped(fn);

fn_((fmt__writeBodyToWriter(io_Writer writer, S_const$u8 body, u_P_const$raw field))(E$void) $scope) {
    let parsed = try_(fmt__parseBodyRuntime(body));
    let layout = parsed.layout;
    if (parsed.wrapped == u8_c('\0') && parsed.type == u8_c('s')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(S_const$u8)));
        return fmt__writerWritePadded(writer, *u_castP$((const S_const$u8*)(field)), layout);
    }
    if (parsed.wrapped == u8_c('\0') && parsed.type == u8_c('z')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(P_const$u8)));
        let text = mem_spanZ0$u8(*u_castP$((const P_const$u8*)(field)));
        return fmt__writerWritePadded(writer, text, layout);
    }
    if (parsed.wrapped == u8_c('?') && parsed.type == u8_c('s')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(O$S_const$u8)));
        let value = *u_castP$((const O$S_const$u8*)(field));
        if_none((value)) return fmt__writerWritePadded(writer, u8_l("none"), layout);
        else_some((payload)) return fmt__writerWritePadded(writer, payload, layout);
    }
    if (parsed.wrapped == u8_c('?') && parsed.type == u8_c('z')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(O$P_const$u8)));
        let value = *u_castP$((const O$P_const$u8*)(field));
        if_none((value)) return fmt__writerWritePadded(writer, u8_l("none"), layout);
        else_some((payload)) return fmt__writerWritePadded(writer, mem_spanZ0$u8(payload), layout);
    }
    if (parsed.wrapped == u8_c('!') && parsed.type == u8_c('s')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(E$S_const$u8)));
        let value = *u_castP$((const E$S_const$u8*)(field));
        if_err((value)(err)) {
            var_(err_mem, A$$(128, u8));
            let written = fmt_printErr(A_ref$((S$u8)err_mem), err);
            return fmt__writerWritePadded(writer, A_prefix$((S_const$u8)(err_mem)(written)), layout);
        } else_ok((payload)) return fmt__writerWritePadded(writer, payload, layout);
    }
    if (parsed.wrapped == u8_c('!') && parsed.type == u8_c('z')) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(E$P_const$u8)));
        let value = *u_castP$((const E$P_const$u8*)(field));
        if_err((value)(err)) {
            var_(err_mem, A$$(128, u8));
            let written = fmt_printErr(A_ref$((S$u8)err_mem), err);
            return fmt__writerWritePadded(writer, A_prefix$((S_const$u8)(err_mem)(written)), layout);
        } else_ok((payload)) return fmt__writerWritePadded(writer, mem_spanZ0$u8(payload), layout);
    }

    if (parsed.type == u8_c('f') || parsed.type == u8_c('F')) {
        var_(flt_mem, A$$(512, u8));
        let written = try_(fmt__writeValueRuntime(A_ref$((S$u8)flt_mem), parsed, field));
        return fmt__writerWritePadded(
            writer,
            A_prefix$((S_const$u8)(flt_mem)(written)),
            layout
        );
    }

    var_(value_mem, A$$(128, u8));
    let written = try_(fmt__writeValueRuntime(A_ref$((S$u8)value_mem), parsed, field));
    return fmt__writerWritePadded(
        writer,
        A_prefix$((S_const$u8)(value_mem)(written)),
        layout
    );
} $unscoped(fn);

$attr($maybe_unused)
fn_((fmt__writeParsedToWriter(io_Writer writer, fmt_ParsedBody parsed, u_P_const$raw field, bool fold_values))(E$void) $scope) {
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
    if (parsed.wrapped == u8_c('?') && parsed.spec.tag == fmt_Spec_str_z0) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(O$P_const$u8)));
        let value = *u_castP$((const O$P_const$u8*)(field));
        if_none((value)) return fmt__writerWritePadded(writer, u8_l("none"), layout);
        else_some((payload)) return fmt__writerWritePadded(writer, mem_spanZ0$u8(payload), layout);
    }
    if (parsed.wrapped == u8_c('!') && parsed.spec.tag == fmt_Spec_str) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(E$S_const$u8)));
        let value = *u_castP$((const E$S_const$u8*)(field));
        if_err((value)(err)) {
            var_(err_mem, A$$(128, u8)) = A_zero();
            let written = fmt_printErr(A_ref$((S$u8)err_mem), err);
            return fmt__writerWritePadded(writer, A_prefix$((S_const$u8)(err_mem)(written)), layout);
        } else_ok((payload)) return fmt__writerWritePadded(writer, payload, layout);
    }
    if (parsed.wrapped == u8_c('!') && parsed.spec.tag == fmt_Spec_str_z0) {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(E$P_const$u8)));
        let value = *u_castP$((const E$P_const$u8*)(field));
        if_err((value)(err)) {
            var_(err_mem, A$$(128, u8)) = A_zero();
            let written = fmt_printErr(A_ref$((S$u8)err_mem), err);
            return fmt__writerWritePadded(writer, A_prefix$((S_const$u8)(err_mem)(written)), layout);
        } else_ok((payload)) return fmt__writerWritePadded(writer, mem_spanZ0$u8(payload), layout);
    }

    if (parsed.spec.tag == fmt_Spec_flt) {
        var_(flt_mem, A$$(512, u8));
        let written = try_(fmt__writeValue(
            A_ref$((S$u8)flt_mem),
            parsed.wrapped,
            parsed.size,
            parsed.spec,
            field,
            fold_values
        ));
        return fmt__writerWritePadded(
            writer,
            A_prefix$((S_const$u8)(flt_mem)(written)),
            layout
        );
    }

    var_(value_mem, A$$(128, u8));
    let written = try_(fmt__writeValue(
        A_ref$((S$u8)value_mem),
        parsed.wrapped,
        parsed.size,
        parsed.spec,
        field,
        fold_values
    ));
    return fmt__writerWritePadded(
        writer,
        A_prefix$((S_const$u8)(value_mem)(written)),
        layout
    );
} $unscoped(fn);


fn_((fmt__nextFoldStep(S_const$u8 fmt, usize cursor))(fmt_FoldCursorStep)) {
    var_(step, fmt_FoldCursorStep) = (fmt_FoldCursorStep){
        .kind = fmt_FoldKind_none,
        .literal_len = 0,
        .body_start = 0,
        .body_len = 0,
    };
    if (cursor >= fmt.len) return step;
    let scan_len = pri_min(fmt.len - cursor, usize_(fmt_fold_event_scan_bytes));
    usize search_at = cursor;
    const u8* brace = (const u8*)__builtin_memchr(S_ptr(fmt) + search_at, u8_c('{'), scan_len);
#define fmt__acceptBraceCandidate() do { \
    if (brace != null) { \
        let pos = as$(usize)(brace - S_ptr(fmt)); \
        if (pos > cursor && *S_at((fmt)[pos - 1]) == u8_c('%')) { \
            step.kind = pos > cursor + 1 && *S_at((fmt)[pos - 2]) == u8_c('%') \
                ? fmt_FoldKind_escaped_percent_brace \
                : fmt_FoldKind_placeholder; \
            let event_pos = step.kind == fmt_FoldKind_placeholder ? pos - 1 : pos - 2; \
            step.literal_len = event_pos - cursor; \
            if (step.kind == fmt_FoldKind_placeholder) { \
                step.body_start = pos + 1; \
                let body_scan_len = pri_min(fmt.len - step.body_start, usize_(fmt_fold_body_scan_bytes)); \
                const u8* close = (const u8*)__builtin_memchr(S_ptr(fmt) + step.body_start, u8_c('}'), body_scan_len); \
                if (close == null) step.kind = fmt_FoldKind_none; \
                else step.body_len = as$(usize)(close - (S_ptr(fmt) + step.body_start)); \
            } \
            return step; \
        } \
        search_at = pos + 1; \
        let searched = search_at - cursor; \
        brace = searched < scan_len \
            ? (const u8*)__builtin_memchr(S_ptr(fmt) + search_at, u8_c('{'), scan_len - searched) \
            : null; \
    } \
} while (false)
    fmt__acceptBraceCandidate();
    fmt__acceptBraceCandidate();
    fmt__acceptBraceCandidate();
    fmt__acceptBraceCandidate();
#undef fmt__acceptBraceCandidate
    if (brace != null) return step;
    step.kind = fmt_FoldKind_literal_chunk;
    step.literal_len = scan_len;
    return step;
}

fn_((fmt__argIdxFolded(S_const$u8 body, usize occ_idx))(usize)) {
    if (body.len == 0 || *S_at((body)[0]) != u8_c('[')) return occ_idx;
    const u8* close = (const u8*)__builtin_memchr(S_ptr(body) + 1, u8_c(']'), body.len - 1);
    if (close == null) return usize_(fmt_max_arg_count);
    let digits = as$(usize)(close - (S_ptr(body) + 1));
    if (digits == 0 || digits > 2) return usize_(fmt_max_arg_count);
    let d0 = *S_at((body)[1]);
    if (!ascii_isDigit(d0)) return usize_(fmt_max_arg_count);
    var idx = as$(usize)(d0 - u8_c('0'));
    if (digits == 2) {
        let d1 = *S_at((body)[2]);
        if (!ascii_isDigit(d1)) return usize_(fmt_max_arg_count);
        idx = idx * 10 + as$(usize)(d1 - u8_c('0'));
    }
    return idx < fmt_max_arg_count ? idx : usize_(fmt_max_arg_count);
}

fn_((fmt__parseBodyFolded(S_const$u8 body))(fmt_RuntimeBody)) {
    var_(result, fmt_RuntimeBody) = (fmt_RuntimeBody){
        .wrapped = u8_c('\0'), .type = u8_c('\0'), .size = fmt_Size_32,
        .mode = u8_c(' '), .alt = false,
        .layout = (fmt_LayoutSpec){ .fill = u8_c(' '), .align = fmt_Align_left, .width = 0 },
        .sign = u8_c('-'), .has_sign = false,
        .precision = 6, .has_precision = false,
    };
    if (body.len == 0) return result;
    var pos = usize_(0);
    if (*S_at((body)[0]) == u8_c('[')) {
        const u8* close = (const u8*)__builtin_memchr(S_ptr(body) + 1, u8_c(']'), body.len - 1);
        if (close == null) return result;
        pos = as$(usize)(close - S_ptr(body)) + 1;
    }
    if (pos >= body.len) return result;
    if (*S_at((body)[pos]) == u8_c('?') || *S_at((body)[pos]) == u8_c('!')) return result;
    let type = *S_at((body)[pos++]);
    if (type != u8_c('0')
        && type != u8_c('b') && type != u8_c('B')
        && type != u8_c('u') && type != u8_c('U')
        && type != u8_c('i') && type != u8_c('I')
        && type != u8_c('p') && type != u8_c('P')
        && type != u8_c('c') && type != u8_c('C')
        && type != u8_c('z') && type != u8_c('s')) return result;

    let rem = body.len - pos;
    const u8* mode_open = (const u8*)__builtin_memchr(S_ptr(body) + pos, u8_c('('), rem);
    const u8* layout_open = (const u8*)__builtin_memchr(S_ptr(body) + pos, u8_c(':'), rem);
    var size_end = body.len;
    if (mode_open != null) size_end = as$(usize)(mode_open - S_ptr(body));
    if (layout_open != null) {
        let layout_pos = as$(usize)(layout_open - S_ptr(body));
        if (layout_pos < size_end) size_end = layout_pos;
    }
    let size_len = size_end - pos;
    var_(size, fmt_Size) = fmt_Size_32;
    if (size_len == 0) size = fmt_Size_32;
    else if (size_len == 1) {
        let ch = *S_at((body)[pos]);
        if (ch == u8_c('8')) size = fmt_Size_8;
        else if (ch == u8_c('h')) size = fmt_Size_16;
        else if (ch == u8_c('l')) size = fmt_Size_long;
        else if (ch == u8_c('z')) size = fmt_Size_ptr;
        else return result;
    } else if (size_len == 2) {
        let a = *S_at((body)[pos]); let b = *S_at((body)[pos + 1]);
        if (a == u8_c('h') && b == u8_c('h')) size = fmt_Size_8;
        else if (a == u8_c('1') && b == u8_c('6')) size = fmt_Size_16;
        else if (a == u8_c('3') && b == u8_c('2')) size = fmt_Size_32;
        else if (a == u8_c('6') && b == u8_c('4')) size = fmt_Size_64;
        else if (a == u8_c('l') && b == u8_c('l')) size = fmt_Size_64;
        else return result;
    } else if (size_len == 4) {
        let p0 = *S_at((body)[pos]); let p1 = *S_at((body)[pos + 1]);
        let p2 = *S_at((body)[pos + 2]); let p3 = *S_at((body)[pos + 3]);
        if (p0 == u8_c('l') && p1 == u8_c('o') && p2 == u8_c('n') && p3 == u8_c('g')) size = fmt_Size_long;
        else if (p0 == u8_c('s') && p1 == u8_c('i') && p2 == u8_c('z') && p3 == u8_c('e')) size = fmt_Size_ptr;
        else return result;
    } else return result;
    pos = size_end;

    let is_uint = type == u8_c('u') || type == u8_c('U');
    let is_iint = type == u8_c('i') || type == u8_c('I');
    let is_int = is_uint || is_iint;
    if (!is_int && size_len != 0) return result;

    var mode = u8_c(' '); var alt = false;
    if (pos < body.len && *S_at((body)[pos]) == u8_c('(')) {
        pos += 1;
        if (pos < body.len && *S_at((body)[pos]) == u8_c('#')) { alt = true; pos += 1; }
        if (pos >= body.len || !fmt_isMode(*S_at((body)[pos]))) return result;
        mode = *S_at((body)[pos++]);
        if (pos >= body.len || *S_at((body)[pos++]) != u8_c(')')) return result;
    }

    var fill = u8_c(' '); var_(align, fmt_Align) = fmt_Align_left;
    var sign = u8_c('-'); var has_sign = false;
    var width = usize_(0); var precision = usize_(6); var has_precision = false;
    if (pos < body.len) {
        if (*S_at((body)[pos++]) != u8_c(':')) return result;
        if (pos >= body.len) return result;
        if (pos + 1 < body.len && fmt_isAlign(*S_at((body)[pos + 1]))) fill = *S_at((body)[pos++]);
        if (pos < body.len && fmt_isAlign(*S_at((body)[pos]))) {
            let ch = *S_at((body)[pos++]);
            align = ch == u8_c('<') ? fmt_Align_left : ch == u8_c('^') ? fmt_Align_center : fmt_Align_right;
        }
        if (pos < body.len && (*S_at((body)[pos]) == u8_c('+') || *S_at((body)[pos]) == u8_c('-') || *S_at((body)[pos]) == u8_c(' '))) {
            sign = *S_at((body)[pos++]); has_sign = true;
        }
#define fmt__takeFoldedDigit(_$dst) do { \
    if (pos < body.len && ascii_isDigit(*S_at((body)[pos]))) { \
        (_$dst) = (_$dst) * 10 + as$(usize)(*S_at((body)[pos++]) - u8_c('0')); \
    } \
} while (false)
        fmt__takeFoldedDigit(width); fmt__takeFoldedDigit(width); fmt__takeFoldedDigit(width);
        fmt__takeFoldedDigit(width); fmt__takeFoldedDigit(width);
        if (pos < body.len && ascii_isDigit(*S_at((body)[pos]))) return result;
        if (pos < body.len && *S_at((body)[pos]) == u8_c('.')) {
            pos += 1; precision = 0; has_precision = true;
            if (pos >= body.len || !ascii_isDigit(*S_at((body)[pos]))) return result;
            fmt__takeFoldedDigit(precision); fmt__takeFoldedDigit(precision); fmt__takeFoldedDigit(precision);
            fmt__takeFoldedDigit(precision); fmt__takeFoldedDigit(precision);
            if (pos < body.len && ascii_isDigit(*S_at((body)[pos]))) return result;
        }
#undef fmt__takeFoldedDigit
    }
    if (pos != body.len) return result;

    let is_void = type == u8_c('0');
    let is_bool = type == u8_c('b') || type == u8_c('B');
    let is_ptr = type == u8_c('p') || type == u8_c('P');
    let is_text = type == u8_c('c') || type == u8_c('C') || type == u8_c('z') || type == u8_c('s');
    if (is_void && (size_len != 0 || mode != u8_c(' ') || alt || has_sign || has_precision
                    || width != 0 || fill != u8_c(' ') || align != fmt_Align_left)) return result;
    if (is_bool && (mode != u8_c(' ') || alt || has_sign || has_precision)) return result;
    if (is_int) {
        if ((is_uint && has_sign) || has_precision || mode == u8_c('e')) return result;
        let upper_type = type == u8_c('U') || type == u8_c('I');
        if ((mode == u8_c(' ') || mode == u8_c('d')) && (upper_type || alt)) return result;
        if (mode == u8_c('o') && upper_type) return result;
        if (mode == u8_c('b') && upper_type && !alt) return result;
    }
    if (is_ptr && (has_sign || has_precision || (mode != u8_c(' ') && mode != u8_c('x') && mode != u8_c('X')))) return result;
    if (is_text && (mode != u8_c(' ') || alt || has_sign || has_precision)) return result;

    result.type = type; result.size = size; result.mode = mode; result.alt = alt;
    result.layout = (fmt_LayoutSpec){ .fill = fill, .align = align, .width = width };
    result.sign = sign; result.has_sign = has_sign;
    result.precision = precision; result.has_precision = has_precision;
    return result;
}

fn_((fmt__writeFoldedText(S$u8 out, S_const$u8 text, fmt_LayoutSpec layout))(E$usize) $scope) {
    let required = layout.width > text.len ? layout.width : text.len;
    if (out.len < required) return_err(E_cause$TooSmallBuffer());
    return_ok(fmt__printPaddedFolded(out, text, layout.fill, layout.align, layout.width));
} $unscoped(fn);

fn_((fmt__writeRuntimeUIntExact(S$u8 out, u64 value, fmt_RuntimeBody body))(E$usize) $scope) {
    var_(tmp, A$$(128, u8));
    var_(mode, u8) = body.mode == u8_c(' ') ? u8_c('d') : body.mode;
    let upper = body.type == u8_c('U') || mode == u8_c('X');
    let written = fmt_printU64(A_ref$((S$u8)tmp), value, mode, upper, body.alt);
    return fmt__writeFoldedText(out, A_prefix$((S_const$u8)(tmp)(written)), body.layout);
} $unscoped(fn);

fn_((fmt__writeRuntimeIIntExact(S$u8 out, i64 value, fmt_RuntimeBody body))(E$usize) $scope) {
    var_(tmp, A$$(128, u8));
    var_(mode, u8) = body.mode == u8_c(' ') ? u8_c('d') : body.mode;
    let upper = body.type == u8_c('I') || mode == u8_c('X');
    let written = fmt_printI64(A_ref$((S$u8)tmp), value, mode, upper, body.alt, body.sign);
    return fmt__writeFoldedText(out, A_prefix$((S_const$u8)(tmp)(written)), body.layout);
} $unscoped(fn);

fn_((fmt__writeRuntimePtrExact(S$u8 out, P_const$raw value, fmt_RuntimeBody body))(E$usize) $scope) {
    var_(tmp, A$$(128, u8));
    let upper = body.type == u8_c('P') || body.mode == u8_c('X');
    let written = fmt_printPtr(A_ref$((S$u8)tmp), value, u8_c('x'), upper, body.alt);
    return fmt__writeFoldedText(out, A_prefix$((S_const$u8)(tmp)(written)), body.layout);
} $unscoped(fn);

fn_((fmt__writeRuntimeUTF8Exact(S$u8 out, u32 value, fmt_RuntimeBody body))(E$usize) $scope) {
    var_(tmp, A$$(4, u8));
    let written = try_(fmt_printUTF8(A_ref$((S$u8)tmp), value));
    return fmt__writeFoldedText(out, A_prefix$((S_const$u8)(tmp)(written)), body.layout);
} $unscoped(fn);

fn_((fmt__writeFoldedBool(S$u8 out, bool value, fmt_RuntimeBody body))(fmt_FoldFlatWrite)) {
    let text = value
        ? (body.type == u8_c('B') ? u8_l("TRUE") : u8_l("true"))
        : (body.type == u8_c('B') ? u8_l("FALSE") : u8_l("false"));
    let required = body.layout.width > text.len ? body.layout.width : text.len;
    if (out.len < required) return (fmt_FoldFlatWrite){ .written = 0, .too_small = true };
    return (fmt_FoldFlatWrite){
        .written = fmt__printPaddedFolded(
            out, text, body.layout.fill, body.layout.align, body.layout.width
        ),
        .too_small = false,
    };
}

fn_((fmt__writeFoldedUInt(S$u8 out, u64 value, fmt_RuntimeBody body))(fmt_FoldFlatWrite)) {
    var_(tmp, A$$(128, u8));
    var_(mode, u8) = body.mode == u8_c(' ') ? u8_c('d') : body.mode;
    let upper = body.type == u8_c('U') || mode == u8_c('X');
    let text_len = fmt__printU64Folded(A_ref$((S$u8)tmp), value, mode, upper, body.alt);
    let required = body.layout.width > text_len ? body.layout.width : text_len;
    if (out.len < required) return (fmt_FoldFlatWrite){ .written = 0, .too_small = true };
    return (fmt_FoldFlatWrite){
        .written = fmt__printPaddedFolded(
            out,
            A_prefix$((S_const$u8)(tmp)(text_len)),
            body.layout.fill,
            body.layout.align,
            body.layout.width
        ),
        .too_small = false,
    };
}

fn_((fmt__writeFoldedIInt(S$u8 out, i64 value, fmt_RuntimeBody body))(fmt_FoldFlatWrite)) {
    var_(tmp, A$$(128, u8));
    var_(mode, u8) = body.mode == u8_c(' ') ? u8_c('d') : body.mode;
    let upper = body.type == u8_c('I') || mode == u8_c('X');
    let text_len = fmt__printI64Folded(
        A_ref$((S$u8)tmp), value, mode, upper, body.alt, body.sign
    );
    let required = body.layout.width > text_len ? body.layout.width : text_len;
    if (out.len < required) return (fmt_FoldFlatWrite){ .written = 0, .too_small = true };
    return (fmt_FoldFlatWrite){
        .written = fmt__printPaddedFolded(
            out,
            A_prefix$((S_const$u8)(tmp)(text_len)),
            body.layout.fill,
            body.layout.align,
            body.layout.width
        ),
        .too_small = false,
    };
}


fn_((fmt__writeFoldedField(
    S$u8 out,
    fmt_RuntimeBody body,
    u_P_const$raw field,
    bool fold_values
))(E$usize) $scope) {
    var_(flat, fmt_FoldFlatWrite) = (fmt_FoldFlatWrite){ .written = 0, .too_small = false };
    switch (body.type) {
    case u8_c('0'):
        claim_assert(TypeInfo_eql(field.type, typeInfo$(Void)));
        return_ok(0);
    case u8_c('b'):
    case u8_c('B'):
        claim_assert(TypeInfo_eql(field.type, typeInfo$(bool)));
        flat = fmt__writeFoldedBool(out, *ptrCast$((const bool*)(field.raw)), body);
        break;
    case u8_c('u'):
    case u8_c('U'): {
        var value = u64_(0);
        switch (body.size) {
        case fmt_Size_8: claim_assert(TypeInfo_eql(field.type, typeInfo$(u8))); value = *ptrCast$((const u8*)(field.raw)); break;
        case fmt_Size_16: claim_assert(TypeInfo_eql(field.type, typeInfo$(u16))); value = *ptrCast$((const u16*)(field.raw)); break;
        case fmt_Size_32: claim_assert(TypeInfo_eql(field.type, typeInfo$(u32))); value = *ptrCast$((const u32*)(field.raw)); break;
        case fmt_Size_long: claim_assert(TypeInfo_eql(field.type, typeInfo$(ulong))); value = *ptrCast$((const ulong*)(field.raw)); break;
        case fmt_Size_64: claim_assert(TypeInfo_eql(field.type, typeInfo$(u64))); value = *ptrCast$((const u64*)(field.raw)); break;
        case fmt_Size_ptr: claim_assert(TypeInfo_eql(field.type, typeInfo$(usize))); value = *ptrCast$((const usize*)(field.raw)); break;
        default: claim_unreachable;
        }
        if (!fold_values) return fmt__writeRuntimeUIntExact(out, value, body);
        flat = fmt__writeFoldedUInt(out, value, body);
        break;
    }
    case u8_c('i'):
    case u8_c('I'): {
        var value = i64_(0);
        switch (body.size) {
        case fmt_Size_8: claim_assert(TypeInfo_eql(field.type, typeInfo$(i8))); value = *ptrCast$((const i8*)(field.raw)); break;
        case fmt_Size_16: claim_assert(TypeInfo_eql(field.type, typeInfo$(i16))); value = *ptrCast$((const i16*)(field.raw)); break;
        case fmt_Size_32: claim_assert(TypeInfo_eql(field.type, typeInfo$(i32))); value = *ptrCast$((const i32*)(field.raw)); break;
        case fmt_Size_long: claim_assert(TypeInfo_eql(field.type, typeInfo$(ilong))); value = *ptrCast$((const ilong*)(field.raw)); break;
        case fmt_Size_64: claim_assert(TypeInfo_eql(field.type, typeInfo$(i64))); value = *ptrCast$((const i64*)(field.raw)); break;
        case fmt_Size_ptr: claim_assert(TypeInfo_eql(field.type, typeInfo$(isize))); value = *ptrCast$((const isize*)(field.raw)); break;
        default: claim_unreachable;
        }
        if (!fold_values) return fmt__writeRuntimeIIntExact(out, value, body);
        flat = fmt__writeFoldedIInt(out, value, body);
        break;
    }
    case u8_c('p'):
    case u8_c('P'): {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(P_const$raw)));
        let value = *ptrCast$((const P_const$raw*)(field.raw));
        return fmt__writeRuntimePtrExact(out, value, body);
    }
    case u8_c('c'): {
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u8)));
        let value = *ptrCast$((const u8*)(field.raw));
        return fmt__writeFoldedText(out, (S_const$u8){ .ptr = &value, .len = 1 }, body.layout);
    }
    case u8_c('C'):
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
        return fmt__writeRuntimeUTF8Exact(out, *ptrCast$((const u32*)(field.raw)), body);
    case u8_c('z'):
        claim_assert(TypeInfo_eql(field.type, typeInfo$(P_const$u8)));
        return fmt__writeFoldedText(out, mem_spanZ0$u8(*ptrCast$((const P_const$u8*)(field.raw))), body.layout);
    case u8_c('s'):
        claim_assert(TypeInfo_eql(field.type, typeInfo$(S_const$u8)));
        return fmt__writeFoldedText(out, *ptrCast$((const S_const$u8*)(field.raw)), body.layout);
    default:
        return fmt__writeValueRuntime(out, body, field);
    }
    if (flat.too_small) return_err(E_cause$TooSmallBuffer());
    return_ok(flat.written);
} $unscoped(fn);


#define fmt__defineTypedValue(_$Type) \
    $attr($maybe_unused) \
    fn_((pp_cat(fmt__writeTyped$, _$Type)(S$u8 out, fmt_ParsedBody parsed, _$Type value, bool value_is_comptime))(E$S$u8) $scope) { \
        let written = try_(fmt__writeValue( \
            out, \
            parsed.wrapped, \
            parsed.size, \
            parsed.spec, \
            u_anyP$((u_P_const$raw)(&value)), \
            value_is_comptime \
        )); \
        return_ok(S_prefix((out)written)); \
    } $unscoped(fn)
fmt__defineTypedValue(bool);
fmt__defineTypedValue(u8);
fmt__defineTypedValue(u16);
fmt__defineTypedValue(u32);
fmt__defineTypedValue(ulong);
fmt__defineTypedValue(u64);
fmt__defineTypedValue(i8);
fmt__defineTypedValue(i16);
fmt__defineTypedValue(i32);
fmt__defineTypedValue(ilong);
fmt__defineTypedValue(i64);
fmt__defineTypedValue(f32);
fmt__defineTypedValue(f64);
fmt__defineTypedValue(P_const$raw);
fmt__defineTypedValue(P_const$u8);
fmt__defineTypedValue(S_const$u8);
fmt__defineTypedValue(EAny);
fmt__defineTypedValue(O$u64);
fmt__defineTypedValue(E$S_const$u8);
#undef fmt__defineTypedValue

$attr($maybe_unused)
fn_((fmt__writeValue(S$u8 out, u8 wrapped, fmt_Size size, fmt_Spec spec, u_P_const$raw field, bool fold_values))(E$usize) $scope) {
    return fmt__writeValueCore(out, fmt__runtimeBodyFromSpec(wrapped, size, spec), field, fold_values);
} $unscoped(fn);

fn_((fmt__runtimeBodyFromSpec(u8 wrapped, fmt_Size size, fmt_Spec spec))(fmt_RuntimeBody) $scope) {
    var body = (fmt_RuntimeBody){
        .wrapped = wrapped,
        .type = u8_c('0'),
        .size = size,
        .mode = u8_c(' '),
        .alt = false,
        .layout = fmt_LayoutSpec_default(),
        .sign = u8_c('-'),
        .has_sign = false,
        .precision = 6,
        .has_precision = false,
    };
    match_(spec) {
    patt_((fmt_Spec_void)($ignore)) {
        body.type = u8_c('0');
    } $end(patt);
    patt_((fmt_Spec_bool)(typed)) {
        body.type = typed.case_ == fmt_Case_upper ? u8_c('B') : u8_c('b');
        body.layout = typed.layout;
    } $end(patt);
    patt_((fmt_Spec_uint)(typed)) {
        body.type = u8_c('u');
        body.layout = typed.layout;
        match_(typed.style) {
        patt_((fmt_IntStyle_decimal)($ignore)) body.mode = u8_c('d') $end(patt);
        patt_((fmt_IntStyle_binary)(prefix)) {
            body.mode = u8_c('b');
            body.alt = prefix != fmt_BinPrefix_none;
            if (prefix == fmt_BinPrefix_upper) body.type = u8_c('U');
        } $end(patt);
        patt_((fmt_IntStyle_octal)(alt_form)) {
            body.mode = u8_c('o');
            body.alt = alt_form;
        } $end(patt);
        patt_((fmt_IntStyle_hex)(style)) {
            body.mode = style.case_ == fmt_Case_upper ? u8_c('X') : u8_c('x');
            body.alt = style.alt_form;
        } $end(patt);
        } $end(match);
    } $end(patt);
    patt_((fmt_Spec_iint)(typed)) {
        body.type = u8_c('i');
        body.layout = typed.layout;
        body.sign = typed.sign == fmt_Sign_always ? u8_c('+')
                  : typed.sign == fmt_Sign_space  ? u8_c(' ')
                                                  : u8_c('-');
        body.has_sign = typed.sign != fmt_Sign_auto;
        match_(typed.style) {
        patt_((fmt_IntStyle_decimal)($ignore)) body.mode = u8_c('d') $end(patt);
        patt_((fmt_IntStyle_binary)(prefix)) {
            body.mode = u8_c('b');
            body.alt = prefix != fmt_BinPrefix_none;
            if (prefix == fmt_BinPrefix_upper) body.type = u8_c('I');
        } $end(patt);
        patt_((fmt_IntStyle_octal)(alt_form)) {
            body.mode = u8_c('o');
            body.alt = alt_form;
        } $end(patt);
        patt_((fmt_IntStyle_hex)(style)) {
            body.mode = style.case_ == fmt_Case_upper ? u8_c('X') : u8_c('x');
            body.alt = style.alt_form;
        } $end(patt);
        } $end(match);
    } $end(patt);
    patt_((fmt_Spec_flt)(typed)) {
        body.type = u8_c('f');
        body.layout = typed.layout;
        body.sign = typed.sign == fmt_Sign_always ? u8_c('+')
                  : typed.sign == fmt_Sign_space  ? u8_c(' ')
                                                  : u8_c('-');
        body.has_sign = typed.sign != fmt_Sign_auto;
        if_some((typed.precision)(precision)) {
            body.precision = precision;
            body.has_precision = true;
        }
        match_(typed.style) {
        patt_((fmt_FltStyle_decimal)(style)) {
            body.mode = u8_c('d');
            body.alt = style.alt_form;
        } $end(patt);
        patt_((fmt_FltStyle_scientific)(style)) {
            body.mode = u8_c('e');
            body.type = style.case_ == fmt_Case_upper ? u8_c('F') : u8_c('f');
            body.alt = style.alt_form;
        } $end(patt);
        } $end(match);
    } $end(patt);
    patt_((fmt_Spec_ptr)(typed)) {
        body.type = typed.case_ == fmt_Case_upper ? u8_c('P') : u8_c('p');
        body.mode = u8_c('x');
        body.alt = typed.alt_form;
        body.layout = typed.layout;
    } $end(patt);
    patt_((fmt_Spec_ascii)(typed)) {
        body.type = u8_c('c');
        body.layout = typed.layout;
    } $end(patt);
    patt_((fmt_Spec_utf8)(typed)) {
        body.type = u8_c('C');
        body.layout = typed.layout;
    } $end(patt);
    patt_((fmt_Spec_str_z0)(typed)) {
        body.type = u8_c('z');
        body.layout = typed.layout;
    } $end(patt);
    patt_((fmt_Spec_str)(typed)) {
        body.type = u8_c('s');
        body.layout = typed.layout;
    } $end(patt);
    patt_((fmt_Spec_err)(typed)) {
        body.type = u8_c('e');
        body.layout = typed.layout;
    } $end(patt);
    } $end(match);
    return body;
} $unscoped(fn);

fn_((fmt__runtimeValueTypeInfo(fmt_RuntimeBody body))(TypeInfo) $scope) {
    if (body.type == u8_c('0')) return typeInfo$(Void);
    if (body.type == u8_c('b') || body.type == u8_c('B')) return typeInfo$(bool);
    if (body.type == u8_c('u') || body.type == u8_c('U')) {
        if (body.size == fmt_Size_8) return typeInfo$(u8);
        if (body.size == fmt_Size_16) return typeInfo$(u16);
        if (body.size == fmt_Size_32) return typeInfo$(u32);
        if (body.size == fmt_Size_long) return typeInfo$(ulong);
        if (body.size == fmt_Size_64) return typeInfo$(u64);
        if (body.size == fmt_Size_ptr) return typeInfo$(usize);
    }
    if (body.type == u8_c('i') || body.type == u8_c('I')) {
        if (body.size == fmt_Size_8) return typeInfo$(i8);
        if (body.size == fmt_Size_16) return typeInfo$(i16);
        if (body.size == fmt_Size_32) return typeInfo$(i32);
        if (body.size == fmt_Size_long) return typeInfo$(ilong);
        if (body.size == fmt_Size_64) return typeInfo$(i64);
        if (body.size == fmt_Size_ptr) return typeInfo$(isize);
    }
    if (body.type == u8_c('f') || body.type == u8_c('F')) {
        if (body.size == fmt_Size_32) return typeInfo$(f32);
        if (body.size == fmt_Size_64) return typeInfo$(f64);
    }
    if (body.type == u8_c('p') || body.type == u8_c('P')) return typeInfo$(P_const$raw);
    if (body.type == u8_c('c')) return typeInfo$(u8);
    if (body.type == u8_c('C')) return typeInfo$(u32);
    if (body.type == u8_c('z')) return typeInfo$(P_const$u8);
    if (body.type == u8_c('s')) return typeInfo$(S_const$u8);
    if (body.type == u8_c('e')) return typeInfo$(EAny);
    claim_unreachable;
} $unscoped(fn);

fn_((fmt__writeValueRuntime(S$u8 out, fmt_RuntimeBody body, u_P_const$raw field))(E$usize)) {
    return fmt__writeValueCore(out, body, field, false);
}

fn_((fmt__writeValueCore(S$u8 out, fmt_RuntimeBody body, u_P_const$raw field, bool fold_values))(E$usize) $scope) {
    var value_field = field;
    if (body.wrapped != u8_c('\0')) {
        let value_type = fmt__runtimeValueTypeInfo(body);
        let value_align = TypeInfo_align(value_type);
        let value_size = TypeInfo_size(value_type);
        let raw = ptrCast$((P_const$u8)(field.raw));
        if (body.wrapped == u8_c('?')) {
            let payload_offset = mem_alignFwd(sizeOf$(bool), value_align);
            let expected_size = mem_alignFwd(payload_offset + value_size, value_align);
            claim_assert(TypeInfo_size(field.type) == expected_size);
            claim_assert(TypeInfo_align(field.type) == value_align);
            if (!*ptrCast$((const bool*)(raw))) return_ok(fmt__copy(out, u8_l("none")));
            if (body.type == u8_c('0')) return_ok(fmt__copy(out, u8_l("some")));
            value_field = P_meta((
                value_type
            )(ptrCast$((P_const$raw)(raw + payload_offset))));
        } else if (body.wrapped == u8_c('!')) {
            let payload_align = pri_max(value_align, alignOf$(EAny));
            let payload_size = pri_max(value_size, sizeOf$(EAny));
            let payload_offset = mem_alignFwd(sizeOf$(bool), payload_align);
            let expected_size = mem_alignFwd(payload_offset + payload_size, payload_align);
            claim_assert(TypeInfo_size(field.type) == expected_size);
            claim_assert(TypeInfo_align(field.type) == payload_align);
            if (!*ptrCast$((const bool*)(raw))) {
                let err = *ptrAlignCast$((const EAny*)(raw + payload_offset));
                return_ok(fmt_printErr(out, err));
            }
            if (body.type == u8_c('0')) return_ok(fmt__copy(out, u8_l("ok")));
            value_field = P_meta((
                value_type
            )(ptrCast$((P_const$raw)(raw + payload_offset))));
        } else {
            return_err(E_cause$fmt_InvalidTypeSpec());
        }
    }

    if (body.type == u8_c('0')) {
        claim_assert(TypeInfo_eql(value_field.type, typeInfo$(Void)));
        return_ok(0);
    }
    if (body.type == u8_c('b') || body.type == u8_c('B')) {
        claim_assert(TypeInfo_eql(value_field.type, typeInfo$(bool)));
        let value = *u_castP$((const bool*)(value_field));
        let upper = body.type == u8_c('B');
        return_ok(fold_values ? fmt__printBoolFolded(out, value, upper) : fmt_printBool(out, value, upper));
    }
    if (body.type == u8_c('u') || body.type == u8_c('U')) {
        var value = u64_(0);
        if (body.size == fmt_Size_8) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(u8)));
            value = *u_castP$((const u8*)(value_field));
        } else if (body.size == fmt_Size_16) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(u16)));
            value = *u_castP$((const u16*)(value_field));
        } else if (body.size == fmt_Size_32) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(u32)));
            value = *u_castP$((const u32*)(value_field));
        } else if (body.size == fmt_Size_long) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(ulong)));
            value = *u_castP$((const ulong*)(value_field));
        } else if (body.size == fmt_Size_64) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(u64)));
            value = *u_castP$((const u64*)(value_field));
        } else {
            claim_assert(body.size == fmt_Size_ptr);
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(usize)));
            value = *u_castP$((const usize*)(value_field));
        }
        var_(mode, u8) = body.mode == u8_c(' ') ? u8_c('d') : body.mode;
        let upper = body.type == u8_c('U') || mode == u8_c('X');
        return_ok(fold_values
            ? fmt__printU64Folded(out, value, mode, upper, body.alt)
            : fmt_printU64(out, value, mode, upper, body.alt));
    }
    if (body.type == u8_c('i') || body.type == u8_c('I')) {
        var value = i64_(0);
        if (body.size == fmt_Size_8) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(i8)));
            value = *u_castP$((const i8*)(value_field));
        } else if (body.size == fmt_Size_16) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(i16)));
            value = *u_castP$((const i16*)(value_field));
        } else if (body.size == fmt_Size_32) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(i32)));
            value = *u_castP$((const i32*)(value_field));
        } else if (body.size == fmt_Size_long) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(ilong)));
            value = *u_castP$((const ilong*)(value_field));
        } else if (body.size == fmt_Size_64) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(i64)));
            value = *u_castP$((const i64*)(value_field));
        } else {
            claim_assert(body.size == fmt_Size_ptr);
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(isize)));
            value = *u_castP$((const isize*)(value_field));
        }
        var_(mode, u8) = body.mode == u8_c(' ') ? u8_c('d') : body.mode;
        let upper = body.type == u8_c('I') || mode == u8_c('X');
        return_ok(fold_values ? fmt__printI64Folded(out, value, mode, upper, body.alt, body.sign) : fmt_printI64(out, value, mode, upper, body.alt, body.sign));
    }
    if (body.type == u8_c('f') || body.type == u8_c('F')) {
        var value = f64_(0.0);
        if (body.size == fmt_Size_32) {
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(f32)));
            value = *u_castP$((const f32*)(value_field));
        } else {
            claim_assert(body.size == fmt_Size_64);
            claim_assert(TypeInfo_eql(value_field.type, typeInfo$(f64)));
            value = *u_castP$((const f64*)(value_field));
        }
        return fmt_printF64(
            out,
            value,
            body.mode == u8_c('e') ? u8_c('e') : u8_c('d'),
            body.type == u8_c('F'),
            body.alt,
            body.sign,
            body.precision,
            body.has_precision
        );
    }
    if (body.type == u8_c('p') || body.type == u8_c('P')) {
        claim_assert(TypeInfo_eql(value_field.type, typeInfo$(P_const$raw)));
        let value = *u_castP$((const P_const$raw*)(value_field));
        let upper = body.type == u8_c('P') || body.mode == u8_c('X');
        return_ok(fold_values ? fmt__printU64Folded(out, as$(u64)(ptrToInt(value)), u8_c('x'), upper, body.alt) : fmt_printPtr(out, value, u8_c('x'), upper, body.alt));
    }
    if (body.type == u8_c('c')) {
        claim_assert(TypeInfo_eql(value_field.type, typeInfo$(u8)));
        let value = *u_castP$((const u8*)(value_field));
        if (fold_values) {
            if (out.len < 1) return_err(E_cause$TooSmallBuffer());
            *S_at((out)[0]) = value;
            return_ok(1);
        }
        return fmt_printASCII(out, value);
    }
    if (body.type == u8_c('C')) {
        claim_assert(TypeInfo_eql(value_field.type, typeInfo$(u32)));
        return fmt_printUTF8(out, *u_castP$((const u32*)(value_field)));
    }
    if (body.type == u8_c('z')) {
        claim_assert(TypeInfo_eql(value_field.type, typeInfo$(P_const$u8)));
        let value = *u_castP$((const P_const$u8*)(value_field));
        return_ok(fold_values ? fmt__copy(out, mem_spanZ0$u8(value)) : fmt_printStrZ0(out, value));
    }
    if (body.type == u8_c('s')) {
        claim_assert(TypeInfo_eql(value_field.type, typeInfo$(S_const$u8)));
        let value = *u_castP$((const S_const$u8*)(value_field));
        return_ok(fold_values ? fmt__copy(out, value) : fmt_printStr(out, value));
    }
    if (body.type == u8_c('e')) {
        claim_assert(TypeInfo_eql(value_field.type, typeInfo$(EAny)));
        return_ok(fmt_printErr(out, *u_castP$((const EAny*)(value_field))));
    }
    return_err(E_cause$fmt_InvalidTypeSpec());
} $unscoped(fn);

typedef struct fmt_TestWriter {
    var_(buf, S$u8);
    var_(written, usize);
} fmt_TestWriter;
$static volatile bool fmt_test_runtime_format = false;

$attr($must_check $inline_always)
$static fn_((fmt_TestWriter_write(P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    let self = ptrAlignCast$((fmt_TestWriter*)(ctx));
    if (self->buf.len - self->written < bytes.len) {
        return_err(E_cause$TooSmallBuffer());
    }
    let_ignore = mem_copyBytes(
        S_slice((self->buf)$r(self->written, self->written + bytes.len)),
        bytes
    );
    self->written += bytes.len;
    return_ok(bytes.len);
} $unscoped(fn);

$attr($inline_never)
$static fn_((fmt_test_expectInvalidType(S_const$u8 body))(void) $scope) {
    let result = fmt__parseBodyRuntime(body);
    if_err((result)(err)) {
        let expected = E_cause$fmt_InvalidTypeSpec();
        claim_assert(E_eql(&err, expected.as_any));
    } else_ok((unexpected)) {
        let_ignore = unexpected;
        claim_unreachable;
    }
} $unscoped(fn);

$attr($inline_never)
$static fn_((fmt_test_runtimeU8(void))(u8)) { return u8_(255); }
$attr($inline_never)
$static fn_((fmt_test_runtimeI16(void))(i16)) { return i16_(7); }
$attr($inline_never)
$static fn_((fmt_test_runtimeASCII(void))(u8)) { return u8_c('A'); }
$attr($inline_never)
$static fn_((fmt_test_runtimeCodepoint(void))(u32)) { return u32_(0x03bb); }

$attr($must_check $inline_never)
$static fn_((fmt_test_constantFormatRuntimeValues(void))(E$void) $scope) {
    var_(scalar_mem, A$$(128, u8)) = A_zero();
    var_(text_mem, A$$(128, u8)) = A_zero();
    var_(ptr_mem, A$$(128, u8)) = A_zero();
    var_(void_mem, A$$(16, u8)) = A_zero();

    let void_text = try_(va_((fmt_print)(
        A_ref$((S$u8)void_mem),
        u8_l("left%{0}right")
    )(Void_())));
    claim_assert(mem_eqlBytes(void_text.as_const, u8_l("leftright")));

    let scalar = try_(va_((fmt_print)(
        A_ref$((S$u8)scalar_mem),
        u8_l("u8=%{u8} i16=%{i16:_>+4} c=%{c}")
    )(
        fmt_test_runtimeU8(),
        fmt_test_runtimeI16(),
        fmt_test_runtimeASCII()
    )));
    claim_assert(mem_eqlBytes(
        scalar.as_const,
        u8_l("u8=255 i16=__+7 c=A")
    ));

    let runtime_s = fmt_test_runtime_format ? u8_l("other") : u8_l("slice");
    let runtime_z_text = fmt_test_runtime_format ? u8_l("other-z") : u8_l("z0");
    let runtime_z = S_ptr(runtime_z_text);
    let text = try_(va_((fmt_print)(
        A_ref$((S$u8)text_mem),
        u8_l("cp=%{C} s=%{s} z=%{z}")
    )(
        fmt_test_runtimeCodepoint(),
        runtime_s,
        runtime_z
    )));
    claim_assert(mem_eqlBytes(
        text.as_const,
        u8_l("cp=λ s=slice z=z0")
    ));

    let runtime_ptr = as$(P_const$raw)(&ptr_mem);
    let ptr_text = try_(va_((fmt_print)(
        A_ref$((S$u8)ptr_mem),
        u8_l("p=%{p(#x)}")
    )(runtime_ptr)));
    claim_assert(ptr_text.len > 4);
    claim_assert(mem_eqlBytes(
        S_prefix((ptr_text.as_const)4),
        u8_l("p=0x")
    ));
    return_ok({});
} $unscoped(fn);

$attr($must_check $inline_never)
$static fn_((fmt_test_constantBufferBoundaries(void))(E$void) $scope) {
    var_(small_mem, A$$(2, u8)) = A_zero();
    let small_result = va_((fmt_print)(
        A_ref$((S$u8)small_mem),
        u8_l("abc")
    )());
    if_err((small_result)(err)) {
        let expected = E_cause$TooSmallBuffer();
        claim_assert(E_eql(&err, expected.as_any));
    } else_ok((unexpected)) {
        let_ignore = unexpected;
        claim_unreachable;
    }

    var_(long_text, A$$(280, u8)) = A_zero();
    var_(long_out, A$$(300, u8)) = A_zero();
    for (usize i = 0; i < A_len(long_text); ++i) {
        *A_at((long_text)[i]) = u8_c('x');
    }
    var long_writer = (fmt_TestWriter){
        .buf = A_ref$((S$u8)long_out),
        .written = 0,
    };
    let writer = (io_Writer){
        .ctx = ptrCast$((P$raw)(&long_writer)),
        .writeFn = fmt_TestWriter_write,
    };
    try_(va_((io_Writer_print)(
        writer,
        u8_l("%{s}")
    )(A_ref$((S_const$u8)long_text))));
    claim_assert(long_writer.written == A_len(long_text));
    claim_assert(*A_at((long_out)[0]) == u8_c('x'));
    claim_assert(*A_at((long_out)[A_len(long_text) - 1]) == u8_c('x'));
    return_ok({});
} $unscoped(fn);

$attr($must_check $inline_never)
$static fn_((fmt_test_constantPaths(void))(E$void) $scope) {
    var_(direct_iint_mem, A$$(32, u8)) = A_zero();
    var_(writer_mem, A$$(128, u8)) = A_zero();
    var_(buffer_mem, A$$(64, u8)) = A_zero();

    let buffer_text = catch_((va_((fmt_print)(
        A_ref$((S$u8)buffer_mem),
        u8_l("constant literal exceeds sixteen bytes %%{ %{[0]u(#x)} %{[0]u}")
    )(u32_(42))))($ignore, claim_unreachable));

    let iint_spec = with_((fmt_IIntSpec_default())(
        (.layout.fill)(u8_c('_')),
        (.layout.align)(fmt_Align_right),
        (.layout.width)(8),
        (.sign)(fmt_Sign_always)
    ));
    let direct_iint = catch_((fmt_format$i16(
        A_ref$((S$u8)direct_iint_mem),
        i16_(-42),
        iint_spec
    ))($ignore, claim_unreachable));

    var test_writer = (fmt_TestWriter){
        .buf = A_ref$((S$u8)writer_mem),
        .written = 0,
    };
    let writer = (io_Writer){
        .ctx = ptrCast$((P$raw)(&test_writer)),
        .writeFn = fmt_TestWriter_write,
    };
    catch_((va_((io_Writer_println)(
        writer,
        u8_l("writer world %{i:+6}")
    )(i32_(123))))($ignore, claim_unreachable));

    claim_assert(mem_eqlBytes(
        buffer_text.as_const,
        u8_l("constant literal exceeds sixteen bytes %{ 0x2a 42")
    ));
    claim_assert(mem_eqlBytes(
        A_prefix$((S_const$u8)(writer_mem)(test_writer.written)),
        u8_l("writer world +123  " io_nl)
    ));

    puts(as$(const char*)(direct_iint.ptr));
    puts(as$(const char*)(buffer_text.ptr));
    puts(as$(const char*)(A_ref$((S_const$u8)writer_mem).ptr));
    va_((io_stream_println)(
        u8_l("stream %{U(#x)} %{B}")
    )(u32_(42), false));
    return_ok({});
} $unscoped(fn);

$attr($must_check $inline_never)
$static fn_((fmt_test_runtimePaths(void))(E$void) $scope) {
    var_(writer_mem, A$$(64, u8)) = A_zero();
    var_(buffer_mem, A$$(64, u8)) = A_zero();
    var_(long_mem, A$$(512, u8)) = A_zero();
    var_(long_text, A$$(300, u8)) = A_zero();
    let runtime_fmt = fmt_test_runtime_format
        ? u8_l("alternate %{u}")
        : u8_l("runtime %{u}");

    var runtime_writer = (fmt_TestWriter){
        .buf = A_ref$((S$u8)writer_mem),
        .written = 0,
    };
    let runtime_io = (io_Writer){
        .ctx = ptrCast$((P$raw)(&runtime_writer)),
        .writeFn = fmt_TestWriter_write,
    };
    catch_((va_((io_Writer_print)(
        runtime_io,
        runtime_fmt
    )(u32_(7))))($ignore, claim_unreachable));
    let buffer_text = catch_((va_((fmt_print)(
        A_ref$((S$u8)buffer_mem),
        runtime_fmt
    )(u32_(7))))($ignore, claim_unreachable));
    claim_assert(mem_eqlBytes(buffer_text.as_const, u8_l("runtime 7")));
    claim_assert(mem_eqlBytes(
        A_prefix$((S_const$u8)(writer_mem)(runtime_writer.written)),
        u8_l("runtime 7")
    ));

    fmt_test_expectInvalidType(u8_l("i:.2"));
    fmt_test_expectInvalidType(u8_l("U"));
    fmt_test_expectInvalidType(u8_l("u(#d)"));
    fmt_test_expectInvalidType(u8_l("I(o)"));
    fmt_test_expectInvalidType(u8_l("F64"));
    fmt_test_expectInvalidType(u8_l("0:>2"));

    for (usize i = 0; i < 280; ++i) {
        *A_at((long_text)[i]) = u8_c('x');
    }
    var long_writer = (fmt_TestWriter){
        .buf = A_ref$((S$u8)long_mem),
        .written = 0,
    };
    let long_io = (io_Writer){
        .ctx = ptrCast$((P$raw)(&long_writer)),
        .writeFn = fmt_TestWriter_write,
    };
    let long_fmt = fmt_test_runtime_format
        ? u8_l("other %{s:<300}")
        : u8_l("long %{s:>300}");
    catch_((va_((io_Writer_print)(
        long_io,
        long_fmt
    )(A_prefix$((S_const$u8)(long_text)(280)))))(
        $ignore,
        claim_unreachable
    ));
    claim_assert(long_writer.written == 305);
    claim_assert(*A_at((long_mem)[5]) == u8_c(' '));
    claim_assert(*A_at((long_mem)[24]) == u8_c(' '));
    claim_assert(*A_at((long_mem)[25]) == u8_c('x'));
    claim_assert(*A_at((long_mem)[304]) == u8_c('x'));

    long_writer.written = 0;
    var_(ok_long, E$S_const$u8) = ok(
        A_prefix$((S_const$u8)(long_text)(280))
    );
    let result_fmt = fmt_test_runtime_format
        ? u8_l("%{!s:<300}")
        : u8_l("%{!s:>300}");
    catch_((va_((io_Writer_print)(
        long_io,
        result_fmt
    )(ok_long)))($ignore, claim_unreachable));
    claim_assert(long_writer.written == 300);
    claim_assert(*A_at((long_mem)[19]) == u8_c(' '));
    claim_assert(*A_at((long_mem)[20]) == u8_c('x'));
    claim_assert(*A_at((long_mem)[299]) == u8_c('x'));

    long_writer.written = 0;
    var_(runtime_opt_u64, O$u64) = some(
        u64_(0x1122334455667788ull)
    );
    let optional_fmt = fmt_test_runtime_format
        ? u8_l("%{?u64}")
        : u8_l("%{?u64(#x)}");
    catch_((va_((io_Writer_print)(
        long_io,
        optional_fmt
    )(runtime_opt_u64)))($ignore, claim_unreachable));
    claim_assert(mem_eqlBytes(
        A_prefix$((S_const$u8)(long_mem)(long_writer.written)),
        u8_l("0x1122334455667788")
    ));

    puts(as$(const char*)(buffer_text.ptr));
    puts(as$(const char*)(A_ref$((S_const$u8)writer_mem).ptr));
    va_((io_stream_println)(runtime_fmt)(u32_(7)));
    return_ok({});
} $unscoped(fn);

fn_((main(void))(E$void) $scope) {
    catch_((fmt_test_constantPaths())($ignore, claim_unreachable));
    catch_((fmt_test_constantFormatRuntimeValues())($ignore, claim_unreachable));
    catch_((fmt_test_constantBufferBoundaries())($ignore, claim_unreachable));
    catch_((fmt_test_runtimePaths())($ignore, claim_unreachable));
    return_ok({});
} $unscoped(fn);

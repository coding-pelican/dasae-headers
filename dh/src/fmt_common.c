#include "dh/fmt/common.h"
#include "dh/mem/common.h"
#include "dh/ascii.h"


fn_((fmt_Align_parse(u8 ch))(O$fmt_Align) $scope) {
    return_(eval_(ReturnType $scope)(for_(($rf(0), $a(fmt_Align_values))(i, value) {
        if (ch == *value) { $break_(some(as$(fmt_Align)(i))); }
    })) eval_(else)($break_(none())) $unscoped_(eval));
} $unscoped_(fn);

fn_((fmt_TypePrefix_parse(u8 ch))(O$fmt_TypePrefix) $scope) {
    return_(eval_(ReturnType $scope)(for_(($rf(0), $a(fmt_TypePrefix_values))(i, value) {
        if (ch == *value) { $break_(some(as$(fmt_TypePrefix)(i))); }
    })) eval_(else)($break_(none())) $unscoped_(eval));
} $unscoped_(fn);

fn_((fmt_Type_parse(S_const$u8 str))(O$fmt_Type) $scope) {
    return_(eval_(ReturnType $scope)(for_(($rf(0), $a(fmt_Type_values))(i, value) {
        if (mem_eqlBytes(str, *value)) { $break_(some(as$(fmt_Type)(i))); }
    })) eval_(else)($break_(none())) $unscoped_(eval));
} $unscoped_(fn);

fn_((fmt_Size_parse(S_const$u8 str))(O$fmt_Size) $scope) {
    return_(eval_(ReturnType $scope)(for_(($rf(0), $a(fmt_Size_values))(i, value) {
        if (mem_eqlBytes(str, *value)) { $break_(some(as$(fmt_Size)(i))); }
    })) eval_(else)($break_(none())) $unscoped_(eval));
} $unscoped_(fn);


$inline_always
$static fn_((digitToInt(u8 c))(u8)) {
    claim_assert(ascii_isDigit(c));
    return c - u8_c('0');
}

$inline_always
$static fn_((skipWhitespace(S_const$u8 str))(S_const$u8)) {
    while (0 < str.len && ascii_isWhitespace(*at$S(str, 0))) {
        str.ptr++;
        str.len--;
    }
    return str;
}


$must_check
$static fn_((parseU8(S_const$u8 str))(E$u8));

typedef struct ConsumedU8 {
    u8 value;
    S_const$u8 remain;
} ConsumedU8;
T_use_E$(ConsumedU8);
$must_check
$static fn_((consumeU8(S_const$u8 str))(E$ConsumedU8));

typedef struct ParsedFormat {
    fmt_Spec spec;
    S_const$u8 remain;
} ParsedFormat;
T_use_E$(ParsedFormat);
$must_check
$static fn_((parseFormat(S_const$u8 fmt_str))(E$ParsedFormat));

typedef variant_((fmt_ArgValue $fits u8)(
    (fmt_ArgValue_void, Void),
    (fmt_ArgValue_bool, bool),
    (fmt_ArgValue_u8, u8),
    (fmt_ArgValue_u16, u16),
    (fmt_ArgValue_u32, u32),
    (fmt_ArgValue_u64, u64),
    (fmt_ArgValue_usize, usize),
    (fmt_ArgValue_i8, i8),
    (fmt_ArgValue_i16, i16),
    (fmt_ArgValue_i32, i32),
    (fmt_ArgValue_i64, i64),
    (fmt_ArgValue_isize, isize),
    (fmt_ArgValue_f32, f32),
    (fmt_ArgValue_f64, f64),
    (fmt_ArgValue_ptr, const void*),
    (fmt_ArgValue_error, Err),
    // (fmt_ArgValue_ascii_ch, u8),
    // (fmt_ArgValue_utf8_cp, u32),
    (fmt_ArgValue_sli_z_u8, const u8*),
    (fmt_ArgValue_sli_u8, S_const$u8)
)) fmt_ArgValue;
T_use$((fmt_ArgValue)(O, E));
typedef FieldType$(fmt_ArgValue, tag) fmt_ArgValue_Tag;
T_use$((fmt_ArgValue_Tag)(O, E));
$must_check
$static fn_((specToArgTag(fmt_Type type, fmt_Size size))(E$fmt_ArgValue_Tag));

typedef variant_((fmt_ArgType $fits u8)(
    (fmt_ArgType_value, fmt_ArgValue),
    (fmt_ArgType_optional, O$fmt_ArgValue),
    (fmt_ArgType_error_result, E$fmt_ArgValue)
)) fmt_ArgType;
T_use$((fmt_ArgType)(O, E));
$static fn_((collectArg(va_list* ap, O$fmt_TypePrefix wrapper, fmt_ArgValue_Tag tag))(fmt_ArgType));
$static fn_((collectArgValue(va_list* ap, fmt_ArgValue_Tag tag))(fmt_ArgValue));
$static fn_((collectArgOptional(va_list* ap, fmt_ArgValue_Tag tag))(O$fmt_ArgValue));
$static fn_((collectArgErrorResult(va_list* ap, fmt_ArgValue_Tag tag))(E$fmt_ArgValue));

$must_check
$static fn_((formatArg(io_Writer writer, fmt_ArgType arg, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatArgValue(io_Writer writer, fmt_ArgValue value, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatArgOptional(io_Writer writer, O$fmt_ArgValue optional, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatArgErrorResult(io_Writer writer, E$fmt_ArgValue error_result, fmt_Spec spec))(E$void));

$must_check
$static fn_((formatBool(io_Writer writer, bool val, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatUInt(io_Writer writer, u64 val, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatIInt(io_Writer writer, i64 val, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatFlt(io_Writer writer, f64 val, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatPtr(io_Writer writer, const void* ptr, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatErr(io_Writer writer, Err err, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatASCIICode(io_Writer writer, u8 c, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatUTF8Codepoint(io_Writer writer, u32 cp, fmt_Spec spec))(E$void));
$must_check
$static fn_((formatStr(io_Writer writer, S_const$u8 str, fmt_Spec spec))(E$void));

$must_check
$static fn_((writePadded(io_Writer writer, S_const$u8 content, fmt_Spec spec))(E$void));

#include <stdio.h>

/*========== Main Format Functions ==========================================*/

fn_((fmt_format(io_Writer writer, S_const$u8 fmt, ...))(E$void) $guard) {
    // printf("--- debug print: fmt_format ---\n");
    va_list va_args = {};
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(fmt_formatVaArgs(writer, fmt, va_args)));
} $unguarded_(fn);

// Store each format spec occurrence with its position and argument binding
typedef struct FormatOccurrence {
    fmt_Spec spec;
    u8 arg_index;        // Which argument this uses
    usize literal_start; // Offset of literal text before this spec
    usize literal_len;   // Length of literal text before this spec
} FormatOccurrence;

typedef struct ParsedFormatSpec {
    A$$(fmt_max_args, FormatOccurrence) occurrences;
    usize occurrence_count;
    u8 max_arg_index; // Highest argument index seen
    usize trailing_literal_start;
    usize trailing_literal_len;
} ParsedFormatSpec;
T_use$((ParsedFormatSpec)(E));

typedef struct ArgInfo {
    u8 index;
    O$fmt_TypePrefix wrapper;
    fmt_ArgValue_Tag tag;
} ArgInfo;
T_use$((ArgInfo)(P, S, O));

// Single-pass parsing - NO REPARSING LATER
fn_((parseFormatSpecOnce(S_const$u8 fmt))(E$ParsedFormatSpec) $scope) {
    ParsedFormatSpec result = {};
    u8 next_positional = 0;
    S_const$u8 scan_fmt = fmt;
    usize current_literal_start = 0;

    while (0 < scan_fmt.len) {
        usize offset = scan_fmt.ptr - fmt.ptr;
        u8 ch = *atS(scan_fmt, 0);
        if (ch == u8_c('{')) {
            // Handle escaped '{{'
            if (1 < scan_fmt.len && *atS(scan_fmt, 1) == u8_c('{')) {
                scan_fmt = suffix$S(scan_fmt, 2);
                continue;
            }
            // Parse format spec
            let parsed = catch_((parseFormat(scan_fmt))($ignore, {
                // Malformed - skip to closing brace or next char
                usize close_pos = 1;
                while (close_pos < scan_fmt.len && *atS(scan_fmt, close_pos) != u8_c('}')) {
                    close_pos++;
                }
                scan_fmt = close_pos < scan_fmt.len ? suffix$S(scan_fmt, close_pos + 1) : suffix$S(scan_fmt, 1);
                continue;
            }));
            // printf("After parseFormat: spec.type=%d, spec.size=%d\n", parsed.spec.type, parsed.spec.size);
            // Store this occurrence with its literal prefix
            if (result.occurrence_count >= fmt_max_args) {
                return_err(fmt_Err_IndexOutOfBounds());
            }
            // printf("--- next_positional: %d\n", next_positional);
            let arg_index = expr_(u8 $scope)(if_some((parsed.spec.index)(idx)) {
                next_positional = prim_max(next_positional, idx + 1);
                $break_(idx);
            }) expr_(else_none)({
                $break_(next_positional++);
            }) $unscoped_(expr);
            if (arg_index >= fmt_max_args) {
                return_err(fmt_Err_IndexOutOfBounds());
            }
            asg_lit((atA(result.occurrences, result.occurrence_count))({ .spec = parsed.spec,
                                                                         .arg_index = arg_index,
                                                                         .literal_start = current_literal_start,
                                                                         .literal_len = offset - current_literal_start }));
            // printf("Stored in occurrence[%zu]: type=%d, size=%d\n", result.occurrence_count, atA(result.occurrences, result.occurrence_count)->spec.type, atA(result.occurrences, result.occurrence_count)->spec.size);
            result.occurrence_count++;
            result.max_arg_index = prim_max(result.max_arg_index, arg_index);
            scan_fmt = parsed.remain;
            current_literal_start = scan_fmt.ptr - fmt.ptr;
        } else if (ch == u8_c('}')) {
            // Handle escaped '}}'
            if (1 < scan_fmt.len && *atS(scan_fmt, 1) == u8_c('}')) {
                scan_fmt = suffix$S(scan_fmt, 2);
                continue;
            }
            scan_fmt = suffix$S(scan_fmt, 1);
        } else {
            scan_fmt = suffix$S(scan_fmt, 1);
        }
    }
    // Store trailing literal info
    result.trailing_literal_start = current_literal_start;
    result.trailing_literal_len = (fmt.ptr + fmt.len) - (fmt.ptr + current_literal_start);
    return_ok(result);
} $unscoped_(fn);

// Write literal text handling escaped braces
fn_((writeLiteralChunk(io_Writer writer, S_const$u8 fmt, usize start, usize len))(E$void) $scope) {
    if (len == 0) {
        return_ok({});
    }
    S_const$u8 chunk = { .ptr = fmt.ptr + start, .len = len };
    usize pos = 0;
    while (pos < chunk.len) {
        u8 ch = *atS(chunk, pos);
        // Check for escaped braces
        if (ch == u8_c('{') && pos + 1 < chunk.len && *atS(chunk, pos + 1) == u8_c('{')) {
            try_(io_Writer_writeByte(writer, u8_c('{')));
            pos += 2;
        } else if (ch == u8_c('}') && pos + 1 < chunk.len && *atS(chunk, pos + 1) == u8_c('}')) {
            try_(io_Writer_writeByte(writer, u8_c('}')));
            pos += 2;
        } else {
            try_(io_Writer_writeByte(writer, ch));
            pos++;
        }
    }
    return_ok({});
} $unscoped_(fn);

$static fn_((isIntegerTag(fmt_ArgValue_Tag tag))(bool)) {
    return (tag >= fmt_ArgValue_u8 && tag <= fmt_ArgValue_usize) || (tag >= fmt_ArgValue_i8 && tag <= fmt_ArgValue_isize);
}

$static fn_((getTagSize(fmt_ArgValue_Tag tag))(u8)) {
    switch (tag) {
    case fmt_ArgValue_u8:
    case fmt_ArgValue_i8:
        return 8;
    case fmt_ArgValue_u16:
    case fmt_ArgValue_i16:
        return 16;
    case fmt_ArgValue_u32:
    case fmt_ArgValue_i32:
        return 32;
    case fmt_ArgValue_u64:
    case fmt_ArgValue_i64:
        return 64;
    default:
        return 0;
    }
}

// Helper: Get most general compatible type
$static fn_((getMostGeneralType(fmt_ArgValue_Tag a, fmt_ArgValue_Tag b))(fmt_ArgValue_Tag)) {
    // If both same, return either
    if (a == b) { return a; }

    // Signed/unsigned of same size: prefer unsigned (can format both ways)
    if (a == fmt_ArgValue_i32 && b == fmt_ArgValue_u32) { return fmt_ArgValue_u32; }
    if (a == fmt_ArgValue_u32 && b == fmt_ArgValue_i32) { return fmt_ArgValue_u32; }
    if (a == fmt_ArgValue_i64 && b == fmt_ArgValue_u64) { return fmt_ArgValue_u64; }
    if (a == fmt_ArgValue_u64 && b == fmt_ArgValue_i64) { return fmt_ArgValue_u64; }

    // Different sizes: prefer larger
    if (isIntegerTag(a) && isIntegerTag(b)) {
        return getTagSize(a) > getTagSize(b) ? a : b;
    }

    // Incompatible types: keep first (best effort)
    return a;
}

// Main formatting function - ZERO REPARSING
fn_((fmt_formatVaArgs(io_Writer writer, S_const$u8 fmt, va_list va_args))(E$void) $scope) {
    // Parse format string ONCE
    let parsed = try_(parseFormatSpecOnce(fmt));

    // STEP 1: Determine the most general type needed for each argument
    // by examining ALL occurrences that use that argument
    A$$(fmt_max_args, O$fmt_ArgValue_Tag) arg_general_types = zero$A();

    for (usize i = 0; i < parsed.occurrence_count; ++i) {
        let occurrence = atA(parsed.occurrences, i);
        let arg_idx = occurrence->arg_index;
        let tag = catch_((specToArgTag(occurrence->spec.type, occurrence->spec.size))($ignore, continue));

        if_none((*atA(arg_general_types, arg_idx))) {
            // First occurrence of this arg - use its type
            asg_lit((atA(arg_general_types, arg_idx))(some(tag)));
        } else_some((existing_tag)) {
            // Argument reused - pick most general compatible type
            let general_tag = getMostGeneralType(existing_tag, tag);
            asg_lit((atA(arg_general_types, arg_idx))(some(general_tag)));
        }
    }

    // STEP 2: Build collection info using general types
    A$$(fmt_max_args, O$ArgInfo) arg_collection_info = zero$A();

    for (usize i = 0; i < parsed.occurrence_count; ++i) {
        let occurrence = atA(parsed.occurrences, i);
        let arg_idx = occurrence->arg_index;

        // Skip if already have collection info for this arg
        if_some((*atA(arg_collection_info, arg_idx))(info)) {
            let_ignore = info;
        } else if_some((*atA(arg_general_types, arg_idx))(general_tag)) {
            asg_lit((atA(arg_collection_info, arg_idx))(some({
                .index = arg_idx,
                .wrapper = occurrence->spec.type_prefix, // Use first occurrence's wrapper
                .tag = general_tag                       // Use general type
            })));
        }
    }

    // STEP 3: Collect all arguments from va_list in sequential order
    A$$(fmt_max_args, O$fmt_ArgType) collected_args = zero$A();

    for (u8 i = 0; i <= parsed.max_arg_index && i < fmt_max_args; ++i) {
        if_some((*atA(arg_collection_info, i))(info)) {
            let arg = collectArg(&va_args, info.wrapper, info.tag);
            asg_lit((atA(collected_args, i))(some(arg)));
        } else_none {
            // Unused arg index - consume dummy
            let dummy = va_arg(va_args, Void);
            let_ignore = dummy;
        }
    }

    // STEP 4: Format using per-occurrence specs
    for (usize i = 0; i < parsed.occurrence_count; ++i) {
        let occurrence = atA(parsed.occurrences, i);

        try_(writeLiteralChunk(writer, fmt, occurrence->literal_start, occurrence->literal_len));

        let arg_idx = occurrence->arg_index;
        if_some((*atA(collected_args, arg_idx))(arg)) {
            // Each occurrence has its own spec - no deduplication
            try_(formatArg(writer, arg, occurrence->spec));
        }
    }

    try_(writeLiteralChunk(writer, fmt, parsed.trailing_literal_start, parsed.trailing_literal_len));

    return_ok({});
} $unscoped_(fn);



/*========== Parsing Functions ==============================================*/

fn_((fmt_parseBool(S_const$u8 str))(E$bool) $scope) {
    str = skipWhitespace(str);
    if (mem_eqlBytes(str, mem_asBytes(&u8_c('1')).as_const) || mem_eqlBytes(str, u8_l("true"))) { return_ok(true); }
    if (mem_eqlBytes(str, mem_asBytes(&u8_c('0')).as_const) || mem_eqlBytes(str, u8_l("false"))) { return_ok(false); }
    return_err(fmt_Err_InvalidBoolFormat());
} $unscoped_(fn);

fn_((fmt_parse$bool(S_const$u8 str))(E$bool)) { return fmt_parseBool(str); }

fn_((fmt_parseUInt(S_const$u8 str, u8 base))(E$u64) $scope) {
    str = skipWhitespace(str);
    if (str.len == 0) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    if (base < 2 || base > 36) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    u64 result = 0;
    for (usize i = 0; i < str.len; i++) {
        u8 ch = str.ptr[i];
        if (ascii_isWhitespace(ch)) {
            break;
        }
        u8 digit_val = 0;
        if (ascii_isDigit(ch)) {
            digit_val = ch - u8_c('0');
        } else if (u8_c('a') <= ch && ch <= u8_c('z')) {
            digit_val = u8_add(10, ch - u8_c('a'));
        } else if (u8_c('A') <= ch && ch <= u8_c('Z')) {
            digit_val = u8_add(10, ch - u8_c('A'));
        } else {
            if (i == 0) {
                return_err(fmt_Err_InvalidIntegerFormat());
            }
            break;
        }
        if (digit_val >= base) {
            if (i == 0) {
                return_err(fmt_Err_InvalidIntegerFormat());
            }
            break;
        }
        // Check for overflow using checked arithmetic
        let mul_result = u64_mulChkd(result, base);
        if (isNone(mul_result)) {
            return_err(fmt_Err_InvalidIntegerFormat());
        }
        let add_result = u64_addChkd(unwrap_(mul_result), digit_val);
        if (isNone(add_result)) {
            return_err(fmt_Err_InvalidIntegerFormat());
        }
        result = unwrap_(add_result);
    }
    return_ok(result);
} $unscoped_(fn);

fn_((fmt_parse$usize(S_const$u8 str, u8 base))(E$usize) $scope) {
    return_ok(as$(usize)(try_(fmt_parseUInt(str, base))));
} $unscoped_(fn);

fn_((fmt_parse$u64(S_const$u8 str, u8 base))(E$u64)) { return fmt_parseUInt(str, base); }

fn_((fmt_parse$u32(S_const$u8 str, u8 base))(E$u32) $scope) {
    let result = try_(fmt_parseUInt(str, base));
    if (result > u32_limit_max) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$(u32)(result));
} $unscoped_(fn);

fn_((fmt_parse$u16(S_const$u8 str, u8 base))(E$u16) $scope) {
    let result = try_(fmt_parseUInt(str, base));
    if (result > u16_limit_max) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$(u16)(result));
} $unscoped_(fn);

fn_((fmt_parse$u8(S_const$u8 str, u8 base))(E$u8) $scope) {
    let result = try_(fmt_parseUInt(str, base));
    if (result > u8_limit_max) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$(u8)(result));
} $unscoped_(fn);

fn_((fmt_parseIInt(S_const$u8 str, u8 base))(E$i64) $scope) {
    str = skipWhitespace(str);
    if (str.len == 0) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    bool negative = false;
    if (str.ptr[0] == u8_c('-')) {
        negative = true;
        str.ptr++;
        str.len--;
    } else if (str.ptr[0] == u8_c('+')) {
        str.ptr++;
        str.len--;
    }
    let unsigned_result = try_(fmt_parseUInt(str, base));
    if (negative) {
        const u64 max_neg = as$(u64)(i64_limit_max) + 1;
        if (unsigned_result > max_neg) {
            return_err(fmt_Err_InvalidIntegerFormat());
        }
        return_ok(-as$(i64)(unsigned_result));
    } else {
        if (unsigned_result > as$(u64)(i64_limit_max)) {
            return_err(fmt_Err_InvalidIntegerFormat());
        }
        return_ok(as$(i64)(unsigned_result));
    }
} $unscoped_(fn);

fn_((fmt_parse$isize(S_const$u8 str, u8 base))(E$isize) $scope) {
    let result = try_(fmt_parseIInt(str, base));
    return_ok(as$(isize)(result));
} $unscoped_(fn);

fn_((fmt_parse$i64(S_const$u8 str, u8 base))(E$i64)) {
    return fmt_parseIInt(str, base);
}

fn_((fmt_parse$i32(S_const$u8 str, u8 base))(E$i32) $scope) {
    let result = try_(fmt_parseIInt(str, base));
    if (result < i32_limit_min || i32_limit_max < result) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$(i32)(result));
} $unscoped_(fn);

fn_((fmt_parse$i16(S_const$u8 str, u8 base))(E$i16) $scope) {
    let result = try_(fmt_parseIInt(str, base));
    if (result < i16_limit_min || i16_limit_max < result) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$(i16)(result));
} $unscoped_(fn);

fn_((fmt_parse$i8(S_const$u8 str, u8 base))(E$i8) $scope) {
    let result = try_(fmt_parseIInt(str, base));
    if (result < i8_limit_min || i8_limit_max < result) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$(i8)(result));
} $unscoped_(fn);

fn_((fmt_parseFlt(S_const$u8 str))(E$f64) $scope) {
    str = skipWhitespace(str);
    if (str.len == 0) {
        return_err(fmt_Err_InvalidFloatFormat());
    }
    usize pos = 0;
    f64 sign = 1.0;
    f64 result = 0.0;
    bool has_int_part = false;
    // Check for sign
    if (pos < str.len && str.ptr[pos] == u8_c('+')) {
        pos++;
    } else if (pos < str.len && str.ptr[pos] == u8_c('-')) {
        sign = -1.0;
        pos++;
    }
    // Parse integer part
    while (pos < str.len && ascii_isDigit(str.ptr[pos])) {
        result = (result * 10.0) + (str.ptr[pos] - u8_c('0'));
        has_int_part = true;
        pos++;
    }
    // Parse fractional part
    if (pos < str.len && str.ptr[pos] == u8_c('.')) {
        pos++;
        f64 power = 0.1;
        bool has_frac_part = false;
        while (pos < str.len && ascii_isDigit(str.ptr[pos])) {
            result += (str.ptr[pos] - u8_c('0')) * power;
            power *= 0.1;
            has_frac_part = true;
            pos++;
        }
        if (!has_int_part && !has_frac_part) {
            return_err(fmt_Err_InvalidFloatFormat());
        }
    } else {
        if (!has_int_part) {
            return_err(fmt_Err_InvalidFloatFormat());
        }
    }
    // Parse exponent part
    if (pos < str.len && (str.ptr[pos] == u8_c('e') || str.ptr[pos] == u8_c('E'))) {
        pos++;
        f64 exp_sign = 1.0;
        i32 exp_val = 0;
        bool has_exp = false;

        if (pos < str.len && str.ptr[pos] == u8_c('+')) {
            pos++;
        } else if (pos < str.len && str.ptr[pos] == u8_c('-')) {
            exp_sign = -1.0;
            pos++;
        }
        while (pos < str.len && ascii_isDigit(str.ptr[pos])) {
            exp_val = (exp_val * 10) + (str.ptr[pos] - u8_c('0'));
            has_exp = true;
            pos++;
        }
        if (!has_exp) {
            return_err(fmt_Err_InvalidFloatFormat());
        }
        result *= __builtin_pow(10.0, exp_val * exp_sign);
    }
    // Check that we consumed all input
    if (pos != str.len) {
        return_err(fmt_Err_InvalidFloatFormat());
    }
    return_ok(result * sign);
} $unscoped_(fn);

fn_((fmt_parse$f64(S_const$u8 str))(E$f64)) { return fmt_parseFlt(str); }

fn_((fmt_parse$f32(S_const$u8 str))(E$f32) $scope) {
    let result = try_(fmt_parseFlt(str));
    return_ok(as$(f32)(result));
} $unscoped_(fn);

/*========== Internal Functions =============================================*/

fn_((parseU8(S_const$u8 str))(E$u8) $scope) {
    // printf("--- debug print: parseU8 ---\n");
    if (str.len == 0 || !ascii_isDigit(*atS(str, 0))) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    u8 result = 0;
    for_(($s(str))(ch) {
        if (!ascii_isDigit(*ch)) { break; }
        let digit = digitToInt(*ch);
        let mul = orelse_((u8_mulChkd(result, 10))(return_err(fmt_Err_InvalidIntegerFormat())));
        let add = orelse_((u8_addChkd(mul, digit))(return_err(fmt_Err_InvalidIntegerFormat())));
        result = add;
    });
    return_ok(result);
} $unscoped_(fn);

fn_((consumeU8(S_const$u8 str))(E$ConsumedU8) $scope) {
    // printf("--- debug print: consumeU8 ---\n");
    usize digit_count = 0;
    while (digit_count < str.len && ascii_isDigit(str.ptr[digit_count])) { digit_count++; }
    if (digit_count == 0) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    let digits = prefixS(str, digit_count);
    let value = try_(parseU8(digits));
    return_ok({ .value = value, .remain = suffix$S(str, digit_count) });
} $unscoped_(fn);

fn_((parseFormat(S_const$u8 fmt_str))(E$ParsedFormat) $scope) {
    // printf("--- debug print: parseFormat ---\n");
    // Skip opening '{'
    fmt_str = suffix$S(fmt_str, 1);
    if (fmt_str.len == 0) {
        return_err(fmt_Err_MissingClosingBrace());
    }

    // Parse optional index (must be followed by ':')
    let index = expr_(O$u8 $scope)({
        let ch = *atS(fmt_str, 0);
        if (ascii_isDigit(ch)) {
            usize colon_pos = 0;
            while (colon_pos < fmt_str.len) {
                let colon_pos_ch = *atS(fmt_str, colon_pos);
                if (colon_pos_ch == u8_c(':') || colon_pos_ch == u8_c('}')) { break; }
                if (!ascii_isDigit(colon_pos_ch)) { break; /* Not an index, treat as width */ }
                colon_pos++;
            }
            if (colon_pos < fmt_str.len) {
                let colon_pos_ch = *atS(fmt_str, colon_pos);
                if (colon_pos_ch == u8_c(':')) {
                    let index_slice = prefix$S(fmt_str, colon_pos);
                    let index = try_(parseU8(index_slice));
                    if (index >= fmt_max_args) {
                        return_err(fmt_Err_IndexOutOfBounds());
                    }
                    fmt_str = suffix$S(fmt_str, colon_pos + 1); // Skip index and ':'
                    $break_(some(index));
                }
            }
        }
        $break_(none());
    }) $unscoped_(expr);

    // Skip optional ':' at format start
    if (0 < fmt_str.len && *atS(fmt_str, 0) == u8_c(':')) {
        fmt_str = suffix$S(fmt_str, 1);
    }

    // Parse fill and alignment (more efficient than function call)
    let fill_w_align = expr_(struct { O$u8 fill; O$fmt_Align align; } $scope)({
        if (fmt_str.len >= 2) {
            let maybe_fill = *atS(fmt_str, 0);
            let maybe_align = *atS(fmt_str, 1);

            // Check if second char is alignment
            if_some((fmt_Align_parse(maybe_align))(align)) {
                fmt_str = suffix$S(fmt_str, 2);
                $break_({ .fill = some(maybe_fill), .align = some(align) });
            } else_none {
                // Check if first char is alignment
                if_some((fmt_Align_parse(maybe_fill))(align)) {
                    fmt_str = suffix$S(fmt_str, 1);
                    $break_({ .fill = none(), .align = some(align) });
                }
            }
        } else if (fmt_str.len >= 1) {
            let maybe_align = *atS(fmt_str, 0);
            if_some((fmt_Align_parse(maybe_align))(align)) {
                fmt_str = suffix$S(fmt_str, 1);
                $break_({ .fill = none(), .align = some(align) });
            }
        }
        $break_({ .fill = none(), .align = none() });
    }) $unscoped_(expr);
    let fill = fill_w_align.fill;
    let align = fill_w_align.align;

    // Parse sign flag (+, -, or space)
    let sign = expr_(fmt_Sign $scope)(if (0 < fmt_str.len) {
        let ch = *atS(fmt_str, 0);
        if (ch == u8_c('+')) {
            fmt_str = suffix$S(fmt_str, 1);
            $break_(fmt_Sign_always);
        } else if (ch == u8_c(' ')) {
            fmt_str = suffix$S(fmt_str, 1);
            $break_(fmt_Sign_space);
        }
    }) expr_(else)($break_(fmt_Sign_auto)) $unscoped_(expr);

    // Parse alternate form flag (#)
    let alt_form = expr_(bool $scope)(if (0 < fmt_str.len && *atS(fmt_str, 0) == u8_c('#')) {
        fmt_str = suffix$S(fmt_str, 1);
        $break_(true);
    }) expr_(else)($break_(false)) $unscoped_(expr);

    // Parse width
    let width = expr_(O$u8 $scope)(if (0 < fmt_str.len && ascii_isDigit(*atS(fmt_str, 0))) {
        let consumed = try_(consumeU8(fmt_str));
        fmt_str = consumed.remain;
        $break_(some(consumed.value));
    }) expr_(else)($break_(none())) $unscoped_(expr);

    // Parse precision
    let precision = expr_(O$u8 $scope)(if (0 < fmt_str.len && *atS(fmt_str, 0) == u8_c('.')) {
        let consumed = try_(consumeU8(suffix$S(fmt_str, 1)));
        fmt_str = consumed.remain;
        $break_(some(consumed.value));
    }) expr_(else)($break_(none())) $unscoped_(expr);

    // Parse type prefix (? or !)
    let type_prefix = expr_(O$fmt_TypePrefix $scope)(if (0 < fmt_str.len) {
        if_some((fmt_TypePrefix_parse(*atS(fmt_str, 0)))(prefix)) { /* NOLINT */
            fmt_str = suffix$S(fmt_str, 1);
            $break_(some(prefix));
        };
    }) expr_(else)($break_(none())) $unscoped_(expr);

    /* clang-format off */
    let type_w_size = expr_(struct { fmt_Type type; fmt_Size size; } $scope)({
        // Parse type character (required if we reach here)
        let type_ch = blk({
            if (fmt_str.len == 0) {
                return_err(fmt_Err_InvalidFormatSpecifier());
            }
            let ch = *atS(fmt_str, 0);
            fmt_str = suffix$S(fmt_str, 1);
            blk_return_(ch);
        });
        // Parse size modifier - use pre-built API for multi-char patterns
        let size = expr_(fmt_Size $scope)({
            if (2 <= fmt_str.len) if_some((fmt_Size_parse(prefix$S(fmt_str, 2)))(size)) { /* NOLINT */
                fmt_str = suffix$S(fmt_str, 2);
                $break_(size);
            }
            if (1 <= fmt_str.len) if_some((fmt_Size_parse(prefix$S(fmt_str, 1)))(size)) { /* NOLINT */
                fmt_str = suffix$S(fmt_str, 1);
                $break_(size);
            }
            $break_(fmt_Size_default);
        }) $unscoped_(expr);
        // Map type character to enum (switch is faster than fmt_Type_parse)
        switch (type_ch) {
            case u8_c('x'): $break_({ .type = fmt_Type_hex_lower, .size = size });
            case u8_c('X'): $break_({ .type = fmt_Type_hex_upper, .size = size });
            case u8_c('o'): $break_({ .type = fmt_Type_octal, .size = size });
            case u8_c('b'): $break_({ .type = fmt_Type_binary, .size = size });
            case u8_c('B'): $break_({ .type = fmt_Type_boolean, .size = size });
            case u8_c('u'): $break_({ .type = fmt_Type_unsigned, .size = size });
            case u8_c('d'):
            case u8_c('i'): $break_({ .type = fmt_Type_signed, .size = size });
            case u8_c('f'): $break_({ .type = fmt_Type_float_lower, .size = size });
            case u8_c('F'): $break_({ .type = fmt_Type_float_upper, .size = size });
            case u8_c('p'): $break_({ .type = fmt_Type_pointer_lower, .size = size });
            case u8_c('P'): $break_({ .type = fmt_Type_pointer_upper, .size = size });
            case u8_c('e'): $break_({ .type = fmt_Type_error, .size = size });
            case u8_c('0'): $break_({ .type = fmt_Type_void, .size = size });
            case u8_c('c'): $break_({ .type = fmt_Type_ascii_code, .size = size });
            case u8_c('C'): $break_({ .type = fmt_Type_utf8_codepoint, .size = size });
            case u8_c('z'): $break_({ .type = fmt_Type_string_z0, .size = size });
            case u8_c('s'): $break_({ .type = fmt_Type_string_s, .size = size });
            default: return_err(fmt_Err_InvalidFormatSpecifier());
        }
    }) $unscoped_(expr);
    /* clang-format on */
    let type = type_w_size.type;
    let size = type_w_size.size;

    // printf("parseFormat: type=%d, size=%d\n", type_w_size.type, type_w_size.size);
    // Expect closing '}'
    if (0 < fmt_str.len && *atS(fmt_str, 0) != u8_c('}')) {
        return_err(fmt_Err_MissingClosingBrace());
    }
    return_ok({
        .spec = {
            .index = index,
            .fill = fill,
            .align = align,
            .sign = sign,
            .alt_form = alt_form,
            .width = width,
            .precision = precision,
            .type_prefix = type_prefix,
            .type = type,
            .size = size,
        },
        .remain = suffix$S(fmt_str, 1), // Skip '}'
    });
} $unscoped_(fn);

fn_((specToArgTag(fmt_Type type, fmt_Size size))(E$fmt_ArgValue_Tag) $scope) {
    // printf("--- debug print: specToArgTag ---\n");
    /* clang-format off */
    switch (type) {
    case fmt_Type_void:
        return_ok(fmt_ArgValue_void);
    case fmt_Type_boolean:
        return_ok(fmt_ArgValue_bool);
    case fmt_Type_unsigned:
    case fmt_Type_hex_lower:
    case fmt_Type_hex_upper:
    case fmt_Type_octal:
    case fmt_Type_binary:
        switch (size) {
        case fmt_Size_8:   return_ok(fmt_ArgValue_u8);
        case fmt_Size_16:  return_ok(fmt_ArgValue_u16);
        case fmt_Size_32:  return_ok(fmt_ArgValue_u32);
        case fmt_Size_64:  return_ok(fmt_ArgValue_u64);
        case fmt_Size_ptr: return_ok(fmt_ArgValue_usize);
        default: return_err(fmt_Err_InvalidFormatSpecifier());
        }
    case fmt_Type_signed:
        switch (size) {
        case fmt_Size_8:   return_ok(fmt_ArgValue_i8);
        case fmt_Size_16:  return_ok(fmt_ArgValue_i16);
        case fmt_Size_32:  return_ok(fmt_ArgValue_i32);
        case fmt_Size_64:  return_ok(fmt_ArgValue_i64);
        case fmt_Size_ptr: return_ok(fmt_ArgValue_isize);
        default: return_err(fmt_Err_InvalidFormatSpecifier());
        }
    case fmt_Type_float_lower:
    case fmt_Type_float_upper:
        switch (size) {
        case fmt_Size_32: return_ok(fmt_ArgValue_f32);
        case fmt_Size_64: return_ok(fmt_ArgValue_f64);
        default: return_err(fmt_Err_InvalidFormatSpecifier());
        }
    case fmt_Type_pointer_lower:
    case fmt_Type_pointer_upper:
        return_ok(fmt_ArgValue_ptr);
    case fmt_Type_error:
        return_ok(fmt_ArgValue_error);
    case fmt_Type_ascii_code:
        return_ok(fmt_ArgValue_u8);
    case fmt_Type_utf8_codepoint:
        return_ok(fmt_ArgValue_u32);
    case fmt_Type_string_z0:
        return_ok(fmt_ArgValue_sli_z_u8);
    case fmt_Type_string_s:
        return_ok(fmt_ArgValue_sli_u8);
    default:
        return_err(fmt_Err_InvalidFormatSpecifier());
    }
    /* clang-format on */
} $unscoped_(fn);


fn_((collectArg(va_list* ap, O$fmt_TypePrefix wrapper, fmt_ArgValue_Tag tag))(fmt_ArgType) $scope) {
    // printf("--- debug print: collectArg ---\n");
    if_none((wrapper)) {
        return_(union_of((fmt_ArgType_value)(collectArgValue(ap, tag))));
    } else_some((prefix)) {
        switch (prefix) {
        case fmt_TypePrefix_optional:
            return_(union_of((fmt_ArgType_optional)(collectArgOptional(ap, tag))));
        case fmt_TypePrefix_error_result:
            return_(union_of((fmt_ArgType_error_result)(collectArgErrorResult(ap, tag))));
        default:
            claim_unreachable;
        }
    }
    claim_unreachable;
} $unscoped_(fn);

fn_((collectArgValue(va_list* ap, fmt_ArgValue_Tag tag))(fmt_ArgValue) $scope) {
    // printf("--- debug print: collectArgValue ---\n");
    switch (tag) {
    case fmt_ArgValue_void:
        return_(union_of((fmt_ArgValue_void)(va_arg(*ap, Void))));
    case fmt_ArgValue_bool:
        return_(union_of((fmt_ArgValue_bool)(as$(bool)(va_arg(*ap, u32)))));
    case fmt_ArgValue_u8:
        return_(union_of((fmt_ArgValue_u8)(as$(u8)(va_arg(*ap, u32)))));
    case fmt_ArgValue_u16:
        return_(union_of((fmt_ArgValue_u16)(as$(u16)(va_arg(*ap, u32)))));
    case fmt_ArgValue_u32:
        return_(union_of((fmt_ArgValue_u32)(va_arg(*ap, u32))));
    case fmt_ArgValue_u64:
        return_(union_of((fmt_ArgValue_u64)(va_arg(*ap, u64))));
    case fmt_ArgValue_usize:
        return_(union_of((fmt_ArgValue_usize)(va_arg(*ap, usize))));
    case fmt_ArgValue_i8:
        return_(union_of((fmt_ArgValue_i8)(as$(i8)(va_arg(*ap, i32)))));
    case fmt_ArgValue_i16:
        return_(union_of((fmt_ArgValue_i16)(as$(i16)(va_arg(*ap, i32)))));
    case fmt_ArgValue_i32:
        return_(union_of((fmt_ArgValue_i32)(va_arg(*ap, i32))));
    case fmt_ArgValue_i64:
        return_(union_of((fmt_ArgValue_i64)(va_arg(*ap, i64))));
    case fmt_ArgValue_isize:
        return_(union_of((fmt_ArgValue_isize)(va_arg(*ap, isize))));
    case fmt_ArgValue_f32:
        return_(union_of((fmt_ArgValue_f32)(as$(f32)(va_arg(*ap, f64)))));
    case fmt_ArgValue_f64:
        return_(union_of((fmt_ArgValue_f64)(va_arg(*ap, f64))));
    case fmt_ArgValue_ptr:
        return_(union_of((fmt_ArgValue_ptr)(va_arg(*ap, const void*))));
    case fmt_ArgValue_error:
        return_(union_of((fmt_ArgValue_error)(va_arg(*ap, Err))));
    case fmt_ArgValue_sli_z_u8:
        return_(union_of((fmt_ArgValue_sli_z_u8)(va_arg(*ap, const u8*))));
    case fmt_ArgValue_sli_u8:
        return_(union_of((fmt_ArgValue_sli_u8)(va_arg(*ap, S_const$u8))));
    default:
        claim_unreachable;
    }
} $unscoped_(fn);

fn_((collectArgOptional(va_list* ap, fmt_ArgValue_Tag tag))(O$fmt_ArgValue) $scope) {
    // printf("--- debug print: collectArgOptional ---\n");
    switch (tag) {
    case fmt_ArgValue_void: {
        let arg = va_arg(*ap, O$$(Void));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_void)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_bool: {
        let arg = va_arg(*ap, O$$(bool));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_bool)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_u8: {
        let arg = va_arg(*ap, O$$(u8));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_u8)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_u16: {
        let arg = va_arg(*ap, O$$(u16));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_u16)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_u32: {
        let arg = va_arg(*ap, O$$(u32));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_u32)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_u64: {
        let arg = va_arg(*ap, O$$(u64));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_u64)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_usize: {
        let arg = va_arg(*ap, O$$(usize));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_usize)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_i8: {
        let arg = va_arg(*ap, O$$(i8));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_i8)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_i16: {
        let arg = va_arg(*ap, O$$(i16));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_i16)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_i32: {
        let arg = va_arg(*ap, O$$(i32));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_i32)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_i64: {
        let arg = va_arg(*ap, O$$(i64));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_i64)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_isize: {
        let arg = va_arg(*ap, O$$(isize));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_isize)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_f32: {
        let arg = va_arg(*ap, O$$(f32));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_f32)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_f64: {
        let arg = va_arg(*ap, O$$(f64));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_f64)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_ptr: {
        let arg = va_arg(*ap, O$$(const void*));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_ptr)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_error: {
        let arg = va_arg(*ap, O$$(Err));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_error)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_sli_z_u8: {
        let arg = va_arg(*ap, O$$(const u8*));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_sli_z_u8)(value)));
        } else_none
            return_none();
    }
    case fmt_ArgValue_sli_u8: {
        let arg = va_arg(*ap, O$$(S_const$u8));
        if_some((arg)(value)) {
            return_some(union_of((fmt_ArgValue_sli_u8)(value)));
        } else_none
            return_none();
    }
    default:
        claim_unreachable;
    }
} $unscoped_(fn);

fn_((collectArgErrorResult(va_list* ap, fmt_ArgValue_Tag tag))(E$fmt_ArgValue) $scope) {
    // printf("--- debug print: collectArgErrorResult ---\n");
    switch (tag) {
    case fmt_ArgValue_void: {
        let arg = va_arg(*ap, E$$(Void));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_void)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_bool: {
        let arg = va_arg(*ap, E$$(bool));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_bool)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_u8: {
        let arg = va_arg(*ap, E$$(u8));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_u8)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_u16: {
        let arg = va_arg(*ap, E$$(u16));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_u16)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_u32: {
        let arg = va_arg(*ap, E$$(u32));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_u32)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_u64: {
        let arg = va_arg(*ap, E$$(u64));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_u64)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_usize: {
        let arg = va_arg(*ap, E$$(usize));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_usize)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_i8: {
        let arg = va_arg(*ap, E$$(i8));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_i8)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_i16: {
        let arg = va_arg(*ap, E$$(i16));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_i16)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_i32: {
        let arg = va_arg(*ap, E$$(i32));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_i32)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_i64: {
        let arg = va_arg(*ap, E$$(i64));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_i64)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_isize: {
        let arg = va_arg(*ap, E$$(isize));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_isize)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_f32: {
        let arg = va_arg(*ap, E$$(f32));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_f32)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_f64: {
        let arg = va_arg(*ap, E$$(f64));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_f64)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_ptr: {
        let arg = va_arg(*ap, E$$(const void*));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_ptr)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_error: {
        let arg = va_arg(*ap, E$$(Err));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_error)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_sli_z_u8: {
        let arg = va_arg(*ap, E$$(const u8*));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_sli_z_u8)(value)));
        } else_err(e)
            return_(err(e));
    }
    case fmt_ArgValue_sli_u8: {
        let arg = va_arg(*ap, E$$(S_const$u8));
        if_ok((arg)(value)) {
            return_ok(union_of((fmt_ArgValue_sli_u8)(value)));
        } else_err(e)
            return_(err(e));
    }
    default:
        claim_unreachable;
    }
} $unscoped_(fn);


fn_((formatArg(io_Writer writer, fmt_ArgType arg, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatArg ---\n");
    match_(arg) {
        pattern_((fmt_ArgType_value)(value)) {
            return formatArgValue(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgType_optional)(optional)) {
            return formatArgOptional(writer, *optional, spec);
        } $end(pattern);
        pattern_((fmt_ArgType_error_result)(error_result)) {
            return formatArgErrorResult(writer, *error_result, spec);
        } $end(pattern);
        default_() {
            return_err(fmt_Err_InvalidFormatSpecifier());
        } $end(default);
    } $end(match);
} $unscoped_(fn);

fn_((formatArgValue(io_Writer writer, fmt_ArgValue value, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatArgValue ---\n");
    match_(value) {
        pattern_((fmt_ArgValue_void)(value)) {
            return_ok(*value);
        } $end(pattern);
        pattern_((fmt_ArgValue_bool)(value)) {
            return formatBool(writer, value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_u8)(value)) {
            // printf("formatArgValue u8: value=%d, spec.type=%d\n", *value, spec.type);
            if (spec.type == fmt_Type_signed) {
                return formatIInt(writer, as$(i8)(*value), spec);
            }
            if (spec.type == fmt_Type_ascii_code) {
                return formatASCIICode(writer, *value, spec);
            }
            return formatUInt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_u16)(value)) {
            if (spec.type == fmt_Type_signed) {
                return formatIInt(writer, as$(i16)(*value), spec);
            }
            return formatUInt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_u32)(value)) {
            if (spec.type == fmt_Type_signed) {
                return formatIInt(writer, as$(i32)(*value), spec);
            }
            if (spec.type == fmt_Type_utf8_codepoint) {
                return formatUTF8Codepoint(writer, *value, spec);
            }
            return formatUInt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_u64)(value)) {
            if (spec.type == fmt_Type_signed) {
                return formatIInt(writer, as$(i64)(*value), spec);
            }
            return formatUInt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_usize)(value)) {
            if (spec.type == fmt_Type_signed) {
                return formatIInt(writer, as$(isize)(*value), spec);
            }
            return formatUInt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_i8)(value)) {
            if (spec.type == fmt_Type_unsigned
                || spec.type == fmt_Type_hex_lower
                || spec.type == fmt_Type_hex_upper
                || spec.type == fmt_Type_octal
                || spec.type == fmt_Type_binary) {
                return formatUInt(writer, as$(u8)(*value), spec);
            }
            return formatIInt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_i16)(value)) {
            if (spec.type == fmt_Type_unsigned
                || spec.type == fmt_Type_hex_lower
                || spec.type == fmt_Type_hex_upper
                || spec.type == fmt_Type_octal
                || spec.type == fmt_Type_binary) {
                return formatUInt(writer, as$(u16)(*value), spec);
            }
            return formatIInt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_i32)(value)) {
            if (spec.type == fmt_Type_unsigned
                || spec.type == fmt_Type_hex_lower
                || spec.type == fmt_Type_hex_upper
                || spec.type == fmt_Type_octal
                || spec.type == fmt_Type_binary) {
                return formatUInt(writer, as$(u32)(*value), spec);
            }
            return formatIInt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_i64)(value)) {
            if (spec.type == fmt_Type_unsigned
                || spec.type == fmt_Type_hex_lower
                || spec.type == fmt_Type_hex_upper
                || spec.type == fmt_Type_octal
                || spec.type == fmt_Type_binary) {
                return formatUInt(writer, as$(u64)(*value), spec);
            }
            return formatIInt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_isize)(value)) {
            if (spec.type == fmt_Type_unsigned
                || spec.type == fmt_Type_hex_lower
                || spec.type == fmt_Type_hex_upper
                || spec.type == fmt_Type_octal
                || spec.type == fmt_Type_binary) {
                return formatUInt(writer, as$(usize)(*value), spec);
            }
            return formatIInt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_f32)(value)) {
            return formatFlt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_f64)(value)) {
            return formatFlt(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_ptr)(value)) {
            return formatPtr(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_error)(value)) {
            return formatErr(writer, *value, spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_sli_z_u8)(value)) {
            let ptr = *value;
            let len = mem_lenZ0$u8(ptr);
            return formatStr(writer, lit$((S_const$u8){ .ptr = ptr, .len = len }), spec);
        } $end(pattern);
        pattern_((fmt_ArgValue_sli_u8)(value)) {
            return formatStr(writer, *value, spec);
        } $end(pattern);
        default_() {
            return_err(fmt_Err_InvalidFormatSpecifier());
        } $end(default);
    } $end(match);
} $unscoped_(fn);

fn_((formatArgOptional(io_Writer writer, O$fmt_ArgValue optional, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatArgOptional ---\n");
    // Handle void optional - just print tag
    if (spec.type == fmt_Type_void) {
        return writePadded(writer, optional.is_some ? u8_l("some") : u8_l("none"), spec);
    }
    // Handle none case
    if_none((optional)) {
        return writePadded(writer, u8_l("none"), spec);
    } else_some((value)) { // Format the inner value
        return formatArgValue(writer, value, spec);
    }
    return_ok({});
} $unscoped_(fn);

fn_((formatArgErrorResult(io_Writer writer, E$fmt_ArgValue error_result, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatArgErrorResult ---\n");
    // Handle void result - just print tag or error
    if (spec.type == fmt_Type_void) {
        if_ok((error_result)(value)) {
            let_ignore = value;
            return writePadded(writer, u8_l("ok"), spec);
        } else_err((e)) {
            return formatErr(writer, e, spec);
        }
    }
    // Handle error case
    if_err((error_result)(e)) {
        return formatErr(writer, e, spec);
    } else_ok((value)) { // Format the ok value
        return formatArgValue(writer, value, spec);
    }
    return_ok({});
} $unscoped_(fn);


fn_((formatBool(io_Writer writer, bool val, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatBool ---\n");
    return writePadded(writer, val ? u8_l("true") : u8_l("false"), spec);
} $unscoped_(fn);

fn_((formatUInt(io_Writer writer, u64 val, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatUInt ---\n");
    // printf("formatUInt: val=%llu, spec.type=%d\n", val, spec.type);
    A$$(128, u8) buf = zero$A();
    usize pos = lenA(buf) - 1;
    // Determine base
    let base_w_digits = expr_(struct { u32 base; S_const$u8 digits; } $scope)({
        // printf("  switch on spec.type=%d\n", spec.type);
        switch (spec.type) {
        case fmt_Type_hex_lower:
            $break_({ .base = 16, .digits = u8_l("0123456789abcdef") });
        case fmt_Type_hex_upper:
            $break_({ .base = 16, .digits = u8_l("0123456789ABCDEF") });
        case fmt_Type_octal:
            $break_({ .base = 8, .digits = u8_l("01234567") });
        case fmt_Type_binary:
            $break_({ .base = 2, .digits = u8_l("01") });
        case fmt_Type_unsigned:
            $break_({ .base = 10, .digits = u8_l("0123456789") });
        default:
            claim_unreachable;
        }
    }) $unscoped_(expr);
    let base = base_w_digits.base;
    let digits = base_w_digits.digits;
    // printf("  base=%u\n", base);
    // Apply size mask
    switch (spec.size) {
    case fmt_Size_8:
        val &= 0xFF;
        break;
    case fmt_Size_16:
        val &= 0xFFFF;
        break;
    case fmt_Size_32:
        val &= 0xFFFFFFFF;
        break;
    case fmt_Size_64:
        $fallthrough;
    case fmt_Size_128:
        // Fallback to 64-bit until 128-bit is fully supported
        $fallthrough;
    case fmt_Size_ptr:
        // No masking needed for 64-bit and pointer size
        break;
    default:
        claim_unreachable;
    }
    // Convert to string (reverse order)
    bool is_zero = (val == 0);
    if (is_zero) {
        *atA(buf, pos--) = u8_c('0');
    } else {
        while (val > 0) { /* NOLINT */
            *atA(buf, pos--) = *atS(digits, val % base);
            val /= base;
        }
    }

    // Add alternate form prefix if requested
    if (spec.alt_form) {
        switch (spec.type) {
        case fmt_Type_hex_lower:
        case fmt_Type_hex_upper:
            // Always use lowercase 0x for hex
            *atA(buf, pos--) = u8_c('x');
            *atA(buf, pos--) = u8_c('0');
            break;
        case fmt_Type_binary:
            *atA(buf, pos--) = u8_c('b');
            *atA(buf, pos--) = u8_c('0');
            break;
        case fmt_Type_octal:
            *atA(buf, pos--) = u8_c('0');
            break;
        default:
            break;
        }
    }

    // pos now points one position BEFORE the first written character, so add 1
    S_const$u8 content = { .ptr = ptr$A(buf) + pos + 1, .len = lenA(buf) - pos - 1 };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

fn_((formatIInt(io_Writer writer, i64 val, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatIInt ---\n");
    A$$(128, u8) buf = zero$A();
    usize pos = lenA(buf) - 1;
    // Apply size extension
    switch (spec.size) {
    case fmt_Size_8:
        val = (i8)val;
        break;
    case fmt_Size_16:
        val = (i16)val;
        break;
    case fmt_Size_32:
        val = (i32)val;
        break;
    case fmt_Size_64:
        $fallthrough;
    case fmt_Size_128:
        // Fallback to 64-bit until 128-bit is fully supported
        $fallthrough;
    case fmt_Size_ptr:
        // No extension needed for 64-bit and pointer size
        break;
    default:
        claim_unreachable;
    }
    bool negative = val < 0;
    u64 abs_value = negative ? as$(u64)(-val) : as$(u64)(val);
    // Convert to string (reverse order)
    if (abs_value == 0) {
        *atA(buf, pos--) = u8_c('0');
    } else {
        while (abs_value > 0) {
            *atA(buf, pos--) = u8_c('0') + (abs_value % 10);
            abs_value /= 10;
        }
    }
    // Apply sign based on sign flag
    if (negative) {
        *atA(buf, pos--) = u8_c('-');
    } else {
        switch (spec.sign) {
        case fmt_Sign_always:
            *atA(buf, pos--) = u8_c('+');
            break;
        case fmt_Sign_space:
            *atA(buf, pos--) = u8_c(' ');
            break;
        case fmt_Sign_auto:
            break;
        }
    }
    // pos now points one position BEFORE the first written character, so add 1
    S_const$u8 content = { .ptr = ptr$A(buf) + pos + 1, .len = lenA(buf) - pos - 1 };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

fn_((formatFlt(io_Writer writer, f64 val, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatFlt ---\n");
    // Apply size
    switch (spec.size) {
    case fmt_Size_32:
        val = (f32)val;
        break;
    case fmt_Size_64:
        // f64 is the default, no conversion needed
        $fallthrough;
    case fmt_Size_128:
        // Fallback to 64-bit until 128-bit is fully supported
        $fallthrough;
    case fmt_Size_ptr:
        break;
    default:
        claim_unreachable;
    }
    // Handle special values with case based on type
    bool use_upper = (spec.type == fmt_Type_float_upper);
    if (flt_isNan(val)) {
        return writePadded(writer, use_upper ? u8_l("NAN") : u8_l("nan"), spec);
    }
    if (flt_isInf(val)) {
        return val < 0
                 ? writePadded(writer, use_upper ? u8_l("-INF") : u8_l("-inf"), spec)
                 : writePadded(writer, use_upper ? u8_l("INF") : u8_l("inf"), spec);
    }

    A$$(128, u8) buf = zero$A();
    usize pos = 0;
    // Handle sign
    bool negative = val < 0;
    if (negative) {
        *atA(buf, pos++) = u8_c('-');
        val = -val;
    } else {
        // Apply sign flag for positive numbers
        switch (spec.sign) {
        case fmt_Sign_always:
            *atA(buf, pos++) = u8_c('+');
            break;
        case fmt_Sign_space:
            *atA(buf, pos++) = u8_c(' ');
            break;
        case fmt_Sign_auto:
            break;
        }
    }
    // Get precision
    u8 precision = orelse_((spec.precision)(6));
    // Get integer and fractional parts
    f64 int_part_f = __builtin_floor(val);
    f64 frac_part_f = val - int_part_f;
    u64 int_part = (u64)int_part_f;
    // Convert integer part
    A$$(32, u8) int_buf = zero$A();
    usize int_pos = lenA(int_buf) - 1;
    if (int_part == 0) {
        *atA(int_buf, int_pos--) = u8_c('0');
    } else {
        u64 temp = int_part;
        while (temp > 0) {
            *atA(int_buf, int_pos--) = u8_c('0') + (temp % 10);
            temp /= 10;
        }
    }
    // Copy integer part to buffer (int_pos points one before first digit, so start at int_pos + 1)
    for (usize i = int_pos + 1; i < lenA(int_buf); ++i) {
        *atA(buf, pos++) = *atA(int_buf, i);
    }
    // Add fractional part if precision > 0
    if (precision > 0) {
        *atA(buf, pos++) = u8_c('.');
        // Convert fractional part
        frac_part_f *= __builtin_pow(10.0, (f64)precision);
        u64 frac_part_u = (u64)(frac_part_f + 0.5); // Rounding
        // Write fractional digits in reverse order
        for (usize i = precision; i > 0; i--) {
            *atA(buf, pos + (i - 1)) = u8_c('0') + (frac_part_u % 10);
            frac_part_u /= 10;
        }
        pos += precision;
    }
    S_const$u8 content = { .ptr = ptr$A(buf), .len = pos };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

fn_((formatPtr(io_Writer writer, const void* ptr, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatPtr ---\n");
    A$$(32, u8) buf = zero$A();
    usize pos = 0;
    // Select digit set based on case
    S_const$u8 digits = (spec.type == fmt_Type_pointer_upper)
                          ? u8_l("0123456789ABCDEF")
                          : u8_l("0123456789abcdef");
    // Convert pointer to usize and format as hex
    usize ptr_val = (usize)ptr;
    bool is_zero = (ptr_val == 0);

    if (is_zero) {
        *atA(buf, pos++) = u8_c('0');
    } else {
        // Find number of hex digits needed
        usize temp = ptr_val;
        usize digit_count = 0;
        while (temp > 0) {
            digit_count++;
            temp >>= 4;
        }
        // Write hex digits in reverse to temp buffer
        A$$(16, u8) temp_buf = zero$A();
        for (usize i = digit_count; i > 0; i--) {
            *atA(temp_buf, i - 1) = *atS(digits, ptr_val & 0xF);
            ptr_val >>= 4;
        }
        // Copy to main buffer
        for (usize i = 0; i < digit_count; i++) {
            *atA(buf, pos++) = *atA(temp_buf, i);
        }
    }

    // Add "0x" prefix if alternate form is requested
    if (spec.alt_form) {
        // Shift content to make room for prefix
        for (usize i = pos; i > 0; i--) {
            *atA(buf, i + 1) = *atA(buf, i - 1);
        }
        *atA(buf, 0) = u8_c('0');
        *atA(buf, 1) = u8_c('x');
        pos += 2;
    }

    S_const$u8 content = { .ptr = ptr$A(buf), .len = pos };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

fn_((formatErr(io_Writer writer, Err err, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatErr ---\n");
    A$$(256, u8) buf = zero$A();
    usize pos = 0;
    // Format: "[Domain] Code"
    *atA(buf, pos++) = u8_c('[');
    // Get domain string (already returns S_const$u8)
    let domain = Err_domainToStr(err);
    for_(($s(domain))(ch) {
        if (pos >= lenA(buf) - 1) { break; }
        *atA(buf, pos++) = *ch;
    });
    *atA(buf, pos++) = u8_c(']');
    *atA(buf, pos++) = u8_c(' ');
    // Get code string (already returns S_const$u8)
    let code = Err_codeToStr(err);
    for_(($s(code))(ch) {
        if (pos >= lenA(buf) - 1) { break; }
        *atA(buf, pos++) = *ch;
    });
    S_const$u8 content = { .ptr = ptr$A(buf), .len = pos };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

fn_((formatASCIICode(io_Writer writer, u8 value, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatASCIICode ---\n");
    A$$(1, u8) buf = zero$A();
    *atA(buf, 0) = value;
    S_const$u8 content = { .ptr = ptr$A(buf), .len = 1 };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

fn_((formatUTF8Codepoint(io_Writer writer, u32 codepoint, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatUTF8Codepoint ---\n");
    A$$(4, u8) buf = zero$A();
    usize len = 0;
    if (codepoint <= 0x7F) {
        *atA(buf, 0) = (u8)codepoint;
        len = 1;
    } else if (codepoint <= 0x7FF) {
        *atA(buf, 0) = 0xC0 | (u8)(codepoint >> 6);
        *atA(buf, 1) = 0x80 | (u8)(codepoint & 0x3F);
        len = 2;
    } else if (codepoint <= 0xFFFF) {
        *atA(buf, 0) = 0xE0 | (u8)(codepoint >> 12);
        *atA(buf, 1) = 0x80 | (u8)((codepoint >> 6) & 0x3F);
        *atA(buf, 2) = 0x80 | (u8)(codepoint & 0x3F);
        len = 3;
    } else if (codepoint <= 0x10FFFF) {
        *atA(buf, 0) = 0xF0 | (u8)(codepoint >> 18);
        *atA(buf, 1) = 0x80 | (u8)((codepoint >> 12) & 0x3F);
        *atA(buf, 2) = 0x80 | (u8)((codepoint >> 6) & 0x3F);
        *atA(buf, 3) = 0x80 | (u8)(codepoint & 0x3F);
        len = 4;
    } else {
        return_err(fmt_Err_InvalidFormatSpecifier());
    }
    S_const$u8 content = { .ptr = ptr$A(buf), .len = len };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

fn_((formatStr(io_Writer writer, S_const$u8 str, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: formatStr ---\n");
    return writePadded(writer, str, spec);
} $unscoped_(fn);

fn_((writePadded(io_Writer writer, S_const$u8 content, fmt_Spec spec))(E$void) $scope) {
    // printf("--- debug print: writePadded ---\n");
    if_none((spec.width)) {
        return io_Writer_writeBytes(writer, content);
    } else_some((width)) {
        if (content.len >= width) {
            return io_Writer_writeBytes(writer, content);
        }
        usize padding = width - content.len;
        // Use explicit fill or default to space
        u8 fill = orelse_((spec.fill)(u8_c(' ')));
        // Default to left alignment
        fmt_Align align = orelse_((spec.align)(fmt_Align_left));
        switch (align) {
        case fmt_Align_left:
            try_(io_Writer_writeBytes(writer, content));
            try_(io_Writer_writeByteN(writer, fill, padding));
            break;
        case fmt_Align_right:
            try_(io_Writer_writeByteN(writer, fill, padding));
            try_(io_Writer_writeBytes(writer, content));
            break;
        case fmt_Align_center: {
            usize left_pad = padding / 2;
            usize right_pad = padding - left_pad;
            try_(io_Writer_writeByteN(writer, fill, left_pad));
            try_(io_Writer_writeBytes(writer, content));
            try_(io_Writer_writeByteN(writer, fill, right_pad));
            break;
        }
        default:
            claim_unreachable;
        }
    }
    return_ok({});
} $unscoped_(fn);

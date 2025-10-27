#include "dh/fmt/common.h"
#include "dh/mem/common.h"
#include "dh/Str.h"
#include "dh/ascii.h"
#include "dh/int.h"
#include "dh/flt.h"

/*========== Internal Types =================================================*/

/// Alignment type for formatting
typedef enum fmt_Align : u8 {
    fmt_Align_left = 0,
    fmt_Align_center = 1,
    fmt_Align_right = 2,
} fmt_Align;
$maybe_unused static const A$$(3, u8) fmt_Align_values = init$A({
    [fmt_Align_left] = u8_c('<'),
    [fmt_Align_center] = u8_c('^'),
    [fmt_Align_right] = u8_c('>'),
});

/// Size modifier for integer and float types
typedef enum fmt_Size : u8 {
    fmt_Size_8 = 0,  // hh
    fmt_Size_16 = 1, // h
    fmt_Size_32 = 2, // (default)
    fmt_Size_64 = 3, // l
    fmt_Size_ptr = 4 // z
} fmt_Size;

/// Format specifier parsed from format string
typedef struct {
    usize index;        // Argument index (0-15)
    bool has_index;     // Whether index was explicitly specified
    u8 fill;            // Fill character (default: space)
    fmt_Align align;    // Alignment
    usize width;        // Minimum field width
    bool has_width;     // Whether width was specified
    usize precision;    // Precision for floats
    bool has_precision; // Whether precision was specified
    fmt_Size size;      // Size modifier
    u8 type;            // Type character
} fmt_Spec;

/// Argument storage (24 bytes each)
typedef union {
    u64 as_u64;
    i64 as_i64;
    f64 as_f64;
    const void* as_ptr;
    struct {
        const u8* ptr;
        usize len;
    } as_slice;
    struct {
        u8 tag; // 0=none/err, 1=some/ok
        u8 padding[7];
        union {
            u64 as_u64;
            i64 as_i64;
            f64 as_f64;
            const void* as_ptr;
        } value;
    } as_opt_err;
} fmt_Arg;

/*========== Internal Helpers ===============================================*/

static inline fn_((digitToInt(u8 c))(u8)) {
    debug_assert(ascii_isDigit(c));
    return c - u8_c('0');
}

static inline fn_((strLenZ(const u8* str))(usize)) {
    debug_assert(str != NULL);
    usize len = 0;
    while (str[len] != '\0') { len++; }
    return len;
}

static fn_((skipWhitespace(S_const$u8* str))(void)) {
    while (str->len > 0 && ascii_isWhitespace(str->ptr[0])) {
        str->ptr++;
        str->len--;
    }
}

/*========== Format Specifier Parser ========================================*/

static fn_((parseFormatSpec(S_const$u8* fmt_str, fmt_Spec* spec))(E$void) $scope) {
    // Initialize with defaults
    *spec = (fmt_Spec){
        .index = 0,
        .has_index = false,
        .fill = u8_c(' '),
        .align = fmt_Align_left,
        .width = 0,
        .has_width = false,
        .precision = 6,
        .has_precision = false,
        .size = fmt_Size_32,
        .type = 0
    };

    if (fmt_str->len == 0) {
        return_err(fmt_Err_InvalidFormatSpecifier());
    }

    usize pos = 0;

    // Parse optional index
    if (ascii_isDigit(fmt_str->ptr[pos])) {
        usize next_pos = pos;
        while (next_pos < fmt_str->len && ascii_isDigit(fmt_str->ptr[next_pos])) {
            next_pos++;
        }
        if (next_pos < fmt_str->len && fmt_str->ptr[next_pos] == u8_c(':')) {
            // This is an index
            spec->index = 0;
            for (usize i = pos; i < next_pos; i++) {
                spec->index = usize_add(usize_mul(spec->index, 10), digitToInt(fmt_str->ptr[i]));
            }
            if (spec->index >= fmt_max_args) {
                return_err(fmt_Err_IndexOutOfBounds());
            }
            spec->has_index = true;
            pos = next_pos + 1; // Skip ':'
        }
    }

    // Parse optional fill and alignment
    if (pos + 1 < fmt_str->len) {
        u8 align_char = fmt_str->ptr[pos + 1];
        if (align_char == u8_c('<') || align_char == u8_c('>') || align_char == u8_c('^')) {
            spec->fill = fmt_str->ptr[pos];
            pos += 2;

            switch (align_char) {
            case '<':
                spec->align = fmt_Align_left;
                break;
            case '>':
                spec->align = fmt_Align_right;
                break;
            case '^':
                spec->align = fmt_Align_center;
                break;
            default:
                break;
            }
        } else if (align_char == u8_c('<') || align_char == u8_c('>') || align_char == u8_c('^')) {
            pos++;
            switch (align_char) {
            case '<':
                spec->align = fmt_Align_left;
                break;
            case '>':
                spec->align = fmt_Align_right;
                break;
            case '^':
                spec->align = fmt_Align_center;
                break;
            default:
                break;
            }
        }
    }

    // Parse optional width
    if (pos < fmt_str->len && ascii_isDigit(fmt_str->ptr[pos])) {
        spec->width = 0;
        while (pos < fmt_str->len && ascii_isDigit(fmt_str->ptr[pos])) {
            spec->width = usize_add(usize_mul(spec->width, 10), digitToInt(fmt_str->ptr[pos]));
            pos++;
        }
        spec->has_width = true;
    }

    // Parse optional precision
    if (pos < fmt_str->len && fmt_str->ptr[pos] == u8_c('.')) {
        pos++;
        if (pos >= fmt_str->len || !ascii_isDigit(fmt_str->ptr[pos])) {
            return_err(fmt_Err_InvalidPrecisionFormat());
        }
        spec->precision = 0;
        while (pos < fmt_str->len && ascii_isDigit(fmt_str->ptr[pos])) {
            spec->precision = usize_add(usize_mul(spec->precision, 10), digitToInt(fmt_str->ptr[pos]));
            pos++;
        }
        spec->has_precision = true;
    }

    // Parse optional size modifier
    if (pos < fmt_str->len) {
        if (fmt_str->ptr[pos] == u8_c('h')) {
            pos++;
            if (pos < fmt_str->len && fmt_str->ptr[pos] == u8_c('h')) {
                spec->size = fmt_Size_8;
                pos++;
            } else {
                spec->size = fmt_Size_16;
            }
        } else if (fmt_str->ptr[pos] == u8_c('l')) {
            spec->size = fmt_Size_64;
            pos++;
        } else if (fmt_str->ptr[pos] == u8_c('z')) {
            spec->size = fmt_Size_ptr;
            pos++;
        }
    }

    // Parse required type character
    if (pos >= fmt_str->len) {
        return_err(fmt_Err_InvalidFormatSpecifier());
    }
    spec->type = fmt_str->ptr[pos];
    pos++;

    // Update fmt_str to remaining part
    fmt_str->ptr += pos;
    fmt_str->len -= pos;

    return_ok({});
} $unscoped_(fn);

/*========== Argument Collection ============================================*/

static fn_((collectArgs(va_list va_args, usize arg_count, fmt_Arg args[fmt_max_args]))(void)) {
    for (usize i = 0; i < arg_count && i < fmt_max_args; i++) {
        // Arguments are collected as raw bytes
        // The format specifier will determine how to interpret them
        args[i].as_u64 = va_arg(va_args, u64);
    }
}

/*========== Formatting Functions ===========================================*/

static fn_((writeCharN(io_Writer writer, u8 ch, usize count))(E$void) $scope) {
    for (usize i = 0; i < count; i++) {
        try_(io_Writer_writeByte(writer, ch));
    }
    return_ok({});
} $unscoped_(fn);

static fn_((writePadded(io_Writer writer, S_const$u8 content, const fmt_Spec* spec))(E$void) $scope) {
    if (!spec->has_width || content.len >= spec->width) {
        return io_Writer_writeBytes(writer, content);
    }

    usize padding = spec->width - content.len;

    switch (spec->align) {
    case fmt_Align_left:
        try_(io_Writer_writeBytes(writer, content));
        try_(writeCharN(writer, spec->fill, padding));
        break;

    case fmt_Align_right:
        try_(writeCharN(writer, spec->fill, padding));
        try_(io_Writer_writeBytes(writer, content));
        break;

    case fmt_Align_center: {
        usize left_pad = padding / 2;
        usize right_pad = padding - left_pad;
        try_(writeCharN(writer, spec->fill, left_pad));
        try_(io_Writer_writeBytes(writer, content));
        try_(writeCharN(writer, spec->fill, right_pad));
        break;
    }
    }

    return_ok({});
} $unscoped_(fn);

static fn_((formatUnsigned(io_Writer writer, u64 value, const fmt_Spec* spec))(E$void) $scope) {
    u8 buffer[128];
    usize pos = sizeof(buffer);

    // Determine base
    u32 base = 10;
    const char* digits = "0123456789abcdef";

    switch (spec->type) {
    case 'x':
        base = 16;
        break;
    case 'X':
        base = 16;
        digits = "0123456789ABCDEF";
        break;
    case 'o':
        base = 8;
        break;
    case 'b':
        base = 2;
        break;
    case 'u':
        base = 10;
        break;
    default:
        return_err(fmt_Err_InvalidIntegerFormat());
    }

    // Apply size mask
    switch (spec->size) {
    case fmt_Size_8:
        value &= 0xFF;
        break;
    case fmt_Size_16:
        value &= 0xFFFF;
        break;
    case fmt_Size_32:
        value &= 0xFFFFFFFF;
        break;
    default:
        break;
    }

    // Convert to string (reverse order)
    if (value == 0) {
        buffer[--pos] = u8_c('0');
    } else {
        while (value > 0) {
            buffer[--pos] = digits[value % base];
            value /= base;
        }
    }

    S_const$u8 content = { .ptr = buffer + pos, .len = sizeof(buffer) - pos };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

static fn_((formatSigned(
    io_Writer writer,
    i64 value,
    const fmt_Spec* spec
))(E$void) $scope) {
    u8 buffer[128];
    usize pos = sizeof(buffer);

    // Apply size extension
    switch (spec->size) {
    case fmt_Size_8:
        value = (i8)value;
        break;
    case fmt_Size_16:
        value = (i16)value;
        break;
    case fmt_Size_32:
        value = (i32)value;
        break;
    default:
        break;
    }

    bool negative = value < 0;
    u64 abs_value = negative ? (u64)(-value) : (u64)value;

    // Convert to string (reverse order)
    if (abs_value == 0) {
        buffer[--pos] = u8_c('0');
    } else {
        while (abs_value > 0) {
            buffer[--pos] = u8_c('0') + (abs_value % 10);
            abs_value /= 10;
        }
    }

    if (negative) {
        buffer[--pos] = u8_c('-');
    }

    S_const$u8 content = { .ptr = buffer + pos, .len = sizeof(buffer) - pos };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

static fn_((formatFloat(
    io_Writer writer,
    f64 value,
    const fmt_Spec* spec
))(E$void) $scope) {
    // Apply size
    if (spec->size == fmt_Size_32) {
        value = (f32)value;
    }

    // Handle special values
    if (flt_isNan(value)) {
        return writePadded(writer, u8_l("nan"), spec);
    }
    if (flt_isInf(value)) {
        return writePadded(writer, value < 0 ? u8_l("-inf") : u8_l("inf"), spec);
    }

    u8 buffer[128];
    usize pos = 0;

    // Handle sign
    bool negative = value < 0;
    if (negative) {
        buffer[pos++] = u8_c('-');
        value = -value;
    }

    // Get precision
    usize precision = spec->has_precision ? spec->precision : 6;

    // Get integer and fractional parts
    f64 int_part_f = __builtin_floor(value);
    f64 frac_part_f = value - int_part_f;

    u64 int_part = (u64)int_part_f;

    // Convert integer part
    u8 int_buffer[32];
    usize int_pos = sizeof(int_buffer);

    if (int_part == 0) {
        int_buffer[--int_pos] = u8_c('0');
    } else {
        u64 temp = int_part;
        while (temp > 0) {
            int_buffer[--int_pos] = u8_c('0') + (temp % 10);
            temp /= 10;
        }
    }

    // Copy integer part to buffer
    for (usize i = int_pos; i < sizeof(int_buffer); i++) {
        buffer[pos++] = int_buffer[i];
    }

    // Add fractional part if precision > 0
    if (precision > 0) {
        buffer[pos++] = u8_c('.');

        // Convert fractional part
        frac_part_f *= __builtin_pow(10.0, (f64)precision);
        u64 frac_part_u = (u64)(frac_part_f + 0.5); // Rounding

        // Write fractional digits in reverse order
        for (usize i = precision; i > 0; i--) {
            buffer[pos + i - 1] = u8_c('0') + (frac_part_u % 10);
            frac_part_u /= 10;
        }
        pos += precision;
    }

    S_const$u8 content = { .ptr = buffer, .len = pos };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

static fn_((formatBool(
    io_Writer writer,
    bool value,
    const fmt_Spec* spec
))(E$void) $scope) {
    const char* str = value ? "true" : "false";
    S_const$u8 content = { .ptr = (const u8*)str, .len = value ? 4 : 5 };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

static fn_((formatPointer(
    io_Writer writer,
    const void* ptr,
    const fmt_Spec* spec
))(E$void) $scope) {
    u8 buffer[32];
    usize pos = sizeof(buffer);

    // Convert pointer to usize and format as hex
    usize ptr_val = (usize)ptr;

    // Add "0x" prefix
    buffer[--pos] = u8_c('x');
    buffer[--pos] = u8_c('0');

    // Convert to hex (reverse order)
    if (ptr_val == 0) {
        buffer[--pos] = u8_c('0');
    } else {
        const u8* digits = (const u8*)"0123456789abcdef";
        while (ptr_val > 0) {
            buffer[--pos] = digits[ptr_val & 0xF];
            ptr_val >>= 4;
        }
    }

    S_const$u8 content = { .ptr = buffer + pos, .len = sizeof(buffer) - pos };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

static fn_((formatChar(
    io_Writer writer,
    u8 value,
    const fmt_Spec* spec
))(E$void) $scope) {
    u8 buffer[1] = { value };
    S_const$u8 content = { .ptr = buffer, .len = 1 };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

static fn_((formatUTF8(
    io_Writer writer,
    u32 codepoint,
    const fmt_Spec* spec
))(E$void) $scope) {
    u8 buffer[4];
    usize len = 0;

    if (codepoint <= 0x7F) {
        buffer[0] = (u8)codepoint;
        len = 1;
    } else if (codepoint <= 0x7FF) {
        buffer[0] = 0xC0 | (u8)(codepoint >> 6);
        buffer[1] = 0x80 | (u8)(codepoint & 0x3F);
        len = 2;
    } else if (codepoint <= 0xFFFF) {
        buffer[0] = 0xE0 | (u8)(codepoint >> 12);
        buffer[1] = 0x80 | (u8)((codepoint >> 6) & 0x3F);
        buffer[2] = 0x80 | (u8)(codepoint & 0x3F);
        len = 3;
    } else if (codepoint <= 0x10FFFF) {
        buffer[0] = 0xF0 | (u8)(codepoint >> 18);
        buffer[1] = 0x80 | (u8)((codepoint >> 12) & 0x3F);
        buffer[2] = 0x80 | (u8)((codepoint >> 6) & 0x3F);
        buffer[3] = 0x80 | (u8)(codepoint & 0x3F);
        len = 4;
    } else {
        return_err(fmt_Err_InvalidFormatSpecifier());
    }

    S_const$u8 content = { .ptr = buffer, .len = len };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

static fn_((formatString(io_Writer writer, S_const$u8 str, const fmt_Spec* spec))(E$void) $scope) {
    return writePadded(writer, str, spec);
} $unscoped_(fn);

static fn_((formatError(io_Writer writer, const void* error_ptr, const fmt_Spec* spec))(E$void) $scope) {
    // Error format: "[Error] 0x..."
    // This is a simplified implementation
    // Real implementation would extract domain and code from error struct
    u8 buffer[64];
    usize pos = 0;

    // Add "[Error] 0x" prefix
    const u8* prefix = (const u8*)"[Error] 0x";
    const usize prefix_len = 11;
    for (usize i = 0; i < prefix_len; i++) {
        buffer[pos++] = prefix[i];
    }

    // Convert pointer to hex
    usize ptr_val = (usize)error_ptr;
    if (ptr_val == 0) {
        buffer[pos++] = u8_c('0');
    } else {
        // Find number of hex digits needed
        usize temp = ptr_val;
        usize digit_count = 0;
        while (temp > 0) {
            digit_count++;
            temp >>= 4;
        }

        // Write hex digits
        const u8* digits = (const u8*)"0123456789abcdef";
        for (usize i = digit_count; i > 0; i--) {
            buffer[pos + i - 1] = digits[ptr_val & 0xF];
            ptr_val >>= 4;
        }
        pos += digit_count;
    }

    S_const$u8 content = { .ptr = buffer, .len = pos };
    return writePadded(writer, content, spec);
} $unscoped_(fn);

static fn_((formatOptional(
    io_Writer writer,
    const fmt_Arg* arg,
    const fmt_Spec* spec
))(E$void) $scope) {
    bool has_value = arg->as_opt_err.tag != 0;

    if (spec->type == '?') {
        // Tag only: "some" or "none"
        const char* str = has_value ? "some" : "none";
        S_const$u8 content = { .ptr = (const u8*)str, .len = 4 };
        return writePadded(writer, content, spec);
    }
    if (spec->type == '0') {
        // O$void: "some" or "none"
        const char* str = has_value ? "some" : "none";
        S_const$u8 content = { .ptr = (const u8*)str, .len = 4 };
        return writePadded(writer, content, spec);
    }
    // Full: format value or "none"
    if (!has_value) {
        S_const$u8 none = { .ptr = (const u8*)"none", .len = 4 };
        return writePadded(writer, none, spec);
    }

    // Create new spec without '?' prefix
    fmt_Spec value_spec = *spec;
    // Format the inner value based on type
    switch (spec->type) {
    case 'd':
    case 'i':
        return formatSigned(writer, arg->as_opt_err.value.as_i64, &value_spec);
    case 'u':
    case 'x':
    case 'X':
    case 'o':
    case 'b':
        return formatUnsigned(writer, arg->as_opt_err.value.as_u64, &value_spec);
    case 'f':
    case 'F':
        return formatFloat(writer, arg->as_opt_err.value.as_f64, &value_spec);
    default:
        return_err(fmt_Err_InvalidFormatSpecifier());
    }
} $unscoped_(fn);

static fn_((formatResult(
    io_Writer writer,
    const fmt_Arg* arg,
    const fmt_Spec* spec
))(E$void) $scope) {
    bool is_ok = arg->as_opt_err.tag != 0;

    if (spec->type == '!') {
        // Tag only: "ok" or "err"
        const char* str = is_ok ? "ok" : "err";
        S_const$u8 content = { .ptr = (const u8*)str, .len = 2 };
        return writePadded(writer, content, spec);
    }
    if (spec->type == '0') {
        // E$void: "ok" or "[Domain] Code"
        if (is_ok) {
            S_const$u8 ok = { .ptr = (const u8*)"ok", .len = 2 };
            return writePadded(writer, ok, spec);
        }
        return formatError(writer, arg->as_opt_err.value.as_ptr, spec);
    }
    // Full: format value or error
    if (!is_ok) {
        return formatError(writer, arg->as_opt_err.value.as_ptr, spec);
    }

    // Format the inner value
    fmt_Spec value_spec = *spec;
    switch (spec->type) {
    case 'd':
    case 'i':
        return formatSigned(writer, arg->as_opt_err.value.as_i64, &value_spec);
    case 'u':
    case 'x':
    case 'X':
    case 'o':
    case 'b':
        return formatUnsigned(writer, arg->as_opt_err.value.as_u64, &value_spec);
    case 'f':
    case 'F':
        return formatFloat(writer, arg->as_opt_err.value.as_f64, &value_spec);
    case 's': {
        S_const$u8 str = {
            .ptr = (const u8*)arg->as_opt_err.value.as_ptr,
            .len = arg->as_opt_err.value.as_u64 >> 32
        };
        return formatString(writer, str, &value_spec);
    }
    default:
        return_err(fmt_Err_InvalidFormatSpecifier());
    }
} $unscoped_(fn);

static fn_((formatArg(io_Writer writer, const fmt_Arg* arg, const fmt_Spec* spec))(E$void) $scope) {
    // Handle Optional types
    if (spec->type == '?' || (spec->type >= '0' && spec->type <= '9')) {
        return formatOptional(writer, arg, spec);
    }

    // Handle Result types
    if (spec->type == '!') {
        return formatResult(writer, arg, spec);
    }

    // Handle regular types
    switch (spec->type) {
    case 'd':
    case 'i':
        return formatSigned(writer, arg->as_i64, spec);

    case 'u':
    case 'x':
    case 'X':
    case 'o':
    case 'b':
        return formatUnsigned(writer, arg->as_u64, spec);

    case 'f':
    case 'F':
        return formatFloat(writer, arg->as_f64, spec);

    case 'B':
        return formatBool(writer, arg->as_u64 != 0, spec);

    case 'p':
        return formatPointer(writer, arg->as_ptr, spec);

    case 'e':
        return formatError(writer, arg->as_ptr, spec);

    case 'c':
        return formatChar(writer, (u8)arg->as_u64, spec);

    case 'C':
        return formatUTF8(writer, (u32)arg->as_u64, spec);

    case 'z': {
        const u8* str = (const u8*)arg->as_ptr;
        usize len = strLenZ(str);
        S_const$u8 slice = { .ptr = str, .len = len };
        return formatString(writer, slice, spec);
    }

    case 's': {
        S_const$u8 slice = { .ptr = arg->as_slice.ptr, .len = arg->as_slice.len };
        return formatString(writer, slice, spec);
    }

    default:
        return_err(fmt_Err_InvalidFormatSpecifier());
    }
} $unscoped_(fn);

/*========== Main Format Functions ==========================================*/

fn_((fmt_formatVaArgs(io_Writer writer, S_const$u8 fmt, va_list va_args))(E$void) $scope) {
    // Collect all arguments first
    fmt_Arg args[fmt_max_args] = {};

    // First pass: count format specifiers to determine arg count
    usize arg_count = 0;
    usize max_index = 0;
    bool has_indexed = false;

    {
        S_const$u8 temp_fmt = fmt;
        while (temp_fmt.len > 0) {
            if (temp_fmt.ptr[0] == u8_c('{')) {
                if (temp_fmt.len > 1 && temp_fmt.ptr[1] == u8_c('{')) {
                    temp_fmt.ptr += 2;
                    temp_fmt.len -= 2;
                    continue;
                }

                temp_fmt.ptr++;
                temp_fmt.len--;

                usize close_pos = 0;
                while (close_pos < temp_fmt.len && temp_fmt.ptr[close_pos] != u8_c('}')) {
                    close_pos++;
                }

                if (close_pos == temp_fmt.len) {
                    return_err(fmt_Err_MissingClosingBrace());
                }

                S_const$u8 spec_str = { .ptr = temp_fmt.ptr, .len = close_pos };
                fmt_Spec spec;
                try_(parseFormatSpec(&spec_str, &spec));

                if (spec.has_index) {
                    has_indexed = true;
                    if (spec.index > max_index) {
                        max_index = spec.index;
                    }
                } else {
                    arg_count++;
                }

                temp_fmt.ptr += close_pos + 1;
                temp_fmt.len -= close_pos + 1;
            } else {
                temp_fmt.ptr++;
                temp_fmt.len--;
            }
        }
    }

    // Determine total args needed
    usize total_args = has_indexed ? (max_index + 1) : arg_count;
    if (total_args > fmt_max_args) {
        return_err(fmt_Err_TooManyArguments());
    }

    // Collect arguments

    with_fini_(va_list va_copy = {}, va_end(va_copy)) {
        va_copy(va_copy, va_args);
        collectArgs(va_copy, total_args, args);
    }

    // Second pass: format output
    usize positional_index = 0;

    while (fmt.len > 0) {
        // Handle literal characters
        if (fmt.ptr[0] != u8_c('{')) {
            try_(io_Writer_writeByte(writer, fmt.ptr[0]));
            fmt.ptr++;
            fmt.len--;
            continue;
        }

        // Handle escaped braces
        if (fmt.len > 1 && fmt.ptr[1] == u8_c('{')) {
            try_(io_Writer_writeByte(writer, u8_c('{')));
            fmt.ptr += 2;
            fmt.len -= 2;
            continue;
        }

        // Parse format specifier
        fmt.ptr++; // Skip '{'
        fmt.len--;

        usize close_pos = 0;
        while (close_pos < fmt.len && fmt.ptr[close_pos] != u8_c('}')) {
            close_pos++;
        }

        if (close_pos == fmt.len) {
            return_err(fmt_Err_MissingClosingBrace());
        }

        S_const$u8 spec_str = { .ptr = fmt.ptr, .len = close_pos };
        fmt_Spec spec;
        try_(parseFormatSpec(&spec_str, &spec));

        // Determine which argument to use
        usize arg_index = spec.has_index ? spec.index : positional_index++;

        if (arg_index >= total_args) {
            return_err(fmt_Err_TooFewArguments());
        }

        // Format the argument
        try_(formatArg(writer, &args[arg_index], &spec));

        fmt.ptr += close_pos + 1; // Skip past '}'
        fmt.len -= close_pos + 1;
    }

    return_ok({});
} $unscoped_(fn);

fn_((fmt_format(io_Writer writer, S_const$u8 fmt, ...))(E$void) $guard) {
    va_list va_args = {};
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(fmt_formatVaArgs(writer, fmt, va_args)));
} $unguarded_(fn);

/*========== Parsing Functions ==============================================*/

fn_((fmt_parseBool(S_const$u8 str))(E$bool) $scope) {
    skipWhitespace(&str);
    if (Str_eql(str, mem_asBytes_const(&u8_c('1'))) || Str_eql(str, u8_l("true"))) { return_ok(true); }
    if (Str_eql(str, mem_asBytes_const(&u8_c('0'))) || Str_eql(str, u8_l("false"))) { return_ok(false); }
    return_err(fmt_Err_InvalidBoolFormat());
} $unscoped_(fn);

fn_((fmt_parse$bool(S_const$u8 str))(E$bool)) { return fmt_parseBool(str); }

fn_((fmt_parseUInt(S_const$u8 str, u8 base))(E$u64) $scope) {
    skipWhitespace(&str);

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
    return_ok(as$((usize)(try_(fmt_parseUInt(str, base)))));
} $unscoped_(fn);

fn_((fmt_parse$u64(S_const$u8 str, u8 base))(E$u64)) { return fmt_parseUInt(str, base); }

fn_((fmt_parse$u32(S_const$u8 str, u8 base))(E$u32) $scope) {
    let result = try_(fmt_parseUInt(str, base));
    if (result > u32_limit_max) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$((u32)(result)));
} $unscoped_(fn);

fn_((fmt_parse$u16(S_const$u8 str, u8 base))(E$u16) $scope) {
    let result = try_(fmt_parseUInt(str, base));
    if (result > u16_limit_max) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$((u16)(result)));
} $unscoped_(fn);

fn_((fmt_parse$u8(S_const$u8 str, u8 base))(E$u8) $scope) {
    let result = try_(fmt_parseUInt(str, base));
    if (result > u8_limit_max) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$((u8)(result)));
} $unscoped_(fn);

fn_((fmt_parseInt(S_const$u8 str, u8 base))(E$i64) $scope) {
    skipWhitespace(&str);

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
        const u64 max_neg = as$((u64)(i64_limit_max)) + 1;
        if (unsigned_result > max_neg) {
            return_err(fmt_Err_InvalidIntegerFormat());
        }
        return_ok(-as$((i64)(unsigned_result)));
    } else {
        if (unsigned_result > as$((u64)(i64_limit_max))) {
            return_err(fmt_Err_InvalidIntegerFormat());
        }
        return_ok(as$((i64)(unsigned_result)));
    }
} $unscoped_(fn);

fn_((fmt_parse$isize(S_const$u8 str, u8 base))(E$isize) $scope) {
    let result = try_(fmt_parseInt(str, base));
    return_ok(as$((isize)(result)));
} $unscoped_(fn);

fn_((fmt_parse$i64(S_const$u8 str, u8 base))(E$i64)) {
    return fmt_parseInt(str, base);
}

fn_((fmt_parse$i32(S_const$u8 str, u8 base))(E$i32) $scope) {
    let result = try_(fmt_parseInt(str, base));
    if (result < i32_limit_min || i32_limit_max < result) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$((i32)(result)));
} $unscoped_(fn);

fn_((fmt_parse$i16(S_const$u8 str, u8 base))(E$i16) $scope) {
    let result = try_(fmt_parseInt(str, base));
    if (result < i16_limit_min || i16_limit_max < result) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$((i16)(result)));
} $unscoped_(fn);

fn_((fmt_parse$i8(S_const$u8 str, u8 base))(E$i8) $scope) {
    let result = try_(fmt_parseInt(str, base));
    if (result < i8_limit_min || i8_limit_max < result) {
        return_err(fmt_Err_InvalidIntegerFormat());
    }
    return_ok(as$((i8)(result)));
} $unscoped_(fn);

fn_((fmt_parseFlt(S_const$u8 str))(E$f64) $scope) {
    skipWhitespace(&str);

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
    return_ok(as$((f32)(result)));
} $unscoped_(fn);

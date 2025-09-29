#include "dh/fmt/common.h"
#include "dh/Arr.h"
#include "dh/Str.h"
#include "dh/mem/common.h"
#include "dh/math/common.h"
#include "dh/core/prim/int.h"

typedef enum fmt_AlignType {
    fmt_AlignType_left,
    fmt_AlignType_right,
    fmt_AlignType_center
} fmt_AlignType;

typedef struct fmt_FormatSpec {
    usize arg_index;
    bool  has_explicit_index;

    // Alignment and fill
    fmt_AlignType align;
    Opt$u32       fill_char;

    // Flags
    bool alternate_form;   // #
    bool uppercase;        // !
    bool zero_pad;         // 0
    bool show_sign;        // +
    bool space_sign;       // ' '
    bool right_align_flag; // -

    // Width and precision
    Opt$usize width;
    Opt$usize precision;

    // Type specifier
    u8   type_char;
    bool is_64bit_type; // ll prefix
    bool is_size_type;  // z prefix
    bool has_type;
} fmt_FormatSpec;

// Argument storage for indexed access
typedef enum fmt_ArgType {
    fmt_ArgType_i32,
    fmt_ArgType_i64,
    fmt_ArgType_isize,
    fmt_ArgType_u32,
    fmt_ArgType_u64,
    fmt_ArgType_usize,
    fmt_ArgType_f64,
    fmt_ArgType_char,
    fmt_ArgType_str_slice,
    fmt_ArgType_str_ptr,
    fmt_ArgType_ptr,
    fmt_ArgType_unknown
} fmt_ArgType;

typedef struct fmt_StoredArg {
    fmt_ArgType type;
    union {
        i32          i32_val;
        i64          i64_val;
        isize        isize_val;
        u32          u32_val;
        u64          u64_val;
        usize        usize_val;
        f64          f64_val;
        u8           char_val;
        Sli_const$u8 str_slice_val;
        const u8*    str_ptr_val;
        const anyptr ptr_val;
    } value;
} fmt_StoredArg;

static fn_(fmt_writeChar(io_Writer writer, u8 ch), Err$void) {
    return io_Writer_writeByte(writer, ch);
}

static fn_(fmt_writeStr(io_Writer writer, Sli_const$u8 str), Err$void) {
    return io_Writer_writeAll(writer, str);
}

static fn_(fmt_writeInt(io_Writer writer, u64 value, fmt_FormatSpec spec), Err$void $scope) {
    use_Arr$(64, u8);
    Arr$64$u8 buffer = Arr_zero();
    usize     pos    = Arr_len(buffer);

    // Handle signed vs unsigned interpretation
    bool is_signed_type = spec.has_type && spec.type_char == 'd';
    bool is_negative    = false;
    u64  abs_value      = value;

    if (is_signed_type) {
        // Treat as signed integer
        i64 signed_value = as$(i64, value);
        is_negative      = signed_value < 0;
        abs_value        = is_negative ? as$(u64, -signed_value) : as$(u64, signed_value);
    }

    let original_abs_value = abs_value;

    // Choose base
    u32        base         = 10;
    static let digits_lower = u8_l("0123456789abcdef");
    static let digits_upper = u8_l("0123456789ABCDEF");
    var        digits       = spec.uppercase ? digits_upper : digits_lower;
    if (spec.has_type) {
        switch (spec.type_char) {
        case 'b':
            base = 2;
            break;
        case 'o':
            base = 8;
            break;
        case 'x':
            base = 16;
            break;
        case 'X':
            base   = 16;
            digits = digits_upper;
            break;
        default:
            break;
        }
    }

    // Convert digits only (no sign or prefix yet)
    if (abs_value == 0) {
        Arr_setAt(buffer, --pos, '0');
    } else {
        while (abs_value > 0) {
            Arr_setAt(buffer, --pos, Sli_getAt(digits, abs_value % base));
            abs_value /= base;
        }
    }

    // Get just the digits
    let buffer_slice = Sli_arr$(Sli_const$u8, buffer);
    let digits_slice = Sli_suffix(buffer_slice, pos);

    // Determine components
    Sli_const$u8 sign_str = u8_l("");
    if (is_signed_type || !spec.has_type) {
        if (is_negative) {
            sign_str = u8_l("-");
        } else if (spec.show_sign) {
            sign_str = u8_l("+");
        } else if (spec.space_sign) {
            sign_str = u8_l(" ");
        }
    }

    Sli_const$u8 prefix_str = u8_l("");
    if (spec.alternate_form && original_abs_value != 0) {
        switch (spec.type_char) {
        case 'b':
            prefix_str = u8_l("0b");
            break;
        case 'o':
            prefix_str = u8_l("0");
            break;
        case 'x':
            prefix_str = u8_l("0x");
            break;
        case 'X':
            prefix_str = u8_l("0X");
            break;
        default:
            break;
        }
    }

    // Calculate precision padding (minimum number of digits)
    usize precision_pad = 0;
    if_some(spec.precision, p) {
        if (p > digits_slice.len) {
            precision_pad = p - digits_slice.len;
        }
    }

    // Calculate total content length
    usize content_len = sign_str.len + prefix_str.len + precision_pad + digits_slice.len;

    // Calculate width padding
    usize width_pad = 0;
    if_some(spec.width, w) {
        if (w > content_len) {
            width_pad = w - content_len;
        }
    }

    // Determine padding character and alignment
    u8 pad_char = ' ';
    if (spec.zero_pad && !spec.right_align_flag && isNone(spec.precision)) {
        pad_char = '0';
    }

    fmt_AlignType align = spec.align;
    if (spec.right_align_flag) {
        align = fmt_AlignType_left;
    }

    // Build final output
    use_Arr$(128, u8);
    Arr$128$u8 final_buffer = Arr_zero();
    Sli$u8     out_slice    = Sli_arr$(Sli$u8, final_buffer);
    usize      out_pos      = 0;

    // Right align with spaces (if not zero-padding)
    if (align == fmt_AlignType_right && pad_char == ' ') {
        for (usize i = 0; i < width_pad; ++i) {
            Sli_setAt(out_slice, out_pos++, ' ');
        }
    }

    // Sign
    bti_memcpy(Sli_ptr(out_slice) + out_pos, Sli_ptr(sign_str), sign_str.len);
    out_pos += sign_str.len;

    // Prefix
    bti_memcpy(Sli_ptr(out_slice) + out_pos, Sli_ptr(prefix_str), prefix_str.len);
    out_pos += prefix_str.len;

    // Zero padding for width (goes after sign/prefix but before precision)
    if (pad_char == '0') {
        for (usize i = 0; i < width_pad; ++i) {
            Sli_setAt(out_slice, out_pos++, '0');
        }
    }

    // Precision padding (minimum digits)
    for (usize i = 0; i < precision_pad; ++i) {
        Sli_setAt(out_slice, out_pos++, '0');
    }

    // Actual digits
    bti_memcpy(Sli_ptr(out_slice) + out_pos, Sli_ptr(digits_slice), digits_slice.len);
    out_pos += digits_slice.len;

    // Left align with spaces (only if not zero-padding)
    if (align == fmt_AlignType_left && pad_char != '0') {
        for (usize i = 0; i < width_pad; ++i) {
            Sli_setAt(out_slice, out_pos++, ' ');
        }
    }

    try_(fmt_writeStr(writer, Sli_slice(out_slice.as_const, (0, out_pos))));

    return_ok({});
} $unscoped;

static fn_(fmt_writeFlt(io_Writer writer, f64 value, fmt_FormatSpec spec), Err$void $scope) {
    // Handle NaN and Infinity
    if (isnan(value)) {
        try_(fmt_writeStr(writer, spec.uppercase ? u8_l("NAN") : u8_l("nan")));
        return_ok({});
    }
    if (isinf(value)) {
        if (value < 0) {
            try_(fmt_writeStr(writer, spec.uppercase ? u8_l("-INF") : u8_l("-inf")));
        } else {
            try_(fmt_writeStr(writer, spec.uppercase ? u8_l("INF") : u8_l("inf")));
        }
        return_ok({});
    }

    // Determine sign
    Sli_const$u8 sign_str = u8_l("");
    if (value < 0) {
        sign_str = u8_l("-");
    } else if (spec.show_sign) {
        sign_str = u8_l("+");
    } else if (spec.space_sign) {
        sign_str = u8_l(" ");
    }

    let abs_value = math_abs(value);

    // Get integer and fractional parts
    f64 int_part_f  = math_floor(abs_value);
    f64 frac_part_f = abs_value - int_part_f;

    u64 int_part = (u64)int_part_f;

    // Convert integer part to string
    use_Arr$(64, u8);
    Arr$64$u8 int_buffer = Arr_zero();
    usize     int_pos    = Arr_len(int_buffer);
    if (int_part == 0) {
        Arr_setAt(int_buffer, --int_pos, '0');
    } else {
        u64 temp_int = int_part;
        while (temp_int > 0) {
            Arr_setAt(int_buffer, --int_pos, (temp_int % 10) + '0');
            temp_int /= 10;
        }
    }
    let int_slice = Sli_suffix(Sli_arr$(Sli_const$u8, int_buffer), int_pos);

    // Convert fractional part to string
    usize     precision   = orelse(spec.precision, 6);
    Arr$64$u8 frac_buffer = Arr_zero();
    Sli$u8    frac_slice  = Sli_arr$(Sli$u8, frac_buffer);
    if (precision > 0) {
        frac_part_f *= math_pow(10.0, precision);
        u64 frac_part_u = (u64)(frac_part_f + 0.5); // rounding
        for (usize i = precision; i > 0; --i) {
            Sli_setAt(frac_slice, i - 1, (frac_part_u % 10) + '0');
            frac_part_u /= 10;
        }
    }

    // Combine parts
    usize content_len = sign_str.len + int_slice.len + (precision > 0 ? 1 : 0) + precision;
    usize width_pad   = 0;
    if_some(spec.width, w) {
        if (w > content_len) { width_pad = w - content_len; }
    }

    u8 pad_char = ' ';
    if (spec.zero_pad && !spec.right_align_flag) { pad_char = '0'; }

    // Writing
    if (spec.align != fmt_AlignType_left && pad_char == ' ') {
        for (usize i = 0; i < width_pad; ++i) { try_(fmt_writeChar(writer, ' ')); }
    }

    try_(fmt_writeStr(writer, sign_str));

    if (spec.align != fmt_AlignType_left && pad_char == '0') {
        for (usize i = 0; i < width_pad; ++i) { try_(fmt_writeChar(writer, '0')); }
    }

    try_(fmt_writeStr(writer, int_slice));
    if (precision > 0) {
        try_(fmt_writeChar(writer, '.'));
        try_(fmt_writeStr(writer, Sli_slice(frac_slice.as_const, (0, precision))));
    }

    if (spec.align == fmt_AlignType_left) {
        for (usize i = 0; i < width_pad; ++i) { try_(fmt_writeChar(writer, ' ')); }
    }

    return_ok({});
} $unscoped;

static fn_(fmt_writePtr(io_Writer writer, const anyptr ptr), Err$void $scope) {
    use_Arr$(32, u8);
    Arr$32$u8 buffer = Arr_zero();
    usize     pos    = Arr_len(buffer);

    var        addr         = intFromRawptr(ptr);
    static let digits_lower = u8_l("0123456789abcdef");
    static let digits_upper = u8_l("0123456789ABCDEF");
    var        digits       = digits_lower;

    // Convert to hex (backwards)
    if (addr == 0) {
        Arr_setAt(buffer, --pos, '0');
    } else {
        while (addr > 0) {
            Arr_setAt(buffer, --pos, Sli_getAt(digits, addr % 16));
            addr /= 16;
        }
    }

    // Add 0x prefix
    Arr_setAt(buffer, --pos, 'x');
    Arr_setAt(buffer, --pos, '0');

    // Convert to slice and get suffix
    let buffer_slice = Sli_arr$(Sli_const$u8, buffer);
    let result_slice = Sli_suffix(buffer_slice, pos);
    try_(fmt_writeStr(writer, result_slice));
    return_ok({});
} $unscoped;

use_Opt$(fmt_FormatSpec);
static let spec_default = (fmt_FormatSpec){
    .arg_index          = 0,
    .has_explicit_index = false,
    .align              = fmt_AlignType_left,
    .fill_char          = none(),
    .alternate_form     = false,
    .uppercase          = false,
    .zero_pad           = false,
    .show_sign          = false,
    .space_sign         = false,
    .right_align_flag   = false,
    .width              = none(),
    .precision          = none(),
    .type_char          = 0,
    .is_64bit_type      = false,
    .is_size_type       = false,
    .has_type           = false
};

static fn_(fmt_isDigit(u8 ch), bool) {
    return '0' <= ch && ch <= '9';
}

static fn_(fmt_parseFormatSpec(Sli_const$u8 spec_str), Opt$fmt_FormatSpec $scope) {
    // Initialize with defaults
    if (spec_str.len == 0) { return_none(); }

    var out_spec = spec_default;

    usize pos = 0;

    // Skip leading ':' if present
    if (pos < spec_str.len && Sli_getAt(spec_str, pos) == ':') {
        ++pos;
    }

    // Parse index (if present) - only if digits are followed by ':'
    if (pos < spec_str.len && fmt_isDigit(Sli_getAt(spec_str, pos))) {
        // Look ahead to see if this is actually an index (digits followed by ':')
        usize lookahead_pos = pos;
        while (lookahead_pos < spec_str.len && fmt_isDigit(Sli_getAt(spec_str, lookahead_pos))) {
            ++lookahead_pos;
        }

        // Only parse as index if followed by ':'
        if (lookahead_pos < spec_str.len && Sli_getAt(spec_str, lookahead_pos) == ':') {
            usize index = 0;
            while (pos < lookahead_pos) {
                index = index * 10 + (Sli_getAt(spec_str, pos) - '0');
                ++pos;
            }
            out_spec.arg_index          = index;
            out_spec.has_explicit_index = true;

            // Skip ':'
            ++pos;
        }
    }

    // Parse flags and alignment
    while (pos < spec_str.len) {
        let ch = Sli_getAt(spec_str, pos);
        switch (ch) {
        case '#':
            out_spec.alternate_form = true;
            ++pos;
            break;
        case '!':
            out_spec.uppercase = true;
            ++pos;
            break;
        case '0':
            out_spec.zero_pad = true;
            ++pos;
            break;
        case '+':
            out_spec.show_sign = true;
            ++pos;
            break;
        case ' ':
            out_spec.space_sign = true;
            ++pos;
            break;
        case '-':
            out_spec.right_align_flag = true;
            ++pos;
            break;
        case '<':
            out_spec.align = fmt_AlignType_left;
            ++pos;
            break;
        case '>':
            out_spec.align = fmt_AlignType_right;
            ++pos;
            break;
        case '^':
            out_spec.align = fmt_AlignType_center;
            ++pos;
            break;
        default:
            goto parse_width;
        }
    }

parse_width:
    // Parse width
    if (pos < spec_str.len && fmt_isDigit(Sli_getAt(spec_str, pos))) {
        usize width = 0;
        while (pos < spec_str.len && fmt_isDigit(Sli_getAt(spec_str, pos))) {
            width = width * 10 + (Sli_getAt(spec_str, pos) - '0');
            ++pos;
        }
        Opt_asg(&out_spec.width, some(width));
    }

    // Parse precision
    if (pos < spec_str.len && Sli_getAt(spec_str, pos) == '.') {
        ++pos;
        usize precision = 0;
        while (pos < spec_str.len && fmt_isDigit(Sli_getAt(spec_str, pos))) {
            precision = precision * 10 + (Sli_getAt(spec_str, pos) - '0');
            ++pos;
        }
        Opt_asg(&out_spec.precision, some(precision));
    }

    // Parse type prefix
    if (pos + 1 < spec_str.len && Sli_getAt(spec_str, pos) == 'l' && Sli_getAt(spec_str, pos + 1) == 'l') {
        out_spec.is_64bit_type = true;
        pos += 2;
    } else if (pos + 1 < spec_str.len && Sli_getAt(spec_str, pos) == 'z') {
        // Only treat 'z' as prefix if there's another character after it
        out_spec.is_size_type = true;
        ++pos;
    }

    // Parse type character
    if (pos < spec_str.len) {
        out_spec.type_char = Sli_getAt(spec_str, pos);
        out_spec.has_type  = true;
        ++pos;
    }

    return_some(out_spec);
} $unscoped;

fn_(fmt_format(io_Writer writer, Sli_const$u8 fmt, ...), Err$void $guard) {
    va_list va_args = {};
    va_start(va_args, fmt);
    defer_(va_end(va_args));
    return_ok(try_(fmt_formatVaArgs(writer, fmt, va_args)));
} $unguarded;

static fn_(fmt_determineArgType(fmt_FormatSpec spec), fmt_ArgType);
static fn_(fmt_storeArg(fmt_StoredArg* stored_arg, fmt_ArgType type, va_list* args), void);
static fn_(fmt_writeStoredArg(io_Writer writer, fmt_StoredArg stored_arg, fmt_FormatSpec spec), Err$void);

fn_(fmt_formatVaArgs(io_Writer writer, Sli_const$u8 fmt, va_list va_args), Err$void $guard) {
    use_Arr$(32, fmt_FormatSpec);
    use_Arr$(32, usize);
    use_Arr$(32, fmt_StoredArg);

    va_list args = {};
    va_copy(args, va_args);
    defer_(va_end(args));

    // First pass: collect all format specifications to determine if indexed access is needed
    Arr$32$fmt_FormatSpec format_specs        = Arr_zero();
    Arr$32$usize          spec_positions      = Arr_zero(); // Store positions in format string
    usize                 format_spec_count   = 0;
    bool                  uses_indexed_access = false;
    usize                 max_arg_index       = 0;

    use_Arr$(32, bool);
    Arr$32$bool is_arg_used         = Arr_zero();
    usize       next_implicit_index = 0;

    usize pos = 0;
    while (pos < fmt.len) {
        let ch = Sli_getAt(fmt, pos);
        if (ch == '{') {
            if (pos + 1 < fmt.len && Sli_getAt(fmt, pos + 1) == '{') {
                pos += 2; // Skip escaped '{'
                continue;
            }

            // Find closing '}'
            usize spec_start = pos + 1;
            usize spec_end   = spec_start;
            while (spec_end < fmt.len && Sli_getAt(fmt, spec_end) != '}') { ++spec_end; }
            if (fmt.len <= spec_end) {
                pos++;
                continue; // Malformed - skip
            }

            // Parse format specification
            let spec_str = Sli_slice(fmt, (spec_start, spec_end));
            let spec_opt = fmt_parseFormatSpec(spec_str);
            if (isNone(spec_opt)) {
                // Malformed specifier, write literally
                try_(fmt_writeStr(writer, Sli_slice(fmt, (pos, spec_end + 1))));
                pos = spec_end + 1;
                continue;
            }
            var spec = unwrap(spec_opt);

            if (format_spec_count < Arr_len(format_specs)) {
                if (spec.has_explicit_index) {
                    uses_indexed_access = true;
                    if (spec.arg_index < Arr_len(is_arg_used)) {
                        Arr_setAt(is_arg_used, spec.arg_index, true);
                    }
                } else {
                    while (next_implicit_index < Arr_len(is_arg_used) && Arr_getAt(is_arg_used, next_implicit_index)) {
                        next_implicit_index++;
                    }
                    spec.arg_index = next_implicit_index;
                    if (next_implicit_index < Arr_len(is_arg_used)) {
                        Arr_setAt(is_arg_used, next_implicit_index, true);
                    }
                    next_implicit_index++;
                }

                Arr_setAt(format_specs, format_spec_count, spec);
                Arr_setAt(spec_positions, format_spec_count, pos);
                max_arg_index = prim_max(max_arg_index, spec.arg_index);
                format_spec_count++;
            }
            pos = spec_end + 1;
        } else if (ch == '}') {
            if (pos + 1 < fmt.len && Sli_getAt(fmt, pos + 1) == '}') {
                pos += 2; // Skip escaped '}'
                continue;
            }
            ++pos;
        } else {
            ++pos;
        }
    }

    let total_arg_count = prim_max(max_arg_index + 1, next_implicit_index);

    // If indexed access is used, store all arguments in an array
    Arr$32$fmt_StoredArg stored_args = Arr_zero();
    if (uses_indexed_access) {
        if (total_arg_count > Arr_len(stored_args)) {
            // Handle error: too many arguments for storage
        } else {
            // Create an array to track argument types by index
            use_Arr$(32, fmt_ArgType);
            Arr$32$fmt_ArgType arg_types = Arr_zero();
            for (usize i = 0; i < Arr_len(arg_types); ++i) {
                Arr_setAt(arg_types, i, fmt_ArgType_unknown);
            }

            // First, determine the type for each argument index
            for (usize i = 0; i < format_spec_count; ++i) {
                let spec = Arr_getAt(format_specs, i);
                if (spec.has_type && spec.arg_index < Arr_len(arg_types)) {
                    let arg_type = fmt_determineArgType(spec);
                    if (Arr_getAt(arg_types, spec.arg_index) == fmt_ArgType_unknown) {
                        Arr_setAt(arg_types, spec.arg_index, arg_type);
                    }
                }
            }

            // Then store arguments in order (0, 1, 2, ...)
            for (usize arg_index = 0; arg_index < total_arg_count; ++arg_index) {
                let arg_type = Arr_getAt(arg_types, arg_index);
                if (arg_type != fmt_ArgType_unknown) {
                    fmt_storeArg(Arr_at(stored_args, arg_index), arg_type, &args);
                } else {
                    // Consume unused arguments as pointers to keep va_list consistent
                    fmt_storeArg(Arr_at(stored_args, arg_index), fmt_ArgType_ptr, &args);
                }
            }
        }
    }

    // Second pass: format the string
    pos                      = 0;
    usize current_spec_index = 0;
    while (pos < fmt.len) {
        let ch = Sli_getAt(fmt, pos);
        if (ch == '{') {
            if (pos + 1 < fmt.len && Sli_getAt(fmt, pos + 1) == '{') {
                // Escaped '{' - write literal '{'
                try_(fmt_writeChar(writer, '{'));
                pos += 2;
                continue;
            }

            // Find closing '}'
            usize spec_start = pos + 1;
            usize spec_end   = spec_start;
            while (spec_end < fmt.len && Sli_getAt(fmt, spec_end) != '}') { ++spec_end; }
            if (fmt.len <= spec_end) {
                // Malformed format string - missing '}'
                try_(fmt_writeChar(writer, ch));
                ++pos;
                continue;
            }

            // Use stored format specification if available
            if (current_spec_index < format_spec_count) {
                let spec = Arr_getAt(format_specs, current_spec_index);
                current_spec_index++;

                if (spec.has_type) {
                    if (uses_indexed_access) {
                        if (spec.arg_index >= total_arg_count) {
                            return_err(fmt_Err_IndexOutOfBounds());
                        } else {
                            // Use stored argument
                            let stored_arg = Arr_getAt(stored_args, spec.arg_index);
                            try_(fmt_writeStoredArg(writer, stored_arg, spec));
                        }
                    } else {
                        // Fall back to sequential argument consumption (original behavior)
                        switch (spec.type_char) {
                        case 'd': {
                            if (spec.is_64bit_type) {
                                let value = va_arg(args, i64);
                                try_(fmt_writeInt(writer, as$(u64, value), spec));
                            } else if (spec.is_size_type) {
                                let value = va_arg(args, isize);
                                try_(fmt_writeInt(writer, as$(u64, value), spec));
                            } else {
                                let value = va_arg(args, i32);
                                try_(fmt_writeInt(writer, as$(u64, value), spec));
                            }
                        } break;
                        case 'u':
                            $fallthrough;
                        case 'b':
                            $fallthrough;
                        case 'o':
                            $fallthrough;
                        case 'x':
                            $fallthrough;
                        case 'X': {
                            if (spec.is_64bit_type) {
                                let value = va_arg(args, u64);
                                try_(fmt_writeInt(writer, value, spec));
                            } else if (spec.is_size_type) {
                                let value = va_arg(args, usize);
                                try_(fmt_writeInt(writer, as$(u64, value), spec));
                            } else {
                                let value = va_arg(args, u32);
                                try_(fmt_writeInt(writer, as$(u64, value), spec));
                            }
                        } break;
                        case 'c': {
                            let value = as$(u8, va_arg(args, i32));
                            try_(fmt_writeChar(writer, value));
                        } break;
                        case 's': {
                            var value = va_arg(args, Sli_const$u8);
                            if_some(spec.precision, precision) {
                                if (precision < value.len) {
                                    value = Sli_slice(value, (0, precision));
                                }
                            }
                            try_(fmt_writeStr(writer, value));
                        } break;
                        case 'z': {
                            var value = Str_viewZ(va_arg(args, const u8*));
                            if_some(spec.precision, precision) {
                                if (precision < value.len) {
                                    value = Sli_slice(value, (0, precision));
                                }
                            }
                            try_(fmt_writeStr(writer, value));
                        } break;
                        case 'p': {
                            let value = va_arg(args, const anyptr);
                            try_(fmt_writePtr(writer, value));
                        } break;
                        case 'f':
                        case 'F': {
                            let value = va_arg(args, f64);
                            try_(fmt_writeFlt(writer, value, spec));
                        } break;
                        default: {
                            // Unknown type - write literally
                            try_(fmt_writeChar(writer, ch));
                            ++pos;
                            continue;
                        } break;
                        }
                    }
                } else {
                    // No type specified - consume argument but don't format it
                    if (!uses_indexed_access) {
                        $ignore = va_arg(args, anyptr);
                    }
                }
            } else {
                // No stored spec - write literally
                try_(fmt_writeChar(writer, ch));
                ++pos;
                continue;
            }

            pos = spec_end + 1;
        } else if (ch == '}') {
            if (pos + 1 < fmt.len && Sli_getAt(fmt, pos + 1) == '}') {
                // Escaped '}' - write literal '}'
                try_(fmt_writeChar(writer, '}'));
                pos += 2;
                continue;
            }
            // Unmatched '}' - write literally
            try_(fmt_writeChar(writer, ch));
            ++pos;
        } else {
            // Regular character
            try_(fmt_writeChar(writer, ch));
            ++pos;
        }
    }
    return_ok({});
} $unguarded;

use_Arr$(32, fmt_FormatSpec);
use_Arr$(32, fmt_StoredArg);
static fn_(fmt_determineArgType(fmt_FormatSpec spec), fmt_ArgType) {
    if (!spec.has_type) { return fmt_ArgType_unknown; }
    switch (spec.type_char) {
    case 'd': {
        if (spec.is_64bit_type) {
            return fmt_ArgType_i64;
        }
        if (spec.is_size_type) {
            return fmt_ArgType_isize;
        }
        return fmt_ArgType_i32;
    } break;
    case 'u':
    case 'b':
    case 'o':
    case 'x':
    case 'X': {
        if (spec.is_64bit_type) { return fmt_ArgType_u64; }
        if (spec.is_size_type) { return fmt_ArgType_usize; }
        return fmt_ArgType_u32;
    } break;
    case 'f':
    case 'F': {
        return fmt_ArgType_f64;
    } break;
    case 'c': {
        return fmt_ArgType_char;
    } break;
    case 's': {
        return fmt_ArgType_str_slice;
    } break;
    case 'z': {
        return fmt_ArgType_str_ptr;
    } break;
    case 'p': {
        return fmt_ArgType_ptr;
    } break;
    default:
        return fmt_ArgType_unknown;
    }
}

static fn_(fmt_storeArg(fmt_StoredArg* stored_arg, fmt_ArgType type, va_list* args), void) {
    stored_arg->type = type;
    switch (type) {
    case fmt_ArgType_i32: {
        stored_arg->value.i32_val = va_arg(*args, i32);
    } break;
    case fmt_ArgType_i64: {
        stored_arg->value.i64_val = va_arg(*args, i64);
    } break;
    case fmt_ArgType_isize: {
        stored_arg->value.isize_val = va_arg(*args, isize);
    } break;
    case fmt_ArgType_u32: {
        stored_arg->value.u32_val = va_arg(*args, u32);
    } break;
    case fmt_ArgType_u64: {
        stored_arg->value.u64_val = va_arg(*args, u64);
    } break;
    case fmt_ArgType_usize: {
        stored_arg->value.usize_val = va_arg(*args, usize);
    } break;
    case fmt_ArgType_f64: {
        stored_arg->value.f64_val = va_arg(*args, f64);
    } break;
    case fmt_ArgType_char: {
        stored_arg->value.char_val = as$(u8, va_arg(*args, i32));
    } break;
    case fmt_ArgType_str_slice: {
        stored_arg->value.str_slice_val = va_arg(*args, Sli_const$u8);
    } break;
    case fmt_ArgType_str_ptr: {
        stored_arg->value.str_ptr_val = va_arg(*args, const u8*);
    } break;
    case fmt_ArgType_ptr: {
        stored_arg->value.ptr_val = va_arg(*args, const anyptr);
    } break;
    case fmt_ArgType_unknown:
        $fallthrough;
    default: {
        $ignore = va_arg(*args, anyptr);
    } break;
    }
}

static fn_(fmt_writeStoredArg(io_Writer writer, fmt_StoredArg stored_arg, fmt_FormatSpec spec), Err$void $scope) {
    switch (stored_arg.type) {
    case fmt_ArgType_i32: {
        try_(fmt_writeInt(writer, as$(u64, stored_arg.value.i32_val), spec));
    } break;
    case fmt_ArgType_i64: {
        try_(fmt_writeInt(writer, as$(u64, stored_arg.value.i64_val), spec));
    } break;
    case fmt_ArgType_isize: {
        try_(fmt_writeInt(writer, as$(u64, stored_arg.value.isize_val), spec));
    } break;
    case fmt_ArgType_u32: {
        try_(fmt_writeInt(writer, as$(u64, stored_arg.value.u32_val), spec));
    } break;
    case fmt_ArgType_u64: {
        try_(fmt_writeInt(writer, stored_arg.value.u64_val, spec));
    } break;
    case fmt_ArgType_usize: {
        try_(fmt_writeInt(writer, as$(u64, stored_arg.value.usize_val), spec));
    } break;
    case fmt_ArgType_char: {
        try_(fmt_writeChar(writer, stored_arg.value.char_val));
    } break;
    case fmt_ArgType_str_slice: {
        var value = stored_arg.value.str_slice_val;
        if_some(spec.precision, precision) {
            if (precision < value.len) {
                value = Sli_slice(value, (0, precision));
            }
        }
        try_(fmt_writeStr(writer, value));
    } break;
    case fmt_ArgType_str_ptr: {
        var value = Str_viewZ(stored_arg.value.str_ptr_val);
        if_some(spec.precision, precision) {
            if (precision < value.len) {
                value = Sli_slice(value, (0, precision));
            }
        }
        try_(fmt_writeStr(writer, value));
    } break;
    case fmt_ArgType_ptr:
        try_(fmt_writePtr(writer, stored_arg.value.ptr_val));
        break;
    case fmt_ArgType_f64:
        try_(fmt_writeFlt(writer, stored_arg.value.f64_val, spec));
        break;
    case fmt_ArgType_unknown: {
        // Not implemented or unknown type
    } break;
    default:
        claim_unreachable;
    }
    return_ok({});
} $unscoped;

fn_(fmt_parseInt_u64(Sli_const$u8 str, u8 base), Err$u64 $scope) {
    if (str.len == 0) { return_err(fmt_Err_InvalidIntegerFormat()); }
    if (base < 2 || 36 < base) { return_err(fmt_Err_InvalidIntegerFormat()); }

    usize pos = 0;
    if (0 < str.len && Sli_getAt(str, 0) == '+') { pos = 1; }

    if (str.len <= pos) { return_err(fmt_Err_InvalidIntegerFormat()); }

    u64       result  = 0;
    const u64 max_div = u64_limit_max / base;
    const u64 max_rem = u64_limit_max % base;

    for (usize i = pos; i < str.len; ++i) {
        let ch    = Sli_getAt(str, i);
        u8  digit = 0;
        if ('0' <= ch && ch <= '9') {
            digit = ch - '0';
        } else if ('a' <= ch && ch <= 'z') {
            digit = ch - 'a' + 10;
        } else if ('A' <= ch && ch <= 'Z') {
            digit = ch - 'A' + 10;
        } else {
            return_err(fmt_Err_InvalidIntegerFormat());
        }

        if (base <= digit) { return_err(fmt_Err_InvalidIntegerFormat()); }

        if (max_div < result || (result == max_div && max_rem < digit)) { return_err(fmt_Err_InvalidIntegerFormat()); }

        result = result * base + digit;
    }

    return_ok(result);
} $unscoped;

fn_(fmt_parseInt_usize(Sli_const$u8 str, u8 base), Err$usize $scope) {
    let result = try_(fmt_parseInt_u64(str, base));
    if (usize_limit_max < result) { return_err(fmt_Err_InvalidIntegerFormat()); }
    return_ok(as$(usize, result));
} $unscoped;

fn_(fmt_parseInt_u32(Sli_const$u8 str, u8 base), Err$u32 $scope) {
    let result = try_(fmt_parseInt_u64(str, base));
    if (u32_limit_max < result) { return_err(fmt_Err_InvalidIntegerFormat()); }
    return_ok(as$(u32, result));
} $unscoped;

fn_(fmt_parseInt_i64(Sli_const$u8 str, u8 base), Err$i64 $scope) {
    if (str.len == 0) { return_err(fmt_Err_InvalidIntegerFormat()); }
    if (base < 2 || 36 < base) { return_err(fmt_Err_InvalidIntegerFormat()); }

    usize pos         = 0;
    bool  is_negative = false;

    if (0 < str.len && Sli_getAt(str, 0) == '+') {
        pos = 1;
    } else if (0 < str.len && Sli_getAt(str, 0) == '-') {
        is_negative = true;
        pos         = 1;
    }

    if (str.len <= pos) { return_err(fmt_Err_InvalidIntegerFormat()); }

    u64 result = 0;

    u64 max_val = 0;
    if (is_negative) {
        max_val = as$(u64, i64_limit_max) + 1;
    } else {
        max_val = i64_limit_max;
    }

    const u64 max_div = max_val / base;
    const u64 max_rem = max_val % base;

    for (usize i = pos; i < str.len; ++i) {
        let ch    = Sli_getAt(str, i);
        u8  digit = 0;
        if ('0' <= ch && ch <= '9') {
            digit = ch - '0';
        } else if ('a' <= ch && ch <= 'z') {
            digit = ch - 'a' + 10;
        } else if ('A' <= ch && ch <= 'Z') {
            digit = ch - 'A' + 10;
        } else {
            return_err(fmt_Err_InvalidIntegerFormat());
        }

        if (base <= digit) { return_err(fmt_Err_InvalidIntegerFormat()); }

        if (max_div < result || (result == max_div && max_rem < digit)) { return_err(fmt_Err_InvalidIntegerFormat()); }

        result = result * base + digit;
    }

    if (is_negative) {
        return_ok(-as$(i64, result));
    } else {
        return_ok(as$(i64, result));
    }
} $unscoped;

fn_(fmt_parseInt_isize(Sli_const$u8 str, u8 base), Err$isize $scope) {
    let result = try_(fmt_parseInt_i64(str, base));
    if (result < isize_limit_min || isize_limit_max < result) { return_err(fmt_Err_InvalidIntegerFormat()); }
    return_ok(as$(isize, result));
} $unscoped;

fn_(fmt_parseInt_i32(Sli_const$u8 str, u8 base), Err$i32 $scope) {
    let result = try_(fmt_parseInt_i64(str, base));
    if (result < i32_limit_min || i32_limit_max < result) { return_err(fmt_Err_InvalidIntegerFormat()); }
    return_ok(as$(i32, result));
} $unscoped;

static fn_(fmt_parseFlt(Sli_const$u8 str), Err$f64 $scope) {
    if (str.len == 0) { return_err(fmt_Err_InvalidFloatFormat()); }

    usize pos          = 0;
    f64   sign         = 1.0;
    f64   result       = 0.0;
    bool  has_int_part = false;

    if (pos < str.len && Sli_getAt(str, pos) == '+') {
        pos++;
    } else if (pos < str.len && Sli_getAt(str, pos) == '-') {
        sign = -1.0;
        pos++;
    }

    // Integer part
    while (pos < str.len && '0' <= Sli_getAt(str, pos) && Sli_getAt(str, pos) <= '9') {
        result       = result * 10.0 + (Sli_getAt(str, pos) - '0');
        has_int_part = true;
        pos++;
    }

    // Fractional part
    if (pos < str.len && Sli_getAt(str, pos) == '.') {
        pos++;
        f64  power        = 0.1;
        bool has_fracpart = false;
        while (pos < str.len && '0' <= Sli_getAt(str, pos) && Sli_getAt(str, pos) <= '9') {
            result += (Sli_getAt(str, pos) - '0') * power;
            power *= 0.1;
            has_fracpart = true;
            pos++;
        }
        if (!has_int_part && !has_fracpart) { return_err(fmt_Err_InvalidFloatFormat()); }
    } else {
        if (!has_int_part) { return_err(fmt_Err_InvalidFloatFormat()); }
    }


    // Exponent part
    if (pos < str.len && (Sli_getAt(str, pos) == 'e' || Sli_getAt(str, pos) == 'E')) {
        pos++;
        f64  exp_sign = 1.0;
        i32  exp_val  = 0;
        bool has_exp  = false;

        if (pos < str.len && Sli_getAt(str, pos) == '+') {
            pos++;
        } else if (pos < str.len && Sli_getAt(str, pos) == '-') {
            exp_sign = -1.0;
            pos++;
        }

        while (pos < str.len && '0' <= Sli_getAt(str, pos) && Sli_getAt(str, pos) <= '9') {
            exp_val = exp_val * 10 + (Sli_getAt(str, pos) - '0');
            has_exp = true;
            pos++;
        }

        if (!has_exp) { return_err(fmt_Err_InvalidFloatFormat()); }

        result *= math_pow(10.0, exp_val * exp_sign);
    }

    if (pos != str.len) { return_err(fmt_Err_InvalidFloatFormat()); }

    return_ok(result * sign);
} $unscoped;

fn_(fmt_parseFlt_f64(Sli_const$u8 str), Err$f64 $scope) {
    return_ok(try_(fmt_parseFlt(str)));
} $unscoped;

fn_(fmt_parseFlt_f32(Sli_const$u8 str), Err$f32 $scope) {
    let res = try_(fmt_parseFlt(str));
    // TODO: Add proper overflow/underflow check for f32
    return_ok(as$(f32, res));
} $unscoped;

fn_(fmt_parseBool(Sli_const$u8 str), Err$bool $scope) {
    if (Str_eqlNoCase(str, u8_l("1")) || Str_eqlNoCase(str, u8_l("true"))) { return_ok(true); }
    if (Str_eqlNoCase(str, u8_l("0")) || Str_eqlNoCase(str, u8_l("false"))) { return_ok(false); }
    return_err(fmt_Err_InvalidBoolFormat());
} $unscoped;

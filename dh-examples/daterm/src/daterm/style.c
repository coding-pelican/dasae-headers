#include "daterm/style.h"

/*========== External Definitions ===========================================*/

fn_((daterm_style_bold(bool enabled))(S_const$u8)) {
    return enabled ? u8_l(daterm_style_bold_static(pp_true))
                   : u8_l(daterm_style_bold_static(pp_false));
};

fn_((daterm_style_boldWrite(bool enabled, io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_style_bold(enabled));
};

fn_((daterm_style_dim(bool enabled))(S_const$u8)) {
    return enabled ? u8_l(daterm_style_dim_static(pp_true))
                   : u8_l(daterm_style_dim_static(pp_false));
}

fn_((daterm_style_dimWrite(bool enabled, io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_style_dim(enabled));
};

fn_((daterm_style_italic(bool enabled))(S_const$u8)) {
    return enabled ? u8_l(daterm_style_italic_static(pp_true))
                   : u8_l(daterm_style_italic_static(pp_false));
};

fn_((daterm_style_italicWrite(bool enabled, io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_style_italic(enabled));
};

fn_((daterm_style_underline(bool enabled))(S_const$u8)) {
    return enabled ? u8_l(daterm_style_underline_static(pp_true))
                   : u8_l(daterm_style_underline_static(pp_false));
};

fn_((daterm_style_underlineWrite(bool enabled, io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_style_underline(enabled));
};

fn_((daterm_style_blinking(bool enabled))(S_const$u8)) {
    return enabled ? u8_l(daterm_style_blinking_static(pp_true))
                   : u8_l(daterm_style_blinking_static(pp_false));
};

fn_((daterm_style_blinkingWrite(bool enabled, io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_style_blinking(enabled));
};

fn_((daterm_style_reverse(bool enabled))(S_const$u8)) {
    return enabled ? u8_l(daterm_style_reverse_static(pp_true))
                   : u8_l(daterm_style_reverse_static(pp_false));
};

fn_((daterm_style_reverseWrite(bool enabled, io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_style_reverse(enabled));
};

fn_((daterm_style_invisible(bool enabled))(S_const$u8)) {
    return enabled ? u8_l(daterm_style_invisible_static(pp_true))
                   : u8_l(daterm_style_invisible_static(pp_false));
};

fn_((daterm_style_invisibleWrite(bool enabled, io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_style_invisible(enabled));
};

fn_((daterm_style_strikethrough(bool enabled))(S_const$u8)) {
    return enabled ? u8_l(daterm_style_strikethrough_static(pp_true))
                   : u8_l(daterm_style_strikethrough_static(pp_false));
};

fn_((daterm_style_strikethroughWrite(bool enabled, io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, daterm_style_strikethrough(enabled));
};

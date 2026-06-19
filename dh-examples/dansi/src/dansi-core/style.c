#include "dansi-core/style.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_style__one(dansi_sgr_Code code, S$u8 buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    catch_((dansi_sgr_setOneWrite(code, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_style_bold(bool enabled, dansi_style_BoldBuf* buf))(S$u8)) {
    return dansi_style__one(enabled ? dansi_sgr_Code_bold : dansi_sgr_Code_no_bold_faint, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_boldWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(enabled ? dansi_sgr_Code_bold : dansi_sgr_Code_no_bold_faint, out);
};

fn_((dansi_style_faint(bool enabled, dansi_style_FaintBuf* buf))(S$u8)) {
    return dansi_style__one(enabled ? dansi_sgr_Code_faint : dansi_sgr_Code_no_bold_faint, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_faintWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(enabled ? dansi_sgr_Code_faint : dansi_sgr_Code_no_bold_faint, out);
};

fn_((dansi_style_italic(bool enabled, dansi_style_ItalicBuf* buf))(S$u8)) {
    return dansi_style__one(enabled ? dansi_sgr_Code_italic : dansi_sgr_Code_no_italic, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_italicWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(enabled ? dansi_sgr_Code_italic : dansi_sgr_Code_no_italic, out);
};

fn_((dansi_style_underline(bool enabled, dansi_style_UnderlineBuf* buf))(S$u8)) {
    return dansi_style__one(enabled ? dansi_sgr_Code_underline : dansi_sgr_Code_no_underline, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_underlineWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(enabled ? dansi_sgr_Code_underline : dansi_sgr_Code_no_underline, out);
};

fn_((dansi_style_blink(bool enabled, dansi_style_BlinkBuf* buf))(S$u8)) {
    return dansi_style__one(enabled ? dansi_sgr_Code_blink : dansi_sgr_Code_no_blink, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_blinkWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(enabled ? dansi_sgr_Code_blink : dansi_sgr_Code_no_blink, out);
};

fn_((dansi_style_reverse(bool enabled, dansi_style_ReverseBuf* buf))(S$u8)) {
    return dansi_style__one(enabled ? dansi_sgr_Code_reverse : dansi_sgr_Code_no_reverse, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_reverseWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(enabled ? dansi_sgr_Code_reverse : dansi_sgr_Code_no_reverse, out);
};

fn_((dansi_style_conceal(bool enabled, dansi_style_ConcealBuf* buf))(S$u8)) {
    return dansi_style__one(enabled ? dansi_sgr_Code_conceal : dansi_sgr_Code_no_conceal, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_concealWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(enabled ? dansi_sgr_Code_conceal : dansi_sgr_Code_no_conceal, out);
};

fn_((dansi_style_crossedOut(bool enabled, dansi_style_CrossedOutBuf* buf))(S$u8)) {
    return dansi_style__one(enabled ? dansi_sgr_Code_crossed_out : dansi_sgr_Code_no_crossed_out, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_crossedOutWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(enabled ? dansi_sgr_Code_crossed_out : dansi_sgr_Code_no_crossed_out, out);
};

fn_((dansi_style_resetIntensity(dansi_style_ResetIntensityBuf* buf))(S$u8)) {
    return dansi_style__one(dansi_sgr_Code_no_bold_faint, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_resetIntensityWrite(io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(dansi_sgr_Code_no_bold_faint, out);
};

fn_((dansi_style_resetItalic(dansi_style_ResetItalicBuf* buf))(S$u8)) {
    return dansi_style__one(dansi_sgr_Code_no_italic, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_resetItalicWrite(io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(dansi_sgr_Code_no_italic, out);
};

fn_((dansi_style_resetUnderline(dansi_style_ResetUnderlineBuf* buf))(S$u8)) {
    return dansi_style__one(dansi_sgr_Code_no_underline, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_resetUnderlineWrite(io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(dansi_sgr_Code_no_underline, out);
};

fn_((dansi_style_resetBlink(dansi_style_ResetBlinkBuf* buf))(S$u8)) {
    return dansi_style__one(dansi_sgr_Code_no_blink, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_resetBlinkWrite(io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(dansi_sgr_Code_no_blink, out);
};

fn_((dansi_style_resetReverse(dansi_style_ResetReverseBuf* buf))(S$u8)) {
    return dansi_style__one(dansi_sgr_Code_no_reverse, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_resetReverseWrite(io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(dansi_sgr_Code_no_reverse, out);
};

fn_((dansi_style_resetConceal(dansi_style_ResetConcealBuf* buf))(S$u8)) {
    return dansi_style__one(dansi_sgr_Code_no_conceal, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_resetConcealWrite(io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(dansi_sgr_Code_no_conceal, out);
};

fn_((dansi_style_resetCrossedOut(dansi_style_ResetCrossedOutBuf* buf))(S$u8)) {
    return dansi_style__one(dansi_sgr_Code_no_crossed_out, A_ref$((S$u8)(*buf)));
};

fn_((dansi_style_resetCrossedOutWrite(io_Writer out))(E$void)) {
    return dansi_sgr_setOneWrite(dansi_sgr_Code_no_crossed_out, out);
};

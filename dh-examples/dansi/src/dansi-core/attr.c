#include "dansi-core/attr.h"

/*========== External Definitions ===========================================*/

fn_((dansi_attr_reset(void))(S_const$u8)) {
    return u8_l(dansi_attr_reset_static());
};

fn_((dansi_attr_resetWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_attr_reset());
};

fn_((dansi_attr_resetFG(void))(S_const$u8)) {
    return u8_l(dansi_attr_resetFG_static());
};

fn_((dansi_attr_resetFGWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_attr_resetFG());
};

fn_((dansi_attr_resetBG(void))(S_const$u8)) {
    return u8_l(dansi_attr_resetBG_static());
};

fn_((dansi_attr_resetBGWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_attr_resetBG());
};

fn_((dansi_attr_resetIntensity(void))(S_const$u8)) {
    return u8_l(dansi_attr_resetIntensity_static());
};

fn_((dansi_attr_resetIntensityWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_attr_resetIntensity());
};

fn_((dansi_attr_resetItalic(void))(S_const$u8)) {
    return u8_l(dansi_attr_resetItalic_static());
};

fn_((dansi_attr_resetItalicWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_attr_resetItalic());
};

fn_((dansi_attr_resetUnderline(void))(S_const$u8)) {
    return u8_l(dansi_attr_resetUnderline_static());
};

fn_((dansi_attr_resetUnderlineWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_attr_resetUnderline());
};

fn_((dansi_attr_resetBlinking(void))(S_const$u8)) {
    return u8_l(dansi_attr_resetBlinking_static());
};

fn_((dansi_attr_resetBlinkingWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_attr_resetBlinking());
};

fn_((dansi_attr_resetReverse(void))(S_const$u8)) {
    return u8_l(dansi_attr_resetReverse_static());
};

fn_((dansi_attr_resetReverseWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_attr_resetReverse());
};

fn_((dansi_attr_resetInvisible(void))(S_const$u8)) {
    return u8_l(dansi_attr_resetInvisible_static());
};

fn_((dansi_attr_resetInvisibleWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_attr_resetInvisible());
};

fn_((dansi_attr_resetStrikethrough(void))(S_const$u8)) {
    return u8_l(dansi_attr_resetStrikethrough_static());
};

fn_((dansi_attr_resetStrikethroughWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_attr_resetStrikethrough());
};

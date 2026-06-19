/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    color.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_color
 */
#pragma once
#ifndef dansi_xterm_color__included
#define dansi_xterm_color__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/color.h"
#include "Palette4bit.h"
#include "Palette8bit.h"
#include "sgr.h"

/*========== Macros and Declarations ========================================*/

typedef struct dansi_xterm_color_RGB8 {
    var_(r, u8);
    var_(g, u8);
    var_(b, u8);
} dansi_xterm_color_RGB8;
T_use_prl$(dansi_xterm_color_RGB8);

typedef struct dansi_xterm_color_RGB16 {
    var_(r, u16);
    var_(g, u16);
    var_(b, u16);
} dansi_xterm_color_RGB16;
T_use_prl$(dansi_xterm_color_RGB16);

$extern fn_((dansi_xterm_color_RGB16_toRGB(dansi_xterm_color_RGB16 rgb))(dansi_xterm_color_RGB8));
$extern fn_((dansi_xterm_color_RGB8_toRGB16(dansi_xterm_color_RGB8 rgb))(dansi_xterm_color_RGB16));

#define dansi_xterm_color_fg4bit_staticParse(_color_tok) \
    ____dansi_xterm_color_fg4bit_staticParse(_color_tok)
#define dansi_xterm_color_fg4bit_static(_color_tok) \
    ____dansi_xterm_color_fg4bit_static(_color_tok)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_color_Fg4bitBuf;
$attr($must_check)
$extern fn_((dansi_xterm_color_fg4bit(
    dansi_xterm_Palette4bit color, dansi_xterm_color_Fg4bitBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_color_fg4bitWrite(dansi_xterm_Palette4bit color, io_Writer out))(E$void));

#define dansi_xterm_color_bg4bit_staticParse(_color_tok) \
    ____dansi_xterm_color_bg4bit_staticParse(_color_tok)
#define dansi_xterm_color_bg4bit_static(_color_tok) \
    ____dansi_xterm_color_bg4bit_static(_color_tok)
typedef A$$(4 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_color_Bg4bitBuf;
$attr($must_check)
$extern fn_((dansi_xterm_color_bg4bit(
    dansi_xterm_Palette4bit color, dansi_xterm_color_Bg4bitBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_color_bg4bitWrite(dansi_xterm_Palette4bit color, io_Writer out))(E$void));

#define dansi_xterm_color_fgBright_static(_color_tok) \
    ____dansi_xterm_color_fgBright_static(_color_tok)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_color_FgBrightBuf;
$attr($must_check)
$extern fn_((dansi_xterm_color_fgBright(dansi_color_Std color, dansi_xterm_color_FgBrightBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_color_fgBrightWrite(dansi_color_Std color, io_Writer out))(E$void));

#define dansi_xterm_color_bgBright_static(_color_tok) \
    ____dansi_xterm_color_bgBright_static(_color_tok)
typedef A$$(4 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_color_BgBrightBuf;
$attr($must_check)
$extern fn_((dansi_xterm_color_bgBright(dansi_color_Std color, dansi_xterm_color_BgBrightBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_color_bgBrightWrite(dansi_color_Std color, io_Writer out))(E$void));

#define dansi_xterm_color_fg8bit_static(_index_tok) \
    ____dansi_xterm_color_fg8bit_static(_index_tok)
typedef A$$(7 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_color_Fg8bitBuf;
$attr($must_check)
$extern fn_((dansi_xterm_color_fg8bit(
    dansi_xterm_Palette8bit color, dansi_xterm_color_Fg8bitBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_color_fg8bitWrite(dansi_xterm_Palette8bit color, io_Writer out))(E$void));

#define dansi_xterm_color_bg8bit_static(_index_tok) \
    ____dansi_xterm_color_bg8bit_static(_index_tok)
typedef A$$(7 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_color_Bg8bitBuf;
$attr($must_check)
$extern fn_((dansi_xterm_color_bg8bit(
    dansi_xterm_Palette8bit color, dansi_xterm_color_Bg8bitBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_color_bg8bitWrite(dansi_xterm_Palette8bit color, io_Writer out))(E$void));

#define dansi_xterm_color_fgRGB_static(_r_tok, _g_tok, _b_tok) \
    ____dansi_xterm_color_fgRGB_static(_r_tok, _g_tok, _b_tok)
typedef A$$(9 + uint_log10Ceil_static(u8_limit_max) * 3, u8) dansi_xterm_color_FgRGBBuf;
$attr($must_check)
$extern fn_((dansi_xterm_color_fgRGB(dansi_xterm_color_RGB8 rgb, dansi_xterm_color_FgRGBBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_color_fgRGBWrite(dansi_xterm_color_RGB8 rgb, io_Writer out))(E$void));

#define dansi_xterm_color_bgRGB_static(_r_tok, _g_tok, _b_tok) \
    ____dansi_xterm_color_bgRGB_static(_r_tok, _g_tok, _b_tok)
typedef A$$(9 + uint_log10Ceil_static(u8_limit_max) * 3, u8) dansi_xterm_color_BgRGBBuf;
$attr($must_check)
$extern fn_((dansi_xterm_color_bgRGB(dansi_xterm_color_RGB8 rgb, dansi_xterm_color_BgRGBBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_color_bgRGBWrite(dansi_xterm_color_RGB8 rgb, io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____dansi_xterm_color_fg4bit_staticParse(_color_tok) \
    dansi_xterm_color__parsePalette4bit(dansi_xterm_color__pp_4bit_layer_fg, _color_tok)
#define ____dansi_xterm_color_fg4bit_static(_color_tok) \
    dansi_sgr_setRaw_static(dansi_xterm_color_fg4bit_staticParse(_color_tok))
#define ____dansi_xterm_color_bg4bit_staticParse(_color_tok) \
    dansi_xterm_color__parsePalette4bit(dansi_xterm_color__pp_4bit_layer_bg, _color_tok)
#define ____dansi_xterm_color_bg4bit_static(_color_tok) \
    dansi_sgr_setRaw_static(dansi_xterm_color_bg4bit_staticParse(_color_tok))
#define ____dansi_xterm_color_fgBright_static(_color_tok) \
    dansi_sgr_setRaw_static("9" dansi_color_Std_staticParse(_color_tok))
#define ____dansi_xterm_color_bgBright_static(_color_tok) \
    dansi_sgr_setRaw_static("10" dansi_color_Std_staticParse(_color_tok))
#define ____dansi_xterm_color_fg8bit_static(_index_tok) \
    dansi_sgr_setRaw_static("38;5;" dansi_xterm_Palette8bit_staticParse(_index_tok))
#define ____dansi_xterm_color_bg8bit_static(_index_tok) \
    dansi_sgr_setRaw_static("48;5;" dansi_xterm_Palette8bit_staticParse(_index_tok))
#define ____dansi_xterm_color_fgRGB_static(_r_tok, _g_tok, _b_tok) \
    dansi_sgr_setRaw_static("38;2;" _r_tok ";" _g_tok ";" _b_tok)
#define ____dansi_xterm_color_bgRGB_static(_r_tok, _g_tok, _b_tok) \
    dansi_sgr_setRaw_static("48;2;" _r_tok ";" _g_tok ";" _b_tok)

#define dansi_xterm_color__pp_4bit_layer_fg 0
#define dansi_xterm_color__pp_4bit_layer_bg 1

#define dansi_xterm_color__parsePalette4bit(_pp_4bit_layer, _tok_palette_color) \
    ____dansi_xterm_color__parsePalette4bit(_pp_4bit_layer, _tok_palette_color)

#define ____dansi_xterm_color__pp_4bit_layer_pair_low ("3", "4")
#define ____dansi_xterm_color__pp_4bit_layer_pair_high ("9", "10")

#define ____dansi_xterm_color__parsePalette4bit__expand(...) __VA_ARGS__
#define ____dansi_xterm_color__parsePalette4bit(_pp_4bit_layer, _tok_palette_color...) \
    ____dansi_xterm_color__parsePalette4bit__expand( \
        ____dansi_xterm_color__parsePalette4bit_layer(_pp_4bit_layer) \
            ____dansi_xterm_color__parsePalette4bit_cmd( \
                pp_Tok_if_(pp_Tok_hasCmp(_tok_palette_color))( \
                    pp_Tok_then_(_tok_palette_color), \
                    pp_Tok_else_(pp_join($, ____dansi_xterm_color__pp_4bit_enum, _tok_palette_color)) \
                ) \
            ) \
    )
#define ____dansi_xterm_color__parsePalette4bit_layer__expand(...) __VA_ARGS__
#define ____dansi_xterm_color__parsePalette4bit_layer(_pp_4bit_layer...) \
    ____dansi_xterm_color__parsePalette4bit_layer__expand( \
        pp_switch_ pp_begin(_pp_4bit_layer)( \
            pp_case_((dansi_xterm_color__pp_4bit_layer_fg)(pp_Tuple_get1st)), \
            pp_case_((dansi_xterm_color__pp_4bit_layer_bg)(pp_Tuple_get2nd)) \
        ) pp_end \
    )
#define ____dansi_xterm_color__parsePalette4bit_cmd__expand(...) __VA_ARGS__
#define ____dansi_xterm_color__parsePalette4bit_cmd(_tok_palette_color...) \
    ____dansi_xterm_color__parsePalette4bit_cmd__expand( \
        pp_switch_ pp_begin(_tok_palette_color)( \
            pp_case_((0)(____dansi_xterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((1)(____dansi_xterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((2)(____dansi_xterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((3)(____dansi_xterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((4)(____dansi_xterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((5)(____dansi_xterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((6)(____dansi_xterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((7)(____dansi_xterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((8)(____dansi_xterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((9)(____dansi_xterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((10)(____dansi_xterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((11)(____dansi_xterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((12)(____dansi_xterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((13)(____dansi_xterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((14)(____dansi_xterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((15)(____dansi_xterm_color__pp_4bit_layer_pair_high)) \
        ) pp_end \
        pp_switch_ pp_begin(_tok_palette_color)( \
            pp_case_((0)("0")), \
            pp_case_((1)("1")), \
            pp_case_((2)("2")), \
            pp_case_((3)("3")), \
            pp_case_((4)("4")), \
            pp_case_((5)("5")), \
            pp_case_((6)("6")), \
            pp_case_((7)("7")), \
            pp_case_((8)("0")), \
            pp_case_((9)("1")), \
            pp_case_((10)("2")), \
            pp_case_((11)("3")), \
            pp_case_((12)("4")), \
            pp_case_((13)("5")), \
            pp_case_((14)("6")), \
            pp_case_((15)("7")) \
        ) pp_end \
    )

#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_black 0
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_maroon 1
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_green 2
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_olive 3
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_navy 4
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_purple 5
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_teal 6
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_silver 7
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_grey 8
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_red 9
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_lime 10
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_yellow 11
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_blue 12
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_fuchsia 13
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_aqua 14
#define ____dansi_xterm_color__pp_4bit_enum$dansi_xterm_Palette4bit_white 15

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_color__included */

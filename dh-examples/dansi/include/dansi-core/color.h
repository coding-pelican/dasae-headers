/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    color.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-21 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_color
 *
 * @brief   Terminal color support (256-color and RGB)
 * @details Reference: https://www.ditig.com/256-colors-cheat-sheet
 */
#ifndef dansi_color__included
#define dansi_color__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Palette4bit.h"
#include "Palette8bit.h"
#include <dh/io/Writer.h>

/*========== Macros and Declarations ========================================*/

claim_assert_static(as$(u8)(dansi_Palette4bit_black) == as$(u8)(dansi_Palette8bit_black));
claim_assert_static(as$(u8)(dansi_Palette4bit_maroon) == as$(u8)(dansi_Palette8bit_maroon));
claim_assert_static(as$(u8)(dansi_Palette4bit_green) == as$(u8)(dansi_Palette8bit_green));
claim_assert_static(as$(u8)(dansi_Palette4bit_olive) == as$(u8)(dansi_Palette8bit_olive));
claim_assert_static(as$(u8)(dansi_Palette4bit_navy) == as$(u8)(dansi_Palette8bit_navy));
claim_assert_static(as$(u8)(dansi_Palette4bit_purple) == as$(u8)(dansi_Palette8bit_purple));
claim_assert_static(as$(u8)(dansi_Palette4bit_teal) == as$(u8)(dansi_Palette8bit_teal));
claim_assert_static(as$(u8)(dansi_Palette4bit_silver) == as$(u8)(dansi_Palette8bit_silver));
claim_assert_static(as$(u8)(dansi_Palette4bit_grey) == as$(u8)(dansi_Palette8bit_grey));
claim_assert_static(as$(u8)(dansi_Palette4bit_red) == as$(u8)(dansi_Palette8bit_red));
claim_assert_static(as$(u8)(dansi_Palette4bit_lime) == as$(u8)(dansi_Palette8bit_lime));
claim_assert_static(as$(u8)(dansi_Palette4bit_yellow) == as$(u8)(dansi_Palette8bit_yellow));
claim_assert_static(as$(u8)(dansi_Palette4bit_blue) == as$(u8)(dansi_Palette8bit_blue));
claim_assert_static(as$(u8)(dansi_Palette4bit_fuchsia) == as$(u8)(dansi_Palette8bit_fuchsia));
claim_assert_static(as$(u8)(dansi_Palette4bit_aqua) == as$(u8)(dansi_Palette8bit_aqua));
claim_assert_static(as$(u8)(dansi_Palette4bit_white) == as$(u8)(dansi_Palette8bit_white));

typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_Color_Palette4bitBuf;

#define dansi_color_fg4bit_staticParse(_tok) \
    ____dansi_color_fg4bit_staticParse(_tok)
#define dansi_color_fg4bit_static(_color_tok) \
    ____dansi_color_fg4bit_static(_color_tok)
/// Get ANSI sequence for 16-color foreground
$extern fn_((dansi_color_fg4bit(dansi_Palette4bit color, dansi_Color_Palette4bitBuf* buf))(S$u8));
/// Write ANSI sequence for 16-color foreground
$attr($must_check)
$extern fn_((dansi_color_fg4bitWrite(dansi_Palette4bit color, io_Writer writer))(E$void));
#define dansi_color_bg4bit_staticParse(_tok) \
    ____dansi_color_bg4bit_staticParse(_tok)
#define dansi_color_bg4bit_static(_color_tok) \
    ____dansi_color_bg4bit_static(_color_tok)
/// Get ANSI sequence for 16-color background
$extern fn_((dansi_color_bg4bit(dansi_Palette4bit color, dansi_Color_Palette4bitBuf* buf))(S$u8));
/// Write ANSI sequence for 16-color background
$attr($must_check)
$extern fn_((dansi_color_bg4bitWrite(dansi_Palette4bit color, io_Writer writer))(E$void));

typedef A$$(8 + uint_log10Ceil_static(u8_limit_max), u8) dansi_Color_Palette8bitBuf;

#define dansi_color_fg8bit_staticParse(_tok) \
    ____dansi_color_fg8bit_staticParse(_tok)
#define dansi_color_fg8bit_static(_color_tok) \
    ____dansi_color_fg8bit_static(_color_tok)
/// Get ANSI sequence for 256-color foreground
$extern fn_((dansi_color_fg8bit(dansi_Palette8bit color, dansi_Color_Palette8bitBuf* buf))(S$u8));
/// Write ANSI sequence for 256-color foreground
$attr($must_check)
$extern fn_((dansi_color_fg8bitWrite(dansi_Palette8bit color, io_Writer writer))(E$void));
#define dansi_color_bg8bit_staticParse(_tok) \
    ____dansi_color_bg8bit_staticParse(_tok)
#define dansi_color_bg8bit_static(_color_tok) \
    ____dansi_color_bg8bit_static(_color_tok)
/// Get ANSI sequence for 256-color background
$extern fn_((dansi_color_bg8bit(dansi_Palette8bit color, dansi_Color_Palette8bitBuf* buf))(S$u8));
/// Write ANSI sequence for 256-color background
$attr($must_check)
$extern fn_((dansi_color_bg8bitWrite(dansi_Palette8bit color, io_Writer writer))(E$void));

typedef A$$(10 + uint_log10Ceil_static(u8_limit_max) * 3, u8) dansi_Color_RGB24bitBuf;

#define dansi_color_fg24bit_staticParse(_chan_tok) \
    ____dansi_color_fg24bit_staticParse(_chan_tok)
#define dansi_color_fg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok) \
    ____dansi_color_fg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok)
/// Get ANSI sequence for RGB foreground color
$extern fn_((dansi_color_fg24bit(u8 r, u8 g, u8 b, dansi_Color_RGB24bitBuf* buf))(S$u8));
/// Write ANSI sequence for RGB foreground color
$attr($must_check)
$extern fn_((dansi_color_fg24bitWrite(u8 r, u8 g, u8 b, io_Writer writer))(E$void));
#define dansi_color_bg24bit_staticParse(_chan_tok) \
    ____dansi_color_bg24bit_staticParse(_chan_tok)
#define dansi_color_bg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok) \
    ____dansi_color_bg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok)
/// Get ANSI sequence for RGB background color
$extern fn_((dansi_color_bg24bit(u8 r, u8 g, u8 b, dansi_Color_RGB24bitBuf* buf))(S$u8));
/// Write ANSI sequence for RGB background color
$attr($must_check)
$extern fn_((dansi_color_bg24bitWrite(u8 r, u8 g, u8 b, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

/*========== 4-bit color ==========*/

/* --- Internal Declarations --- */

#define dansi_color__pp_4bit_layer_fg 0
#define dansi_color__pp_4bit_layer_bg 1

#define dansi_color__parsePalette4bit(_pp_4bit_layer, _tok_palette_color) \
    ____dansi_color__parsePalette4bit(_pp_4bit_layer, _tok_palette_color)
#define dansi_color__formatPalette4bit(_color_tok) \
    ____dansi_color__formatPalette4bit(_color_tok)

/* --- External Definitions --- */

#define ____dansi_color_fg4bit_staticParse(_tok) \
    dansi_color__parsePalette4bit(dansi_color__pp_4bit_layer_fg, _tok)
#if DEPRECATED_CODE
#define ____dansi_color_fg4bit_staticParse(_color_tok) \
    pp_if_(pp_Tok_isComparable(_color_tok))( \
        pp_then_(pp_expand( \
            pp_switch_ pp_begin(_color_tok)( \
                pp_case_((0)("30")), \
                pp_case_((1)("31")), \
                pp_case_((2)("32")), \
                pp_case_((3)("33")), \
                pp_case_((4)("34")), \
                pp_case_((5)("35")), \
                pp_case_((6)("36")), \
                pp_case_((7)("37")), \
                pp_case_((8)("90")), \
                pp_case_((9)("91")), \
                pp_case_((10)("92")), \
                pp_case_((11)("93")), \
                pp_case_((12)("94")), \
                pp_case_((13)("95")), \
                pp_case_((14)("96")), \
                pp_case_((15)("97")) \
            ) pp_end \
        )), \
        pp_else_(pp_join($, ____dansi_color_fg4bit__pp_enum, _color_tok)) \
    )
#endif /* DEPRECATED_CODE */
#define ____dansi_color_fg4bit_static(_color_tok) \
    dansi_color__formatPalette4bit(_color_tok)

#define ____dansi_color_bg4bit_staticParse(_color_tok) \
    dansi_color__parsePalette4bit(dansi_color__pp_4bit_layer_bg, _color_tok)
#if DEPRECATED_CODE
#define ____dansi_color_bg4bit_staticParse(_color_tok) \
    pp_if_(pp_Tok_isComparable(_color_tok))( \
        pp_then_(pp_expand( \
            pp_switch_ pp_begin(_color_tok)( \
                pp_case_((0)("40")), \
                pp_case_((1)("41")), \
                pp_case_((2)("42")), \
                pp_case_((3)("43")), \
                pp_case_((4)("44")), \
                pp_case_((5)("45")), \
                pp_case_((6)("46")), \
                pp_case_((7)("47")), \
                pp_case_((8)("100")), \
                pp_case_((9)("101")), \
                pp_case_((10)("102")), \
                pp_case_((11)("103")), \
                pp_case_((12)("104")), \
                pp_case_((13)("105")), \
                pp_case_((14)("106")), \
                pp_case_((15)("107")) \
            ) pp_end \
        )), \
        pp_else_(pp_join($, ____dansi_color_bg4bit__pp_enum, _color_tok)) \
    )
#endif /* DEPRECATED_CODE */
#define ____dansi_color_bg4bit_static(_color_tok) \
    dansi_color__formatPalette4bit(_color_tok)

/* --- Internal Definitions --- */

#define ____dansi_color__pp_4bit_layer_pair_low ("3", "4")
#define ____dansi_color__pp_4bit_layer_pair_high ("9", "10")

#define ____dansi_color__parsePalette4bit__expand(...) __VA_ARGS__
#define ____dansi_color__parsePalette4bit(_pp_4bit_layer, _tok_palette_color...) \
    ____dansi_color__parsePalette4bit__expand( \
        ____dansi_color__parsePalette4bit_layer(_pp_4bit_layer) \
            ____dansi_color__parsePalette4bit_cmd( \
                pp_Tok_if_(pp_Tok_hasCmp(_tok_palette_color))( \
                    pp_Tok_then_(_tok_palette_color), \
                    pp_Tok_else_(pp_join($, ____dansi_color__pp_4bit_enum, _tok_palette_color)) \
                ) \
            ) \
    )
#define ____dansi_color__parsePalette4bit_layer__expand(...) __VA_ARGS__
#define ____dansi_color__parsePalette4bit_layer(_pp_4bit_layer...) \
    ____dansi_color__parsePalette4bit_layer__expand( \
        pp_switch_ pp_begin(_pp_4bit_layer)( \
            pp_case_((dansi_color__pp_4bit_layer_fg)(pp_Tuple_get1st)), \
            pp_case_((dansi_color__pp_4bit_layer_bg)(pp_Tuple_get2nd)) \
        ) pp_end \
    )
#define ____dansi_color__parsePalette4bit_cmd__expand(...) __VA_ARGS__
#define ____dansi_color__parsePalette4bit_cmd(_tok_palette_color...) /* clang-format off */ \
    ____dansi_color__parsePalette4bit_cmd__expand( \
        pp_switch_ pp_begin(_tok_palette_color)( \
            pp_case_((0)(____dansi_color__pp_4bit_layer_pair_low)), \
            pp_case_((1)(____dansi_color__pp_4bit_layer_pair_low)), \
            pp_case_((2)(____dansi_color__pp_4bit_layer_pair_low)), \
            pp_case_((3)(____dansi_color__pp_4bit_layer_pair_low)), \
            pp_case_((4)(____dansi_color__pp_4bit_layer_pair_low)), \
            pp_case_((5)(____dansi_color__pp_4bit_layer_pair_low)), \
            pp_case_((6)(____dansi_color__pp_4bit_layer_pair_low)), \
            pp_case_((7)(____dansi_color__pp_4bit_layer_pair_low)), \
            pp_case_((8)(____dansi_color__pp_4bit_layer_pair_high)), \
            pp_case_((9)(____dansi_color__pp_4bit_layer_pair_high)), \
            pp_case_((10)(____dansi_color__pp_4bit_layer_pair_high)), \
            pp_case_((11)(____dansi_color__pp_4bit_layer_pair_high)), \
            pp_case_((12)(____dansi_color__pp_4bit_layer_pair_high)), \
            pp_case_((13)(____dansi_color__pp_4bit_layer_pair_high)), \
            pp_case_((14)(____dansi_color__pp_4bit_layer_pair_high)), \
            pp_case_((15)(____dansi_color__pp_4bit_layer_pair_high)) \
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
    ) /* clang-format on */

#define ____dansi_color__formatPalette4bit(_color_tok) \
    dansi_utils_csi _color_tok dansi_utils_attr

#pragma region "pp 4-bit color enum"
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_black 0
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_maroon 1
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_green 2
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_olive 3
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_navy 4
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_purple 5
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_teal 6
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_silver 7
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_grey 8
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_red 9
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_lime 10
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_yellow 11
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_blue 12
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_fuchsia 13
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_aqua 14
#define ____dansi_color__pp_4bit_enum$dansi_Palette4bit_white 15
#pragma endregion "pp 4-bit color enum"

/*========== 8-bit color ==========*/

/* --- Internal Declarations --- */

#define dansi_color__pp_8bit_layer_fg 0
#define dansi_color__pp_8bit_layer_bg 1

#define dansi_color__parsePalette8bit(_tok_palette_color) \
    ____dansi_color__parsePalette8bit(_tok_palette_color)
#define dansi_color__formatPalette8bit(_pp_8bit_layer, _color_tok) \
    ____dansi_color__formatPalette8bit(_pp_8bit_layer, _color_tok)

/* --- External Definitions --- */

#define ____dansi_color_fg8bit_staticParse(_tok) \
    dansi_color__parsePalette8bit(_tok)
#define ____dansi_color_fg8bit_static(_color_tok) \
    dansi_color__formatPalette8bit(dansi_color__pp_8bit_layer_fg, _color_tok)

#define ____dansi_color_bg8bit_staticParse(_tok) \
    dansi_color__parsePalette8bit(_tok)
#define ____dansi_color_bg8bit_static(_color_tok) \
    dansi_color__formatPalette8bit(dansi_color__pp_8bit_layer_bg, _color_tok)

/* --- Internal Definitions --- */

#define ____dansi_color__parsePalette8bit(_tok_palette_color) \
    pp_Tok_if_(pp_Tok_hasCmp(_tok_palette_color))( \
        pp_Tok_then_(#_tok_palette_color), \
        pp_Tok_else_(pp_join($, ____dansi_color__pp_8bit_str, _tok_palette_color)) \
    )

#define ____dansi_color__formatPalette8bit__expand(...) __VA_ARGS__
#define ____dansi_color__formatPalette8bit(_pp_8bit_layer, _color_tok) \
    ____dansi_color__formatPalette8bit__expand( \
        dansi_utils_csi pp_switch_ pp_begin(_pp_8bit_layer)( \
            pp_case_((dansi_color__pp_8bit_layer_fg)(dansi_utils_attr_color_fg_8bit)), \
            pp_case_((dansi_color__pp_8bit_layer_bg)(dansi_utils_attr_color_bg_8bit)) \
        ) pp_end _color_tok dansi_utils_attr \
    )

#pragma region "pp 8-bit color str"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_black "0"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_maroon "1"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_green "2"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_olive "3"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_navy "4"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_purple "5"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_teal "6"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_silver "7"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey "8"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_red "9"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_lime "10"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_yellow "11"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_blue "12"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_fuchsia "13"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_aqua "14"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_white "15"

#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_0 "16"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_navy_blue "17"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_blue "18"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_blue_3 "19"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_blue_3b "20"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_blue_1 "21"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_green "22"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_sky_blue_4 "23"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_sky_blue_4b "24"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_sky_blue_4c "25"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dodger_blue_3 "26"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dodger_blue_2 "27"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_green_4 "28"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_spring_green_4 "29"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_turquoise_4 "30"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_sky_blue_3 "31"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_sky_blue_3b "32"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dodger_blue_1 "33"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_green_3 "34"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_spring_green_3 "35"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_cyan "36"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_sea_green "37"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_sky_blue_2 "38"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_sky_blue_1 "39"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_green_3_2 "40"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_spring_green_3b "41"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_spring_green_2 "42"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_cyan_3 "43"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_turquoise "44"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_turquoise_2 "45"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_green_1 "46"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_spring_green_2b "47"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_spring_green_1 "48"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_spring_green "49"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_cyan_2 "50"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_cyan_1 "51"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_red "52"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_pink_4 "53"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_purple_4 "54"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_purple_4b "55"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_purple_3 "56"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_blue_violet "57"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_orange_4 "58"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_37 "59"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_purple_4 "60"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_slate_blue_3 "61"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_slate_blue_3b "62"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_royal_blue_1 "63"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_chartreuse_4 "64"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_sea_green_4 "65"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_pale_turquoise_4 "66"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_steel_blue "67"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_steel_blue_3 "68"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_cornflower_blue "69"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_chartreuse_3 "70"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_sea_green_4b "71"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_cadet_blue "72"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_cadet_blue_2 "73"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_sky_blue_3 "74"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_steel_blue_1 "75"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_chartreuse_3b "76"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_pale_green_3 "77"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_sea_green_3 "78"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_aquamarine_3 "79"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_turquoise "80"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_steel_blue_1b "81"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_chartreuse_2 "82"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_sea_green_2 "83"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_sea_green_1 "84"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_sea_green_1b "85"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_aquamarine_1 "86"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_slate_gray_2 "87"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_red_2 "88"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_pink_4b "89"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_magenta "90"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_magenta_2 "91"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_violet "92"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_purple_2 "93"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_orange_4b "94"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_pink_4 "95"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_plum_4 "96"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_purple_3 "97"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_purple_3b "98"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_slate_blue_1b "99"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_yellow_4 "100"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_wheat_4 "101"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_53 "102"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_slate_grey "103"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_purple "104"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_slate_blue "105"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_yellow_4b "106"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_olive_green_3 "107"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_sea_green "108"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_sky_blue_3 "109"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_sky_blue_3b "110"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_sky_blue_2 "111"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_chartreuse_2b "112"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_olive_green_3b "113"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_pale_green_3b "114"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_sea_green_3 "115"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_slate_gray_3 "116"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_sky_blue_1 "117"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_chartreuse_1 "118"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_green "119"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_green_2 "120"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_pale_green_1 "121"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_aquamarine_1b "122"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_slate_gray_1 "123"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_red_3 "124"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_pink_4c "125"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_violet_red "126"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_magenta_3 "127"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_violet_2 "128"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_purple_3b "129"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_orange_3 "130"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_indian_red "131"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_hot_pink_3 "132"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_orchid_3 "133"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_orchid "134"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_purple_2 "135"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_goldenrod "136"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_salmon_3 "137"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_rosy_brown "138"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_63 "139"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_purple_2b "140"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_purple_1 "141"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_gold_3 "142"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_khaki "143"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_navajo_white_3 "144"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_69 "145"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_steel_blue_3 "146"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_steel_blue "147"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_yellow_3 "148"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_bark_olive_green_3c "149"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_sea_green_3b "150"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_sea_green_2 "151"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_cyan_3 "152"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_sky_blue_1 "153"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_green_yellow "154"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_olive_green_2 "155"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_pale_green_1b "156"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_sea_green_2b "157"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_sea_green_1 "158"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_pale_turquoise_1 "159"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_red_3b "160"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_pink_3 "161"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_pink_3b "162"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_magenta_3b "163"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_magenta_3c "164"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_magenta_2 "165"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_orange_3b "166"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_indian_red_2 "167"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_hot_pink_3b "168"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_hot_pink_2 "169"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_orchid "170"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_orchid_1 "171"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_orange_3 "172"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_salmon_3b "173"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_pink_3 "174"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_pink_3 "175"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_plum_3 "176"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_violet "177"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_gold_3b "178"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_goldenrod_3 "179"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_tan "180"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_misty_rose_3 "181"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_thistle_3 "182"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_plum_2 "183"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_yellow_3b "184"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_khaki_3 "185"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_goldenrod_2 "186"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_yellow_3 "187"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_84 "188"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_steel_blue_1 "189"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_yellow_2 "190"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_olive_green_1 "191"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_olive_green_1b "192"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_sea_green_1b "193"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_honeydew_2 "194"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_cyan_1 "195"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_red_1 "196"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_pink_2 "197"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_pink_1 "198"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_deep_pink_1b "199"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_magenta_2b "200"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_magenta_1 "201"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_orange_red_1 "202"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_indian_red_1 "203"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_indian_red_1b "204"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_hot_pink "205"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_hot_pink_2b "206"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_medium_orchid_1b "207"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_dark_orange "208"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_salmon_1 "209"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_coral "210"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_pale_violet_red_1 "211"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_orchid_2 "212"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_orchid_1 "213"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_orange_1 "214"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_sandy_brown "215"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_salmon_1 "216"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_pink_1 "217"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_pink_1 "218"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_plum_1 "219"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_gold_1 "220"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_goldenrod_2b "221"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_goldenrod_2c "222"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_navajo_white_1 "223"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_misty_rose_1 "224"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_thistle_1 "225"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_yellow_1 "226"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_light_goldenrod_1 "227"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_khaki_1 "228"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_wheat_1 "229"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_cornsilk_1 "230"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_100 "231"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_3 "232"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_7 "233"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_11 "234"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_15 "235"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_19 "236"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_23 "237"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_27 "238"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_30 "239"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_35 "240"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_39 "241"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_42 "242"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_46 "243"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_50 "244"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_54 "245"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_58 "246"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_62 "247"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_66 "248"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_70 "249"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_74 "250"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_78 "251"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_82 "252"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_85 "253"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_89 "254"
#define ____dansi_color__pp_8bit_str$dansi_Palette8bit_grey_93 "255"
#pragma endregion "pp 8-bit color str"

/*========== 24-bit color ==========*/

/* --- Internal Declarations --- */

#define dansi_color__pp_24bit_layer_fg 0
#define dansi_color__pp_24bit_layer_bg 1

#define dansi_color__parseRGB24bit(_channel_tok) \
    ____dansi_color__parseRGB24bit(_channel_tok)
#define dansi_color__formatRGB24bit(_pp_24bit_layer, _chan_r_tok, _chan_g_tok, _chan_b_tok) \
    ____dansi_color__formatRGB24bit(_pp_24bit_layer, _chan_r_tok, _chan_g_tok, _chan_b_tok)

/* --- External Definitions --- */

#define ____dansi_color_fg24bit_staticParse(_chan_tok) \
    dansi_color__parseRGB24bit(_chan_tok)
#define ____dansi_color_fg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok) \
    dansi_color__formatRGB24bit(dansi_color__pp_24bit_layer_fg, _chan_r_tok, _chan_g_tok, _chan_b_tok)
#define ____dansi_color_bg24bit_staticParse(_chan_tok) \
    dansi_color__parseRGB24bit(_chan_tok)
#define ____dansi_color_bg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok) \
    dansi_color__formatRGB24bit(dansi_color__pp_24bit_layer_bg, _chan_r_tok, _chan_g_tok, _chan_b_tok)

/* --- Internal Definitions --- */

#define ____dansi_color__parseRGB24bit(_channel_tok) \
    #_channel_tok

#define ____dansi_color__formatRGB24bit__expand(...) __VA_ARGS__
#define ____dansi_color__formatRGB24bit(_pp_24bit_layer, _chan_r_tok, _chan_g_tok, _chan_b_tok) \
    ____dansi_color__formatRGB24bit__expand( \
        dansi_utils_csi pp_switch_ pp_begin(_pp_24bit_layer)( \
            pp_case_((dansi_color__pp_24bit_layer_fg)(dansi_utils_attr_color_fg_24bit)), \
            pp_case_((dansi_color__pp_24bit_layer_bg)(dansi_utils_attr_color_bg_24bit)) \
        ) pp_end _chan_r_tok dansi_utils_sep _chan_g_tok dansi_utils_sep _chan_b_tok dansi_utils_attr \
    )

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_color__included */

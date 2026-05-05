/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    color.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-21 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_color
 *
 * @brief   Terminal color support (256-color and RGB)
 * @details Reference: https://www.ditig.com/256-colors-cheat-sheet
 */
#ifndef daterm_color__included
#define daterm_color__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Palette4bit.h"
#include "Palette8bit.h"
#include <dh/io/Writer.h>

/*========== Macros and Declarations ========================================*/

claim_assert_static(as$(u8)(daterm_Palette4bit_black) == as$(u8)(daterm_Palette8bit_black));
claim_assert_static(as$(u8)(daterm_Palette4bit_maroon) == as$(u8)(daterm_Palette8bit_maroon));
claim_assert_static(as$(u8)(daterm_Palette4bit_green) == as$(u8)(daterm_Palette8bit_green));
claim_assert_static(as$(u8)(daterm_Palette4bit_olive) == as$(u8)(daterm_Palette8bit_olive));
claim_assert_static(as$(u8)(daterm_Palette4bit_navy) == as$(u8)(daterm_Palette8bit_navy));
claim_assert_static(as$(u8)(daterm_Palette4bit_purple) == as$(u8)(daterm_Palette8bit_purple));
claim_assert_static(as$(u8)(daterm_Palette4bit_teal) == as$(u8)(daterm_Palette8bit_teal));
claim_assert_static(as$(u8)(daterm_Palette4bit_silver) == as$(u8)(daterm_Palette8bit_silver));
claim_assert_static(as$(u8)(daterm_Palette4bit_grey) == as$(u8)(daterm_Palette8bit_grey));
claim_assert_static(as$(u8)(daterm_Palette4bit_red) == as$(u8)(daterm_Palette8bit_red));
claim_assert_static(as$(u8)(daterm_Palette4bit_lime) == as$(u8)(daterm_Palette8bit_lime));
claim_assert_static(as$(u8)(daterm_Palette4bit_yellow) == as$(u8)(daterm_Palette8bit_yellow));
claim_assert_static(as$(u8)(daterm_Palette4bit_blue) == as$(u8)(daterm_Palette8bit_blue));
claim_assert_static(as$(u8)(daterm_Palette4bit_fuchsia) == as$(u8)(daterm_Palette8bit_fuchsia));
claim_assert_static(as$(u8)(daterm_Palette4bit_aqua) == as$(u8)(daterm_Palette8bit_aqua));
claim_assert_static(as$(u8)(daterm_Palette4bit_white) == as$(u8)(daterm_Palette8bit_white));

typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) daterm_Color_Palette4bitBuf;

#define daterm_color_fg4bit_staticParse(_tok) \
    ____daterm_color_fg4bit_staticParse(_tok)
#define daterm_color_fg4bit_static(_color_tok) \
    ____daterm_color_fg4bit_static(_color_tok)
/// Get ANSI sequence for 16-color foreground
$extern fn_((daterm_color_fg4bit(daterm_Palette4bit color, daterm_Color_Palette4bitBuf* buf))(S$u8));
/// Write ANSI sequence for 16-color foreground
$attr($must_check)
$extern fn_((daterm_color_fg4bitWrite(daterm_Palette4bit color, io_Writer writer))(E$void));
#define daterm_color_bg4bit_staticParse(_tok) \
    ____daterm_color_bg4bit_staticParse(_tok)
#define daterm_color_bg4bit_static(_color_tok) \
    ____daterm_color_bg4bit_static(_color_tok)
/// Get ANSI sequence for 16-color background
$extern fn_((daterm_color_bg4bit(daterm_Palette4bit color, daterm_Color_Palette4bitBuf* buf))(S$u8));
/// Write ANSI sequence for 16-color background
$attr($must_check)
$extern fn_((daterm_color_bg4bitWrite(daterm_Palette4bit color, io_Writer writer))(E$void));

typedef A$$(8 + uint_log10Ceil_static(u8_limit_max), u8) daterm_Color_Palette8bitBuf;

#define daterm_color_fg8bit_staticParse(_tok) \
    ____daterm_color_fg8bit_staticParse(_tok)
#define daterm_color_fg8bit_static(_color_tok) \
    ____daterm_color_fg8bit_static(_color_tok)
/// Get ANSI sequence for 256-color foreground
$extern fn_((daterm_color_fg8bit(daterm_Palette8bit color, daterm_Color_Palette8bitBuf* buf))(S$u8));
/// Write ANSI sequence for 256-color foreground
$attr($must_check)
$extern fn_((daterm_color_fg8bitWrite(daterm_Palette8bit color, io_Writer writer))(E$void));
#define daterm_color_bg8bit_staticParse(_tok) \
    ____daterm_color_bg8bit_staticParse(_tok)
#define daterm_color_bg8bit_static(_color_tok) \
    ____daterm_color_bg8bit_static(_color_tok)
/// Get ANSI sequence for 256-color background
$extern fn_((daterm_color_bg8bit(daterm_Palette8bit color, daterm_Color_Palette8bitBuf* buf))(S$u8));
/// Write ANSI sequence for 256-color background
$attr($must_check)
$extern fn_((daterm_color_bg8bitWrite(daterm_Palette8bit color, io_Writer writer))(E$void));

typedef A$$(10 + uint_log10Ceil_static(u8_limit_max) * 3, u8) daterm_Color_RGB24bitBuf;

#define daterm_color_fg24bit_staticParse(_chan_tok) \
    ____daterm_color_fg24bit_staticParse(_chan_tok)
#define daterm_color_fg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok) \
    ____daterm_color_fg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok)
/// Get ANSI sequence for RGB foreground color
$extern fn_((daterm_color_fg24bit(u8 r, u8 g, u8 b, daterm_Color_RGB24bitBuf* buf))(S$u8));
/// Write ANSI sequence for RGB foreground color
$attr($must_check)
$extern fn_((daterm_color_fg24bitWrite(u8 r, u8 g, u8 b, io_Writer writer))(E$void));
#define daterm_color_bg24bit_staticParse(_chan_tok) \
    ____daterm_color_bg24bit_staticParse(_chan_tok)
#define daterm_color_bg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok) \
    ____daterm_color_bg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok)
/// Get ANSI sequence for RGB background color
$extern fn_((daterm_color_bg24bit(u8 r, u8 g, u8 b, daterm_Color_RGB24bitBuf* buf))(S$u8));
/// Write ANSI sequence for RGB background color
$attr($must_check)
$extern fn_((daterm_color_bg24bitWrite(u8 r, u8 g, u8 b, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "daterm/utils.h"

/*========== 4-bit color ==========*/

/* --- Internal Declarations --- */

#define daterm_color__pp_4bit_layer_fg 0
#define daterm_color__pp_4bit_layer_bg 1

#define daterm_color__parsePalette4bit(_pp_4bit_layer, _tok_palette_color) \
    ____daterm_color__parsePalette4bit(_pp_4bit_layer, _tok_palette_color)
#define daterm_color__formatPalette4bit(_color_tok) \
    ____daterm_color__formatPalette4bit(_color_tok)

/* --- External Definitions --- */

#define ____daterm_color_fg4bit_staticParse(_tok) \
    daterm_color__parsePalette4bit(daterm_color__pp_4bit_layer_fg, _tok)
#if DEPRECATED_CODE
#define ____daterm_color_fg4bit_staticParse(_color_tok) \
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
        pp_else_(pp_join($, ____daterm_color_fg4bit__pp_enum, _color_tok)) \
    )
#endif /* DEPRECATED_CODE */
#define ____daterm_color_fg4bit_static(_color_tok) \
    daterm_color__formatPalette4bit(_color_tok)

#define ____daterm_color_bg4bit_staticParse(_color_tok) \
    daterm_color__parsePalette4bit(daterm_color__pp_4bit_layer_bg, _color_tok)
#if DEPRECATED_CODE
#define ____daterm_color_bg4bit_staticParse(_color_tok) \
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
        pp_else_(pp_join($, ____daterm_color_bg4bit__pp_enum, _color_tok)) \
    )
#endif /* DEPRECATED_CODE */
#define ____daterm_color_bg4bit_static(_color_tok) \
    daterm_color__formatPalette4bit(_color_tok)

/* --- Internal Definitions --- */

#define ____daterm_color__pp_4bit_layer_pair_low ("3", "4")
#define ____daterm_color__pp_4bit_layer_pair_high ("9", "10")

#define ____daterm_color__parsePalette4bit__expand(...) __VA_ARGS__
#define ____daterm_color__parsePalette4bit(_pp_4bit_layer, _tok_palette_color...) \
    ____daterm_color__parsePalette4bit__expand( \
        ____daterm_color__parsePalette4bit_layer(_pp_4bit_layer) \
            ____daterm_color__parsePalette4bit_cmd( \
                pp_Tok_if_(pp_Tok_hasCmp(_tok_palette_color))( \
                    pp_Tok_then_(_tok_palette_color), \
                    pp_Tok_else_(pp_join($, ____daterm_color__pp_4bit_enum, _tok_palette_color)) \
                ) \
            ) \
    )
#define ____daterm_color__parsePalette4bit_layer__expand(...) __VA_ARGS__
#define ____daterm_color__parsePalette4bit_layer(_pp_4bit_layer...) \
    ____daterm_color__parsePalette4bit_layer__expand( \
        pp_switch_ pp_begin(_pp_4bit_layer)( \
            pp_case_((daterm_color__pp_4bit_layer_fg)(pp_Tuple_get1st)), \
            pp_case_((daterm_color__pp_4bit_layer_bg)(pp_Tuple_get2nd)) \
        ) pp_end \
    )
#define ____daterm_color__parsePalette4bit_cmd__expand(...) __VA_ARGS__
#define ____daterm_color__parsePalette4bit_cmd(_tok_palette_color...) /* clang-format off */ \
    ____daterm_color__parsePalette4bit_cmd__expand( \
        pp_switch_ pp_begin(_tok_palette_color)( \
            pp_case_((0)(____daterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((1)(____daterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((2)(____daterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((3)(____daterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((4)(____daterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((5)(____daterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((6)(____daterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((7)(____daterm_color__pp_4bit_layer_pair_low)), \
            pp_case_((8)(____daterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((9)(____daterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((10)(____daterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((11)(____daterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((12)(____daterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((13)(____daterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((14)(____daterm_color__pp_4bit_layer_pair_high)), \
            pp_case_((15)(____daterm_color__pp_4bit_layer_pair_high)) \
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

#define ____daterm_color__formatPalette4bit(_color_tok) \
    daterm_utils_csi _color_tok daterm_utils_attr

#pragma region "pp 4-bit color enum"
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_black 0
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_maroon 1
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_green 2
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_olive 3
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_navy 4
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_purple 5
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_teal 6
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_silver 7
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_grey 8
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_red 9
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_lime 10
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_yellow 11
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_blue 12
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_fuchsia 13
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_aqua 14
#define ____daterm_color__pp_4bit_enum$daterm_Palette4bit_white 15
#pragma endregion "pp 4-bit color enum"

/*========== 8-bit color ==========*/

/* --- Internal Declarations --- */

#define daterm_color__pp_8bit_layer_fg 0
#define daterm_color__pp_8bit_layer_bg 1

#define daterm_color__parsePalette8bit(_tok_palette_color) \
    ____daterm_color__parsePalette8bit(_tok_palette_color)
#define daterm_color__formatPalette8bit(_pp_8bit_layer, _color_tok) \
    ____daterm_color__formatPalette8bit(_pp_8bit_layer, _color_tok)

/* --- External Definitions --- */

#define ____daterm_color_fg8bit_staticParse(_tok) \
    daterm_color__parsePalette8bit(_tok)
#define ____daterm_color_fg8bit_static(_color_tok) \
    daterm_color__formatPalette8bit(daterm_color__pp_8bit_layer_fg, _color_tok)

#define ____daterm_color_bg8bit_staticParse(_tok) \
    daterm_color__parsePalette8bit(_tok)
#define ____daterm_color_bg8bit_static(_color_tok) \
    daterm_color__formatPalette8bit(daterm_color__pp_8bit_layer_bg, _color_tok)

/* --- Internal Definitions --- */

#define ____daterm_color__parsePalette8bit(_tok_palette_color) \
    pp_Tok_if_(pp_Tok_hasCmp(_tok_palette_color))( \
        pp_Tok_then_(#_tok_palette_color), \
        pp_Tok_else_(pp_join($, ____daterm_color__pp_8bit_str, _tok_palette_color)) \
    )

#define ____daterm_color__formatPalette8bit__expand(...) __VA_ARGS__
#define ____daterm_color__formatPalette8bit(_pp_8bit_layer, _color_tok) \
    ____daterm_color__formatPalette8bit__expand( \
        daterm_utils_csi pp_switch_ pp_begin(_pp_8bit_layer)( \
            pp_case_((daterm_color__pp_8bit_layer_fg)(daterm_utils_attr_color_fg_8bit)), \
            pp_case_((daterm_color__pp_8bit_layer_bg)(daterm_utils_attr_color_bg_8bit)) \
        ) pp_end _color_tok daterm_utils_attr \
    )

#pragma region "pp 8-bit color str"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_black "0"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_maroon "1"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_green "2"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_olive "3"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_navy "4"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_purple "5"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_teal "6"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_silver "7"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey "8"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_red "9"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_lime "10"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_yellow "11"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_blue "12"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_fuchsia "13"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_aqua "14"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_white "15"

#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_0 "16"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_navy_blue "17"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_blue "18"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_blue_3 "19"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_blue_3b "20"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_blue_1 "21"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_green "22"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_sky_blue_4 "23"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_sky_blue_4b "24"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_sky_blue_4c "25"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dodger_blue_3 "26"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dodger_blue_2 "27"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_green_4 "28"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_spring_green_4 "29"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_turquoise_4 "30"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_sky_blue_3 "31"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_sky_blue_3b "32"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dodger_blue_1 "33"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_green_3 "34"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_spring_green_3 "35"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_cyan "36"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_sea_green "37"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_sky_blue_2 "38"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_sky_blue_1 "39"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_green_3_2 "40"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_spring_green_3b "41"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_spring_green_2 "42"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_cyan_3 "43"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_turquoise "44"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_turquoise_2 "45"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_green_1 "46"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_spring_green_2b "47"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_spring_green_1 "48"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_spring_green "49"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_cyan_2 "50"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_cyan_1 "51"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_red "52"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_pink_4 "53"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_purple_4 "54"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_purple_4b "55"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_purple_3 "56"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_blue_violet "57"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_orange_4 "58"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_37 "59"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_purple_4 "60"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_slate_blue_3 "61"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_slate_blue_3b "62"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_royal_blue_1 "63"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_chartreuse_4 "64"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_sea_green_4 "65"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_pale_turquoise_4 "66"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_steel_blue "67"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_steel_blue_3 "68"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_cornflower_blue "69"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_chartreuse_3 "70"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_sea_green_4b "71"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_cadet_blue "72"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_cadet_blue_2 "73"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_sky_blue_3 "74"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_steel_blue_1 "75"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_chartreuse_3b "76"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_pale_green_3 "77"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_sea_green_3 "78"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_aquamarine_3 "79"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_turquoise "80"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_steel_blue_1b "81"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_chartreuse_2 "82"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_sea_green_2 "83"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_sea_green_1 "84"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_sea_green_1b "85"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_aquamarine_1 "86"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_slate_gray_2 "87"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_red_2 "88"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_pink_4b "89"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_magenta "90"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_magenta_2 "91"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_violet "92"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_purple_2 "93"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_orange_4b "94"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_pink_4 "95"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_plum_4 "96"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_purple_3 "97"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_purple_3b "98"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_slate_blue_1b "99"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_yellow_4 "100"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_wheat_4 "101"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_53 "102"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_slate_grey "103"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_purple "104"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_slate_blue "105"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_yellow_4b "106"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_olive_green_3 "107"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_sea_green "108"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_sky_blue_3 "109"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_sky_blue_3b "110"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_sky_blue_2 "111"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_chartreuse_2b "112"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_olive_green_3b "113"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_pale_green_3b "114"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_sea_green_3 "115"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_slate_gray_3 "116"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_sky_blue_1 "117"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_chartreuse_1 "118"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_green "119"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_green_2 "120"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_pale_green_1 "121"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_aquamarine_1b "122"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_slate_gray_1 "123"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_red_3 "124"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_pink_4c "125"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_violet_red "126"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_magenta_3 "127"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_violet_2 "128"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_purple_3b "129"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_orange_3 "130"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_indian_red "131"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_hot_pink_3 "132"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_orchid_3 "133"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_orchid "134"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_purple_2 "135"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_goldenrod "136"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_salmon_3 "137"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_rosy_brown "138"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_63 "139"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_purple_2b "140"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_purple_1 "141"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_gold_3 "142"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_khaki "143"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_navajo_white_3 "144"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_69 "145"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_steel_blue_3 "146"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_steel_blue "147"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_yellow_3 "148"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_bark_olive_green_3c "149"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_sea_green_3b "150"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_sea_green_2 "151"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_cyan_3 "152"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_sky_blue_1 "153"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_green_yellow "154"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_olive_green_2 "155"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_pale_green_1b "156"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_sea_green_2b "157"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_sea_green_1 "158"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_pale_turquoise_1 "159"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_red_3b "160"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_pink_3 "161"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_pink_3b "162"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_magenta_3b "163"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_magenta_3c "164"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_magenta_2 "165"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_orange_3b "166"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_indian_red_2 "167"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_hot_pink_3b "168"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_hot_pink_2 "169"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_orchid "170"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_orchid_1 "171"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_orange_3 "172"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_salmon_3b "173"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_pink_3 "174"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_pink_3 "175"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_plum_3 "176"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_violet "177"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_gold_3b "178"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_goldenrod_3 "179"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_tan "180"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_misty_rose_3 "181"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_thistle_3 "182"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_plum_2 "183"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_yellow_3b "184"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_khaki_3 "185"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_goldenrod_2 "186"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_yellow_3 "187"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_84 "188"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_steel_blue_1 "189"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_yellow_2 "190"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_olive_green_1 "191"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_olive_green_1b "192"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_sea_green_1b "193"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_honeydew_2 "194"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_cyan_1 "195"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_red_1 "196"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_pink_2 "197"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_pink_1 "198"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_deep_pink_1b "199"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_magenta_2b "200"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_magenta_1 "201"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_orange_red_1 "202"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_indian_red_1 "203"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_indian_red_1b "204"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_hot_pink "205"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_hot_pink_2b "206"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_medium_orchid_1b "207"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_dark_orange "208"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_salmon_1 "209"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_coral "210"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_pale_violet_red_1 "211"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_orchid_2 "212"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_orchid_1 "213"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_orange_1 "214"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_sandy_brown "215"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_salmon_1 "216"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_pink_1 "217"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_pink_1 "218"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_plum_1 "219"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_gold_1 "220"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_goldenrod_2b "221"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_goldenrod_2c "222"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_navajo_white_1 "223"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_misty_rose_1 "224"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_thistle_1 "225"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_yellow_1 "226"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_light_goldenrod_1 "227"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_khaki_1 "228"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_wheat_1 "229"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_cornsilk_1 "230"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_100 "231"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_3 "232"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_7 "233"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_11 "234"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_15 "235"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_19 "236"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_23 "237"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_27 "238"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_30 "239"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_35 "240"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_39 "241"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_42 "242"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_46 "243"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_50 "244"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_54 "245"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_58 "246"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_62 "247"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_66 "248"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_70 "249"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_74 "250"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_78 "251"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_82 "252"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_85 "253"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_89 "254"
#define ____daterm_color__pp_8bit_str$daterm_Palette8bit_grey_93 "255"
#pragma endregion "pp 8-bit color str"

/*========== 24-bit color ==========*/

/* --- Internal Declarations --- */

#define daterm_color__pp_24bit_layer_fg 0
#define daterm_color__pp_24bit_layer_bg 1

#define daterm_color__parseRGB24bit(_channel_tok) \
    ____daterm_color__parseRGB24bit(_channel_tok)
#define daterm_color__formatRGB24bit(_pp_24bit_layer, _chan_r_tok, _chan_g_tok, _chan_b_tok) \
    ____daterm_color__formatRGB24bit(_pp_24bit_layer, _chan_r_tok, _chan_g_tok, _chan_b_tok)

/* --- External Definitions --- */

#define ____daterm_color_fg24bit_staticParse(_chan_tok) \
    daterm_color__parseRGB24bit(_chan_tok)
#define ____daterm_color_fg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok) \
    daterm_color__formatRGB24bit(daterm_color__pp_24bit_layer_fg, _chan_r_tok, _chan_g_tok, _chan_b_tok)
#define ____daterm_color_bg24bit_staticParse(_chan_tok) \
    daterm_color__parseRGB24bit(_chan_tok)
#define ____daterm_color_bg24bit_static(_chan_r_tok, _chan_g_tok, _chan_b_tok) \
    daterm_color__formatRGB24bit(daterm_color__pp_24bit_layer_bg, _chan_r_tok, _chan_g_tok, _chan_b_tok)

/* --- Internal Definitions --- */

#define ____daterm_color__parseRGB24bit(_channel_tok) \
    #_channel_tok

#define ____daterm_color__formatRGB24bit__expand(...) __VA_ARGS__
#define ____daterm_color__formatRGB24bit(_pp_24bit_layer, _chan_r_tok, _chan_g_tok, _chan_b_tok) \
    ____daterm_color__formatRGB24bit__expand( \
        daterm_utils_csi pp_switch_ pp_begin(_pp_24bit_layer)( \
            pp_case_((daterm_color__pp_24bit_layer_fg)(daterm_utils_attr_color_fg_24bit)), \
            pp_case_((daterm_color__pp_24bit_layer_bg)(daterm_utils_attr_color_bg_24bit)) \
        ) pp_end _chan_r_tok daterm_utils_sep _chan_g_tok daterm_utils_sep _chan_b_tok daterm_utils_attr \
    )

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_color__included */

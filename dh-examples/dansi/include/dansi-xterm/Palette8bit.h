/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Palette8bit.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_Palette8bit
 */
#pragma once
#ifndef dansi_xterm_Palette8bit__included
#define dansi_xterm_Palette8bit__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/prl.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_xterm_Palette8bit $fits($packed))(
    dansi_xterm_Palette8bit_black = 0,
    dansi_xterm_Palette8bit_maroon = 1,
    dansi_xterm_Palette8bit_green = 2,
    dansi_xterm_Palette8bit_olive = 3,
    dansi_xterm_Palette8bit_navy = 4,
    dansi_xterm_Palette8bit_purple = 5,
    dansi_xterm_Palette8bit_teal = 6,
    dansi_xterm_Palette8bit_silver = 7,
    dansi_xterm_Palette8bit_grey = 8,
    dansi_xterm_Palette8bit_red = 9,
    dansi_xterm_Palette8bit_lime = 10,
    dansi_xterm_Palette8bit_yellow = 11,
    dansi_xterm_Palette8bit_blue = 12,
    dansi_xterm_Palette8bit_fuchsia = 13,
    dansi_xterm_Palette8bit_aqua = 14,
    dansi_xterm_Palette8bit_white = 15,

    // Extended colors (16-255) - add as needed
    // Non-system colors (16–255)
    dansi_xterm_Palette8bit_grey_0 = 16,
    dansi_xterm_Palette8bit_navy_blue = 17,
    dansi_xterm_Palette8bit_dark_blue = 18,
    dansi_xterm_Palette8bit_blue_3 = 19,
    dansi_xterm_Palette8bit_blue_3b = 20,
    dansi_xterm_Palette8bit_blue_1 = 21,
    dansi_xterm_Palette8bit_dark_green = 22,
    dansi_xterm_Palette8bit_deep_sky_blue_4 = 23,
    dansi_xterm_Palette8bit_deep_sky_blue_4b = 24,
    dansi_xterm_Palette8bit_deep_sky_blue_4c = 25,

    dansi_xterm_Palette8bit_dodger_blue_3 = 26,
    dansi_xterm_Palette8bit_dodger_blue_2 = 27,
    dansi_xterm_Palette8bit_green_4 = 28,

    dansi_xterm_Palette8bit_spring_green_4 = 29,
    dansi_xterm_Palette8bit_turquoise_4 = 30,

    dansi_xterm_Palette8bit_deep_sky_blue_3 = 31,
    dansi_xterm_Palette8bit_deep_sky_blue_3b = 32,
    dansi_xterm_Palette8bit_dodger_blue_1 = 33,
    dansi_xterm_Palette8bit_green_3 = 34,
    dansi_xterm_Palette8bit_spring_green_3 = 35,
    dansi_xterm_Palette8bit_dark_cyan = 36,
    dansi_xterm_Palette8bit_light_sea_green = 37,
    dansi_xterm_Palette8bit_deep_sky_blue_2 = 38,
    dansi_xterm_Palette8bit_deep_sky_blue_1 = 39,
    dansi_xterm_Palette8bit_green_3_2 = 40,
    dansi_xterm_Palette8bit_spring_green_3b = 41,
    dansi_xterm_Palette8bit_spring_green_2 = 42,
    dansi_xterm_Palette8bit_cyan_3 = 43,
    dansi_xterm_Palette8bit_dark_turquoise = 44,
    dansi_xterm_Palette8bit_turquoise_2 = 45,
    dansi_xterm_Palette8bit_green_1 = 46,
    dansi_xterm_Palette8bit_spring_green_2b = 47,
    dansi_xterm_Palette8bit_spring_green_1 = 48,
    dansi_xterm_Palette8bit_medium_spring_green = 49,
    dansi_xterm_Palette8bit_cyan_2 = 50,
    dansi_xterm_Palette8bit_cyan_1 = 51,
    dansi_xterm_Palette8bit_dark_red = 52,
    dansi_xterm_Palette8bit_deep_pink_4 = 53,
    dansi_xterm_Palette8bit_purple_4 = 54,
    dansi_xterm_Palette8bit_purple_4b = 55,
    dansi_xterm_Palette8bit_purple_3 = 56,
    dansi_xterm_Palette8bit_blue_violet = 57,
    dansi_xterm_Palette8bit_orange_4 = 58,
    dansi_xterm_Palette8bit_grey_37 = 59,
    dansi_xterm_Palette8bit_medium_purple_4 = 60,
    dansi_xterm_Palette8bit_slate_blue_3 = 61,
    dansi_xterm_Palette8bit_slate_blue_3b = 62,
    dansi_xterm_Palette8bit_royal_blue_1 = 63,
    dansi_xterm_Palette8bit_chartreuse_4 = 64,
    dansi_xterm_Palette8bit_dark_sea_green_4 = 65,
    dansi_xterm_Palette8bit_pale_turquoise_4 = 66,
    dansi_xterm_Palette8bit_steel_blue = 67,
    dansi_xterm_Palette8bit_steel_blue_3 = 68,
    dansi_xterm_Palette8bit_cornflower_blue = 69,
    dansi_xterm_Palette8bit_chartreuse_3 = 70,
    dansi_xterm_Palette8bit_dark_sea_green_4b = 71,
    dansi_xterm_Palette8bit_cadet_blue = 72,
    dansi_xterm_Palette8bit_cadet_blue_2 = 73,
    dansi_xterm_Palette8bit_sky_blue_3 = 74,
    dansi_xterm_Palette8bit_steel_blue_1 = 75,
    dansi_xterm_Palette8bit_chartreuse_3b = 76,
    dansi_xterm_Palette8bit_pale_green_3 = 77,
    dansi_xterm_Palette8bit_sea_green_3 = 78,
    dansi_xterm_Palette8bit_aquamarine_3 = 79,
    dansi_xterm_Palette8bit_medium_turquoise = 80,
    dansi_xterm_Palette8bit_steel_blue_1b = 81,
    dansi_xterm_Palette8bit_chartreuse_2 = 82,
    dansi_xterm_Palette8bit_sea_green_2 = 83,
    dansi_xterm_Palette8bit_sea_green_1 = 84,
    dansi_xterm_Palette8bit_sea_green_1b = 85,
    dansi_xterm_Palette8bit_aquamarine_1 = 86,
    dansi_xterm_Palette8bit_dark_slate_gray_2 = 87,
    dansi_xterm_Palette8bit_dark_red_2 = 88,
    dansi_xterm_Palette8bit_deep_pink_4b = 89,
    dansi_xterm_Palette8bit_dark_magenta = 90,
    dansi_xterm_Palette8bit_dark_magenta_2 = 91,
    dansi_xterm_Palette8bit_dark_violet = 92,
    dansi_xterm_Palette8bit_purple_2 = 93,
    dansi_xterm_Palette8bit_orange_4b = 94,
    dansi_xterm_Palette8bit_light_pink_4 = 95,
    dansi_xterm_Palette8bit_plum_4 = 96,
    dansi_xterm_Palette8bit_medium_purple_3 = 97,
    dansi_xterm_Palette8bit_medium_purple_3b = 98,
    dansi_xterm_Palette8bit_slate_blue_1b = 99,
    dansi_xterm_Palette8bit_yellow_4 = 100,
    dansi_xterm_Palette8bit_wheat_4 = 101,
    dansi_xterm_Palette8bit_grey_53 = 102,
    dansi_xterm_Palette8bit_light_slate_grey = 103,
    dansi_xterm_Palette8bit_medium_purple = 104,
    dansi_xterm_Palette8bit_light_slate_blue = 105,
    dansi_xterm_Palette8bit_yellow_4b = 106,
    dansi_xterm_Palette8bit_dark_olive_green_3 = 107,
    dansi_xterm_Palette8bit_dark_sea_green = 108,
    dansi_xterm_Palette8bit_light_sky_blue_3 = 109,
    dansi_xterm_Palette8bit_light_sky_blue_3b = 110,
    dansi_xterm_Palette8bit_sky_blue_2 = 111,
    dansi_xterm_Palette8bit_chartreuse_2b = 112,
    dansi_xterm_Palette8bit_dark_olive_green_3b = 113,
    dansi_xterm_Palette8bit_pale_green_3b = 114,
    dansi_xterm_Palette8bit_dark_sea_green_3 = 115,
    dansi_xterm_Palette8bit_dark_slate_gray_3 = 116,
    dansi_xterm_Palette8bit_sky_blue_1 = 117,
    dansi_xterm_Palette8bit_chartreuse_1 = 118,
    dansi_xterm_Palette8bit_light_green = 119,
    dansi_xterm_Palette8bit_light_green_2 = 120,
    dansi_xterm_Palette8bit_pale_green_1 = 121,
    dansi_xterm_Palette8bit_aquamarine_1b = 122,
    dansi_xterm_Palette8bit_dark_slate_gray_1 = 123,
    dansi_xterm_Palette8bit_red_3 = 124,
    dansi_xterm_Palette8bit_deep_pink_4c = 125,
    dansi_xterm_Palette8bit_medium_violet_red = 126,
    dansi_xterm_Palette8bit_magenta_3 = 127,
    dansi_xterm_Palette8bit_dark_violet_2 = 128,
    dansi_xterm_Palette8bit_purple_3b = 129,
    dansi_xterm_Palette8bit_dark_orange_3 = 130,
    dansi_xterm_Palette8bit_indian_red = 131,
    dansi_xterm_Palette8bit_hot_pink_3 = 132,
    dansi_xterm_Palette8bit_medium_orchid_3 = 133,
    dansi_xterm_Palette8bit_medium_orchid = 134,
    dansi_xterm_Palette8bit_medium_purple_2 = 135,
    dansi_xterm_Palette8bit_dark_goldenrod = 136,
    dansi_xterm_Palette8bit_light_salmon_3 = 137,
    dansi_xterm_Palette8bit_rosy_brown = 138,
    dansi_xterm_Palette8bit_grey_63 = 139,
    dansi_xterm_Palette8bit_medium_purple_2b = 140,
    dansi_xterm_Palette8bit_medium_purple_1 = 141,
    dansi_xterm_Palette8bit_gold_3 = 142,
    dansi_xterm_Palette8bit_dark_khaki = 143,
    dansi_xterm_Palette8bit_navajo_white_3 = 144,
    dansi_xterm_Palette8bit_grey_69 = 145,
    dansi_xterm_Palette8bit_light_steel_blue_3 = 146,
    dansi_xterm_Palette8bit_light_steel_blue = 147,
    dansi_xterm_Palette8bit_yellow_3 = 148,
    dansi_xterm_Palette8bit_bark_olive_green_3c = 149,
    dansi_xterm_Palette8bit_dark_sea_green_3b = 150,
    dansi_xterm_Palette8bit_dark_sea_green_2 = 151,
    dansi_xterm_Palette8bit_light_cyan_3 = 152,
    dansi_xterm_Palette8bit_light_sky_blue_1 = 153,
    dansi_xterm_Palette8bit_green_yellow = 154,
    dansi_xterm_Palette8bit_dark_olive_green_2 = 155,
    dansi_xterm_Palette8bit_pale_green_1b = 156,
    dansi_xterm_Palette8bit_dark_sea_green_2b = 157,
    dansi_xterm_Palette8bit_dark_sea_green_1 = 158,
    dansi_xterm_Palette8bit_pale_turquoise_1 = 159,
    dansi_xterm_Palette8bit_red_3b = 160,
    dansi_xterm_Palette8bit_deep_pink_3 = 161,
    dansi_xterm_Palette8bit_deep_pink_3b = 162,
    dansi_xterm_Palette8bit_magenta_3b = 163,
    dansi_xterm_Palette8bit_magenta_3c = 164,
    dansi_xterm_Palette8bit_magenta_2 = 165,
    dansi_xterm_Palette8bit_dark_orange_3b = 166,
    dansi_xterm_Palette8bit_indian_red_2 = 167,
    dansi_xterm_Palette8bit_hot_pink_3b = 168,
    dansi_xterm_Palette8bit_hot_pink_2 = 169,
    dansi_xterm_Palette8bit_orchid = 170,
    dansi_xterm_Palette8bit_medium_orchid_1 = 171,
    dansi_xterm_Palette8bit_orange_3 = 172,
    dansi_xterm_Palette8bit_light_salmon_3b = 173,
    dansi_xterm_Palette8bit_light_pink_3 = 174,
    dansi_xterm_Palette8bit_pink_3 = 175,
    dansi_xterm_Palette8bit_plum_3 = 176,
    dansi_xterm_Palette8bit_violet = 177,
    dansi_xterm_Palette8bit_gold_3b = 178,
    dansi_xterm_Palette8bit_light_goldenrod_3 = 179,
    dansi_xterm_Palette8bit_tan = 180,
    dansi_xterm_Palette8bit_misty_rose_3 = 181,
    dansi_xterm_Palette8bit_thistle_3 = 182,
    dansi_xterm_Palette8bit_plum_2 = 183,
    dansi_xterm_Palette8bit_yellow_3b = 184,
    dansi_xterm_Palette8bit_khaki_3 = 185,
    dansi_xterm_Palette8bit_light_goldenrod_2 = 186,
    dansi_xterm_Palette8bit_light_yellow_3 = 187,
    dansi_xterm_Palette8bit_grey_84 = 188,
    dansi_xterm_Palette8bit_light_steel_blue_1 = 189,
    dansi_xterm_Palette8bit_yellow_2 = 190,
    dansi_xterm_Palette8bit_dark_olive_green_1 = 191,
    dansi_xterm_Palette8bit_dark_olive_green_1b = 192,
    dansi_xterm_Palette8bit_dark_sea_green_1b = 193,
    dansi_xterm_Palette8bit_honeydew_2 = 194,
    dansi_xterm_Palette8bit_light_cyan_1 = 195,
    dansi_xterm_Palette8bit_red_1 = 196,
    dansi_xterm_Palette8bit_deep_pink_2 = 197,
    dansi_xterm_Palette8bit_deep_pink_1 = 198,
    dansi_xterm_Palette8bit_deep_pink_1b = 199,
    dansi_xterm_Palette8bit_magenta_2b = 200,
    dansi_xterm_Palette8bit_magenta_1 = 201,
    dansi_xterm_Palette8bit_orange_red_1 = 202,
    dansi_xterm_Palette8bit_indian_red_1 = 203,
    dansi_xterm_Palette8bit_indian_red_1b = 204,
    dansi_xterm_Palette8bit_hot_pink = 205,
    dansi_xterm_Palette8bit_hot_pink_2b = 206,
    dansi_xterm_Palette8bit_medium_orchid_1b = 207,
    dansi_xterm_Palette8bit_dark_orange = 208,
    dansi_xterm_Palette8bit_salmon_1 = 209,
    dansi_xterm_Palette8bit_light_coral = 210,
    dansi_xterm_Palette8bit_pale_violet_red_1 = 211,
    dansi_xterm_Palette8bit_orchid_2 = 212,
    dansi_xterm_Palette8bit_orchid_1 = 213,
    dansi_xterm_Palette8bit_orange_1 = 214,
    dansi_xterm_Palette8bit_sandy_brown = 215,
    dansi_xterm_Palette8bit_light_salmon_1 = 216,
    dansi_xterm_Palette8bit_light_pink_1 = 217,
    dansi_xterm_Palette8bit_pink_1 = 218,
    dansi_xterm_Palette8bit_plum_1 = 219,
    dansi_xterm_Palette8bit_gold_1 = 220,
    dansi_xterm_Palette8bit_light_goldenrod_2b = 221,
    dansi_xterm_Palette8bit_light_goldenrod_2c = 222,
    dansi_xterm_Palette8bit_navajo_white_1 = 223,
    dansi_xterm_Palette8bit_misty_rose_1 = 224,
    dansi_xterm_Palette8bit_thistle_1 = 225,
    dansi_xterm_Palette8bit_yellow_1 = 226,
    dansi_xterm_Palette8bit_light_goldenrod_1 = 227,
    dansi_xterm_Palette8bit_khaki_1 = 228,
    dansi_xterm_Palette8bit_wheat_1 = 229,
    dansi_xterm_Palette8bit_cornsilk_1 = 230,
    dansi_xterm_Palette8bit_grey_100 = 231,
    dansi_xterm_Palette8bit_grey_3 = 232,
    dansi_xterm_Palette8bit_grey_7 = 233,
    dansi_xterm_Palette8bit_grey_11 = 234,
    dansi_xterm_Palette8bit_grey_15 = 235,
    dansi_xterm_Palette8bit_grey_19 = 236,
    dansi_xterm_Palette8bit_grey_23 = 237,
    dansi_xterm_Palette8bit_grey_27 = 238,
    dansi_xterm_Palette8bit_grey_30 = 239,
    dansi_xterm_Palette8bit_grey_35 = 240,
    dansi_xterm_Palette8bit_grey_39 = 241,
    dansi_xterm_Palette8bit_grey_42 = 242,
    dansi_xterm_Palette8bit_grey_46 = 243,
    dansi_xterm_Palette8bit_grey_50 = 244,
    dansi_xterm_Palette8bit_grey_54 = 245,
    dansi_xterm_Palette8bit_grey_58 = 246,
    dansi_xterm_Palette8bit_grey_62 = 247,
    dansi_xterm_Palette8bit_grey_66 = 248,
    dansi_xterm_Palette8bit_grey_70 = 249,
    dansi_xterm_Palette8bit_grey_74 = 250,
    dansi_xterm_Palette8bit_grey_78 = 251,
    dansi_xterm_Palette8bit_grey_82 = 252,
    dansi_xterm_Palette8bit_grey_85 = 253,
    dansi_xterm_Palette8bit_grey_89 = 254,
    dansi_xterm_Palette8bit_grey_93 = 255
)) dansi_xterm_Palette8bit;
claim_assert_static(eqlType$(dansi_xterm_Palette8bit, u8));
T_use_prl$(dansi_xterm_Palette8bit);

#define dansi_xterm_Palette8bit_staticParse(_color_tok) \
    ____dansi_xterm_Palette8bit_staticParse(_color_tok)

$attr($inline_always)
$static fn_((dansi_xterm_Palette8bit_system(u8 index))(dansi_xterm_Palette8bit)) {
    return claim_assert(index < 16), index;
};

$attr($inline_always)
$static fn_((dansi_xterm_Palette8bit_cube(u8 r, u8 g, u8 b))(dansi_xterm_Palette8bit)) {
    return claim_assert(r < 6), claim_assert(g < 6), claim_assert(b < 6), as$(u8)(16 + 36 * r + 6 * g + b);
};

$attr($inline_always)
$static fn_((dansi_xterm_Palette8bit_gray(u8 index))(dansi_xterm_Palette8bit)) {
    return claim_assert(index < 24), as$(u8)(232 + index);
};

/*========== Macros and Definitions =========================================*/

#define ____dansi_xterm_Palette8bit_staticParse(_color_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_color_tok))( \
        pp_Tok_then_(#_color_tok), \
        pp_Tok_else_(pp_join($, ____dansi_xterm_Palette8bit_str, _color_tok)) \
    )
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_black "0"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_maroon "1"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_green "2"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_olive "3"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_navy "4"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_purple "5"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_teal "6"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_silver "7"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey "8"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_red "9"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_lime "10"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_yellow "11"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_blue "12"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_fuchsia "13"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_aqua "14"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_white "15"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_0 "16"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_navy_blue "17"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_blue "18"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_blue_3 "19"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_blue_3b "20"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_blue_1 "21"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_green "22"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_sky_blue_4 "23"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_sky_blue_4b "24"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_sky_blue_4c "25"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dodger_blue_3 "26"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dodger_blue_2 "27"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_green_4 "28"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_spring_green_4 "29"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_turquoise_4 "30"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_sky_blue_3 "31"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_sky_blue_3b "32"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dodger_blue_1 "33"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_green_3 "34"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_spring_green_3 "35"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_cyan "36"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_sea_green "37"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_sky_blue_2 "38"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_sky_blue_1 "39"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_green_3_2 "40"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_spring_green_3b "41"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_spring_green_2 "42"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_cyan_3 "43"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_turquoise "44"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_turquoise_2 "45"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_green_1 "46"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_spring_green_2b "47"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_spring_green_1 "48"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_spring_green "49"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_cyan_2 "50"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_cyan_1 "51"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_red "52"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_pink_4 "53"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_purple_4 "54"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_purple_4b "55"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_purple_3 "56"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_blue_violet "57"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_orange_4 "58"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_37 "59"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_purple_4 "60"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_slate_blue_3 "61"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_slate_blue_3b "62"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_royal_blue_1 "63"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_chartreuse_4 "64"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_sea_green_4 "65"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_pale_turquoise_4 "66"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_steel_blue "67"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_steel_blue_3 "68"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_cornflower_blue "69"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_chartreuse_3 "70"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_sea_green_4b "71"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_cadet_blue "72"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_cadet_blue_2 "73"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_sky_blue_3 "74"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_steel_blue_1 "75"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_chartreuse_3b "76"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_pale_green_3 "77"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_sea_green_3 "78"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_aquamarine_3 "79"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_turquoise "80"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_steel_blue_1b "81"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_chartreuse_2 "82"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_sea_green_2 "83"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_sea_green_1 "84"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_sea_green_1b "85"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_aquamarine_1 "86"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_slate_gray_2 "87"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_red_2 "88"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_pink_4b "89"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_magenta "90"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_magenta_2 "91"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_violet "92"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_purple_2 "93"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_orange_4b "94"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_pink_4 "95"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_plum_4 "96"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_purple_3 "97"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_purple_3b "98"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_slate_blue_1b "99"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_yellow_4 "100"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_wheat_4 "101"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_53 "102"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_slate_grey "103"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_purple "104"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_slate_blue "105"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_yellow_4b "106"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_olive_green_3 "107"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_sea_green "108"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_sky_blue_3 "109"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_sky_blue_3b "110"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_sky_blue_2 "111"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_chartreuse_2b "112"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_olive_green_3b "113"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_pale_green_3b "114"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_sea_green_3 "115"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_slate_gray_3 "116"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_sky_blue_1 "117"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_chartreuse_1 "118"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_green "119"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_green_2 "120"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_pale_green_1 "121"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_aquamarine_1b "122"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_slate_gray_1 "123"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_red_3 "124"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_pink_4c "125"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_violet_red "126"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_magenta_3 "127"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_violet_2 "128"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_purple_3b "129"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_orange_3 "130"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_indian_red "131"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_hot_pink_3 "132"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_orchid_3 "133"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_orchid "134"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_purple_2 "135"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_goldenrod "136"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_salmon_3 "137"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_rosy_brown "138"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_63 "139"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_purple_2b "140"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_purple_1 "141"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_gold_3 "142"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_khaki "143"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_navajo_white_3 "144"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_69 "145"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_steel_blue_3 "146"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_steel_blue "147"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_yellow_3 "148"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_bark_olive_green_3c "149"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_sea_green_3b "150"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_sea_green_2 "151"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_cyan_3 "152"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_sky_blue_1 "153"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_green_yellow "154"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_olive_green_2 "155"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_pale_green_1b "156"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_sea_green_2b "157"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_sea_green_1 "158"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_pale_turquoise_1 "159"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_red_3b "160"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_pink_3 "161"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_pink_3b "162"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_magenta_3b "163"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_magenta_3c "164"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_magenta_2 "165"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_orange_3b "166"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_indian_red_2 "167"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_hot_pink_3b "168"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_hot_pink_2 "169"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_orchid "170"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_orchid_1 "171"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_orange_3 "172"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_salmon_3b "173"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_pink_3 "174"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_pink_3 "175"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_plum_3 "176"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_violet "177"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_gold_3b "178"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_goldenrod_3 "179"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_tan "180"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_misty_rose_3 "181"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_thistle_3 "182"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_plum_2 "183"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_yellow_3b "184"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_khaki_3 "185"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_goldenrod_2 "186"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_yellow_3 "187"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_84 "188"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_steel_blue_1 "189"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_yellow_2 "190"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_olive_green_1 "191"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_olive_green_1b "192"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_sea_green_1b "193"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_honeydew_2 "194"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_cyan_1 "195"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_red_1 "196"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_pink_2 "197"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_pink_1 "198"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_deep_pink_1b "199"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_magenta_2b "200"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_magenta_1 "201"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_orange_red_1 "202"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_indian_red_1 "203"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_indian_red_1b "204"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_hot_pink "205"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_hot_pink_2b "206"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_medium_orchid_1b "207"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_dark_orange "208"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_salmon_1 "209"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_coral "210"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_pale_violet_red_1 "211"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_orchid_2 "212"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_orchid_1 "213"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_orange_1 "214"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_sandy_brown "215"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_salmon_1 "216"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_pink_1 "217"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_pink_1 "218"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_plum_1 "219"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_gold_1 "220"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_goldenrod_2b "221"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_goldenrod_2c "222"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_navajo_white_1 "223"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_misty_rose_1 "224"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_thistle_1 "225"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_yellow_1 "226"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_light_goldenrod_1 "227"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_khaki_1 "228"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_wheat_1 "229"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_cornsilk_1 "230"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_100 "231"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_3 "232"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_7 "233"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_11 "234"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_15 "235"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_19 "236"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_23 "237"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_27 "238"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_30 "239"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_35 "240"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_39 "241"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_42 "242"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_46 "243"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_50 "244"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_54 "245"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_58 "246"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_62 "247"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_66 "248"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_70 "249"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_74 "250"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_78 "251"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_82 "252"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_85 "253"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_89 "254"
#define ____dansi_xterm_Palette8bit_str$dansi_xterm_Palette8bit_grey_93 "255"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_Palette8bit__included */

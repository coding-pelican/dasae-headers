/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    sgr.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_sgr
 */
#pragma once
#ifndef dansi_sgr__included
#define dansi_sgr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "csi.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_sgr_Code $fits($packed))(
    dansi_sgr_Code_reset = 0,
    dansi_sgr_Code_bold = 1,
    dansi_sgr_Code_faint = 2,
    dansi_sgr_Code_italic = 3,
    dansi_sgr_Code_underline = 4,
    dansi_sgr_Code_blink = 5,
    dansi_sgr_Code_reverse = 7,
    dansi_sgr_Code_conceal = 8,
    dansi_sgr_Code_crossed_out = 9,
    dansi_sgr_Code_primary_font = 10,
    dansi_sgr_Code_no_bold_faint = 22,
    dansi_sgr_Code_no_italic = 23,
    dansi_sgr_Code_no_underline = 24,
    dansi_sgr_Code_no_blink = 25,
    dansi_sgr_Code_no_reverse = 27,
    dansi_sgr_Code_no_conceal = 28,
    dansi_sgr_Code_no_crossed_out = 29,
    dansi_sgr_Code_fg_black = 30,
    dansi_sgr_Code_fg_red = 31,
    dansi_sgr_Code_fg_green = 32,
    dansi_sgr_Code_fg_yellow = 33,
    dansi_sgr_Code_fg_blue = 34,
    dansi_sgr_Code_fg_magenta = 35,
    dansi_sgr_Code_fg_cyan = 36,
    dansi_sgr_Code_fg_white = 37,
    dansi_sgr_Code_fg_default = 39,
    dansi_sgr_Code_bg_black = 40,
    dansi_sgr_Code_bg_red = 41,
    dansi_sgr_Code_bg_green = 42,
    dansi_sgr_Code_bg_yellow = 43,
    dansi_sgr_Code_bg_blue = 44,
    dansi_sgr_Code_bg_magenta = 45,
    dansi_sgr_Code_bg_cyan = 46,
    dansi_sgr_Code_bg_white = 47,
    dansi_sgr_Code_bg_default = 49
)) dansi_sgr_Code;
claim_assert_static(eqlType$(dansi_sgr_Code, u8));
T_use_prl$(dansi_sgr_Code);

#define dansi_sgr_Code_staticParse(_code_tok) \
    ____dansi_sgr_Code_staticParse(_code_tok)

#define dansi_sgr_reset_static() \
    ____dansi_sgr_reset_static()
$extern fn_((dansi_sgr_reset(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_sgr_resetWrite(io_Writer out))(E$void));

#define dansi_sgr_setRaw_static(_params_tok) \
    ____dansi_sgr_setRaw_static(_params_tok)
$attr($must_check)
$extern fn_((dansi_sgr_setRaw(S_const$u8 params, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_sgr_setRawWrite(S_const$u8 params, io_Writer out))(E$void));

#define dansi_sgr_setOne_static(_code_tok) \
    ____dansi_sgr_setOne_static(_code_tok)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_sgr_SetOneBuf;
$attr($must_check)
$extern fn_((dansi_sgr_setOne(dansi_sgr_Code code, dansi_sgr_SetOneBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_sgr_setOneWrite(dansi_sgr_Code code, io_Writer out))(E$void));

$attr($must_check)
$extern fn_((dansi_sgr_set(S_const$u16 codes, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_sgr_setWrite(S_const$u16 codes, io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____dansi_sgr_reset_static() \
    dansi_csi_make1_static("0", "m")
#define ____dansi_sgr_setRaw_static(_params_tok) \
    dansi_csi_make1_static(_params_tok, "m")
#define ____dansi_sgr_setOne_static(_code_tok) \
    dansi_sgr_setRaw_static(dansi_sgr_Code_staticParse(_code_tok))
#define ____dansi_sgr_Code_staticParse(_code_tok) \
    pp_join(____dansi_sgr_Code_str$, _code_tok)
#define ____dansi_sgr_Code_str$dansi_sgr_Code_reset "0"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_bold "1"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_faint "2"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_italic "3"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_underline "4"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_blink "5"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_reverse "7"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_conceal "8"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_crossed_out "9"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_primary_font "10"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_no_bold_faint "22"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_no_italic "23"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_no_underline "24"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_no_blink "25"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_no_reverse "27"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_no_conceal "28"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_no_crossed_out "29"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_fg_black "30"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_fg_red "31"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_fg_green "32"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_fg_yellow "33"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_fg_blue "34"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_fg_magenta "35"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_fg_cyan "36"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_fg_white "37"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_fg_default "39"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_bg_black "40"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_bg_red "41"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_bg_green "42"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_bg_yellow "43"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_bg_blue "44"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_bg_magenta "45"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_bg_cyan "46"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_bg_white "47"
#define ____dansi_sgr_Code_str$dansi_sgr_Code_bg_default "49"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_sgr__included */

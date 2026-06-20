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
 * @prefix  dansi_color
 */
#pragma once
#ifndef dansi_color__included
#define dansi_color__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "sgr.h"

/*========== Macros and Declarations ========================================*/

#define dansi_color_fg_prefix __str__dansi_color_fg_prefix
#define dansi_color_fg_prefix_byte __uint__dansi_color_fg_prefix_byte
#define dansi_color_bg_prefix __str__dansi_color_bg_prefix
#define dansi_color_bg_prefix_byte __uint__dansi_color_bg_prefix_byte

typedef enum_((dansi_color_Std $fits($packed))(
    dansi_color_Std_black = 0,
    dansi_color_Std_red = 1,
    dansi_color_Std_green = 2,
    dansi_color_Std_yellow = 3,
    dansi_color_Std_blue = 4,
    dansi_color_Std_magenta = 5,
    dansi_color_Std_cyan = 6,
    dansi_color_Std_white = 7
)) dansi_color_Std;
claim_assert_static(eqlType$(dansi_color_Std, u8));
T_use_prl$(dansi_color_Std);

#define dansi_color_Std_staticParse(_color_tok) \
    ____dansi_color_Std_staticParse(_color_tok)

#define dansi_color_fg_static(_color_tok) \
    ____dansi_color_fg_static(_color_tok)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_color_FgBuf;
$attr($must_check)
$extern fn_((dansi_color_fg(dansi_color_Std color, dansi_color_FgBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_color_fgWrite(dansi_color_Std color, io_Writer out))(E$void));

#define dansi_color_bg_static(_color_tok) \
    ____dansi_color_bg_static(_color_tok)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_color_BgBuf;
$attr($must_check)
$extern fn_((dansi_color_bg(dansi_color_Std color, dansi_color_BgBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_color_bgWrite(dansi_color_Std color, io_Writer out))(E$void));

#define dansi_color_resetFG_static() \
    ____dansi_color_resetFG_static()
typedef dansi_color_FgBuf dansi_color_ResetFGBuf;
$attr($must_check)
$extern fn_((dansi_color_resetFG(dansi_color_ResetFGBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_color_resetFGWrite(io_Writer out))(E$void));

#define dansi_color_resetBG_static() \
    ____dansi_color_resetBG_static()
typedef dansi_color_BgBuf dansi_color_ResetBGBuf;
$attr($must_check)
$extern fn_((dansi_color_resetBG(dansi_color_ResetBGBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_color_resetBGWrite(io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_color_fg_prefix "3"
#define __uint__dansi_color_fg_prefix_byte u8_c('3')
#define __str__dansi_color_bg_prefix "4"
#define __uint__dansi_color_bg_prefix_byte u8_c('4')

#define ____dansi_color_fg_static(_color_tok) \
    dansi_sgr_setRaw_static(dansi_color_fg_prefix dansi_color_Std_staticParse(_color_tok))
#define ____dansi_color_bg_static(_color_tok) \
    dansi_sgr_setRaw_static(dansi_color_bg_prefix dansi_color_Std_staticParse(_color_tok))
#define ____dansi_color_resetFG_static() \
    dansi_sgr_setOne_static(dansi_sgr_Code_fg_default)
#define ____dansi_color_resetBG_static() \
    dansi_sgr_setOne_static(dansi_sgr_Code_bg_default)
#define ____dansi_color_Std_staticParse(_color_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_color_tok))( \
        pp_Tok_then_(#_color_tok), \
        pp_Tok_else_(pp_join($, ____dansi_color_Std_str, _color_tok)) \
    )
#define ____dansi_color_Std_str$dansi_color_Std_black "0"
#define ____dansi_color_Std_str$dansi_color_Std_red "1"
#define ____dansi_color_Std_str$dansi_color_Std_green "2"
#define ____dansi_color_Std_str$dansi_color_Std_yellow "3"
#define ____dansi_color_Std_str$dansi_color_Std_blue "4"
#define ____dansi_color_Std_str$dansi_color_Std_magenta "5"
#define ____dansi_color_Std_str$dansi_color_Std_cyan "6"
#define ____dansi_color_Std_str$dansi_color_Std_white "7"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_color__included */

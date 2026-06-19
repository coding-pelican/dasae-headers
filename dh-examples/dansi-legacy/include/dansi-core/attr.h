/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    attr.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_attr
 *
 * @brief   Terminal attribute reset
 * @details Provides functions to reset all colors and styles.
 */
#pragma once
#ifndef dansi_attr__included
#define dansi_attr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "color.h"
#include "style.h"

/*========== Macros and Declarations ========================================*/

#define dansi_attr_reset_static() \
    ____dansi_attr_reset_static()
/// Get ANSI sequence to reset all colors and styles
/// Returns constant string
$extern fn_((dansi_attr_reset(void))(S_const$u8));
/// Write ANSI sequence to reset all colors and styles
$attr($must_check)
$extern fn_((dansi_attr_resetWrite(io_Writer writer))(E$void));

#define dansi_attr_resetFG_static() \
    ____dansi_attr_resetFG_static()
$extern fn_((dansi_attr_resetFG(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_resetFGWrite(io_Writer writer))(E$void));

#define dansi_attr_resetBG_static() \
    ____dansi_attr_resetBG_static()
$extern fn_((dansi_attr_resetBG(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_resetBGWrite(io_Writer writer))(E$void));

#define dansi_attr_resetIntensity_static() \
    ____dansi_attr_resetIntensity_static()
$extern fn_((dansi_attr_resetIntensity(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_resetIntensityWrite(io_Writer writer))(E$void));

#define dansi_attr_resetItalic_static() \
    ____dansi_attr_resetItalic_static()
$extern fn_((dansi_attr_resetItalic(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_resetItalicWrite(io_Writer writer))(E$void));

#define dansi_attr_resetUnderline_static() \
    ____dansi_attr_resetUnderline_static()
$extern fn_((dansi_attr_resetUnderline(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_resetUnderlineWrite(io_Writer writer))(E$void));

#define dansi_attr_resetBlinking_static() \
    ____dansi_attr_resetBlinking_static()
$extern fn_((dansi_attr_resetBlinking(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_resetBlinkingWrite(io_Writer writer))(E$void));

#define dansi_attr_resetReverse_static() \
    ____dansi_attr_resetReverse_static()
$extern fn_((dansi_attr_resetReverse(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_resetReverseWrite(io_Writer writer))(E$void));

#define dansi_attr_resetInvisible_static() \
    ____dansi_attr_resetInvisible_static()
$extern fn_((dansi_attr_resetInvisible(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_resetInvisibleWrite(io_Writer writer))(E$void));

#define dansi_attr_resetStrikethrough_static() \
    ____dansi_attr_resetStrikethrough_static()
$extern fn_((dansi_attr_resetStrikethrough(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_resetStrikethroughWrite(io_Writer writer))(E$void));

#define dansi_attr_push_static() \
    ____dansi_attr_push_static()
$extern fn_((dansi_attr_push(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_pushWrite(io_Writer writer))(E$void));

#define dansi_attr_pop_static() \
    ____dansi_attr_pop_static()
$extern fn_((dansi_attr_pop(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_attr_popWrite(io_Writer writer))(E$void));

typedef A$$(4 * uint_log10Ceil_static(u16_limit_max) + 7, u8) dansi_attr_ReportRectBuf;

#define dansi_attr_reportRect_static(_top_tok, _left_tok, _bottom_tok, _right_tok) \
    ____dansi_attr_reportRect_static(_top_tok, _left_tok, _bottom_tok, _right_tok)
$extern fn_((dansi_attr_reportRect(u16 top, u16 left, u16 bottom, u16 right, dansi_attr_ReportRectBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_attr_reportRectWrite(u16 top, u16 left, u16 bottom, u16 right, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_attr_reset_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_reset)
#define ____dansi_attr_resetFG_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_reset_fg)
#define ____dansi_attr_resetBG_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_reset_bg)
#define ____dansi_attr_resetIntensity_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_style_no_bold)
#define ____dansi_attr_resetItalic_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_style_no_italic)
#define ____dansi_attr_resetUnderline_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_style_no_underline)
#define ____dansi_attr_resetBlinking_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_style_no_blinking)
#define ____dansi_attr_resetReverse_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_style_no_reverse)
#define ____dansi_attr_resetInvisible_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_style_no_invisible)
#define ____dansi_attr_resetStrikethrough_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_style_no_strikethrough)
#define ____dansi_attr_push_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_stack_push)
#define ____dansi_attr_pop_static() \
    dansi_utils_formatCSI_static(dansi_utils_attr_stack_pop)
#define ____dansi_attr_reportRect_static(_top_tok, _left_tok, _bottom_tok, _right_tok) \
    dansi_utils_csi _top_tok dansi_utils_sep _left_tok dansi_utils_sep _bottom_tok dansi_utils_sep _right_tok dansi_utils_attr_report_rect

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_attr__included */

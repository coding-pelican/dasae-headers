/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    style.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_style
 */
#pragma once
#ifndef dansi_style__included
#define dansi_style__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "sgr.h"

/*========== Macros and Declarations ========================================*/

#define dansi_style_bold_static(_pp_enabled) \
    ____dansi_style_bold_static(_pp_enabled)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_style_BoldBuf;
$attr($must_check)
$extern fn_((dansi_style_bold(bool enabled, dansi_style_BoldBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_boldWrite(bool enabled, io_Writer out))(E$void));

#define dansi_style_faint_static(_pp_enabled) \
    ____dansi_style_faint_static(_pp_enabled)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_style_FaintBuf;
$attr($must_check)
$extern fn_((dansi_style_faint(bool enabled, dansi_style_FaintBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_faintWrite(bool enabled, io_Writer out))(E$void));

#define dansi_style_italic_static(_pp_enabled) \
    ____dansi_style_italic_static(_pp_enabled)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_style_ItalicBuf;
$attr($must_check)
$extern fn_((dansi_style_italic(bool enabled, dansi_style_ItalicBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_italicWrite(bool enabled, io_Writer out))(E$void));

#define dansi_style_underline_static(_pp_enabled) \
    ____dansi_style_underline_static(_pp_enabled)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_style_UnderlineBuf;
$attr($must_check)
$extern fn_((dansi_style_underline(bool enabled, dansi_style_UnderlineBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_underlineWrite(bool enabled, io_Writer out))(E$void));

#define dansi_style_blink_static(_pp_enabled) \
    ____dansi_style_blink_static(_pp_enabled)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_style_BlinkBuf;
$attr($must_check)
$extern fn_((dansi_style_blink(bool enabled, dansi_style_BlinkBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_blinkWrite(bool enabled, io_Writer out))(E$void));

#define dansi_style_reverse_static(_pp_enabled) \
    ____dansi_style_reverse_static(_pp_enabled)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_style_ReverseBuf;
$attr($must_check)
$extern fn_((dansi_style_reverse(bool enabled, dansi_style_ReverseBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_reverseWrite(bool enabled, io_Writer out))(E$void));

#define dansi_style_conceal_static(_pp_enabled) \
    ____dansi_style_conceal_static(_pp_enabled)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_style_ConcealBuf;
$attr($must_check)
$extern fn_((dansi_style_conceal(bool enabled, dansi_style_ConcealBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_concealWrite(bool enabled, io_Writer out))(E$void));

#define dansi_style_crossedOut_static(_pp_enabled) \
    ____dansi_style_crossedOut_static(_pp_enabled)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_style_CrossedOutBuf;
$attr($must_check)
$extern fn_((dansi_style_crossedOut(bool enabled, dansi_style_CrossedOutBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_crossedOutWrite(bool enabled, io_Writer out))(E$void));

#define dansi_style_resetIntensity_static() \
    ____dansi_style_resetIntensity_static()
typedef dansi_style_BoldBuf dansi_style_ResetIntensityBuf;
$attr($must_check)
$extern fn_((dansi_style_resetIntensity(dansi_style_ResetIntensityBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_resetIntensityWrite(io_Writer out))(E$void));

#define dansi_style_resetItalic_static() \
    ____dansi_style_resetItalic_static()
typedef dansi_style_ItalicBuf dansi_style_ResetItalicBuf;
$attr($must_check)
$extern fn_((dansi_style_resetItalic(dansi_style_ResetItalicBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_resetItalicWrite(io_Writer out))(E$void));

#define dansi_style_resetUnderline_static() \
    ____dansi_style_resetUnderline_static()
typedef dansi_style_UnderlineBuf dansi_style_ResetUnderlineBuf;
$attr($must_check)
$extern fn_((dansi_style_resetUnderline(dansi_style_ResetUnderlineBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_resetUnderlineWrite(io_Writer out))(E$void));

#define dansi_style_resetBlink_static() \
    ____dansi_style_resetBlink_static()
typedef dansi_style_BlinkBuf dansi_style_ResetBlinkBuf;
$attr($must_check)
$extern fn_((dansi_style_resetBlink(dansi_style_ResetBlinkBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_resetBlinkWrite(io_Writer out))(E$void));

#define dansi_style_resetReverse_static() \
    ____dansi_style_resetReverse_static()
typedef dansi_style_ReverseBuf dansi_style_ResetReverseBuf;
$attr($must_check)
$extern fn_((dansi_style_resetReverse(dansi_style_ResetReverseBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_resetReverseWrite(io_Writer out))(E$void));

#define dansi_style_resetConceal_static() \
    ____dansi_style_resetConceal_static()
typedef dansi_style_ConcealBuf dansi_style_ResetConcealBuf;
$attr($must_check)
$extern fn_((dansi_style_resetConceal(dansi_style_ResetConcealBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_resetConcealWrite(io_Writer out))(E$void));

#define dansi_style_resetCrossedOut_static() \
    ____dansi_style_resetCrossedOut_static()
typedef dansi_style_CrossedOutBuf dansi_style_ResetCrossedOutBuf;
$attr($must_check)
$extern fn_((dansi_style_resetCrossedOut(dansi_style_ResetCrossedOutBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_style_resetCrossedOutWrite(io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____dansi_style_bold_static(_pp_enabled) \
    pp_if_(_pp_enabled)(pp_then_(dansi_sgr_setOne_static(dansi_sgr_Code_bold)), pp_else_(dansi_style_resetIntensity_static()))
#define ____dansi_style_faint_static(_pp_enabled) \
    pp_if_(_pp_enabled)(pp_then_(dansi_sgr_setOne_static(dansi_sgr_Code_faint)), pp_else_(dansi_style_resetIntensity_static()))
#define ____dansi_style_italic_static(_pp_enabled) \
    pp_if_(_pp_enabled)(pp_then_(dansi_sgr_setOne_static(dansi_sgr_Code_italic)), pp_else_(dansi_style_resetItalic_static()))
#define ____dansi_style_underline_static(_pp_enabled) \
    pp_if_(_pp_enabled)(pp_then_(dansi_sgr_setOne_static(dansi_sgr_Code_underline)), pp_else_(dansi_style_resetUnderline_static()))
#define ____dansi_style_blink_static(_pp_enabled) \
    pp_if_(_pp_enabled)(pp_then_(dansi_sgr_setOne_static(dansi_sgr_Code_blink)), pp_else_(dansi_style_resetBlink_static()))
#define ____dansi_style_reverse_static(_pp_enabled) \
    pp_if_(_pp_enabled)(pp_then_(dansi_sgr_setOne_static(dansi_sgr_Code_reverse)), pp_else_(dansi_style_resetReverse_static()))
#define ____dansi_style_conceal_static(_pp_enabled) \
    pp_if_(_pp_enabled)(pp_then_(dansi_sgr_setOne_static(dansi_sgr_Code_conceal)), pp_else_(dansi_style_resetConceal_static()))
#define ____dansi_style_crossedOut_static(_pp_enabled) \
    pp_if_(_pp_enabled)(pp_then_(dansi_sgr_setOne_static(dansi_sgr_Code_crossed_out)), pp_else_(dansi_style_resetCrossedOut_static()))
#define ____dansi_style_resetIntensity_static() \
    dansi_sgr_setOne_static(dansi_sgr_Code_no_bold_faint)
#define ____dansi_style_resetItalic_static() \
    dansi_sgr_setOne_static(dansi_sgr_Code_no_italic)
#define ____dansi_style_resetUnderline_static() \
    dansi_sgr_setOne_static(dansi_sgr_Code_no_underline)
#define ____dansi_style_resetBlink_static() \
    dansi_sgr_setOne_static(dansi_sgr_Code_no_blink)
#define ____dansi_style_resetReverse_static() \
    dansi_sgr_setOne_static(dansi_sgr_Code_no_reverse)
#define ____dansi_style_resetConceal_static() \
    dansi_sgr_setOne_static(dansi_sgr_Code_no_conceal)
#define ____dansi_style_resetCrossedOut_static() \
    dansi_sgr_setOne_static(dansi_sgr_Code_no_crossed_out)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_style__included */

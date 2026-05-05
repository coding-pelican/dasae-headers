/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    style.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-21 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_style
 *
 * @brief   Terminal text styling support
 */
#ifndef daterm_style__included
#define daterm_style__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/Writer.h>

/*========== Macros and Declarations ========================================*/

#define daterm_style_bold_static(_pp_enabled) \
    ____daterm_style_bold_static(_pp_enabled)
/// Get ANSI sequence to set/unset bold
$extern fn_((daterm_style_bold(bool enabled))(S_const$u8));
/// Write ANSI sequence to set/unset bold
$attr($must_check)
$extern fn_((daterm_style_boldWrite(bool enabled, io_Writer writer))(E$void));

#define daterm_style_dim_static(_pp_enabled) \
    ____daterm_style_dim_static(_pp_enabled)
/// Get ANSI sequence to set/unset dim
$extern fn_((daterm_style_dim(bool enabled))(S_const$u8));
/// Write ANSI sequence to set/unset dim
$attr($must_check)
$extern fn_((daterm_style_dimWrite(bool enabled, io_Writer writer))(E$void));

#define daterm_style_italic_static(_pp_enabled) \
    ____daterm_style_italic_static(_pp_enabled)
/// Get ANSI sequence to set/unset italic
$extern fn_((daterm_style_italic(bool enabled))(S_const$u8));
/// Write ANSI sequence to set/unset italic
$attr($must_check)
$extern fn_((daterm_style_italicWrite(bool enabled, io_Writer writer))(E$void));

#define daterm_style_underline_static(_pp_enabled) \
    ____daterm_style_underline_static(_pp_enabled)
/// Get ANSI sequence to set/unset underline
$extern fn_((daterm_style_underline(bool enabled))(S_const$u8));
/// Write ANSI sequence to set/unset underline
$attr($must_check)
$extern fn_((daterm_style_underlineWrite(bool enabled, io_Writer writer))(E$void));

#define daterm_style_blinking_static(_pp_enabled) \
    ____daterm_style_blinking_static(_pp_enabled)
/// Get ANSI sequence to set/unset blinking
$extern fn_((daterm_style_blinking(bool enabled))(S_const$u8));
/// Write ANSI sequence to set/unset blinking
$attr($must_check)
$extern fn_((daterm_style_blinkingWrite(bool enabled, io_Writer writer))(E$void));

#define daterm_style_reverse_static(_pp_enabled) \
    ____daterm_style_reverse_static(_pp_enabled)
/// Get ANSI sequence to set/unset reverse (swap fg/bg)
$extern fn_((daterm_style_reverse(bool enabled))(S_const$u8));
/// Write ANSI sequence to set/unset reverse
$attr($must_check)
$extern fn_((daterm_style_reverseWrite(bool enabled, io_Writer writer))(E$void));

#define daterm_style_invisible_static(_pp_enabled) \
    ____daterm_style_invisible_static(_pp_enabled)
/// Get ANSI sequence to set/unset invisible/hidden
$extern fn_((daterm_style_invisible(bool enabled))(S_const$u8));
/// Write ANSI sequence to set/unset invisible
$attr($must_check)
$extern fn_((daterm_style_invisibleWrite(bool enabled, io_Writer writer))(E$void));

#define daterm_style_strikethrough_static(_pp_enabled) \
    ____daterm_style_strikethrough_static(_pp_enabled)
/// Get ANSI sequence to set/unset strikethrough
$extern fn_((daterm_style_strikethrough(bool enabled))(S_const$u8));
/// Write ANSI sequence to set/unset strikethrough
$attr($must_check)
$extern fn_((daterm_style_strikethroughWrite(bool enabled, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "daterm/utils.h"

#define ____daterm_style_bold_static(_pp_enabled) daterm_utils_formatCSI_static(pp_if_(_pp_enabled)( \
    pp_then_(daterm_utils_attr_style_bold), \
    pp_else_(daterm_utils_attr_style_no_bold) \
))
#define ____daterm_style_dim_static(_pp_enabled) daterm_utils_formatCSI_static(pp_if_(_pp_enabled)( \
    pp_then_(daterm_utils_attr_style_dim), \
    pp_else_(daterm_utils_attr_style_no_dim) \
))
#define ____daterm_style_italic_static(_pp_enabled) daterm_utils_formatCSI_static(pp_if_(_pp_enabled)( \
    pp_then_(daterm_utils_attr_style_italic), \
    pp_else_(daterm_utils_attr_style_no_italic) \
))
#define ____daterm_style_underline_static(_pp_enabled) daterm_utils_formatCSI_static(pp_if_(_pp_enabled)( \
    pp_then_(daterm_utils_attr_style_underline), \
    pp_else_(daterm_utils_attr_style_no_underline) \
))
#define ____daterm_style_blinking_static(_pp_enabled) daterm_utils_formatCSI_static(pp_if_(_pp_enabled)( \
    pp_then_(daterm_utils_attr_style_blinking), \
    pp_else_(daterm_utils_attr_style_no_blinking) \
))
#define ____daterm_style_reverse_static(_pp_enabled) daterm_utils_formatCSI_static(pp_if_(_pp_enabled)( \
    pp_then_(daterm_utils_attr_style_reverse), \
    pp_else_(daterm_utils_attr_style_no_reverse) \
))
#define ____daterm_style_invisible_static(_pp_enabled) daterm_utils_formatCSI_static(pp_if_(_pp_enabled)( \
    pp_then_(daterm_utils_attr_style_invisible), \
    pp_else_(daterm_utils_attr_style_no_invisible) \
))
#define ____daterm_style_strikethrough_static(_pp_enabled) daterm_utils_formatCSI_static(pp_if_(_pp_enabled)( \
    pp_then_(daterm_utils_attr_style_strikethrough), \
    pp_else_(daterm_utils_attr_style_no_strikethrough) \
))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_style__included */

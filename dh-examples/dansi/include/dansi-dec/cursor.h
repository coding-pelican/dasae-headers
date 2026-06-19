/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    cursor.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_dec_cursor
 */
#pragma once
#ifndef dansi_dec_cursor__included
#define dansi_dec_cursor__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/esc.h"
#include "dansi-core/csi.h"
#include "mode.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_dec_cursor_Style $fits($packed))(
    dansi_dec_cursor_Style_default = 0,
    dansi_dec_cursor_Style_block_blink = 1,
    dansi_dec_cursor_Style_block = 2,
    dansi_dec_cursor_Style_underline_blink = 3,
    dansi_dec_cursor_Style_underline = 4,
    dansi_dec_cursor_Style_bar_blink = 5,
    dansi_dec_cursor_Style_bar = 6
)) dansi_dec_cursor_Style;
claim_assert_static(eqlType$(dansi_dec_cursor_Style, u8));
T_use_prl$(dansi_dec_cursor_Style);

#define dansi_dec_cursor_Style_staticParse(_style_tok) \
    ____dansi_dec_cursor_Style_staticParse(_style_tok)

#define dansi_dec_cursor_show_static() \
    ____dansi_dec_cursor_show_static()
typedef dansi_dec_mode_EnableBuf dansi_dec_cursor_ShowBuf;
$attr($must_check)
$extern fn_((dansi_dec_cursor_show(dansi_dec_cursor_ShowBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_cursor_showWrite(io_Writer out))(E$void));

#define dansi_dec_cursor_hide_static() \
    ____dansi_dec_cursor_hide_static()
typedef dansi_dec_mode_DisableBuf dansi_dec_cursor_HideBuf;
$attr($must_check)
$extern fn_((dansi_dec_cursor_hide(dansi_dec_cursor_HideBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_cursor_hideWrite(io_Writer out))(E$void));

#define dansi_dec_cursor_save_static() \
    ____dansi_dec_cursor_save_static()
$extern fn_((dansi_dec_cursor_save(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_cursor_saveWrite(io_Writer out))(E$void));

#define dansi_dec_cursor_restore_static() \
    ____dansi_dec_cursor_restore_static()
$extern fn_((dansi_dec_cursor_restore(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_cursor_restoreWrite(io_Writer out))(E$void));

#define dansi_dec_cursor_setStyle_static(_style_tok) \
    ____dansi_dec_cursor_setStyle_static(_style_tok)
typedef A$$(4 + uint_log10Ceil_static(u8_limit_max), u8) dansi_dec_cursor_SetStyleBuf;
$attr($must_check)
$extern fn_((dansi_dec_cursor_setStyle(
    dansi_dec_cursor_Style style, dansi_dec_cursor_SetStyleBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_cursor_setStyleWrite(dansi_dec_cursor_Style style, io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____dansi_dec_cursor_show_static() \
    dansi_dec_mode_enable_static(dansi_dec_mode_Code_text_cursor)
#define ____dansi_dec_cursor_hide_static() \
    dansi_dec_mode_disable_static(dansi_dec_mode_Code_text_cursor)
#define ____dansi_dec_cursor_save_static() \
    dansi_esc_make_static("", "7")
#define ____dansi_dec_cursor_restore_static() \
    dansi_esc_make_static("", "8")
#define ____dansi_dec_cursor_setStyle_static(_style_tok) \
    dansi_csi_make_static(dansi_dec_cursor_Style_staticParse(_style_tok), " ", "q")
#define ____dansi_dec_cursor_Style_staticParse(_style_tok) \
    pp_join(____dansi_dec_cursor_Style_str$, _style_tok)
#define ____dansi_dec_cursor_Style_str$dansi_dec_cursor_Style_default "0"
#define ____dansi_dec_cursor_Style_str$dansi_dec_cursor_Style_block_blink "1"
#define ____dansi_dec_cursor_Style_str$dansi_dec_cursor_Style_block "2"
#define ____dansi_dec_cursor_Style_str$dansi_dec_cursor_Style_underline_blink "3"
#define ____dansi_dec_cursor_Style_str$dansi_dec_cursor_Style_underline "4"
#define ____dansi_dec_cursor_Style_str$dansi_dec_cursor_Style_bar_blink "5"
#define ____dansi_dec_cursor_Style_str$dansi_dec_cursor_Style_bar "6"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_dec_cursor__included */

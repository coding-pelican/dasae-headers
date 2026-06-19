/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    mode.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_mode
 */
#pragma once
#ifndef dansi_xterm_mode__included
#define dansi_xterm_mode__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/csi.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_xterm_mode_Code $fits($packed))(
    dansi_xterm_mode_Code_mouse_x10 = 9,
    dansi_xterm_mode_Code_mouse_normal = 1000,
    dansi_xterm_mode_Code_mouse_hilite = 1001,
    dansi_xterm_mode_Code_mouse_button_event = 1002,
    dansi_xterm_mode_Code_mouse_any_event = 1003,
    dansi_xterm_mode_Code_focus_event = 1004,
    dansi_xterm_mode_Code_mouse_utf8 = 1005,
    dansi_xterm_mode_Code_mouse_sgr = 1006,
    dansi_xterm_mode_Code_alternate_scroll = 1007,
    dansi_xterm_mode_Code_mouse_urxvt = 1015,
    dansi_xterm_mode_Code_mouse_sgr_pixels = 1016,
    dansi_xterm_mode_Code_bracketed_paste = 2004,
    dansi_xterm_mode_Code_alt_screen = 1047,
    dansi_xterm_mode_Code_save_cursor = 1048,
    dansi_xterm_mode_Code_alt_screen_save_cursor = 1049,
    dansi_xterm_mode_Code_sync_output = 2026
)) dansi_xterm_mode_Code;
claim_assert_static(eqlType$(dansi_xterm_mode_Code, u16));
T_use_prl$(dansi_xterm_mode_Code);

#define dansi_xterm_mode_Code_staticParse(_mode_tok) \
    ____dansi_xterm_mode_Code_staticParse(_mode_tok)

#define dansi_xterm_mode_setRaw_static(_mode_tok, _pp_enabled) \
    ____dansi_xterm_mode_setRaw_static(_mode_tok, _pp_enabled)
typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_xterm_mode_SetRawBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mode_setRaw(u16 mode, bool enabled, dansi_xterm_mode_SetRawBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_setRawWrite(u16 mode, bool enabled, io_Writer out))(E$void));

#define dansi_xterm_mode_enableRaw_static(_mode_tok) \
    ____dansi_xterm_mode_enableRaw_static(_mode_tok)
typedef dansi_xterm_mode_SetRawBuf dansi_xterm_mode_EnableRawBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mode_enableRaw(u16 mode, dansi_xterm_mode_EnableRawBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_enableRawWrite(u16 mode, io_Writer out))(E$void));

#define dansi_xterm_mode_disableRaw_static(_mode_tok) \
    ____dansi_xterm_mode_disableRaw_static(_mode_tok)
typedef dansi_xterm_mode_SetRawBuf dansi_xterm_mode_DisableRawBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mode_disableRaw(u16 mode, dansi_xterm_mode_DisableRawBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_disableRawWrite(u16 mode, io_Writer out))(E$void));

$attr($must_check)
$extern fn_((dansi_xterm_mode_setMany(S_const$u16 modes, bool enabled, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_setManyWrite(S_const$u16 modes, bool enabled, io_Writer out))(E$void));

#define dansi_xterm_mode_set_static(_mode_tok, _pp_enabled) \
    dansi_xterm_mode_setRaw_static(dansi_xterm_mode_Code_staticParse(_mode_tok), _pp_enabled)
typedef dansi_xterm_mode_SetRawBuf dansi_xterm_mode_SetBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mode_set(dansi_xterm_mode_Code mode, bool enabled, dansi_xterm_mode_SetBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_setWrite(dansi_xterm_mode_Code mode, bool enabled, io_Writer out))(E$void));

#define dansi_xterm_mode_enable_static(_mode_tok) \
    dansi_xterm_mode_enableRaw_static(dansi_xterm_mode_Code_staticParse(_mode_tok))
typedef dansi_xterm_mode_EnableRawBuf dansi_xterm_mode_EnableBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mode_enable(dansi_xterm_mode_Code mode, dansi_xterm_mode_EnableBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_enableWrite(dansi_xterm_mode_Code mode, io_Writer out))(E$void));

#define dansi_xterm_mode_disable_static(_mode_tok) \
    dansi_xterm_mode_disableRaw_static(dansi_xterm_mode_Code_staticParse(_mode_tok))
typedef dansi_xterm_mode_DisableRawBuf dansi_xterm_mode_DisableBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mode_disable(dansi_xterm_mode_Code mode, dansi_xterm_mode_DisableBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_disableWrite(dansi_xterm_mode_Code mode, io_Writer out))(E$void));

#define dansi_xterm_mode_saveRaw_static(_modes_tok) \
    ____dansi_xterm_mode_saveRaw_static(_modes_tok)
$attr($must_check)
$extern fn_((dansi_xterm_mode_saveRaw(S_const$u16 modes, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_saveRawWrite(S_const$u16 modes, io_Writer out))(E$void));

#define dansi_xterm_mode_restoreRaw_static(_modes_tok) \
    ____dansi_xterm_mode_restoreRaw_static(_modes_tok)
$attr($must_check)
$extern fn_((dansi_xterm_mode_restoreRaw(S_const$u16 modes, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_restoreRawWrite(S_const$u16 modes, io_Writer out))(E$void));

$attr($must_check)
$extern fn_((dansi_xterm_mode_save(S_const$dansi_xterm_mode_Code modes, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_saveWrite(S_const$dansi_xterm_mode_Code modes, io_Writer out))(E$void));

$attr($must_check)
$extern fn_((dansi_xterm_mode_restore(S_const$dansi_xterm_mode_Code modes, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_restoreWrite(S_const$dansi_xterm_mode_Code modes, io_Writer out))(E$void));

#define dansi_xterm_mode_saveOne_static(_mode_tok) \
    dansi_xterm_mode_saveRaw_static(dansi_xterm_mode_Code_staticParse(_mode_tok))
typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_xterm_mode_SaveOneBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mode_saveOne(dansi_xterm_mode_Code mode, dansi_xterm_mode_SaveOneBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_saveOneWrite(dansi_xterm_mode_Code mode, io_Writer out))(E$void));

#define dansi_xterm_mode_restoreOne_static(_mode_tok) \
    dansi_xterm_mode_restoreRaw_static(dansi_xterm_mode_Code_staticParse(_mode_tok))
typedef dansi_xterm_mode_SaveOneBuf dansi_xterm_mode_RestoreOneBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mode_restoreOne(dansi_xterm_mode_Code mode, dansi_xterm_mode_RestoreOneBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mode_restoreOneWrite(dansi_xterm_mode_Code mode, io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____dansi_xterm_mode_enableRaw_static(_mode_tok) \
    dansi_csi_makePrivate1_static(_mode_tok, "h")
#define ____dansi_xterm_mode_disableRaw_static(_mode_tok) \
    dansi_csi_makePrivate1_static(_mode_tok, "l")
#define ____dansi_xterm_mode_setRaw_static(_mode_tok, _pp_enabled) \
    pp_if_(_pp_enabled)( \
        pp_then_(dansi_xterm_mode_enableRaw_static(_mode_tok)), \
        pp_else_(dansi_xterm_mode_disableRaw_static(_mode_tok)) \
    )
#define ____dansi_xterm_mode_saveRaw_static(_mode_tok) \
    dansi_csi_makePrivate1_static(_mode_tok, "s")
#define ____dansi_xterm_mode_restoreRaw_static(_mode_tok) \
    dansi_csi_makePrivate1_static(_mode_tok, "r")
#define ____dansi_xterm_mode_Code_staticParse(_mode_tok) \
    pp_join($, ____dansi_xterm_mode_Code_str, _mode_tok)
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_mouse_x10 "9"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_mouse_normal "1000"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_mouse_hilite "1001"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_mouse_button_event "1002"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_mouse_any_event "1003"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_focus_event "1004"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_mouse_utf8 "1005"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_mouse_sgr "1006"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_alternate_scroll "1007"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_mouse_urxvt "1015"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_mouse_sgr_pixels "1016"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_bracketed_paste "2004"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_alt_screen "1047"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_save_cursor "1048"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_alt_screen_save_cursor "1049"
#define ____dansi_xterm_mode_Code_str$dansi_xterm_mode_Code_sync_output "2026"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_mode__included */

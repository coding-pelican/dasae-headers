/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    mode.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_mode
 */
#pragma once
#ifndef dansi_mode__included
#define dansi_mode__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_mode_Private $fits($packed))(
    dansi_mode_Private_cursor_keys = 1,
    dansi_mode_Private_132_col = 3,
    dansi_mode_Private_origin = 6,
    dansi_mode_Private_auto_wrap = 7,
    dansi_mode_Private_mouse_x10 = 9,
    dansi_mode_Private_toolbar = 10,
    dansi_mode_Private_blink_cursor = 12,
    dansi_mode_Private_blink_cursor_xor = 14,
    dansi_mode_Private_show_cursor = 25,
    dansi_mode_Private_scrollbar = 30,
    dansi_mode_Private_allow_80_to_132 = 40,
    dansi_mode_Private_margin_bell = 44,
    dansi_mode_Private_reverse_wrap = 45,
    dansi_mode_Private_alt_screen_47 = 47,
    dansi_mode_Private_app_keypad = 66,
    dansi_mode_Private_left_right_margin = 69,
    dansi_mode_Private_no_clear_col_mode = 95,
    dansi_mode_Private_mouse_normal = 1000,
    dansi_mode_Private_mouse_hilite = 1001,
    dansi_mode_Private_mouse_button_event = 1002,
    dansi_mode_Private_mouse_any_event = 1003,
    dansi_mode_Private_focus_events = 1004,
    dansi_mode_Private_mouse_utf8 = 1005,
    dansi_mode_Private_mouse_sgr = 1006,
    dansi_mode_Private_alt_scroll = 1007,
    dansi_mode_Private_scroll_on_tty_output = 1010,
    dansi_mode_Private_scroll_on_key = 1011,
    dansi_mode_Private_fast_scroll = 1014,
    dansi_mode_Private_mouse_urxvt = 1015,
    dansi_mode_Private_mouse_sgr_pixels = 1016,
    dansi_mode_Private_meta_8bit = 1034,
    dansi_mode_Private_alt_numlock_mods = 1035,
    dansi_mode_Private_meta_sends_esc = 1036,
    dansi_mode_Private_delete_sends_del = 1037,
    dansi_mode_Private_alt_sends_esc = 1039,
    dansi_mode_Private_keep_selection = 1040,
    dansi_mode_Private_select_to_clipboard = 1041,
    dansi_mode_Private_bell_urgent = 1042,
    dansi_mode_Private_pop_on_bell = 1043,
    dansi_mode_Private_keep_clipboard = 1044,
    dansi_mode_Private_reverse_wrap_extended = 1045,
    dansi_mode_Private_allow_alt_screen = 1046,
    dansi_mode_Private_alt_screen_1047 = 1047,
    dansi_mode_Private_save_cursor = 1048,
    dansi_mode_Private_alt_screen = 1049,
    dansi_mode_Private_fn_keys_terminfo = 1050,
    dansi_mode_Private_fn_keys_sun = 1051,
    dansi_mode_Private_fn_keys_hp = 1052,
    dansi_mode_Private_fn_keys_sco = 1053,
    dansi_mode_Private_keyboard_legacy = 1060,
    dansi_mode_Private_keyboard_vt220 = 1061,
    dansi_mode_Private_readline_mouse_btn1 = 2001,
    dansi_mode_Private_readline_mouse_btn2 = 2002,
    dansi_mode_Private_readline_mouse_btn3 = 2003,
    dansi_mode_Private_bracketed_paste = 2004,
    dansi_mode_Private_readline_quote = 2005,
    dansi_mode_Private_readline_newline_paste = 2006,
    dansi_mode_Private_synchronized_output = 2026
)) dansi_mode_Private;
#define dansi_mode_Private_staticParse(_mode_tok) \
    ____dansi_mode_Private_staticParse(_mode_tok)

typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_mode_PrivateBuf;

#define dansi_mode_enablePrivate_static(_mode_tok) \
    ____dansi_mode_enablePrivate_static(_mode_tok)
$extern fn_((dansi_mode_enablePrivate(u16 mode, dansi_mode_PrivateBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_enablePrivateWrite(u16 mode, io_Writer writer))(E$void));

#define dansi_mode_disablePrivate_static(_mode_tok) \
    ____dansi_mode_disablePrivate_static(_mode_tok)
$extern fn_((dansi_mode_disablePrivate(u16 mode, dansi_mode_PrivateBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_disablePrivateWrite(u16 mode, io_Writer writer))(E$void));

#define dansi_mode_setPrivate_static(_mode_tok, _pp_enabled) \
    ____dansi_mode_setPrivate_static(_mode_tok, _pp_enabled)
$extern fn_((dansi_mode_setPrivate(u16 mode, bool enabled, dansi_mode_PrivateBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_setPrivateWrite(u16 mode, bool enabled, io_Writer writer))(E$void));

#define dansi_mode_savePrivate_static(_mode_tok) \
    ____dansi_mode_savePrivate_static(_mode_tok)
$extern fn_((dansi_mode_savePrivate(u16 mode, dansi_mode_PrivateBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_savePrivateWrite(u16 mode, io_Writer writer))(E$void));

#define dansi_mode_restorePrivate_static(_mode_tok) \
    ____dansi_mode_restorePrivate_static(_mode_tok)
$extern fn_((dansi_mode_restorePrivate(u16 mode, dansi_mode_PrivateBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_restorePrivateWrite(u16 mode, io_Writer writer))(E$void));

#define dansi_mode_requestPrivate_static(_mode_tok) \
    ____dansi_mode_requestPrivate_static(_mode_tok)
$extern fn_((dansi_mode_requestPrivate(u16 mode, dansi_mode_PrivateBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_requestPrivateWrite(u16 mode, io_Writer writer))(E$void));

typedef enum_((dansi_mode_ANSI $fits($packed))(
    dansi_mode_ANSI_insert = 4,
    dansi_mode_ANSI_line_feed_new_line = 20,
)) dansi_mode_ANSI;
#define dansi_mode_ANSI_staticParse(_mode_tok) \
    ____dansi_mode_ANSI_staticParse(_mode_tok)

typedef A$$(3 + uint_log10Ceil_static(u16_limit_max), u8) dansi_mode_ANSIBuf;

#define dansi_mode_enableANSI_static(_mode_tok) \
    ____dansi_mode_enableANSI_static(_mode_tok)
$extern fn_((dansi_mode_enableANSI(u16 mode, dansi_mode_ANSIBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_enableANSIWrite(u16 mode, io_Writer writer))(E$void));

#define dansi_mode_disableANSI_static(_mode_tok) \
    ____dansi_mode_disableANSI_static(_mode_tok)
$extern fn_((dansi_mode_disableANSI(u16 mode, dansi_mode_ANSIBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_disableANSIWrite(u16 mode, io_Writer writer))(E$void));

#define dansi_mode_setANSI_static(_mode_tok, _pp_enabled) \
    ____dansi_mode_setANSI_static(_mode_tok, _pp_enabled)
$extern fn_((dansi_mode_setANSI(u16 mode, bool enabled, dansi_mode_ANSIBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_setANSIWrite(u16 mode, bool enabled, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_mode_Private_staticParse(_mode_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_mode_tok))( \
        pp_Tok_then_(#_mode_tok), \
        pp_Tok_else_(pp_join($, ____dansi_mode__pp_Private_str, _mode_tok)) \
    )
#define ____dansi_mode_enablePrivate_static(_mode_tok) \
    dansi_utils_csi dansi_utils_mode_private_prefix _mode_tok dansi_utils_mode_set
#define ____dansi_mode_disablePrivate_static(_mode_tok) \
    dansi_utils_csi dansi_utils_mode_private_prefix _mode_tok dansi_utils_mode_reset
#define ____dansi_mode_setPrivate_static(_mode_tok, _pp_enabled) \
    pp_if_(_pp_enabled)( \
        pp_then_(dansi_mode_enablePrivate_static(_mode_tok)), \
        pp_else_(dansi_mode_disablePrivate_static(_mode_tok)) \
    )
#define ____dansi_mode_savePrivate_static(_mode_tok) \
    dansi_utils_csi dansi_utils_mode_private_prefix _mode_tok "s"
#define ____dansi_mode_restorePrivate_static(_mode_tok) \
    dansi_utils_csi dansi_utils_mode_private_prefix _mode_tok "r"
#define ____dansi_mode_requestPrivate_static(_mode_tok) \
    dansi_utils_csi dansi_utils_mode_private_prefix _mode_tok "$p"
#define ____dansi_mode_ANSI_staticParse(_mode_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_mode_tok))( \
        pp_Tok_then_(#_mode_tok), \
        pp_Tok_else_(pp_join($, ____dansi_mode__pp_ANSI_str, _mode_tok)) \
    )
#define ____dansi_mode_enableANSI_static(_mode_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_mode_set, _mode_tok)
#define ____dansi_mode_disableANSI_static(_mode_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_mode_reset, _mode_tok)
#define ____dansi_mode_setANSI_static(_mode_tok, _pp_enabled) \
    pp_if_(_pp_enabled)( \
        pp_then_(dansi_mode_enableANSI_static(_mode_tok)), \
        pp_else_(dansi_mode_disableANSI_static(_mode_tok)) \
    )

#pragma region "pp private mode str"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_cursor_keys "1"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_132_col "3"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_origin "6"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_auto_wrap "7"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_mouse_x10 "9"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_toolbar "10"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_blink_cursor "12"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_blink_cursor_xor "14"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_show_cursor "25"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_scrollbar "30"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_allow_80_to_132 "40"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_margin_bell "44"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_reverse_wrap "45"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_alt_screen_47 "47"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_app_keypad "66"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_left_right_margin "69"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_no_clear_col_mode "95"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_mouse_normal "1000"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_mouse_hilite "1001"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_mouse_button_event "1002"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_mouse_any_event "1003"
#define pp_Tok_cmp$1004(_x) _x
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_focus_events "1004"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_mouse_utf8 "1005"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_mouse_sgr "1006"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_alt_scroll "1007"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_scroll_on_tty_output "1010"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_scroll_on_key "1011"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_fast_scroll "1014"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_mouse_urxvt "1015"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_mouse_sgr_pixels "1016"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_meta_8bit "1034"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_alt_numlock_mods "1035"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_meta_sends_esc "1036"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_delete_sends_del "1037"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_alt_sends_esc "1039"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_keep_selection "1040"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_select_to_clipboard "1041"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_bell_urgent "1042"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_pop_on_bell "1043"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_keep_clipboard "1044"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_reverse_wrap_extended "1045"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_allow_alt_screen "1046"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_alt_screen_1047 "1047"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_save_cursor "1048"
#define pp_Tok_cmp$1049(_x) _x
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_alt_screen "1049"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_fn_keys_terminfo "1050"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_fn_keys_sun "1051"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_fn_keys_hp "1052"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_fn_keys_sco "1053"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_keyboard_legacy "1060"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_keyboard_vt220 "1061"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_readline_mouse_btn1 "2001"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_readline_mouse_btn2 "2002"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_readline_mouse_btn3 "2003"
#define pp_Tok_cmp$2004(_x) _x
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_bracketed_paste "2004"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_readline_quote "2005"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_readline_newline_paste "2006"
#define pp_Tok_cmp$2026(_x) _x
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_synchronized_output "2026"
#pragma endregion "pp private mode str"

#pragma region "pp ansi mode str"
#define ____dansi_mode__pp_ANSI_str$dansi_mode_ANSI_insert "4"
#define ____dansi_mode__pp_ANSI_str$dansi_mode_ANSI_line_feed_new_line "20"
#pragma endregion "pp ansi mode str"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_mode__included */

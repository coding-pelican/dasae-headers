/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    utils.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_utils
 *
 * @brief   Low-level ANSI escape sequence utilities
 * @details Provides constants and helper functions for building ANSI sequences
 */
#ifndef daterm_utils__included
#define daterm_utils__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/Writer.h>

/*========== Macros and Declarations ========================================*/

/* --- CSI Sequences --- */

/// Escape (ESC)
#define daterm_utils_esc "\x1b"
/// Control Sequence Introducer (ESC[)
#define daterm_utils_csi daterm_utils_esc "["
/// Operating System Command (ESC])
#define daterm_utils_osc daterm_utils_esc "]"
/// Device Control String (ESC P)
#define daterm_utils_dcs daterm_utils_esc "P"
/// String Terminator (ESC\)
#define daterm_utils_st daterm_utils_esc "\\"
/// Separator (';')
#define daterm_utils_sep ";"

/* --- Attribute Sequences --- */

/// Attribute Terminator ('m')
#define daterm_utils_attr "m"
/// Reset all attributes (color and style)
#define daterm_utils_attr_reset "0" daterm_utils_attr

/* --- Color Sequences --- */

/// Foreground 256-color prefix (ESC[38;5;)
#define daterm_utils_attr_color_fg_8bit "38;5;"
/// Background 256-color prefix (ESC[48;5;)
#define daterm_utils_attr_color_bg_8bit "48;5;"
/// Foreground RGB prefix (ESC[38;2;)
#define daterm_utils_attr_color_fg_24bit "38;2;"
/// Background RGB prefix (ESC[48;2;)
#define daterm_utils_attr_color_bg_24bit "48;2;"

/* --- Style Sequences --- */

/// Bold on
#define daterm_utils_attr_style_bold "1" daterm_utils_attr
/// Bold off (normal intensity)
#define daterm_utils_attr_style_no_bold "22" daterm_utils_attr

/// Dim/faint on
#define daterm_utils_attr_style_dim "2" daterm_utils_attr
/// Dim off (normal intensity)
#define daterm_utils_attr_style_no_dim "22" daterm_utils_attr

/// Italic on
#define daterm_utils_attr_style_italic "3" daterm_utils_attr
/// Italic off
#define daterm_utils_attr_style_no_italic "23" daterm_utils_attr

/// Underline on
#define daterm_utils_attr_style_underline "4" daterm_utils_attr
/// Underline off
#define daterm_utils_attr_style_no_underline "24" daterm_utils_attr

/// Blinking on
#define daterm_utils_attr_style_blinking "5" daterm_utils_attr
/// Blinking off
#define daterm_utils_attr_style_no_blinking "25" daterm_utils_attr

/// Reverse video on (swap fg/bg)
#define daterm_utils_attr_style_reverse "7" daterm_utils_attr
/// Reverse video off
#define daterm_utils_attr_style_no_reverse "27" daterm_utils_attr

/// Hidden/invisible on
#define daterm_utils_attr_style_invisible "8" daterm_utils_attr
/// Hidden/invisible off
#define daterm_utils_attr_style_no_invisible "28" daterm_utils_attr

/// Strikethrough on
#define daterm_utils_attr_style_strikethrough "9" daterm_utils_attr
/// Strikethrough off
#define daterm_utils_attr_style_no_strikethrough "29" daterm_utils_attr

/* --- Screen Sequences --- */

/// Enter alternate screen buffer
#define daterm_utils_screen_enter_alternate "?1049h"
/// Exit alternate screen buffer
#define daterm_utils_screen_exit_alternate "?1049l"

/// Clear from cursor to end of screen
#define daterm_utils_screen_clear_from_cursor "0J"
/// Clear from start of screen to cursor
#define daterm_utils_screen_clear_to_cursor "1J"
/// Clear entire screen
#define daterm_utils_screen_clear "2J"

/* --- Line Sequences --- */

/// Scroll up by n lines
#define daterm_utils_line_scroll_dir_up /*{n}*/ "S"
/// Scroll down by n lines
#define daterm_utils_line_scroll_dir_down /*{n}*/ "T"

/// Clear from cursor to end of line
#define daterm_utils_line_clear_from_cursor "0K"
/// Clear from start of line to cursor
#define daterm_utils_line_clear_to_cursor "1K"
/// Clear entire line
#define daterm_utils_line_clear "2K"

/* --- Cursor Sequences --- */

/// Move cursor to position
#define daterm_utils_cursor_move_pos /*{row};{col}*/ "H"
/// Move cursor up
#define daterm_utils_cursor_move_dir_up /*{rows}*/ "A"
/// Move cursor down
#define daterm_utils_cursor_move_dir_dn /*{rows}*/ "B"
/// Move cursor forward (right)
#define daterm_utils_cursor_move_dir_fwd /*{cols}*/ "C"
/// Move cursor backward (left)
#define daterm_utils_cursor_move_dir_bwd /*{cols}*/ "D"

/// Hide cursor
#define daterm_utils_cursor_hide "?25l"
/// Show cursor
#define daterm_utils_cursor_show "?25h"

/// Store cursor position
#define daterm_utils_cursor_store_pos "s"
/// Restore cursor position
#define daterm_utils_cursor_restore_pos "u"

/// Request cursor position (response: ESC[{row};{col}R)
#define daterm_utils_cursor_request_pos "6n"
#define daterm_utils_cursor_response_pos /*{row};{col}*/ "R"

/* --- Mouse Sequences --- */

/// Enable mouse tracking (all events)
#define daterm_utils_mouse_enable "?1003h"
/// Disable mouse tracking
#define daterm_utils_mouse_disable "?1003l"
/// Enable SGR extended mouse mode
#define daterm_utils_mouse_sgr_enable "?1006h"
/// Disable SGR extended mouse mode
#define daterm_utils_mouse_sgr_disable "?1006l"

/* --- CSI --- */

typedef A$$(32, u8) daterm_utils_CSIBuf;
#define daterm_utils_formatCSI_static(_code_tok) \
    ____daterm_utils_formatCSI_static(_code_tok)
/// Format a complete CSI sequence from a code string
/// Example: formatCSI("2J") -> "\x1b[2J"
$extern fn_((daterm_utils_formatCSI(S_const$u8 code, daterm_utils_CSIBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((daterm_utils_formatCSIWrite(S_const$u8 code, io_Writer writer))(E$void));

typedef A$$(32, u8) daterm_utils_CSI_u16Buf;
#define daterm_utils_formatCSI_u16_static(_code_token, _val_token) \
    ____daterm_utils_formatCSI_u16_static(_code_token, _val_token)
/// Format CSI sequence with one numeric parameter
/// Example: formatCSI_u16("A", 5) -> "\x1b[5A"
$extern fn_((daterm_utils_formatCSI_u16(S_const$u8 code, u16 val, daterm_utils_CSI_u16Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((daterm_utils_formatCSI_u16Write(S_const$u8 code, u16 val, io_Writer writer))(E$void));

typedef A$$(64, u8) daterm_utils_CSI_u16x2Buf;
#define daterm_utils_formatCSI_u16x2_static(_code_token, _val1_token, _val2_token) \
    ____daterm_utils_formatCSI_u16x2_static(_code_token, _val1_token, _val2_token)
/// Format CSI sequence with two numeric parameters
/// Example: formatCSI_u16x2("H", 10, 20) -> "\x1b[10;20H"
$extern fn_((daterm_utils_formatCSI_u16x2(S_const$u8 code, u16 val1, u16 val2, daterm_utils_CSI_u16x2Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((daterm_utils_formatCSI_u16x2Write(S_const$u8 code, u16 val1, u16 val2, io_Writer writer))(E$void));

typedef A$$(32, u8) daterm_utils_CSI_u8x3Buf;
#define daterm_utils_formatCSI_u8x3_static(_code_token, _val1_token, _val2_token, _val3_token) \
    ____daterm_utils_formatCSI_u8x3_static(_code_token, _val1_token, _val2_token, _val3_token)
/// Format CSI sequence with three numeric parameters
/// Example: formatCSI_u8x3("m", 38, 2, 255) -> "\x1b[38;2;255m"
$extern fn_((daterm_utils_formatCSI_u8x3(S_const$u8 code, u8 val1, u8 val2, u8 val3, daterm_utils_CSI_u8x3Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((daterm_utils_formatCSI_u8x3Write(S_const$u8 code, u8 val1, u8 val2, u8 val3, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____daterm_utils_formatCSI_static(_code_token) \
    daterm_utils_csi _code_token
#define ____daterm_utils_formatCSI_u16_static(_code_token, _val_token) \
    daterm_utils_csi _val_token daterm_utils_sep _code_token
#define ____daterm_utils_formatCSI_u16x2_static(_code_token, _val1_token, _val2_token) \
    daterm_utils_csi _val1_token daterm_utils_sep _val2_token daterm_utils_sep _code_token
#define ____daterm_utils_formatCSI_u8x3_static(_code_token, _val1_token, _val2_token, _val3_token) \
    daterm_utils_csi _val1_token daterm_utils_sep _val2_token daterm_utils_sep _val3_token daterm_utils_sep _code_token

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_utils__included */

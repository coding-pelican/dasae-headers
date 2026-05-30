/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    utils.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-05-30 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_utils
 *
 * @brief   Low-level ANSI escape sequence utilities
 * @details Provides constants and helper functions for building ANSI sequences
 */
#ifndef dansi_utils__included
#define dansi_utils__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/Writer.h>

/*========== Macros and Declarations ========================================*/

/* --- CSI Sequences --- */

/// Escape (ESC)
#define dansi_utils_esc "\x1b"
/// Control Sequence Introducer (ESC[)
#define dansi_utils_csi dansi_utils_esc "["
/// Operating System Command (ESC])
#define dansi_utils_osc dansi_utils_esc "]"
/// Device Control String (ESC P)
#define dansi_utils_dcs dansi_utils_esc "P"
/// String Terminator (ESC\)
#define dansi_utils_st dansi_utils_esc "\\"
/// Separator (';')
#define dansi_utils_sep ";"

/* --- Device Sequences --- */

/// Request device status report
#define dansi_utils_device_request_status "5n"
/// Device status OK report
#define dansi_utils_device_response_status_ok "0n"
/// Device status malfunction report
#define dansi_utils_device_response_status_malformed "3n"
/// Request primary device attributes
#define dansi_utils_device_request_pri_attrs "c"
/// Request secondary device attributes
#define dansi_utils_device_request_sec_attrs ">c"
/// Request tertiary device attributes
#define dansi_utils_device_request_ter_attrs "=c"
/// Device attributes report terminator
#define dansi_utils_device_response_attrs /*{params}*/ "c"

/* --- Mode Sequences --- */

/// Set mode
#define dansi_utils_mode_set "h"
/// Reset mode
#define dansi_utils_mode_reset "l"
/// Private mode prefix
#define dansi_utils_mode_private_prefix "?"

/* --- Title Sequences --- */

/// OSC set icon and window title (ESC ] 0 ; {title} ST)
#define dansi_utils_osc_title_both "0"
/// OSC set icon title (ESC ] 1 ; {title} ST)
#define dansi_utils_osc_title_icon "1"
/// OSC set window title (ESC ] 2 ; {title} ST)
#define dansi_utils_osc_title_window "2"
/// Push title stack (CSI 22 ; {target} t)
#define dansi_utils_title_stack_push /*{target}*/ "22"
/// Pop title stack (CSI 23 ; {target} t)
#define dansi_utils_title_stack_pop /*{target}*/ "23"

/* --- Screen Sequences --- */

/// Enter alternate screen buffer
#define dansi_utils_screen_enter_alternate "?1049h"
/// Exit alternate screen buffer
#define dansi_utils_screen_exit_alternate "?1049l"

/// Clear from cursor to end of screen
#define dansi_utils_screen_clear_from_cursor "0J"
/// Clear from start of screen to cursor
#define dansi_utils_screen_clear_to_cursor "1J"
/// Clear entire screen
#define dansi_utils_screen_clear "2J"

/// Request cell size in pixels
#define dansi_utils_screen_request_cell_size_pixels "16t"
/// Response cell size in pixels: CSI 6 ; height ; width t
#define dansi_utils_screen_response_cell_size_pixels "6"
/// Request text area size in characters
#define dansi_utils_screen_request_text_area_size_chars "18t"
/// Response text area size in characters: CSI 8 ; height ; width t
#define dansi_utils_screen_response_text_area_size_chars "8"
/// Request text area size in pixels
#define dansi_utils_screen_request_text_area_size_pixels "14t"
/// Response text area size in pixels: CSI 4 ; height ; width t
#define dansi_utils_screen_response_text_area_size_pixels "4"
#define dansi_utils_screen_response_size /*{kind};{height};{width}*/ "t"

/* --- Scroll Sequences --- */

/// Set scrolling region
#define dansi_utils_scroll_set_region /*{top};{bottom}*/ "r"
/// Reset scrolling region
#define dansi_utils_scroll_reset_region "r"

/* --- Line Sequences --- */

/// Scroll up by n lines
#define dansi_utils_line_scroll_dir_up /*{n}*/ "S"
/// Scroll down by n lines
#define dansi_utils_line_scroll_dir_down /*{n}*/ "T"

/// Clear from cursor to end of line
#define dansi_utils_line_clear_from_cursor "0K"
/// Clear from start of line to cursor
#define dansi_utils_line_clear_to_cursor "1K"
/// Clear entire line
#define dansi_utils_line_clear "2K"

/* --- Cursor Sequences --- */

/// Move cursor to position
#define dansi_utils_cursor_move_pos /*{row};{col}*/ "H"
/// Move cursor up
#define dansi_utils_cursor_move_dir_up /*{rows}*/ "A"
/// Move cursor down
#define dansi_utils_cursor_move_dir_dn /*{rows}*/ "B"
/// Move cursor forward (right)
#define dansi_utils_cursor_move_dir_fwd /*{cols}*/ "C"
/// Move cursor backward (left)
#define dansi_utils_cursor_move_dir_bwd /*{cols}*/ "D"
/// Move cursor to next line
#define dansi_utils_cursor_move_next_line /*{rows}*/ "E"
/// Move cursor to previous line
#define dansi_utils_cursor_move_prev_line /*{rows}*/ "F"
/// Move cursor to absolute column
#define dansi_utils_cursor_move_abs_col /*{col}*/ "G"
/// Move cursor to absolute row
#define dansi_utils_cursor_move_abs_row /*{row}*/ "d"

/// Hide cursor
#define dansi_utils_cursor_hide "?25l"
/// Show cursor
#define dansi_utils_cursor_show "?25h"

/// Store cursor position
#define dansi_utils_cursor_store_pos "s"
/// Restore cursor position
#define dansi_utils_cursor_restore_pos "u"
/// Set cursor style
#define dansi_utils_cursor_style /*{style}*/ " q"

/// Set horizontal tab stop
#define dansi_utils_cursor_tab_set "H"
/// Clear horizontal tab stop
#define dansi_utils_cursor_tab_clear_current "0g"
/// Clear all horizontal tab stops
#define dansi_utils_cursor_tab_clear_all "3g"

/// Request cursor position (response: ESC[{row};{col}R)
#define dansi_utils_cursor_request_pos "6n"
#define dansi_utils_cursor_response_pos /*{row};{col}*/ "R"

/* --- Attribute Sequences --- */

/// Attribute Terminator ('m')
#define dansi_utils_attr "m"
/// Reset all attributes (color and style)
#define dansi_utils_attr_reset "0" dansi_utils_attr
/// Reset foreground color
#define dansi_utils_attr_reset_fg "39" dansi_utils_attr
/// Reset background color
#define dansi_utils_attr_reset_bg "49" dansi_utils_attr

/* --- Color Sequences --- */

/// Foreground 256-color prefix (ESC[38;5;)
#define dansi_utils_attr_color_fg_8bit "38;5;"
/// Background 256-color prefix (ESC[48;5;)
#define dansi_utils_attr_color_bg_8bit "48;5;"
/// Foreground RGB prefix (ESC[38;2;)
#define dansi_utils_attr_color_fg_24bit "38;2;"
/// Background RGB prefix (ESC[48;2;)
#define dansi_utils_attr_color_bg_24bit "48;2;"

/* --- Style Sequences --- */

/// Bold on
#define dansi_utils_attr_style_bold "1" dansi_utils_attr
/// Bold off (normal intensity)
#define dansi_utils_attr_style_no_bold "22" dansi_utils_attr

/// Dim/faint on
#define dansi_utils_attr_style_dim "2" dansi_utils_attr
/// Dim off (normal intensity)
#define dansi_utils_attr_style_no_dim "22" dansi_utils_attr

/// Italic on
#define dansi_utils_attr_style_italic "3" dansi_utils_attr
/// Italic off
#define dansi_utils_attr_style_no_italic "23" dansi_utils_attr

/// Underline on
#define dansi_utils_attr_style_underline "4" dansi_utils_attr
/// Underline off
#define dansi_utils_attr_style_no_underline "24" dansi_utils_attr

/// Blinking on
#define dansi_utils_attr_style_blinking "5" dansi_utils_attr
/// Blinking off
#define dansi_utils_attr_style_no_blinking "25" dansi_utils_attr

/// Reverse video on (swap fg/bg)
#define dansi_utils_attr_style_reverse "7" dansi_utils_attr
/// Reverse video off
#define dansi_utils_attr_style_no_reverse "27" dansi_utils_attr

/// Hidden/invisible on
#define dansi_utils_attr_style_invisible "8" dansi_utils_attr
/// Hidden/invisible off
#define dansi_utils_attr_style_no_invisible "28" dansi_utils_attr

/// Strikethrough on
#define dansi_utils_attr_style_strikethrough "9" dansi_utils_attr
/// Strikethrough off
#define dansi_utils_attr_style_no_strikethrough "29" dansi_utils_attr

/* --- CSI --- */

typedef A$$(32, u8) dansi_utils_CSIBuf;
#define dansi_utils_formatCSI_static(_code_tok) \
    ____dansi_utils_formatCSI_static(_code_tok)
/// Format a complete CSI sequence from a code string
/// Example: formatCSI("2J") -> "\x1b[2J"
$extern fn_((dansi_utils_formatCSI(S_const$u8 code, dansi_utils_CSIBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_utils_formatCSIWrite(S_const$u8 code, io_Writer writer))(E$void));

typedef A$$(32, u8) dansi_utils_CSI_u16Buf;
#define dansi_utils_formatCSI_u16_static(_code_token, _val_token) \
    ____dansi_utils_formatCSI_u16_static(_code_token, _val_token)
/// Format CSI sequence with one numeric parameter
/// Example: formatCSI_u16("A", 5) -> "\x1b[5A"
$extern fn_((dansi_utils_formatCSI_u16(S_const$u8 code, u16 val, dansi_utils_CSI_u16Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_utils_formatCSI_u16Write(S_const$u8 code, u16 val, io_Writer writer))(E$void));

typedef A$$(64, u8) dansi_utils_CSI_u16x2Buf;
#define dansi_utils_formatCSI_u16x2_static(_code_token, _val1_token, _val2_token) \
    ____dansi_utils_formatCSI_u16x2_static(_code_token, _val1_token, _val2_token)
/// Format CSI sequence with two numeric parameters
/// Example: formatCSI_u16x2("H", 10, 20) -> "\x1b[10;20H"
$extern fn_((dansi_utils_formatCSI_u16x2(S_const$u8 code, u16 val1, u16 val2, dansi_utils_CSI_u16x2Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_utils_formatCSI_u16x2Write(S_const$u8 code, u16 val1, u16 val2, io_Writer writer))(E$void));

typedef A$$(32, u8) dansi_utils_CSI_u8x3Buf;
#define dansi_utils_formatCSI_u8x3_static(_code_token, _val1_token, _val2_token, _val3_token) \
    ____dansi_utils_formatCSI_u8x3_static(_code_token, _val1_token, _val2_token, _val3_token)
/// Format CSI sequence with three numeric parameters
/// Example: formatCSI_u8x3("m", 38, 2, 255) -> "\x1b[38;2;255m"
$extern fn_((dansi_utils_formatCSI_u8x3(S_const$u8 code, u8 val1, u8 val2, u8 val3, dansi_utils_CSI_u8x3Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_utils_formatCSI_u8x3Write(S_const$u8 code, u8 val1, u8 val2, u8 val3, io_Writer writer))(E$void));

/* --- OSC / DCS / Raw --- */

typedef A$$(32, u8) dansi_utils_DCSBuf;
#define dansi_utils_formatOSC_static(_code_tok, _payload_tok) \
    ____dansi_utils_formatOSC_static(_code_tok, _payload_tok)
$extern fn_((dansi_utils_formatOSC(S_const$u8 code, S_const$u8 payload, S$u8 buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_utils_formatOSCWrite(S_const$u8 code, S_const$u8 payload, io_Writer writer))(E$void));
#define dansi_utils_formatDCS_static(_payload_tok) \
    ____dansi_utils_formatDCS_static(_payload_tok)
$extern fn_((dansi_utils_formatDCS(S_const$u8 payload, S$u8 buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_utils_formatDCSWrite(S_const$u8 payload, io_Writer writer))(E$void));
#define dansi_utils_raw_static(_bytes_tok) \
    ____dansi_utils_raw_static(_bytes_tok)
$extern fn_((dansi_utils_raw(S_const$u8 bytes))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_utils_rawWrite(S_const$u8 bytes, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____dansi_utils_formatCSI_static(_code_token) \
    dansi_utils_csi _code_token
#define ____dansi_utils_formatCSI_u16_static(_code_token, _val_token) \
    dansi_utils_csi _val_token _code_token
#define ____dansi_utils_formatCSI_u16x2_static(_code_token, _val1_token, _val2_token) \
    dansi_utils_csi _val1_token dansi_utils_sep _val2_token _code_token
#define ____dansi_utils_formatCSI_u8x3_static(_code_token, _val1_token, _val2_token, _val3_token) \
    dansi_utils_csi _val1_token dansi_utils_sep _val2_token dansi_utils_sep _val3_token _code_token
#define ____dansi_utils_formatOSC_static(_code_tok, _payload_tok) \
    dansi_utils_osc _code_tok dansi_utils_sep _payload_tok dansi_utils_st
#define ____dansi_utils_formatDCS_static(_payload_tok) \
    dansi_utils_dcs _payload_tok dansi_utils_st
#define ____dansi_utils_raw_static(_bytes_tok) \
    _bytes_tok

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_utils__included */

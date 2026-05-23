/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    cursor.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-21 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_cursor
 *
 * @brief   Terminal cursor position and visibility control
 */
#ifndef dansi_cursor__included
#define dansi_cursor__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

/* --- Movement --- */

typedef A$$(4 + uint_log10Ceil_static(u16_limit_max) * 2, u8) dansi_cursor_MovePosBuf;

#define dansi_cursor_moveTo_static(_col_tok, _row_tok) \
    ____dansi_cursor_moveTo_static(_col_tok, _row_tok)
/// Get ANSI sequence to move cursor to position
/// Returns static string - valid until next call
$extern fn_((dansi_cursor_moveTo(u16 col, u16 row, dansi_cursor_MovePosBuf* buf))(S$u8));
/// Write ANSI sequence to move cursor to position
$attr($must_check)
$extern fn_((dansi_cursor_moveToWrite(u16 col, u16 row, io_Writer writer))(E$void));

typedef A$$(2 + uint_log10Ceil_static(u16_limit_max), u8) dansi_cursor_MoveDirBuf;

#define dansi_cursor_moveUp_static(_rows_tok) \
    ____dansi_cursor_moveUp_static(_rows_tok)
/// Get ANSI sequence to move cursor up
$extern fn_((dansi_cursor_moveUp(u16 rows, dansi_cursor_MoveDirBuf* buf))(S$u8));
/// Write ANSI sequence to move cursor up
$attr($must_check)
$extern fn_((dansi_cursor_moveUpWrite(u16 rows, io_Writer writer))(E$void));

#define dansi_cursor_moveDown_static(_rows_tok) \
    ____dansi_cursor_moveDown_static(_rows_tok)
/// Get ANSI sequence to move cursor down
$extern fn_((dansi_cursor_moveDown(u16 rows, dansi_cursor_MoveDirBuf* buf))(S$u8));
/// Write ANSI sequence to move cursor down
$attr($must_check)
$extern fn_((dansi_cursor_moveDownWrite(u16 rows, io_Writer writer))(E$void));

#define dansi_cursor_moveRight_static(_cols_tok) \
    ____dansi_cursor_moveRight_static(_cols_tok)
/// Get ANSI sequence to move cursor right
$extern fn_((dansi_cursor_moveRight(u16 cols, dansi_cursor_MoveDirBuf* buf))(S$u8));
/// Write ANSI sequence to move cursor right
$attr($must_check)
$extern fn_((dansi_cursor_moveRightWrite(u16 cols, io_Writer writer))(E$void));

#define dansi_cursor_moveLeft_static(_cols_tok) \
    ____dansi_cursor_moveLeft_static(_cols_tok)
/// Get ANSI sequence to move cursor left
$extern fn_((dansi_cursor_moveLeft(u16 cols, dansi_cursor_MoveDirBuf* buf))(S$u8));
/// Write ANSI sequence to move cursor left
$attr($must_check)
$extern fn_((dansi_cursor_moveLeftWrite(u16 cols, io_Writer writer))(E$void));

#define dansi_cursor_moveToRow_static(_row_tok) \
    ____dansi_cursor_moveToRow_static(_row_tok)
$extern fn_((dansi_cursor_moveToRow(u16 row, dansi_cursor_MoveDirBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveToRowWrite(u16 row, io_Writer writer))(E$void));

#define dansi_cursor_moveToCol_static(_col_tok) \
    ____dansi_cursor_moveToCol_static(_col_tok)
$extern fn_((dansi_cursor_moveToCol(u16 col, dansi_cursor_MoveDirBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveToColWrite(u16 col, io_Writer writer))(E$void));

#define dansi_cursor_moveNextLine_static(_rows_tok) \
    ____dansi_cursor_moveNextLine_static(_rows_tok)
$extern fn_((dansi_cursor_moveNextLine(u16 rows, dansi_cursor_MoveDirBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveNextLineWrite(u16 rows, io_Writer writer))(E$void));

#define dansi_cursor_movePrevLine_static(_rows_tok) \
    ____dansi_cursor_movePrevLine_static(_rows_tok)
$extern fn_((dansi_cursor_movePrevLine(u16 rows, dansi_cursor_MoveDirBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_movePrevLineWrite(u16 rows, io_Writer writer))(E$void));

/* --- Visibility --- */

#define dansi_cursor_hide_static() \
    ____dansi_cursor_hide_static()
/// Get ANSI sequence to hide cursor
$extern fn_((dansi_cursor_hide(void))(S_const$u8));
/// Write ANSI sequence to hide cursor
$attr($must_check)
$extern fn_((dansi_cursor_hideWrite(io_Writer writer))(E$void));

#define dansi_cursor_show_static() \
    ____dansi_cursor_show_static()
/// Get ANSI sequence to show cursor
$extern fn_((dansi_cursor_show(void))(S_const$u8));
/// Write ANSI sequence to show cursor
$attr($must_check)
$extern fn_((dansi_cursor_showWrite(io_Writer writer))(E$void));

/* --- Store/Restore --- */

#define dansi_cursor_storePos_static() \
    ____dansi_cursor_storePos_static()
/// Get ANSI sequence to store cursor position
$extern fn_((dansi_cursor_storePos(void))(S_const$u8));
/// Write ANSI sequence to store cursor position
$attr($must_check)
$extern fn_((dansi_cursor_storePosWrite(io_Writer writer))(E$void));

#define dansi_cursor_restorePos_static() \
    ____dansi_cursor_restorePos_static()
/// Get ANSI sequence to restore cursor position
$extern fn_((dansi_cursor_restorePos(void))(S_const$u8));
/// Write ANSI sequence to restore cursor position
$attr($must_check)
$extern fn_((dansi_cursor_restorePosWrite(io_Writer writer))(E$void));

/* --- Style / Tab Stops --- */

typedef A$$(3 + uint_log10Ceil_static(u16_limit_max), u8) dansi_cursor_StyleBuf;

typedef enum_((dansi_cursor_Style $fits($packed))(
    dansi_cursor_Style_default = 0,
    dansi_cursor_Style_block_blink = 1,
    dansi_cursor_Style_block = 2,
    dansi_cursor_Style_underline_blink = 3,
    dansi_cursor_Style_underline = 4,
    dansi_cursor_Style_bar_blink = 5,
    dansi_cursor_Style_bar = 6,
)) dansi_cursor_Style;
#define dansi_cursor_Style_staticParse(_style_tok) \
    ____dansi_cursor_Style_staticParse(_style_tok)

#define dansi_cursor_setStyle_static(_style_tok) \
    ____dansi_cursor_setStyle_static(_style_tok)
$extern fn_((dansi_cursor_setStyle(dansi_cursor_Style style, dansi_cursor_StyleBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_setStyleWrite(dansi_cursor_Style style, io_Writer writer))(E$void));

#define dansi_cursor_setTabStop_static() \
    ____dansi_cursor_setTabStop_static()
$extern fn_((dansi_cursor_setTabStop(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_cursor_setTabStopWrite(io_Writer writer))(E$void));

#define dansi_cursor_clearTabStop_static() \
    ____dansi_cursor_clearTabStop_static()
$extern fn_((dansi_cursor_clearTabStop(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_cursor_clearTabStopWrite(io_Writer writer))(E$void));

#define dansi_cursor_clearAllTabStops_static() \
    ____dansi_cursor_clearAllTabStops_static()
$extern fn_((dansi_cursor_clearAllTabStops(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_cursor_clearAllTabStopsWrite(io_Writer writer))(E$void));

/* --- Position Report --- */

/// Cursor position (1-indexed as per ANSI spec)
typedef struct dansi_cursor_Pos {
    var_(row, u16); // 1-indexed
    var_(col, u16); // 1-indexed
} dansi_cursor_Pos;
T_use_prl$(dansi_cursor_Pos);
errset_((dansi_cursor_E)(dansi_cursor_InvalidResponse));
T_use_E$($set(dansi_cursor_E)(dansi_cursor_Pos));
/// Write cursor position request.
typedef A$$(4 + uint_log10Ceil_static(u16_limit_max) * 2, u8) dansi_cursor_PosReportBuf;
#define dansi_cursor_requestPos_static() \
    ____dansi_cursor_requestPos_static()
$extern fn_((dansi_cursor_requestPos(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_cursor_requestPosWrite(io_Writer out))(E$void));
/// Receive cursor position report bytes from `in`.
$attr($must_check)
$extern fn_((dansi_cursor_receivePosReport(io_Reader in, S$u8 buf))(E$S$u8));
/// Parse already-read cursor position report bytes.
$attr($must_check)
$extern fn_((dansi_cursor_parsePosReport(S_const$u8 report))(dansi_cursor_E$dansi_cursor_Pos));
/// Request, receive, and parse cursor position.
$attr($must_check)
$extern fn_((dansi_cursor_fetchPos(io_Writer out, io_Reader in, S$u8 buf))(dansi_cursor_E$dansi_cursor_Pos));

/*========== Macros and Definitions =========================================*/

#include "dansi-core/utils.h"

#define ____dansi_cursor_moveTo_static(_col_tok, _row_tok) \
    dansi_utils_formatCSI_u16x2_static(dansi_utils_cursor_move_pos, _row_tok, _col_tok)
#define ____dansi_cursor_moveUp_static(_rows_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_cursor_move_dir_up, _rows_tok)
#define ____dansi_cursor_moveDown_static(_rows_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_cursor_move_dir_dn, _rows_tok)
#define ____dansi_cursor_moveRight_static(_cols_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_cursor_move_dir_fwd, _cols_tok)
#define ____dansi_cursor_moveLeft_static(_cols_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_cursor_move_dir_bwd, _cols_tok)
#define ____dansi_cursor_moveToRow_static(_row_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_cursor_move_abs_row, _row_tok)
#define ____dansi_cursor_moveToCol_static(_col_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_cursor_move_abs_col, _col_tok)
#define ____dansi_cursor_moveNextLine_static(_rows_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_cursor_move_next_line, _rows_tok)
#define ____dansi_cursor_movePrevLine_static(_rows_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_cursor_move_prev_line, _rows_tok)
#define ____dansi_cursor_hide_static() \
    dansi_utils_formatCSI_static(dansi_utils_cursor_hide)
#define ____dansi_cursor_show_static() \
    dansi_utils_formatCSI_static(dansi_utils_cursor_show)
#define ____dansi_cursor_storePos_static() \
    dansi_utils_formatCSI_static(dansi_utils_cursor_store_pos)
#define ____dansi_cursor_restorePos_static() \
    dansi_utils_formatCSI_static(dansi_utils_cursor_restore_pos)
#define ____dansi_cursor_Style_staticParse(_style_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_style_tok))( \
        pp_Tok_then_(#_style_tok), \
        pp_Tok_else_(pp_join($, ____dansi_cursor__pp_Style_str, _style_tok)) \
    )
#define ____dansi_cursor_setStyle_static(_style_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_cursor_style, _style_tok)
#define ____dansi_cursor_setTabStop_static() \
    dansi_utils_formatCSI_static(dansi_utils_cursor_tab_set)
#define ____dansi_cursor_clearTabStop_static() \
    dansi_utils_formatCSI_static(dansi_utils_cursor_tab_clear_current)
#define ____dansi_cursor_clearAllTabStops_static() \
    dansi_utils_formatCSI_static(dansi_utils_cursor_tab_clear_all)
#define ____dansi_cursor_requestPos_static() \
    dansi_utils_formatCSI_static(dansi_utils_cursor_request_pos)

#pragma region "pp cursor style str"
#define ____dansi_cursor__pp_Style_str$dansi_cursor_Style_default "0"
#define ____dansi_cursor__pp_Style_str$dansi_cursor_Style_block_blink "1"
#define ____dansi_cursor__pp_Style_str$dansi_cursor_Style_block "2"
#define ____dansi_cursor__pp_Style_str$dansi_cursor_Style_underline_blink "3"
#define ____dansi_cursor__pp_Style_str$dansi_cursor_Style_underline "4"
#define ____dansi_cursor__pp_Style_str$dansi_cursor_Style_bar_blink "5"
#define ____dansi_cursor__pp_Style_str$dansi_cursor_Style_bar "6"
#pragma endregion "pp cursor style str"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_cursor__included */

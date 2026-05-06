/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    cursor.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-21 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_cursor
 *
 * @brief   Terminal cursor position and visibility control
 */
#ifndef daterm_cursor__included
#define daterm_cursor__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

/* --- Movement --- */

typedef A$$(4 + uint_log10Ceil_static(u16_limit_max) * 2, u8) daterm_cursor_MovePosBuf;

#define daterm_cursor_moveTo_static(_col_tok, _row_tok) \
    ____daterm_cursor_moveTo_static(_col_tok, _row_tok)
/// Get ANSI sequence to move cursor to position
/// Returns static string - valid until next call
$extern fn_((daterm_cursor_moveTo(u16 col, u16 row, daterm_cursor_MovePosBuf* buf))(S$u8));
/// Write ANSI sequence to move cursor to position
$attr($must_check)
$extern fn_((daterm_cursor_moveToWrite(u16 col, u16 row, io_Writer writer))(E$void));

typedef A$$(2 + uint_log10Ceil_static(u16_limit_max), u8) daterm_cursor_MoveDirBuf;

#define daterm_cursor_moveUp_static(_rows_tok) \
    ____daterm_cursor_moveUp_static(_rows_tok)
/// Get ANSI sequence to move cursor up
$extern fn_((daterm_cursor_moveUp(u16 rows, daterm_cursor_MoveDirBuf* buf))(S$u8));
/// Write ANSI sequence to move cursor up
$attr($must_check)
$extern fn_((daterm_cursor_moveUpWrite(u16 rows, io_Writer writer))(E$void));

#define daterm_cursor_moveDown_static(_rows_tok) \
    ____daterm_cursor_moveDown_static(_rows_tok)
/// Get ANSI sequence to move cursor down
$extern fn_((daterm_cursor_moveDown(u16 rows, daterm_cursor_MoveDirBuf* buf))(S$u8));
/// Write ANSI sequence to move cursor down
$attr($must_check)
$extern fn_((daterm_cursor_moveDownWrite(u16 rows, io_Writer writer))(E$void));

#define daterm_cursor_moveRight_static(_cols_tok) \
    ____daterm_cursor_moveRight_static(_cols_tok)
/// Get ANSI sequence to move cursor right
$extern fn_((daterm_cursor_moveRight(u16 cols, daterm_cursor_MoveDirBuf* buf))(S$u8));
/// Write ANSI sequence to move cursor right
$attr($must_check)
$extern fn_((daterm_cursor_moveRightWrite(u16 cols, io_Writer writer))(E$void));

#define daterm_cursor_moveLeft_static(_cols_tok) \
    ____daterm_cursor_moveLeft_static(_cols_tok)
/// Get ANSI sequence to move cursor left
$extern fn_((daterm_cursor_moveLeft(u16 cols, daterm_cursor_MoveDirBuf* buf))(S$u8));
/// Write ANSI sequence to move cursor left
$attr($must_check)
$extern fn_((daterm_cursor_moveLeftWrite(u16 cols, io_Writer writer))(E$void));

/* --- Visibility --- */

#define daterm_cursor_hide_static() \
    ____daterm_cursor_hide_static()
/// Get ANSI sequence to hide cursor
$extern fn_((daterm_cursor_hide(void))(S_const$u8));
/// Write ANSI sequence to hide cursor
$attr($must_check)
$extern fn_((daterm_cursor_hideWrite(io_Writer writer))(E$void));

#define daterm_cursor_show_static() \
    ____daterm_cursor_show_static()
/// Get ANSI sequence to show cursor
$extern fn_((daterm_cursor_show(void))(S_const$u8));
/// Write ANSI sequence to show cursor
$attr($must_check)
$extern fn_((daterm_cursor_showWrite(io_Writer writer))(E$void));

/* --- Store/Restore --- */

#define daterm_cursor_storePos_static() \
    ____daterm_cursor_storePos_static()
/// Get ANSI sequence to store cursor position
$extern fn_((daterm_cursor_storePos(void))(S_const$u8));
/// Write ANSI sequence to store cursor position
$attr($must_check)
$extern fn_((daterm_cursor_storePosWrite(io_Writer writer))(E$void));

#define daterm_cursor_restorePos_static() \
    ____daterm_cursor_restorePos_static()
/// Get ANSI sequence to restore cursor position
$extern fn_((daterm_cursor_restorePos(void))(S_const$u8));
/// Write ANSI sequence to restore cursor position
$attr($must_check)
$extern fn_((daterm_cursor_restorePosWrite(io_Writer writer))(E$void));

/* --- Position Query --- */

/// Cursor position (1-indexed as per ANSI spec)
typedef struct daterm_cursor_Pos {
    var_(row, u16); // 1-indexed
    var_(col, u16); // 1-indexed
} daterm_cursor_Pos;
T_use_prl$(daterm_cursor_Pos);
errset_((daterm_cursor_E)(daterm_cursor_InvalidResponse));
T_use_E$($set(daterm_cursor_E)(daterm_cursor_Pos));
/// Query cursor position from terminal
/// Requires terminal to be in raw mode or have echo disabled
/// Reads response from `in` and parses it into `out`
$attr($must_check)
$extern fn_((daterm_cursor_queryPos(io_Reader in, io_Writer out))(daterm_cursor_E$daterm_cursor_Pos));

/*========== Macros and Definitions =========================================*/

#include "daterm/utils.h"

#define ____daterm_cursor_moveTo_static(_col_tok, _row_tok) \
    daterm_utils_formatCSI_u16x2_static(daterm_utils_cursor_move_pos, _row_tok, _col_tok)
#define ____daterm_cursor_moveUp_static(_rows_tok) \
    daterm_utils_formatCSI_u16_static(daterm_utils_cursor_move_dir_up, _rows_tok)
#define ____daterm_cursor_moveDown_static(_rows_tok) \
    daterm_utils_formatCSI_u16_static(daterm_utils_cursor_move_dir_dn, _rows_tok)
#define ____daterm_cursor_moveRight_static(_cols_tok) \
    daterm_utils_formatCSI_u16_static(daterm_utils_cursor_move_dir_fwd, _cols_tok)
#define ____daterm_cursor_moveLeft_static(_cols_tok) \
    daterm_utils_formatCSI_u16_static(daterm_utils_cursor_move_dir_bwd, _cols_tok)
#define ____daterm_cursor_hide_static() \
    daterm_utils_formatCSI_static(daterm_utils_cursor_hide)
#define ____daterm_cursor_show_static() \
    daterm_utils_formatCSI_static(daterm_utils_cursor_show)
#define ____daterm_cursor_storePos_static() \
    daterm_utils_formatCSI_static(daterm_utils_cursor_store_pos)
#define ____daterm_cursor_restorePos_static() \
    daterm_utils_formatCSI_static(daterm_utils_cursor_restore_pos)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_cursor__included */

/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    cursor.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_cursor
 */
#pragma once
#ifndef dansi_cursor__included
#define dansi_cursor__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "esc.h"
#include "csi.h"

/*========== Macros and Declarations ========================================*/

typedef struct dansi_cursor_Pos {
    var_(row, u16);
    var_(col, u16);
} dansi_cursor_Pos;
T_use_prl$(dansi_cursor_Pos);

errset_((dansi_cursor_E)(dansi_cursor_InvalidResponse));
T_use_E$($set(dansi_cursor_E)(dansi_cursor_Pos));

typedef A$$(4 + uint_log10Ceil_static(u16_limit_max) * 2, u8) dansi_cursor_MovePosBuf;
typedef A$$(3 + uint_log10Ceil_static(u16_limit_max), u8) dansi_cursor_MoveDirBuf;
typedef A$$(4 + uint_log10Ceil_static(u16_limit_max) * 2, u8) dansi_cursor_PosReportBuf;

#define dansi_cursor_moveTo_static(_row_tok, _col_tok) \
    ____dansi_cursor_moveTo_static(_row_tok, _col_tok)
typedef dansi_cursor_MovePosBuf dansi_cursor_MoveToBuf;
$attr($must_check)
$extern fn_((dansi_cursor_moveTo(u16 row, u16 col, dansi_cursor_MoveToBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveToWrite(u16 row, u16 col, io_Writer out))(E$void));

#define dansi_cursor_moveUp_static(_rows_tok) \
    ____dansi_cursor_moveUp_static(_rows_tok)
typedef dansi_cursor_MoveDirBuf dansi_cursor_MoveUpBuf;
$attr($must_check)
$extern fn_((dansi_cursor_moveUp(u16 rows, dansi_cursor_MoveUpBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveUpWrite(u16 rows, io_Writer out))(E$void));

#define dansi_cursor_moveDown_static(_rows_tok) \
    ____dansi_cursor_moveDown_static(_rows_tok)
typedef dansi_cursor_MoveDirBuf dansi_cursor_MoveDownBuf;
$attr($must_check)
$extern fn_((dansi_cursor_moveDown(u16 rows, dansi_cursor_MoveDownBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveDownWrite(u16 rows, io_Writer out))(E$void));

#define dansi_cursor_moveRight_static(_cols_tok) \
    ____dansi_cursor_moveRight_static(_cols_tok)
typedef dansi_cursor_MoveDirBuf dansi_cursor_MoveRightBuf;
$attr($must_check)
$extern fn_((dansi_cursor_moveRight(u16 cols, dansi_cursor_MoveRightBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveRightWrite(u16 cols, io_Writer out))(E$void));

#define dansi_cursor_moveLeft_static(_cols_tok) \
    ____dansi_cursor_moveLeft_static(_cols_tok)
typedef dansi_cursor_MoveDirBuf dansi_cursor_MoveLeftBuf;
$attr($must_check)
$extern fn_((dansi_cursor_moveLeft(u16 cols, dansi_cursor_MoveLeftBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveLeftWrite(u16 cols, io_Writer out))(E$void));

#define dansi_cursor_moveToRow_static(_row_tok) \
    ____dansi_cursor_moveToRow_static(_row_tok)
typedef dansi_cursor_MoveDirBuf dansi_cursor_MoveToRowBuf;
$attr($must_check)
$extern fn_((dansi_cursor_moveToRow(u16 row, dansi_cursor_MoveToRowBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveToRowWrite(u16 row, io_Writer out))(E$void));

#define dansi_cursor_moveToCol_static(_col_tok) \
    ____dansi_cursor_moveToCol_static(_col_tok)
typedef dansi_cursor_MoveDirBuf dansi_cursor_MoveToColBuf;
$attr($must_check)
$extern fn_((dansi_cursor_moveToCol(u16 col, dansi_cursor_MoveToColBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveToColWrite(u16 col, io_Writer out))(E$void));

#define dansi_cursor_moveNextLine_static(_rows_tok) \
    ____dansi_cursor_moveNextLine_static(_rows_tok)
typedef dansi_cursor_MoveDirBuf dansi_cursor_MoveNextLineBuf;
$attr($must_check)
$extern fn_((dansi_cursor_moveNextLine(u16 rows, dansi_cursor_MoveNextLineBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_moveNextLineWrite(u16 rows, io_Writer out))(E$void));

#define dansi_cursor_movePrevLine_static(_rows_tok) \
    ____dansi_cursor_movePrevLine_static(_rows_tok)
typedef dansi_cursor_MoveDirBuf dansi_cursor_MovePrevLineBuf;
$attr($must_check)
$extern fn_((dansi_cursor_movePrevLine(u16 rows, dansi_cursor_MovePrevLineBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_movePrevLineWrite(u16 rows, io_Writer out))(E$void));

#define dansi_cursor_setTabStop_static() \
    ____dansi_cursor_setTabStop_static()
$extern fn_((dansi_cursor_setTabStop(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_cursor_setTabStopWrite(io_Writer out))(E$void));

#define dansi_cursor_clearTabStop_static() \
    ____dansi_cursor_clearTabStop_static()
$extern fn_((dansi_cursor_clearTabStop(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_cursor_clearTabStopWrite(io_Writer out))(E$void));

#define dansi_cursor_clearAllTabStops_static() \
    ____dansi_cursor_clearAllTabStops_static()
$extern fn_((dansi_cursor_clearAllTabStops(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_cursor_clearAllTabStopsWrite(io_Writer out))(E$void));

#define dansi_cursor_requestPos_static() \
    ____dansi_cursor_requestPos_static()
$extern fn_((dansi_cursor_requestPos(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_cursor_requestPosWrite(io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_cursor_receivePosReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_cursor_parsePosReport(S_const$u8 report))(dansi_cursor_E$dansi_cursor_Pos));
$attr($must_check)
$extern fn_((dansi_cursor_fetchPos(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_cursor_E$dansi_cursor_Pos));

/*========== Macros and Definitions =========================================*/

#define ____dansi_cursor_moveTo_static(_row_tok, _col_tok) \
    dansi_csi_make2_static(_row_tok, _col_tok, "H")
#define ____dansi_cursor_moveUp_static(_rows_tok) \
    dansi_csi_make1_static(_rows_tok, "A")
#define ____dansi_cursor_moveDown_static(_rows_tok) \
    dansi_csi_make1_static(_rows_tok, "B")
#define ____dansi_cursor_moveRight_static(_cols_tok) \
    dansi_csi_make1_static(_cols_tok, "C")
#define ____dansi_cursor_moveLeft_static(_cols_tok) \
    dansi_csi_make1_static(_cols_tok, "D")
#define ____dansi_cursor_moveToRow_static(_row_tok) \
    dansi_csi_make1_static(_row_tok, "d")
#define ____dansi_cursor_moveToCol_static(_col_tok) \
    dansi_csi_make1_static(_col_tok, "G")
#define ____dansi_cursor_moveNextLine_static(_rows_tok) \
    dansi_csi_make1_static(_rows_tok, "E")
#define ____dansi_cursor_movePrevLine_static(_rows_tok) \
    dansi_csi_make1_static(_rows_tok, "F")
#define ____dansi_cursor_setTabStop_static() \
    dansi_esc_make_static("", "H")
#define ____dansi_cursor_clearTabStop_static() \
    dansi_csi_make1_static("0", "g")
#define ____dansi_cursor_clearAllTabStops_static() \
    dansi_csi_make1_static("3", "g")
#define ____dansi_cursor_requestPos_static() \
    dansi_csi_make1_static("6", "n")

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_cursor__included */

/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    line.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-21 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_line
 *
 * @brief   Terminal line scrolling and clearing operations
 */
#pragma once
#ifndef dansi_line__included
#define dansi_line__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/Writer.h>

/*========== Macros and Declarations ========================================*/

/* --- Scrolling --- */

typedef A$$(3 + uint_log10Ceil_static(u16_limit_max), u8) dansi_line_ScrollDirBuf;

#define dansi_line_scrollUp_static(_lines_tok) \
    ____dansi_line_scrollUp_static(_lines_tok)
/// Get ANSI sequence to scroll terminal up by n lines
/// Lines that scroll off the top are discarded, new lines appear at bottom
$extern fn_((dansi_line_scrollUp(u16 lines, dansi_line_ScrollDirBuf* buf))(S$u8));
/// Write ANSI sequence to scroll terminal up
$attr($must_check)
$extern fn_((dansi_line_scrollUpWrite(u16 lines, io_Writer writer))(E$void));

#define dansi_line_scrollDown_static(_lines_tok) \
    ____dansi_line_scrollDown_static(_lines_tok)
/// Get ANSI sequence to scroll terminal down by n lines
/// Lines that scroll off the bottom are discarded, new lines appear at top
$extern fn_((dansi_line_scrollDown(u16 lines, dansi_line_ScrollDirBuf* buf))(S$u8));
/// Write ANSI sequence to scroll terminal down
$attr($must_check)
$extern fn_((dansi_line_scrollDownWrite(u16 lines, io_Writer writer))(E$void));

/* --- Clearing --- */

#define dansi_line_clearFromCursor_static() \
    ____dansi_line_clearFromCursor_static()
/// Get ANSI sequence to clear from cursor to end of line
$extern fn_((dansi_line_clearFromCursor(void))(S_const$u8));
/// Write ANSI sequence to clear from cursor to end of line
$attr($must_check)
$extern fn_((dansi_line_clearFromCursorWrite(io_Writer writer))(E$void));

#define dansi_line_clearToCursor_static() \
    ____dansi_line_clearToCursor_static()
/// Get ANSI sequence to clear from start of line to cursor
$extern fn_((dansi_line_clearToCursor(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_line_clearToCursorWrite(io_Writer writer))(E$void));

#define dansi_line_clear_static() \
    ____dansi_line_clear_static()
/// Get ANSI sequence to clear entire line
$extern fn_((dansi_line_clear(void))(S_const$u8));
/// Write ANSI sequence to clear entire line
$attr($must_check)
$extern fn_((dansi_line_clearWrite(io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_line_scrollUp_static(_lines_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_line_scroll_dir_up, _lines_tok)
#define ____dansi_line_scrollDown_static(_lines_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_line_scroll_dir_down, _lines_tok)
#define ____dansi_line_clearFromCursor_static() \
    dansi_utils_formatCSI_static(dansi_utils_line_clear_from_cursor)
#define ____dansi_line_clearToCursor_static() \
    dansi_utils_formatCSI_static(dansi_utils_line_clear_to_cursor)
#define ____dansi_line_clear_static() \
    dansi_utils_formatCSI_static(dansi_utils_line_clear)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_line__included */

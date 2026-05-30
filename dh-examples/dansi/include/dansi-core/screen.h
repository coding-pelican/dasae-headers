/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    screen.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-21 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_screen
 *
 * @brief   Terminal screen buffer and clearing operations
 */
#ifndef dansi_screen__included
#define dansi_screen__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

/* --- Alternate --- */

#define dansi_screen_enterAlternate_static() \
    ____dansi_screen_enterAlternate_static()
/// Get ANSI sequence to enter alternate screen buffer
/// The alternate screen is a separate buffer that doesn't affect main screen
$extern fn_((dansi_screen_enterAlternate(void))(S_const$u8));
/// Write ANSI sequence to enter alternate screen
$attr($must_check)
$extern fn_((dansi_screen_enterAlternateWrite(io_Writer writer))(E$void));

#define dansi_screen_exitAlternate_static() \
    ____dansi_screen_exitAlternate_static()
/// Get ANSI sequence to exit alternate screen buffer
$extern fn_((dansi_screen_exitAlternate(void))(S_const$u8));
/// Write ANSI sequence to exit alternate screen
$attr($must_check)
$extern fn_((dansi_screen_exitAlternateWrite(io_Writer writer))(E$void));

/* --- Clearing --- */

#define dansi_screen_clearFromCursor_static() \
    ____dansi_screen_clearFromCursor_static()
/// Get ANSI sequence to clear from cursor to end of screen
$extern fn_((dansi_screen_clearFromCursor(void))(S_const$u8));
/// Write ANSI sequence to clear from cursor to end of screen
$attr($must_check)
$extern fn_((dansi_screen_clearFromCursorWrite(io_Writer writer))(E$void));

#define dansi_screen_clearToCursor_static() \
    ____dansi_screen_clearToCursor_static()
/// Get ANSI sequence to clear from start of screen to cursor
$extern fn_((dansi_screen_clearToCursor(void))(S_const$u8));
/// Write ANSI sequence to clear from start of screen to cursor
$attr($must_check)
$extern fn_((dansi_screen_clearToCursorWrite(io_Writer writer))(E$void));

#define dansi_screen_clear_static() \
    ____dansi_screen_clear_static()
/// Get ANSI sequence to clear entire screen
$extern fn_((dansi_screen_clear(void))(S_const$u8));
/// Write ANSI sequence to clear entire screen
$attr($must_check)
$extern fn_((dansi_screen_clearWrite(io_Writer writer))(E$void));

/* --- Size Reports --- */

typedef A$$(6 + uint_log10Ceil_static(u16_limit_max) * 2, u8) dansi_screen_SizeReportBuf;

/// Screen size in columns and rows.
typedef struct dansi_screen_Size {
    var_(cols, u16);
    var_(rows, u16);
} dansi_screen_Size;
T_use_prl$(dansi_screen_Size);

typedef struct dansi_screen_PixelSize {
    var_(width, u16);
    var_(height, u16);
} dansi_screen_PixelSize;
T_use_prl$(dansi_screen_PixelSize);

errset_((dansi_screen_E)(dansi_screen_InvalidResponse));
T_use_E$($set(dansi_screen_E)(dansi_screen_Size));
T_use_E$($set(dansi_screen_E)(dansi_screen_PixelSize));

#define dansi_screen_requestCellSizePixels_static() \
    ____dansi_screen_requestCellSizePixels_static()
$extern fn_((dansi_screen_requestCellSizePixels(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_screen_requestCellSizePixelsWrite(io_Writer writer))(E$void));
$attr($must_check)
$extern fn_((dansi_screen_receiveCellSizePixelsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_screen_parseCellSizePixelsReport(S_const$u8 report))(dansi_screen_E$dansi_screen_PixelSize));
$attr($must_check)
$extern fn_((dansi_screen_fetchCellSizePixels(io_Writer out, io_Reader in, S$u8 buf))(dansi_screen_E$dansi_screen_PixelSize));

#define dansi_screen_requestTextAreaSizeChars_static() \
    ____dansi_screen_requestTextAreaSizeChars_static()
$extern fn_((dansi_screen_requestTextAreaSizeChars(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_screen_requestTextAreaSizeCharsWrite(io_Writer writer))(E$void));
$attr($must_check)
$extern fn_((dansi_screen_receiveTextAreaSizeCharsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_screen_parseTextAreaSizeCharsReport(S_const$u8 report))(dansi_screen_E$dansi_screen_Size));
$attr($must_check)
$extern fn_((dansi_screen_fetchTextAreaSizeChars(io_Writer out, io_Reader in, S$u8 buf))(dansi_screen_E$dansi_screen_Size));

#define dansi_screen_requestTextAreaSizePixels_static() \
    ____dansi_screen_requestTextAreaSizePixels_static()
$extern fn_((dansi_screen_requestTextAreaSizePixels(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_screen_requestTextAreaSizePixelsWrite(io_Writer writer))(E$void));
$attr($must_check)
$extern fn_((dansi_screen_receiveTextAreaSizePixelsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_screen_parseTextAreaSizePixelsReport(S_const$u8 report))(dansi_screen_E$dansi_screen_PixelSize));
$attr($must_check)
$extern fn_((dansi_screen_fetchTextAreaSizePixels(io_Writer out, io_Reader in, S$u8 buf))(dansi_screen_E$dansi_screen_PixelSize));

/// Request, receive, and parse screen size by cursor-position probing.
/// Requires terminal to be in raw mode or have echo disabled.
$attr($must_check)
$extern fn_((dansi_screen_fetchSizeByCursorPos(io_Writer out, io_Reader in, S$u8 buf))(dansi_screen_E$dansi_screen_Size));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_screen_enterAlternate_static() \
    dansi_utils_formatCSI_static(dansi_utils_screen_enter_alternate)
#define ____dansi_screen_exitAlternate_static() \
    dansi_utils_formatCSI_static(dansi_utils_screen_exit_alternate)
#define ____dansi_screen_clearFromCursor_static() \
    dansi_utils_formatCSI_static(dansi_utils_screen_clear_from_cursor)
#define ____dansi_screen_clearToCursor_static() \
    dansi_utils_formatCSI_static(dansi_utils_screen_clear_to_cursor)
#define ____dansi_screen_clear_static() \
    dansi_utils_formatCSI_static(dansi_utils_screen_clear)
#define ____dansi_screen_requestCellSizePixels_static() \
    dansi_utils_formatCSI_static(dansi_utils_screen_request_cell_size_pixels)
#define ____dansi_screen_requestTextAreaSizeChars_static() \
    dansi_utils_formatCSI_static(dansi_utils_screen_request_text_area_size_chars)
#define ____dansi_screen_requestTextAreaSizePixels_static() \
    dansi_utils_formatCSI_static(dansi_utils_screen_request_text_area_size_pixels)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_screen__included */

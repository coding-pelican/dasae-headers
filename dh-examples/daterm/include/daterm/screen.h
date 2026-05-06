/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    screen.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-21 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_screen
 *
 * @brief   Terminal screen buffer and clearing operations
 */
#ifndef daterm_screen__included
#define daterm_screen__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

/* --- Alternate --- */

#define daterm_screen_enterAlternate_static() \
    ____daterm_screen_enterAlternate_static()
/// Get ANSI sequence to enter alternate screen buffer
/// The alternate screen is a separate buffer that doesn't affect main screen
$extern fn_((daterm_screen_enterAlternate(void))(S_const$u8));
/// Write ANSI sequence to enter alternate screen
$attr($must_check)
$extern fn_((daterm_screen_enterAlternateWrite(io_Writer writer))(E$void));

#define daterm_screen_exitAlternate_static() \
    ____daterm_screen_exitAlternate_static()
/// Get ANSI sequence to exit alternate screen buffer
$extern fn_((daterm_screen_exitAlternate(void))(S_const$u8));
/// Write ANSI sequence to exit alternate screen
$attr($must_check)
$extern fn_((daterm_screen_exitAlternateWrite(io_Writer writer))(E$void));

/* --- Clearing --- */

#define daterm_screen_clearFromCursor_static() \
    ____daterm_screen_clearFromCursor_static()
/// Get ANSI sequence to clear from cursor to end of screen
$extern fn_((daterm_screen_clearFromCursor(void))(S_const$u8));
/// Write ANSI sequence to clear from cursor to end of screen
$attr($must_check)
$extern fn_((daterm_screen_clearFromCursorWrite(io_Writer writer))(E$void));

#define daterm_screen_clearToCursor_static() \
    ____daterm_screen_clearToCursor_static()
/// Get ANSI sequence to clear from start of screen to cursor
$extern fn_((daterm_screen_clearToCursor(void))(S_const$u8));
/// Write ANSI sequence to clear from start of screen to cursor
$attr($must_check)
$extern fn_((daterm_screen_clearToCursorWrite(io_Writer writer))(E$void));

#define daterm_screen_clear_static() \
    ____daterm_screen_clear_static()
/// Get ANSI sequence to clear entire screen
$extern fn_((daterm_screen_clear(void))(S_const$u8));
/// Write ANSI sequence to clear entire screen
$attr($must_check)
$extern fn_((daterm_screen_clearWrite(io_Writer writer))(E$void));

/* --- Size Query --- */

/// Screen size in columns and rows
typedef struct daterm_screen_Size {
    var_(cols, u16); // Width in columns
    var_(rows, u16); // Height in rows
} daterm_screen_Size;
T_use_prl$(daterm_screen_Size);
errset_((daterm_screen_E)(daterm_screen_InvalidResponse));
T_use_E$($set(daterm_screen_E)(daterm_screen_Size));
/// Query screen size from terminal
/// Requires terminal to be in raw mode or have echo disabled
/// Reads response from `in` and parses it into `out`
$attr($must_check)
$extern fn_((daterm_screen_querySize(io_Reader in, io_Writer out))(daterm_screen_E$daterm_screen_Size));

/*========== Macros and Definitions =========================================*/

#include "daterm/utils.h"

#define ____daterm_screen_enterAlternate_static() \
    daterm_utils_formatCSI_static(daterm_utils_screen_enter_alternate)
#define ____daterm_screen_exitAlternate_static() \
    daterm_utils_formatCSI_static(daterm_utils_screen_exit_alternate)
#define ____daterm_screen_clearFromCursor_static() \
    daterm_utils_formatCSI_static(daterm_utils_screen_clear_from_cursor)
#define ____daterm_screen_clearToCursor_static() \
    daterm_utils_formatCSI_static(daterm_utils_screen_clear_to_cursor)
#define ____daterm_screen_clear_static() \
    daterm_utils_formatCSI_static(daterm_utils_screen_clear)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_screen__included */

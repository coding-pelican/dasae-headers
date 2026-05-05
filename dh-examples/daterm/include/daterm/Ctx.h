/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Ctx.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-21 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_Ctx
 *
 * @brief   Terminal context (raw mode, terminal size, mouse tracking)
 */
#ifndef daterm_Ctx__included
#define daterm_Ctx__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Event.h"
#include "cursor.h"
#include "screen.h"
#include <dh/time/Duration.h>
#if plat_is_windows
#include <dh/os/windows/handle.h>
#endif /* plat_is_windows */
#if plat_is_posix
#include <termios.h>
#endif /* plat_is_posix */

/*========== Macros and Declarations ========================================*/

/* --- Raw Mode --- */

typedef struct daterm_Ctx__Impl {
#if plat_is_windows
    var_(old_in_mode, DWORD);
    var_(old_out_mode, DWORD);
#endif /* plat_is_windows */
#if plat_is_posix
    var_(old_in_termios, struct termios);
    var_(old_out_termios, struct termios);
#endif /* plat_is_posix */
} daterm_Ctx__Impl;

/// Terminal state for raw mode management
typedef struct daterm_Ctx {
    var_(input, fs_File_Handle);
    var_(output, fs_File_Handle);
    var_(impl, daterm_Ctx__Impl);
    var_(is_raw, bool);
    var_(mouse_enabled, bool);
} daterm_Ctx;
T_use_prl$(daterm_Ctx);
/// Initialize terminal and enter raw mode
/// Raw mode disables line buffering and echo for direct input handling
$attr($must_check)
$extern fn_((daterm_Ctx_init(fs_File_Handle input, fs_File_Handle output))(E$daterm_Ctx));
/// Restore terminal to previous state
$attr($must_check)
$extern fn_((daterm_Ctx_fini(daterm_Ctx* self))(E$void));

/* --- Mouse Tracking --- */

/// Enable mouse tracking
/// Terminal will send mouse events as escape sequences
/// Recommends terminal to be in raw mode
$attr($must_check)
$extern fn_((daterm_Ctx_enableMouse(daterm_Ctx* self, io_Writer writer))(E$void));
/// Disable mouse tracking
/// Recommends terminal to be in raw mode
$attr($must_check)
$extern fn_((daterm_Ctx_disableMouse(daterm_Ctx* self, io_Writer writer))(E$void));

/* --- Event Polling --- */

errset_((daterm_Ctx_E)(
    daterm_Ctx_Unsupported,
    daterm_Ctx_Timeout
));
T_use_E$($set(daterm_Ctx_E)(daterm_Event));

/// Parse next event from terminal input (non-blocking)
/// Returns `none` if no event available
/// Requires terminal to be in raw mode
$extern fn_((daterm_Ctx_poll(const daterm_Ctx* self))(O$daterm_Event));
/// Wait for next event (blocking)
/// Requires terminal to be in raw mode
$extern fn_((daterm_Ctx_wait(const daterm_Ctx* self))(daterm_Event));
/// Parse next event with timeout (blocking up to timeout)
/// Returns `Timeout` if timeout expires
/// Requires terminal to be in raw mode
$attr($must_check)
$extern fn_((daterm_Ctx_timedWait(const daterm_Ctx* self, time_Duration timeout))(daterm_Ctx_E$daterm_Event));

/* --- Queries --- */

/// Get current cursor position
/// Requires terminal to be in raw mode
$attr($must_check)
$extern fn_((daterm_Ctx_queryCursorPos(const daterm_Ctx* self))(E$daterm_cursor_Pos));
/// Get current screen size
/// Requires terminal to be in raw mode
$attr($must_check)
$extern fn_((daterm_Ctx_queryScreenSize(const daterm_Ctx* self))(E$daterm_screen_Size));


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_Ctx__included */

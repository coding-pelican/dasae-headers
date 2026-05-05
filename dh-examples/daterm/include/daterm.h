/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    daterm.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-28 (date of last update)
 * @version v0.1.0
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm
 *
 * @brief   Low-level terminal control library
 * @details Provides ANSI terminal control primitives for TUI applications.
 *          This is the foundation layer for datui and other TUI libraries.
 *
 *          ## Architecture
 *          - **utils**: ANSI escape sequence constants and helpers
 *          - **Ctx**: control raw mode, query terminal size, enable mouse tracking
 *          - **Event**: keyboard and mouse event handling
 *          - **cursor**: position and visibility control
 *          - **screen**: alternate screen buffer and clearing operations
 *          - **line**: line scrolling and clearing operations
 *          - **Palette4bit**: 4-bit (16 color) palette
 *          - **Palette8bit**: 8-bit (256 color) palette
 *          - **color**: 4-bit (16 color), 8-bit (256 color), and 24-bit (RGB color) support
 *          - **style**: text styling (bold, dim, italic, underline, blinking, reverse, invisible, strikethrough)
 *          - **attr**: reset all colors and styles
 *
 *          ## Usage
 *          ```c
 *          // Enable raw mode
 *          var term = try_(daterm_Ctx_init(stdin_handle));
 *          defer_(daterm_Ctx_fini(&term));
 *
 *          // Set colors and print
 *          try_(daterm_color_fg24BitWrite(255, 100, 50, stdout_writer));
 *          try_(io_Writer_write(stdout_writer, u8_l("Hello, Terminal!")));
 *
 *          // Move cursor
 *          try_(daterm_cursor_moveToWrite(10, 5, stdout_writer));
 *          ```
 */
#ifndef daterm__included
#define daterm__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "daterm/utils.h"
#include "daterm/Ctx.h"
#include "daterm/Event.h"
#include "daterm/cursor.h"
#include "daterm/screen.h"
#include "daterm/line.h"
#include "daterm/Palette4bit.h"
#include "daterm/Palette8bit.h"
#include "daterm/color.h"
#include "daterm/style.h"
#include "daterm/attr.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm__included */

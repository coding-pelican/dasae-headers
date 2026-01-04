/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file      wnd.h
 * @author    Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date      2025-12-31 (date of creation)
 * @updated   2026-01-04 (date of last update)
 * @ingroup   dasae-headers-extras(dh-extras)/os/windows
 * @prefix    (none)
 *
 * @brief     Windows GDI for Software Rendering
 * @details   Provides functions to draw pixel buffers directly to the window.
 *
 * @note Links with `-luser32 -lgdi32 -lshcore`
 */
#ifndef dh_extras_os_windows_wnd__included
#define dh_extras_os_windows_wnd__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include <dh/os/windows/base.h>
#include <dh/os/windows/pp-def/guard.h>
#if plat_is_windows

#include <winuser.h>
#include <wingdi.h>
#include <shellscalingapi.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shcore.lib")

#endif /* plat_is_windows */
#include <dh/os/windows/pp-def/unguarded.h>
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dh_extras_os_windows_wnd__included */

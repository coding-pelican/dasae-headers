/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file      x11.h
 * @author    Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date      2025-12-31 (date of creation)
 * @updated   2025-12-31 (date of last update)
 * @ingroup   dasae-headers/extensions(dh-ext)/os/linux/x11
 * @prefix    (none)
 *
 * @brief     X11 Library (Xlib, Xutil, KeySym)
 * @details   Provides functions to create and manage X11 windows.
 *
 * API Overview:
 * - Xlib: XOpenDisplay, XCloseDisplay, XCreateWindow, XDestroyWindow, XMapWindow, XUnmapWindow
 * - Xutil: XSetWindowAttributes, XSetWMProperties
 * - KeySym: XLookupKeysym
 *
 * @note Links with `-lX11`
 */
#ifndef dh_ext_os_linux_x11__included
#define dh_ext_os_linux_x11__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "dh/builtin/plat_cfg.h"
#if plat_is_linux

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#endif /* plat_is_linux */
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dh_ext_os_linux_x11__included */

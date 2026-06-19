/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    dansi-xterm.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm
 */
#pragma once
#ifndef dansi_xterm__included
#define dansi_xterm__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-xterm/mode.h"
#include "dansi-xterm/mouse.h"
#include "dansi-xterm/focus.h"
#include "dansi-xterm/paste.h"
#include "dansi-xterm/key.h"

#include "dansi-xterm/screen.h"
#include "dansi-xterm/window.h"
#include "dansi-xterm/title.h"
#include "dansi-xterm/selection.h"
#include "dansi-xterm/resrc.h"

#include "dansi-xterm/sgr.h"
#include "dansi-xterm/color.h"
#include "dansi-xterm/Palette4bit.h"
#include "dansi-xterm/Palette8bit.h"
#include "dansi-xterm/palette.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm__included */

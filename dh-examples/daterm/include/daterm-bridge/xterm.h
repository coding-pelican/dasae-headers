/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    xterm.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-20 (date of creation)
 * @updated 2026-06-21 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_xterm
 */
#pragma once
#ifndef daterm_xterm__included
#define daterm_xterm__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dansi-xterm.h>
#include "daterm-context/ANSI.h"

/*========== Macros and Declarations ========================================*/

typedef struct daterm_xterm_MouseCfg {
    var_(report_mode, dansi_xterm_mouse_ReportMode);
    var_(encoding, dansi_xterm_mouse_Encoding);
} daterm_xterm_MouseCfg;
T_use_prl$(daterm_xterm_MouseCfg);

$attr($must_check)
$extern fn_((daterm_xterm_setMouse(daterm_ANSI* ansi, O$daterm_xterm_MouseCfg cfg_enabled))(E$void));
$attr($must_check)
$extern fn_((daterm_xterm_enableMouse(daterm_ANSI* ansi, daterm_xterm_MouseCfg cfg))(E$void));
$attr($must_check)
$extern fn_((daterm_xterm_disableMouse(daterm_ANSI* ansi))(E$void));

$attr($must_check)
$extern fn_((daterm_xterm_setFocusTracking(daterm_ANSI* ansi, bool enabled))(E$void));
$attr($must_check)
$extern fn_((daterm_xterm_enableFocusTracking(daterm_ANSI* ansi))(E$void));
$attr($must_check)
$extern fn_((daterm_xterm_disableFocusTracking(daterm_ANSI* ansi))(E$void));

$attr($must_check)
$extern fn_((daterm_xterm_setEnhancedKeyboard(daterm_ANSI* ansi, bool enabled))(E$void));
$attr($must_check)
$extern fn_((daterm_xterm_enableEnhancedKeyboard(daterm_ANSI* ansi))(E$void));
$attr($must_check)
$extern fn_((daterm_xterm_disableEnhancedKeyboard(daterm_ANSI* ansi))(E$void));

$attr($must_check)
$extern fn_((daterm_xterm_fetchCellPixels(
    daterm_Term term,
    time_Dur timeout,
    daterm_PixelSize* out
))(daterm_Txn_E$Void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_xterm__included */

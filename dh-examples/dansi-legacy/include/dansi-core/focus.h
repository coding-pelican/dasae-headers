/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    focus.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-17 (date of creation)
 * @updated 2026-06-17 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_focus
 *
 * @brief   xterm focus tracking mode controls (mode 1004).
 */
#pragma once
#ifndef dansi_focus__included
#define dansi_focus__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_focus_TrackingBuf;

#define dansi_focus_enableTracking_static() \
    ____dansi_focus_enableTracking_static()
$extern fn_((dansi_focus_enableTracking(dansi_focus_TrackingBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_focus_enableTrackingWrite(io_Writer writer))(E$void));

#define dansi_focus_disableTracking_static() \
    ____dansi_focus_disableTracking_static()
$extern fn_((dansi_focus_disableTracking(dansi_focus_TrackingBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_focus_disableTrackingWrite(io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "mode.h"

#define ____dansi_focus_enableTracking_static() \
    dansi_mode_enablePrivate_static(dansi_mode_Private_staticParse(dansi_mode_Private_focus_events))
#define ____dansi_focus_disableTracking_static() \
    dansi_mode_disablePrivate_static(dansi_mode_Private_staticParse(dansi_mode_Private_focus_events))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_focus__included */

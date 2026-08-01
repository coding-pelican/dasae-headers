/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    focus.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_focus
 */
#pragma once
#ifndef dansi_xterm_focus__included
#define dansi_xterm_focus__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "mode.h"

/*========== Macros and Declarations ========================================*/

#define dansi_xterm_focus_in_final __str__dansi_xterm_focus_in_final
#define dansi_xterm_focus_in_final_byte __uint__dansi_xterm_focus_in_final_byte
#define dansi_xterm_focus_out_final __str__dansi_xterm_focus_out_final
#define dansi_xterm_focus_out_final_byte __uint__dansi_xterm_focus_out_final_byte

typedef enum_((dansi_xterm_focus_Event $fits($packed))(
    dansi_xterm_focus_Event_in,
    dansi_xterm_focus_Event_out
)) dansi_xterm_focus_Event;
claim_assert_static(eqlType$(dansi_xterm_focus_Event, u8));
T_use_prl$(dansi_xterm_focus_Event);

#define dansi_xterm_focus_setTracking_static(_pp_enabled) \
    ____dansi_xterm_focus_setTracking_static(_pp_enabled)
typedef dansi_xterm_mode_SetBuf dansi_xterm_focus_SetTrackingBuf;
$attr($must_check)
$extern fn_((dansi_xterm_focus_setTracking(bool enabled, dansi_xterm_focus_SetTrackingBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_focus_setTrackingWrite(bool enabled, io_Writer out))(io_PrintE$void));

#define dansi_xterm_focus_enableTracking_static() \
    ____dansi_xterm_focus_enableTracking_static()
typedef dansi_xterm_mode_EnableBuf dansi_xterm_focus_EnableTrackingBuf;
$attr($must_check)
$extern fn_((dansi_xterm_focus_enableTracking(dansi_xterm_focus_EnableTrackingBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_focus_enableTrackingWrite(io_Writer out))(io_PrintE$void));

#define dansi_xterm_focus_disableTracking_static() \
    ____dansi_xterm_focus_disableTracking_static()
typedef dansi_xterm_mode_DisableBuf dansi_xterm_focus_DisableTrackingBuf;
$attr($must_check)
$extern fn_((dansi_xterm_focus_disableTracking(dansi_xterm_focus_DisableTrackingBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_focus_disableTrackingWrite(io_Writer out))(io_PrintE$void));
$extern fn_((dansi_xterm_focus_parseReport(S_const$u8 report))(O$dansi_xterm_focus_Event));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_xterm_focus_in_final "I"
#define __uint__dansi_xterm_focus_in_final_byte u8_c('I')
#define __str__dansi_xterm_focus_out_final "O"
#define __uint__dansi_xterm_focus_out_final_byte u8_c('O')
#define ____dansi_xterm_focus_enableTracking_static() \
    dansi_xterm_mode_enable_static(dansi_xterm_mode_Code_focus_event)
#define ____dansi_xterm_focus_disableTracking_static() \
    dansi_xterm_mode_disable_static(dansi_xterm_mode_Code_focus_event)
#define ____dansi_xterm_focus_setTracking_static(_pp_enabled) \
    dansi_xterm_mode_set_static(dansi_xterm_mode_Code_focus_event, _pp_enabled)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_focus__included */

/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Event.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-06-17 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_Event
 *
 * @brief   ANSI and xterm input report parsing.
 */
#pragma once
#ifndef dansi_Event__included
#define dansi_Event__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Seq.h"
#include "key.h"
#include "mouse.h"

/*========== Standard Events ===============================================*/

typedef struct dansi_Event_Special {
    var_(code, dansi_key_Code);
    var_(mods, dansi_key_Mods);
} dansi_Event_Special;
T_use_prl$(dansi_Event_Special);

typedef struct dansi_Event_Text {
    var_(codepoint, u32);
    var_(mods, dansi_key_Mods);
} dansi_Event_Text;
T_use_prl$(dansi_Event_Text);

typedef enum_((dansi_Event_Focus $fits($packed))(
    dansi_Event_Focus_in,
    dansi_Event_Focus_out
)) dansi_Event_Focus;
T_use_prl$(dansi_Event_Focus);

typedef struct dansi_Event_MouseBtnReport {
    var_(x, u16);
    var_(y, u16);
    var_(btn, dansi_mouse_Btn);
    var_(mods, dansi_key_Mods);
} dansi_Event_MouseBtnReport;
T_use_prl$(dansi_Event_MouseBtnReport);

typedef struct dansi_Event_MouseMotionReport {
    var_(x, u16);
    var_(y, u16);
    var_(mods, dansi_key_Mods);
} dansi_Event_MouseMotionReport;
T_use_prl$(dansi_Event_MouseMotionReport);

typedef struct dansi_Event_MouseWheelReport {
    var_(x, u16);
    var_(y, u16);
    var_(wheel, dansi_mouse_Wheel);
    var_(mods, dansi_key_Mods);
} dansi_Event_MouseWheelReport;
T_use_prl$(dansi_Event_MouseWheelReport);

typedef variant_((dansi_Event_Mouse $fits($packed))(
    (dansi_Event_Mouse_press, dansi_Event_MouseBtnReport),
    (dansi_Event_Mouse_release, dansi_Event_MouseBtnReport),
    (dansi_Event_Mouse_drag, dansi_Event_MouseBtnReport),
    (dansi_Event_Mouse_motion, dansi_Event_MouseMotionReport),
    (dansi_Event_Mouse_wheel, dansi_Event_MouseWheelReport)
)) dansi_Event_Mouse;
T_use_prl$(dansi_Event_Mouse);

typedef variant_((dansi_Event $fits($packed))(
    (dansi_Event_special, dansi_Event_Special),
    (dansi_Event_text, dansi_Event_Text),
    (dansi_Event_mouse, dansi_Event_Mouse),
    (dansi_Event_focus, dansi_Event_Focus)
)) dansi_Event;
T_use_prl$(dansi_Event);

errset_((dansi_Event_E)(
    dansi_Event_UnknownSeq,
    dansi_Event_InvalidSeq
));
T_use_E$($set(dansi_Event_E)(dansi_Event));

$attr($must_check)
$extern fn_((dansi_Event_parse(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$extern fn_((dansi_Event_tryParse(dansi_Seq seq))(O$dansi_Event));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_Event__included */

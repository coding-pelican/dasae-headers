/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Event.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-06-17 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm_Event
 */
#pragma once
#ifndef daterm_Event__included
#define daterm_Event__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"
#include <dansi-core/key.h>
#include <dansi-core/mouse.h>
#include <dansi-core/Event.h>

typedef enum_((daterm_Event_MouseAction $fits($packed))(
    daterm_Event_MouseAction_press,
    daterm_Event_MouseAction_release,
    daterm_Event_MouseAction_drag,
    daterm_Event_MouseAction_motion,
    daterm_Event_MouseAction_wheel
)) daterm_Event_MouseAction;
T_use_prl$(daterm_Event_MouseAction);

typedef struct daterm_Event_Mouse {
    var_(col, u16);
    var_(row, u16);
    var_(btn, O$dansi_mouse_Btn);
    var_(action, daterm_Event_MouseAction);
    var_(wheel, O$dansi_mouse_Wheel);
    var_(mods, dansi_key_Mods);
} daterm_Event_Mouse;
T_use_prl$(daterm_Event_Mouse);

typedef variant_((daterm_Event $fits($packed))(
    (daterm_Event_special, dansi_Event_Special),
    (daterm_Event_text, dansi_Event_Text),
    (daterm_Event_mouse, daterm_Event_Mouse),
    (daterm_Event_focus, dansi_Event_Focus),
    (daterm_Event_resize, daterm_Size)
)) daterm_Event;
T_use_prl$(daterm_Event);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_Event__included */

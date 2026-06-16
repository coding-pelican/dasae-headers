/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Event.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-05-23 (date of last update)
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
#include <dansi-core/Event.h>

typedef enum_((daterm_Event_MouseButton $fits($packed))(
    daterm_Event_MouseButton_none,
    daterm_Event_MouseButton_left,
    daterm_Event_MouseButton_middle,
    daterm_Event_MouseButton_right,
)) daterm_Event_MouseButton;

typedef enum_((daterm_Event_MouseAction $fits($packed))(
    daterm_Event_MouseAction_press,
    daterm_Event_MouseAction_release,
    daterm_Event_MouseAction_drag,
    daterm_Event_MouseAction_motion,
)) daterm_Event_MouseAction;

typedef enum_((daterm_Event_MouseWheel $fits($packed))(
    daterm_Event_MouseWheel_none,
    daterm_Event_MouseWheel_up,
    daterm_Event_MouseWheel_down,
)) daterm_Event_MouseWheel;

typedef enum_((daterm_Event_Focus $fits($packed))(
    daterm_Event_Focus_in,
    daterm_Event_Focus_out,
)) daterm_Event_Focus;
T_use_O$(daterm_Event_Focus);

typedef struct daterm_Event_Mouse {
    var_(col, u16);
    var_(row, u16);
    var_(button, daterm_Event_MouseButton);
    var_(action, daterm_Event_MouseAction);
    var_(wheel, daterm_Event_MouseWheel);
    var_(mods, dansi_Event_KeyMods);
} daterm_Event_Mouse;
T_use_prl$(daterm_Event_Mouse);

typedef variant_((daterm_Event $fits($packed))(
    (daterm_Event_key, dansi_Event_Key),
    (daterm_Event_mouse, daterm_Event_Mouse),
    (daterm_Event_focus, daterm_Event_Focus),
    (daterm_Event_resize, daterm_Size)
)) daterm_Event;
T_use_prl$(daterm_Event);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_Event__included */

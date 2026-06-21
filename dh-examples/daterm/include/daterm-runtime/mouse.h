/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    mouse.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-20 (date of creation)
 * @updated 2026-06-20 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm_mouse
 */
#pragma once
#ifndef daterm_mouse__included
#define daterm_mouse__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "input.h"

typedef enum_((daterm_mouse_Btn $fits($packed))(
    daterm_mouse_Btn_left = 0,
    daterm_mouse_Btn_middle,
    daterm_mouse_Btn_right,
    daterm_mouse_Btn_backward,
    daterm_mouse_Btn_forward,
    daterm_mouse_Btn_aux1,
    daterm_mouse_Btn_aux2,
    count$daterm_mouse_Btn
)) daterm_mouse_Btn;
T_use_prl$(daterm_mouse_Btn);

typedef enum_((daterm_mouse_Wheel $fits($packed))(
    daterm_mouse_Wheel_up = 0,
    daterm_mouse_Wheel_down,
    daterm_mouse_Wheel_left,
    daterm_mouse_Wheel_right,
    count$daterm_mouse_Wheel
)) daterm_mouse_Wheel;
T_use_prl$(daterm_mouse_Wheel);

typedef enum_((daterm_mouse_PosKind $fits($packed))(
    daterm_mouse_PosKind_cell = 0,
    daterm_mouse_PosKind_pixel
)) daterm_mouse_PosKind;
T_use_prl$(daterm_mouse_PosKind);

typedef struct daterm_mouse_Pos {
    var_(x, u16);
    var_(y, u16);
    var_(kind, daterm_mouse_PosKind);
} daterm_mouse_Pos;
T_use_prl$(daterm_mouse_Pos);

typedef struct daterm_mouse_PressEvent {
    var_(btn, daterm_mouse_Btn);
    var_(pos, daterm_mouse_Pos);
    var_(mods, daterm_input_Mods);
} daterm_mouse_PressEvent;
T_use_prl$(daterm_mouse_PressEvent);

typedef struct daterm_mouse_ReleaseEvent {
    var_(btn, O$daterm_mouse_Btn);
    var_(pos, daterm_mouse_Pos);
    var_(mods, daterm_input_Mods);
} daterm_mouse_ReleaseEvent;
T_use_prl$(daterm_mouse_ReleaseEvent);

typedef struct daterm_mouse_MotionEvent {
    var_(btn, O$daterm_mouse_Btn);
    var_(pos, daterm_mouse_Pos);
    var_(mods, daterm_input_Mods);
} daterm_mouse_MotionEvent;
T_use_prl$(daterm_mouse_MotionEvent);

typedef struct daterm_mouse_WheelEvent {
    var_(wheel, daterm_mouse_Wheel);
    var_(pos, daterm_mouse_Pos);
    var_(mods, daterm_input_Mods);
} daterm_mouse_WheelEvent;
T_use_prl$(daterm_mouse_WheelEvent);

typedef variant_((daterm_mouse_Event $fits($packed))(
    (daterm_mouse_Event_press, daterm_mouse_PressEvent),
    (daterm_mouse_Event_release, daterm_mouse_ReleaseEvent),
    (daterm_mouse_Event_motion, daterm_mouse_MotionEvent),
    (daterm_mouse_Event_wheel, daterm_mouse_WheelEvent)
)) daterm_mouse_Event;
T_use_prl$(daterm_mouse_Event);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_mouse__included */

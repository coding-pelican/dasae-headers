/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Event.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-06-20 (date of last update)
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
#include "key.h"
#include "mouse.h"
#include "focus.h"

typedef variant_((daterm_Event $fits($packed))(
    (daterm_Event_key, daterm_key_Event),
    (daterm_Event_text, daterm_key_Text),
    (daterm_Event_mouse, daterm_mouse_Event),
    (daterm_Event_focus, daterm_focus_Event),
    (daterm_Event_resize, daterm_CellSize)
)) daterm_Event;
T_use_prl$(daterm_Event);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_Event__included */

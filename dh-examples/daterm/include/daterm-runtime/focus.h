/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    focus.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-20 (date of creation)
 * @updated 2026-06-20 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm_focus
 */
#pragma once
#ifndef daterm_focus__included
#define daterm_focus__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"

typedef enum_((daterm_focus_Event $fits($packed))(
    daterm_focus_Event_in,
    daterm_focus_Event_out
)) daterm_focus_Event;
T_use_prl$(daterm_focus_Event);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_focus__included */

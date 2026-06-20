/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Caps.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-20 (date of creation)
 * @updated 2026-06-20 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm_TermCaps
 */
#pragma once
#ifndef daterm_TermCaps__included
#define daterm_TermCaps__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

typedef struct daterm_TermCaps {
    var_(native_screen_cells, bool);
    var_(native_cursor_pos, bool);
    var_(protocol_txn, bool);
    var_(pending_event_queue, bool);
    var_(flush, bool);
    var_(native_key_action, bool);
    var_(native_mouse_event, bool);
    var_(native_resize_event, bool);
    var_(native_focus_event, bool);
} daterm_TermCaps;
T_use_prl$(daterm_TermCaps);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_TermCaps__included */

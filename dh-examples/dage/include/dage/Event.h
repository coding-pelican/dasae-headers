/**
 * @file    Event.h
 * @brief   Unified event system for dage
 * @details Combines input events and window events into a single stream
 *          to preserve ordering (e.g., key down → resize → key up sequence).
 *
 * @note    This is an internal protocol (variant) - not for external extension.
 *          Backend implementations generate these events, engine consumes them.
 */
#ifndef dage_Event__included
#define dage_Event__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage/common.h"
#include "dage/KeyCode.h"
#include "dage/MouseButton.h"
#include "dh/ArrDeq.h"

/*========== Macros and Definitions =========================================*/

/*========== Event Payloads ==========*/

/// @brief Key modifier flags
typedef union dage_KeyMods {
    struct {
        u8 shift     : 1;
        u8 ctrl      : 1;
        u8 alt       : 1;
        u8 caps_lock : 1;
        u8 num_lock  : 1;
        u8 reserved_ : 3;
    };
    u8 packed;
} dage_KeyMods;

/// @brief Keyboard event payload
typedef struct dage_Event_Key {
    dage_KeyCode key;
    dage_KeyMods mods;
} dage_Event_Key;

/// @brief Mouse button event payload
typedef struct dage_Event_MouseButton {
    dage_MouseButton button;
    dage_KeyMods mods;
    m_V2i32 pos; /* Window-space coordinates */
} dage_Event_MouseButton;

/// @brief Mouse movement event payload
typedef struct dage_Event_MouseMove {
    m_V2i32 pos; /* Current position (Window-space) */
    m_V2i32 delta; /* Movement delta from last frame */
} dage_Event_MouseMove;

/// @brief Mouse scroll event payload
typedef struct dage_Event_Scroll {
    m_V2f32 delta; /* Scroll amount (x: horizontal, y: vertical) */
} dage_Event_Scroll;

/// @brief Window resize event payload
typedef struct dage_Event_Resize {
    m_V2u32 old_size;
    m_V2u32 new_size;
} dage_Event_Resize;

/// @brief Window move event payload
typedef struct dage_Event_Move {
    m_V2i32 old_pos;
    m_V2i32 new_pos;
} dage_Event_Move;

/*========== Unified Event Variant ==========*/

/// @brief Unified event type
/// @details All events (input + window) in a single variant to preserve ordering.
///          This is an internal protocol - external backends generate these,
///          engine core consumes them.
typedef variant_((dage_Event $bits(8))(
    /*=== Window Events (System) ===*/
    (dage_Event_close_request, Void), /* User requested window close */
    (dage_Event_resize, dage_Event_Resize), /* Window resized */
    (dage_Event_move, dage_Event_Move), /* Window moved */
    (dage_Event_focus, Void), /* Window gained focus */
    (dage_Event_blur, Void), /* Window lost focus */
    (dage_Event_minimize, Void), /* Window minimized */
    (dage_Event_maximize, Void), /* Window maximized */
    (dage_Event_restore, Void), /* Window restored from min/max */

    /*=== Input Events (User) ===*/
    (dage_Event_key_down, dage_Event_Key), /* Key pressed */
    (dage_Event_key_up, dage_Event_Key), /* Key released */
    (dage_Event_key_repeat, dage_Event_Key), /* Key repeat (held) */

    (dage_Event_mouse_down, dage_Event_MouseButton), /* Mouse button pressed */
    (dage_Event_mouse_up, dage_Event_MouseButton), /* Mouse button released */
    (dage_Event_mouse_move, dage_Event_MouseMove), /* Mouse moved */
    (dage_Event_mouse_enter, Void), /* Mouse entered window */
    (dage_Event_mouse_leave, Void), /* Mouse left window */
    (dage_Event_scroll, dage_Event_Scroll) /* Mouse wheel scrolled */
)) dage_Event;
T_use_prl$(dage_Event);
T_use_ArrDeq$(dage_Event);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_Event__included */

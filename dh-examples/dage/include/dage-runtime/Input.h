/**
 * @file    InputState.h
 * @brief   Input state snapshot for dage
 * @details Manages keyboard/mouse state per-window.
 *          Event queue is removed - unified Event system handles that.
 *
 * ## Design
 * - Each Window owns its InputState (no global input)
 * - State is updated when events are dispatched to Window
 * - Provides both "instantaneous" queries (pressed/released) and "continuous" queries (held)
 *
 * ## Frame Lifecycle
 * 1. Runtime pumps events from Backend
 * 2. Runtime dispatches events to Windows
 * 3. Window updates InputState via dage_Input_applyEvent()
 * 4. Game logic queries InputState
 * 5. Frame end: dage_Input_endFrame() clears pressed/released flags
 */
#ifndef dage_runtime_Input__included
#define dage_runtime_Input__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Event.h"

/*========== Macros and Declarations ========================================*/

/// @brief Input frame type (prev/curr)
typedef enum_((dage_Input_Frame $fits($packed))(
    dage_Input_Frame_curr = 0,
    dage_Input_Frame_prev = 1,
    dage_Input_Frame$count = 2
)) dage_Input_Frame;

/// @brief Combined input state (owned by Window)
typedef struct dage_Input {
    var_(keyboard, A$$(dage_Input_Frame$count, dage_Keyboard));
    var_(mouse, A$$(dage_Input_Frame$count, dage_Mouse));
    var_(mods, A$$(dage_Input_Frame$count, dage_KeyMods));
} dage_Input;

/*========== Initialization ==========*/

/// @brief Initialize input state to default values
$extern fn_((dage_Input_init(dage_Input* self))(void));
/// @brief Clear all state
$extern fn_((dage_Input_clear(dage_Input* self))(void));

/*========== Event Application ==========*/

/// @brief Apply an event to update state
/// @details Called by Window when dispatching events.
///          Updates both instantaneous (pressed/released) and continuous (held) state.
$extern fn_((dage_Input_applyEvent(dage_Input* self, const dage_Event* event))(void));
/// @brief End frame - clear pressed/released flags
/// @details Must be called at end of each frame.
///          Pressed/released are one-frame states, held persists.
$extern fn_((dage_Input_endFrame(dage_Input* self))(void));

/*========== Keyboard Queries ==========*/

/// @brief Check if key was just pressed this frame
$attr($inline_always)
$static fn_((dage_Input_isKeyPressed(const dage_Input* self, dage_KeyCode key))(bool));
/// @brief Check if key is currently held
$attr($inline_always)
$static fn_((dage_Input_isKeyHeld(const dage_Input* self, dage_KeyCode key))(bool));
/// @brief Check if key was just released this frame
$attr($inline_always)
$static fn_((dage_Input_isKeyReleased(const dage_Input* self, dage_KeyCode key))(bool));
/// @brief Get raw key state flags
$attr($inline_always)
$static fn_((dage_Input_getKeyState(const dage_Input* self, dage_KeyCode key))(dage_KeyBtnFlags));

/*========== Mouse Button Queries ==========*/

/// @brief Check if mouse button was just pressed
$attr($inline_always)
$static fn_((dage_Input_isMousePressed(const dage_Input* self, dage_MouseBtn btn))(bool));
/// @brief Check if mouse button is held
$attr($inline_always)
$static fn_((dage_Input_isMouseHeld(const dage_Input* self, dage_MouseBtn btn))(bool));
/// @brief Check if mouse button was just released
$attr($inline_always)
$static fn_((dage_Input_isMouseReleased(const dage_Input* self, dage_MouseBtn btn))(bool));
/// @brief Get raw mouse button state flags
$attr($inline_always)
$static fn_((dage_Input_getMouseState(const dage_Input* self, dage_MouseBtn btn))(dage_KeyBtnFlags));

/*========== Mouse Position/Movement Queries ==========*/

/// @brief Get current mouse position (Window-space)
$attr($inline_always)
$static fn_((dage_Input_getMousePos(const dage_Input* self))(m_V2i32));
/// @brief Get mouse movement delta since last frame
$attr($inline_always)
$static fn_((dage_Input_getMouseDelta(const dage_Input* self))(m_V2i32));
/// @brief Get scroll delta this frame
$attr($inline_always)
$static fn_((dage_Input_getScrollDelta(const dage_Input* self))(m_V2f32));
/// @brief Check if mouse is inside window
$attr($inline_always)
$static fn_((dage_Input_isMouseInWindow(const dage_Input* self))(bool));

/*========== Modifier Queries ==========*/

/// @brief Check if shift is held
$attr($inline_always)
$static fn_((dage_Input_isShiftHeld(const dage_Input* self))(bool));
/// @brief Check if ctrl is held
$attr($inline_always)
$static fn_((dage_Input_isCtrlHeld(const dage_Input* self))(bool));
/// @brief Check if alt is held
$attr($inline_always)
$static fn_((dage_Input_isAltHeld(const dage_Input* self))(bool));
/// @brief Get current modifier state
$attr($inline_always)
$static fn_((dage_Input_getMods(const dage_Input* self))(dage_KeyMods));

/*========== Macros and Definitions =========================================*/

fn_((dage_Input_isKeyPressed(const dage_Input* self, dage_KeyCode key))(bool)) {
    return (dage_Input_getKeyState(self, key) & dage_KeyBtnFlags_pressed) != 0;
};

fn_((dage_Input_isKeyHeld(const dage_Input* self, dage_KeyCode key))(bool)) {
    return (dage_Input_getKeyState(self, key) & dage_KeyBtnFlags_held) != 0;
};

fn_((dage_Input_isKeyReleased(const dage_Input* self, dage_KeyCode key))(bool)) {
    return (dage_Input_getKeyState(self, key) & dage_KeyBtnFlags_released) != 0;
};

fn_((dage_Input_getKeyState(const dage_Input* self, dage_KeyCode key))(dage_KeyBtnFlags)) {
    debug_assert_nonnull(self);
    debug_assert(key < dage_KeyCode$count);
    return *A_at((A_at((self->keyboard)[dage_Input_Frame_curr])->key_states)[key]);
};

fn_((dage_Input_isMousePressed(const dage_Input* self, dage_MouseBtn btn))(bool)) {
    return (dage_Input_getMouseState(self, btn) & dage_KeyBtnFlags_pressed) != 0;
};

fn_((dage_Input_isMouseHeld(const dage_Input* self, dage_MouseBtn btn))(bool)) {
    return (dage_Input_getMouseState(self, btn) & dage_KeyBtnFlags_held) != 0;
};

fn_((dage_Input_isMouseReleased(const dage_Input* self, dage_MouseBtn btn))(bool)) {
    return (dage_Input_getMouseState(self, btn) & dage_KeyBtnFlags_released) != 0;
};

fn_((dage_Input_getMouseState(const dage_Input* self, dage_MouseBtn btn))(dage_KeyBtnFlags)) {
    debug_assert_nonnull(self);
    debug_assert(btn < dage_MouseBtn$count);
    return *A_at((A_at((self->mouse)[dage_Input_Frame_curr])->btn_states)[btn]);
};

fn_((dage_Input_getMousePos(const dage_Input* self))(m_V2i32)) {
    debug_assert_nonnull(self);
    return A_at((self->mouse)[dage_Input_Frame_curr])->pos;
};

fn_((dage_Input_getMouseDelta(const dage_Input* self))(m_V2i32)) {
    debug_assert_nonnull(self);
    return m_V2i32_sub(A_at((self->mouse)[dage_Input_Frame_curr])->pos, A_at((self->mouse)[dage_Input_Frame_prev])->pos);
};

fn_((dage_Input_getScrollDelta(const dage_Input* self))(m_V2f32)) {
    debug_assert_nonnull(self);
    return A_at((self->mouse)[dage_Input_Frame_curr])->scroll_delta;
};

fn_((dage_Input_isMouseInWindow(const dage_Input* self))(bool)) {
    debug_assert_nonnull(self);
    return A_at((self->mouse)[dage_Input_Frame_curr])->inside_window;
};

fn_((dage_Input_isShiftHeld(const dage_Input* self))(bool)) {
    return dage_Input_getMods(self).shift;
};

fn_((dage_Input_isCtrlHeld(const dage_Input* self))(bool)) {
    return dage_Input_getMods(self).ctrl;
};

fn_((dage_Input_isAltHeld(const dage_Input* self))(bool)) {
    return dage_Input_getMods(self).alt;
};

fn_((dage_Input_getMods(const dage_Input* self))(dage_KeyMods)) {
    debug_assert_nonnull(self);
    return *A_at((self->mods)[dage_Input_Frame_curr]);
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_runtime_Input__included */

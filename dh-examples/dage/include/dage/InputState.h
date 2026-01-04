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
 * 3. Window updates InputState via dage_InputState_applyEvent()
 * 4. Game logic queries InputState
 * 5. Frame end: dage_InputState_endFrame() clears pressed/released flags
 */
#ifndef dage_InputState__included
#define dage_InputState__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage/common.h"
#include "dage/KeyCode.h"
#include "dage/MouseBtn.h"
#include "dage/Event.h"

/*========== Macros and Declarations ========================================*/

/*========== Key/Button State Flags ==========*/

/// @brief State flags for keys/buttons
typedef enum_(dage_KeyBtnState $bits(8)) {
    dage_KeyBtnState_none = 0,
    dage_KeyBtnState_pressed = (1 << 0), /* Just pressed this frame */
    dage_KeyBtnState_held = (1 << 1), /* Currently held down */
    dage_KeyBtnState_released = (1 << 2), /* Just released this frame */
} dage_KeyBtnState;

/*========== Keyboard State ==========*/

/// @brief Keyboard state snapshot
typedef struct dage_KeyboardState {
    A$$(dage_KeyCode_count, dage_KeyBtnState) keys;
} dage_KeyboardState;

/*========== Mouse State ==========*/

/// @brief Mouse state snapshot
typedef struct dage_MouseState {
    A$$(dage_MouseBtn_count, dage_KeyBtnState) btns;
    m_V2i32 pos; /* Current position (Window-space) */
    m_V2i32 pos_prev; /* Previous frame position */
    m_V2f32 scroll; /* Scroll delta this frame */
    bool inside_window; /* Mouse is inside window bounds */
} dage_MouseState;

/*========== Combined Input State ==========*/

/// @brief Combined input state (owned by Window)
typedef struct dage_InputState {
    dage_KeyboardState keyboard;
    dage_MouseState mouse;
    dage_KeyMods curr_mods; /* Current modifier state */
} dage_InputState;

/*========== Initialization ==========*/

/// @brief Initialize input state to default values
$extern fn_((dage_InputState_init(dage_InputState* self))(void));
/// @brief Clear all state
$extern fn_((dage_InputState_clear(dage_InputState* self))(void));

/*========== Event Application ==========*/

/// @brief Apply an event to update state
/// @details Called by Window when dispatching events.
///          Updates both instantaneous (pressed/released) and continuous (held) state.
$extern fn_((dage_InputState_applyEvent(dage_InputState* self, const dage_Event* event))(void));
/// @brief End frame - clear pressed/released flags
/// @details Must be called at end of each frame.
///          Pressed/released are one-frame states, held persists.
$extern fn_((dage_InputState_endFrame(dage_InputState* self))(void));

/*========== Keyboard Queries ==========*/

/// @brief Check if key was just pressed this frame
$attr($inline_always)
$static fn_((dage_InputState_isKeyPressed(const dage_InputState* self, dage_KeyCode key))(bool));
/// @brief Check if key is currently held
$attr($inline_always)
$static fn_((dage_InputState_isKeyHeld(const dage_InputState* self, dage_KeyCode key))(bool));
/// @brief Check if key was just released this frame
$attr($inline_always)
$static fn_((dage_InputState_isKeyReleased(const dage_InputState* self, dage_KeyCode key))(bool));
/// @brief Get raw key state flags
$attr($inline_always)
$static fn_((dage_InputState_getKeyState(const dage_InputState* self, dage_KeyCode key))(dage_KeyBtnState));

/*========== Mouse Button Queries ==========*/

/// @brief Check if mouse button was just pressed
$attr($inline_always)
$static fn_((dage_InputState_isMousePressed(const dage_InputState* self, dage_MouseBtn button))(bool));
/// @brief Check if mouse button is held
$attr($inline_always)
$static fn_((dage_InputState_isMouseHeld(const dage_InputState* self, dage_MouseBtn button))(bool));
/// @brief Check if mouse button was just released
$attr($inline_always)
$static fn_((dage_InputState_isMouseReleased(const dage_InputState* self, dage_MouseBtn button))(bool));

/*========== Mouse Position/Movement Queries ==========*/

/// @brief Get current mouse position (Window-space)
$attr($inline_always)
$static fn_((dage_InputState_getMousePos(const dage_InputState* self))(m_V2i32));
/// @brief Get mouse movement delta since last frame
$attr($inline_always)
$static fn_((dage_InputState_getMouseDelta(const dage_InputState* self))(m_V2i32));
/// @brief Get scroll delta this frame
$attr($inline_always)
$static fn_((dage_InputState_getScrollDelta(const dage_InputState* self))(m_V2f32));
/// @brief Check if mouse is inside window
$attr($inline_always)
$static fn_((dage_InputState_isMouseInWindow(const dage_InputState* self))(bool));

/*========== Modifier Queries ==========*/

/// @brief Check if shift is held
$attr($inline_always)
$static fn_((dage_InputState_isShiftHeld(const dage_InputState* self))(bool));
/// @brief Check if ctrl is held
$attr($inline_always)
$static fn_((dage_InputState_isCtrlHeld(const dage_InputState* self))(bool));
/// @brief Check if alt is held
$attr($inline_always)
$static fn_((dage_InputState_isAltHeld(const dage_InputState* self))(bool));
/// @brief Get current modifier state
$attr($inline_always)
$static fn_((dage_InputState_getMods(const dage_InputState* self))(dage_KeyMods));

/*========== Macros and Definitions =========================================*/

fn_((dage_InputState_isKeyPressed(const dage_InputState* self, dage_KeyCode key))(bool)) {
    debug_assert_nonnull(self);
    debug_assert(key < dage_KeyCode_count);
    return (*A_at((self->keyboard.keys)[key]) & dage_KeyBtnState_pressed) != 0;
};

fn_((dage_InputState_isKeyHeld(const dage_InputState* self, dage_KeyCode key))(bool)) {
    debug_assert_nonnull(self);
    debug_assert(key < dage_KeyCode_count);
    return (*A_at((self->keyboard.keys)[key]) & dage_KeyBtnState_held) != 0;
};

fn_((dage_InputState_isKeyReleased(const dage_InputState* self, dage_KeyCode key))(bool)) {
    debug_assert_nonnull(self);
    debug_assert(key < dage_KeyCode_count);
    return (*A_at((self->keyboard.keys)[key]) & dage_KeyBtnState_released) != 0;
};

fn_((dage_InputState_getKeyState(const dage_InputState* self, dage_KeyCode key))(dage_KeyBtnState)) {
    debug_assert_nonnull(self);
    debug_assert(key < dage_KeyCode_count);
    return *A_at((self->keyboard.keys)[key]);
};

fn_((dage_InputState_isMousePressed(const dage_InputState* self, dage_MouseBtn button))(bool)) {
    debug_assert_nonnull(self);
    debug_assert(button < dage_MouseBtn_count);
    return (*A_at((self->mouse.btns)[button]) & dage_KeyBtnState_pressed) != 0;
};

fn_((dage_InputState_isMouseHeld(const dage_InputState* self, dage_MouseBtn button))(bool)) {
    debug_assert_nonnull(self);
    debug_assert(button < dage_MouseBtn_count);
    return (*A_at((self->mouse.btns)[button]) & dage_KeyBtnState_held) != 0;
};

fn_((dage_InputState_isMouseReleased(const dage_InputState* self, dage_MouseBtn button))(bool)) {
    debug_assert_nonnull(self);
    debug_assert(button < dage_MouseBtn_count);
    return (*A_at((self->mouse.btns)[button]) & dage_KeyBtnState_released) != 0;
};

fn_((dage_InputState_getMousePos(const dage_InputState* self))(m_V2i32)) {
    debug_assert_nonnull(self);
    return self->mouse.pos;
};

fn_((dage_InputState_getMouseDelta(const dage_InputState* self))(m_V2i32)) {
    debug_assert_nonnull(self);
    return m_V2i32_sub(self->mouse.pos, self->mouse.pos_prev);
};

fn_((dage_InputState_getScrollDelta(const dage_InputState* self))(m_V2f32)) {
    debug_assert_nonnull(self);
    return self->mouse.scroll;
};

fn_((dage_InputState_isMouseInWindow(const dage_InputState* self))(bool)) {
    debug_assert_nonnull(self);
    return self->mouse.inside_window;
};

fn_((dage_InputState_isShiftHeld(const dage_InputState* self))(bool)) {
    debug_assert_nonnull(self);
    return self->curr_mods.shift;
};

fn_((dage_InputState_isCtrlHeld(const dage_InputState* self))(bool)) {
    debug_assert_nonnull(self);
    return self->curr_mods.ctrl;
};

fn_((dage_InputState_isAltHeld(const dage_InputState* self))(bool)) {
    debug_assert_nonnull(self);
    return self->curr_mods.alt;
};

fn_((dage_InputState_getMods(const dage_InputState* self))(dage_KeyMods)) {
    debug_assert_nonnull(self);
    return self->curr_mods;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_InputState__included */

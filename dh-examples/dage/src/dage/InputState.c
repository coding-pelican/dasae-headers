#include "dage/InputState.h"

/*========== Initialization ==========*/

fn_((dage_InputState_init(dage_InputState* self))(void)) {
    claim_assert_nonnull(self);
    dage_InputState_clear(self);
};

fn_((dage_InputState_clear(dage_InputState* self))(void)) {
    claim_assert_nonnull(self);

    /* Clear keyboard state */
    mem_setBytes0(mem_asBytes(&self->keyboard.keys));

    /* Clear mouse state */
    mem_setBytes0(mem_asBytes(&self->mouse.buttons));
    self->mouse.position = m_V2i32_zero;
    self->mouse.position_prev = m_V2i32_zero;
    self->mouse.scroll = m_V2f32_zero;
    self->mouse.inside_window = false;

    /* Clear modifiers */
    self->current_mods.packed = 0;
};

/*========== Event Application ==========*/

/// @brief Update modifier state from key event
$static fn_((updateModsFromKey(dage_InputState* self, dage_KeyCode key, bool is_down))(void)) {
    switch (key) {
    case dage_KeyCode_shift_left:
    case dage_KeyCode_shift_right:
        self->current_mods.shift = is_down;
        break;
    case dage_KeyCode_ctrl_left:
    case dage_KeyCode_ctrl_right:
        self->current_mods.ctrl = is_down;
        break;
    case dage_KeyCode_alt_left:
    case dage_KeyCode_alt_right:
        self->current_mods.alt = is_down;
        break;
    default:
        break;
    }
};

fn_((dage_InputState_applyEvent(dage_InputState* self, const dage_Event* event))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(event);

    match_(*event) {
    /*=== Keyboard Events ===*/
    pattern_((dage_Event_key_down)(on_key_down)) {
        let state = A_at((self->keyboard.keys)[on_key_down->key]);
        /* Only set pressed if not already held */
        if (!(*state & dage_ButtonState_held)) {
            *state |= dage_ButtonState_pressed;
        }
        *state |= dage_ButtonState_held;
        updateModsFromKey(self, on_key_down->key, true);
    } $end(pattern);

    pattern_((dage_Event_key_up)(on_key_up)) {
        let state = A_at((self->keyboard.keys)[on_key_up->key]);
        *state &= ~dage_ButtonState_held;
        *state |= dage_ButtonState_released;
        updateModsFromKey(self, on_key_up->key, false);
    } $end(pattern);

    pattern_((dage_Event_key_repeat)($ignore)) {
        /* Key repeat doesn't change state, just confirms held */
    } $end(pattern);

    /*=== Mouse Button Events ===*/
    pattern_((dage_Event_mouse_down)(on_mouse_down)) {
        let state = A_at((self->mouse.buttons)[on_mouse_down->button]);
        if (!(*state & dage_ButtonState_held)) {
            *state |= dage_ButtonState_pressed;
        }
        *state |= dage_ButtonState_held;
        self->mouse.position = on_mouse_down->pos;
    } $end(pattern);

    pattern_((dage_Event_mouse_up)(on_mouse_up)) {
        let state = A_at((self->mouse.buttons)[on_mouse_up->button]);
        *state &= ~dage_ButtonState_held;
        *state |= dage_ButtonState_released;
        self->mouse.position = on_mouse_up->pos;
    } $end(pattern);

    /*=== Mouse Movement Events ===*/
    pattern_((dage_Event_mouse_move)(on_mouse_move)) {
        self->mouse.position = on_mouse_move->pos;
    } $end(pattern);

    pattern_((dage_Event_mouse_enter)($ignore)) {
        self->mouse.inside_window = true;
    } $end(pattern);

    pattern_((dage_Event_mouse_leave)($ignore)) {
        self->mouse.inside_window = false;
    } $end(pattern);

    pattern_((dage_Event_scroll)(on_scroll)) {
        /* Accumulate scroll for this frame */
        self->mouse.scroll.x += on_scroll->delta.x;
        self->mouse.scroll.y += on_scroll->delta.y;
    } $end(pattern);

    /*=== Window Events - No input state change ===*/
    case dage_Event_close_request: $fallthrough;
    case dage_Event_resize:        $fallthrough;
    case dage_Event_move:          $fallthrough;
    case dage_Event_focus:         $fallthrough;
    case dage_Event_blur:          {
        /* On blur, might want to release all held keys */
        /* For now, do nothing - user can handle this */
        $fallthrough;
    };
    case dage_Event_minimize: $fallthrough;
    case dage_Event_maximize: $fallthrough;
    case dage_Event_restore:  $fallthrough;
    default_() {
        /* Unknown event - ignore */
    } $end(default);
    } $end(match);
};

fn_((dage_InputState_endFrame(dage_InputState* self))(void)) {
    claim_assert_nonnull(self);

    /* Clear pressed/released flags for keys (held remains) */
    for_(($s(A_ref(self->keyboard.keys)))(state) {
        *state &= ~(dage_ButtonState_pressed | dage_ButtonState_released);
    });

    /* Clear pressed/released flags for mouse buttons */
    for_(($s(A_ref(self->mouse.buttons)))(state) {
        *state &= ~(dage_ButtonState_pressed | dage_ButtonState_released);
    });

    /* Save current position for delta calculation next frame */
    self->mouse.position_prev = self->mouse.position;

    /* Reset scroll accumulator */
    self->mouse.scroll = m_V2f32_zero;
};

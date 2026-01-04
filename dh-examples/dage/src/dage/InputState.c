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
    mem_setBytes0(mem_asBytes(&self->mouse.btns));
    self->mouse.pos = m_V2i32_zero;
    self->mouse.pos_prev = m_V2i32_zero;
    self->mouse.scroll = m_V2f32_zero;
    self->mouse.inside_window = false;

    /* Clear modifiers */
    self->curr_mods.packed = 0;
};

/*========== Event Application ==========*/

/// @brief Update modifier state from key event
$static fn_((updateModsFromKey(dage_InputState* self, dage_KeyCode key, bool is_down))(void)) {
    $supress_switch_enum(switch (key) {
        case dage_KeyCode_shift_left:
        case dage_KeyCode_shift_right:
            self->curr_mods.shift = is_down;
            break;
        case dage_KeyCode_ctrl_left:
        case dage_KeyCode_ctrl_right:
            self->curr_mods.ctrl = is_down;
            break;
        case dage_KeyCode_alt_left:
        case dage_KeyCode_alt_right:
            self->curr_mods.alt = is_down;
            break;
        default:
            break;
    });
};

fn_((dage_InputState_applyEvent(dage_InputState* self, const dage_Event* event))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(event);

    match_(*event) {
    /*=== Keyboard Events ===*/
    pattern_((dage_Event_key_down)(on_key_down)) {
        if (on_key_down->key == dage_KeyCode_unknown) { return; }
        claim_assert(dage_KeyCode_unknown < on_key_down->key);
        claim_assert(on_key_down->key < dage_KeyCode_count);
        let state = A_at((self->keyboard.keys)[on_key_down->key]);
        /* Only set pressed if not already held */
        if (!(*state & dage_KeyBtnState_held)) {
            *state |= dage_KeyBtnState_pressed;
        }
        *state |= dage_KeyBtnState_held;
        updateModsFromKey(self, on_key_down->key, true);
    } $end(pattern);

    pattern_((dage_Event_key_up)(on_key_up)) {
        if (on_key_up->key == dage_KeyCode_unknown) { return; }
        claim_assert(dage_KeyCode_unknown < on_key_up->key);
        claim_assert(on_key_up->key < dage_KeyCode_count);
        let state = A_at((self->keyboard.keys)[on_key_up->key]);
        *state &= ~dage_KeyBtnState_held;
        *state |= dage_KeyBtnState_released;
        updateModsFromKey(self, on_key_up->key, false);
    } $end(pattern);

    pattern_((dage_Event_key_repeat)($ignore)) {
        /* Key repeat doesn't change state, just confirms held */
    } $end(pattern);

    /*=== Mouse Button Events ===*/
    pattern_((dage_Event_mouse_down)(on_mouse_down)) {
        if (on_mouse_down->btn == dage_MouseBtn_unknown) { return; }
        claim_assert(dage_MouseBtn_unknown < on_mouse_down->btn);
        claim_assert(on_mouse_down->btn < dage_MouseBtn_count);
        let state = A_at((self->mouse.btns)[on_mouse_down->btn]);
        if (!(*state & dage_KeyBtnState_held)) {
            *state |= dage_KeyBtnState_pressed;
        }
        *state |= dage_KeyBtnState_held;
        self->mouse.pos = on_mouse_down->pos;
    } $end(pattern);

    pattern_((dage_Event_mouse_up)(on_mouse_up)) {
        if (on_mouse_up->btn == dage_MouseBtn_unknown) { return; }
        claim_assert(dage_MouseBtn_unknown < on_mouse_up->btn);
        claim_assert(on_mouse_up->btn < dage_MouseBtn_count);
        let state = A_at((self->mouse.btns)[on_mouse_up->btn]);
        *state &= ~dage_KeyBtnState_held;
        *state |= dage_KeyBtnState_released;
        self->mouse.pos = on_mouse_up->pos;
    } $end(pattern);

    /*=== Mouse Movement Events ===*/
    pattern_((dage_Event_mouse_move)(on_mouse_move)) {
        self->mouse.pos = on_mouse_move->pos;
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
        *state &= ~(dage_KeyBtnState_pressed | dage_KeyBtnState_released);
    });

    /* Clear pressed/released flags for mouse buttons */
    for_(($s(A_ref(self->mouse.btns)))(state) {
        *state &= ~(dage_KeyBtnState_pressed | dage_KeyBtnState_released);
    });

    /* Save current position for delta calculation next frame */
    self->mouse.pos_prev = self->mouse.pos;

    /* Reset scroll accumulator */
    self->mouse.scroll = m_V2f32_zero;
};

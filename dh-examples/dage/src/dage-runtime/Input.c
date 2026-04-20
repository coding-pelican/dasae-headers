#include "dage-runtime/Input.h"

/*========== Initialization ==========*/

fn_((dage_Input_init(dage_Input* self))(void)) {
    claim_assert_nonnull(self);
    dage_Input_clear(self);
};

fn_((dage_Input_clear(dage_Input* self))(void)) {
    claim_assert_nonnull(self);
#if UNUSED_CODE
    mem_set0Bytes(mem_asBytesMut(u_anyP(&self->keyboard)));
    mem_set0Bytes(mem_asBytesMut(u_anyP(&self->mouse)));
    mem_set0Bytes(mem_asBytesMut(u_anyP(&self->mods)));
#endif /* UNUSED_CODE */
    mem_set0Bytes(mem_asBytesMut(u_anyP(self)));
};

/*========== Event Application ==========*/

/// @brief Update modifier state from key event
$static fn_((updateModsFromKey(dage_Input* self, dage_KeyCode key, bool is_down))(void)) {
    $supress_switch_enum(switch (key) {
        case dage_KeyCode_shift_left:
        case dage_KeyCode_shift_right:
            A_at((self->mods)[dage_Input_Frame_curr])->shift = is_down;
            break;
        case dage_KeyCode_ctrl_left:
        case dage_KeyCode_ctrl_right:
            A_at((self->mods)[dage_Input_Frame_curr])->ctrl = is_down;
            break;
        case dage_KeyCode_alt_left:
        case dage_KeyCode_alt_right:
            A_at((self->mods)[dage_Input_Frame_curr])->alt = is_down;
            break;
        default:
            break;
    });
};

fn_((dage_Input_applyEvent(dage_Input* self, const dage_Event* event))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(event);

    match_(*event) {
    /*=== Keyboard Events ===*/
    pattern_((dage_Event_key_down)(on_key_down)) {
        if (on_key_down.key == dage_KeyCode_unknown) { return; }
        claim_assert(dage_KeyCode_unknown != on_key_down.key);
        claim_assert(on_key_down.key < dage_KeyCode$count);
        let state = A_at((A_at((self->keyboard)[dage_Input_Frame_curr])->key_states)[on_key_down.key]);
        /* Only set pressed if not already held */
        if (!(*state & dage_KeyBtnFlags_held)) {
            *state |= dage_KeyBtnFlags_pressed;
        }
        *state |= dage_KeyBtnFlags_held;
        updateModsFromKey(self, on_key_down.key, true);
    } $end(pattern);

    pattern_((dage_Event_key_up)(on_key_up)) {
        if (on_key_up.key == dage_KeyCode_unknown) { return; }
        claim_assert(dage_KeyCode_unknown != on_key_up.key);
        claim_assert(on_key_up.key < dage_KeyCode$count);
        let state = A_at((A_at((self->keyboard)[dage_Input_Frame_curr])->key_states)[on_key_up.key]);
        *state &= ~dage_KeyBtnFlags_held;
        *state |= dage_KeyBtnFlags_released;
        updateModsFromKey(self, on_key_up.key, false);
    } $end(pattern);

    pattern_((dage_Event_key_repeat)($ignore)) {
        /* Key repeat doesn't change state, just confirms held */
    } $end(pattern);

    /*=== Mouse Button Events ===*/
    pattern_((dage_Event_mouse_down)(on_mouse_down)) {
        if (on_mouse_down.btn == dage_MouseBtn_unknown) { return; }
        claim_assert(dage_MouseBtn_unknown != on_mouse_down.btn);
        claim_assert(on_mouse_down.btn < dage_MouseBtn$count);
        let state = A_at((A_at((self->mouse)[dage_Input_Frame_curr])->btn_states)[on_mouse_down.btn]);
        if (!(*state & dage_KeyBtnFlags_held)) {
            *state |= dage_KeyBtnFlags_pressed;
        }
        *state |= dage_KeyBtnFlags_held;
        A_at((self->mouse)[dage_Input_Frame_curr])->pos = on_mouse_down.pos;
    } $end(pattern);

    pattern_((dage_Event_mouse_up)(on_mouse_up)) {
        if (on_mouse_up.btn == dage_MouseBtn_unknown) { return; }
        claim_assert(dage_MouseBtn_unknown != on_mouse_up.btn);
        claim_assert(on_mouse_up.btn < dage_MouseBtn$count);
        let state = A_at((A_at((self->mouse)[dage_Input_Frame_curr])->btn_states)[on_mouse_up.btn]);
        *state &= ~dage_KeyBtnFlags_held;
        *state |= dage_KeyBtnFlags_released;
        A_at((self->mouse)[dage_Input_Frame_curr])->pos = on_mouse_up.pos;
    } $end(pattern);

    /*=== Mouse Movement Events ===*/
    pattern_((dage_Event_mouse_move)(on_mouse_move)) {
        A_at((self->mouse)[dage_Input_Frame_curr])->pos = on_mouse_move.pos;
    } $end(pattern);

    pattern_((dage_Event_mouse_enter)($ignore)) {
        A_at((self->mouse)[dage_Input_Frame_curr])->inside_window = true;
    } $end(pattern);

    pattern_((dage_Event_mouse_leave)($ignore)) {
        A_at((self->mouse)[dage_Input_Frame_curr])->inside_window = false;
    } $end(pattern);

    pattern_((dage_Event_scroll)(on_scroll)) {
        /* Accumulate scroll for this frame */
        A_at((self->mouse)[dage_Input_Frame_curr])->scroll_delta.x += on_scroll.delta.x;
        A_at((self->mouse)[dage_Input_Frame_curr])->scroll_delta.y += on_scroll.delta.y;
    } $end(pattern);

    /*=== Window Events - No input state change ===*/
    case dage_Event_close_request: $fallthrough;
    case dage_Event_resize: $fallthrough;
    case dage_Event_move: $fallthrough;
    case dage_Event_focus: $fallthrough;
    case dage_Event_blur: {
        /* On blur, might want to release all held keys */
        /* For now, do nothing - user can handle this */
        $fallthrough;
    };
    case dage_Event_minimize: $fallthrough;
    case dage_Event_maximize: $fallthrough;
    case dage_Event_restore: $fallthrough;
    default_() {
        /* Unknown event - ignore */
    } $end(default);
    } $end(match);
};

fn_((dage_Input_endFrame(dage_Input* self))(void)) {
    claim_assert_nonnull(self);

    mem_copyBytes(
        mem_asBytesMut(u_anyP(A_at((self->keyboard)[dage_Input_Frame_prev]))),
        mem_asBytes(u_anyP(A_at((self->keyboard)[dage_Input_Frame_curr])).as_const)
    );
    mem_copyBytes(
        mem_asBytesMut(u_anyP(A_at((self->mouse)[dage_Input_Frame_prev]))),
        mem_asBytes(u_anyP(A_at((self->mouse)[dage_Input_Frame_curr])).as_const)
    );
    mem_copyBytes(
        mem_asBytesMut(u_anyP(A_at((self->mods)[dage_Input_Frame_prev]))),
        mem_asBytes(u_anyP(A_at((self->mods)[dage_Input_Frame_curr])).as_const)
    );

    /* Clear pressed/released flags for keys (held remains) */
    for_(($s(A_ref(A_at((self->keyboard)[dage_Input_Frame_curr])->key_states)))(state)) {
        *state &= ~(dage_KeyBtnFlags_pressed | dage_KeyBtnFlags_released);
    } $end(for);

    /* Clear pressed/released flags for mouse buttons */
    for_(($s(A_ref(A_at((self->mouse)[dage_Input_Frame_curr])->btn_states)))(state)) {
        *state &= ~(dage_KeyBtnFlags_pressed | dage_KeyBtnFlags_released);
    } $end(for);

    /* Reset scroll accumulator */
    A_at((self->mouse)[dage_Input_Frame_curr])->scroll_delta = m_V2f32_zero;
};

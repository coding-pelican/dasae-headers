#include "engine/Mouse.h"
#include "engine/KeyButtonStates.h"

fn_((engine_Mouse_getState(const engine_Mouse* self, engine_MouseButton button))(engine_KeyButtonStates)) {
    debug_assert_nonnull(self);
    debug_assert_true(engine_MouseButton_none < button);
    debug_assert_true(button < engine_MouseButton_count);
    return Arr_getAt(self->buttons.curr_states, as$((usize)(button)));
}

fn_((engine_Mouse_isState(const engine_Mouse* self, engine_MouseButton button, engine_KeyButtonStates state))(bool)) {
    debug_assert_nonnull(self);
    debug_assert_true(engine_KeyButtonStates_none <= state);
    debug_assert_true(state <= (engine_KeyButtonStates_pressed | engine_KeyButtonStates_held | engine_KeyButtonStates_released));
    return (engine_Mouse_getState(self, button) & state) != engine_KeyButtonStates_none;
}

fn_((engine_Mouse_pressed(const engine_Mouse* self, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(self);
    return engine_Mouse_isState(self, button, engine_KeyButtonStates_pressed);
}

fn_((engine_Mouse_held(const engine_Mouse* self, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(self);
    return engine_Mouse_isState(self, button, engine_KeyButtonStates_held);
}

fn_((engine_Mouse_released(const engine_Mouse* self, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(self);
    return engine_Mouse_isState(self, button, engine_KeyButtonStates_released);
}

fn_((engine_Mouse_getPos(const engine_Mouse* self))(Vec2i)) {
    debug_assert_nonnull(self);
    return self->cursor.curr_pos;
}

fn_((engine_Mouse_getPosDelta(const engine_Mouse* self))(Vec2i)) {
    debug_assert_nonnull(self);
    return m_V2i32_sub(self->cursor.curr_pos, self->cursor.prev_pos);
}

fn_((engine_Mouse_getWheelScrollDelta(const engine_Mouse* self))(Vec2f)) {
    debug_assert_nonnull(self);
    return m_V2f32_sub(self->wheel.curr_scroll_amount, self->wheel.prev_scroll_amount);
}

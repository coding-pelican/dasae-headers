#include "engine-wip/Mouse.h"
#include "engine-wip/KeyButtonStates.h"

engine_KeyButtonStates engine_Mouse_getState(const engine_Mouse* self, engine_MouseButton button) {
    debug_assert_nonnull(self);
    debug_assert_true(engine_MouseButton_none < button);
    debug_assert_true(button < engine_MouseButton_count);
    return self->buttons.curr_states[button];
}

bool engine_Mouse_isState(const engine_Mouse* self, engine_MouseButton button, engine_KeyButtonStates state) {
    debug_assert_nonnull(self);
    debug_assert_true(engine_KeyButtonStates_none <= state);
    debug_assert_true(state <= (engine_KeyButtonStates_pressed | engine_KeyButtonStates_held | engine_KeyButtonStates_released));
    return (engine_Mouse_getState(self, button) & state) != engine_KeyButtonStates_none;
}

bool engine_Mouse_pressed(const engine_Mouse* self, engine_MouseButton button) {
    debug_assert_nonnull(self);
    return engine_Mouse_isState(self, button, engine_KeyButtonStates_pressed);
}

bool engine_Mouse_held(const engine_Mouse* self, engine_MouseButton button) {
    debug_assert_nonnull(self);
    return engine_Mouse_isState(self, button, engine_KeyButtonStates_held);
}

bool engine_Mouse_released(const engine_Mouse* self, engine_MouseButton button) {
    debug_assert_nonnull(self);
    return engine_Mouse_isState(self, button, engine_KeyButtonStates_released);
}

Vec2i engine_Mouse_getPos(const engine_Mouse* self) {
    debug_assert_nonnull(self);
    return self->cursor.curr_pos;
}

Vec2i engine_Mouse_getPosDelta(const engine_Mouse* self) {
    debug_assert_nonnull(self);
    return math_Vec2i_sub(self->cursor.curr_pos, self->cursor.prev_pos);
}

Vec2f engine_Mouse_getWheelScrollDelta(const engine_Mouse* self) {
    debug_assert_nonnull(self);
    return math_Vec2f_sub(self->wheel.curr_scroll_amount, self->wheel.prev_scroll_amount);
}

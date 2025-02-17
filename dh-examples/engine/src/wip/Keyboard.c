#include "engine-wip/Keyboard.h"

engine_KeyButtonStates engine_Keyboard_getState(const engine_Keyboard* self, engine_KeyCode key) {
    debug_assert_nonnull(self);
    debug_assert_true(engine_KeyCode_none < key);
    debug_assert_true(key < engine_KeyCode_count);
    return self->keys.curr_states[key];
}

bool engine_Keyboard_isState(const engine_Keyboard* self, engine_KeyCode key, engine_KeyButtonStates state) {
    debug_assert_nonnull(self);
    debug_assert_true(engine_KeyButtonStates_none <= state);
    debug_assert_true(state <= (engine_KeyButtonStates_pressed | engine_KeyButtonStates_held | engine_KeyButtonStates_released));
    return (engine_Keyboard_getState(self, key) & state) != engine_KeyButtonStates_none;
}

bool engine_Keyboard_pressed(const engine_Keyboard* self, engine_KeyCode key) {
    debug_assert_nonnull(self);
    return engine_Keyboard_isState(self, key, engine_KeyButtonStates_pressed);
}

bool engine_Keyboard_held(const engine_Keyboard* self, engine_KeyCode key) {
    debug_assert_nonnull(self);
    return engine_Keyboard_isState(self, key, engine_KeyButtonStates_held);
}

bool engine_Keyboard_released(const engine_Keyboard* self, engine_KeyCode key) {
    debug_assert_nonnull(self);
    return engine_Keyboard_isState(self, key, engine_KeyButtonStates_released);
}

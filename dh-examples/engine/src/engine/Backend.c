#include "engine/Backend.h"
#include "Backend_Internal.h"

fn_((engine_Backend_processEvents(engine_Backend self))(void)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    self.vt->processEvents(self.ptr);
}

fn_((engine_Backend_presentBuffer(engine_Backend self))(void)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    self.vt->presentBuffer(self.ptr);
}

fn_((engine_Backend_getWindowPos(const engine_Backend self))(Vec2i)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowPos(self.ptr);
}

fn_((engine_Backend_getWindowDim(const engine_Backend self))(Vec2u)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowDim(self.ptr);
}

fn_((engine_Backend_getWindowRes(const engine_Backend self))(Vec2u)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowRes(self.ptr);
}

fn_((engine_Backend_getWindowMinRes(const engine_Backend self))(Vec2u)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowMinRes(self.ptr);
}

fn_((engine_Backend_getWindowMaxRes(const engine_Backend self))(Vec2u)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowMaxRes(self.ptr);
}

fn_((engine_Backend_setWindowMinRes(engine_Backend self, Vec2u size))(Err$void)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->setWindowMinRes(self.ptr, size);
}

fn_((engine_Backend_setWindowMaxRes(engine_Backend self, Vec2u size))(Err$void)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->setWindowMaxRes(self.ptr, size);
}

fn_((engine_Backend_isWindowFocused(const engine_Backend self))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->isWindowFocused(self.ptr);
}

fn_((engine_Backend_isWindowMinimized(const engine_Backend self))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->isWindowMinimized(self.ptr);
}

fn_((engine_Backend_isWindowMaximized(const engine_Backend self))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->isWindowMaximized(self.ptr);
}

fn_((engine_Backend_getKeyboardState(const engine_Backend self, engine_KeyCode key))(u8)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getKeyboardState(self.ptr, key);
}

fn_((engine_Backend_isKeyboardState(const engine_Backend self, engine_KeyCode key, engine_KeyButtonStates state))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->isKeyboardState(self.ptr, key, state);
}

fn_((engine_Backend_pressedKeyboard(const engine_Backend self, engine_KeyCode key))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->pressedKeyboard(self.ptr, key);
}

fn_((engine_Backend_heldKeyboard(const engine_Backend self, engine_KeyCode key))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->heldKeyboard(self.ptr, key);
}

fn_((engine_Backend_releasedKeyboard(const engine_Backend self, engine_KeyCode key))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->releasedKeyboard(self.ptr, key);
}

fn_((engine_Backend_getMouseState(const engine_Backend self, engine_MouseButton button))(u8)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getMouseState(self.ptr, button);
}

fn_((engine_Backend_isMouseState(const engine_Backend self, engine_MouseButton button, engine_KeyButtonStates state))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->isMouseState(self.ptr, button, state);
}

fn_((engine_Backend_pressedMouse(const engine_Backend self, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->pressedMouse(self.ptr, button);
}

fn_((engine_Backend_heldMouse(const engine_Backend self, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->heldMouse(self.ptr, button);
}

fn_((engine_Backend_releasedMouse(const engine_Backend self, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->releasedMouse(self.ptr, button);
}

fn_((engine_Backend_getMousePos(const engine_Backend self))(Vec2i)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getMousePos(self.ptr);
}

fn_((engine_Backend_getMousePosDelta(const engine_Backend self))(Vec2i)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getMousePosDelta(self.ptr);
}

fn_((engine_Backend_getMouseWheelScrollDelta(const engine_Backend self))(Vec2f)) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getMouseWheelScrollDelta(self.ptr);
}

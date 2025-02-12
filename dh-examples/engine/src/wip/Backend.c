#include "engine-wip/Backend.h"
#include "Backend_Internal.h"

void engine_Backend_processEvents(engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    self.vt->processEvents(self.ptr);
}

void engine_Backend_presentBuffer(engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    self.vt->presentBuffer(self.ptr);
}

Vec2u engine_Backend_getWindowPos(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowPos(self.ptr);
}

Vec2u engine_Backend_getWindowDim(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowDim(self.ptr);
}

Vec2u engine_Backend_getWindowRes(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowRes(self.ptr);
}

Vec2u engine_Backend_getWindowMinRes(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowMinRes(self.ptr);
}

Vec2u engine_Backend_getWindowMaxRes(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowMaxRes(self.ptr);
}

Err$void engine_Backend_setWindowMinRes(engine_Backend self, Vec2u size) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->setWindowMinRes(self.ptr, size);
}

Err$void engine_Backend_setWindowMaxRes(engine_Backend self, Vec2u size) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->setWindowMaxRes(self.ptr, size);
}

bool engine_Backend_isWindowFocused(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->isWindowFocused(self.ptr);
}

bool engine_Backend_isWindowMinimized(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->isWindowMinimized(self.ptr);
}

bool engine_Backend_isWindowMaximized(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->isWindowMaximized(self.ptr);
}

u8 engine_Backend_getKeyboardState(const engine_Backend self, engine_KeyCode key) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getKeyboardState(self.ptr, key);
}

bool engine_Backend_isKeyboardState(const engine_Backend self, engine_KeyCode key, engine_KeyButtonStates state) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->isKeyboardState(self.ptr, key, state);
}

bool engine_Backend_pressedKeyboard(const engine_Backend self, engine_KeyCode key) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->pressedKeyboard(self.ptr, key);
}

bool engine_Backend_heldKeyboard(const engine_Backend self, engine_KeyCode key) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->heldKeyboard(self.ptr, key);
}

bool engine_Backend_releasedKeyboard(const engine_Backend self, engine_KeyCode key) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->releasedKeyboard(self.ptr, key);
}

u8 engine_Backend_getMouseState(const engine_Backend self, engine_MouseButton button) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getMouseState(self.ptr, button);
}

bool engine_Backend_isMouseState(const engine_Backend self, engine_MouseButton button, engine_KeyButtonStates state) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->isMouseState(self.ptr, button, state);
}

bool engine_Backend_pressedMouse(const engine_Backend self, engine_MouseButton button) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->pressedMouse(self.ptr, button);
}

bool engine_Backend_heldMouse(const engine_Backend self, engine_MouseButton button) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->heldMouse(self.ptr, button);
}

bool engine_Backend_releasedMouse(const engine_Backend self, engine_MouseButton button) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->releasedMouse(self.ptr, button);
}

Vec2i engine_Backend_getMousePos(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getMousePos(self.ptr);
}

Vec2i engine_Backend_getMousePosDelta(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getMousePosDelta(self.ptr);
}

Vec2f engine_Backend_getMouseWheelScrollDelta(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getMouseWheelScrollDelta(self.ptr);
}

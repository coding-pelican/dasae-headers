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

bool engine_Backend_pressedKey(const engine_Backend self, engine_KeyCode code) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->pressedKey(self.ptr, code);
}

bool engine_Backend_heldKey(const engine_Backend self, engine_KeyCode code) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->heldKey(self.ptr, code);
}

bool engine_Backend_releasedKey(const engine_Backend self, engine_KeyCode code) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->releasedKey(self.ptr, code);
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

Vec2i engine_Backend_getMouseDelta(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getMouseDelta(self.ptr);
}

Vec2f engine_Backend_getMouseScrollDelta(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getMouseScrollDelta(self.ptr);
}

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

Vec2u engine_Backend_getWindowSize(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowSize(self.ptr);
}

Vec2u engine_Backend_getWindowMinSize(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowMinSize(self.ptr);
}

Vec2u engine_Backend_getWindowMaxSize(const engine_Backend self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->getWindowMaxSize(self.ptr);
}

Err$void engine_Backend_setWindowMinSize(engine_Backend self, Vec2u size) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->setWindowMinSize(self.ptr, size);
}

Err$void engine_Backend_setWindowMaxSize(engine_Backend self, Vec2u size) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt_internal);
    return self.vt_internal->setWindowMaxSize(self.ptr, size);
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
